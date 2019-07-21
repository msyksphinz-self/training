#include <stdio.h>

int v_test(int size, float *f0, float *f1, float *f2)
{
  float *v0_p, *v1_p, *v2_p;
  int vl_length = 32;

  for (int i = 0; i < size; i+=vl_length) {
    asm volatile ("vsetvli %0, %1, e32":"=r"(vl_length):"r"(size - i));

    asm volatile ("vlw.v  v0,(%0)"::"r"(f0));   // 32b signed
    asm volatile ("vlw.v  v1,(%0)"::"r"(f1));   // 32b signed

    asm volatile ("vfmul.vv v2, v1, v0");
    asm volatile ("vsw.v  v2,(%0)"::"r"(f2));   // 32b signed

    f0 += vl_length;
    f1 += vl_length;
    f2 += vl_length;
  }

}


int main ()
{
  const int arr_size = 120;

  float f0[arr_size];
  float f1[arr_size];
  float f2[arr_size];

  for (int i = 0; i < arr_size; i++) {
    f0[i] = (float)i;
    f1[i] = (float)(128-i);
    f2[i] = 0;
  }

  printf("calling v_test\n");

  v_test(arr_size, f0, f1, f2);
  for (int i = 0; i < arr_size; i++) {
    printf("%f, ", f2[i]);
    if ((i % 10) == 10-1) {
      printf("\n");
    }
  }

}
