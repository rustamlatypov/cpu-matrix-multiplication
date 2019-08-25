# Parallel CPU matrix multiplication 

Developed during June-August, 2019.


## Description

A console interface for testing and benchmarking two matrix multiplication implementations. Both use the same basic algorithm running O(n^3) for matrices of dimension n. The base implementation is sequential and the fast implementation is parallel. 


## Building and running

Built for a multicore linux system that supports AVX operations. Use `make -j` to build binaries and `make clean` to clean up the directory. When built there are three command available: `./run <dim> <iter>`, `./test <ny> <nm> <nx>` and `./benchmark <dim> <iter>`.

- `./run <dim> <iter>` default: dim=1000, iter=3 <br/>
Runs both implementations on the same matrices for `iter` times and outputs the average running times, the speedup and the error term.

- `./test <ny> <nm> <nx>` default: ny,nm,nx in {5,1500} <br/>
Runs extensive tests with different matrices and different combinations of ny, nm and nx. Dimension wise the multiplication is (ny x nm)(nm x nx). Each combination is run 20 times and the error term is accumulated and outputed. The tests pass if the error is below 1e-3. 

- `./benchmark <dim> <iter>` default: dim=3000, iter=10 <br/>
Runs the fast implementation on different matrices of dimension `dim` for `iter` times and outputs the running times and the total average. 

### Error term
The error term is defined to be the sum of the element wise absolute difference of the two matrices.


## Parallel implementation
???


## Remarks



## Results
The results are captured using `./run n 2`, with n = 4000, 5000, 6000, 7000, 8000. 

Linux g++-8, -march=native -O2 -fopenmp <br/>
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
sequential:    2948.5
parallel:      6.38565	ratio: 461.743

n = 8000
sequential:    5037.0
parallel:      12.383 	ratio: 406.8     
```


## Author

[Rustam Latypov](mailto:rustam.latypov@aalto.fi)
