#include "mf.cc"

int main() {

    /*
    float data[30] = {
        2, 4, 2, 5, 3, 6,
        3, 7, 3, 9, 8, 4,
        1, 5, 7, 8, 5, 1,
        5, 5, 6, 6, 7, 3,
        8, 4, 3, 9, 8, 3};

    float result[30] = {0};
    int ny = 5;
    int nx = 6;
    */

    int ny = 2;
    int nx = 5;
    float data[10] = {0.13548, 0.83501, 0.96887, 0.22103, 0.30817,
                      0.54722, 0.18838, 0.99288, 0.99646, 0.96769};
    float result[10] = {0};


    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            std::cout << data[nx*j+i] << "  ";
        }
        std::cout << std::endl;
    }

    mf(ny, nx, 1, 2, data, result);

    std::cout << std::endl;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            std::cout << result[nx*j+i] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
