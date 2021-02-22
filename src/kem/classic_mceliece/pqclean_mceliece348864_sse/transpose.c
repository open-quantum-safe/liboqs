#include "transpose.h"

extern void PQCLEAN_MCELIECE348864_SSE_transpose_64x64_asm(uint64_t *);
void PQCLEAN_MCELIECE348864_SSE_transpose_64x64(uint64_t *in) {
    PQCLEAN_MCELIECE348864_SSE_transpose_64x64_asm(in);
}

extern void PQCLEAN_MCELIECE348864_SSE_transpose_64x128_sp_asm(vec128 *);

void PQCLEAN_MCELIECE348864_SSE_transpose_64x128_sp(vec128 *in) {
    PQCLEAN_MCELIECE348864_SSE_transpose_64x128_sp_asm(in);
}
