/**
 * @file pack.c
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief Parameter specific pack/unpack functions
 */

#include "type.h"
#include "param.h"

#define MSB    0x8000
#define MS3B   0xe000
#define LOWER8 0x00ff

/**
 * @brief pack 64 elems to 104 bytes, SIMD friendly implementation.
 */
void pack_64elems(U8 *rop, const U16 *op){
  const U32 n = 8;
  U16 *ptr = (U16*)rop;
  U32 i;
  for(i = 0; i < n; ++i) ptr[0 * n + i] = op[0 * n + i] | ((op[6 * n + i] << 3) & MS3B);
  for(i = 0; i < n; ++i) ptr[1 * n + i] = op[1 * n + i] | ((op[6 * n + i] << 6) & MS3B);
  for(i = 0; i < n; ++i) ptr[2 * n + i] = op[2 * n + i] | ((op[6 * n + i] << 9) & MS3B);
  for(i = 0; i < n; ++i) ptr[3 * n + i] = op[3 * n + i] | ((op[6 * n + i] << 12) & MS3B);
  for(i = 0; i < n; ++i) ptr[4 * n + i] = op[4 * n + i] | (((op[6 * n + i] << 15)
                                                            | ((op[7 * n + i] << 2))) & MS3B);
  for(i = 0; i < n; ++i) ptr[5 * n + i] = op[5 * n + i] | ((op[7 * n + i] << 5) & MS3B);
  for(i = 0; i < n / 2; ++i) ptr[6 * n + i] = (op[7 * n + i] & LOWER8)
                               | ((op[7 * n + n / 2 + i] & LOWER8) << 8);
}

/**
 * @brief unpack 64 elems from 104 bytes, SIMD friendly implementation.
 */
