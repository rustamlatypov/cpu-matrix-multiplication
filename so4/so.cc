#include "so.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include <tuple>
#include "stopwatch.h"
#include "debug.cpp"
ppc::stopwatch sw;
using namespace std;


void mergeSort(data_t* data, data_t* aux, int start, int end, int threads, int counter)
{
    int threshold = 10;
    int length = (end-start);
    int half = start + length/2;

    if (length <= threshold && counter%2==0)
    {
        sort(data + start, data + end);
    }
    else
    {
        if (threads>0)
        {
            #pragma omp task
            mergeSort(aux, data, start, half, threads-1, counter+1);
            #pragma omp task
            mergeSort(aux, data, half, end, threads-1, counter+1);
            #pragma omp taskwait
            merge(aux + start, aux + half, aux + half, aux + end, data + start);
        }
        else
        {
            mergeSort(aux, data, start, half, 0, counter+1);
            mergeSort(aux, data, half, end, 0, counter+1);
            merge(aux + start, aux + half, aux + half, aux + end, data + start);
        }
    }
}

void psort(int n, data_t* data)
{
    vector<data_t> aux(n);
    int threads = omp_get_max_threads();

    #pragma omp parallel
    #pragma omp single
    mergeSort(data, aux.data(), 0, n, threads, 0);
}