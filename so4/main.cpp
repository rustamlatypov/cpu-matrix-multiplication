#include "so-gen.h"
#include "so.cc"
#include "random.h"
#include <vector>
#include <iostream>
#include <array>
#include <cassert>

int main(void)
{
    static constexpr data_t MAGIC = 12345;
    static constexpr uint64_t seed1 = 230;
    static constexpr uint64_t seed2 = 964;
    ppc::random rng(seed1, seed2);

    int testsize = 1000;
    const auto mode = gen_types[0];

    std::vector<data_t> input(testsize);
    std::vector<data_t> reference_input(input.size());

    generate(input.begin(), input.begin()+testsize, rng, mode);
    std::copy(input.begin(), input.begin()+testsize, reference_input.begin());

    //print(input);

    psort(testsize, input.data());
    std::sort(reference_input.begin(), reference_input.begin()+testsize);

    print(input);

    print(reference_input);

    const bool ok = std::equal(input.begin(), input.end(), reference_input.begin());

    if (ok) {
        std::cout << std::endl << "Correct" << std::endl;
    } else {
        std::cout << std::endl << "Wrong" << std::endl;
    }

    return 1;
}
