# Parallel CPU matrix multiplication 

Developed during June-August, 2019.



## Description

A console interface for testing and benchmarking a optimized parallel matrix multiplication implementation. It is benchmarked against a naive sequential implementation. Both implementations use the same basic algorithm running in ``O(n^3)`` for square matrices of dimension ``n`` and use double-precision.


## Platform

Ubuntu 18.04.3, GCC 8 <br/> 
-std=c++17 -march=native -O2 -fopenmp <br/>
Intel Xeon E3-1230v5 Skylake 3.4–3.8 GHz


## Building and running

Built for a multicore linux system that supports 256-bit wide SIMD operations. Use `make -j` to build binaries and `make clean` to clean up the directory. When built there are four commands available: `./run <dim> <iter>`, `./test <ny> <nm> <nx> <verbose>`, `./benchmark <dim> <iter>` and `./help`. Assembly code for file x can be produced by running `make x.asm1` and `make x.asm2`. Either none or all parameters should be given to the following commands.

- `./run <dim> <iter>` default: dim=1000, iter=3 <br/>
Runs both implementations on the same matrices for `iter` times and outputs the average running times, cpu usage, the speedup obtained and the error term.

- `./test <ny> <nm> <nx> <verbose>` default: ny,nm,nx in {5,1500}, print=0 <br/>
Runs both implementations on different matrices and different combinations of ny, nm and nx. Dimension wise the multiplication is `(ny*nm)(nm*nx)`. Each combination is run 10 times and the error term is accumulated and outputed. Each test passes if the accumulated error is below 1e-3. Printing can be enabled using verbose=1. 

- `./benchmark <dim> <iter>` default: dim=3000, iter=10 <br/>
Runs the parallel implementation on the same square matrices of dimension `dim` for `iter` times and outputs running times, cpu usages and the their averages.

- `./help` <br/>
Expains in the commands in short.

### Error term
The error term is defined to be the sum of the element wise absolute difference of the two result matrices produced by the implementations.

### CPU usage
With FMA instructions the platform is able to perform ≈230x10^9 double-precision floating point operations per second. Since matrix multiplication takes 2n^3 floating point operations, the cpu usage for running time `t` is given by 2*n^3/t/230*10^9. 


## Parallel implementation

In matrix multiplication memory access is the bottleneck rather than processing power. So in addition to multicore processing and SIMD, a optimized memory access pattern is necessary.

Working with doubles and SIMD requires 32-byte memory alignment and a vector framework. These are provided in ``vector.h`` with type ``double4_t`` holding 4 doubles, 8 bytes each. Multicore processing is handled by OpenMP.

Let A and B be of type ``double*`` representing matrices as a row wise array. The goal is to produce matrix A x B = C. As preprocessing, A and the transpose of B are transformed into a type ``double4_t*`` representation with vertical vectors and 0 valued vertical padding. Although the vectors run vertically, the memory layout is such that is goes through the *rows* of the vertical vectors, from top to bottom and from left to right. 

This memory layout enables the efficient use of the outer product. The first vector row of A and the first vector row of B can be used to accumulate a 4x4=16 sized block with repeated outer products. This block can then by assigned to be the left top corner of matrix C. Similarly, any 16 sized block of C can be calculated by scanning the correct rows of A and B. 

Following the same logic, even a 8x8=64 and 12x12=144 sized blocks can be used. The block size controls the tradoff between register/L1/L2/L3 reuse and a 8x8=64 sized block is optimal for this setup. 

Both padding and the main execution loop are wrapped with ``#pragma omp parallel for`` for multicore processing since all threads should recieve similar loads. 



## Remarks

On some computers the compiler has difficulties producing FMA instructions resulting in rounding errors. Since large numbers are used, rounding errors accumulate and result in a high error term even though the implementation is correct. This can be fixed by compiler optimization. 


## Results

The results are captured using `./run n 2`, with different n values. 

```

lowerbound = 2*n^3/230*10^9

n = 3000
Sequential:   134.492 
Parallel:       0.353 
Speedup:      381.372 

n = 4000
Sequential:   366.784 
Parallel:       0.840 
Speedup:      436.670

n = 5000
Sequential:   769.893 
Parallel:       1.644 
Speedup:      468.347 

n = 6000
Sequential:  1435.166 
Parallel:       2.799 
Speedup:      512.822

n = 7000
Sequential:  2466.168 
Parallel:       4.764 
Speedup:      517.708

n = 8000
Sequential:  5086.581
Parallel:       7.285
Speedup:      698.271



n = 3000
Sequential:   134.710
Parallel:       0.353
Speedup:      381.489

n = 4000
Sequential:   366.686
Parallel:       0.855
Speedup:      429.095

n = 5000
Sequential:   777.599
Parallel:       1.656
Speedup:      469.597

n = 6000
Sequential:  1429.605
Parallel:       3.031
Speedup:      471.718

n = 7000
Sequential:  2458.338
Parallel:       4.759
Speedup:      516.607

```

## Author

[Rustam Latypov](mailto:rustam.latypov@aalto.fi)