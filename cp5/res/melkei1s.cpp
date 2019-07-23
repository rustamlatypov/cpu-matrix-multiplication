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

/*
static void print(int ny, int nx, float *matrix) {
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
*/


void preprocess(int ny, int nx, const float* data, float* aux) {


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
}


__global__ void mykernel(const float* data, float* result, int ny, int nx, int nye) 
{
	constexpr int B = 8;

	int jjj = threadIdx.y + blockIdx.y * blockDim.y;
    int iii = threadIdx.x + blockIdx.x * blockDim.x;

    int jj = jjj*B;
    int ii = iii*B;

    if (ii < jj) return;

    float block[B][B] = {0};

    for (int k = 0; k < nx; k++) {
        for (int m = 0; m < B; m++) {

        	float a = data[k*nye + (jj+m)];

            for (int l = 0; l < B; l++) {

                float b = data[k*nye + (ii+l)];
                block[m][l] += a*b;
            }
        }
    }

    for (int m = 0; m < B; m++) {
        for (int l = 0; l < B; l++) {

        	if ((jj+m) < ny && (ii+l) < ny) {
            	result[(jj+m)*ny+(ii+l)] = block[m][l];
            }
        }
    }

    /*
    for (int j = jj; j < jj+8; j++) {

        for (int i = ii; i < ii+8; i++) {

        	if (j < ny && i < ny) {

        		//printf("%d, %d \n", j, i);
        	
	        	float e = 0;
	            for (int k = 0; k < nx; k++) {
	                e += data[j*nx+k] * data[i*nx+k];
	            }
	            result[j*ny+i] = e;
    		
    		}
        }
    }
    */
}

__global__ void myppkernel(float* raw, float* data, int ny, int nx, int nye) 
{
	int j = blockIdx.y;
	int iii = threadIdx.x;

	int ii = iii*128;

	for (int i = ii; i < ii+128; i++) {
		if(i < nx) 
		{
			//printf("%d\n", i);
			data[i*nye + j] = j < ny ? raw[j*nx + i] : 0;
		}
    }

}



void correlate(int ny, int nx, const float* data, float* result) {

	sw.record();
	std::vector<float> aux(ny*nx);
    preprocess(ny, nx, data, aux.data());
    sw.record();

    /*
    printf("\n");
    print(ny, nx, aux.data());
    */

    constexpr int B = 64;

    int nye = ny;
    while (nye%B != 0) nye++;
    int nyb = nye/B;

    /*
    std::vector<float> d(nye*nx);

    for (int j = 0; j < nye; j++) {
        for (int i = 0; i < nx; i++) 
        {
            d[i*nye + j] = j < ny ? aux[j*nx + i] : 0;
        }
    }
    */
    

    //print(nx, nye, t.data());
    
    sw.record();
    float* rawGPU = NULL;
    CHECK(cudaMalloc((void**)&rawGPU, ny * nx * sizeof(float)));

    float* dataGPU = NULL;
    CHECK(cudaMalloc((void**)&dataGPU, nx * nye * sizeof(float)));

    float* resultGPU = NULL;
    CHECK(cudaMalloc((void**)&resultGPU, ny * ny * sizeof(float)));
    //cout << resultGPU << endl;

    CHECK(cudaMemcpy(rawGPU, aux.data(), ny * nx * sizeof(float), cudaMemcpyHostToDevice));
    
	// Run preprocess kernel
    {
        dim3 dimBlock(128, 1);
        dim3 dimGrid(1, nye);

        myppkernel<<<dimGrid, dimBlock>>>(rawGPU, dataGPU, ny, nx, nye);
        CHECK(cudaGetLastError());
    }

    // Run work kernel
    {
        dim3 dimBlock(8, 8);
        dim3 dimGrid(nyb, nyb);

        mykernel<<<dimGrid, dimBlock>>>(dataGPU, resultGPU, ny, nx, nye);
        CHECK(cudaGetLastError());
    }

    // Copy data back to CPU & release memory
    //cout << resultGPU << endl;
    CHECK(cudaMemcpy(result, resultGPU, ny * ny * sizeof(float), cudaMemcpyDeviceToHost));

    CHECK(cudaFree(rawGPU));
    CHECK(cudaFree(dataGPU));
    CHECK(cudaFree(resultGPU));
    sw.record();
    sw.print();
 	//print(ny, ny, result);
}



