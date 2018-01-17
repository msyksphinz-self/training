#include <stdio.h>
#include <stdint.h>
#include "rocc_memtotal.h"
#include "rocc.h"

uint64_t mem_array[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11, 12,13,14,15};

int main()
{
  uint64_t mem_total_hw = 10;
  uint64_t mem_total_sw = 0;
  memtotal (mem_total_hw, mem_array, 10);
  for (int i = 0; i < 10; i++) {
    mem_total_sw += mem_array[i];
  }
  printf ("mem_total_hw = %d\n", mem_total_hw);
  printf ("mem_total_sw = %d\n", mem_total_sw);
}
