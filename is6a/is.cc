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
    float h_value;
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

void toFloat(int ny, int nx, const float* data, float* D) {

    #pragma omp parallel for
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++)
        {
            D[j*nx + i] = data[3*j*nx + 3*i + 0];
        }
    }
}

void process(int ny, int nx, float* D, float* S) {

    for (int j = 0; j < ny+1; j++) {
        S[j*(nx+1)] = 0;
    }

    for (int i = 0; i < nx+1; i++) {
        S[i] = 0;
    }

    for (int j = 1; j < ny+1; j++) {
        for (int i = 1; i < nx+1; i++) {

            float sum = S[(j-1)*(nx+1) + (i)];

            for (int ii = 0; ii < i; ii++) {
                sum += D[(j-1)*nx + ii];
            }

            S[j*(nx+1) + i] = sum;
        }
    }
}


Result segment(int ny, int nx, const float* data) {

    vector<float> D(ny*nx);
    toFloat(ny, nx, data, D.data());

    //sw.record();
    vector<float> S((ny+1)*(nx+1));
    process(ny, nx, D.data(), S.data());
    //sw.record();

    //print(ny, nx, D);
    //print(ny+1, nx+1, S);

    float vp = 0;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++)
        {
            vp += D[j*nx + i];
        }
    }
    float vp2 = vp*vp;

    float P = ny*nx;
    Req best = {0.0, 0, 0, 0, 0};


    #pragma omp parallel for schedule(dynamic,1)
    for (int X1 = 1; X1 <= ny; X1++) {
        Req current = {0.0, 0, 0, 0, 0};
        for (int X2 = 1; X2 <= nx; X2++) {

            float X = X1*X2;
            float Y = P - X;

            float A = (X+Y)/(X*Y);
            float B = -2*vp/Y;
            float C = vp2/Y;

            for (int j0 = 0; j0 <= ny-X1; j0++) {
                for (int i0 = 0; i0 <= nx-X2; i0++) {

                    int j1 = j0+X1;
                    int i1 = i0+X2;

                    float vx = S[j1*(nx+1) + i1]
                               - S[j0*(nx+1) + i1]
                               - S[j1*(nx+1) + i0]
                               + S[j0*(nx+1) + i0];

                    float h_value = (vx*A + B)*vx + C;

                    if (h_value > current.h_value && !isinf(h_value))
                    {
                        current = Req {h_value, j0, i0, j1, i1};
                    }
                }
            }
        }
        #pragma omp critical
        {
            if (current.h_value > best.h_value) best = current;
        }
    }

    int y0 = best.y0;
    int x0 = best.x0;
    int y1 = best.y1;
    int x1 = best.x1;

    float a = 0;
    for (int j = y0; j < y1; j++) {
        for (int i = x0; i < x1; i++) {

            a += D[j*nx + i];

        }
    }

    float b = vp - a;

    float X = (y1-y0)*(x1-x0);
    a /= X;

    float Y = P - X;
    b /= Y;

    //sw.record();
    //sw.print();

    Result result { y0, x0, y1, x1,
                    {(float) b, (float) b, (float) b},
                    {(float) a, (float) a, (float) a} };
    return result;
}