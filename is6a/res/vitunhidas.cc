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

void print(int ny, int nx, const float* data) {

    cout << endl;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            for (int k = 0; k < 3; k++)
            {
                cout << data[j*nx*3 + i + k] << " ";
            }
            cout << endl;
        }
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

    //print(ny, nx, data);
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
    Req req_v[8] = {0.0, 0, 0, 0, 0};

    int F = 8;


    //#pragma omp parallel for schedule(dynamic,1)
    for (int X1 = 1; X1 <= ny; X1++) {
        int thread = omp_get_thread_num();
        for (int X2 = 1; X2 <= nx; X2++) {

            float X = X1*X2;
            float Y = P - X;

            float A = (X+Y)/(X*Y);
            float B = -2*vp/Y;
            float C = vp2/Y;

            vector<tuple<int, int, int, int, float>> visit;

            for (int j0 = 0; j0 <= ny-X1; j0++) {
                for (int i0 = 0; i0 <= nx-X2; i0++)
                {
                    int j1 = j0+X1;
                    int i1 = i0+X2;

                    float vx = S[j1*(nx+1) + i1]
                               - S[j0*(nx+1) + i1]
                               - S[j1*(nx+1) + i0]
                               + S[j0*(nx+1) + i0];

                    visit.push_back(make_tuple(j0, i0, j1, i1, vx));
                }
            }

            int left = visit.size()%F;

            if (left != 0)
            {
                for (int k = 0; k < (F-left); k++) visit.push_back(make_tuple(0,0,0,0,0.0));
            }

            /*
            for (auto e : visit) {
                cout << get<0>(e)
                        << " " << get<1>(e)
                        << " " << get<2>(e)
                        << " " << get<3>(e)
                        << " " << get<4>(e) << endl;
            }
            printf("\n");
            */

            int nv = visit.size()/F;


            float8_t* h_value = float8_alloc(nv);

            for (int k = 0; k < nv; k++) {
                for (int m = 0; m < F; m++)
                {
                    h_value[k][m] = get<4>(visit[k*F + m]);
                }
            }

            for (int k = 0; k < nv; k++)
            {
                h_value[k] = (h_value[k]*A + B)*h_value[k] + C;
            }


            if (left != 0)
            {
                for (int k = left; k < F; k++)
                {
                    h_value[nv-1][k] = -1;
                }
            }

            /*
            for (int k = 0; k < nv; k++){
                for (int m = 0; m < F; m++) {
                    cout << h_value[k][m] << endl;
                }
            }*/

            for (int k = 0; k < nv; k++) {
                for (int m = 0; m < F; ++m) {
                    if (h_value[k][m] > req_v[thread].h_value && !isinf(h_value[k][m]))
                    {
                        req_v[thread].h_value = h_value[k][m];
                        req_v[thread].y0 = get<0>(visit[k*F + m]);
                        req_v[thread].x0 = get<1>(visit[k*F + m]);
                        req_v[thread].y1 = get<2>(visit[k*F + m]);
                        req_v[thread].x1 = get<3>(visit[k*F + m]);
                    }
                }
            }
        }
    }

    Req current = req_v[0];

    for (int m = 1; m < 8; m++) {
        if (req_v[m].h_value > current.h_value) {
            current = req_v[m];
        }
    }

    int y0 = current.y0;
    int x0 = current.x0;
    int y1 = current.y1;
    int x1 = current.x1;

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