// relu_template.h
// 2018/05/06 by marsee
// テンプレートを使用して汎用化した relu
//

#ifndef __RELU_TEMPLATE_H__
#define __RELU_TEMPLATE_H__

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_video.h>
#include <ap_fixed.h>

#include "layer_general.h"

template <const size_t W,
		  const size_t I,
		  const size_t NUMBER_OF_KERNEL,
		  const size_t VERTICAL_HIGHT,
		  const size_t HORIZONTAL_WIDTH>
int relu_template(hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >& ins,
				  hls::stream<ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> >& outs)
{
  typedef ap_fixed<W,I,AP_TRN,AP_WRAP> conv_type;

  ap_fixed_axis<W,I,NUMBER_OF_KERNEL,1> pix;
  
  do {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=1 max=1 avg=1
    // user が 1になった時にフレームがスタートする
    ins >> pix;
  } while(pix.user == 0);

 Loop1: for (int y=0; y<VERTICAL_HIGHT; y++){
  Loop2: for (int x=0; x<HORIZONTAL_WIDTH; x++){
#pragma HLS PIPELINE II=1
      if (!(x==0 && y==0))    // 最初の入力はすでに入力されている
        ins >> pix;    // AXI4-Stream からの入力

      for(int i=0; i<NUMBER_OF_KERNEL; i++){              
        if (pix.data[i] < conv_type(0.0)) // データが 0 以下だったら 0 にする
          pix.data[i] = conv_type(0.0);
      }

      outs << pix;
    }
  }

  return(0);
}
#endif



