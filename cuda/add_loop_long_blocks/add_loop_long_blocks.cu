#include "./common/book.h"

#define N (33 * 1024)

__global__ void add (int *a, int *b, int *c)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    while (tid < N) {
        c[tid] = a[tid] + b[tid];
        // tid += blockDim.x * gridDim.x;
        tid += blockDim.x * gridDim.x;
    }
}

int main (void)
{
    int *a, *b, *c;
    int *dev_a, *dev_b, *dev_c;

    // Allocate CPU's memory
    a = (int *) malloc (N * sizeof (int));
    b = (int *) malloc (N * sizeof (int));
    c = (int *) malloc (N * sizeof (int));

    // Allocate GPU's memory
    HANDLE_ERROR (cudaMalloc ((void **)&dev_a,
                              N * sizeof (int)));
    HANDLE_ERROR (cudaMalloc ((void **)&dev_b,
                              N * sizeof (int)));
    HANDLE_ERROR (cudaMalloc ((void **)&dev_c,
                              N * sizeof (int)));

    // Set Array a & b
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }

    // Copy Array a & b into GPU
    HANDLE_ERROR (cudaMemcpy (dev_a,
                              a,
                              N * sizeof (int),
                              cudaMemcpyHostToDevice));
    HANDLE_ERROR (cudaMemcpy (dev_b,
                              b,
                              N * sizeof (int),
                              cudaMemcpyHostToDevice));

    add <<<128, 128>>> (dev_a, dev_b, dev_c);

    // Copy Array c into CPU
    HANDLE_ERROR (cudaMemcpy (c,
                              dev_c,
                              N * sizeof (int),
                              cudaMemcpyDeviceToHost));

    // check GPU did it that requested from CPU
    bool success = true;
    for (int i = 0; i < N; i++) {
        if ((a[i] + b[i]) != c[i]) {
            printf ("Error: %d + %d != %d\n", a[i], b[i], c[i]);
            success = false;
        }
    }
    if (success) printf ("We did it!\n");

    // Free GPU's allocated memory
    HANDLE_ERROR (cudaFree (dev_a));
    HANDLE_ERROR (cudaFree (dev_b));
    HANDLE_ERROR (cudaFree (dev_c));

    // Free CPU's allocated memory
    free (a);
    free (b);
    free (c);

    return 0;
}
