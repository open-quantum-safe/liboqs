#ifndef PQCLEAN_DILITHIUM2_AVX2_ALIGN_H
#define PQCLEAN_DILITHIUM2_AVX2_ALIGN_H

#include <immintrin.h>
#include <stdint.h>

#define ALIGNED_UINT8(N)        \
    union {                     \
        uint8_t coeffs[N];      \
        __m256i vec[((N)+31)/32]; \
    }

#define ALIGNED_INT32(N)        \
    union {                     \
        int32_t coeffs[N];      \
        __m256i vec[((N)+7)/8];   \
    }

#endif
