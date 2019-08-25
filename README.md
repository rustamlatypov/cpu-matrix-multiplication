# Parallelized CPU matrix multiplication 

Developed during June-August, 2019.


## Description

A console interface for testing and benchmarking two matrix multiplication implementations. Both use the same basic algorithm running O(n^3) for matrices of dimension n. The base implementation is sequential and the fast implementation is parallel. 


## Building and running

Built for a multicore linux system that supports AVX operations. Use `make -j` to build binaries and `make clean` to clean up the directory. When built there are three command available: `./run <dim> <iter>`, `./test <ny> <nm> <nx>` and `./benchmark <dim> <iter>`.

- `./run <dim> <iter>` default: dim=1000, iter=3 <br/>
Running both implementations on the same matrices for `iter` times and outputing the average running times, the speedup and the error term.

- `./test <ny> <nm> <nx>` default: ny,nm,nx in {5,1500} <br/>


- `./benchmark <dim> <iter>` default: dim=3000, iter=10 <br/>
Running the fast implementation on different matrices of dimension `dim` for `iter` times and outputing the running times and the total average. 

### Error term
The error term is defined to be the sum of the element wise absolute difference of the two matrices.


## Parallel implementation
???


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
sequential:    2679.8
parallel:      6.8740	ratio: 389.8  

n = 8000
sequential:    5037.0
parallel:      12.383 	ratio: 406.8     
```


## Author

[Rustam Latypov](mailto:rustam.latypov@aalto.fi)
