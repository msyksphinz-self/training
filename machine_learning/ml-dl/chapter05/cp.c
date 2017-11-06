#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTSIZE   (11)    /* number of input */
#define FILTERSIZE  (3)     /* size of filter */
#define POOLSIZE    (3)     /* Pooling size */
#define POOLOUTSIZE (3)     /* Pooling output size */

void poolres (double poolout[][POOLOUTSIZE]);
void pool(double convout[][INPUTSIZE],
		  double poolout[][POOLOUTSIZE]);
double maxpooling (double convout[][INPUTSIZE],
				   int i, int j);
void convres (double convout[][INPUTSIZE]);
void getdata (double e[][INPUTSIZE]);
void conv (double filter[][FILTERSIZE],
		   double e[][INPUTSIZE], double convout[][INPUTSIZE]);
double calcconv (double filter[][FILTERSIZE],
				 double [][INPUTSIZE], int i, int j);


int main ()
{
  double filter[FILTERSIZE][FILTERSIZE] = {{0, 1, 0},
										   {0, 1, 0},
										   {0, 1, 0}};
  double e[INPUTSIZE][INPUTSIZE];
  double convout [INPUTSIZE][INPUTSIZE] = {0};
  double poolout [POOLOUTSIZE][POOLOUTSIZE];

  getdata (e);

  /* Calculation of convolution */
  conv (filter, e, convout);
  convres (convout);

  /* Calculation of pooling */
  pool (convout, poolout);
  poolres (poolout);

  return 0;
}


void poolres (double poolout[][POOLOUTSIZE])
{
  for (int i = 0; i < POOLOUTSIZE; i++) {
	for (int j = 0; j < POOLOUTSIZE; j++) {
	  printf ("%.3lf ", poolout[i][j]);
	}
	printf ("\n");
  }
  printf ("\n");
}


void pool(double convout[][INPUTSIZE],
		  double poolout[][POOLOUTSIZE])
{
  for (int i = 0; i < POOLOUTSIZE; i++) {
	for (int j = 0; j < POOLOUTSIZE; j++) {
	  poolout[i][j] = maxpooling (convout, i, j);
	}
  }
}


double maxpooling (double convout[][INPUTSIZE],
				   int i, int j)
{
  double max;
  int halfpool = POOLSIZE / 2;

  max = convout[i * POOLSIZE + 1 + halfpool][j * POOLOUTSIZE + 1 + halfpool];
  for (int m = POOLOUTSIZE * i + 1; m <= POOLOUTSIZE * i + 1 + (POOLSIZE - halfpool); m++) {
	for (int n = POOLOUTSIZE * j + 1; n <= POOLOUTSIZE * j + 1 + (POOLSIZE - halfpool); n++) {
	  if (max < convout[m][n]) max = convout[m][n];
	}
  }

  return max;
}


void convres (double convout[][INPUTSIZE])
{
  for (int i = 1; i < INPUTSIZE-1; i++) {
	for (int j = 1; j < INPUTSIZE-1; j++) {
	  printf ("%.3lf ", convout[i][j]);
	}
	printf ("\n");
  }
  printf ("\n");
}


void getdata (double e[][INPUTSIZE])
{
  int i = 0, j = 0;

  while (scanf ("%lf", &e[i][j]) != EOF) {
	j++;
	if (j >= INPUTSIZE) {
	  j = 0;
	  i++;
	  if (i >= INPUTSIZE) break;
	}
  }
}


void conv (double filter[][FILTERSIZE],
		   double e[][INPUTSIZE], double convout[][INPUTSIZE])
{
  int startpoint = FILTERSIZE / 2;
  for (int i = startpoint; i < INPUTSIZE - startpoint; i++) {
	for (int j = startpoint; j < INPUTSIZE - startpoint; j++) {
	  convout[i][j] = calcconv(filter, e, i, j);
	}
  }
}


double calcconv (double filter[][FILTERSIZE],
				 double e[][INPUTSIZE], int i, int j)
{
  double sum = 0;
  for (int m = 0; m < FILTERSIZE; m++) {
	for (int n = 0; n < FILTERSIZE; n++) {
	  sum += e[i - FILTERSIZE/2+m][j - FILTERSIZE/2+n] * filter[m][n];
	}
  }

  return sum;
}

