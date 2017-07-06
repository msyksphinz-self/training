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

#ifdef GPERF
#include <gperftools/profiler.h>
#endif // GPERF

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
			 float *in_data,
			 float *ans);
void hlearn (float **wh, // weight of hidden layer
			 float *wo, // weight of output layer
			 int output_size, int input_size,
			 float *hi, // current hidden layer ansewr
			 float *e,  // input value
			 float *o);   // output value
void print_images (float data[INPUTNO], float label);
void olearn (float *wo,
			 int     element_size,
			 float *hi,
			 float *e,
			 float *ans,
			 float *o);

float affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   float *out,           // [batch_size][output_size],
			   const float *in_data, // [batch_size][input_size],
			   const float *wh,      // [input_size][output_size],
			   const float *wb);      // [output_size]
float affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						float *dx,  // [batch_size][output_size],
						float *db,  // [output_size],
						float *dw,  // [output_size][hidden_size],
						const float *dout, // [batch_size][output_size],
						const float *w,    // [hidden_size][output_size],
						const float *x,    // [batch_size][hidden_size],
						int debug);

void relu (const int batch_size,
		   const int size,
		   float *o,         // [batch_size][size],
		   const float *e);  // [batch_size][size]
float relu_backward (const int batch_size,
					  const int size,
					  float       *dx,  // [batch_size][size],
					  const float *x,   // [batch_size][size],
					  const float *dout); // [batch_size][size]);

float softmax (const int batch_size,
				const int size,
				float       *o,  // [batch_size][size],
				const float *e); // [batch_size][size]
float softmax_backward (const int batch_size,
						 const int size,
						 float       *dx,   // [batch_size][size],
						 const float *y,    // [batch_size][size],
						 const float *t);   // [batch_size][size])

void forward (float *o, float *e, int input_size, int output_size);

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const float *wh0,   // [input_size][hidden_size],
				  const float *wb0,   // [hidden_size],
				  const float *wh1,   // [hidden_size][output_size],
				  const float *wb1);   // [output_size]

int argmax (const int x_size, float *o);

void print (float wh[HIDDENNO][INPUTNO],
			float wo[HIDDENNO]);

void initwh(const int y_size, const int x_size, float *wh);
void initwb(const int x_size, float *wb);

float rand_normal ( float mu, float sigma );
float drnd ();
float f (float u);


