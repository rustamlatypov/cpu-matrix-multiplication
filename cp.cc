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

double4_t* pad(int nyv, int ny, int nx, const float* data_, int P, int na) {

    // aux data, padded
    double4_t* data = double4_alloc(nyv*nx);

    for (int j = 0; j < nyv; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < P; k++) {

                data[k][i] = j < ny ? data_[k] : 0;

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

    int nye2 = nx2;
    while (nye2%(P*A) != 0) nye2++;
    int nyv2 = nye2/A;

    const int na = (nx1 + P - 1) / P;

    double4_t* data1 = pad(nyv1, ny1, nx1, data1__, P, na);
    float* data2_ = transpose(ny2, nx2, data2__);
    double4_t* data2 = pad(nyv2, nx2, ny2, data2_, P, na);

    print_v(nyv1, na, P, data1);
    print_v(nyv2, na, P, data2);

    free(data1);
    free(data2);
}