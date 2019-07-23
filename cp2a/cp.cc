#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "cp.h"

void correlate(int ny, int nx, const float* data_, float* result) {

    constexpr int P = 4;
    int na = (nx + P - 1) / P;
    int nab = na*P;

    // input data, padded
    std::vector<double> data(ny*nab, 0);


    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            data[nab*j + i] = data_[nx*j + i];
        }
    }


    for (int j = 0; j < ny; j++) {

        double e;

        // running sum array
        double sum_v[P] = {0};
        // actual sum
        double sum = 0;

        //------------------------


        for (int i = 0; i < na; i++)
        {
            for (int k = 0; k < P; k++)
            {
                sum_v[k] += data[j*nab + i*P + k];
            }
        }

        for (int k = 0; k < P; ++k) sum += sum_v[k];

        //------------------------


        double avg = sum/nx;


        for (int i = 0; i < nx; i++) {
            data[j*nab+i] -= avg;
        }

        //------------------------


        memset(sum_v, 0, P*sizeof(double));
        sum = 0;

        for (int i = 0; i < na; i++)
        {
            for (int k = 0; k < P; k++)
            {
                e = data[j*nab + i*P + k];
                sum_v[k] += e*e;
            }
        }

        for (int k = 0; k < P; ++k) sum += sum_v[k];

        //------------------------


        sum = sqrt(sum);

        for (int i = 0; i < nx; i++) {
            data[j*nab+i] /= sum;
        }
    }



    for (int i = 0; i < ny; i++) {

        for (int j = i+1; j < ny; j++) {

            // running sum array
            double sum_v[P] = {0};
            // actual sum
            double sum = 0;

            for (int l = 0; l < na; l++) {

                for (int k = 0; k < P; k++)
                {
                    sum_v[k] += data[i*nab + l*P + k] * data[j*nab + l*P + k];

                }
            }
            for (int u = 0; u < P; u++) sum += sum_v[u];
            result[i*ny+j] = sum;
        }
        result[i*ny+i] = 1;
    }

}