int main ()
{
  float wh0[INPUTNO * HIDDENNO];
  float wb0[HIDDENNO];

  float wh1[HIDDENNO * OUTPUTNO];
  float wb1[OUTPUTNO];

  float hi[HIDDENNO + 1];
  float *in_data = malloc (sizeof(float) * MAXINPUTNO * INPUTNO);
  float ans_data[MAXINPUTNO];
  float o[OUTPUTNO];
  int n_of_e;

  // srand (SEED);

  initwh (INPUTNO, HIDDENNO, wh0);
  initwb (HIDDENNO, wb0);
  initwh (HIDDENNO, OUTPUTNO, wh0);
  initwb (OUTPUTNO, wb1);

  n_of_e = MAXINPUTNO / BATCH_SIZE;
  int fd_image = open_image ();
  int fd_label = open_label ();

  const int epoch_size = 50;

  getdata (fd_image, fd_label, in_data, ans_data);

#ifdef GPERF
  ProfilerStart("train_twolayernet_float.prof");
#endif // GPERF
  
  for (int no_input = 0; no_input < n_of_e; no_input++) {

    float af0 [BATCH_SIZE * HIDDENNO];
    float af1 [BATCH_SIZE * OUTPUTNO];
    float rel0[BATCH_SIZE * HIDDENNO];
    float rel1[BATCH_SIZE * OUTPUTNO];

	affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, &in_data[INPUTNO * BATCH_SIZE * no_input] , wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);

    affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);
	softmax (BATCH_SIZE, OUTPUTNO, rel1, af1);

	// Back ward
	float ans_label[BATCH_SIZE * OUTPUTNO] = {0.0};
	for (int b = 0; b < BATCH_SIZE; b++) {
	  ans_label[b * OUTPUTNO + (int)ans_data[no_input * BATCH_SIZE + b]] = 1.0;
	}
	float softmax_dx[BATCH_SIZE * OUTPUTNO];
	softmax_backward (BATCH_SIZE, OUTPUTNO, softmax_dx, rel1, ans_label);
	float affine1_dx[BATCH_SIZE * HIDDENNO];
	float affine1_dw[HIDDENNO * OUTPUTNO];
	float affine1_db[OUTPUTNO];

	affine_backward (OUTPUTNO, HIDDENNO, BATCH_SIZE,
					 affine1_dx, affine1_db, affine1_dw,
					 softmax_dx, wh1, rel0, no_input == DUMP_IDX);

	float relu_dx[BATCH_SIZE * HIDDENNO];
	relu_backward (BATCH_SIZE, HIDDENNO, relu_dx, af0, affine1_dx);
	float affine0_dx[BATCH_SIZE * INPUTNO];
	float affine0_dw[INPUTNO * HIDDENNO];
	float affine0_db[HIDDENNO];
	affine_backward (HIDDENNO, INPUTNO, BATCH_SIZE,
					 affine0_dx, affine0_db, affine0_dw,
					 relu_dx, wh0, &in_data[INPUTNO * BATCH_SIZE * no_input], 0);

	for (int i = 0; i < INPUTNO; i++) {
	  for (int h = 0; h < HIDDENNO; h++) {
		wh0[i * HIDDENNO + h] -= (LEARNING_RATE * affine0_dw[i * HIDDENNO + h]);
	  }
	}

	for (int h = 0; h < HIDDENNO; h++) {
	  wb0[h] -= (LEARNING_RATE * affine0_db[h]);
	}

	for (int h = 0; h < HIDDENNO; h++) {
	  for (int o = 0; o < OUTPUTNO; o++) {
		wh1[h * OUTPUTNO + o] -= (LEARNING_RATE * affine1_dw[h * OUTPUTNO + o]);
	  }
	}
	for (int o = 0; o < OUTPUTNO; o++) {
	  wb1[o] -= (LEARNING_RATE * affine1_db[o]);
	}

	if ((no_input % epoch_size) == 0) {
	  TestNetwork (INPUTNO, OUTPUTNO, HIDDENNO, wh0, wb0, wh1, wb1);
	}
  }

#ifdef GPERF
  ProfilerStop ();
#endif // GPERF
  
  // }

  close(fd_image);
  close(fd_label);

  return 0;
}

