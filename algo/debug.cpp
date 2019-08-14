#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include "vector.h"

static void generate(int ny, int nx, double* data) {
    std::mt19937 rng;
    std::uniform_real_distribution<double> unif(0.0f, 1.0f);
    std::bernoulli_distribution coin(0.2);
    for (int y = 0; y < ny; ++y) {
        if (y > 0 && coin(rng)) {
            // Introduce some correlations
            int row = std::min(static_cast<int>(y * unif(rng)), y - 1);
            double offset = 2.0 * (unif(rng) - 0.5f);
            double mult = 2.0 * unif(rng);
            for (int x = 0; x < nx; ++x) {
                double error = 0.1 * unif(rng);
                data[x + nx * y] = mult * data[x + nx * row] + offset + error;
            }
        } else {
            // Generate random data
            for (int x = 0; x < nx; ++x) {
                double v = unif(rng);
                data[x + nx * y] = v;
            }
        }
    }
}


static void print(int ny, int nx, const double *matrix) {
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            float x = matrix[j * nx + i];
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