// conv_layer_template.h
// 2018/05/01 : by marsee
// テンプレートを使用して汎用化した conv_layer
//

#ifndef __CONV_LAYER_TEMPLATE_H___
#define __CONV_LAYER_TEMPLATE_H___

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_video.h>
#include <ap_fixed.h>

#include "layer_general.h"

#define TO_LITERAL(x) #x
#define PRAGMA_HLS(tok) _Pragma(TO_LITERAL(HLS tok)) // @hiyuhさんから

template<const size_t IN_W,     // 入力のビット幅
		 const size_t IN_I,     // 入力の小数点位置
		 const size_t MID_W, // 中間の演算ビット長
		 const size_t MID_I, // 中間の値の小数点位置
		 const size_t OUT_W, // 出力のビット長
		 const size_t OUT_I, // 出力の小数点位置
		 const size_t WB_W,     // 重みとバイアスのビット長
		 const size_t WB_I,     // 重みとバイアスの小数点位置
		 const size_t NUMBER_OF_IN_CHANNELS,
		 const size_t NUMBER_OF_OUT_CHANNELS,
		 const size_t H_KERNEL_SIZE,
		 const size_t V_KERNEL_SIZE,
		 const size_t PADDING,
		 const size_t PADDING_DATA,
		 const size_t STRIDE,
		 const size_t HORIZONTAL_PIXEL_WIDTH,
		 const size_t VERTICAL_PIXEL_WIDTH>
int conv_layer_template(hls::stream<ap_fixed_axis<IN_W,IN_I,NUMBER_OF_IN_CHANNELS,1> >&ins,
						hls::stream<ap_fixed_axis<OUT_W,OUT_I,NUMBER_OF_OUT_CHANNELS,1> >&outs,
						const ap_fixed<WB_W,WB_I,AP_TRN,AP_WRAP> conv_weight[NUMBER_OF_OUT_CHANNELS][NUMBER_OF_IN_CHANNELS][V_KERNEL_SIZE][H_KERNEL_SIZE],
						const ap_fixed<WB_W,WB_I,AP_TRN,AP_WRAP> conv_bias[NUMBER_OF_OUT_CHANNELS]
						){
  typedef ap_fixed<IN_W,IN_I,AP_TRN,AP_WRAP> in_type;
  typedef ap_fixed<MID_W,MID_I,AP_TRN,AP_WRAP> val_type;

  ap_fixed_axis<IN_W,IN_I,NUMBER_OF_IN_CHANNELS,1> pix;
  ap_fixed_axis<OUT_W,OUT_I,NUMBER_OF_OUT_CHANNELS,1> conv_out;

  in_type line_buf[V_KERNEL_SIZE-1][HORIZONTAL_PIXEL_WIDTH+2*PADDING][NUMBER_OF_IN_CHANNELS];

  const size_t AP_FACTOR = V_KERNEL_SIZE-1;
  PRAGMA_HLS(array_partition variable=line_buf block factor=AP_FACTOR dim=1)

    in_type pix_mat[V_KERNEL_SIZE][H_KERNEL_SIZE][NUMBER_OF_IN_CHANNELS];

  val_type val;

  do {
    // user が 1になった時にフレームがスタートする
	ins >> pix;
  } while(pix.user == 0);

  for (int y=0; y<(VERTICAL_PIXEL_WIDTH+2*PADDING); y++){
	for (int x=0; x<(HORIZONTAL_PIXEL_WIDTH+2*PADDING); x++){
	  if (y<PADDING || y>=(VERTICAL_PIXEL_WIDTH+PADDING) ||
		  x<PADDING || x>=(HORIZONTAL_PIXEL_WIDTH+PADDING)){ // PADDING
		for (int n=0; n<NUMBER_OF_OUT_CHANNELS; n++){
		  pix.data[n] = PADDING_DATA;
		}
	  } else if (!(x==PADDING && y==PADDING)){    // 最初の入力はすでに入力されている
		ins >> pix;
	  }

	  // 2次元配列のデータを左シフト
	  for (int k=0; k<V_KERNEL_SIZE; k++){
		for (int m=0; m<H_KERNEL_SIZE-1; m++){
		  for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
			pix_mat[k][m][n] = pix_mat[k][m+1][n];
		  }
		}
	  }
   
	  for (int i=0; i<V_KERNEL_SIZE-1; i++){ // 以前の行のデータを line_buf から入力
		for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
		  pix_mat[i][H_KERNEL_SIZE-1][n] = line_buf[i][x][n];
		}
	  }
	  for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
		pix_mat[V_KERNEL_SIZE-1][H_KERNEL_SIZE-1][n] = pix.data[n]; // pix_mat の最後に新しいデータを入力
	  }

	Loop_lineb_exchg0 : for (int i=0; i<V_KERNEL_SIZE-2; i++){ // 行の入れ替え
	  Loop_lineb_exchg1 : for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
		  line_buf[i][x][n] = line_buf[i+1][x][n];
		}
	  }
	Loop_lineb_exchg2 : for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
		line_buf[V_KERNEL_SIZE-2][x][n] = pix.data[n];
	  }

	  // conv_layer の演算
	Conv_calc0 : for (int k=0; k<NUMBER_OF_OUT_CHANNELS; k++){
		val = 0.0;
	  Conv_calc1 : for (int j=0; j<V_KERNEL_SIZE; j++){
		Conv_calc2 : for (int i=0; i<H_KERNEL_SIZE; i++){
		  Conv_calc3 : for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
			  val += (val_type)pix_mat[j][i][n] * (val_type)conv_weight[k][n][j][i];
			}
		  }
		}
		if (NUMBER_OF_IN_CHANNELS == 1){ // 現在はこっち
		  val += (val_type)conv_bias[k];
		} else {
		Bias_add : for (int n=0; n<NUMBER_OF_IN_CHANNELS; n++){
			val += (val_type)conv_bias[n][k];
		  }
		}
		conv_out.data[k] = val;
	  }

	  // 最初の　V_KERNEL_SIZE-1行とその他の行の最初の　H_KERNEL_SIZE-1 列は無効データなので出力しない
	  if (x<(H_KERNEL_SIZE-1) || y<(V_KERNEL_SIZE-1))
		continue;
	  else { // 有効なデータの時
		if (x==(H_KERNEL_SIZE-1) && y==(V_KERNEL_SIZE-1)){ // 最初のデータでは、TUSERをアサートする
		  conv_out.user = 1;
		} else {
		  conv_out.user = 0;
		}

		if (x > (HORIZONTAL_PIXEL_WIDTH+2*PADDING-1-STRIDE)){ // 行の最後で TLAST をアサートする
		  conv_out.last = 1;
		} else {
		  conv_out.last = 0;
		}

		if ((x%STRIDE==STRIDE-1) && (y%STRIDE==STRIDE-1)){
		  outs << conv_out;
		}
	  }
	}
  }
  return(0);
}

#endif


