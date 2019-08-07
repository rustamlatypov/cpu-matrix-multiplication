#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <cstdlib>
#include "cp.h"
using namespace std;

float* preprocess(int ny, int nx, const float* data) {

    std::vector<float> aux(ny*nx);

    for (int i = 0; i < ny; i++) {

        float sum;
        float avg;
        float e;

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
    return aux.data();
}

void correlate(int ny, int nx, const float* data, float* result) {

    float* aux = preprocess(ny, nx, data);
    print(ny, nx, aux);

    constexpr int B = 8;

    int nye = ny;
    while (nye%B != 0) nye++;

    std::vector<float> d(nye*nx);

    for (int j = 0; j < nye; j++) {
        for (int i = 0; i < nx; i++) {
            d[j*nx + i] = j < ny ? aux[j*nx + i] : 0;
        }
    }


    print(nye, nx, d.data());
}
