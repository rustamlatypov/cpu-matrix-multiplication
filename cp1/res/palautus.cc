#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "cp.h"

void correlate(int ny, int nx, const float* data, float* result) {

    std::vector<float> aux(ny*nx);

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

    //print(ny, nx, aux.data());

    for (int i = 0; i < ny; i++) {

        for (int j = i+1; j < ny; j++) {

            double e = 0;

            for (int k = 0; k < nx; k++) {
                e += aux[i*nx+k] * aux[j*nx+k];
            }

            result[i*ny+j] = e;
            //print(ny, ny, result);

        }
        result[i*ny+i] = 1;
    }

}
