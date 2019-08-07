#include <vector>
#include <iostream>
#include "is.cc"
#include "random.h"
#include "debug.cpp"
using namespace std;

int main(void) {
    ppc::random rng(24, 132);
    rng();

    //test_gradient(rng, 10, 10);


    std::vector<float> data =
            {1,1,1,
             2,2,2,
             3,3,3,
             4,4,4,
             5,5,5,
             6,6,6,
             7,7,7,
             8,8,8,
             9,9,9};

    segment(3, 3, data.data());

}

