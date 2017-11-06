#define INF 2e10f
#include "./raytrace.h"

float hit(__global struct Sphere *s, float ox, float oy, float *n)
{
	float dx = ox - s->x;
	float dy = oy - s->y;

	float radius = s->radius;

	if (dx*dx + dy*dy < radius * radius) {
		float dz = sqrt (radius * radius - dx*dx - dy*dy);
		*n = dz / sqrt (radius * radius);
		return dz + s->z;
	}
	return -INF;
}

__kernel void calc_sphere(__global struct Sphere *s, __global unsigned char *ptr)
{
	int x = get_group_id(0);
	int y = get_local_id(0);
	
	int offset = x + y * IMAGE_WIDTH;

	float ox = (x - IMAGE_WIDTH / 2);
	float oy = (y - IMAGE_WIDTH / 2);

	float r = 0, g = 0, b = 0;
	float maxz = -INF;

	for (int i = 0; i < SPHERES; i++) {
		float n;
		float t = hit(&(s[i]), ox, oy, &n);
//		if (x == IMAGE_WIDTH/2 &&
//		    y == IMAGE_WIDTH/2) {
//			printf("s[i].r = %f s[i].g = %f, s[i].b = %f\n", s[i].r, s[i].g, s[i].b);
//		}
		if (t > maxz) {
			float fscale = n;
			r = s[i].r * fscale;
			g = s[i].g * fscale;
			b = s[i].b * fscale;
			maxz = t;
		}
	}
	ptr[offset * 4 + 0] = (int)(r * 255);
	ptr[offset * 4 + 1] = (int)(g * 255);
	ptr[offset * 4 + 2] = (int)(b * 255);
	ptr[offset * 4 + 3] = 255;
	
//	if (x == IMAGE_WIDTH/2 &&
//	    y == IMAGE_WIDTH/2) {
//		printf("r=%f, g=%f, b=%f\n",r, g, b);
//	}
}
