#include "debug.cpp"
#include "base.cc"
#include "fast.cc"
#include "stopwatch.h"
ppc::stopwatch sw;

int main() {
    
    int s = 1000;

    int ny = s; int nm = s; int nx = s;

    std::vector<double> D1(ny * nm);
    std::fill(D1.begin(), D1.end(), 0);

    std::vector<double> D2(nm * nx);
    std::fill(D2.begin(), D2.end(), 0);

    std::vector<double> result_base(ny * nx);
    std::fill(result_base.begin(), result_base.end(), 0);

    std::vector<double> result_fast(ny * nx);
    std::fill(result_fast.begin(), result_fast.end(), 0);


    generate(ny, nm, D1.data());
    //print(ny, nm, D1.data());

    generate(nm, nx, D2.data());
    //print(nm, nxm, D2.data());

    sw.record();
    base_multiply(ny, nm, nx, D1.data(), D2.data(), result_base.data());
    //print(ny, nx, result_base.data());
    sw.record();

    multiply(ny, nm, nx, D1.data(), D2.data(), result_fast.data());
    //print(ny, nx, result_fast.data());
    sw.record();
    sw.print();

    double cumsum = 0;
    for (int i = 0; i < ny1*nx2; i++) {
        cumsum += abs(result_base[i] - result_fast[i]);
    }
    std::cout << "\nCumulative error: " << cumsum << std::endl;
    

    /*
    int x = 8000;
    int y = 8000;

    double4_t d1 = {0.99, 0.99, 0.99, 0.99};
    double d2 = 0.99;

    sw.record();
    for (int i = 0; i < x*y/4; i++) {
        d1 += d1*d1;
    }

    sw.record();
    for (int i = 0; i < x*y; i++) {
        d2 += d2*d2;
    }

    std::cout << std::endl << d1[0] << ' ' << d1[1] << ' ' << d1[2] << ' ' << d1[3] << std::endl;
    std::cout << d2 << std::endl;

    sw.record();
    sw.print();
    */
}