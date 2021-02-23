#include "vec.h"

#include "params.h"

extern void PQCLEAN_MCELIECE348864F_AVX_vec_mul_asm(uint64_t *, const uint64_t *, const uint64_t *);
extern void PQCLEAN_MCELIECE348864F_AVX_vec_mul_sp_asm(uint64_t *, const uint64_t *, const uint64_t *);


void PQCLEAN_MCELIECE348864F_AVX_vec_mul(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    PQCLEAN_MCELIECE348864F_AVX_vec_mul_asm(h, f, g);
}


void PQCLEAN_MCELIECE348864F_AVX_vec_mul_sp(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    PQCLEAN_MCELIECE348864F_AVX_vec_mul_sp_asm(h, f, g);
}

void PQCLEAN_MCELIECE348864F_AVX_vec_add(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    int b;

    for (b = 0; b < GFBITS; b++) {
        h[b] = f[b] ^ g[b];
    }
}

