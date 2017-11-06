#include "./common/book.h"

#define imin(a,b) (a<b?a:b)

const int N = 33 * 1024;
const int threadsPerBlock = 256;
const int blocksPerGrid = imin(32, (N+threadsPerBlock-1)/threadsPerBlock);

__global__ void dot (float *a, float *b, float *c)
{
    __shared__ float cache[threadsPerBlock];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int cacheIndex = threadIdx.x;

    float temp = 0;
    while (tid < N) {
        temp += a[tid] * b[tid];
        tid += blockDim.x * gridDim.x;
    }

    // Set Cache
    cache[cacheIndex] = temp;
    // synchronize threads in the block
    __syncthreads ();

    // This code, threadsPerBlock should be power of 2
    int i = blockDim.x / 2;
    while (i != 0) {
        if (cacheIndex < i) {
            cache[cacheIndex] += cache[cacheIndex+i];
        }
        __syncthreads();
        i /= 2;
    }

    if (cacheIndex == 0) {
        c[blockIdx.x] = cache[0];
    }
}

int main (void)
{
    float *a, *b, c, *partial_c;
    float *dev_a, *dev_b, *dev_partial_c;

    // allocate memory in CPU
    a = (float *)malloc(N * sizeof (float));
    b = (float *)malloc(N * sizeof (float));
    partial_c = (float *)malloc(blocksPerGrid * sizeof (float));

    // Allocate GPU's memory
    HANDLE_ERROR (cudaMalloc ((void **)&dev_a,
                              N * sizeof (float)));
    HANDLE_ERROR (cudaMalloc ((void **)&dev_b,
                              N * sizeof (float)));
    HANDLE_ERROR (cudaMalloc ((void **)&dev_partial_c,
                              N * sizeof (float)));

    // Set memory data by Host
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Copy Array a & b into GPU
    HANDLE_ERROR (cudaMemcpy (dev_a,
                              a,
                              N * sizeof (float),
                              cudaMemcpyHostToDevice));
    HANDLE_ERROR (cudaMemcpy (dev_b,
                              b,
                              N * sizeof (float),
                              cudaMemcpyHostToDevice));

    dot<<<blocksPerGrid, threadsPerBlock>>> (dev_a,
                                             dev_b,
                                             dev_partial_c);

    // copy c from GPU into CPU
    HANDLE_ERROR (cudaMemcpy (partial_c,
                              dev_partial_c,
                              blocksPerGrid * sizeof (float),
                              cudaMemcpyDeviceToHost));

    // Final calculation ni CPU
    c = 0;
    for (int i = 0; i < blocksPerGrid; i++) {
        c += partial_c[i];
    }

#define sum_squares(x) (x*(x+1)*(2*x+1)/6)
    printf ("Des GPU value %.6g = %.6g?\n", c,
            2 * sum_squares ((float)(N-1)));

    // Free GPU's memory
    HANDLE_ERROR (cudaFree (dev_a));
    HANDLE_ERROR (cudaFree (dev_b));
    HANDLE_ERROR (cudaFree (dev_partial_c));

    // Free CPU's memory
    free (a);
    free (b);
    free (partial_c);

    return 0;

}
