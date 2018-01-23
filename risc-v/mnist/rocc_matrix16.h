// See LICENSE for license details.

#ifndef SRC_ROCC_MATRIX16_H
#define SRC_ROCC_MATRIX16_H

#define XCUSTOM_DOT 0

#include "rocc.h"

#define rocc_dot(y, mat_A, mat_B, step, len)            \
  ROCC_INSTRUCTION(XCUSTOM_DOT, y, len,   0,     0);    \
  ROCC_INSTRUCTION(XCUSTOM_DOT, y, step,  0,     1);    \
  y = 0;                                                \
  ROCC_INSTRUCTION(XCUSTOM_DOT, y, mat_A, mat_B, 2);

#define ROCC_READ_LOG(y, addr)                      \
  ROCC_INSTRUCTION(XCUSTOM_DOT, y, addr,  0,     3)

uint32_t dot_read_log(uint32_t addr)
{
  uint32_t ret;
  ROCC_READ_LOG (ret, addr);
  return ret;
}


#endif // SORC_ROCC_MATRIX16_H
