#include "debug.cpp"
#include "cp.cc"
#include "base.cc"
#include "stopwatch.h"
ppc::stopwatch sw;

int main() {
    
    int s = 500;

    int ny1 = s; int nx1 = s;
    int ny2 = s; int nx2 = s;

    std::vector<double> D1(ny1 * nx1);
    std::fill(D1.begin(), D1.end(), 0);

    std::vector<double> D2(ny2 * nx2);
    std::fill(D2.begin(), D2.end(), 0);

    std::vector<double> result_base(ny1 * nx2);
    std::fill(result_base.begin(), result_base.end(), 0);

    std::vector<double> result_fast(ny1 * nx2);
    std::fill(result_fast.begin(), result_fast.end(), 0);


    generate(ny1, nx1, D1.data());
    //print(ny1, nx1, D1.data());

    generate(ny2, nx2, D2.data());
    //print(ny2, nx2, D2.data());

    sw.record();
    base_multiply(ny1, nx1, D1.data(), ny2, nx2, D2.data(), result_base.data());
    //print(ny1, nx2, result_base.data());
    sw.record();

    multiply(ny1, nx1, D1.data(), ny2, nx2, D2.data(), result_fast.data());
    //print(ny1, nx2, result_fast.data());
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