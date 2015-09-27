#include "julia_param.h"

int julia(int x, int y);

__kernel void julia_kernel(__global unsigned char *ptr)
{
	int x = get_group_id(0);
	int y = get_local_id(0);

	int offset = y * DIM + x;

	int juliaValue = julia(x, y);

	ptr[offset * 4 + 0] = 255 * juliaValue;
	ptr[offset * 4 + 1] = 0;
	ptr[offset * 4 + 2] = 0;
	ptr[offset * 4 + 3] = 255;
}


float magnitude2(float x, float y)
{
	return x*x + y*y;
}


void array_mult(float a[2], float b[2], float c[2])
{
	c[0] = a[0] * b[0] - a[1] * b[1];
	c[1] = a[1] * b[0] + a[0] * b[1];
	return;
}

void array_add(float a[2], float b[2], float c[2])
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	return;
}


int julia(int x, int y)
{
	const float scale = 1.5;
	float jx = scale * (float)(DIM / 2 - x) / (DIM / 2);
	float jy = scale * (float)(DIM / 2 - y) / (DIM / 2);

	float c[2] = { -0.8, 0.156 };
	float a[2] = { jx, jy };

	int i = 0;
	for (i = 0; i < 200; i++) {
		// a = a * a + c;
		float tmp[2];
		array_mult(a, a, tmp);
		array_add(tmp, c, a);
		if (magnitude2(a[0], a[1]) > 1000) {
			return 0;
		}
	}
	return 1;
}

