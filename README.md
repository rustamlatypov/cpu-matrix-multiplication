# Parallelized CPU matrix multiplication 

Developed during July-August, 2019.

## Description

A console interface for testing and benchmarking two matrix multiplication implementations. Both use the same basic algorithm running O(n^3) for nxn matrices. The base implementation is sequential and the fast implementation is parallel. 

## Instructions for building and using


## Parallel implementation


## Results
Linux g++-8, -march=native -O2 -fopenmp 
Intel Xeon E3-1230v5 Skylake 3.4–3.8 GHz

```
n = 4000
sequential:    388.01
parallel:      0.9805	ratio: 395.8

n = 5000
sequential:    825.70
parallel:      2.0379   ratio: 405.2

n = 6000
sequential:    1498.6 
parallel:      3.6714	ratio: 408.2
  
n = 7000
sequential:    2604.8
parallel:      6.7220	ratio: 387.5    

n = 8000
sequential:    5037.0
parallel:      12.383 	ratio: 406.8     
```

## Author

[Rustam Latypov](mailto:rustam.latypov@aalto.fi)
