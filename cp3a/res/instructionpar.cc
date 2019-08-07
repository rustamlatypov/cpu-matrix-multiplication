#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "vector.h"
#include "cp.h"
using namespace std;
#include "stopwatch.h"
ppc::stopwatch sw;


vector<double> process(int ny, int nx, const float* data_) {

    constexpr int P = 4;
    int na = (nx + P - 1) / P;
    int nab = na * P;

    // input data, padded
    std::vector<double> data(ny*nab, 0);

    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            data[j*nab + i] = data_[j*nx + i];
        }
    }

    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {

        // running sum array
        double sum_v[P] = {0};
        // actual sum
        double sum = 0;

        //------------------------

        for (int i = 0; i < na; i++) {
            for (int k = 0; k < P; k++) {
                sum_v[k] += data[j*nab + i*P + k];
            }
        }

        for (int k = 0; k < P; ++k) sum += sum_v[k];

        //------------------------

        double avg = sum / nx;

        for (int i = 0; i < nx; i++) {
            data[j*nab + i] -= avg;
        }

        //------------------------

        memset(sum_v, 0, P*sizeof(double));
        double e = 0;
        sum = 0;

        for (int i = 0; i < na; i++) {
            for (int k = 0; k < P; k++) {
                e = data[j*nab + i*P + k];
                sum_v[k] += e * e;
            }
        }

        for (int k = 0; k < P; ++k) sum += sum_v[k];

        //------------------------

        sum = sqrt(sum);

        for (int i = 0; i < nx; i++) {
            data[j*nab + i] /= sum;
        }
    }

    //print(ny, nab, data.data());
    return data;
}

void correlate(int ny, int nx, const float* data_, float* result) {


    constexpr int P = 4;
    constexpr int A = 2;

    sw.record();
    std::vector<double> data = process(ny, nx, data_);
    sw.record();


    int nye = ny;
    while (nye%(P*A) != 0) nye++;
    int nyv = nye/P;
    int nyb = nyv/A;


    // aux data, padded
    double4_t* aux = double4_alloc(nyv*nx);


    int na = (nx + P - 1) / P;
    int nab = na * P;

    #pragma omp parallel for
    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                if ((j*P+k) < ny) {
                    aux[j*nx + i][k] = data[j*nab*P + i + k*nab];
                } else {
                    aux[j*nx + i][k] = 0;
                }
            }
        }
    }

    sw.record();
    /*
    for (int j = 0; j < nye; j++) {
        for (int i = 0; i < nx; i++) {
            int ind = (j/P)*nx + i;
            printf("% -7.3f", aux[ind][j%P]);
        }
        cout << endl;
    }
    cout << endl;*/


    #pragma omp parallel for schedule(dynamic,1)
    for (int j = 0; j < nyb; j++) {

        for (int i = j; i < nyb; i++) {

            double4_t block[A*A*P] = {double4_0};

            for (int k = 0; k < nx; k++) {

                __builtin_prefetch(&aux[(j*A)*nx + k + 5]);
                double4_t a0 = aux[(j*A)*nx + k];
                double4_t a1 = aux[(j*A+1)*nx + k];

                double4_t b0 = aux[(i*A)*nx + k];
                double4_t b1 = aux[(i*A+1)*nx + k];


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

                        if (jjj < ny && iii < ny) {
                            result[jjj * ny + iii] = block[jj1*A+jj2][ii];
                        }

                    }
                }
            }
        }
    }

    free(aux);
    //print(ny, ny, result);
    sw.record();
    sw.print();
}