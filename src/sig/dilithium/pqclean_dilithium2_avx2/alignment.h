#ifndef PQCLEAN_DILITHIUM2_AVX2_ALIGNMENT_H
#define PQCLEAN_DILITHIUM2_AVX2_ALIGNMENT_H

#define ALIGNED_UINT8(N)        \
    union {                     \
        uint32_t as_arr[N];     \
        __m256i as_vec[(N)/32]; \
    }

#define ALIGNED_UINT32(N)       \
    union {                     \
        uint32_t as_arr[N];     \
        __m256i as_vec[(N)/8];  \
    }

#define ALIGNED_UINT64(N)       \
    union {                     \
        uint64_t as_arr[N];     \
        __m256i as_vec[(N)/8];  \
    }

#endif //PQCLEAN_DILITHIUM2_AVX2_ALIGNMENT_H
