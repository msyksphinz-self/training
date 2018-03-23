/*************************************************/
/* layers.h                                      */
/* cifar10 Layers                                */
/*************************************************/

#ifndef __LAYERS_H__
#define __LAYERS_H__

double affine (const int output_size,
			   const int input_size,
			   const int batch_size,
			   double *out,           // [batch_size][output_size],
			   const double *in_data, // [batch_size][input_size],
			   const double *wh,      // [input_size][output_size],
			   const double *wb);      // [output_size]
double affine_backward (const int output_size,
						const int hidden_size,
						const int batch_size,
						double *dx,  // [batch_size][output_size],
						double *db,  // [output_size],
						double *dw,  // [output_size][hidden_size],
						const double *dout, // [batch_size][output_size],
						const double *w,    // [hidden_size][output_size],
						const double *x);   // [batch_size][hidden_size],
void relu (const int batch_size,
		   const int size,
		   double *o,         // [batch_size][size],
		   const double *e);  // [batch_size][size]
double relu_backward (const int batch_size,
					  const int size,
					  double       *dx,  // [batch_size][size],
					  const double *x,   // [batch_size][size],
					  const double *dout); // [batch_size][size]);

double softmax (const int batch_size,
				const int size,
				double       *o,  // [batch_size][size],
				const double *e); // [batch_size][size]
double softmax_backward (const int batch_size,
						 const int size,
						 double       *dx,   // [batch_size][size],
						 const double *y,    // [batch_size][size],
						 const double *t);   // [batch_size][size])

int argmax (const int x_size, double *o);



#endif // __LAYERS_H__

