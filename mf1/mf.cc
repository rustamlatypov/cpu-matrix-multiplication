#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include "mf.h"


void mf(int ny, int nx, int hy, int hx, const float* in, float* out) {
    
    std::vector<float> v = {};

    int j_min;
    int j_max;
    int i_min;
    int i_max;
    float e;

    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            
            if (j-hy < 0) j_min = 0; else j_min = j-hy;
            
            if (j+hy >= ny) j_max = ny; else j_max = j+hy+1;
            
            
            if (i-hx < 0) i_min = 0; else i_min = i-hx;
            
            if (i+hx >= nx) i_max = nx; else i_max = i+hx+1;
            
            
            for (int j1 = j_min; j1 < j_max; j1++) {
                for (int i1 = i_min; i1 < i_max; i1++) {
                    
                    v.push_back(in[j1*nx+i1]);
                    
                }
            }
            
            
            if (v.size()%2 == 1) {
                nth_element(v.begin(), v.begin() + v.size()/2, v.end());
                e = v[v.size()/2];
            } else {
                nth_element(v.begin(), v.begin() + v.size()/2, v.end());
                float a = v[v.size()/2];
                
                nth_element(v.begin(), v.begin() + v.size()/2-1, v.end());
                float b = v[v.size()/2-1];
                
                e = (a+b) / 2;
            }
            
            out[j*nx+i] = e;
            
            v.clear();
            
        }
    }
}
