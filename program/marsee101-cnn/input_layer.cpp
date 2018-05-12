// input_layer.cpp
// 2018/05/08 by marsee
//

#include <ap_int.h>
#include <hls_stream.h>

#include "layer_general.h"

#include <ap_axi_sdata.h>

#include "input_layer.h"

int input_layer(hls::stream<ap_axiu<IN_W,1,1,1> >&ins,
				hls::stream<ap_fixed_axis<OUT_W,OUT_I,1,1> >&outs){

  ap_axiu<IN_W,1,1,1> pix;
  ap_fixed_axis<OUT_W,OUT_I,1,1> out;

  do {
    // user が 1になった時にフレームがスタートする
	ins >> pix;
  } while(pix.user == 0);

  for (int y=0; y<VERTICAL_PIXEL_HIGHT; y++){
	for (int x=0; x<HORIZONTAL_PIXEL_WIDTH; x++){
	  if (!(x==0 && y==0))    // 最初の入力はすでに入力されている
		ins >> pix;    // AXI4-Stream からの入力

	  out.data[0] = (ap_fixed<OUT_W,OUT_I,AP_TRN,AP_WRAP>)((ap_ufixed<16, 8, AP_TRN, AP_WRAP>)(pix.data & 0xff) / 256);
	  out.user = pix.user;
	  out.last = pix.last;

	  outs << out;
	}
  }
  return(0);
}
