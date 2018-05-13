// affine_layer2.h
// 2018/05/03 by marsee
//

#ifndef __AFFINE_LAYER2_H__
#define __AFFINE_LAYER2_H__
#include <ap_fixed.h>

static const size_t NUMBER_OF_MIDDLE_LAYER = 100;
static const size_t NUMBER_OF_OUTPUT_LAYER = 3;

static const size_t IN_W = 19;
static const size_t IN_I = 7;
static const size_t OUT_W = 12;
static const size_t OUT_I = 7;

typedef struct {
  ap_fixed<12,7,AP_TRN,AP_WRAP> data [NUMBER_OF_OUTPUT_LAYER];
} mdata_type;

typedef struct {
  float data [NUMBER_OF_OUTPUT_LAYER];
} fmdata_type;

typedef ap_fixed<OUT_W,OUT_I,AP_TRN,AP_WRAP> out_type;

typedef ap_fixed<IN_W,IN_I,AP_TRN,AP_WRAP> affine_type;

#endif

