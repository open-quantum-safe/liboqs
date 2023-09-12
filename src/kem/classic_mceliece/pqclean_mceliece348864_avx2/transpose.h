#ifndef TRANSPOSE_H
#define TRANSPOSE_H
/*
  This file is for matrix transposition
*/

#include "namespace.h"

#define transpose_64x128_sp_asm CRYPTO_NAMESPACE(transpose_64x128_sp_asm)
#define transpose_64x256_sp_asm CRYPTO_NAMESPACE(transpose_64x256_sp_asm)
#define transpose_64x64_asm CRYPTO_NAMESPACE(transpose_64x64_asm)

#include "vec256.h"

#include <stdint.h>

extern void transpose_64x64_asm(uint64_t *);

static inline void transpose_64x64(uint64_t *in) {
    transpose_64x64_asm(in);
}

extern void transpose_64x256_sp_asm(vec256 *);

static inline void transpose_64x256_sp(vec256 *in) {
    transpose_64x256_sp_asm(in);
}

#endif
