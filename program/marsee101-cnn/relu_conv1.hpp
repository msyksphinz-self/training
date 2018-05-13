// relu_conv1.h
// 2018/02/20 by marsee (HLS stream)
//

#ifndef __RELU_H__
#define __RELU_H__

static const size_t HORIZONTAL_PIXEL_WIDTH = 52;
static const size_t VERTICAL_PIXEL_WIDTH = 6;
static const size_t ALL_PIXELS = HORIZONTAL_PIXEL_WIDTH * VERTICAL_PIXEL_WIDTH;

static const size_t NUMBER_OF_KERNEL = 2;
static const size_t ARRAY_SIZE = 2;
static const size_t W = 16;
static const size_t I = 6;

typedef ap_fixed<W, I, AP_TRN, AP_WRAP> conv_type;

#endif

