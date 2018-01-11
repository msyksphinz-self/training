/*************************************************/
/* bp1.c                                         */
/* Neural network learning with back propagation */
/*************************************************/

#define IMAGE_FILE "train-images-idx3-ubyte"
#define LABEL_FILE "train-labels-idx1-ubyte"

#include "libfixmath/libfixmath/fix16.h"

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (50)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (60000)  // Max number of learning data
#define BATCH_SIZE (2)
#define LEARNING_RATE (0.1)
#define WEIGHT_INIT (0.01)

#define rdmcycle(hi_cycle, lo_cycle)  {							   \
    uint32_t lo, hi, hi2;                              \
    __asm__ __volatile__ ("1:\n\t"                     \
                          "csrr %0, mcycleh\n\t"       \
                          "csrr %1, mcycle\n\t"        \
                          "csrr %2, mcycleh\n\t"       \
                          "bne  %0, %2, 1b\n\t"                 \
                          : "=r" (hi), "=r" (lo), "=r" (hi2)) ; \
	hi_cycle = hi; lo_cycle = lo;								\
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

// double rand_normal ( double mu, double sigma );
// double drnd ();

extern char _binary_train_images_idx3_ubyte_100_start[];
extern char _binary_train_images_idx3_ubyte_100_end[];

extern char _binary_t10k_images_idx3_ubyte_start[];
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


const char* hex_enum[] = {"0", "1", "2", "3", "4", "5", "6", "7",
                          "8", "9", "a", "b", "c", "d", "e", "f"};

// fix16_t wh0[INPUTNO * HIDDENNO];
fix16_t wb0[HIDDENNO];
fix16_t wh1[HIDDENNO * OUTPUTNO];
fix16_t wb1[OUTPUTNO];

const fix16_t *wh0   = (fix16_t *)_binary_wh0_bin_start;  // [INPUTNO * HIDDENNO];
const fix16_t *c_wb0 = (fix16_t *)_binary_wb0_bin_start;  // [HIDDENNO];
const fix16_t *c_wh1 = (fix16_t *)_binary_wh1_bin_start;  // [HIDDENNO * OUTPUTNO];
const fix16_t *c_wb1 = (fix16_t *)_binary_wb1_bin_start;  // [OUTPUTNO];

int main ()
{
  int i;

  // for (i = 0; i < INPUTNO * HIDDENNO; i++)  wh0[i] = c_wh0[i];
  for (i = 0; i < HIDDENNO; i++)            wb0[i] = c_wb0[i];
  for (i = 0; i < HIDDENNO * OUTPUTNO; i++) wh1[i] = c_wh1[i];
  for (i = 0; i < OUTPUTNO; i++)            wb1[i] = c_wb1[i];

  TestNetwork (INPUTNO, OUTPUTNO, HIDDENNO, wh0, wb0, wh1, wb1);

  return 0;
}

fix16_t af0 [BATCH_SIZE * HIDDENNO];
fix16_t fix16_in_data[BATCH_SIZE*INPUTNO];
char *in_data;
char *ans_data;
fix16_t af1 [BATCH_SIZE * OUTPUTNO];
fix16_t rel0[BATCH_SIZE * HIDDENNO];

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const fix16_t *wh0,  // [input_size][hidden_size],
				  const fix16_t *wb0,  // [hidden_size],
				  const fix16_t *wh1,  // [hidden_size][output_size],
				  const fix16_t *wb1)  // [output_size]
{
  printf ("=== TestNetwork ===\n");

  in_data  = &_binary_t10k_images_idx3_ubyte_start[0x10];
  ans_data = &_binary_t10k_labels_idx1_ubyte_start[0x08];

  int correct = 0;
  uint32_t start_cycle[2], stop_cycle[2];
  // rdmcycle(start_cycle[1], start_cycle[0]);

  printf (" === start ===\n");

  for (int no_input = 0; no_input < 100 * BATCH_SIZE; no_input += BATCH_SIZE) {
	for (int i = 0; i < 28 * 28 * BATCH_SIZE; i++) {
	  fix16_in_data[i] = (in_data[i] << 8);
	}

	affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, (const fix16_t *)fix16_in_data, wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);
	affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);

	for (int b = 0; b < BATCH_SIZE; b++) {
	  int t = argmax (OUTPUTNO, &af1[b * OUTPUTNO]);
	  if (t == ans_data[b]) {
        correct++;
      }
	}

	in_data += (INPUTNO * BATCH_SIZE);
	ans_data += BATCH_SIZE;
  }

  // rdmcycle(stop_cycle[1], stop_cycle[0]);

  printf ("Correct = %d\n", correct);
  printf ("Time = %08x%08x - %08x%08x\n", stop_cycle[1], stop_cycle[0], start_cycle[1], start_cycle[0]);

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
  	  out[b * output_size + o] = 0;
  	  for (int i = 0; i < input_size; i++) {
  	  	out[b * output_size + o] = fix16_add (out[b * output_size + o],
                                              fix16_mul (in_data[b * input_size + i], wh[i * output_size + o]));
  	  }
  	  out[b * output_size + o] = fix16_add (out[b * output_size + o], wb[o]);
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


// double rand_normal (double mu, double sigma)
// {
//   // double z = sqrt( -2.0 * log(drnd()) ) * sin( 2.0 * M_PI * drnd() );
//   // return mu + sigma*z;
//   return drnd ();
// }
//
//
// double drnd ()
// {
//   double rndno;
//   while ((rndno = (double)rand() / RAND_MAX) == 1.0);
//   return rndno;
// }
