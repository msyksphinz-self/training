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

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (50)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (60000)  // Max number of learning data
#define BATCH_SIZE (100)
#define BIGNUM     (100)  // Initial value of error
#define LEARNING_RATE (0.1)
#define LIMIT      (0.001)   // Max number of error
#define WEIGHT_INIT (0.01)

int open_image ();
int open_label ();

int getdata (int fd_image,
			 int fd_label,
			 double in_data[BATCH_SIZE][INPUTNO],
			 double *ans);
void hlearn (double **wh, // weight of hidden layer
			 double *wo, // weight of output layer
			 int output_size, int input_size,
			 double *hi, // current hidden layer ansewr
			 double *e,  // input value
			 double *o);   // output value
void print_images (double data[INPUTNO], double label);
void olearn (double *wo,
			 int     element_size,
			 double *hi,
			 double *e,
			 double *ans,
			 double *o);

double affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   double o[batch_size][output_size],
			   const double in_data[batch_size][input_size],
			   const double wh[input_size][output_size],
			   const double *b);
double affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						double dx[batch_size][output_size],
						double db[output_size],
						double dw[output_size][hidden_size],
						const double dout[batch_size][output_size],
						const double w[hidden_size][output_size],
						const double x[batch_size][hidden_size]);

void relu (const int batch_size,
		   const int size,
		   double o[batch_size][size],
		   const double e[batch_size][size]);
double relu_backward (const int batch_size,
					  const int size,
					  double dx[batch_size][size],
					  const double x[batch_size][size],
					  const double dout[batch_size][size]);

double softmax (const int batch_size,
				const int size,
				double o[batch_size][size],
				double e[batch_size][size]);
double softmax_backward (const int batch_size,
						 const int size,
						 double dx[batch_size][size],
						 const double y[batch_size][size],
						 const double t[batch_size][size]);

void forward (double *o, double *e, int input_size, int output_size);

int argmax (const int x_size, double *o);

void print (double wh[HIDDENNO][INPUTNO],
			double wo[HIDDENNO]);

void initwh(const int y_size, const int x_size, double wh[y_size][x_size]);
void initwb(const int x_size, double *wb);

void initwh0 (double wh[INPUTNO][HIDDENNO]);
void initwh1 (double wh[HIDDENNO][OUTPUTNO]);
void initwb0 (double wb[HIDDENNO]);
void initwb1 (double wb[HIDDENNO]);


double rand_normal ( double mu, double sigma );
double drnd ();
double f (double u);


