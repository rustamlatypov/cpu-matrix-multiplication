#include <math.h>
#include <chrono>
#include <limits>

#include "fast.cc"
#include "base.cc"
#include "helper.cc"



using c = std::chrono::high_resolution_clock;
using time_point = decltype(c::now());
std::vector<time_point> timePoints;


double validate(int ny, int nx, double* D1, double* D2, int iter) {

    double cumsum = 0.0f;
    for (int j = 0; j < iter; j++) {
        for (int i = 0; i < ny*nx; i++) {
            cumsum += fabs(D1[i] - D2[i]);
        }
    }
    return cumsum;
}


int main(int argc, const char** argv) {

    /*
    const double mini = std::numeric_limits<double>::min();
    const double maxi = std::numeric_limits<double>::max();
    std::cout << mini << '\n' << maxi << std::endl;
    */

    int dim;

    if (argc == 1) {
        dim = 1000;
    } else if(argc == 2) {
        dim = std::stoi(argv[1]);
    } else {
        std::cout << "Usage:\trun <dim>\n";
        return 0;
    }

    int ny = dim; int nm = dim; int nx = dim;

    std::vector<double> D1(ny * nm);
    std::vector<double> D2(nm * nx);

    std::vector<double> base_result(ny * nx);
    std::fill(base_result.begin(), base_result.end(), 0);

    std::vector<double> fast_result(ny * nx);
    std::fill(fast_result.begin(), fast_result.end(), 0);


    gen(ny, nm, D1.data());
    //print(ny, nm, D1.data());

    gen(nm, nx, D2.data());
    //print(nm, nx, D2.data());

    base_multiply(ny, nm, nx, D1.data(), D2.data(), base_result.data());
    //print(ny, nx, base_result.data());

    fast_multiply(ny, nm, nx, D1.data(), D2.data(), fast_result.data());
    //print(ny, nx, fast_result.data());

    double error = validate(ny, nx, base_result.data(), fast_result.data(), 1);

    std::cout << "\nCumulative error: " << error << std::endl;

    std::cout.precision(9999999999);
    std::cout << base_result[0] << std::endl;
    std::cout << fast_result[0] << std::endl;
}