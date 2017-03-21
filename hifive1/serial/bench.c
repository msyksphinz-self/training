#include "Arduino.h"
#include "encoding.h"
#include "fe300prci/fe300prci_driver.h"
#include "./printf.h"
#include "./serial.h"


#define rdmcycle(x)  {				       \
    uint32_t lo, hi, hi2;			       \
    __asm__ __volatile__ ("1:\n\t"		       \
			  "csrr %0, mcycleh\n\t"       \
			  "csrr %1, mcycle\n\t"	       \
			  "csrr %2, mcycleh\n\t"       \
			  "bne  %0, %2, 1b\n\t"			\
			  : "=r" (hi), "=r" (lo), "=r" (hi2)) ;	\
    *(x) = lo | ((uint64_t) hi << 32); 				\
  }

inline void add_inst(int *dst, int src0, int src1) {
  asm volatile ("add %0,%1,%2" :"=r"(*dst) :"r"(src0),"r"(src1) : );
}
inline void sub_inst(int *dst, int src0, int src1) {
  asm volatile ("sub %0,%1,%2" :"=r"(*dst) :"r"(src0),"r"(src1) : );
}
inline void mult_inst(int *dst, int src0, int src1) {
  asm volatile ("mul %0,%1,%2"  :"=r"(*dst) :"r"(src0),"r"(src1) : );
}
inline void div_inst(int *dst, int src0, int src1) {
  asm volatile ("div %0,%1,%2"  :"=r"(*dst) :"r"(src0),"r"(src1) : );
}


uint32_t inst_latency(void (*func)(int*,int,int), int init_src0, int init_src1)
{
  uint32_t start_time, end_time, wrap_time;
  int dst, src0 = init_src0, src1 = init_src1;
  rdmcycle (&start_time);
  for(int i=0; i < 65565; i++) {
    func (&dst, src0, src1);
    func (&dst, dst,  src1);
    func (&dst, dst,  src1);
    func (&dst, dst,  src1);
    func (&dst, dst,  src1);
    func (&dst, dst,  src1);
    func (&dst, dst,  src1);
    func (&dst, dst,  src1);
  }
  rdmcycle (&end_time);
  // printint(0, start_time, 10, 1); uart_print("\r\n");
  // printint(0, end_time, 10, 1);   uart_print("\r\n");
  // printint(0, wrap_time, 10, 1);  uart_print("\r\n");
  return end_time - start_time;
}


uint32_t inst_throughput(void (*func)(int*,int,int), int init_src0, int init_src1)
{
  uint32_t start_time, end_time, wrap_time;
  int dst, src0 = init_src0, src1 = init_src1;

  rdmcycle (&start_time);
  for(int i=0; i < 65565; i++) {
    func (&dst, src0, src1);
    func (&dst, src0, src1);
    func (&dst, src0, src1);
    func (&dst, src0, src1);
    func (&dst, src0, src1);
    func (&dst, src0, src1);
    func (&dst, src0, src1);
    func (&dst, src0, src1);
  }
  rdmcycle (&end_time);
  // printint(0, start_time, 10, 1); uart_print("\r\n");
  // printint(0, end_time, 10, 1);   uart_print("\r\n");
  // printint(0, wrap_time, 10, 1);  uart_print("\r\n");
  return end_time - start_time;
}


void add_bench ()
{
  uint32_t ret;
  ret = inst_latency    (add_inst, 1, 1); printint(0, ret, 10, 1); uart_print("\r\n");
  ret = inst_throughput (add_inst, 1, 1); printint(0, ret, 10, 1); uart_print("\r\n");

  ret = inst_latency    (sub_inst, 1, 1); printint(0, ret, 10, 1); uart_print("\r\n");
  ret = inst_throughput (sub_inst, 1, 1); printint(0, ret, 10, 1); uart_print("\r\n");

  ret = inst_latency    (mult_inst, 1, 1); printint(0, ret, 10, 1); uart_print("\r\n");
  ret = inst_throughput (mult_inst, 1, 1); printint(0, ret, 10, 1); uart_print("\r\n");

  ret = inst_latency    (div_inst, 0x7fffffff, 1); printint(0, ret, 10, 1); uart_print("\r\n");
  ret = inst_throughput (div_inst, 0x7fffffff, 1); printint(0, ret, 10, 1); uart_print("\r\n");
}
