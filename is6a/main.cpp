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
            {0, 0, 0,
             1, 1, 1,
             0, 0, 0, };

    segment(3, 1, data.data());

}

