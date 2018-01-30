/*************************************************/
/* bp1.c                                         */
/* Neural network learning with back propagation */
/*************************************************/

#include "libfixmath/libfixmath/fix16.h"

#include "rocc_matrix16.h"

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (50)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (60000)  // Max number of learning data
#define BATCH_SIZE (2)
#define LEARNING_RATE (0.1)
#define WEIGHT_INIT (0.01)

//#define rdmcycle(hi_cycle, lo_cycle)  {							   \
//    uint32_t lo, hi, hi2;                              \
//    __asm__ __volatile__ ("1:\n\t"                     \
//                          "csrr %0, mcycleh\n\t"       \
//                          "csrr %1, mcycle\n\t"        \
//                          "csrr %2, mcycleh\n\t"       \
//                          "bne  %0, %2, 1b\n\t"                 \
//                          : "=r" (hi), "=r" (lo), "=r" (hi2)) ; \
//	hi_cycle = hi; lo_cycle = lo;								\
//  }

#define rdmcycle(cycle)  {                                      \
    __asm__ __volatile__ ("csrr %0, mcycle"                 \
                          : "=r" (cycle)) ;                     \
  }


fix16_t affine (const int output_size,
                const int input_size,
                const int batch_size,
                fix16_t *out,           // [batch_size][output_size],
                const fix16_t *in_data, // [batch_size][input_size],
                const fix16_t *wh,      // [input_size][output_size],
                const fix16_t *wb);      // [output_size]
fix16_t affine_backward (const int output_size,
                         const int hidden_size,
                         const int batch_size,
                         fix16_t *dx,  // [batch_size][output_size],
                         fix16_t *db,  // [output_size],
                         fix16_t *dw,  // [output_size][hidden_size],
                         const fix16_t *dout, // [batch_size][output_size],
                         const fix16_t *w,    // [hidden_size][output_size],
                         const fix16_t *x);   // [batch_size][hidden_size],

void relu (const int batch_size,
		   const int size,
		   fix16_t *o,         // [batch_size][size],
		   const fix16_t *e);  // [batch_size][size]
fix16_t relu_backward (const int batch_size,
                       const int size,
                       fix16_t       *dx,  // [batch_size][size],
                       const fix16_t *x,   // [batch_size][size],
                       const fix16_t *dout); // [batch_size][size]);

fix16_t softmax (const int batch_size,
                 const int size,
                 fix16_t       *o,  // [batch_size][size],
                 const fix16_t *e); // [batch_size][size]
fix16_t softmax_backward (const int batch_size,
                          const int size,
                          fix16_t       *dx,   // [batch_size][size],
                          const fix16_t *y,    // [batch_size][size],
                          const fix16_t *t);   // [batch_size][size])

void forward (fix16_t *o, fix16_t *e, int input_size, int output_size);

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const fix16_t *wh0,   // [input_size][hidden_size],
				  const fix16_t *wb0,   // [hidden_size],
				  const fix16_t *wh1,   // [hidden_size][output_size],
				  const fix16_t *wb1);   // [output_size]

int argmax (const int x_size, fix16_t *o);

#ifdef IMAGE_SIZE_4000
extern char _binary_t10k_images_idx3_ubyte_small_start[];
#else // IMAGE_SIZE_4000
extern char _binary_t10k_images_idx3_ubyte_start[];
#endif // IMAGE_SIZE_4000

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
  const fix16_t *wh0 = (fix16_t *)_binary_wh0_bin_start;  // [INPUTNO * HIDDENNO];
  const fix16_t *wb0 = (fix16_t *)_binary_wb0_bin_start;  // [HIDDENNO];
  const fix16_t *wh1 = (fix16_t *)_binary_wh1_bin_start;  // [HIDDENNO * OUTPUTNO];
  const fix16_t *wb1 = (fix16_t *)_binary_wb1_bin_start;  // [OUTPUTNO];


  TestNetwork (INPUTNO, OUTPUTNO, HIDDENNO, wh0, wb0, wh1, wb1);

  return 0;
}

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const fix16_t *wh0,  // [input_size][hidden_size],
				  const fix16_t *wb0,  // [hidden_size],
				  const fix16_t *wh1,  // [hidden_size][output_size],
				  const fix16_t *wb1)  // [output_size]
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

  fix16_t af0 [BATCH_SIZE * HIDDENNO];
  fix16_t fix16_in_data[BATCH_SIZE*INPUTNO];

  fix16_t af1 [BATCH_SIZE * OUTPUTNO];
  fix16_t rel0[BATCH_SIZE * HIDDENNO];

#ifdef IMAGE_SIZE_4000
  int max_try = BATCH_SIZE * 3;
#else // IMAGE_SIZE_4000
  int max_try = BATCH_SIZE * 100;
#endif // IMAGE_SIZE_4000

  for (int no_input = 0; no_input < max_try; no_input += BATCH_SIZE) {
	for (int i = 0; i < 28 * 28 * BATCH_SIZE; i++) {
	  fix16_in_data[i] = (in_data[i] << 8);
	}

    rdmcycle(start_cycle);

	affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, (const fix16_t *)fix16_in_data, wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);
	affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);

    rdmcycle(stop_cycle);

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



