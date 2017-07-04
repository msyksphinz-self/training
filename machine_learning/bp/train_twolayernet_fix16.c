/*************************************************/
/* bp1.c                                         */
/* Neural network learning with back propagation */
/*************************************************/

#define IMAGE_FILE "train-images-idx3-ubyte"
#define LABEL_FILE "train-labels-idx1-ubyte"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

typedef union {
  int32_t i;
  struct {
    unsigned int fraction: 16;
    unsigned int decimal : 15;
    unsigned int sign : 1;
  } st_fix16;
} fix16_t;

#define FX16_RAW(fx16) (fx16.i)
#define FX16_SIGN(fx16) (fx16.st_fix16.sign)
#define FX16_MULT(a,b) (a.i * b.i / 65536))

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (50)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (60000)  // Max number of learning data
#define BATCH_SIZE (100)
#define LEARNING_RATE (0.1)
#define WEIGHT_INIT (0.01)

#define DUMP_IDX  (1)

int open_image ();
int open_label ();

int getdata (int fd_image,
			 int fd_label,
			 fix16_t *in_data,
			 int     *ans);
void hlearn (fix16_t **wh, // weight of hidden layer
			 fix16_t *wo, // weight of output layer
			 int output_size, int input_size,
			 fix16_t *hi, // current hidden layer ansewr
			 fix16_t *e,  // input value
			 fix16_t *o);   // output value
void print_images (fix16_t data[INPUTNO], int label);
void olearn (fix16_t *wo,
			 int     element_size,
			 fix16_t *hi,
			 fix16_t *e,
			 fix16_t *ans,
			 fix16_t *o);

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
                         const fix16_t *x,    // [batch_size][hidden_size],
                         int debug);

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

void initwh(const int y_size, const int x_size, fix16_t *wh);
void initwb(const int x_size, fix16_t *wb);

double rand_normal ( double mu, double sigma );
double drnd ();

inline void dtofx16 (fix16_t *f16, double d);
inline double fx16tod (fix16_t *f16);


