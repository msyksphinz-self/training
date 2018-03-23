/*************************************************/
/* layers.c                                      */
/* cifar10 Layers                                */
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "layers.h"

double affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   double *out,            // [batch_size][output_size],
			   const double *in_data,  // [batch_size][input_size],
			   const double *wh,       // [input_size][output_size],
			   const double *wb)       // [output_size]
{
  for (int b = 0; b < batch_size; b++) {
  	for (int o = 0; o < output_size; o++) {
  	  out[b * output_size + o] = 0.0;
  	  for (int i = 0; i < input_size; i++) {
		double tmp = in_data[b * input_size + i] * wh[i * output_size + o];
  	  	out[b * output_size + o] = out[b * output_size + o] + tmp;
  	  }
  	  out[b * output_size + o] = out[b * output_size + o] + wb[o];
  	}
  }
}


double affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						double *dx,  // [batch_size][hidden_size],
						double *db,  // [output_size],
						double *dw,  // [hidden_size][output_size],
						const double *dout,  // [batch_size][output_size],
						const double *w,     // [hidden_size][output_size],
						const double *x)     // [batch_size][hidden_size],
{
  for (int b = 0; b < batch_size; b++) {
	for (int h = 0; h < hidden_size; h++) {
	  dx[b * hidden_size + h] = 0.0;
	  for (int o = 0;o < output_size; o++) {
		dx[b * hidden_size + h] = dx[b * hidden_size + h] + dout[b * output_size + o] * w[h * output_size + o];
	  }
	}
  }
  for (int h = 0; h < hidden_size; h++) {
	for (int o = 0; o < output_size; o++) {
	  dw[h * output_size + o] = 0.0;
	  for (int b = 0; b < batch_size; b++) {
		dw[h * output_size + o] = dw[h * output_size + o] + x[b * hidden_size + h] * dout[b * output_size + o];
	  }
	}
  }

  for (int o = 0; o < output_size; o++) {
	db[o] = 0.0;
	for (int b = 0; b < batch_size; b++) {
	  db[o] = db[o] + dout[b * output_size + o];
	}
  }
}


void relu (const int batch_size,
		   const int size,
		   double *o,        // [batch_size][size],
		   const double *e)  // [batch_size][size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  o[b * size + i] = e[b * size + i] > 0 ? e[b * size + i] : 0;
	}
  }
  return;
}


double relu_backward (const int batch_size,
					  const int size,
					  double       *dx,     // [batch_size][size],
					  const double *x,      // [batch_size][size],
					  const double *dout)   // [batch_size][size])
{
  for (int b = 0; b < batch_size; b++) {
	for (int i = 0; i < size; i++) {
	  dx[b * size + i] = x[b * size + i] > 0 ? dout[b * size + i] : 0;
	}
  }
}


double softmax (const int batch_size,
                 const int size,
                 double *o,       // [batch_size][size],
                 const double *e) // e[batch_size][size]
{
  double *max = (double *)malloc(sizeof(double) * batch_size);
  for (int b = 0; b < batch_size; b++) {
	max[b] = e[b * size + 0];
	for (int i = 1; i < size; i++) {
	  max[b] = max[b] < e[b * size + i] ? e[b * size + i] : max[b];
	}
  }

  for (int b = 0; b < batch_size; b++) {
	double exp_sum = 0;
	double *a = (double *)malloc(sizeof(double) * size);
	for (int i = 0; i < size; i++) {
      a[i] = e[b * size + i] - max[b];
	  exp_sum = exp_sum + exp(a[i]);
	}
	for (int i = 0; i < size; i++) {
	  o[b * size + i] = exp(a[i]) / exp_sum;
	}
    free (a);
  }


  free (max);
}


double softmax_backward (const int batch_size,
                          const int size,
                          double        *dx,  // [batch_size][size],
                          const double  *y,  // [batch_size][size],
                          const double  *t)  // [batch_size][size]
{
  for (int b = 0; b < batch_size; b++) {
	for (int y_idx = 0; y_idx < size; y_idx++) {
	  dx[b * size + y_idx] = (y[b * size + y_idx] - t[b * size + y_idx]) / batch_size;
	}
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

