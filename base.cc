#include <iostream>
#include "cp.h"

void base_multiply(int ny1, int nx1, const float* D1,
        int ny2, int nx2, const float* D2, float* result) {

    for (int i = 0; i < ny1; i++) {
        for (int j = 0; j < nx2; j++) {

            double e = 0;
            for (int k = 0; k < nx1; k++) {
                e += D1[i*nx1+k] * D2[k*nx2+j];
            }
            //std::cout << e << std::endl;
            result[i*nx2+j] = e;
        }
    }
}