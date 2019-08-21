#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

#include "fast.cc"
#include "helper.cc"

static void benchmark(int dim) {
    std::vector<double> D1(dim * dim);
    std::vector<double> D2(dim * dim);
    std::vector<double> result(dim * dim);

    gen(dim, dim, D1.data());
    gen(dim, dim, D2.data());

    std::cout << "cp\t" << dim << "\t" << std::flush;
    time_point t1 = c::now();
    fast_multiply(dim, dim, dim, D1.data(), D2.data(), result.data());
    time_point t2 = c::now();
    double t = (t2-t1).count() / double(1E9);
    
    std::cout.precision(3);
    printf("%.3f\n", t);
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
        std::cout << "Usage:\tbenchmark <dim> <iterations>" << std::endl;
        return 0;
    }

    for (int i = 0; i < iter; ++i) {
        benchmark(dim);
    }
    std::cout << std::endl;
}
