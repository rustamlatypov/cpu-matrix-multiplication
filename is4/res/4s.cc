#include "is.h"
#include "vector.h"
#include "stopwatch.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <tuple>
#include <math.h>
#include <cmath>
using namespace std;
ppc::stopwatch sw;

struct Req {
    double h_value;
    int y0;
    int x0;
    int y1;
    int x1;
};

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

    for (int j = 0; j < ny+1; j++) {
        S[j*(nx+1)] = double4_0;
    }

    for (int i = 0; i < nx+1; i++) {
        S[i] = double4_0;
    }

    for (int j = 1; j < ny+1; j++) {
        for (int i = 1; i < nx+1; i++) {

            double4_t sum = S[(j-1)*(nx+1) + (i)];

            for (int ii = 0; ii < i; ii++) {
                sum += D[(j-1)*nx + ii];
            }

            S[j*(nx+1) + i] = sum;
        }
    }
}


Result segment(int ny, int nx, const float* data) {

    double4_t* D = double4_alloc(ny*nx);
    toDouble4(ny, nx, data, D);

    //sw.record();
    double4_t* S = double4_alloc((ny+1)*(nx+1));
    process(ny, nx, D, S);
    //sw.record();


    //print(ny, nx, D);
    //print(ny+1, nx+1, S);

    double4_t vp = double4_0;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {

            vp += D[j*nx + i];

        }
    }

    double P = ny*nx;
    Req req_v[8] {0.0, 0, 0, 0, 0};


    #pragma omp parallel for schedule(dynamic,1)
    for (int X1 = 1; X1 <= ny; X1++) {
        int thread = omp_get_thread_num();
        for (int X2 = 1; X2 <= nx; X2++) {

            double X = X1*X2;
            double Y = P - X;

            X = 1/X;
            Y = 1/Y;

            double4_t X_v = {X, X, X, 0};
            double4_t Y_v = {Y, Y, Y, 0};

            for (int j0 = 0; j0 <= ny-X1; j0++) {
                for (int i0 = 0; i0 <= nx-X2; i0++) {

                    int j1 = j0+X1;
                    int i1 = i0+X2;

                    double4_t vx = S[j1*(nx+1) + i1]
                            - S[j0*(nx+1) + i1]
                            - S[j1*(nx+1) + i0]
                            + S[j0*(nx+1) + i0];
                    double4_t vy = vp - vx;

                    vx *= vx;
                    vy *= vy;

                    vx *= X_v;
                    vy *= Y_v;

                    double4_t h_v = vx + vy;
                    double h_value = h_v[0] + h_v[1] + h_v[2];

                    if (h_value > req_v[thread].h_value)
                    {
                        req_v[thread] = Req {h_value, j0, i0, j1, i1};
                    }
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