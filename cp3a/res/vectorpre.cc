#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "vector.h"
#include "cp.h"
using namespace std;
#include "stopwatch.h"
ppc::stopwatch sw;


double4_t* process(int ny, int nx, int P, const float* data_) {

    const int na = (nx + P - 1) / P;

    // input data with room for horizontal and vertical padding
    double4_t* data = double4_alloc(ny*na);

    // horizontal padding
    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < na; i++) {
            for (int k = 0; k < P; k++) {

                int pos = i * P + k;
                data[na*j + i][k] = pos < nx ? data_[nx*j + pos] : 0;

            }
        }
    }

    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {

        double4_t sum4 = double4_0;

        for (int i = 0; i < na; i++) {
            sum4 += data[j*na+i];
        }

        double avg = 0;
        for (int i = 0; i < P; i++) avg += sum4[i];
        avg /= nx;

        //---------------------------

        double4_t avg_v = double4_0;
        for (int k = 0; k < P; k++) {
            avg_v[k] = avg;
        }

        for (int i = 0; i < na-1; i++) {
            data[j*na+i] -= avg_v;
        }

        avg_v = double4_0;
        int lim = P - (na*P-nx);
        for (int k = 0; k < lim; k++) avg_v[k] = avg;

        data[j*na + na-1] -= avg_v;


        //---------------------------

        sum4 = double4_0;

        for (int i = 0; i < na; i++) {
            double4_t e_v = data[j*na+i];
            sum4 += e_v * e_v;
        }

        double sum = 0;
        for (int i = 0; i < P; i++) sum += sum4[i];
        sum = sqrt(sum);

        //---------------------------

        sum4 = double4_0;
        for (int k = 0; k < P; k++) sum4[k] = sum;

        for (int i = 0; i < na; i++) {
            data[j*na+i] /= sum4;
        }
    }
    /*
    //sw.record();
    vector<double> aux_(ny*nx);

    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < na; i++) {
            for (int k = 0; k < P; k++) {

                int pos = i * P + k;
                if (pos < nx) aux_[j*nx + pos] = data[na*j + i][k];

            }
        }
    }
     */
    return data;
}

void correlate(int ny, int nx, const float* data_, float* result) {


    constexpr int P = 4;
    constexpr int A = 2;

    sw.record();
    double4_t* data = process(ny, nx, P, data_);
    sw.record();


    int nye = ny;
    while (nye%(P*A) != 0) nye++;
    int nyv = nye/P;
    int nyb = nyv/A;

    const int na = (nx + P - 1) / P;

    // aux data, padded
    double4_t* aux = double4_alloc(nyv*nx);

    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                if ((j*P+k) < ny)
                {
                    aux[j*nx + i][k] = data[(j*P + k)*na + i/P][i%P];
                }
                else
                {
                    aux[j*nx + i][k] = 0;
                }

            }
        }
    }
    sw.record();
    /*
    //#pragma omp parallel for
    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                if ((j*P+k) < ny) {
                    aux[j*nx + i][k] = data[j*nx*P + i + k*nx];
                } else {
                    aux[j*nx + i][k] = 0;
                }
            }
        }
    }*/

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
    free(data);
    //print(ny, ny, result);
    sw.record();
    sw.print();
}