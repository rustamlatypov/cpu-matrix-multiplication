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

    int dim; int iter;
    if (argc == 1) {
        dim = 1000;
        iter = 3;
    } else if(argc == 3) {
        dim = std::stoi(argv[1]);
        iter = std::stoi(argv[2]);
    } else {
        std::cout << "Usage:\trun <dim> <iter>\n";
        return 0;
    }

    std::vector<double> base_time(iter);
    std::vector<double> fast_time(iter);

    int ny = dim; int nm = dim; int nx = dim;
    std::vector<double> D1(ny * nm);
    std::vector<double> D2(nm * nx);

    std::vector<double> base_result(ny * nx);
    std::vector<double> fast_result(ny * nx);
    std::vector<double> cumerror(ny * nx);

    for (int i = 0; i < iter; i++)
    {
        std::fill(base_result.begin(), base_result.end(), 0);
        std::fill(fast_result.begin(), fast_result.end(), 0);

        gen(ny, nm, D1.data());
        //print(ny, nm, D1.data());

        gen(nm, nx, D2.data());
        //print(nm, nx, D2.data());

        time_point t1 = c::now();
        base_multiply(ny, nm, nx, D1.data(), D2.data(), base_result.data());
        //print(ny, nx, base_result.data());
        time_point t2 = c::now();
        base_time[i] = (t2-t1).count() / double(1E9);

        t1 = c::now();
        fast_multiply(ny, nm, nx, D1.data(), D2.data(), fast_result.data());
        //print(ny, nx, fast_result.data());
        t2 = c::now();
        fast_time[i] = (t2-t1).count() / double(1E9);

        double error = validate(ny, nx, base_result.data(), fast_result.data(), 1);
        cumerror[i] = error;
    }

    float base = accumulate( base_time.begin(), base_time.end(), 0.0)/base_time.size();
    float fast = accumulate( fast_time.begin(), fast_time.end(), 0.0)/fast_time.size();
    float error = accumulate( cumerror.begin(), cumerror.end(), 0.0)/cumerror.size();

    std::cout << "Average of " << iter << " runs: \n" << "Base: " << base << ", Fast: " << fast
    << ", Ratio: " << base/fast  << ", Error: " << error << "\n" << std::endl;
    /*
    std::cout.precision(9999999999);
    std::cout << base_result[0] << std::endl;
    std::cout << fast_result[0] << std::endl;
    */
}