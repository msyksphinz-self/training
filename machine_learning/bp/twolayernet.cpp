/*************************************************/
/* bp1.c                                         */
/* Neural network learning with back propagation */
/*************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTNO  (28*28)    // No of input cell
#define OUTPUTNO (10)
#define HIDDENNO (3)    // No of hidden cell
#define ALPHA    (10)   // Coefficient of learning
#define SEED     (65535)  // Seed of random
#define MAXINPUTNO (100)  // Max number of learning data
#define BIGNUM     (100)  // Initial value of error
#define LIMIT      (0.001)   // Max number of error

void hlearn (double wh[HIDDENNO][INPUTNO + 1],
			 double wo[HIDDENNO + 1],
			 double hi[], double e[INPUTNO + 1], double o);
int getdata (double **in_data, double *ans);
void olearn (double wo[HIDDENNO + 1],
			 double hi[], double e[INPUTNO + 1], double o);

double affine (double *o, double **wh, double *e,
			   int output_size, int input_size);
double relu    (double *o, double *e, int input_size);
double softmax (double *o, double *e, int input_size);
void forward (double *o, double *e, int input_size, int output_size);

void print (double wh[HIDDENNO][INPUTNO + 1],
			double wo[HIDDENNO + 1]);
void initwh(double wh[HIDDENNO][INPUTNO + 1]);
void initwo (double wo[HIDDENNO + 1]);
double drnd (void);
double f (double u);


int main ()
{
  double wh[HIDDENNO][INPUTNO + 1];
  double wo[HIDDENNO + 1];
  double hi[HIDDENNO + 1];
  double **in_data = (double **)malloc(sizeof(int) * MAXINPUTNO * INPUTNO);
  double *ans_data = (double  *)malloc(sizeof(int) * MAXINPUTNO);
  double o[OUTPUTNO];
  double err = BIGNUM;
  int i, j;
  int n_of_e;
  int count = 0;

  srand (SEED);

  initwh (wh);
  initwo (wo);
  print (wh, wo);    // Print result

  n_of_e = getdata (in_data, ans_data);
  printf ("No of learning data: %d\n", n_of_e);

  while (err > LIMIT) {
	err = 0.0;
	for (int no_input = 0; no_input < n_of_e; no_input++) {
	  double **wh0 = (double **)malloc(sizeof(double) * HIDDENNO * INPUTNO);
	  double **wb0 = (double **)malloc(sizeof(double) * HIDDENNO);

	  double **wh1 = (double **)malloc(sizeof(double) * OUTPUTNO * HIDDENNO);

	  double af0[HIDDENNO];
	  double af1[OUTPUTNO];
	  double rel0[HIDDENNO];
	  double rel1[HIDDENNO];
	  affine (af0, in_data[no_input], wh0, wb0, HIDDENNO, INPUTNO);
	  relu (rel0, af0, HIDDENNO);
	  affine (af1, wh1, rel0, OUTPUTNO, HIDDENNO);
	  relu (rel1, af1, OUTPUTNO);
	  
	  // forward (o, e[no_input], OUTPUTNO, INPUTNO);
	  for (int err_idx = 0; err_idx < OUTPUTNO; err_idx++) {
		err += (o[err_idx] - in_data[j][err_idx]) * (o[err_idx] - in_data[j][err_idx]);
	  }
	  // olearn (wo, HIDDENNO, rel0, af0, ans_data, rel0);
	}
	count++;
	fprintf (stderr, "%d\t%lf\n", count, err);
  }
  
  print (wh, wo);

  // for (i = 0; i < n_of_e; i++) {
  // 	printf ("%d ", i);
  // 	for (j = 0; j < INPUTNO; j++) {
  // 	  printf ("%lf ", e[i][j]);
  // 	}
  // 	o = forward (wh, wo, hi, e[i]);
  // 	printf ("%lf\n", o);
  // }

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


int getdata (double **in_data, double *ans)
{
  int n_of_e = 0;
  int j = 0;
  while (scanf ("%lf", &in_data[n_of_e][j]) != EOF) {
	j++;
	if (j > INPUTNO) {
	  scanf ("%lf", &ans[n_of_e]);
	  j = 0;
	  n_of_e++;
	}
  }

  return n_of_e;
}


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
			   double **wh, double *b,
			   int output_size, int input_size)
{
  for (int i = 0; i < output_size; i++) {
	o[i] = 0;
	for (int j = 0; j < input_size; j++) {
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


void print (double wh[HIDDENNO][INPUTNO + 1],
			double wo[HIDDENNO + 1])
{
  int i, j;
  for (i = 0; i < HIDDENNO; i++) {
	for (j = 0;j < INPUTNO + 1; j++) {
	  printf ("%lf ", wh[i][j]);
	}
  }
  printf ("\n");
  for (i = 0; i < HIDDENNO + 1; i++) {
	printf ("%lf\n", wo[i]);
  }
  printf ("\n");
}


void initwh(double wh[HIDDENNO][INPUTNO + 1])
{
  for (int i = 0; i < HIDDENNO; i++) {
	for (int j = 0; j < INPUTNO + 1; j++) {
	  wh[i][j] = drnd ();
	}
  }
}


void initwo (double wo[HIDDENNO + 1])
{
  for (int i = 0; i < HIDDENNO + 1; i++) {
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


		  