int main ()
{
  double wh0[INPUTNO][HIDDENNO];
  double wb0[HIDDENNO];

  double wh1[HIDDENNO][OUTPUTNO];
  double wb1[OUTPUTNO];

  double hi[HIDDENNO + 1];
  double in_data[BATCH_SIZE][INPUTNO];
  double ans_data[BATCH_SIZE];
  double o[OUTPUTNO];
  double err = BIGNUM;
  int i, j;
  int n_of_e;

  srand (SEED);


  initwh0 (wh0);
  initwh1 (wh1);

  initwb0 (wb0);
  initwb1 (wb1);

  // initwh (INPUTNO, HIDDENNO,  wh0);
  // initwh (HIDDENNO, OUTPUTNO, wh1);
  //
  // initwb (HIDDENNO, wb0);
  // initwb (HIDDENNO, wb1);

  n_of_e = MAXINPUTNO / BATCH_SIZE;
  int fd_image = open_image ();
  int fd_label = open_label ();

  // while (err > LIMIT) {
  err = 0.0;
  int correct = 0;
  for (int no_input = 0; no_input < n_of_e; no_input++) {
	getdata (fd_image, fd_label, in_data, ans_data);

	// print_images (in_data[0], ans_data[0]);

	double af0[BATCH_SIZE][HIDDENNO];
	double af1[BATCH_SIZE][OUTPUTNO];
	double rel0[BATCH_SIZE][HIDDENNO];
	double rel1[BATCH_SIZE][OUTPUTNO];

	affine (HIDDENNO, INPUTNO, BATCH_SIZE, af0, in_data, wh0, wb0);
	relu (BATCH_SIZE, HIDDENNO, rel0, af0);

    // printf ("affine0 : "); for (int i = 0; i < HIDDENNO; i++) { printf ("%1.10lf ", af0[i]); if ((i%10) == 9) { printf ("\n"); }  } printf("\n");
    // printf ("relu0   : "); for (int i = 0; i < HIDDENNO; i++) { printf ("%1.3lf ", rel0[i]); } printf("\n");

    affine (OUTPUTNO, HIDDENNO, BATCH_SIZE, af1, rel0,    wh1, wb1);
	softmax (BATCH_SIZE, OUTPUTNO, rel1, af1);

    // printf ("affine1 : "); for (int i = 0; i < OUTPUTNO; i++) { printf ("%1.5lf ", af1[i]); } printf("\n");

	// printf ("affine1:\n");
	// for (int b = 0; b < BATCH_SIZE; b++) {
	//   for (int i = 0; i < OUTPUTNO; i++) { 
	// 	printf ("%1.10lf ", af1[b][i]); 
	//   } 
	//   printf("\n");
	// }
	// 
	// printf ("softmax1:\n");
	// for (int b = 0; b < BATCH_SIZE; b++) {
	//   for (int i = 0; i < OUTPUTNO; i++) { 
	// 	printf ("%1.10lf ", rel1[b][i]); 
	//   } 
	//   printf("\n");
	// }

	for (int b = 0; b < BATCH_SIZE; b++) {
	  int t = argmax (OUTPUTNO, af1[b]);
	  if (t == (int)ans_data[b]) correct++;
	}

	// Back ward
	double ans_label[BATCH_SIZE][OUTPUTNO] = {{0.0}};
	for (int b = 0; b < BATCH_SIZE; b++) {
	  ans_label[b][(int)ans_data[b]] = 1.0;
	}

	double softmax_dx[BATCH_SIZE][OUTPUTNO];
	softmax_backward (BATCH_SIZE, OUTPUTNO, softmax_dx, rel1, ans_label);
	
	// printf ("softmax_dx : \n"); 
	// for (int b = 0; b < BATCH_SIZE; b++) {
	//   for (int o = 0; o < OUTPUTNO; o++) {
	// 	printf ("%1.10lf ", softmax_dx[b][o]);
	//   }
	//   printf ("\n");
	// }

	// printf ("wh1 : \n"); 
	// for (int h = 0; h < HIDDENNO; h++) {
	//   for (int o = 0; o < OUTPUTNO; o++) {
	// 	printf ("%1.10lf ", wh1[h][o]);
	//   }
	//   printf ("\n");
	// }
	// 
	// 
	// printf ("rel0 : \n"); 
	// for (int b = 0; b < BATCH_SIZE; b++) {
	//   for (int h = 0; h < HIDDENNO; h++) {
	// 	printf ("%1.10lf ", rel0[b][h]);
	//   }
	//   printf ("\n");
	// }


	double affine1_dx[BATCH_SIZE][HIDDENNO];
	double affine1_dw[HIDDENNO][OUTPUTNO];
	double affine1_db[OUTPUTNO];
	affine_backward (OUTPUTNO, HIDDENNO, BATCH_SIZE,
					 affine1_dx, affine1_db, affine1_dw, 
					 softmax_dx, wh1, rel0);

	// printf ("affine1_dw : "); 
	// for (int h = 0; h < HIDDENNO; h++) {
	//   for (int o = 0; o < OUTPUTNO; o++) {
	// 	printf ("%1.10lf ", affine1_dw[h][o]);
	//   }
	//   printf ("\n");
	// }

	// printf ("affine1_dx : "); 
	// for (int b = 0; b < BATCH_SIZE; b++) {
	//   for (int h = 0; h < HIDDENNO; h++) {
	// 	printf ("%1.10lf ", affine1_dx[b][h]);
	//   }
	//   printf ("\n");
	// }

	double relu_dx[BATCH_SIZE][HIDDENNO];
	relu_backward (BATCH_SIZE, HIDDENNO, relu_dx, af0, affine1_dx);

	// printf ("relu_dx : \n");
	// for (int b = 0; b < BATCH_SIZE; b++) {
	//   for (int h = 0; h < HIDDENNO; h++) {
	// 	printf ("%1.10lf ", relu_dx[b][h]);
	//   }
	//   printf ("\n");
	// }


	double affine0_dx[BATCH_SIZE][INPUTNO];
	double affine0_dw[INPUTNO][HIDDENNO];
	double affine0_db[HIDDENNO];
	affine_backward (HIDDENNO, INPUTNO, BATCH_SIZE,
					 affine0_dx, affine0_db, affine0_dw, 
					 relu_dx, wh0, in_data);

	// printf ("affine0_dw : "); 
	// for (int i = 0; i < INPUTNO; i++) {
	//   for (int h = 0; h < HIDDENNO; h++) {
	// 	printf ("%1.10lf ", affine0_dw[i][h]);
	//   }
	//   printf ("\n");
	// }

	err = (double)correct / (no_input * BATCH_SIZE);
	printf ("%d\t%lf\n", no_input, err);
  }

  // }

  close(fd_image);
  close(fd_label);

  return 0;
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
  read(fd, num, 4 * sizeof(int));

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
  read(fd, num, 2 * sizeof(int));

  for (int i = 0; i < 2; i++) {
	ptr = (unsigned char *)(num + i);
	FlipLong( ptr);
	printf("%d\n", num[i]);
	ptr = ptr + sizeof(int);
  }

  return fd;
}


