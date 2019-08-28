#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <omp.h>
#include <chrono>
#include "vector.h"
#include "helper.h"

// Vertical padding such that rows are devisible by P*S
// 1  2  3  4  5     1  2  3  4  5
// 6  7  8  9  10    6  7  8  9  10
// 11 12 13 14 15 => 11 12 13 14 15
// 16 17 18 19 20    16 17 18 19 20
// 21 22 23 24 25    21 22 23 24 25
//                   0  0  0  0  0
//                   0  0  0  0  0
//                   0  0  0  0  0

// When A*B, pad1 is for A and pad2 is for B
// pad1 is straighforward vertical padding
// pad2 performes a transpose as well as vertical padding


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
    constexpr int S = 2;

    int nye1 = ny1;
    while (nye1%(P*3) != 0) nye1++;
    int nyv1 = nye1/P;
    int nyb1 = nyv1/3;

    int nye2 = nx2;
    while (nye2%(P*S) != 0) nye2++;
    int nyv2 = nye2/P;
    int nyb2 = nyv2/S;

    double4_t* D1 = pad1(nyv1, ny1, nx1, D1_, P);
    double4_t* D2 = pad2(nyv2, ny2, nx2, D2_, P);

    ny2 = nx2;

    #pragma omp parallel for
    for (int j = 0; j < nyb1; j++) {

        for (int i = 0; i < nyb2; i++) {

            double4_t block[S*3*P] = {double4_0};

            for (int k = 0; k < nx1; k++) {
                
                double4_t a0 = D1[(j*3)*nx1 + k];
                double4_t a1 = D1[(j*3+1)*nx1 + k];
                double4_t a2 = D1[(j*3+2)*nx1 + k];

                double4_t b0 = D2[(i*S)*nx1 + k];
                double4_t b1 = D2[(i*S+1)*nx1 + k];
        
                block[0] += a0[0]*b0;
                block[1] += a0[0]*b1;

                block[2] += a0[1]*b0;
                block[3] += a0[1]*b1;

                block[4] += a0[2]*b0;
                block[5] += a0[2]*b1;

                block[6] += a0[3]*b0;
                block[7] += a0[3]*b1;


                block[8] += a1[0]*b0;
                block[9] += a1[0]*b1;

                block[10] += a1[1]*b0;
                block[11] += a1[1]*b1;

                block[12] += a1[2]*b0;
                block[13] += a1[2]*b1;

                block[14] += a1[3]*b0;
                block[15] += a1[3]*b1;


                block[16] += a2[0]*b0;
                block[17] += a2[0]*b1;

                block[18] += a2[1]*b0;
                block[19] += a2[1]*b1;

                block[20] += a2[2]*b0;
                block[21] += a2[2]*b1;

                block[22] += a2[3]*b0;
                block[23] += a2[3]*b1;
            }
            
            for (int jj1 = 0; jj1 < P*3; jj1++) {
                for (int jj2 = 0; jj2 < S; jj2++) {

                    for (int ii = 0; ii < P; ii++) {

                        int jjj = j*3 * P + jj1;
                        int iii = i*S * P + jj2*P + ii;

                        if (jjj < ny1 && iii < ny2) {
                            result[jjj * ny2 + iii] = block[jj1*3+jj2][ii];
                        }

                    }

                }
            }
        }
    }
    free(D1);
    free(D2);

    //print(ny, nm, D1_);
    //print(nm, nx, D2_);
    //print(ny, nx, result);
}