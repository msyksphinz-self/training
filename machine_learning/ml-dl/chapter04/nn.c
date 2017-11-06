#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTNO    2
#define HIDDENNO   2
#define MAXINPUTNO 100

double f (double u);
void initwh (double wh[HIDDENNO][INPUTNO+1]);
void initwo (double wo[HIDDENNO + 1]);
double forward (double wh[HIDDENNO][INPUTNO + 1],
                double wo[HIDDENNO + 1], double hi[],
                double e[INPUTNO]);
int getdata(double e[][INPUTNO]);

int main ()
{
  double wh[HIDDENNO][INPUTNO+1];
  double wo[HIDDENNO + 1];
  double e[MAXINPUTNO][INPUTNO];
  double hi[HIDDENNO + 1];
  double o;
  int i, j;
  int n_of_e;

  initwh(wh);
  initwo(wo);

  n_of_e = getdata (e);
  printf ("No of Data : %d\n", n_of_e);

  for (i = 0; i < n_of_e; i++) {
    printf ("%d ", i);
    for (j = 0; j < INPUTNO; j++) {
      printf ("%lf ", e[i][j]);
    }
    o = forward (wh, wo, hi, e[i]);
    printf ("%lf\n", o);
  }

  return 0;
}


int getdata (double e[][INPUTNO])
{
  int n_of_e = 0;
  int j = 0;

  while (scanf("%lf", &e[n_of_e][j]) != EOF) {
    j++;
    if (j >= INPUTNO) {
      j = 0;
      ++n_of_e;
    }
  }
  return n_of_e;
}


double forward (double wh[HIDDENNO][INPUTNO + 1],
                double wo[HIDDENNO + 1], double hi[],
                double e[INPUTNO])
{
  int i, j;
  double u;
  double o;

  for (i = 0; i < HIDDENNO; i++) {
    u = 0;
    for (j = 0; j < INPUTNO; j++) {
      u += e[j] * wh[i][j];
    }
    u -= wh[i][j];
    hi[i] = f(u);
  }
  o = 0;
  for (i = 0; i < HIDDENNO; i++) {
    o += hi[i] * wo[i];
  }

  return f(o);
}


void initwh (double wh[HIDDENNO][INPUTNO + 1])
{
  wh[0][0] = -2;
  wh[0][1] = 3;
  wh[0][2] = -1;
  wh[1][0] = -2;
  wh[1][1] = 1;
  wh[1][2] = 0.5;
}


void initwo (double wo[HIDDENNO + 1])
{
  wo[0] = -60;
  wo[1] = 94;
  wo[2] = -1;
}


double f(double u)
{
  if (u >= 0) return 1.0;
  else        return 0.0;
}


