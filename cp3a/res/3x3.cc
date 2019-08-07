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

    std::vector<double> aux_(ny*nx, 0);

    #pragma omp parallel for
    for (int i = 0; i < ny; i++) {


        double sum = 0;

        for (int j = 0; j < nx; j++) {
            sum += data_[i*nx+j];
        }

        double avg = sum/nx;

        sum = 0;
        double e;

        for (int j = 0; j < nx; j++) {
            e = data_[i*nx+j]-avg;
            sum += e*e;
        }

        sum = sqrt(sum);

        for (int j = 0; j < nx; j++) {
            aux_[i*nx+j] = (data_[i*nx+j]-avg) / sum;
        }
    }
    return aux_;
}

void correlate(int ny, int nx, const float* data_, float* result) {

    //sw.record();

    vector<double> aux_ = process(ny, nx, data_);

    //sw.record();

    constexpr int P = 4;
    constexpr int A = 3;

    int nye = ny;
    while (nye%(P*A) != 0) nye++;
    int nyv = nye/P;
    int nyb = nyv/A;
    //print(ny, nx, aux_.data());

    // aux data, padded
    double4_t* aux = double4_alloc(nyv*nx);

    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {

            for (int k = 0; k < P; k++)
            {
                aux[j*nx + i][k] = (j*P+k) < ny ? aux_[j*nx*P + i + k*nx] : 0;
            }
        }
    }

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

            double4_t block[A*A*P] = {double4_0};

            for (int k = 0; k < nx; k++) {

                double4_t a0 = aux[(j*A)*nx + k];
                double4_t a1 = aux[(j*A+1)*nx + k];
                double4_t a2 = aux[(j*A+2)*nx + k];

                double4_t b0 = aux[(i*A)*nx + k];
                double4_t b1 = aux[(i*A+1)*nx + k];
                double4_t b2 = aux[(i*A+2)*nx + k];


                block[0] += a0[0]*b0;
                block[1] += a0[0]*b1;
                block[2] += a0[0]*b2;

                block[3] += a0[1]*b0;
                block[4] += a0[1]*b1;
                block[5] += a0[1]*b2;

                block[6] += a0[2]*b0;
                block[7] += a0[2]*b1;
                block[8] += a0[2]*b2;

                block[9] += a0[3]*b0;
                block[10] += a0[3]*b1;
                block[11] += a0[3]*b2;


                block[12] += a1[0]*b0;
                block[13] += a1[0]*b1;
                block[14] += a1[0]*b2;

                block[15] += a1[1]*b0;
                block[16] += a1[1]*b1;
                block[17] += a1[1]*b2;

                block[18] += a1[2]*b0;
                block[19] += a1[2]*b1;
                block[20] += a1[2]*b2;

                block[21] += a1[3]*b0;
                block[22] += a1[3]*b1;
                block[23] += a1[3]*b2;


                block[24] += a2[0]*b0;
                block[25] += a2[0]*b1;
                block[26] += a2[0]*b2;

                block[27] += a2[1]*b0;
                block[28] += a2[1]*b1;
                block[29] += a2[1]*b2;

                block[30] += a2[2]*b0;
                block[31] += a2[2]*b1;
                block[32] += a2[2]*b2;

                block[33] += a2[3]*b0;
                block[34] += a2[3]*b1;
                block[35] += a2[3]*b2;
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
    //sw.record();
    //sw.print();
}
