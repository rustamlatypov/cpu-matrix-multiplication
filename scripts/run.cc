#include <math.h>
#include <limits>
#include "fast.cc"
#include "slow.cc"
#include "helper.h"


int main(int argc, const char** argv) {

    (void) print;
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

    std::vector<double> slow_time(iter);
    std::vector<double> fast_time(iter);
    std::vector<double> cpu1(iter);
    std::vector<double> cpu2(iter);

    int ny = dim; int nm = dim; int nx = dim;
    std::vector<double> D1(ny * nm);
    std::vector<double> D2(nm * nx);

    std::vector<double> slow_result(ny * nx);
    std::vector<double> fast_result(ny * nx);
    std::vector<double> cumerror(ny * nx);

    std::fill(slow_result.begin(), slow_result.end(), 0);
    std::fill(fast_result.begin(), fast_result.end(), 0);
    gen(ny, nm, D1.data());
    //print(ny, nm, D1.data());
    gen(nm, nx, D2.data());
    //print(nm, nx, D2.data());

    for (int i = 0; i < iter; i++)
    {   
        slow_time[i] = funcTime(slow_multiply, ny, nm, nx, D1.data(), D2.data(), slow_result.data());
        fast_time[i] = funcTime(fast_multiply, ny, nm, nx, D1.data(), D2.data(), fast_result.data());

        double error = verify_result(ny, nx, slow_result.data(), fast_result.data(), 1);
        cumerror[i] = error;

        double n = dim;

        cpu1[i] = calculate_cpu(n, slow_time[i]);
        cpu2[i] = calculate_cpu(n, fast_time[i]);
    }

    double slow = get_avg(slow_time);
    double fast = get_avg(fast_time);
    double error = get_avg(cumerror);

    double cp1 = get_avg(cpu1);
    double cp2 = get_avg(cpu2);

    printf("\ndim    %4d\niter   %4d\n", dim, iter);
    std::cout << "\t       time      cpu" << std::endl;

    printf("Sequential %9.3f \t %.4f\n", slow, cp1);
    printf("Parallel   %9.3f \t %.4f\n\n", fast, cp2);
    printf("Speedup    %9.3f \n", slow/fast);
    printf("Error      %9.3f \n\n", error);
}