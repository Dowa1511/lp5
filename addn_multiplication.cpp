#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>
#include <stdbool.h>

__global__ void vectorAdd(int* a, int* b, int* c, int size)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < size) {
        c[tid] = a[tid] + b[tid];
    }
}
void vectorAddNormal(int* a, int* b, int* c, int size) {
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}
// compare results
bool verifyResults(int* c_cuda, int* c_normal, int size) {
    for (int i = 0; i < size; i++) {
        if (c_cuda[i] != c_normal[i]) {
            return false;
        }
    }
    return true;
}

int main()
{
    int size = 1000000;
    int* a, * b, * c_cuda, * c_normal;
    int* dev_a, * dev_b, * dev_c;

    // Allocate memory for CPU vectors
    a = (int*)malloc(size * sizeof(int));
    b = (int*)malloc(size * sizeof(int));
    c_cuda = (int*)malloc(size * sizeof(int));
    c_normal = (int*)malloc(size * sizeof(int));

    // Generate random vector
    for (int i = 0; i < size; i++) {
        a[i] = rand() % 1000;
        b[i] = rand() % 1000;
    }

    cudaMalloc((void**)&dev_a, size * sizeof(int));
    cudaMalloc((void**)&dev_b, size * sizeof(int));
    cudaMalloc((void**)&dev_c, size * sizeof(int));

    cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);

    //GPU
    clock_t start_cuda, end_cuda;
    start_cuda = clock();

    int blockSize = 256;
    int gridSize = (size + blockSize - 1) / blockSize;
    vectorAdd<<<gridSize, blockSize>>>(dev_a, dev_b, dev_c, size);
    cudaDeviceSynchronize();

    end_cuda = clock();
    double cuda_time = (double)(end_cuda - start_cuda) / CLOCKS_PER_SEC;

    printf("Time using GPU: %f", cuda_time);

    cudaMemcpy(c_cuda, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    //CPU
    clock_t start_normal, end_normal;
    start_normal = clock();
    vectorAddNormal(a, b, c_normal, size);
    end_normal = clock();
    double normal_time = (double)(end_normal - start_normal) / CLOCKS_PER_SEC;

    printf("\nTIme using CPU: %f ", normal_time);

    bool is_correct = verifyResults(c_cuda, c_normal, size);
    if(is_correct)
        printf("\nOutput Match: True");
    else
        printf("\nOutput Match: False");

    double speedup = normal_time / cuda_time;
    printf("\nSpeedup factor: %f\n", speedup);

    free(a);
    free(b);
    free(c_cuda);
    free(c_normal);

    return 0;
}
---------------------------------------------------------------

#matrix multiplication


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>
#include <stdbool.h>

void matrix_mult(int* a, int* b, int* c, int rowsA, int colsA, int colsB) {
    for (int row = 0; row < rowsA; row++) {
        for (int col = 0; col < colsB; col++) {
            int sum = 0;
            for (int i = 0; i < colsA; i++) {
                sum += a[row * colsA + i] * b[i * colsB + col];
            }
            c[row * colsB + col] = sum;
        }
    }
}

__global__ void matrixMul(int* a, int* b, int* c, int rowsA, int colsA, int colsB) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int sum = 0;
    if (row < rowsA && col < colsB) {
        for (int i = 0; i < colsA; i++) {
            sum += a[row * colsA + i] * b[i * colsB + col];
        }
        c[row * colsB + col] = sum;
    }
}


// Compare results
bool verifyMatrixResults(int* c_cuda, int* c_normal, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        if (c_cuda[i] != c_normal[i]) {
            return false;
        }
    }
    return true;
}
int main() {
    int rowsA = 1000;
    int colsA = 1000;
    int rowsB = 1000;
    int colsB = 1000;

    int *a, *b, *c_cuda, *c_normal;
    int *dev_a, *dev_b, *dev_c;

    a = (int*)malloc(rowsA * colsA * sizeof(int));
    b = (int*)malloc(rowsB * colsB * sizeof(int));
    c_cuda = (int*)malloc(rowsA * colsB * sizeof(int));
    c_normal = (int*)malloc(rowsA * colsB * sizeof(int));

    for (int i = 0; i < rowsA * colsA; i++) {
        a[i] = rand() % 100;
    }
    for (int i = 0; i < rowsB * colsB; i++) {
        b[i] = rand() % 100;
    }

    cudaMalloc((void**)&dev_a, rowsA * colsA * sizeof(int));
    cudaMalloc((void**)&dev_b, rowsB * colsB * sizeof(int));
    cudaMalloc((void**)&dev_c, rowsA * colsB * sizeof(int));

    cudaMemcpy(dev_a, a, rowsA * colsA * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, rowsB * colsB * sizeof(int), cudaMemcpyHostToDevice);

    clock_t start_cuda = clock();

    dim3 blockSize(16, 16);
    dim3 gridSize((colsB + blockSize.x - 1) / blockSize.x, (rowsA + blockSize.y - 1) / blockSize.y);
    matrixMul<<<gridSize, blockSize>>>(dev_a, dev_b, dev_c, rowsA, colsA, colsB);
    cudaMemcpy(c_cuda, dev_c, rowsA * colsB * sizeof(int), cudaMemcpyDeviceToHost);

    clock_t end_cuda = clock();
    double cuda_time = (double)(end_cuda - start_cuda) / CLOCKS_PER_SEC;
    printf("Time Taken GPU : %f", cuda_time);

    clock_t start_normal = clock();
    matrix_mult(a, b, c_normal, rowsA, colsA, colsB);
    clock_t end_normal = clock();
    double normal_time = (double)(end_normal - start_normal) / CLOCKS_PER_SEC;
    printf("\nTime Taken CPU : %f ", normal_time);

    bool match = verifyMatrixResults(c_cuda, c_normal, rowsA, colsB);
    printf("\nOutput Match: %s", match ? "True" : "False");

    double speedup = normal_time / cuda_time;
    printf("\nSpeedup Factor: %f\n", speedup);

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
    free(a);
    free(b);
    free(c_cuda);
    free(c_normal);

    return 0;
}
