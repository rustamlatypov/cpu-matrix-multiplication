#include "so.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include "stopwatch.h"
#include "debug.cpp"
ppc::stopwatch sw;
using namespace std;


data_t medianOf3(data_t* right, data_t* half, data_t* left)
{
    data_t a = *right;
    data_t b = *half;
    data_t c = *(left-1);

    if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
    return c;
}


void quickSort(data_t* right, data_t* left, int threads)
{
    int threshold = 7000;
    int length = (left-right);
    data_t* half = right + length/2;

    if (length <= threshold)
    {
        sort(right, left);
    }
    else
    {
        if (threads > 0)
        {
            data_t pv = medianOf3(right, half, left);
            data_t* cut1 = partition(right, left, [pv](const auto& em){ return em < pv; });
            data_t* cut2 = partition(cut1, left, [pv](const auto& em){ return em <= pv; });

            #pragma omp task
            quickSort(right, cut1, threads-1);
            #pragma omp task
            quickSort(cut2, left, threads-1);
        }
        else
        {
            data_t pv = medianOf3(right, half, left);
            data_t* cut1 = partition(right, left, [pv](const auto& em){ return em < pv; });
            data_t* cut2 = partition(cut1, left, [pv](const auto& em){ return em <= pv; });
            quickSort(right, cut1, 0);
            quickSort(cut2, left, 0);
        }
    }
}


void psort(int n, data_t* data)
{
    int threads = omp_get_max_threads();

    #pragma omp parallel
    #pragma omp single
    quickSort(data, data + n, threads);
}
