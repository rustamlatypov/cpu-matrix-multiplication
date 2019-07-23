#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <cuda_runtime.h>
#include <cstdlib>
#include "cp.h"
using namespace std;



inline void check(cudaError_t err, const char* context) {
    if (err != cudaSuccess) {
        std::cerr << "CUDA error: " << context << ": "
            << cudaGetErrorString(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
#define CHECK(x) check(x, #x)



inline int static divup(int a, int b) {
    return (a + b - 1)/b;
}



static void print(int ny, int nx, const float *matrix) {
    for(int j = 0; j < ny; j++) {
        for(int i = 0; i < nx; i++) {
            float x = matrix[j*nx+i];
            if(std::abs(x) < 10.0) {
                printf("% -7.3f", x);
            } else {
                printf("% -7.0e", x);
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}



__global__ void mykernel(const float* data, float* result, int ny, int nx) 
{
	int j = threadIdx.y + blockIdx.y * blockDim.y;
    int i = threadIdx.x + blockIdx.x * blockDim.x;

	if (i <= j || j >= ny || i >= ny) return;

    double e = 0;

    for (int k = 0; k < nx; k++)
    {
        e += data[j*nx+k] * data[i*nx+k];
    }

    result[j*ny+i] = e;
}




void correlate(int ny, int nx, const float* data, float* result) {

    std::vector<float> aux(ny*nx);

    for (int i = 0; i < ny; i++) {

        float sum;
        float avg;
        float e;

        sum = 0;

        for (int j = 0; j < nx; j++) {
            sum += data[i*nx+j];
        }

        avg = sum/nx;

        for (int j = 0; j < nx; j++) {
            aux[i*nx+j] = data[i*nx+j] - avg;
        }

        sum = 0;

        for (int j = 0; j < nx; j++) {
            e = aux[i*nx+j];
            sum += e*e;
        }

        sum = sqrt(sum);

        for (int j = 0; j < nx; j++) {
            aux[i*nx+j] /= sum;
        }
    }

    float* dataGPU = NULL;
    CHECK(cudaMalloc((void**)&dataGPU, ny * nx * sizeof(float)));

    float* resultGPU = NULL;
    CHECK(cudaMalloc((void**)&resultGPU, ny * ny * sizeof(float)));

    CHECK(cudaMemcpy(dataGPU, aux.data(), ny * nx * sizeof(float), cudaMemcpyHostToDevice));

    // Run kernel
    dim3 dimBlock(16, 16);
    dim3 dimGrid(divup(ny, dimBlock.x), divup(ny, dimBlock.y));
    mykernel<<<dimGrid, dimBlock>>>(dataGPU, resultGPU, ny, nx);
    CHECK(cudaGetLastError());

    // Copy data back to CPU & release memory
    CHECK(cudaMemcpy(result, resultGPU, ny * ny * sizeof(float), cudaMemcpyDeviceToHost));
    CHECK(cudaFree(dataGPU));
    CHECK(cudaFree(resultGPU));
    
	for (int i = 0; i < ny; i++) 
	{
        result[i*ny+i] = 1;
    }
}








