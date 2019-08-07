#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <cuda_runtime.h>
#include <cstdlib>
#include "cp.h"
using namespace std;
#include "../common/stopwatch.h"
ppc::stopwatch sw;


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


void preprocess(int ny, int nx, const float* data, float* aux) {

    for (int j = 0; j < ny; j++) {

        float sum;
        float avg;
        float e;

        sum = 0;

        for (int i = 0; i < nx; i++) {
            sum += data[j*nx+i];
        }

        avg = sum/nx;

        for (int i = 0; i < nx; i++) {
            aux[j*nx+i] = data[j*nx+i] - avg;
        }

        sum = 0;

        for (int i = 0; i < nx; i++) {
            e = aux[j*nx+i];
            sum += e*e;
        }

        sum = sqrt(sum);

        for (int i = 0; i < nx; i++) {
            aux[j*nx+i] /= sum;
        }
    }
}


__global__ void mykernel( int ny, int nx, float* data, float* result) 
{
	constexpr int A = 16;
    constexpr int B = 8;

	int jjj = threadIdx.y + blockIdx.y * blockDim.y;
    int iii = threadIdx.x + blockIdx.x * blockDim.x;

    int jj = jjj*A;
    int ii = iii*B;

    if (ii < jj) return;

    float block[A][B] = {0};

    for (int k = 0; k < nx; k++) {
        for (int m = 0; m < A; m++) {

        	float a = data[k*ny + (jj+m)];

            for (int l = 0; l < B; l++) {

                float b = data[k*ny + (ii+l)];
                block[m][l] += a*b;
            }
        }
    }
    
    int M = (jj+A) < ny ? A : ny-jj;
    int L = (ii+B) < ny ? B : ny-ii;

    for (int m = 0; m < M; m++) {
        for (int l = 0; l < L; l++) 
        {
            result[(jj+m)*ny+(ii+l)] = block[m][l];
        }
    }
}


__global__ void myppkernel(int ny, int nx, float* raw) 
{
    int j = threadIdx.x + blockIdx.y*blockDim.x;

    if (j < ny) {

        float sum;
        float avg;
        float e;

        sum = 0;

        for (int i = 0; i < nx; i++) {
            sum += raw[i*ny+j];
        }

        avg = sum/nx;

        for (int i = 0; i < nx; i++) {
            raw[i*ny+j] -= avg;
        }

        sum = 0;

        for (int i = 0; i < nx; i++) {
            e = raw[i*ny+j];
            sum += e*e;
        }

        sum = sqrt(sum);

        for (int i = 0; i < nx; i++) {
            raw[i*ny+j] /= sum;
        }
    }
}


__global__ void mytranskernel(int ny, int nx, float* raw, float* data) 
{
    int j = blockIdx.y;
    int ii = threadIdx.x;

    for (int i = ii; i < nx; i+=64) data[i*ny + j] = raw[j*nx + i];
}


void correlate(int ny, int nx, const float* data, float* result) {

    float* dataGPU = NULL;
    CHECK(cudaMalloc((void**)&dataGPU, nx * ny * sizeof(float)));

    int dim = max(ny,nx);
    float* resultGPU = NULL;
    CHECK(cudaMalloc((void**)&resultGPU, dim * dim * sizeof(float)));

    CHECK(cudaMemcpy(resultGPU, data, ny * nx * sizeof(float), cudaMemcpyHostToDevice));

	// Run transpose kernel
    {
        dim3 dimBlock(64, 1);
        dim3 dimGrid(1, ny);

        mytranskernel<<<dimGrid, dimBlock>>>(ny, nx, resultGPU, dataGPU);
        CHECK(cudaGetLastError());
    }

    // Run preprocess kernel
    {
        dim3 dimBlock(64, 1);
        dim3 dimGrid(1, divup(ny, 64));

        myppkernel<<<dimGrid, dimBlock>>>(ny, nx, dataGPU);
        CHECK(cudaGetLastError());
    }
    
    // Run multiplier kernel
    {
        dim3 dimBlock(8, 4);
        dim3 dimGrid(divup(ny, 64), divup(ny, 64));

        mykernel<<<dimGrid, dimBlock>>>(ny, nx, dataGPU, resultGPU);
        CHECK(cudaGetLastError());
    }

    // Copy data back to CPU & release memory
    CHECK(cudaMemcpy(result, resultGPU, ny * ny * sizeof(float), cudaMemcpyDeviceToHost));
    CHECK(cudaFree(dataGPU));
    CHECK(cudaFree(resultGPU));
}



