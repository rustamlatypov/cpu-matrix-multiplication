# Parallel CPU matrix multiplication 

Developed during June-August, 2019.


## Description

A console interface for testing and benchmarking two matrix multiplication implementations. Both use the same basic algorithm running O(n^3) for matrices of dimension n. The base implementation is sequential and the fast implementation is parallel. Both implementations work with double values exclusively. 


## Building and running

Built for a multicore linux system that supports AVX operations. Use `make -j` to build binaries and `make clean` to clean up the directory. When built there are three command available: `./run <dim> <iter>`, `./test <ny> <nm> <nx>` and `./benchmark <dim> <iter>`. Assembly code for file x can be produced by running `make x.asm1` and `make x.asm2`.

- `./run <dim> <iter>` default: dim=1000, iter=3 <br/>
Runs both implementations on the same matrices for `iter` times and outputs the average running times, the speedup and the error term.

- `./test <ny> <nm> <nx>` default: ny,nm,nx in {5,1500} <br/>
Runs both implementations on different matrices and different combinations of ny, nm and nx. Dimension wise the multiplication is (ny•nm)(nm•nx). Each combination is run 10 times and the error term is accumulated and outputed. Each test passes if the accumulated error is below 1e-3. 

- `./benchmark <dim> <iter>` default: dim=3000, iter=10 <br/>
Runs the parallel implementation on the same matrices of dimensions `dim` for `iter` times and outputs the running times and the total average.

### Error term
The error term is defined to be the sum of the element wise absolute difference of the two result matrices produced by the implementations.


## Parallel implementation
Working with doubles and AVX requires 32-byte memory alignment and a vector framework. These are provided in ``vector.h`` with type ``double4_t`` holding 4 doubles, 8 bytes each. Multicore processing is handled by OpenMP.

Let A and B be of type ``double*`` representing matrices as a row wise array. The goal is to produce matrix A x B = C. As preprocessing, A and the transpose of B are transformed into a type ``double4_t*`` representation with vertical vectors and 0 valued vertical padding. Although the vectors run vertically, the memory layout is such that is goes through the *rows* of the vertical vectors, from top to bottom and from left to right. C is calculated one 16 vector (64 value) block at a time moving vertically up to bottom and left to right.

The logic behind this lies in the outer product. Using the memory layout mensioned previously, the first vector 

Using this approach reuses registers and memory caches efficiently reducing memory access bottlenecks.


Both padding and the main execution loop are wrapped with ``#pragma omp parallel for`` for multicore processing since all threads should recieve similar loads. 



## Remarks
On some computers the compiler has difficulties producing FMA instructions resulting in rounding errors. Since large numbers are used, rounding errors accumulate and result in a high error term even though the implementation is correct. This can be fixed by compiler optimization. 


## Results
The results are captured using `./run n 2`, with different n values. 

Ubuntu 18.04.3 <br/>
g++-8 -march=native -O2 -fopenmp <br/>
Intel Xeon E3-1230v5 Skylake 3.4–3.8 GHz

```
n = 3000
Sequential:   134.710
Parallel:       0.353
Speedup:      381.489

n = 4000
Sequential:   366.686
Parallel:       0.855
Speedup:      429.095

n = 5000
Sequential:   838.840
Parallel:       1.664
Speedup:      504.129



n = 6000
Sequential:  1419.769
Parallel:       2.971
Speedup:      477.899

n = 6000
Sequential:  1429.605
Parallel:       3.031
Speedup:      471.718



n = 2000
Sequential:    31.942 
Parallel:       0.135
Speedup:      236.488

n = 3000
Sequential:   135.225 
Parallel:       0.435 
Speedup:      310.608

n = 4000
Sequential:   432.167 
Parallel:       0.971 
Speedup:      444.986

n = 5000
Sequential:   921.171 
Parallel:       2.141 
Speedup:      430.281

n = 6000
Sequential:  1671.762 
Parallel:       3.753 
Ratio:        445.461

n = 7000
Sequential:  2960.984 
Parallel:       6.834 
Speedup:      433.245 

n = 8000
Sequential:  5149.222 
Parallel:      11.882 
Speedup:      432.363 
```

## Author

[Rustam Latypov](mailto:rustam.latypov@aalto.fi)