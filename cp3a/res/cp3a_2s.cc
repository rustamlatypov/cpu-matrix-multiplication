#include <cmath>
#include <iostream>
#include <vector>
#include <immintrin.h>
#include <tuple>
#include <algorithm>
#include "vector.h"
#include "memory.h"
#include "cp.h"


double4_t* process(const float* data_, int ny, int nx,
        const int nb, const int na, const int ncd) {

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
        for (int i = 0; i < 4; i++) avg += sum4[i];
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
        for (int i = 0; i < 4; i++) sum += sum4[i];
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


    // elements per vector
    constexpr int nb = 4;

    // block vsize
    const int V = 8;

    // block hsize
    const int H = 3;

    // vectors per input row
    const int na = (nx + nb - 1) / nb;


    // number of rows
    int ncd = ny;
    while (ncd%V!=0 || ncd%H!=0) ncd++;

    // how many blocks of rows V
    const int ncV = ncd / V;
    // how many blocks of rows H
    const int ncH = ncd / H;

    double4_t* data = process(data_, ny, nx, nb, na, ncd);

    //--------------------

    //print_v(ncd, na, nb, data);

    #pragma omp parallel for schedule(dynamic,1)
    for (int j = 0; j < ncH; j++) {

        for (int i = (j*H+1)/V; i < ncV; i++) {

            asm("#foo");
            double4_t block[V][H];

            for (int jd = 0; jd < H; jd++) {
                for (int id = 0; id < V; id++) {

                    block[id][jd] = double4_0;

                }
            }

            // lets go
            for (int k = 0; k < na; k++) {

                for (int l = 0; l < V; l++) {

                    __builtin_prefetch(&data[na*(i * V + l) + k + 2]);

                    double4_t b8 = data[na*(i * V + l) + k];

                    for (int d = 0; d < H; d++) {

                        block[l][d] += data[na*(j * H + d) + k] * b8;

                    }
                }
            }

            for (int jd = 0; jd < H; jd++) {
                for (int id = 0; id < V; id++) {

                    int jj = j * H + jd;
                    int ii = i * V + id;

                    if (ii < ny && jj < ny && ii >= jj) {

                        double sum = 0;
                        for (int kk = 0; kk < nb; kk++) sum += block[id][jd][kk];
                        result[jj*ny+ii] = sum;

                    }

                }
            }
            asm("#bar");
        }
    }

    for(int i = 0; i < ny; i++) result[i*ny+i] = 1;

    free(data);
}