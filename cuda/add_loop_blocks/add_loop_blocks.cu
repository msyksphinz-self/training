#include "./common/book.h"

#define N 10


__global__ void add (int *a, int *b, int *c)
{
    int tid = threadIdx.x;

    if (tid < N) {
        c[tid] = a[tid] + b[tid];
    }
}



int main (void)
{
    int a[N], b[N], c[N];
    int *dev_a, *dev_b, *dev_c;

    // Allocate GPU's memory
    HANDLE_ERROR (cudaMalloc ((void **)&dev_a, N * sizeof (int)));
    HANDLE_ERROR (cudaMalloc ((void **)&dev_b, N * sizeof (int)));
    HANDLE_ERROR (cudaMalloc ((void **)&dev_c, N * sizeof (int)));

    // Set Array a and b by CPU
    for (int i = 0; i < N; i++) {
        a[i] = -i;
        b[i] = i * i;
    }

    // Copy Array a and b into GPU
    HANDLE_ERROR (cudaMemcpy (dev_a, a, N * sizeof (int),
                              cudaMemcpyHostToDevice));
    HANDLE_ERROR (cudaMemcpy (dev_b, b, N * sizeof (int),
                              cudaMemcpyHostToDevice));

    add<<<1, N>>> (dev_a, dev_b, dev_c);

    // Copy Array c into CPU
    HANDLE_ERROR (cudaMemcpy (c, dev_c, N * sizeof(int),
                              cudaMemcpyDeviceToHost));

    // Display result
    for (int i = 0; i < N; i++) {
        printf ("%d + %d = %d\n", a[i], b[i], c[i]);
    }

    // Free GPU's memory
    HANDLE_ERROR (cudaFree (dev_a));
    HANDLE_ERROR (cudaFree (dev_b));
    HANDLE_ERROR (cudaFree (dev_c));

    return 0;
}
