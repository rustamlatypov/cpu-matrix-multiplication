#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include "fast.cc"
#include "helper.h"

std::vector<double> times;
std::vector<double> cpus;


static void benchmark(int dim, int iter) {
    std::vector<double> D1(dim * dim);
    std::vector<double> D2(dim * dim);
    std::vector<double> result(dim * dim);

    gen(dim, dim, D1.data());
    gen(dim, dim, D2.data());

    for (int i = 0; i < iter; i++) {
        printf("%d\t ", dim);
        double t = funcTime(fast_multiply, dim, dim, dim, D1.data(), D2.data(), result.data());
        printf("%.3f\t", t);
        times.push_back(t);

        double cpu_usage = dim*dim;
        printf("%.3f\n", cpu_usage);
        cpus.push_back(cpu_usage);
    }
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

    benchmark(dim, iter);

    float avg1 = get_avg(times);
    float avg2 = get_avg(cpus);
    printf("Average: %.3f\t%.3f\n\n", avg1, avg2);
}
