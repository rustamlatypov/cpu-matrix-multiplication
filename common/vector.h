#ifndef VECTOR
#define VECTOR

#include <cstdlib>

typedef double double4_t __attribute__ ((vector_size (32)));
constexpr double4_t double4_0 = {0,0,0,0};

// Malloc replacement, retuns memory aligned to 32 bytes, free with free()
inline void* aligned_malloc(std::size_t bytes) {
    void* ret = nullptr;
    if (posix_memalign(&ret, 32, bytes)) {
        return nullptr;
    }
    return ret;
}

inline double4_t* double4_alloc(std::size_t n) {
    return static_cast<double4_t*>(aligned_malloc(sizeof(double4_t) * n));
}

#endif
