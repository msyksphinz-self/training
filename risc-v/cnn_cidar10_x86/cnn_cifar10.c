/*************************************************/
/* cnn_cifar10.c                                 */
/* cifar10 Convolutional Neural Network          */
/*************************************************/

#include <stdint.h>
#include <stdio.h>
#include "layers.h"

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (50)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (60000)  // Max number of learning data
#define BATCH_SIZE (2)
#define LEARNING_RATE (0.1)
#define WEIGHT_INIT (0.01)

#ifdef IMAGE_SIZE_4000
extern char _binary_t10k_images_idx3_ubyte_small_start[];
#else // IMAGE_SIZE_4000
extern char _binary_t10k_images_idx3_ubyte_start[];
#endif // IMAGE_SIZE_4000

void forward (double *o, double *e, int input_size, int output_size);

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const double *wh0,   // [input_size][hidden_size],
				  const double *wb0,   // [hidden_size],
				  const double *wh1,   // [hidden_size][output_size],
				  const double *wb1);   // [output_size]

extern char _binary_t10k_images_idx3_ubyte_end[];

extern char _binary_t10k_labels_idx1_ubyte_start[];
extern char _binary_t10k_labels_idx1_ubyte_end[];

extern char _binary_wb0_bin_start[];
extern char _binary_wb0_bin_end[];
extern char _binary_wb1_bin_start[];
extern char _binary_wb1_bin_end[];
extern char _binary_wh0_bin_start[];
extern char _binary_wh0_bin_end[];
extern char _binary_wh1_bin_start[];
extern char _binary_wh1_bin_end[];


int main ()
{
  const double *wh0 = (double *)_binary_wh0_bin_start;  // [INPUTNO * HIDDENNO];
  const double *wb0 = (double *)_binary_wb0_bin_start;  // [HIDDENNO];
  const double *wh1 = (double *)_binary_wh1_bin_start;  // [HIDDENNO * OUTPUTNO];
  const double *wb1 = (double *)_binary_wb1_bin_start;  // [OUTPUTNO];


  TestNetwork (INPUTNO, OUTPUTNO, HIDDENNO, wh0, wb0, wh1, wb1);

  return 0;
}

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const double *wh0,  // [input_size][hidden_size],
				  const double *wb0,  // [hidden_size],
				  const double *wh1,  // [hidden_size][output_size],
				  const double *wb1)  // [output_size]
{
  printf ("=== TestNetwork ===\n");

  char *in_data;
  char *ans_data;

#ifdef IMAGE_SIZE_4000
  in_data  = &_binary_t10k_images_idx3_ubyte_small_start[0x10];
#else // IMAGE_SIZE_4000
  in_data  = &_binary_t10k_images_idx3_ubyte_start[0x10];
#endif // IMAGE_SIZE_4000
  ans_data = &_binary_t10k_labels_idx1_ubyte_start[0x08];

  int correct = 0;
  uint64_t start_cycle, stop_cycle, total_cycle = 0;

  printf (" === start ===\n");

  double af0 [BATCH_SIZE * HIDDENNO];
  double fix16_in_data[BATCH_SIZE*INPUTNO];

  double af1 [BATCH_SIZE * OUTPUTNO];
  double rel0[BATCH_SIZE * HIDDENNO];

#ifdef IMAGE_SIZE_4000
  int max_try = BATCH_SIZE * 3;
#else // IMAGE_SIZE_4000
  int max_try = BATCH_SIZE * 100;
#endif // IMAGE_SIZE_4000

  for (int no_input = 0; no_input < max_try; no_input += BATCH_SIZE) {
	for (int i = 0; i < 28 * 28 * BATCH_SIZE; i++) {
	  fix16_in_data[i] = (in_data[i] << 8);
	}

    // rdmcycle(start_cycle);

	affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, (const double *)fix16_in_data, wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);
	affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);

    // rdmcycle(stop_cycle);

    total_cycle += (stop_cycle - start_cycle);

	for (int b = 0; b < BATCH_SIZE; b++) {
	  int t = argmax (OUTPUTNO, &af1[b * OUTPUTNO]);
      printf ("%3d : expect=%d, actual= %d", no_input + b, t, ans_data[b]);
	  if (t == ans_data[b]) {
        printf (" CORRECT\n");
        correct++;
      } else {
        printf (" FAIL\n");
      }
	}

	in_data += (INPUTNO * BATCH_SIZE);
	ans_data += BATCH_SIZE;
  }

  printf ("Final Result : Correct = %d / %d\n", correct, max_try);
  printf ("Time = %d\n", total_cycle);

  return;
}




