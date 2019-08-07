#include <iostream>
#include <vector>

void print(std::vector<data_t> input)
{
    std::cout << std::endl;
    for (data_t e : input) std::cout << e << " ";
    std::cout << std::endl;
}

void printv(data_t* input, int start, int end)
{
    std::cout << std::endl;
    for (int i = 0; i < end; i++) {
        if (i < start) {
            std::cout << "-" << " ";
        } else {
            std::cout << input[i] << " ";
        }
    }
    std::cout << std::endl;
}

void printti(data_t* start, data_t* end)
{
    std::cout << std::endl;
    for (auto i = start; i < end; i++) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}