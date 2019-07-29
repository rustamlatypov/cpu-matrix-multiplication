#include "debug.cpp"
#include "cp.cc"
#include "base.cc"

int main() {

    int ny1 = 3;
    int nx1 = 8;

    int ny2 = 8;
    int nx2 = 10;

    std::vector<float> D1(ny1 * nx1);
    std::fill(D1.begin(), D1.end(), 0);

    std::vector<float> D2(ny2 * nx2);
    std::fill(D2.begin(), D2.end(), 0);

    std::vector<float> result(ny1 * nx2);
    std::fill(result.begin(), result.end(), 0);


    generate(ny1, nx1, D1.data());
    print(ny1, nx1, D1.data());

    generate(ny2, nx2, D2.data());
    print(ny2, nx2, D2.data());

    //correlate(ny, nx, data.data(), result.data());
    base_multiply(ny1, nx1, D1.data(), ny2, nx2, D2.data(), result.data());
    //print(ny1, nx2, result.data());

    multiply(ny1, nx1, D1.data(), ny2, nx2, D2.data(), result.data());
}