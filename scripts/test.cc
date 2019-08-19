#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>

#include "fast.cc"
#include "base.cc"
#include "error.h"


constexpr double error_limit = 1e-3;



static void gen(int ny, int nx, double* data) {
    std::mt19937 rng;
    const float a = std::numeric_limits<float>::max();
    std::uniform_real_distribution<double> unif(-a, a);
    std::generate(data, data+nx*ny, [&]{ return unif(rng); });
}


float verify_result(int ny, int nm, int nx, double* D1, double* D2, double* result, int iter) {
    double cumsum = 0.0f;

    std::vector<double> correct(ny * nx);
    base_multiply(ny, nm, D1, nm, nx, D2, correct.data());

    for (int i = 0; i < iter; i++) {
        for (int i = 0; i < ny*nx; i++) {
            cumsum += abs(result[i] - correct[i]);
        }
    }

    return cumsum;
}


static bool test(int ny, int nm, int nx, int mode, bool verbose) {
    std::vector<double> D1(ny * nm);
    std::vector<double> D2(nm * nx);

    gen(ny, nm, D1.data());
    gen(nm, nx, D2.data());

    std::vector<double> result(ny * nx);
    multiply(ny, nm, D1.data(), nm, nx, D2.data(), result.data());

    float error = verify_result(ny, nm, nx, D1.data(), D2.data(), result.data(), 20);
    bool pass = error < error_limit;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << '\t' << error / error_limit << '\t';

    return pass;
}


static bool has_fails = false;
static struct { int ny; int nm; int nx; int mode; } first_fail = {};
static int passcount = 0;
static int testcount = 0;


// To be used in batch mode to keep track of test suite progress
static void run_test(int ny, int nm, int nx, int mode, bool verbose) {
    std::cout << "test "
        << std::setw(4) << ny << ' '
        << std::setw(4) << nm << ' '
        << std::setw(4) << nx << ' '
        << std::setw(1) << mode << ' '
        << std::flush;
    bool pass = test(ny, nm, nx, mode, verbose);
    std::cout << (pass ? "OK\n" : "ERR\n");
    if(pass) {
        passcount++;
    } else if(!has_fails) {
        has_fails = true;
        first_fail.ny = ny;
        first_fail.nm = nm;
        first_fail.nx = nx;
        first_fail.mode = mode;
    }
    testcount++;
}

int main(int argc, const char** argv) {

    std::vector<int> modes = {0,1,2,3};
    std::vector<int> nxs = {10,50,100,200,500,1000};
    std::vector<int> nms = {5,30,60,80,100,150};
    std::vector<int> nys = {2,5,10,100,200};
    for(int ny : nys)
    for(int nm : nms)
    for(int nx : nxs)
    for(int mode : modes)
        run_test(ny, nm, nx, mode, false);

    std::cout << passcount << "/" << testcount << " tests passed.\n";
    if(has_fails) {
        std::cout
            << "To repeat the first failed test with more output, run:\n"
            << argv[0] << " "
            << first_fail.ny << " "
            << first_fail.nm << " "
            << first_fail.nx << " "
            << first_fail.mode << std::endl;
        exit(EXIT_FAILURE);
    }

}