#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "vector.h"
#include "cp.h"
using namespace std;
#include "stopwatch.h"
ppc::stopwatch sw;


float* transpose(int ny, int nx, const float* data_) {

    std::vector<float> data(ny*nx);
    std::fill(data.begin(), data.end(), 0);

    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            data[j*ny+i] = data_[i*nx+j];
        }
    }

    return data.data();
}

double4_t* pad(int nyv, int ny, int nx, const float* data_, int P) {

    // aux data, padded
    double4_t* data = double4_alloc(nyv*nx);

    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                data[j*nx+i][k] = j*P+k < ny ? data_[(j*P+k)*nx+i] : 0;

            }
        }
    }

    return data;
}


void multiply(int ny1, int nx1, const float* data1__,
              int ny2, int nx2, const float* data2__, float* result) {

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


    double4_t* data1 = pad(nyv1, ny1, nx1, data1__, P);
    float* data2_ = transpose(ny2, nx2, data2__);

    int aux = nx2;
    nx2 = ny2;
    ny2 = aux;
    print(ny2, nx2, data2_);

    double4_t* data2 = pad(nyv2, ny2, nx2, data2_, P);

    print_v(data1, nyv1, nx1, P);
    print_v(data2, nyv2, nx2, P);

    free(data1);
    free(data2);
}