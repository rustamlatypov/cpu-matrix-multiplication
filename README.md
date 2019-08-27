# Parallel CPU matrix multiplication 

Developed during June-August, 2019.


## Description

A console interface for testing and benchmarking two matrix multiplication implementations. Both use the same basic algorithm running O(n^3) for matrices of dimension n. The base implementation is sequential and the fast implementation is parallel.


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
???


## Remarks
On some computers the compiler has difficulties producing FMA instructions resulting in rounding errors. Since large numbers are used, rounding errors accumulate and result in a high error term even though the implementation is correct. This can be fixed by compiler optimization. 


## Results
The results are captured using `./run n 2`, with different n values. 

Ubuntu 18.04.3 <br/>
g++-8 -march=native -O2 -fopenmp <br/>
Intel Xeon E3-1230v5 Skylake 3.4–3.8 GHz

```
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
