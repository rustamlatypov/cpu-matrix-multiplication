#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

#include "fast.cc"
#include "helper.h"

std::vector<double> times;

static void benchmark(int dim) {
    std::vector<double> D1(dim * dim);
    std::vector<double> D2(dim * dim);
    std::vector<double> result(dim * dim);

    gen(dim, dim, D1.data());
    gen(dim, dim, D2.data());

    printf("%d\t", dim);
    double t = funcTime(fast_multiply, dim, dim, dim, D1.data(), D2.data(), result.data());
    
    printf("%.3f\n", t);
    times.push_back(t);
}

int main(int argc, const char** argv) {

    int dim; int iter;
    if (argc == 3) {
        dim = std::stoi(argv[1]);
        iter = std::stoi(argv[2]);
    } else if (argc == 1) {
        dim = 3000;
        iter = 10;
    } else {
        std::cout << "Usage:\tbenchmark <dim> <iter>" << std::endl;
        return 0;
    }

    for (int i = 0; i < iter; ++i) {
        benchmark(dim);
    }

    float avg = get_avg(times);
    printf("Average: %.3f\n\n", avg);
}
