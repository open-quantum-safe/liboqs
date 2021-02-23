#include "transpose.h"

/*
  This file is for matrix transposition
*/

extern void PQCLEAN_MCELIECE348864_AVX_transpose_64x64_asm(uint64_t *);
extern void PQCLEAN_MCELIECE348864_AVX_transpose_64x256_sp_asm(vec256 *);


void PQCLEAN_MCELIECE348864_AVX_transpose_64x64(uint64_t *in) {
    PQCLEAN_MCELIECE348864_AVX_transpose_64x64_asm(in);
}

void PQCLEAN_MCELIECE348864_AVX_transpose_64x256_sp(vec256 *in) {
    PQCLEAN_MCELIECE348864_AVX_transpose_64x256_sp_asm(in);
}
