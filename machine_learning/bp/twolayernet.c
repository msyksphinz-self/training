/*************************************************/
/* bp1.c                                         */
/* Neural network learning with back propagation */
/*************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <fcntl.h>

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (50)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (60000)  // Max number of learning data
#define BIGNUM     (100)  // Initial value of error
#define LIMIT      (0.001)   // Max number of error

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

double affine (double *o, double *e,
			   double wh[HIDDENNO][INPUTNO], double *b,
			   int output_size, int input_size);
double relu    (double *o, double *e, int input_size);
double softmax (double *o, double *e, int input_size);
void forward (double *o, double *e, int input_size, int output_size);

void print (double wh[HIDDENNO][INPUTNO],
			double wo[HIDDENNO]);
void initwh(double wh[HIDDENNO][INPUTNO]);
void initwb(double wb[HIDDENNO]);
void initwo (double wo[HIDDENNO]);
double drnd (void);
double f (double u);


int main ()
{
  double wh0[HIDDENNO][INPUTNO];
  double wb0[HIDDENNO];
  
  double wh1[OUTPUTNO][HIDDENNO];
  double wb1[HIDDENNO];

  double wo[HIDDENNO];
  
  double hi[HIDDENNO + 1];
  double in_data[INPUTNO];
  double ans_data;
  double o[OUTPUTNO];
  double err = BIGNUM;
  int i, j;
  int n_of_e;

  srand (SEED);

  initwh (wh0);
  initwh (wh1);

  initwb (wb0);
  initwb (wb1);

  initwo (wo);
  // print (wh0, wo);    // Print result

  n_of_e = MAXINPUTNO;
  int fd_image = open_image ();
  int fd_label = open_label ();

  // while (err > LIMIT) {
  	err = 0.0;
	int correct = 0;
  	for (int no_input = 0; no_input < n_of_e; no_input++) {
	  getdata (fd_image, fd_label, in_data, &ans_data);

	  // print_images (in_data, ans_data);
	  
  	  double af0[HIDDENNO];
  	  double af1[OUTPUTNO];
  	  double rel0[HIDDENNO];
  	  double rel1[HIDDENNO];
  	  affine (af0, in_data, wh0, wb0, HIDDENNO, INPUTNO);
  	  relu (rel0, af0, HIDDENNO);
  	  affine (af1, rel0, wh1, wb1, OUTPUTNO, HIDDENNO);
  	  softmax (rel1, af1, OUTPUTNO);

	  int t = argmax (rel1);
	  if (t == (int)ans_data) correct++;
  	  // olearn (wo, HIDDENNO, rel0, af0, ans_data, rel0);

	  printf ("t = %d, ans_data=%d\n", t, (int)ans_data);
	  
	  err = (double)correct / no_input;
	  fprintf (stderr, "%d\t%lf\n", no_input, err);
  	}

	// }
  
	// print (wh0, wo);

  // for (i = 0; i < n_of_e; i++) {
  // 	printf ("%d ", i);
  // 	for (j = 0; j < INPUTNO; j++) {
  // 	  printf ("%lf ", e[i][j]);
  // 	}
  // 	o = forward (wh, wo, hi, e[i]);
  // 	printf ("%lf\n", o);
  // }

  close(fd_image);
  close(fd_label);

  return 0;
}


void hlearn (double **wh, // weight of hidden layer
			 double *wo, // weight of output layer
			 int output_size, int input_size,
			 double *hi, // current hidden layer ansewr
			 double *e,  // input value
			 double *o)   // output value
{
  int i, j;
  double dj;

  for (j = 0; j < output_size; j++) {
	dj = hi[j] * (1 - hi[j]) * wo[j] * (e[INPUTNO] - o[i]) * o[j] * (1 - o[j]);
	for (i = 0; i < input_size; i++) {
	  wh[j][i] += ALPHA * e[i] * dj;
	}
	wh[j][i] += ALPHA * (-1.0) * dj;
  }
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

#define IMAGE_FILE "train-images-idx3-ubyte"
#define LABEL_FILE "train-labels-idx1-ubyte"

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
	in_data[j] = (double)image[j];
  }
  uint8_t label;
  read (fd_label, &label, sizeof(uint8_t));
  *ans = (double)label;
}


void print_images (double data[INPUTNO], double label)
{
  printf ("=== LABEL %d ===\n", (int)label);
  for (int j = 0; j < INPUTNO; j++) { 
	printf("%3d ", (int)data[j]); 
	if ( (j+1) % 28 == 0 ){
	  printf("\n"); 
	}
  }
}



// int getdata (double **in_data, double *ans)
// {
//   int n_of_e = 0;
//   int j = 0;
//   while (scanf ("%lf", &in_data[n_of_e][j]) != EOF) {
// 	j++;
// 	if (j > INPUTNO) {
// 	  scanf ("%lf", &ans[n_of_e]);
// 	  j = 0;
// 	  n_of_e++;
// 	}
//   }
// 
//   return n_of_e;
// }


void olearn (double *wo,
			 int     element_size,
			 double *hi,
			 double *e,
			 double *ans,
			 double *o)
{
  for (int idx = 0; idx < element_size; idx++) {
	int i;
	double d = (ans[idx] - o[idx]) * o[idx] * (1 - o[idx]);
	for (i = 0; i < HIDDENNO; i++) {
	  wo[i] += ALPHA * hi[i] * d;
	}
	wo[i] += ALPHA * (-1.0) * d;
  }
}


double affine (double *o, double *e,
			   double wh[HIDDENNO][INPUTNO], double *b,
			   int output_size, int input_size)
{
  for (int i = 0; i < output_size; i++) {
	o[i] = 0;
	for (int j = 0; j < input_size; j++) {
	  fflush (stdout);
	  o[i] += e[j] * wh[i][j];
	}
	o[i] += b[i];
  }
}


double relu (double *o, double *e, int input_size)
{
  for (int i = 0; i < input_size; i++) {
	o[i] = e[i] > 0.0 ? e[i] : 0.0;
  }
}
			 

double softmax (double *o, double *e, int input_size)
{
  double max = e[0];
  for (int i = 1; i < input_size; i++) {
	max = max < e[i] ? e[i] : max;
  }
  double exp_sum = 0.0;
  double *exp_a = (double *)malloc(sizeof(double) * input_size);
  for (int i = 0; i < input_size; i++) {
	exp_a[i] = exp (e[i] - max);
	exp_sum += exp_a[i];
  }
  for (int i = 0; i < input_size; i++) {
	o[i] = exp_a[i] / exp_sum;
  }
}


int argmax (double o[OUTPUTNO])
{
  double ret = o[0];
  int    max_idx = 0;
  for (int i = 1; i < OUTPUTNO; i++) {
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


void initwh(double wh[HIDDENNO][INPUTNO])
{
  for (int i = 0; i < HIDDENNO; i++) {
	for (int j = 0; j < INPUTNO + 1; j++) {
	  wh[i][j] = drnd ();
	}
  }
}


void initwb(double wb[HIDDENNO])
{
  for (int j = 0; j < HIDDENNO + 1; j++) {
	wb[j] = drnd ();
  }
}


void initwo (double wo[HIDDENNO])
{
  for (int i = 0; i < HIDDENNO; i++) {
	wo[i] = drnd();
  }
}


double drnd (void)
{
  double rndno;
  while ((rndno = (double)rand() / RAND_MAX) == 1.0);
  return rndno;
}


double f (double u)
{
  return 1.0 / (1.0 + exp(-u));
}


		  
