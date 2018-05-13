// relu_conv1.cpp
// 2018/05/06 by marsee
// relu after conv1
//

#include "relu_template.h"

int relu_conv1 (hls::stream<ap_fixed_axis<16,6,2,1> >& ins,
				hls::stream<ap_fixed_axis<16,6,2,1> >& outs){
#pragma HLS DATA_PACK variable=outs
#pragma HLS DATA_PACK variable=ins
  return(relu_template<16,6,2,6,52>(ins, outs));
}
