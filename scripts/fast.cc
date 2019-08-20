#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <omp.h>

#include "vector.h"


void transpose(int ny, int nx, const double* data_, double* data) {

    #pragma omp parallel for
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            data[j*ny+i] = data_[i*nx+j];
        }
    }
}

double4_t* pad(int nyv, int ny, int nx, const double* data_, int P) {

    // horizontal padding such that rows are devisible by P*A
    // 1  2  3  4  5     1  2  3  4  5
    // 6  7  8  9  10    6  7  8  9  10
    // 11 12 13 14 15 => 11 12 13 14 15
    // 16 17 18 19 20    16 17 18 19 20
    // 21 22 23 24 25    21 22 23 24 25
    //                   0  0  0  0  0
    //                   0  0  0  0  0
    //                   0  0  0  0  0
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


void fast_multiply(int ny, int nm, int nx, const double* D1_, const double* D2__, double* result) {

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


    double4_t* D1 = pad(nyv1, ny1, nx1, D1_, P);

    std::vector<double> D2_(ny2*nx2);
    std::fill(D2_.begin(), D2_.end(), 0);

    transpose(ny2, nx2, D2__, D2_.data());
    int aux = nx2;
    nx2 = ny2;
    ny2 = aux;
    //print(ny2, nx2, D2_.data());

    double4_t* D2 = pad(nyv2, ny2, nx2, D2_.data(), P);
    //print_v(D1, nyv1, nx1, P);
    //print_v(D2, nyv2, nx2, P);

    #pragma omp parallel for
    for (int j = 0; j < nyb1; j++) {

        for (int i = 0; i < nyb2; i++) {

            double4_t block[A*A*P] = {double4_0};

            for (int k = 0; k < nx1; k++) {

                __builtin_prefetch(&D1[(j*A)*nx1 + k + 10]);
                __builtin_prefetch(&D2[(j*A)*nx1 + k + 10]);
                double4_t a0 = D1[(j*A)*nx1 + k];
                double4_t a1 = D1[(j*A+1)*nx1 + k];

                double4_t b0 = D2[(i*A)*nx1 + k];
                double4_t b1 = D2[(i*A+1)*nx1 + k];


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
}