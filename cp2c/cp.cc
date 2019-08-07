#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "vector.h"
#include "cp.h"
using namespace std;

void correlate(int ny, int nx, const float* data_, float* result) {


    std::vector<double> aux_(ny*nx, 0);

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

    constexpr int P = 4;
    int nyv = (ny + P - 1) / P;
    //int nye = nyv*P;

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

    for (int j = 0; j < nyv; j++) {

        for (int i = j; i < nyv; i++) {

            double4_t block[4] = {double4_0};

            for (int k = 0; k < nx; k++) {

                double4_t a = aux[j*nx + k];
                double4_t b = aux[i*nx + k];

                double _a0 = a[0];
                double4_t a0 = {_a0, _a0, _a0, _a0};

                double _a1 = a[1];
                double4_t a1 = {_a1, _a1, _a1, _a1};

                double _a2 = a[2];
                double4_t a2 = {_a2, _a2, _a2, _a2};

                double _a3 = a[3];
                double4_t a3 = {_a3, _a3, _a3, _a3};

                block[0] += a0*b;
                block[1] += a1*b;
                block[2] += a2*b;
                block[3] += a3*b;
            }

            for (int jj = 0; jj < P; jj++) {
                for (int ii = 0; ii < P; ii++) {

                    int jjj = j*P + jj;
                    int iii = i*P + ii;

                    if (jjj < ny && iii < ny)
                    {
                        result[jjj*ny + iii] = block[jj][ii];
                    }

                }
            }
        }
    }

    free(aux);
    //print(ny, ny, result);

}
