/*
  This file is for matrix transposition
*/

#include "transpose.h"

extern void PQCLEAN_MCELIECE8192128_AVX_transpose_64x128_sp_asm(vec128 *);

void PQCLEAN_MCELIECE8192128_AVX_transpose_64x128_sp(vec128 *in) {
    PQCLEAN_MCELIECE8192128_AVX_transpose_64x128_sp_asm(in);
}

extern void PQCLEAN_MCELIECE8192128_AVX_transpose_64x256_sp_asm(vec256 *);

void PQCLEAN_MCELIECE8192128_AVX_transpose_64x256_sp(vec256 *in) {
    PQCLEAN_MCELIECE8192128_AVX_transpose_64x256_sp_asm(in);
}