void TestNetwork (const int input_size,
				  const int output_size,
				  const int hidden_size,
				  const float *wh0,  // [input_size][hidden_size],
				  const float *wb0,  // [hidden_size],
				  const float *wh1,  // [hidden_size][output_size],
				  const float *wb1)  // [output_size]
{
  printf ("=== TestNetwork ===\n");
  int image_fd;
  if ((image_fd = open("t10k-images-idx3-ubyte", O_RDONLY))==-1){
	printf("couldn't open image file t10k-images-idx3-ubyte");
	exit(0);
  }
  unsigned char *ptr;
  static int num[10];
  if (read(image_fd, num, 4 * sizeof(int)) == -1) { perror ("read"); exit(EXIT_FAILURE); };

  int label_fd;
  if ((label_fd = open("t10k-labels-idx1-ubyte", O_RDONLY)) == -1){
	printf("couldn't open image file t10k-labels-idx1-ubyte");
	exit(0);
  }
  if (read(label_fd, num, 2 * sizeof(int)) == -1) { perror ("read"); exit(EXIT_FAILURE); };

  float in_data[BATCH_SIZE][INPUTNO];
  float ans_data[BATCH_SIZE];

  int correct = 0;

  for (int no_input = 0; no_input < 10000; no_input += BATCH_SIZE) {
	for (int b = 0; b < BATCH_SIZE; b++) {
	  uint8_t image[INPUTNO];
	  if (read (image_fd, image, INPUTNO * sizeof(unsigned char)) == -1) { perror ("read"); exit(EXIT_FAILURE); };
	  for (int i = 0; i < INPUTNO; i++) {
		in_data[b][i] = (float)image[i] / 255.0;
	  }
	  uint8_t label;
	  if (read (label_fd, &label, sizeof(uint8_t)) == -1) { perror ("read"); exit(EXIT_FAILURE); };
	  ans_data[b] = (float)label;
	}
	// if (no_input == 10000-BATCH_SIZE) print_images (in_data[0], ans_data[0]);

	float af0 [BATCH_SIZE * HIDDENNO];
	float af1 [BATCH_SIZE * OUTPUTNO];
	float rel0[BATCH_SIZE * HIDDENNO];
	float rel1[BATCH_SIZE * OUTPUTNO];

	affine (HIDDENNO, INPUTNO,  BATCH_SIZE, af0, (const float *)in_data, wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);
	affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);

	for (int b = 0; b < BATCH_SIZE; b++) {
	  int t = argmax (OUTPUTNO, &af1[b * OUTPUTNO]);
	  if (t == (int)ans_data[b]) correct++;
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
  if (read(fd, num, 4 * sizeof(int)) == -1) { perror ("read"); exit(EXIT_FAILURE); }

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
  if (read(fd, num, 2 * sizeof(int)) == -1) { perror ("read"); exit(EXIT_FAILURE); }

  for (int i = 0; i < 2; i++) {
	ptr = (unsigned char *)(num + i);
	FlipLong( ptr);
	printf("%d\n", num[i]);
	ptr = ptr + sizeof(int);
  }

  return fd;
}


int getdata (int fd_image, int fd_label, float *in_data, float *ans)
{
  uint8_t image[INPUTNO];
  for (int b = 0; b < MAXINPUTNO; b++) {
	if (read (fd_image, image, INPUTNO * sizeof(unsigned char)) == -1) { perror ("read"); exit(EXIT_FAILURE); }
	for (int i = 0; i < INPUTNO; i++) {
	  in_data[b * INPUTNO + i] = (float)image[i] / 255.0;
	}
	uint8_t label;
	if (read (fd_label, &label, sizeof(uint8_t)) == -1) { perror ("read"); exit(EXIT_FAILURE); }
	ans[b] = (float)label;
  }
}


void print_images (float data[INPUTNO], float label)
{
  printf ("=== LABEL %d ===\n", (int)label);
  for (int j = 0; j < INPUTNO; j++) {
	// printf("%3d ", (int)(data[j] * 256.0));
    printf("%1.5f ", (float)data[j]);
	if ( (j+1) % 28 == 0 ){
	  printf("\n");
	}
  }
}


float affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   float *out,            // [batch_size][output_size],
			   const float *in_data,  // [batch_size][input_size],
			   const float *wh,       // [input_size][output_size],
			   const float *wb)       // [output_size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int o = 0; o < output_size; o++) {
	  out[b * output_size + o] = 0.0;
	  for (int i = 0; i < input_size; i++) {
		out[b * output_size + o] += (in_data[b * input_size + i] * wh[i * output_size + o]);
	  }
	  out[b * output_size + o] += wb[o];
	}
  }
}


float affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						float *dx,  // [batch_size][hidden_size],
						float *db,  // [output_size],
						float *dw,  // [hidden_size][output_size],
						const float *dout,  // [batch_size][output_size],
						const float *w,     // [hidden_size][output_size],
						const float *x,     // [batch_size][hidden_size],
						const int debug)
{
  for (int b = 0; b < batch_size; b++) {
	for (int h = 0; h < hidden_size; h++) {
	  dx[b * hidden_size + h] = 0.0;
	  for (int o = 0;o < output_size; o++) {
		dx[b * hidden_size + h] += (dout[b * output_size + o] * w[h * output_size + o]);  // w is Transpose
	  }
	}
  }
  for (int h = 0; h < hidden_size; h++) {
	for (int o = 0; o < output_size; o++) {
	  dw[h * output_size + o] = 0.0;
	  for (int b = 0; b < batch_size; b++) {
		dw[h * output_size + o] += (x[b * hidden_size + h] * dout[b * output_size + o]);
	  }
	}
  }

  for (int o = 0; o < output_size; o++) {
	db[o] = 0.0;
	for (int b = 0; b < batch_size; b++) {
	  db[o] += dout[b * output_size + o];
	}
  }
}