fix16_t affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   fix16_t *out,            // [batch_size][output_size],
			   const fix16_t *in_data,  // [batch_size][input_size],
			   const fix16_t *wh,       // [input_size][output_size],
			   const fix16_t *wb)       // [output_size]
{
  for (int b = 0; b < batch_size; b++) {
  	for (int o = 0; o < output_size; o++) {
#ifdef ROCC_MATRIX16
	  uint64_t start_cycle, stop_cycle;
	  rdmcycle (start_cycle);
      rocc_dot (out[b * output_size + o],
                &in_data[b * input_size],
                &wh[o],
                output_size,
                input_size);
	  rdmcycle (stop_cycle);
  	  out[b * output_size + o] = fix16_add (out[b * output_size + o], wb[o]);
	  printf ("rocc_dot(input=%03d) : %d\n", input_size, stop_cycle - start_cycle);
	  
// #ifdef DEBUG
//       for (int log_idx = 0; log_idx < 300; log_idx++) {
//         uint32_t data, input, weight;
//         ROCC_READ_LOG(data, log_idx);
//         ROCC_INPUT_LOG(input, log_idx);
//         ROCC_WEIGHT_LOG(weight, log_idx);
//         printf ("MEM[%03d]=%08x,INPUT=%08x,WEIGHT=%08x\n", log_idx, data, input, weight);
//       }
// #endif // DEBUG
#else // ROCC_MATRIX16
  	  out[b * output_size + o] = 0;
  	  for (int i = 0; i < input_size; i++) {
		fix16_t tmp = fix16_mul (in_data[b * input_size + i], wh[i * output_size + o]);
  	  	out[b * output_size + o] = fix16_add (out[b * output_size + o], tmp);
// #ifdef DEBUG
// 		if (i < 300) {
// 		  printf("MEM[%03d]=%08x,INPUT=%08x,WEIGHT=%08x\n", i, tmp, in_data[b * input_size + i], wh[i * output_size + o]);
// 		}
// #endif // DEBUG
  	  }
#ifdef DEBUG
	  printf("out = %08x\n", out[b * output_size + o]);
#endif // DEBUG
  	  out[b * output_size + o] = fix16_add (out[b * output_size + o], wb[o]);
#endif // ROCC_MATRIX16
  	}
  }
}


fix16_t affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						fix16_t *dx,  // [batch_size][hidden_size],
						fix16_t *db,  // [output_size],
						fix16_t *dw,  // [hidden_size][output_size],
						const fix16_t *dout,  // [batch_size][output_size],
						const fix16_t *w,     // [hidden_size][output_size],
						const fix16_t *x)     // [batch_size][hidden_size],
{
  for (int b = 0; b < batch_size; b++) {
	for (int h = 0; h < hidden_size; h++) {
	  dx[b * hidden_size + h] = 0;
	  for (int o = 0;o < output_size; o++) {
		dx[b * hidden_size + h] = fix16_add (dx[b * hidden_size + h],
                                             fix16_mul (dout[b * output_size + o], w[h * output_size + o]));
	  }
	}
  }
  for (int h = 0; h < hidden_size; h++) {
	for (int o = 0; o < output_size; o++) {
	  dw[h * output_size + o] = 0;
	  for (int b = 0; b < batch_size; b++) {
		dw[h * output_size + o] = fix16_add (dw[h * output_size + o],
                                             fix16_mul (x[b * hidden_size + h], dout[b * output_size + o]));
	  }
	}
  }

  for (int o = 0; o < output_size; o++) {
	db[o] = 0;
	for (int b = 0; b < batch_size; b++) {
	  db[o] = fix16_add (db[o], dout[b * output_size + o]);
	}
  }
}


void relu (const int batch_size,
		   const int size,
		   fix16_t *o,        // [batch_size][size],
		   const fix16_t *e)  // [batch_size][size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  o[b * size + i] = e[b * size + i] > 0 ? e[b * size + i] : 0;
	}
  }
  return;
}


fix16_t relu_backward (const int batch_size,
					  const int size,
					  fix16_t       *dx,     // [batch_size][size],
					  const fix16_t *x,      // [batch_size][size],
					  const fix16_t *dout)   // [batch_size][size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  dx[b * size + i] = x[b * size + i] > 0 ? dout[b * size + i] : 0;
	}
  }
}


fix16_t softmax (const int batch_size,
                 const int size,
                 fix16_t *o,       // [batch_size][size],
                 const fix16_t *e) // e[batch_size][size]
{
  fix16_t *max = (fix16_t *)malloc(sizeof(fix16_t) * batch_size);
  for (int b = 0; b < batch_size; b++) {
	max[b] = e[b * size + 0];
	for (int i = 1; i < size; i++) {
	  max[b] = max[b] < e[b * size + i] ? e[b * size + i] : max[b];
	}
  }

  for (int b = 0; b < batch_size; b++) {
	fix16_t exp_sum;
    exp_sum = 0;
	fix16_t *a = (fix16_t *)malloc(sizeof(fix16_t) * size);
	for (int i = 0; i < size; i++) {
      a[i] = fix16_sub (e[b * size + i], max[b]);
	  exp_sum = fix16_add (exp_sum, fix16_exp(a[i]));
	}
	for (int i = 0; i < size; i++) {
	  o[b * size + i] = fix16_div (fix16_exp(a[i]), exp_sum);
	}
    free (a);
  }


  free (max);
}


fix16_t softmax_backward (const int batch_size,
                          const int size,
                          fix16_t        *dx,  // [batch_size][size],
                          const fix16_t  *y,  // [batch_size][size],
                          const fix16_t  *t)  // [batch_size][size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int y_idx = 0; y_idx < size; y_idx++) {
	  dx[b * size + y_idx] = fix16_div (fix16_sub (y[b * size + y_idx], t[b * size + y_idx]),
                                        fix16_from_int (batch_size));
	}
  }
}


int argmax (const int x_size, fix16_t *o)
{
  fix16_t ret = o[0];
  int    max_idx = 0;
  for (int i = 1; i < x_size; i++) {
	if (o[i] > ret) {
	  ret = o[i];
	  max_idx = i;
	}
  }

  return max_idx;
}
