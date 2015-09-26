#pragma once

#define IMAGE_WIDTH (1000)
#define SPHERES (20)

#define sphere_rand(x) ((x/rnd()) / RAND_MAX)

struct Sphere {
	float r, g, b;
	float radius;
	float x, y, z;
};
