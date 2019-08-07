#include "is.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <tuple>
#include <math.h>
#include <cmath>
#include "../common/stopwatch.h"
ppc::stopwatch sw;
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

void toFloat(int ny, int nx, const float* data, float* D) {

    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++)
        {
            D[j*nx + i] = data[3*j*nx + 3*i + 0];
        }
    }
}

void process(int ny, int nx, float* D, float* S) {

    for (int j = 0; j < ny+1; j++) {
        S[j*(nx+1)] = 0;
    }

    for (int i = 0; i < nx+1; i++) {
        S[i] = 0;
    }

    for (int j = 1; j < ny+1; j++) {
        for (int i = 1; i < nx+1; i++) {

            float sum = S[(j-1)*(nx+1) + (i)];

            for (int ii = 0; ii < i; ii++) {
                sum += D[(j-1)*nx + ii];
            }

            S[j*(nx+1) + i] = sum;
        }
    }
}

__global__ void segkernel(int ny, int nx, float* S, float* resultGPU, int P, int vp, int vp2) 
{

	int X1 = threadIdx.y + blockIdx.y * blockDim.y + 1;
    int X2 = threadIdx.x + blockIdx.x * blockDim.x + 1;

    if (X1 > ny || X2 > nx) return;

    //printf("(%d,%d)", X1, X2);

    float h = 0.0;
    float y0 = 0;
    float x0 = 0;

    float X = X1*X2;
    float Y = P - X;

    float A = (X+Y)/(X*Y);
    float B = -2*vp/Y;
    float C = vp2/Y;

    for (int j0 = 0; j0 <= ny-X1; j0++) {
        for (int i0 = 0; i0 <= nx-X2; i0++) {

            int j1 = j0+X1;
            int i1 = i0+X2;

            float vx = S[j1*(nx+1) + i1]
                       - S[j0*(nx+1) + i1]
                       - S[j1*(nx+1) + i0]
                       + S[j0*(nx+1) + i0];

            float h_value = (vx*A + B)*vx + C;

            //printf("(%d,%d,%d,%d,%.3f)", j0, i0, j1, i1, h_value);

            if (h_value > h)
            {
            	h = h_value;
            	y0 = j0;
			    x0 = i0;
            }
        }
    }

    //printf("\n%.3f, %d, %d, %d, %d", h, y0, x0, y1, x1);

    int ind = ((X1-1)*nx + (X2-1))*5;
    resultGPU[ind] = h;
    resultGPU[ind + 1] = y0;
    resultGPU[ind + 2] = x0;
    resultGPU[ind + 3] = y0+X1;
    resultGPU[ind + 4] = x0+X2;
} 


Result segment(int ny, int nx, const float* data) {

    vector<float> D(ny*nx);
    toFloat(ny, nx, data, D.data());

    vector<float> S((ny+1)*(nx+1));
    process(ny, nx, D.data(), S.data());

    //print(ny, nx, D);
    //print(ny+1, nx+1, S);

    float vp = 0;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++)
        {
            vp += D[j*nx + i];
        }
    }
    float vp2 = vp*vp;
    float P = ny*nx;
   

    float* sGPU = NULL;
    CHECK(cudaMalloc((void**)&sGPU, (ny+1) * (nx+1) * sizeof(float)));
    CHECK(cudaMemcpy(sGPU, S.data(), (ny+1) * (nx+1) * sizeof(float), cudaMemcpyHostToDevice));


	vector<float> result(5 * ny * nx);
    float* resultGPU = NULL;
    CHECK(cudaMalloc((void**)&resultGPU, 5 * ny * nx * sizeof(float)));


    // Run kernel
    {
        dim3 dimBlock(16, 16);
        dim3 dimGrid(divup(nx, 16), divup(ny, 16));

        segkernel<<<dimGrid, dimBlock>>>(ny, nx, sGPU, resultGPU, P, vp, vp2);
        CHECK(cudaGetLastError());
    }


    // Copy data back, release memory, find best solution
    CHECK(cudaMemcpy(result.data(), resultGPU, 5 * ny * nx * sizeof(float), cudaMemcpyDeviceToHost));
    CHECK(cudaFree(sGPU));
    CHECK(cudaFree(resultGPU));

    /*
    cout << endl;
    for (int i = 0; i < 5*ny*nx; i++)
    {
    	cout << result[i] << " ";
    }
    cout << endl;
    */


    float h = 0.0;
    int y0 = 0;
    int x0 = 0;
    int y1 = 0;
    int x1 = 0;

    for (int i = 0; i < 5*ny*nx; i+=5) {
    	if (result[i] > h)
    	{
    		h = result[i];
    		y0 = result[i+1];
    		x0 = result[i+2];
    		y1 = result[i+3];
    		x1 = result[i+4];
    	}
    }


    float a = 0;
    for (int j = y0; j < y1; j++) {
        for (int i = x0; i < x1; i++) {

            a += D[j*nx + i];

        }
    }

    float b = vp - a;

    float X = (y1-y0)*(x1-x0);
    a /= X;

    float Y = P - X;
    b /= Y;

    return { y0, x0, y1, x1, {(float) b, (float) b, (float) b},
                    		 {(float) a, (float) a, (float) a} };
}










