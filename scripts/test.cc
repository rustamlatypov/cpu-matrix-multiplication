#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>
#include <cassert>
#include "fast.cc"
#include "base.cc"
#include "helper.h"



static bool test(int ny, int nm, int nx) {
    std::vector<double> D1(ny * nm);
    std::vector<double> D2(nm * nx);

    gen(ny, nm, D1.data());
    gen(nm, nx, D2.data());

    std::vector<double> result(ny * nx);
    fast_multiply(ny, nm, nx, D1.data(), D2.data(), result.data());

    //print(ny, nx, result.data());

    std::vector<double> correct(ny * nx);
    base_multiply(ny, nm, nx, D1.data(), D2.data(), correct.data());

    double error = verify_result(ny, nx, result.data(), correct.data(), 20);
    bool pass = error < error_limit;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << '\t' << error / error_limit << '\t';

    return pass;
}


static bool has_fails = false;
static struct { int ny; int nm; int nx;} first_fail = {};
static int passcount = 0;
static int testcount = 0;


// To be used in batch mode to keep track of test suite progress
static void run_test(int ny, int nm, int nx) {
    std::cout << "test "
        << std::setw(4) << ny << ' '
        << std::setw(4) << nm << ' '
        << std::setw(4) << nx << ' '
        << std::flush;
    bool pass = test(ny, nm, nx);
    std::cout << (pass ? "OK\n" : "ERR\n");
    if(pass) {
        passcount++;
    } else if(!has_fails) {
        has_fails = true;
        first_fail.ny = ny;
        first_fail.nm = nm;
        first_fail.nx = nx;
    }
    testcount++;
}


int main(int argc, const char** argv) {

    if (argc == 1) {
        /*
        for(int ny = 50; ny < 55; ny++) {
            for(int nm = 50; nm < 60; nm++) {
                for(int nx = 50; nx < 65; nx++) {
                    run_test(ny, nm, nx);
                }
            }
        }

        std::vector<int> nxs = {10,50,100,200,300,1000};
        std::vector<int> nms = {15,75,150,300,450,1500};
        std::vector<int> nys = {5,25,50,100,150,500};
        for(int ny : nys) {
            for(int nm : nms) {
                for(int nx : nxs) {
                    run_test(ny, nm, nx);
                }
            }
        }*/

        for(int ny = 10; ny < 15; ny++) {
            for(int nm = 10; nm < 15; nm++) {
                for(int nx = 10; nx < 15 nx++) {
                    run_test(ny, nm, nx);
                }
            }
        }

        std::cout << passcount << "/" << testcount << " tests passed.\n" << std::endl;
        if(has_fails) {
            std::cout
                << "To repeat the first failed test with more output, run:\n"
                << argv[0] << " "
                << first_fail.ny << " "
                << first_fail.nm << " "
                << first_fail.nx << std::endl;
            exit(EXIT_FAILURE);
        }
    } else if(argc == 4) {
        int ny = std::stoi(argv[1]);
        int nm = std::stoi(argv[2]);
        int nx = std::stoi(argv[3]);
        run_test(ny, nm, nx);
        if(has_fails) {
            exit(EXIT_FAILURE);
        }
    } else {
        std::cout << "Usage:\ttest <ny> <nm> <nx>" << std::endl;
    }
}