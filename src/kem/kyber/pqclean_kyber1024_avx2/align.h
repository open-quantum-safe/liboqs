#ifndef PQCLEAN_KYBER1024_AVX2_ALIGN_H
#define PQCLEAN_KYBER1024_AVX2_ALIGN_H

#include <immintrin.h>
#include <stdint.h>

#define ALIGNED_UINT8(N)        \
    union {                     \
        uint8_t coeffs[(N)];      \
        __m256i vec[((N)+31)/32]; \
    }

#define ALIGNED_INT16(N)        \
    union {                     \
        int16_t coeffs[(N)];      \
        __m256i vec[((N)+15)/16]; \
    }

#endif
