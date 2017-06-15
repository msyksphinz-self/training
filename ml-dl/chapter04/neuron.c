#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTNO 2
#define MAXINPUTNO 100

double f (double u);
void initw (double w[INPUTNO+1]);
double forward (double w[INPUTNO + 1],
                double e[INPUTNO]);
int getdata(double e[][INPUTNO]);

int main()
{
  double w[INPUTNO+1];
  double e[MAXINPUTNO][INPUTNO];
  double o;
  int i, j;
  int n_of_e;

  initw(w);

  n_of_e = getdata(e);
  printf ("No of Data: %d\n", n_of_e);

  for (i = 0; i < n_of_e; i++) {
    printf ("%d ", i);
    for (j = 0; j < INPUTNO; j++) {
      printf ("%lf ", e[i][j]);
    }
    o = forward(w, e[i]);
    printf ("%lf\n", o);
  }

  return 0;
}


int getdata(double e[][INPUTNO])
{
  int no_of_e = 0;
  int j = 0;

  while (scanf("%lf", &e[no_of_e][j]) != EOF) {
    j++;
    if (j >= INPUTNO) {
      j = 0;
      ++no_of_e;
    }
  }
  return no_of_e;
}


double forward (double w[INPUTNO+1], double e[INPUTNO])
{
  int i;
  double u, o;

  u = 0;
  for (i = 0; i < INPUTNO; i++) {
    u += e[i] * w[i];
  }
  u -= w[i];
  o = f(u);
  return o;
}


void initw (double w[INPUTNO+1])
{
  w[0] = 1;
  w[1] = 1;
  w[2] = 1.5;
}

double f(double u)
{
  if (u >= 0) return 1.0;
  else        return 0.0;
}
