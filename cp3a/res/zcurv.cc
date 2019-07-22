#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <immintrin.h>
#include <algorithm>
#include <tuple>
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

    vector<double> data = process(ny, nx, data_);

    //sw.record();

    constexpr int P = 4;
    constexpr int A = 2;

    int nye = ny;
    while (nye%(P*A) != 0) nye++;
    int nyv = nye/P;
    int nyb = nyv/A;

    //print(ny, nx, data.data());

    // aux data, padded
    double4_t* aux = double4_alloc(nyv*nx);


    #pragma omp parallel for
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

    std::vector<std::tuple<int,int,int>> rows;

    for (int j = 0; j < nyb; j++) {
        for (int i = j; i < nyb; i++) {
            int ij = _pdep_u32(j, 0x55555555) | _pdep_u32(i, 0xAAAAAAAA);
            rows.push_back(std::make_tuple(ij, j, i));
        }
    }

    std::sort(rows.begin(), rows.end());

    #pragma omp parallel for schedule(dynamic,1)
    for (unsigned int m = 0; m < rows.size(); m++) {

        std::tuple<int,int,int> pair = rows[m];

        int j = std::get<1>(pair);
        int i = std::get<2>(pair);

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

    free(aux);
    //print(ny, ny, result);
    //sw.record();
    //sw.print();
}