void relu (const int batch_size,
		   const int size,
		   float *o,        // [batch_size][size],
		   const float *e)  // [batch_size][size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  o[b * size + i] = e[b * size + i] > 0.0 ? e[b * size + i] : 0.0;
	}
  }
  return;
}


float relu_backward (const int batch_size,
					  const int size,
					  float       *dx,     // [batch_size][size],
					  const float *x,      // [batch_size][size],
					  const float *dout)   // [batch_size][size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  dx[b * size + i] = (x[b * size + i] > 0.0) ? dout[b * size + i] : 0.0;
	}
  }
}


float softmax (const int batch_size,
				const int size,
				float *o,       // [batch_size][size],
				const float *e) // e[batch_size][size]
{
  float *max = (float *)malloc(sizeof(float) * batch_size);
  for (int b = 0; b < batch_size; b++) {
	max[b] = e[b * size + 0];
	for (int i = 1; i < size; i++) {
	  max[b] = max[b] < e[b * size + i] ? e[b * size + i] : max[b];
	}
  }

  for (int b = 0; b < batch_size; b++) {
	float exp_sum = 0.0;
	float *a = (float *)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++) {
	  a[i] = e[b * size + i] - max[b];
	  exp_sum += exp(a[i]);
	}
	for (int i = 0; i < size; i++) {
	  o[b * size + i] = exp(a[i]) / exp_sum;
	}
    free (a);
  }
  free (max);
}


float softmax_backward (const int batch_size,
						 const int size,
						 float        *dx,  // [batch_size][size],
						 const float  *y,  // [batch_size][size],
						 const float  *t)  // [batch_size][size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int y_idx = 0; y_idx < size; y_idx++) {
	  dx[b * size + y_idx] = (y[b * size + y_idx] - t[b * size + y_idx]) / batch_size;
	}
  }
}


int argmax (const int x_size, float *o)
{
  float ret = o[0];
  int    max_idx = 0;
  for (int i = 1; i < x_size; i++) {
	if (o[i] > ret) {
	  ret = o[i];
	  max_idx = i;
	}
  }

  return max_idx;
}



void print (float wh[HIDDENNO][INPUTNO],
			float wo[HIDDENNO])
{
  int i, j;
  for (i = 0; i < HIDDENNO; i++) {
	for (j = 0;j < INPUTNO; j++) {
	  printf ("%f ", wh[i][j]);
	}
  }
  printf ("\n");
  for (i = 0; i < HIDDENNO; i++) {
	printf ("%f\n", wo[i]);
  }
  printf ("\n");
}


void initwh (const int y_size, const int x_size, float *wh)
{
  for (int y = 0; y < y_size; y++) {
	for (int x = 0; x < x_size; x++) {
	  wh[y * x_size + x] = WEIGHT_INIT * rand_normal (0.0, 1.0);
	}
  }
}


void initwb (const int x_size, float *wb)
{
  for (int j = 0; j < x_size + 1; j++) {
	wb[j] = 0.0;
  }
}


float rand_normal( float mu, float sigma ){
  float z=sqrt( -2.0 * log(drnd()) ) * sin( 2.0 * M_PI * drnd() );
  return mu + sigma*z;
}


float drnd ()
{
  float rndno;
  while ((rndno = (float)rand() / RAND_MAX) == 1.0);
  return rndno;
}


float f (float u)
{
  return 1.0 / (1.0 + exp(-u));
}
