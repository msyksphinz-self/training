/*************************************************/
/* bp1.c                                         */
/* Neural network learning with back propagation */
/*************************************************/


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
#define BIGNUM     (100)  // Initial value of error
#define LEARNING_RATE (0.1)
#define LIMIT      (0.001)   // Max number of error
#define WEIGHT_INIT (0.01)

int open_image ();
int open_label ();

int getdata (int fd_image, int fd_label, double *in_data, double *ans);
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

double affine (const int output_size, const int input_size,
			   double *o, const double *e,
			   const double wh[output_size][input_size], const double *b);
double affine_backward (const int output_size, const int hidden_size,
						double *dx, double dw[output_size][hidden_size], double *db,
						double *dout, double *x);

double relu    (double *o, const double *e, const int input_size);
double relu_backward (double *dx, double *dout, int size);

double softmax (const int input_size, double *o, double *e);
double softmax_backward (double *dx, const double *y, const double *t,
						 const int size);
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
  double in_data[INPUTNO];
  double ans_data;
  double o[OUTPUTNO];
  double err = BIGNUM;
  int i, j;
  int n_of_e;

  srand (SEED);


  initwh0 (wh0);
  initwh1 (wh1);

  initwb0 (wb0);
  initwb1 (wb1);

  for (int y = 0; y < INPUTNO; y++) {
    for (int x = 0; x < HIDDENNO; x++) {
      printf ("%lf ", wh0[y][x]);
    }
    printf ("\n");
  }

  // initwh (INPUTNO, HIDDENNO,  wh0);
  // initwh (HIDDENNO, OUTPUTNO, wh1);
  //
  // initwb (HIDDENNO, wb0);
  // initwb (HIDDENNO, wb1);

  n_of_e = MAXINPUTNO;
  int fd_image = open_image ();
  int fd_label = open_label ();

  // while (err > LIMIT) {
  err = 0.0;
  int correct = 0;
  for (int no_input = 0; no_input < n_of_e; no_input++) {
	getdata (fd_image, fd_label, in_data, &ans_data);

    print_images (in_data, ans_data);

	double ans_label[OUTPUTNO] = {0.0};
	ans_label[(int)ans_data] = 1.0;

	double af0[HIDDENNO];
	double af1[OUTPUTNO];
	double rel0[HIDDENNO];
	double rel1[HIDDENNO];

	affine (HIDDENNO, INPUTNO,  af0, in_data, wh0, wb0);
	relu (rel0, af0, HIDDENNO);

    printf ("affine0 : "); for (int i = 0; i < HIDDENNO; i++) { printf ("%1.10lf ", af0[i]); if ((i%10) == 9) { printf ("\n"); }  } printf("\n");
    // printf ("relu0   : "); for (int i = 0; i < HIDDENNO; i++) { printf ("%1.3lf ", rel0[i]); } printf("\n");

	affine (OUTPUTNO, HIDDENNO, af1, rel0,    wh1, wb1);
	// softmax (OUTPUTNO, rel1, af1);

	// printf ("affine1 : "); for (int i = 0; i < OUTPUTNO; i++) { printf ("%1.5lf ", af1[i]); } printf("\n");
    // printf ("softmax1: "); for (int i = 0; i < OUTPUTNO; i++) { printf ("%1.5lf ", rel1[i]);  } printf("\n");

	int t = argmax (OUTPUTNO, af1);
	if (t == (int)ans_data) correct++;

	printf ("t = %d, ans_data=%d\n", t, (int)ans_data);

	// Back ward
	// double softmax_dx[OUTPUTNO];
	// softmax_backward (softmax_dx, rel1, ans_label, OUTPUTNO);
    //
	// double affine1_dx[HIDDENNO];
	// double affine1_dw[HIDDENNO][OUTPUTNO];
	// double affine1_db[OUTPUTNO];
	// affine_backward (OUTPUTNO, HIDDENNO,
	// 				 affine1_dx, affine1_dw, affine1_db,
	// 				 af1, rel0);
	// double relu_dx[HIDDENNO];
	// relu_backward (relu_dx, rel0, HIDDENNO);
	// double affine0_dx[INPUTNO];
	// double affine0_dw[INPUTNO][HIDDENNO];
	// double affine0_db[HIDDENNO];
	// affine_backward (OUTPUTNO, HIDDENNO,
	// 				 affine0_dx, affine0_dw, affine0_db,
	// 				 af0, in_data);
    //
	// printf ("affine1_db : "); for (int i = 0; i < OUTPUTNO; i++) printf ("%1.5lf ", affine1_db[i]); printf ("\n");



	// for (int j = 0; j < OUTPUTNO; j++) {
	//   for (int i = 0; i < HIDDENNO; i++) {
	// 	printf ("%1.5lf ", affine0_dw[j][i]);
	//   }
	//   printf ("\n");
	// }
	// for (int i = 0; i < HIDDENNO; i++) wb1[i] = LEARNING_RATE * affine1_db[i];
	// for (int j = 0; j < HIDDENNO; j++) {
	//   for (int i = 0; i < INPUTNO; i++) wh1[j][i] = LEARNING_RATE * affine1_dw[j][i];
	// }


	// for (int i = 0; i < HIDDENNO; i++) wb0[i] = LEARNING_RATE * affine0_db[i];
	// for (int j = 0; j < OUTPUTNO; j++) {
	//   for (int i = 0; i < HIDDENNO; i++) wh0[j][i] = LEARNING_RATE * affine0_dw[j][i];
	// }
	// for (int i = 0; i < HIDDENNO; i++) wb1[i] = LEARNING_RATE * affine1_db[i];
	// for (int j = 0; j < HIDDENNO; j++) {
	//   for (int i = 0; i < INPUTNO; i++) wh1[j][i] = LEARNING_RATE * affine1_dw[j][i];
	// }

	err = (double)correct / no_input;
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

#define IMAGE_FILE "t10k-images-idx3-ubyte"
#define LABEL_FILE "t10k-labels-idx1-ubyte"

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


int getdata (int fd_image, int fd_label, double *in_data, double *ans)
{
  uint8_t image[INPUTNO];
  read (fd_image, image, INPUTNO * sizeof(unsigned char));
  for (int j = 0; j < INPUTNO; j++) {
	in_data[j] = (double)image[j] / 256.0;
    // printf ("%lf\n", in_data[j]);
  }
  uint8_t label;
  read (fd_label, &label, sizeof(uint8_t));
  *ans = (double)label;
}


void print_images (double data[INPUTNO], double label)
{
  printf ("=== LABEL %d ===\n", (int)label);
  for (int j = 0; j < INPUTNO; j++) {
	printf("%3d ", (int)(data[j] * 256.0));
	if ( (j+1) % 28 == 0 ){
	  printf("\n");
	}
  }
}


double affine (const int output_size, const int input_size,
			   double *o, const double *e,
			   const double wh[output_size][input_size], const double *b)
{
  for (int i = 0; i < output_size; i++) {
	o[i] = 0.0;
	for (int j = 0; j < input_size; j++) {
	  o[i] += e[j] * wh[i][j];
	}
	o[i] += b[i];
  }
}


double affine_backward (const int output_size, const int hidden_size,
						double *dx, double dw[output_size][hidden_size], double *db,
						double *dout, double *x)
{
  for (int x = 0;x < output_size; x++) {
	dx[x] = 0.0;
	for (int y = 0; y < hidden_size; y++) {
	  dx[x] += dout[y] * dw[x][y];  // dw is Transpose
	}
  }

  for (int y_idx = 0; y_idx < hidden_size; y_idx++) {
	for (int x_idx = 0; x_idx < output_size; x_idx++) {
	  dw[y_idx][x_idx] = x[y_idx] * dout[x_idx];
	}
  }

  for (int y = 0; y < output_size; y++) {
	db[y] = dout[y];
  }
}


double relu (double *o, const double *e, int size)
{
  for (int i = 0; i < size; i++) {
	o[i] = e[i] > 0.0 ? e[i] : 0.0;
  }
}


double relu_backward (double *dx, double *dout, int size)
{
  for (int i = 0; i < size; i++) {
	dx[i] = dout[i];
  }
}


double softmax (const int x_size, double *o, double *e)
{
  double max = e[0];
  for (int i = 1; i < x_size; i++) {
	max = max < e[i] ? e[i] : max;
  }
  double exp_sum = 0.0;
  double *exp_a = (double *)malloc(sizeof(double) * x_size);
  for (int i = 0; i < x_size; i++) {
	exp_a[i] = exp (e[i] - max);
	exp_sum += exp_a[i];
  }
  for (int i = 0; i < x_size; i++) {
	o[i] = exp_a[i] / exp_sum;
  }
}


double softmax_backward (double *dx, const double *y, const double *t,
						 const int size)
{
  for (int y_idx = 0; y_idx < size; y_idx++) {
	dx[y_idx] = y[y_idx] - t[y_idx];
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
  FILE *fp = fopen("./dl_scratch/grad_w1.h", "r");
  for (int y = 0; y < INPUTNO; y++) {
    for (int x = 0; x < HIDDENNO; x++) {
      fscanf(fp, "%lf", &wh[y][x]);
    }
  }
}


void initwh1 (double wh[HIDDENNO][OUTPUTNO])
{
  FILE *fp = fopen("./dl_scratch/grad_w2.h", "r");
  for (int y = 0; y < HIDDENNO; y++) {
    for (int x = 0; x < OUTPUTNO; x++) {
      fscanf(fp, "%lf", &wh[y][x]);
    }
  }
}


void initwb0 (double wb[HIDDENNO])
{
  FILE *fp = fopen("./dl_scratch/grad_b1.h", "r");
  for (int x = 0; x < HIDDENNO; x++) {
    fscanf (fp, "%lf", &wb[x]);
  }
}


void initwb1 (double wb[OUTPUTNO])
{
  FILE *fp = fopen("./dl_scratch/grad_b2.h", "r");
  for (int x = 0; x < OUTPUTNO; x++) {
    fscanf (fp, "%lf", &wb[x]);
  }
}


initwh(const int y_size, const int x_size, double wh[y_size][x_size])
{
  for (int i = 0; i < y_size; i++) {
	for (int j = 0; j < x_size; j++) {
	  wh[i][j] = WEIGHT_INIT * rand_normal (0.0, 1.0);
	}
  }
}


void initwb(const int x_size, double *wb)
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