int main ()
{
  fix16_t wh0[INPUTNO * HIDDENNO];
  fix16_t wb0[HIDDENNO];

  fix16_t wh1[HIDDENNO * OUTPUTNO];
  fix16_t wb1[OUTPUTNO];

  fix16_t hi[HIDDENNO + 1];
  fix16_t *in_data = malloc (sizeof(fix16_t) * MAXINPUTNO * INPUTNO);
  int     ans_data[MAXINPUTNO];
  fix16_t o[OUTPUTNO];
  int n_of_e;

  srand (SEED);

  initwh (INPUTNO, HIDDENNO, wh0);
  initwb (HIDDENNO, wb0);
  initwh (HIDDENNO, OUTPUTNO, wh0);
  initwb (OUTPUTNO, wb1);

  for (int i = 0; i < INPUTNO; i++) {
    for (int h = 0; h < HIDDENNO; h++) {
      printf ("%-2.5f ", FX16_RAW (wh0[i * HIDDENNO + h]) / 65536.0);
    }
    printf ("\n");
  }

  n_of_e = MAXINPUTNO / BATCH_SIZE;
  int fd_image = open_image ();
  int fd_label = open_label ();

  const int epoch_size = 50;

  getdata (fd_image, fd_label, in_data, ans_data);

  for (int no_input = 0; no_input < n_of_e; no_input++) {

    fix16_t af0 [BATCH_SIZE * HIDDENNO];
    fix16_t af1 [BATCH_SIZE * OUTPUTNO];
    fix16_t rel0[BATCH_SIZE * HIDDENNO];
    fix16_t rel1[BATCH_SIZE * OUTPUTNO];

    // print_images (&in_data[INPUTNO * BATCH_SIZE * no_input], ans_data[BATCH_SIZE * no_input]);

    affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, &in_data[INPUTNO * BATCH_SIZE * no_input] , wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);

    affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);
	softmax (BATCH_SIZE, OUTPUTNO, rel1, af1);

	// Back ward
	fix16_t ans_label[BATCH_SIZE * OUTPUTNO] = {0};
	for (int b = 0; b < BATCH_SIZE; b++) {
      dtofx16(&ans_label[b * OUTPUTNO + ans_data[no_input * BATCH_SIZE + b]], 1.0);
	}
	fix16_t softmax_dx[BATCH_SIZE * OUTPUTNO];
	softmax_backward (BATCH_SIZE, OUTPUTNO, softmax_dx, rel1, ans_label);
	fix16_t affine1_dx[BATCH_SIZE * HIDDENNO];
	fix16_t affine1_dw[HIDDENNO * OUTPUTNO];
	fix16_t affine1_db[OUTPUTNO];

	affine_backward (OUTPUTNO, HIDDENNO, BATCH_SIZE,
					 affine1_dx, affine1_db, affine1_dw,
					 softmax_dx, wh1, rel0, no_input == DUMP_IDX);

	fix16_t relu_dx[BATCH_SIZE * HIDDENNO];
	relu_backward (BATCH_SIZE, HIDDENNO, relu_dx, af0, affine1_dx);
	fix16_t affine0_dx[BATCH_SIZE * INPUTNO];
	fix16_t affine0_dw[INPUTNO * HIDDENNO];
	fix16_t affine0_db[HIDDENNO];
	affine_backward (HIDDENNO, INPUTNO, BATCH_SIZE,
					 affine0_dx, affine0_db, affine0_dw,
					 relu_dx, wh0, &in_data[INPUTNO * BATCH_SIZE * no_input], 0);

	for (int i = 0; i < INPUTNO; i++) {
	  for (int h = 0; h < HIDDENNO; h++) {
		FX16_RAW (wh0[i * HIDDENNO + h]) -= (LEARNING_RATE * FX16_RAW (affine0_dw[i * HIDDENNO + h]));
	  }
	}

	for (int h = 0; h < HIDDENNO; h++) {
	  FX16_RAW (wb0[h]) -= (LEARNING_RATE * FX16_RAW (affine0_db[h]));
	}

	for (int h = 0; h < HIDDENNO; h++) {
	  for (int o = 0; o < OUTPUTNO; o++) {
		FX16_RAW (wh1[h * OUTPUTNO + o]) -= (LEARNING_RATE * FX16_RAW (affine1_dw[h * OUTPUTNO + o]));
	  }
	}
	for (int o = 0; o < OUTPUTNO; o++) {
	  FX16_RAW (wb1[o]) -= (LEARNING_RATE * FX16_RAW (affine1_db[o]));
	}

    if (no_input == 0) {
      for (int i = 0; i < INPUTNO; i++) {
        for (int h = 0; h < HIDDENNO; h++) {
          printf ("%-2.5f ", FX16_RAW (wh0[i * HIDDENNO + h]) / 65536.0);
        }
        printf ("\n");
      }
      for (int h = 0; h < HIDDENNO; h++) {
        printf ("%-2.5f\n", FX16_RAW (wb0[h]) / 65536.0);
      }
      for (int h = 0; h < HIDDENNO; h++) {
        for (int o = 0; o < OUTPUTNO; o++) {
          printf ("%-2.5f ", FX16_RAW (wh1[h * OUTPUTNO + o]) / 65536.0);
        }
        printf ("\n");
      }
      for (int o = 0; o < OUTPUTNO; o++) {
        printf ("%-2.5f\n", FX16_RAW (wb1[o]) / 65536.0);
      }
    }

    if ((no_input % epoch_size) == 0) {
      TestNetwork (INPUTNO, OUTPUTNO, HIDDENNO, wh0, wb0, wh1, wb1);
	}
  }

  close(fd_image);
  close(fd_label);

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
  int image_fd;
  if ((image_fd = open("t10k-images-idx3-ubyte", O_RDONLY))==-1){
	printf("couldn't open image file t10k-images-idx3-ubyte");
	exit(0);
  }
  unsigned char *ptr;
  static int num[10];
  if (read(image_fd, num, 4 * sizeof(int)) == -1) { perror("read"); exit (EXIT_FAILURE); }

  int label_fd;
  if ((label_fd = open("t10k-labels-idx1-ubyte", O_RDONLY)) == -1){
	printf("couldn't open image file t10k-labels-idx1-ubyte");
	exit(0);
  }
  if (read(label_fd, num, 2 * sizeof(int)) == -1) { perror("read"); exit (EXIT_FAILURE); }

  fix16_t in_data[BATCH_SIZE][INPUTNO];
  int ans_data[BATCH_SIZE];

  int correct = 0;

  for (int no_input = 0; no_input < 10000; no_input += BATCH_SIZE) {
	for (int b = 0; b < BATCH_SIZE; b++) {
	  uint8_t image[INPUTNO];
	  if (read (image_fd, image, INPUTNO * sizeof(unsigned char)) == -1)  { perror("read"); exit (EXIT_FAILURE); }
	  for (int i = 0; i < INPUTNO; i++) {
		FX16_RAW (in_data[b][i]) = image[i] / 255.0;
	  }
	  uint8_t label;
	  if (read (label_fd, &label, sizeof(uint8_t)) == -1) { perror("read"); exit (EXIT_FAILURE); }
	  ans_data[b] = label;
	}

	fix16_t af0 [BATCH_SIZE * HIDDENNO];
	fix16_t af1 [BATCH_SIZE * OUTPUTNO];
	fix16_t rel0[BATCH_SIZE * HIDDENNO];
	fix16_t rel1[BATCH_SIZE * OUTPUTNO];

	affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, (const fix16_t *)in_data, wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);
	affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);

	for (int b = 0; b < BATCH_SIZE; b++) {
	  int t = argmax (OUTPUTNO, &af1[b * OUTPUTNO]);
	  if (t == ans_data[b]) correct++;
	}
  }
  printf ("Correct = %d\n", correct);

  close (image_fd);
  close (label_fd);

  return;
}