int getdata (int fd_image, int fd_label, double in_data[BATCH_SIZE][INPUTNO], double *ans)
{
  uint8_t image[INPUTNO];
  for (int b = 0; b < BATCH_SIZE; b++) {
	read (fd_image, image, INPUTNO * sizeof(unsigned char));
	for (int j = 0; j < INPUTNO; j++) {
	  in_data[b][j] = (double)image[j] / 256.0;
	}
	uint8_t label;
	read (fd_label, &label, sizeof(uint8_t));
	ans[b] = (double)label;
  }
}


void print_images (double data[INPUTNO], double label)
{
  printf ("=== LABEL %d ===\n", (int)label);
  for (int j = 0; j < INPUTNO; j++) {
	// printf("%3d ", (int)(data[j] * 256.0));
    printf("%1.5lf ", data[j]);
	if ( (j+1) % 28 == 0 ){
	  printf("\n");
	}
  }
}


double affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   double o[batch_size][output_size],
			   const double in_data[batch_size][input_size],
			   const double wh[input_size][output_size],
			   const double wb[output_size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < output_size; i++) {
	  o[b][i] = 0.0;
	  for (int j = 0; j < input_size; j++) {
		o[b][i] += (in_data[b][j] * wh[j][i]);
	  }
	  o[b][i] += wb[i];
	}
  }
}


double affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						double dx[batch_size][hidden_size],
						double db[output_size],
						double dw[hidden_size][output_size],
						const double dout[batch_size][output_size],
						const double w[hidden_size][output_size],
						const double x[batch_size][hidden_size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int y = 0; y < hidden_size; y++) {
	  dx[b][y] = 0.0;
	  for (int x = 0;x < output_size; x++) {
		dx[b][y] += (dout[b][x] * w[y][x]);  // w is Transpose
	  }
	}
  }
  for (int h = 0; h < hidden_size; h++) {
	for (int o = 0; o < output_size; o++) {
	  for (int b = 0; b < batch_size; b++) {
		dw[h][o] += (x[b][h] * dout[b][o]);
	  }
	}
  }

  for (int o = 0; o < output_size; o++) {
	db[o] = 0.0;
	for (int b = 0; b < batch_size; b++) {
	  db[o] += dout[b][o];
	}
  }
}


void relu (const int batch_size,
		   const int size,
		   double o[batch_size][size],
		   const double e[batch_size][size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  o[b][i] = e[b][i] > 0.0 ? e[b][i] : 0.0;
	}
  }
  return;
}


double relu_backward (const int batch_size,
					  const int size,
					  double dx[batch_size][size],
					  const double x[batch_size][size],
					  const double dout[batch_size][size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  dx[b][i] = (x[b][i] > 0.0) ? dout[b][i] : 0.0;
	}
  }
}


