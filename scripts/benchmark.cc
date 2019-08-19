#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

#include "fast.cc"
#include "error.h"
#include "timer.h"

static void benchmark(int dim) {
    std::mt19937 rng;
    std::uniform_real_distribution<float> u(0.0f, 1.0f);
    std::vector<double> data(dim * dim);
    std::vector<double> result(dim * dim);
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            float v = u(rng);
            data[x + dim * y] = v;
        }
    }
    std::cout << "cp\t" << dim << "\t" << std::flush;
    { ppc::timer t; fast_multiply(dim, dim, dim, data.data(), data.data(), result.data()); }
    std::cout << std::endl;
}

int main(int argc, const char** argv) {
    if (argc < 2 || argc > 3) {
        error("usage: benchmark <dim> <iterations>");
    }
    int dim = std::stoi(argv[1]);
    int iter = argc == 3 ? std::stoi(argv[2]) : 1;
    for (int i = 0; i < iter; ++i) {
        benchmark(dim);
    }
}
