#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>

#include "vector.h"



static void print(int ny, int nx, const double *matrix) {
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            double x = matrix[j * nx + i];
            if (std::abs(x) < 10.0) {
                printf("% -7.3f", x);
            } else {
                printf("% -7.0e", x);
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

static void print_v(double4_t* data, int nyv, int nx, int P) {
    for (int j = 0; j < nyv; j++) {
        for (int k = 0; k < P; k++) {
            for (int i = 0; i < nx; i++) {

                double x = data[j * nx + i][k];

                if (std::abs(x) < 10.0) {
                    printf("% -7.3f", x);
                } else {
                    printf("% -7.0e", x);
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}


static void gen(int ny, int nx, double* data) {
    std::mt19937 rng(42);
    const short a = std::numeric_limits<short>::max();
    std::uniform_real_distribution<double> unif(-a,a);
    std::generate(data, data+nx*ny, [&]{ return unif(rng); });
}


double verify_result(int ny, int nx, double* D1, double* D2, int iter) {
    double cumsum = 0.0f;
    for (int i = 0; i < iter; i++) {
        for (int j = 0; j < ny*nx; j++) {
            cumsum += fabs(D1[j] - D2[j]);
        }
    }
    return cumsum;
}