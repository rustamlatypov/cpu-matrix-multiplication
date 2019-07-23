#include <iostream>

void print(std::vector<data_t> input)
{
    std::cout << std::endl;
    for (data_t e : input) std::cout << e << " ";
    std::cout << std::endl;
}

void printv(int n, data_t* input)
{
    std::cout << std::endl;
    for (int i = 0; i < n; i++) std::cout << input[i] << " ";
    std::cout << std::endl;
}