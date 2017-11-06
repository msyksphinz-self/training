#define INF 2e10f
#include "./raytrace.h"

const float fscale (1.5f)


float hit(Sphere *s, float ox, float oy, float *n)
{
	float dx = ox - s->x;
	float dy = oy - s->y;

	float radius = s->radius;

	if (dx*dx + dy*dy < radius * radius) {
		float dz = sqrtf(radius * radius - dx*dx - dy*dy);
		*n = dz / sqrtf(radius * radius);
		return dz + s->z;
	}
	return -INF;
}

__global__ void kernel(Sphere *s, unsigned char *ptr)
{
	int x = get_global_id(0);
	int y = get_global_id(1);
	int offset = x + y *IMAGE_WIDTH;

	float ox = (x - IMAGE_WIDTH / 2);
	float oy = (y - IMAGE_WIDTH / 2);

	float r = 0, g = 0, b = 0;
	float maxz = -INF;

	for (int i = 0; i < SPHERES; i++) {
		float n;
		float t = hit(s[i], ox, oy, &n);
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
}