double softmax (const int batch_size,
				const int size,
				double o[batch_size][size],
				double e[batch_size][size])
{
  double *max = (double *)malloc(sizeof(double) * batch_size);
  for (int b = 0; b < batch_size; b++) {
	max[b] = e[b][0];
	for (int i = 1; i < size; i++) {
	  max[b] = max[b] < e[b][i] ? e[b][i] : max[b];
	}
  }

  for (int b = 0; b < batch_size; b++) {
	double exp_sum = 0.0;
	double *a = (double *)malloc(sizeof(double) * size);
	for (int i = 0; i < size; i++) {
	  a[i] = e[b][i] - max[b];
	  exp_sum += exp(a[i]);
	}
	for (int i = 0; i < size; i++) {
	  o[b][i] = exp(a[i]) / exp_sum;
	}
  }
}


double softmax_backward (const int batch_size,
						 const int size,
						 double dx[batch_size][size],
						 const double y[batch_size][size],
						 const double t[batch_size][size])
{
  // printf ("=== softmax_backward ===\n");
  for (int b = 0; b < batch_size; b++) {
	// for (int y_idx = 0; y_idx < size; y_idx++) {
	//   printf ("%1.10lf ", t[b][y_idx]);
	// }
	// printf ("\n");
	for (int y_idx = 0; y_idx < size; y_idx++) {
	  dx[b][y_idx] = (y[b][y_idx] - t[b][y_idx]) / batch_size;
	  // printf ("%1.10lf ", dx[b][y_idx]);
	}
	// printf ("\n");
  }
}


int argmax (const int x_size, double *o)
{
  double ret = o[0];
  int    max_idx = 0;
  for (int i = 1; i < x_size; i++) {
	if (o[i] > ret) {
	  ret = o[i];
	  max_idx = i;
	}
  }

  return max_idx;
}



void print (double wh[HIDDENNO][INPUTNO],
			double wo[HIDDENNO])
{
  int i, j;
  for (i = 0; i < HIDDENNO; i++) {
	for (j = 0;j < INPUTNO; j++) {
	  printf ("%lf ", wh[i][j]);
	}
  }
  printf ("\n");
  for (i = 0; i < HIDDENNO; i++) {
	printf ("%lf\n", wo[i]);
  }
  printf ("\n");
}


void initwh0 (double wh[INPUTNO][HIDDENNO])
{
  FILE *fp = fopen("./w1_init.h", "r");
  for (int y = 0; y < INPUTNO; y++) {
    for (int x = 0; x < HIDDENNO; x++) {
      fscanf(fp, "%lf", &wh[y][x]);
    }
  }
}


void initwh1 (double wh[HIDDENNO][OUTPUTNO])
{
  FILE *fp = fopen("./w2_init.h", "r");
  for (int y = 0; y < HIDDENNO; y++) {
    for (int x = 0; x < OUTPUTNO; x++) {
      fscanf(fp, "%lf", &wh[y][x]);
    }
  }
}


void initwb0 (double wb[HIDDENNO])
{
  FILE *fp = fopen("./b1_init.h", "r");
  for (int x = 0; x < HIDDENNO; x++) {
    fscanf (fp, "%lf", &wb[x]);
  }
}


void initwb1 (double wb[OUTPUTNO])
{
  FILE *fp = fopen("./b2_init.h", "r");
  for (int x = 0; x < OUTPUTNO; x++) {
    fscanf (fp, "%lf", &wb[x]);
  }
}


void initwh (const int y_size, const int x_size, double wh[y_size][x_size])
{
  for (int i = 0; i < y_size; i++) {
	for (int j = 0; j < x_size; j++) {
	  wh[i][j] = WEIGHT_INIT * rand_normal (0.0, 1.0);
	}
  }
}


void initwb (const int x_size, double *wb)
{
  for (int j = 0; j < x_size + 1; j++) {
	wb[j] = 0.0;
  }
}


double rand_normal( double mu, double sigma ){
  double z=sqrt( -2.0 * log(drnd()) ) * sin( 2.0 * M_PI * drnd() );
  return mu + sigma*z;
}


double drnd ()
{
  double rndno;
  while ((rndno = (double)rand() / RAND_MAX) == 1.0);
  return rndno;
}


double f (double u)
{
  return 1.0 / (1.0 + exp(-u));
}
