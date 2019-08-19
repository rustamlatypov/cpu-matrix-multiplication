#include <math.h>

#include "debug.cc"
#include "base.cc"
#include "fast.cc"
#include "stopwatch.h"
ppc::stopwatch sw;


static void gen(int ny, int nx, double* data) {
    std::mt19937 rng(42);
    const double a = std::numeric_limits<short>::max();
    std::uniform_real_distribution<double> unif(-a, a);
    std::generate(data, data+nx*ny, [&]{ return unif(rng); });
}


int main(int argc, const char** argv) {

    int dim;

    if (argc == 1) {
        dim = 10;
    } else if(argc == 2) {
        dim = std::stoi(argv[1]);
    } else {
        std::cout << "Usage:\n  run <dim>\n";
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

    sw.record();
    base_multiply(ny, nm, nx, D1.data(), D2.data(), base_result.data());
    print(ny, nx, base_result.data());
    sw.record();

    fast_multiply(ny, nm, nx, D1.data(), D2.data(), fast_result.data());
    print(ny, nx, fast_result.data());
    sw.record();
    sw.print();

    double cumsum = 0;
    for (int i = 0; i < ny*nx; i++) {
        cumsum += fabs(base_result[i] - fast_result[i]);
    }
    std::cout << "\nCumulative error: " << cumsum << std::endl;

}