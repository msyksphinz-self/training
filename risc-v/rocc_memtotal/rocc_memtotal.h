// See LICENSE for license details.

#ifndef SRC_ROCC_MEMTOTAL_H
#define SRC_ROCC_MEMTOTAL_H

#define XCUSTOM_MEMTOTAL 0

#define memtotal(y, mem_addr, len)                          \
  ROCC_INSTRUCTION(XCUSTOM_MEMTOTAL, y, mem_addr, len, 0);

#endif // SORC_ROCC_MEMTOTAL_H
