#include "so.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include "debug.cpp"
using namespace std;


void quickSort(data_t* start, data_t* end, int threads)
{
    int threshold = 10;
    int length = (end-start);
    data_t* half = end - (end-start)/2;

    if (length <= threshold)
    {
        sort(start, end);
    }
    else
    {
        if (threads > 0)
        {
            data_t pivot = *half;
            auto cut1 = partition (start, end, [pivot](const auto& em){ return em < pivot; });
            auto cut2 = partition (cut1, end, [pivot](const auto& em){ return em <= pivot; });

            #pragma omp task
            quickSort(start, cut1, 0);
            #pragma omp task
            quickSort(cut2, end, 0);
        }
        else
        {
            data_t pivot = *half;
            auto cut1 = partition (start, end, [pivot](const auto& em){ return em < pivot; });
            auto cut2 = partition (cut1, end, [pivot](const auto& em){ return em <= pivot; });

            quickSort(start, cut1, 0);
            quickSort(cut2, end, 0);
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
