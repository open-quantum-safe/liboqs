
#include "vec.h"

#include "params.h"

void PQCLEAN_MCELIECE348864_SSE_vec_mul(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    PQCLEAN_MCELIECE348864_SSE_vec_mul_asm(h, f, g, 8);
}

void PQCLEAN_MCELIECE348864_SSE_vec_add(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    int b;

    for (b = 0; b < GFBITS; b++) {
        h[b] = f[b] ^ g[b];
    }
}

