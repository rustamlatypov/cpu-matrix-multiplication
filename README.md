# Parallel CPU matrix multiplication 

Developed during June-September, 2019.



## Description

A console interface for testing and benchmarking an optimized parallel matrix multiplication implementation. It is benchmarked against a naive sequential implementation. Both implementations use the same basic **O(n^3)** algorithm for square matrices of dimension **n** and use double-precision.


## Platform

`-std=c++17 -march=native -O2 -fopenmp` <br/>
Ubuntu 18.04.3, GCC 7.4.0 <br/> 
Intel Xeon E3-1230v5 Skylake 3.4–3.8 GHz


## Building and running

Built for a multicore linux system that supports 256-bit wide SIMD operations. In `/scripts` Use `make -j` to build binaries and `make clean` to clean up the directory. When built, there are four commands available: `run <dim> <iter>`, `test <ny> <nm> <nx> <verbose>`, `benchmark <dim> <iter>` and `help`. Assembly code for file x can be produced by running `make x.asm1` and `make x.asm2`. Either none or all parameters should be given to the following commands.

- `run <dim> <iter>` default: `dim=1000`, `iter=3` <br/>
Runs both implementations on the same square matrices of dimension `dim` for `iter` times and outputs the average running times, cpu usages, the speedup obtained and the error term.

- `test <ny> <nm> <nx> <verbose>` default: `ny,nm,nx in {5,1500}`, `verbose=0` <br/>
Runs both implementations on different matrices and different combinations of `ny`, `nm` and `nx`. Dimension wise the multiplication is `(ny*nm)(nm*nx)`. Each combination is run **10** times and the error term is accumulated and outputted. Each test passes if the accumulated error is below **1e-3** (can be adjusted by changing `error_limit` in `/common/helper.h`). Matrices can be outputted by setting `verbose=1`. 

- `benchmark <dim> <iter>` default: `dim=3000`, `iter=10` <br/>
Runs the parallel implementation on the same square matrices of dimension `dim` for `iter` times and outputs running times, cpu usages and the their averages.

- `help` <br/>
Expains the commands in short.

### Error term
The error term is defined to be the sum of the element wise absolute difference of the two result matrices produced by the implementations. It is assumed that the sequential implementation is correct.

### CPU usage
Taking FMA instructions into account, the platform is able to perform **≈230G** double-precision floating point operations per second. Since matrix multiplication takes **2n^3** floating point operations, the CPU usage for running time **t** is given by **2n^3/t/230G**. The lower bound for a matrix multiplication is thus **2n^3/230G**. 

The CPU usages calculations use a hardcoded variable for this specific platform. If one wishes to adjust for their own system, variable `platform_spec` in `common/helper.h` should be changed to the systems number of double-precision floating point operations per second.


## Parallel implementation

In matrix multiplication memory access is the bottleneck rather than processing power. So in addition to multicore processing and SIMD, a optimized memory access pattern is necessary.

Working with doubles and SIMD requires 32-byte memory alignment and a vector framework. These are provided in ``vector.h`` with type ``double4_t`` holding 4 doubles, 8 bytes each. Multicore processing is handled by OpenMP.

Let A and B be of type ``double*`` representing matrices as a row wise array. The goal is to produce matrix A x B = C. As preprocessing, B is transformed into a type ``double4_t*`` representation with horizontal vectors and 0 valued horizontal padding. Although the vectors run horizontally, the memory layout is such that is goes through the *columns* of the horizontal vectors, from top to bottom and from left to right. 

This memory layout enables the efficient use of the outer product. Using the first four rows of A and the first column of horizontal vectors of B, a 4x4=16 sized block of C can be accumulated using the outer product. This block can then be copied to the the left top corner of C. Similarly, any 16 sized block of C can be computed by scanning the correct rows of A and the correct column of horizontal vectors of B. 


Following the same logic, even 8x8=64 or 12x12=144 sized blocks can be used. The block size controls the tradoff between register/L1/L2/L3 reuse. Experimentation showed that a 64 sized block is optimal for this platform. 


On top of this, 2x2 tiling is used to further optimize memory reuse in the large memory caches. So instead of computing 64 sized blocks, superblocks of 4x64 blocks are computed.  


Both transformation of B and the main execution loop are wrapped with ``#pragma omp parallel for`` for multicore processing since all threads should recieve similar loads. 



## Remarks

On some computers the compiler has difficulties producing FMA instructions resulting in rounding errors. Since large numbers are used, rounding errors accumulate and result in a high error term even though the implementation is correct. This can be fixed by tweaking the compiler parameters. 


## Results

The results are captured using `run n 2`, with different n values. 

```
n = 3000
Sequential      
Parallel        
Speedup         
CPU usage       

n = 4000
Sequential      
Parallel        
Speedup         
CPU usage       

n = 5000
Sequential      
Parallel        
Speedup         
CPU usage       

n = 6000
Sequential      
Parallel        
Speedup         
CPU usage       

n = 7000
Sequential      
Parallel        
Speedup         
CPU usage       

n = 8000
Sequential      
Parallel        
Speedup         
CPU usage       
```




## Author

[Rustam Latypov](mailto:rustam.latypov@aalto.fi)