#include "debug.cpp"
#include "cp.cc"

int main() {

    int ny = 7;
    int nx = 9;

    std::vector<float> data(ny * nx);
    std::fill(data.begin(), data.end(), 0);
    std::vector<float> result(ny * ny);
    std::fill(result.begin(), result.end(), 0);

    generate(ny, nx, data.data());
    print(ny, nx, data.data());
    correlate(ny, nx, data.data(), result.data());
    print(ny, ny, result.data());
}