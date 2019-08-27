#include <math.h>
#include <limits>

#include "fast.cc"
#include "base.cc"
#include "helper.h"


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

    std::fill(base_result.begin(), base_result.end(), 0);
    std::fill(fast_result.begin(), fast_result.end(), 0);
    gen(ny, nm, D1.data());
    //print(ny, nm, D1.data());
    gen(nm, nx, D2.data());
    //print(nm, nx, D2.data());

    for (int i = 0; i < iter; i++)
    {   
        /*
        time_point t1 = c::now();
        base_multiply(ny, nm, nx, D1.data(), D2.data(), base_result.data());
        //print(ny, nx, base_result.data());
        time_point t2 = c::now();
        base_time[i] = (t2-t1).count() / double(1E9);*/

        base_time[i] = funcTime(base_multiply(ny, nm, nx, D1.data(), D2.data(), base_result.data()));

        /*
        t1 = c::now();
        fast_multiply(ny, nm, nx, D1.data(), D2.data(), fast_result.data());
        //print(ny, nx, fast_result.data());
        t2 = c::now();
        fast_time[i] = (t2-t1).count() / double(1E9);*/

        fast_time[i] = funcTime(fast_multiply(ny, nm, nx, D1.data(), D2.data(), fast_result.data()));

        double error = validate(ny, nx, base_result.data(), fast_result.data(), 1);
        cumerror[i] = error;
    }

    double base = get_avg(base_time);
    double fast = get_avg(fast_time);
    double error = get_avg(cumerror);

    std::cout << "Average of " << iter << " runs: " <<  std::endl;
    std::cout << "n = " << dim  << std::endl;
    
    printf("Sequential: %9.3f \n", base);
    printf("Parallel:   %9.3f \n", fast);
    printf("Speedup:    %9.3f \n", base/fast);
    printf("Error:      %9.3f \n\n", error);


}