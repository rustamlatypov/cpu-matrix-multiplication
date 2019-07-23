#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "vector.h"
#include "cp.h"
using namespace std;
#include "stopwatch.h"
ppc::stopwatch sw;


float8_t* process(int ny, int nx, int P, const float* data_) {

    const int na = (nx + P - 1) / P;

    // input data with room for horizontal and vertical padding
    float8_t* data = float8_alloc(ny*na);

    // horizontal padding
    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < na; i++) {
            for (int k = 0; k < P; k++)
            {
                int pos = i * P + k;
                data[na*j + i][k] = pos < nx ? data_[nx*j + pos] : 0;

            }
        }
    }

    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {

        float8_t sum8 = float8_0;

        for (int i = 0; i < na; i++) {
            sum8 += data[j*na+i];
        }

        float avg = 0;
        for (int i = 0; i < P; i++) avg += sum8[i];
        avg /= nx;

        //---------------------------

        float8_t avg_v = float8_0;
        for (int k = 0; k < P; k++) {
            avg_v[k] = avg;
        }

        for (int i = 0; i < na-1; i++) {
            data[j*na+i] -= avg_v;
        }

        avg_v = float8_0;
        int lim = P - (na*P-nx);
        for (int k = 0; k < lim; k++) avg_v[k] = avg;

        data[j*na + na-1] -= avg_v;

        //---------------------------

        sum8 = float8_0;

        for (int i = 0; i < na; i++) {
            float8_t e_v = data[j*na+i];
            sum8 += e_v * e_v;
        }

        float sum = 0;
        for (int i = 0; i < P; i++) sum += sum8[i];
        sum = sqrt(sum);

        //---------------------------

        sum8 = float8_0;
        for (int k = 0; k < P; k++) sum8[k] = sum;

        for (int i = 0; i < na; i++) {
            data[j*na+i] /= sum8;
        }
    }
    return data;
}


void correlate(int ny, int nx, const float* data_, float* result) {

    constexpr int P = 8;
    constexpr int B = 2;

    //sw.record();
    float8_t* data = process(ny, nx, P, data_);
    //sw.record();

    int nye = ny;
    while (nye%(P*B) != 0) nye++;
    int nyv = nye/P;
    int nyb = nyv/B;

    const int na = (nx + P - 1) / P;

    // aux data, padded
    float8_t* aux = float8_alloc(nyv*nx);

    #pragma omp parallel for
    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                aux[j*nx + i][k] = j*P+k < ny ? data[(j*P + k)*na + i/P][i%P] : 0;

            }
        }
    }
    //sw.record();

    /*
    for (int j = 0; j < nye; j++) {
        for (int i = 0; i < nx; i++) {
            int ind = (j/P)*nx + i;
            printf("% -7.3f", aux[ind][j%P]);
        }
        cout << endl;
    }
    cout << endl;
    */

    #pragma omp parallel for schedule(dynamic,1)
    for (int j = 0; j < nyb; j++) {

        for (int i = j; i < nyb; i++) {

            float8_t block[B*B*P] = {float8_0};

            asm("#foo");
            for (int k = 0; k < nx; k++) {

                __builtin_prefetch(&aux[(j*B)*nx + k + 10]);
                float8_t a0 = aux[(j*B)*nx + k];
                float8_t a1 = aux[(j*B+1)*nx + k];

                float8_t b0 = aux[(i*B)*nx + k];
                float8_t b1 = aux[(i*B+1)*nx + k];


                block[0] += a0[0]*b0;
                block[1] += a0[0]*b1;

                block[2] += a0[1]*b0;
                block[3] += a0[1]*b1;

                block[4] += a0[2]*b0;
                block[5] += a0[2]*b1;

                block[6] += a0[3]*b0;
                block[7] += a0[3]*b1;

                block[8] += a0[4]*b0;
                block[9] += a0[4]*b1;

                block[10] += a0[5]*b0;
                block[11] += a0[5]*b1;

                block[12] += a0[6]*b0;
                block[13] += a0[6]*b1;

                block[14] += a0[7]*b0;
                block[15] += a0[7]*b1;


                block[16] += a1[0]*b0;
                block[17] += a1[0]*b1;

                block[18] += a1[1]*b0;
                block[19] += a1[1]*b1;

                block[20] += a1[2]*b0;
                block[21] += a1[2]*b1;

                block[22] += a1[3]*b0;
                block[23] += a1[3]*b1;

                block[24] += a1[4]*b0;
                block[25] += a1[4]*b1;

                block[26] += a1[5]*b0;
                block[27] += a1[5]*b1;

                block[28] += a1[6]*b0;
                block[29] += a1[6]*b1;

                block[30] += a1[7]*b0;
                block[31] += a1[7]*b1;
            }
            asm("#foo");

            for (int jj1 = 0; jj1 < P*B; jj1++) {
                for (int jj2 = 0; jj2 < B; jj2++) {
                    for (int ii = 0; ii < P; ii++) {

                        int jjj = j*B * P + jj1;
                        int iii = i*B * P + jj2*P + ii;

                        if (iii >= jjj && jjj < ny && iii < ny) {
                            result[jjj * ny + iii] = block[jj1*B+jj2][ii];
                        }

                    }
                }
            }
        }
    }

    free(aux);
    free(data);
    //sw.record();
    //sw.print();
}