/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLK_NATIVE_X86_64_SRC_ARITH_NATIVE_X86_64_H
#define MLK_NATIVE_X86_64_SRC_ARITH_NATIVE_X86_64_H

#include "../../../common.h"

#include <immintrin.h>
#include <stdint.h>
#include "consts.h"

#define MLK_AVX2_REJ_UNIFORM_BUFLEN \
  (3 * 168) /* REJ_UNIFORM_NBLOCKS * SHAKE128_RATE */

#define mlk_rej_uniform_avx2 MLK_NAMESPACE(rej_uniform_avx2)
unsigned mlk_rej_uniform_avx2(int16_t *r, const uint8_t *buf);

#define mlk_rej_uniform_table MLK_NAMESPACE(rej_uniform_table)
extern const uint8_t mlk_rej_uniform_table[256][8];

#define mlk_ntt_avx2 MLK_NAMESPACE(ntt_avx2)
void mlk_ntt_avx2(__m256i *r, const __m256i *mlk_qdata);

#define mlk_invntt_avx2 MLK_NAMESPACE(invntt_avx2)
void mlk_invntt_avx2(__m256i *r, const __m256i *mlk_qdata);

#define mlk_nttunpack_avx2 MLK_NAMESPACE(nttunpack_avx2)
void mlk_nttunpack_avx2(__m256i *r);

#define mlk_reduce_avx2 MLK_NAMESPACE(reduce_avx2)
void mlk_reduce_avx2(__m256i *r, const __m256i *mlk_qdata);

#define mlk_basemul_avx2 MLK_NAMESPACE(basemul_avx2)
void mlk_basemul_avx2(__m256i *r, const __m256i *a, const __m256i *b,
                      const __m256i *b_cache, const __m256i *mlk_qdata);

#define mlk_poly_mulcache_compute_avx2 MLK_NAMESPACE(poly_mulcache_compute_avx2)
void mlk_poly_mulcache_compute_avx2(__m256i *out, const __m256i *in,
                                    const __m256i *mlk_qdata);

#define mlk_polyvec_basemul_acc_montgomery_cached_avx2 \
  MLK_NAMESPACE(polyvec_basemul_acc_montgomery_cached_avx2)
void mlk_polyvec_basemul_acc_montgomery_cached_avx2(unsigned k,
                                                    int16_t r[MLKEM_N],
                                                    const int16_t *a,
                                                    const int16_t *b,
                                                    const int16_t *kb_cache);

#define mlk_ntttobytes_avx2 MLK_NAMESPACE(ntttobytes_avx2)
void mlk_ntttobytes_avx2(uint8_t *r, const __m256i *a,
                         const __m256i *mlk_qdata);

#define mlk_nttfrombytes_avx2 MLK_NAMESPACE(nttfrombytes_avx2)
void mlk_nttfrombytes_avx2(__m256i *r, const uint8_t *a,
                           const __m256i *mlk_qdata);

#define mlk_tomont_avx2 MLK_NAMESPACE(tomont_avx2)
void mlk_tomont_avx2(__m256i *r, const __m256i *mlk_qdata);

#define mlk_poly_compress_d4_avx2 MLK_NAMESPACE(poly_compress_d4_avx2)
void mlk_poly_compress_d4_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D4],
                               const __m256i *MLK_RESTRICT a);
#define mlk_poly_decompress_d4_avx2 MLK_NAMESPACE(poly_decompress_d4_avx2)
void mlk_poly_decompress_d4_avx2(__m256i *MLK_RESTRICT r,
                                 const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D4]);
#define mlk_poly_compress_d10_avx2 MLK_NAMESPACE(poly_compress10_avx2)
void mlk_poly_compress_d10_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D10],
                                const __m256i *MLK_RESTRICT a);
#define mlk_poly_decompress_d10_avx2 MLK_NAMESPACE(poly_decompress10_avx2)
void mlk_poly_decompress_d10_avx2(
    __m256i *MLK_RESTRICT r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D10]);
#define mlk_poly_compress_d5_avx2 MLK_NAMESPACE(poly_compress_d5_avx2)
void mlk_poly_compress_d5_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D5],
                               const __m256i *MLK_RESTRICT a);
#define mlk_poly_decompress_d5_avx2 MLK_NAMESPACE(poly_decompress_d5_avx2)
void mlk_poly_decompress_d5_avx2(__m256i *MLK_RESTRICT r,
                                 const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D5]);
#define mlk_poly_compress_d11_avx2 MLK_NAMESPACE(poly_compress11_avx2)
void mlk_poly_compress_d11_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D11],
                                const __m256i *MLK_RESTRICT a);
#define mlk_poly_decompress_d11_avx2 MLK_NAMESPACE(poly_decompress11_avx2)
void mlk_poly_decompress_d11_avx2(
    __m256i *MLK_RESTRICT r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D11]);

#endif /* !MLK_NATIVE_X86_64_SRC_ARITH_NATIVE_X86_64_H */
