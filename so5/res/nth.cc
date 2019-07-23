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
    int threshold = 100;
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
            nth_element(start, half, end);

            #pragma omp task
            quickSort(start, half, threads-1);
            #pragma omp task
            quickSort(half, end, threads-1);
        }
        else
        {
            nth_element(start, half, end);

            quickSort(start, half, 0);
            quickSort(half, end, 0);
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
