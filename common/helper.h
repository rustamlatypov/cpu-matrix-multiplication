#ifndef HELPER
#define HELPER

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include <utility>
#include <chrono>
#include <limits>
#include "vector.h"

constexpr double error_limit = 1e-3;
constexpr double platform_spec = 230000000000;

std::mt19937 rng(42);
static void gen(int ny, int nx, double* data) {
    //const short a = std::numeric_limits<short>::max();
    std::uniform_real_distribution<double> unif(-1,1);
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


double get_avg(std::vector<double> v) {
    return accumulate( v.begin(), v.end(), 0.0)/v.size();
}


typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

template<typename F, typename... Args>
double funcTime(F func, Args&&... args){
    TimeVar t1=timeNow();
    func(std::forward<Args>(args)...);
    return duration(timeNow()-t1) / double(1E9);
}


static void print(int ny, int nx, const double *matrix) {
    printf("\n");
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            double x = matrix[j * nx + i];
            if (std::abs(x) < 10.0) {
                printf("% -8.4f", x);
            } else {
                printf("% -8.0e", x);
            }
        }
        printf("\n");
    }
    printf("\n");
}


double calculate_cpu(double n, double t) { return 2.0*n*n*n/t/platform_spec; }

printf("hello");

#endif