#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "cp.h"

void correlate(int ny, int nx, const float* data, float* result) {

    std::vector<double> aux(ny*nx);

    for (int i = 0; i < ny; i++) {

        double sum;
        double avg;
        double e;

        sum = 0;

        for (int j = 0; j < nx; j++) {
            sum += data[i*nx+j];
        }

        avg = sum/nx;

        for (int j = 0; j < nx; j++) {
            aux[i*nx+j] = data[i*nx+j] - avg;
        }

        sum = 0;

        for (int j = 0; j < nx; j++) {
            e = aux[i*nx+j];
            sum += e*e;
        }

        sum = sqrt(sum);

        for (int j = 0; j < nx; j++) {
            aux[i*nx+j] /= sum;
        }
    }

    print(ny, nx, aux.data());
    /*
    for (int i = 0; i < ny; i++) {

        for (int j = 0; j < ny; j++) {

            double e = 0;

            for (int k = 0; k < nx; k++) {
                e += aux[i*nx+k] * aux[j*nx+k];
            }

            result[i*ny+j] = e;
            //print(ny, ny, result);

        }
    }*/

    std::vector<double> aux_(ny*nx);

    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            aux_[i*ny+j] = aux[j*nx+i];

        }
    }

    print(nx, ny, aux.data());

    double block[8][8] = {0};

    int j = 0;
    int i = 0;

    for (int k = 0; k < nx; k++) {

        for (int m = 0; m < 8; m++) {
            for (int l = 0; l < 8; l++) {

                float a = aux[m*nx+k];
                float b = aux[l*nx+k];

                block[m][l] += a*b;
            }
        }
    }

    for (int m = 0; m < 8; m++) {
        for (int l = 0; l < 8; l++) {

            result[m*ny+l] = block[m][l];
        }
    }
}
