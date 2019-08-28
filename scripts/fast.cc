#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <omp.h>
#include <chrono>
#include "vector.h"
#include "helper.h"

// vertical padding such that rows are devisible by P*A
// 1  2  3  4  5     1  2  3  4  5
// 6  7  8  9  10    6  7  8  9  10
// 11 12 13 14 15 => 11 12 13 14 15
// 16 17 18 19 20    16 17 18 19 20
// 21 22 23 24 25    21 22 23 24 25
//                   0  0  0  0  0
//                   0  0  0  0  0
//                   0  0  0  0  0

// when A*B, pad1 is for A and pad2 is for B


double4_t* pad1(int nyv, int ny, int nx, const double* data_, int P) {

    double4_t* data = double4_alloc(nyv*nx);

    #pragma omp parallel for
    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                data[j*nx+i][k] = j*P+k < ny ? data_[(j*P+k)*nx+i] : 0;

            }
        }
    }

    return data;
}


double4_t* pad2(int nyv, int nx, int ny, const double* data_, int P) {


    double4_t* data = double4_alloc(nyv*nx);

    #pragma omp parallel for
    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                data[j*nx+i][k] = j*P+k < ny ? data_[i*ny+(j*P+k)] : 0;

            }
        }
    }

    return data;
}


void fast_multiply(int ny, int nm, int nx, const double* D1_, const double* D2_, double* result) {

    int ny1 = ny;
    int nx1 = nm;
    int ny2 = nm;
    int nx2 = nx;

    constexpr int P = 4;
    constexpr int A = 2;

    int nye1 = ny1;
    while (nye1%(P*A) != 0) nye1++;
    int nyv1 = nye1/P;
    int nyb1 = nyv1/A;

    int nye2 = nx2;
    while (nye2%(P*A) != 0) nye2++;
    int nyv2 = nye2/P;
    int nyb2 = nyv2/A;

    double4_t* D1 = pad1(nyv1, ny1, nx1, D1_, P);
    double4_t* D2 = pad2(nyv2, ny2, nx2, D2_, P);

    ny2 = nx2;

    #pragma omp parallel for
    for (int j = 0; j < nyb1; j++) {

        for (int i = 0; i < nyb2; i++) {

            double4_t block[A*A*P] = {double4_0};

            for (int k = 0; k < nx1; k++) {
                
                //double4_t a0 = D1[(j*A)*nx1 + k];

                //printf("%f\n", a0[0]);
                //printf("%f\n", D1_[(j*A)*nx1*P + k]);
            	double a00 = D1_[(j*A)*nx1*P + k]
            	double a01 = D1_[(j*A+1)*nx1*P + k]
            	double a02 = D1_[(j*A+2)*nx1*P + k]
            	double a03 = D1_[(j*A+3)*nx1*P + k]

                double4_t a1 = D1[(j*A+1)*nx1 + k];

                double4_t b0 = D2[(i*A)*nx1 + k];
                double4_t b1 = D2[(i*A+1)*nx1 + k];

                
                block[0] += a00*b0;
                block[1] += a00*b1;

                block[2] += a01*b0;
                block[3] += a01*b1;

                block[4] += a02*b0;
                block[5] += a02*b1;

                block[6] += a03*b0;
                block[7] += a03*b1;


                block[8] += a1[0]*b0;
                block[9] += a1[0]*b1;

                block[10] += a1[1]*b0;
                block[11] += a1[1]*b1;

                block[12] += a1[2]*b0;
                block[13] += a1[2]*b1;

                block[14] += a1[3]*b0;
                block[15] += a1[3]*b1;
            }
            
            for (int jj1 = 0; jj1 < P*A; jj1++) {
                for (int jj2 = 0; jj2 < A; jj2++) {
                    for (int ii = 0; ii < P; ii++) {

                        int jjj = j*A * P + jj1;
                        int iii = i*A * P + jj2*P + ii;

                        if (jjj < ny1 && iii < ny2) {
                            result[jjj * ny2 + iii] = block[jj1*A+jj2][ii];
                        }

                    }
                }
            }
        }
    }
    free(D1);
    free(D2);
    //print(ny1, nx2, result);
}