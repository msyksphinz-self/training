#include "cuda.h"
#include "./common/book.h"
#include "./common/cpu_bitmap.h"

#define INF 2e10f
#define DIM 1024
#define rnd(x) (x * rand() / RAND_MAX)
#define SPHERES (100)


struct Sphere {
    float r, b, g;
    float radius;
    float x, y, z;
    __device__ float hit (float ox, float oy, float *n) {
        float dx = ox - x;
        float dy = oy - y;
        if ((dx * dx + dy * dy) < (radius*radius)) {
            float dz = sqrtf (radius*radius - dx*dx - dy*dy);
            *n = dz / sqrtf (radius * radius);
            return dz + z;
        }
        return -INF;
    }
};


__constant__ Sphere s[SPHERES];

__global__ void kernel (unsigned char *ptr)
{
    // Pixel mapping from threadIdx/BlockIdx
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset  = x + y * blockDim.x * gridDim.x;

    float ox = (x - DIM/2);
    float oy = (y - DIM/2);

    float r = 0, g = 0, b = 0;
    float maxz = -INF;
    for (int i = 0; i < SPHERES; i++) {
        float n;
        float t = s[i].hit (ox, oy, &n);
        if (t > maxz) {
            float fscale = n;
            r = s[i].r * fscale;
            g = s[i].g * fscale;
            b = s[i].b * fscale;
            maxz = t;
        }
    }

    ptr[offset * 4 + 0] = (int) (r * 255);
    ptr[offset * 4 + 1] = (int) (g * 255);
    ptr[offset * 4 + 2] = (int) (b * 255);
    ptr[offset * 4 + 3] = 255;
}


struct DataBlock {
    unsigned char *dev_bitmap;
    Sphere *s;
};


int main (void)
{
    DataBlock data;
    cudaEvent_t start, stop;
    HANDLE_ERROR (cudaEventCreate (&start));
    HANDLE_ERROR (cudaEventCreate (&stop));
    HANDLE_ERROR (cudaEventRecord (start, 0));

    CPUBitmap bitmap (DIM, DIM, &data);
    unsigned char *dev_bitmap;

    // Memory allocation in GPU side
    HANDLE_ERROR (cudaMalloc ((void **)&dev_bitmap,
                              bitmap.image_size()));

    // Allocate temporary memory, format, Copy into GPU memory, and free
    Sphere *temp_s = (Sphere *)malloc(sizeof (Sphere) * SPHERES);
    for (int i = 0; i < SPHERES; i++) {
        temp_s[i].r = rnd(1.0f);
        temp_s[i].g = rnd(1.0f);
        temp_s[i].b = rnd(1.0f);
        temp_s[i].x = rnd(1000.0f) - 500;
        temp_s[i].y = rnd(1000.0f) - 500;
        temp_s[i].z = rnd(1000.0f) - 500;
        temp_s[i].radius = rnd (100.0f) + 20;
    }

    HANDLE_ERROR (cudaMemcpyToSymbol (s, temp_s,
                                      sizeof (Sphere) * SPHERES));
    free (temp_s);

    dim3 grids(DIM/16, DIM/16);
    dim3 threads(16, 16);
    kernel<<<grids, threads>>>(dev_bitmap);

    // Copy Bitmap from GPU
    HANDLE_ERROR (cudaMemcpy (bitmap.get_ptr(),
                              dev_bitmap,
                              bitmap.image_size(),
                              cudaMemcpyDeviceToHost));


    HANDLE_ERROR (cudaEventRecord (stop, 0));
    HANDLE_ERROR (cudaEventSynchronize (stop));

    float elapsedTime;
    HANDLE_ERROR (cudaEventElapsedTime (&elapsedTime,
                                        start,
                                        stop));
    printf ("Time to generate: %3.1f ms\n", elapsedTime);

    HANDLE_ERROR (cudaEventDestroy (start));
    HANDLE_ERROR (cudaEventDestroy (stop));

    // Free memory
    HANDLE_ERROR (cudaFree (dev_bitmap));

    // Dispay bitmap
    bitmap.display_and_exit ();

    return 0;
}
