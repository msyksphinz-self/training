// relu_affine1.cpp
// 2018/05/06 by marsee
// relu after affine1
//

#include "relu_template.h"

int relu_affine1(hls::stream<ap_fixed_axis<19,7,1,1> >& ins,
				 hls::stream<ap_fixed_axis<19,7,1,1> >& outs){
#pragma HLS DATA_PACK variable=outs
#pragma HLS DATA_PACK variable=ins
  return(relu_template<19,7,1,1,100>(ins, outs));
}
