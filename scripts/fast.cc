#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include "vector.h"



double4_t* pad(int nyv, int nx, int ny, const double* data_, int P) {

    /*
     * horizontal padding such that number of columns is devisible by P*B
     * 1  2  3  4  5     1  2  3  4  5  0  0  0
     * 6  7  8  9  10    6  7  8  9  10 0  0  0
     * 11 12 13 14 15 => 11 12 13 14 15 0  0  0
     * 16 17 18 19 20    16 17 18 19 20 0  0  0
     * 21 22 23 24 25    21 22 23 24 25 0  0  0
     *
     */

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

    /*
     * matrix multiplication of dimensions (ny*nm)(nm*nx)=(ny*nx)
     *
     * P: the amount of values to be stored in double4_t
     * A: block dimension of D1
     * B: block dimension of D2
     *
     * na: tiling parameter for D1
     * nb: tiling parameter for D2
     *
     */

    int ny1 = ny;
    int nx1 = nm;
    int ny2 = nm;
    int nx2 = nx;

    constexpr int P = 4;
    constexpr int A = 2;
    constexpr int B = 2;

    constexpr int na = 1;
    constexpr int nb = 1;

    int nye1 = ny1;
    while (nye1%(P*A*na) != 0) nye1++;
    int nyv1 = nye1/P;
    int nyb1 = nyv1/A;

    int nye2 = nx2;
    while (nye2%(P*B*nb) != 0) nye2++;
    int nyv2 = nye2/P;
    int nyb2 = nyv2/B;

    // not to read out of boundaries, since one step is one block
    std::vector<double> D1(nye1*nm);
    std::memcpy(D1.data(), D1_, ny*nm*sizeof(double));

    double4_t* D2 = pad(nyv2, ny2, nx2, D2_, P);
    ny2 = nx2;

    double dummy = 0.0;

    // main execution loops
    #pragma omp parallel for
    for (int n = 0; n < nyb1; n=n+na) {

        for (int m = 0; m < nyb2; m=m+nb) {

            for (int j = n; j < n+na; j++) {

                for (int i = m; i < m+nb; i++) {

                    // 64 value block to accumulate to
                    double4_t block[A*B*P] = {double4_0};

                    // performace critical loop
                    for (int k = 0; k < nx1; k++) {

/*
                        double a00 = D1[(j*A*P+0)*nx1 + k];
                        double a01 = D1[(j*A*P+1)*nx1 + k];
                        double a02 = D1[(j*A*P+2)*nx1 + k];
                        double a03 = D1[(j*A*P+3)*nx1 + k];

                        double a10 = D1[((j*A+1)*P+0)*nx1 + k];
                        double a11 = D1[((j*A+1)*P+1)*nx1 + k];
                        double a12 = D1[((j*A+1)*P+2)*nx1 + k];
                        double a13 = D1[((j*A+1)*P+3)*nx1 + k];

                        double4_t b0 = D2[(i*B)*nx1 + k];
                        double4_t b1 = D2[(i*B+1)*nx1 + k];
*/

                        //to be able to test cpu without memory bottleneck

                        volatile double a00 = D1[0];
                        volatile double a01 = D1[0];
                        volatile double a02 = D1[0];
                        volatile double a03 = D1[0];

                        volatile double a10 = D1[0];
                        volatile double a11 = D1[0];
                        volatile double a12 = D1[0];
                        volatile double a13 = D1[0];

                        volatile double4_t b0 = D2[0];
                        volatile double4_t b1 = D2[0];


                        block[0] += a00*b0;
                        block[1] += a00*b1;

                        block[2] += a01*b0;
                        block[3] += a01*b1;

                        block[4] += a02*b0;
                        block[5] += a02*b1;

                        block[6] += a03*b0;
                        block[7] += a03*b1;


                        block[8] += a10*b0;
                        block[9] += a10*b1;

                        block[10] += a11*b0;
                        block[11] += a11*b1;

                        block[12] += a12*b0;
                        block[13] += a12*b1;

                        block[14] += a13*b0;
                        block[15] += a13*b1;
                    }

                    // write accumulated block back to memory
                    for (int jj1 = 0; jj1 < P*A; jj1++) {
                        for (int jj2 = 0; jj2 < B; jj2++) {
                            for (int ii = 0; ii < P; ii++) {

                                int jjj = j*A * P + jj1;
                                int iii = i*B * P + jj2*P + ii;

                                if (jjj < ny1 && iii < ny2) {
                                    //jjj * ny2 + iii result[0]
                                    dummy = block[jj1*B+jj2][ii];
                                }

                            }
                        }
                    }
                }
            }
        }
    }

    free(D2);
}


