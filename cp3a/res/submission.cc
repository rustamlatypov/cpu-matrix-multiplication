#include <cmath>
#include <iostream>
#include <vector>
#include <immintrin.h>
#include <tuple>
#include <algorithm>
#include "vector.h"
#include "cp.h"
#include "stopwatch.h"
ppc::stopwatch sw;


double4_t* process(const float* data_, int ny, int nx, const int nb, const int na, const int ncd) {

    // input data with room for horizontal and vertical padding
    double4_t* data = double4_alloc(ncd*na);

    // horizontal padding
#pragma omp parallel for
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < na; i++) {
            for (int k = 0; k < nb; k++) {

                int pos = i * nb + k;
                data[na*j + i][k] = pos < nx ? data_[nx*j + pos] : 0;

            }
        }
    }


    // vertical padding
#pragma omp parallel for
    for (int j = ny; j < ncd; j++) {
        for (int i = 0; i < na; i++) {
            for (int k = 0; k < nb; k++) {

                data[na*j + i][k] = 0;

            }
        }
    }

    //print_v(ncd, mc, nb, data);

#pragma omp parallel for
    for (int j = 0; j < ny; j++) {

        double4_t sum4 = double4_0;

        for (int i = 0; i < na; i++) {
            sum4 += data[j*na+i];
        }

        double avg = 0;
        for (int i = 0; i < nb; i++) avg += sum4[i];
        avg /= nx;

        //---------------------------

        double4_t avg_v = double4_0;
        for (int k = 0; k < nb; k++) {
            avg_v[k] = avg;
        }

        for (int i = 0; i < na-1; i++) {
            data[j*na+i] -= avg_v;
        }

        avg_v = double4_0;
        int lim = nb - (na*nb-nx);
        for (int k = 0; k < lim; k++) avg_v[k] = avg;

        data[j*na + na-1] -= avg_v;


        //---------------------------

        sum4 = double4_0;

        for (int i = 0; i < na; i++) {
            double4_t e_v = data[j*na+i];
            sum4 += e_v * e_v;
        }

        double sum = 0;
        for (int i = 0; i < nb; i++) sum += sum4[i];
        sum = sqrt(sum);

        //---------------------------

        sum4 = double4_0;
        for (int k = 0; k < nb; k++) sum4[k] = sum;

        for (int i = 0; i < na; i++) {
            data[j*na+i] /= sum4;
        }
    }

    return data;
}

void correlate(int ny, int nx, const float* data_, float* result) {

    //sw.record();

    // elements per vector
    constexpr int NB = 4;

    // block A size
    constexpr  int A = 3;

    // block B size
    constexpr  int B = 6;

    // prefetch
    const int PRE = 1;


    // vectors per input row
    const int na = (nx + NB - 1) / NB;

    // number of rows
    int ncd = ny;
    while (ncd%A!=0 || ncd%B!=0) ncd++;

    // how many blocks of rows V
    const int ncV = ncd / B;
    // how many blocks of rows H
    const int ncH = ncd / A;

    double4_t* data = process(data_, ny, nx, NB, na, ncd);


    //sw.record();

    //--------------------

    //print_v(ncd, na, nb, data);

    std::vector<std::tuple<int,int,int>> rows;

    for (int j = 0; j < ncH; j++) {
        for (int i = (j*A+1)/B; i < ncV; i++) {
            int ij = _pdep_u32(j, 0x55555555) | _pdep_u32(i, 0xAAAAAAAA);
            rows.push_back(std::make_tuple(ij, j, i));
        }
    }

    std::sort(rows.begin(), rows.end());

    //sw.record();

    #pragma omp parallel for schedule(dynamic, 1)
    for (unsigned int m = 0; m < rows.size(); m++) {

        std::tuple<int,int,int> pair = rows[m];

        int j = std::get<1>(pair);
        int i = std::get<2>(pair);
        //std::cout << j << i << std::endl;

        double4_t block[A][B];

        for (int jd = 0; jd < A; jd++) {
            for (int id = 0; id < B; id++) {

                block[jd][id] = double4_0;

            }
        }

        // lets go
        for (int k = 0; k < na; k++) {

            asm("#foo");
            for (int id = 0; id < B; id++) {

                __builtin_prefetch(&data[na*(i * B + id) + k + PRE]);

                double4_t temp8 = data[na*(i * B + id) + k];

                for (int jd = 0; jd < A; jd++) {

                    block[jd][id] += data[na*(j * A + jd) + k] * temp8;

                }
            }
            asm("#bar");
        }

        for (int jd = 0; jd < A; jd++) {
            for (int id = 0; id < B; id++) {

                int jj = j * A + jd;
                int ii = i * B + id;

                if (ii < ny && jj < ny && ii > jj) {

                    double sum = 0;
                    for (int kk = 0; kk < NB; kk++) sum += block[jd][id][kk];
                    result[jj*ny+ii] = sum;

                }

            }
        }
    }

    for(int i = 0; i < ny; i++) result[i*ny+i] = 1;

    free(data);
    //sw.record();
    //sw.print();
}