void unpack_64elems(U16 *rop, const U8 *op){
  const U32 n = 8;
  const U16 *ptr = (const U16*)op;
  U32 i;
  for(i = 0; i < n; ++i) rop[0 * n + i] = ptr[0 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[1 * n + i] = ptr[1 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[2 * n + i] = ptr[2 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[3 * n + i] = ptr[3 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[4 * n + i] = ptr[4 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[5 * n + i] = ptr[5 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[6 * n + i] = ((ptr[0 * n + i] & MS3B) >> 3)
                           | ((ptr[1 * n + i] & MS3B) >> 6)
                           | ((ptr[2 * n + i] & MS3B) >> 9)
                           | ((ptr[3 * n + i] & MS3B) >> 12)
                           | ((ptr[4 * n + i] & MS3B) >> 15);
  for(i = 0; i < n / 2; ++i) rop[7 * n + i] = ((ptr[4 * n + i] & (MSB ^ MS3B)) >> 2)
                               | ((ptr[5 * n + i] & MS3B) >> 5)
                               | (ptr[6 * n + i] & LOWER8);
  for(i = 0; i < n / 2; ++i) rop[7 * n + n / 2 + i] = ((ptr[4 * n + n / 2 + i]
                                                        & (MSB ^ MS3B)) >> 2)
                               | ((ptr[5 * n + n / 2 + i] & MS3B) >> 5)
                               | ((ptr[6 * n + i] >> 8) & LOWER8);
}

/**
 * @brief pack 128 elems to 208 bytes, SIMD friendly implementation.
 */
void pack_128elems(U8 *rop, const U16 *op){
  const U32 n = 16;
  U16 *ptr = (U16*)rop;
  U32 i;
  for(i = 0; i < n; ++i) ptr[0 * n + i] = op[0 * n + i] | ((op[6 * n + i] << 3) & MS3B);
  for(i = 0; i < n; ++i) ptr[1 * n + i] = op[1 * n + i] | ((op[6 * n + i] << 6) & MS3B);
  for(i = 0; i < n; ++i) ptr[2 * n + i] = op[2 * n + i] | ((op[6 * n + i] << 9) & MS3B);
  for(i = 0; i < n; ++i) ptr[3 * n + i] = op[3 * n + i] | ((op[6 * n + i] << 12) & MS3B);
  for(i = 0; i < n; ++i) ptr[4 * n + i] = op[4 * n + i] | (((op[6 * n + i] << 15)
                                                            | ((op[7 * n + i] << 2))) & MS3B);
  for(i = 0; i < n; ++i) ptr[5 * n + i] = op[5 * n + i] | ((op[7 * n + i] << 5) & MS3B);
  for(i = 0; i < n / 2; ++i) ptr[6 * n + i] = (op[7 * n + i] & LOWER8)
                               | ((op[7 * n + n / 2 + i] & LOWER8) << 8);
}

/**
 * @brief unpack 128 elems from 208 bytes, SIMD friendly implementation.
 */
void unpack_128elems(U16 *rop, const U8 *op){
  const U32 n = 16;
  const U16 *ptr = (const U16*)op;
  U32 i;
  for(i = 0; i < n; ++i) rop[0 * n + i] = ptr[0 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[1 * n + i] = ptr[1 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[2 * n + i] = ptr[2 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[3 * n + i] = ptr[3 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[4 * n + i] = ptr[4 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[5 * n + i] = ptr[5 * n + i] & (~MS3B);
  for(i = 0; i < n; ++i) rop[6 * n + i] = ((ptr[0 * n + i] & MS3B) >> 3)
                           | ((ptr[1 * n + i] & MS3B) >> 6)
                           | ((ptr[2 * n + i] & MS3B) >> 9)
                           | ((ptr[3 * n + i] & MS3B) >> 12)
                           | ((ptr[4 * n + i] & MS3B) >> 15);
  for(i = 0; i < n / 2; ++i) rop[7 * n + i] = ((ptr[4 * n + i] & (MSB ^ MS3B)) >> 2)
                               | ((ptr[5 * n + i] & MS3B) >> 5)
                               | (ptr[6 * n + i] & LOWER8);
  for(i = 0; i < n / 2; ++i) rop[7 * n + n / 2 + i] = ((ptr[4 * n + n / 2 + i]
                                                        & (MSB ^ MS3B)) >> 2)
                               | ((ptr[5 * n + n / 2 + i] & MS3B) >> 5)
                               | ((ptr[6 * n + i] >> 8) & LOWER8);
}

/**
 * @brief Pack 128 discrete Gaussian samples to 96 bytes, SIMD friendly.
 */
void pack_128dg(U8 *rop, const U16 *op){
  const U32 n = 16;
  U16 *ptr = (U16*)rop;
  U32 i;
  for(i = 0; i < n; ++i) ptr[0 * n + i] = (op[0 * n + i] & 0x3f)
                           | ((op[1 * n + i] & 0x3f) << 6)
                           | ((op[2 * n + i] & 0x3f) << 12);
  for(i = 0; i < n; ++i) ptr[1 * n + i] = ((op[2 * n + i] & 0x3f) >> 4)
                           | ((op[3 * n + i] & 0x3f) << 2) | ((op[4 * n + i] & 0x3f) << 8)
                           | ((op[5 * n + i] & 0x3f) << 14);
  for(i = 0; i < n; ++i) ptr[2 * n + i] = ((op[5 * n + i] & 0x3f) >> 2)
                           | ((op[6 * n + i] & 0x3f) << 4)
                           | ((op[7 * n + i] & 0x3f) << 10);
}

/**
 * @brief Unpack 128 discrete Gaussian samples from 96 bytes, SIMD friendly.
 */
void unpack_128dg(U16 *rop, const U8 *op){
  const U32 n = 16;
  const U16 *ptr = (const U16*)op;
  U32 i;
  for(i = 0; i < n; ++i) rop[0 * n + i] = ptr[0 * n + i] & 0x3f;
  for(i = 0; i < n; ++i) rop[0 * n + i] = (((0 - (rop[0 * n + i] >> 5)) << 6)
                                           | rop[0 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[1 * n + i] = (ptr[0 * n + i] >> 6) & 0x3f;
  for(i = 0; i < n; ++i) rop[1 * n + i] = (((0 - (rop[1 * n + i] >> 5)) << 6)
                                           | rop[1 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[2 * n + i] = ((ptr[0 * n + i] >> 12)
                                           | (ptr[1 * n + i] << 4)) & 0x3f;
  for(i = 0; i < n; ++i) rop[2 * n + i] = (((0 - (rop[2 * n + i] >> 5)) << 6)
                                           | rop[2 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[3 * n + i] = (ptr[1 * n + i] >> 2) & 0x3f;
  for(i = 0; i < n; ++i) rop[3 * n + i] = (((0 - (rop[3 * n + i] >> 5)) << 6)
                                           | rop[3 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[4 * n + i] = (ptr[1 * n + i] >> 8) & 0x3f;
  for(i = 0; i < n; ++i) rop[4 * n + i] = (((0 - (rop[4 * n + i] >> 5)) << 6)
                                           | rop[4 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[5 * n + i] = ((ptr[1 * n + i] >> 14)
                                           | (ptr[2 * n + i] << 2)) & 0x3f;
  for(i = 0; i < n; ++i) rop[5 * n + i] = (((0 - (rop[5 * n + i] >> 5)) << 6)
                                           | rop[5 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[6 * n + i] = (ptr[2 * n + i] >> 4) & 0x3f;
  for(i = 0; i < n; ++i) rop[6 * n + i] = (((0 - (rop[6 * n + i] >> 5)) << 6)
                                           | rop[6 * n + i]) & 0x1fff;
  for(i = 0; i < n; ++i) rop[7 * n + i] = ptr[2 * n + i] >> 10;
  for(i = 0; i < n; ++i) rop[7 * n + i] = (((0 - (rop[7 * n + i] >> 5)) << 6)
                                           | rop[7 * n + i]) & 0x1fff;
}


/**
 * @brief Pack public key
 */
void pack_pk(U8 *bin, const U16 *pk){
  const U32 len = (_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT) / 128;
  U32 i;
  for(i = 0; i < len; ++i){
    pack_128elems(bin, pk);
    bin += 208, pk += 128;
  }
  /* len % 128 = 0 */
}

/**
 * @brief Unpack public key
 */
void unpack_pk(U16 *pk, const U8 *bin){
  const U32 len = (_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT) / 128;
  U32 i;
  for(i = 0; i < len; ++i){
    unpack_128elems(pk, bin);
    pk += 128, bin += 208;
  }
  /* len % 128 = 0 */
}

/**
 * @brief Pack secret key
 */
void pack_sk(U8 *bin, const U16 *sk){
  const U32 len = (_LOTUS_LWE_DIM * _LOTUS_LWE_PT) / 128;
  U32 i;
  for(i = 0; i < len; ++i){
    pack_128dg(bin, sk);
    bin += 96, sk += 128;
  }
}

/**
 * @brief Unpack secret key
 */
void unpack_sk(U16 *sk, const U8 *bin){
  const U32 len = (_LOTUS_LWE_DIM * _LOTUS_LWE_PT) / 128;
  U32 i;
  for(i = 0; i < len; ++i){
    unpack_128dg(sk, bin);
    sk += 128, bin += 96;
  }
}

/**
 * @brief Pack ciphertext
 */
void pack_ct(U8 *bin, const U16 *ct){
  const U32 len = (_LOTUS_LWE_DIM + _LOTUS_LWE_PT) / 128;
  U32 i;
  for(i = 0; i < len; ++i){
    pack_128elems(bin, ct);
    bin += 208, ct += 128;
  }
  pack_64elems(bin, ct); /* len % 128 = 64 */
}

/**
 * @brief Unpack ciphertext
 */
void unpack_ct(U16 *ct, const U8 *bin){
  const U32 len = (_LOTUS_LWE_DIM + _LOTUS_LWE_PT) / 128;
  U32 i;
  for(i = 0; i < len; ++i){
    unpack_128elems(ct, bin);
    ct += 128, bin += 208;
  }
  unpack_64elems(ct, bin); /* len % 128 = 64 */
}
