#include "debug.cpp"
#include "cp.cc"
#include "base.cc"
#include "stopwatch.h"
ppc::stopwatch sw;

int main() {

    int ny1 = 500;
    int nx1 = 500;

    int ny2 = 500;
    int nx2 = 500;

    std::vector<float> D1(ny1 * nx1);
    std::fill(D1.begin(), D1.end(), 0);

    std::vector<float> D2(ny2 * nx2);
    std::fill(D2.begin(), D2.end(), 0);

    std::vector<float> result_base(ny1 * nx2);
    std::fill(result_base.begin(), result_base.end(), 0);

    std::vector<float> result_fast(ny1 * nx2);
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
    std::cout << cumsum << std::endl;

}