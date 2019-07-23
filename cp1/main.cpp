#include "debug.cpp"
#include "cp.cc"

int main() {
    /*
    int ny = 8;
    int nx = 8;

    std::vector<float> data(ny * nx);
    std::fill(data.begin(), data.end(), 0);
    std::vector<float> result(ny * ny);
    std::fill(result.begin(), result.end(), 0);

    generate(ny, nx, data.data());
    print(ny, nx, data.data());
     */
    std::vector<float> data = {-0.171,  0.386,
    0.356, -0.500,
    -0.245,  1.501,
    -0.171,  1.619,
    0.218,  0.426,
    -0.163, -0.207,
    0.065,  0.217,
    -0.260, -0.207};

    std::vector<float> result(8 * 8);

    correlate(8, 2, data.data(), result.data());
    print(8, 8, result.data());
}