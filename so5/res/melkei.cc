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

data_t medianOf3(data_t* start, data_t* half, data_t* end)
{
    data_t a = *start;
    data_t b = *half;
    data_t c = *(end-1);
    data_t arr[3] = { a, b, c };

    sort(arr, arr+3);
    *start = arr[0];
    *half = arr[1];
    *(end-1) = arr[2];

    return arr[1];
}

void quickSort(data_t* start, data_t* end, int threads)
{
    int threshold = 4;
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

            data_t pivot = medianOf3(start, half, end);
            auto cut1 = partition (start, end, [pivot](const auto& em){ return em < pivot; });
            auto cut2 = partition (cut1, end, [pivot](const auto& em){ return em <= pivot; });

            #pragma omp task
            quickSort(start, cut1, threads-1);
            #pragma omp task
            quickSort(cut2, end, threads-1);
        }
        else
        {
            data_t pivot = medianOf3(start, half, end);
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