/* http://www.kk.iij4u.or.jp/~kondo/wave/swab.html */
void FlipLong(unsigned char * ptr) {
    register unsigned char val;

    /* Swap 1st and 4th bytes */
    val = *(ptr);
    *(ptr) = *(ptr+3);
    *(ptr+3) = val;

    /* Swap 2nd and 3rd bytes */
    ptr += 1;
    val = *(ptr);
    *(ptr) = *(ptr+1);
    *(ptr+1) = val;
}

int open_image ()
{
  int fd;
  if ((fd = open(IMAGE_FILE,O_RDONLY))==-1){
	printf("couldn't open image file");
	exit(0);
  }

  unsigned char *ptr;
  static int num[10];
  if (read(fd, num, 4 * sizeof(int)) == -1) { perror("read"); exit (EXIT_FAILURE); }

  for (int i = 0; i < 4; i++) {
	ptr = (unsigned char *)(num + i);
	FlipLong( ptr);
	printf("%d\n", num[i]);
	ptr = ptr + sizeof(int);
  }

  return fd;
}


int open_label ()
{
  int fd;
  if ((fd = open(LABEL_FILE,O_RDONLY))==-1){
	printf("couldn't open image file");
	exit(0);
  }

  unsigned char *ptr;
  static int num[10];
  if (read(fd, num, 2 * sizeof(int)) == -1) { perror("read"); exit (EXIT_FAILURE); }

  for (int i = 0; i < 2; i++) {
	ptr = (unsigned char *)(num + i);
	FlipLong( ptr);
	printf("%d\n", num[i]);
	ptr = ptr + sizeof(int);
  }

  return fd;
}


int getdata (int fd_image, int fd_label, fix16_t *in_data, int *ans)
{
  uint8_t image[INPUTNO];
  for (int b = 0; b < MAXINPUTNO; b++) {
	if (read (fd_image, image, INPUTNO * sizeof(unsigned char)) == -1) { perror("read"); exit (EXIT_FAILURE); }
	for (int i = 0; i < INPUTNO; i++) {
	  FX16_RAW (in_data[b * INPUTNO + i]) = image[i];
	}
	uint8_t label;
	if (read (fd_label, &label, sizeof(uint8_t)) == -1) { perror("read"); exit (EXIT_FAILURE); }
	ans[b] = label;
  }
}


