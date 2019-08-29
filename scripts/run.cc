#include <math.h>
#include <limits>
#include "fast.cc"
#include "base.cc"
#include "helper.h"


int main(int argc, const char** argv) {

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
    std::vector<double> cpu1;
    std::vector<double> cpu2;

    std::fill(base_result.begin(), base_result.end(), 0);
    std::fill(fast_result.begin(), fast_result.end(), 0);
    gen(ny, nm, D1.data());
    //print(ny, nm, D1.data());
    gen(nm, nx, D2.data());
    //print(nm, nx, D2.data());

    for (int i = 0; i < iter; i++)
    {   
        base_time[i] = funcTime(base_multiply, ny, nm, nx, D1.data(), D2.data(), base_result.data());
        fast_time[i] = funcTime(fast_multiply, ny, nm, nx, D1.data(), D2.data(), fast_result.data());

        double error = verify_result(ny, nx, base_result.data(), fast_result.data(), 1);
        cumerror[i] = error;

        double n = dim;
        cpu1[i] = 2*n*n*n/base_time[i]/230000000000;
        cpu2[i] = 2*n*n*n/fast_time[i]/230000000000;
    }

    double base = get_avg(base_time);
    double fast = get_avg(fast_time);
    double error = get_avg(cumerror);

    double cp1 = get_avg(cpu1);
    double cp2 = get_avg(cpu2);

    std::cout << "Average of " << iter << " runs: " <<  std::endl;
    std::cout << "n = " << dim  << std::endl;
    
    printf("Sequential: %9.3f \t %.3f\n", base, cp1);
    printf("Parallel:   %9.3f \t %.3f\n\n", fast, cp2);
    printf("Speedup:    %9.3f \n", base/fast);
    printf("Error:      %9.3f \n\n", error);
}