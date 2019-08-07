#include "is.h"
#include "vector.h"
#include "stopwatch.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <tuple>
using namespace std;
ppc::stopwatch sw;

struct Req {
    double h_value;
    int y0;
    int x0;
    int y1;
    int x1;
};

void toDouble4(int ny, int nx, const float* data, double4_t* D) {

    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            D[j*nx + i][0] = data[3*j*nx + 3*i + 0];
            D[j*nx + i][1] = data[3*j*nx + 3*i + 1];
            D[j*nx + i][2] = data[3*j*nx + 3*i + 2];
            D[j*nx + i][3] = 0;

        }
    }
}

void process(int ny, int nx, double4_t* D, double4_t* S) {

    #pragma omp parallel for schedule(static,1)
    for (int j = 0; j < ny+1; j++) {
        for (int i = 0; i < nx+1; i++) {

            double4_t sum = double4_0;

            for (int jj = 0; jj < j; jj++) {
                for (int ii = 0; ii < i; ii++) {

                sum += D[jj*nx + ii];

                }
            }

            S[j*(nx+1) + i] = sum;
        }
    }
}

void print(int ny, int nx, double4_t* S) {

    cout << endl;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            for (int k = 0; k < 4; k++) {
                cout << S[j * nx + i][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

}

Result segment(int ny, int nx, const float* data) {

    double4_t* D = double4_alloc(ny*nx);
    toDouble4(ny, nx, data, D);

    //sw.record();
    double4_t* S = double4_alloc((ny+1)*(nx+1));
    process(ny, nx, D, S);
    //sw.record();


    //print(ny, nx, S1);
    //print(ny+1, nx+1, S);

    double4_t vp = double4_0;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            vp += D[j*nx + i];

        }
    }


    int P = ny*nx;

    Req req_v[8] {0.0, 0, 0, 0, 0};

    #pragma omp parallel for schedule(static,1)
    for (int j0 = 0; j0 < ny; j0++) {
        int thread = omp_get_thread_num();
        for (int i0 = 0; i0 < nx; i0++) {

            for (int j1 = j0+1; j1 < ny+1; j1++) {
                for (int i1 = i0+1; i1 < nx+1; i1++) {

                    asm("#LOOP");

                    double4_t a = S[j1*(nx+1) + i1];
                    double4_t b = S[j0*(nx+1) + i1];
                    double4_t c = S[j1*(nx+1) + i0];
                    double4_t d = S[j0*(nx+1) + i0];

                    double4_t vx = a - b - c + d;
                    double4_t vy = vp - vx;

                    vx *= vx;
                    vy *= vy;

                    double X = (j1-j0)*(i1-i0);
                    double Y = P - X;

                    X = 1/X;
                    Y = 1/Y;

                    vx *= double4_t {X, X, X, 0};
                    vy *= double4_t {Y, Y, Y, 0};

                    double4_t h_v = vx + vy;

                    double h_value = h_v[0] + h_v[1] + h_v[2];

                    if (h_value > req_v[thread].h_value) {
                        req_v[thread] = Req {h_value, j0, i0, j1, i1};
                    }

                    asm("#LOOP");
                }
            }
        }
    }

    int indx = 0;
    Req current = req_v[indx];

    for (int m = 1; m < 8; m++) {
        if (req_v[m].h_value > current.h_value) {
            current = req_v[m];
        }
    }

    int y0 = current.y0;
    int x0 = current.x0;
    int y1 = current.y1;
    int x1 = current.x1;

    double4_t a = double4_0;
    for (int j = y0; j < y1; j++) {
        for (int i = x0; i < x1; i++) {

            a += D[j*nx + i];

        }
    }

    double4_t b = vp - a;

    double X = (y1-y0)*(x1-x0);
    a /= X;

    double Y = P - X;
    b /= Y;

    free(D);
    free(S);

    //sw.record();
    //sw.print();

    Result result { y0, x0, y1, x1,
                    {(float) b[0], (float) b[1], (float) b[2]},
                    {(float) a[0], (float) a[1], (float) a[2]} };
    return result;
}