void print_images (fix16_t data[INPUTNO], int label)
{
  printf ("=== LABEL %d ===\n", label);
  for (int j = 0; j < INPUTNO; j++) {
    printf("%1.5lf ", fx16tod (&data[j]));
	if ( (j+1) % 28 == 0 ){
	  printf("\n");
	}
  }
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
	  FX16_RAW(out[b * output_size + o]) = 0.0;
	  for (int i = 0; i < input_size; i++) {
		FX16_RAW(out[b * output_size + o]) += (FX16_RAW(in_data[b * input_size + i]) * FX16_RAW(wh[i * output_size + o]));
	  }
	  FX16_RAW(out[b * output_size + o]) += FX16_RAW (wb[o]);
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
						const fix16_t *x,     // [batch_size][hidden_size],
						const int debug)
{
  for (int b = 0; b < batch_size; b++) {
	for (int h = 0; h < hidden_size; h++) {
	  FX16_RAW(dx[b * hidden_size + h]) = 0.0;
	  for (int o = 0;o < output_size; o++) {
		FX16_RAW(dx[b * hidden_size + h]) += (FX16_RAW(dout[b * output_size + o]) * FX16_RAW(w[h * output_size + o]) / 65536);  // w is Transpose
	  }
	}
  }
  for (int h = 0; h < hidden_size; h++) {
	for (int o = 0; o < output_size; o++) {
	  FX16_RAW(dw[h * output_size + o]) = 0.0;
	  for (int b = 0; b < batch_size; b++) {
		FX16_RAW(dw[h * output_size + o]) += (FX16_RAW(x[b * hidden_size + h]) * FX16_RAW(dout[b * output_size + o]) / 65536);
	  }
	}
  }

  for (int o = 0; o < output_size; o++) {
	FX16_RAW(db[o]) = 0;
	for (int b = 0; b < batch_size; b++) {
	  FX16_RAW(db[o]) += FX16_RAW(dout[b * output_size + o]);
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
	  FX16_RAW(o[b * size + i]) = FX16_SIGN(e[b * size + i]) == 0 ? FX16_RAW(e[b * size + i]) : 0;
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
	  FX16_RAW(dx[b * size + i]) = FX16_SIGN(x[b * size + i]) == 0 ? FX16_RAW (dout[b * size + i]) : 0;
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
	FX16_RAW(max[b]) = FX16_RAW(e[b * batch_size + 0]);
	for (int i = 1; i < size; i++) {
	  FX16_RAW(max[b]) = FX16_RAW(max[b]) < FX16_RAW(e[b * size + i]) ? FX16_RAW(e[b * size + i]) : FX16_RAW(max[b]);
	}
  }

  for (int b = 0; b < batch_size; b++) {
	fix16_t exp_sum;
    FX16_RAW(exp_sum) = 0.0;
	fix16_t *a = (fix16_t *)malloc(sizeof(fix16_t) * size);
	for (int i = 0; i < size; i++) {
	  FX16_RAW(a[i]) = FX16_RAW(e[b * size + i]) - FX16_RAW(max[b]);
	  FX16_RAW(exp_sum) += exp(FX16_RAW(a[i]));
	}
	for (int i = 0; i < size; i++) {
	  FX16_RAW(o[b * size + i]) = exp(FX16_RAW(a[i])) / FX16_RAW(exp_sum);
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
	  FX16_RAW(dx[b * size + y_idx]) = (FX16_RAW(y[b * size + y_idx]) - FX16_RAW(t[b * size + y_idx])) / batch_size;
	}
  }
}


int argmax (const int x_size, fix16_t *o)
{
  int    ret = FX16_RAW (o[0]);
  int    max_idx = 0;
  for (int i = 1; i < x_size; i++) {
	if (FX16_RAW(o[i]) > ret) {
	  ret = FX16_RAW(o[i]);
	  max_idx = i;
	}
  }

  return max_idx;
}


inline void dtofx16 (fix16_t *f16, double d)
{
  f16->i = (int)(d * 65536.0);
}


inline double fx16tod (fix16_t *f16)
{
  return (double)(f16->i) / 65536.0;
}



void initwh (const int y_size, const int x_size, fix16_t *wh)
{
  for (int y = 0; y < y_size; y++) {
	for (int x = 0; x < x_size; x++) {
	  dtofx16 (&wh[y * x_size + x], WEIGHT_INIT * rand_normal (0.0, 1.0));
	}
  }
}


void initwb (const int x_size, fix16_t *wb)
{
  for (int j = 0; j < x_size + 1; j++) {
	dtofx16 (&wb[j], 0.0);
  }
}


double rand_normal (double mu, double sigma)
{
  double z = sqrt( -2.0 * log(drnd()) ) * sin( 2.0 * M_PI * drnd() );
  return mu + sigma*z;
}


double drnd ()
{
  double rndno;
  while ((rndno = (double)rand() / RAND_MAX) == 1.0);
  return rndno;
}
