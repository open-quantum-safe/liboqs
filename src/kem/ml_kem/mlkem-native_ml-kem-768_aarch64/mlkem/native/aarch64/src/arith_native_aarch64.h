/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLK_NATIVE_AARCH64_SRC_ARITH_NATIVE_AARCH64_H
#define MLK_NATIVE_AARCH64_SRC_ARITH_NATIVE_AARCH64_H

#include <stdint.h>
#include "../../../common.h"

#define mlk_aarch64_ntt_zetas_layer12345 \
  MLK_NAMESPACE(aarch64_ntt_zetas_layer12345)
#define mlk_aarch64_ntt_zetas_layer67 MLK_NAMESPACE(aarch64_ntt_zetas_layer67)
#define mlk_aarch64_invntt_zetas_layer12345 \
  MLK_NAMESPACE(aarch64_invntt_zetas_layer12345)
#define mlk_aarch64_invntt_zetas_layer67 \
  MLK_NAMESPACE(aarch64_invntt_zetas_layer67)
#define mlk_aarch64_zetas_mulcache_native \
  MLK_NAMESPACE(aarch64_zetas_mulcache_native)
#define mlk_aarch64_zetas_mulcache_twisted_native \
  MLK_NAMESPACE(aarch64_zetas_mulcache_twisted_native)
#define mlk_rej_uniform_table MLK_NAMESPACE(rej_uniform_table)

extern const int16_t mlk_aarch64_ntt_zetas_layer12345[];
extern const int16_t mlk_aarch64_ntt_zetas_layer67[];
extern const int16_t mlk_aarch64_invntt_zetas_layer12345[];
extern const int16_t mlk_aarch64_invntt_zetas_layer67[];
extern const int16_t mlk_aarch64_zetas_mulcache_native[];
extern const int16_t mlk_aarch64_zetas_mulcache_twisted_native[];
extern const uint8_t mlk_rej_uniform_table[];

#define mlk_ntt_asm MLK_NAMESPACE(ntt_asm)
void mlk_ntt_asm(int16_t *, const int16_t *, const int16_t *);

#define mlk_intt_asm MLK_NAMESPACE(intt_asm)
void mlk_intt_asm(int16_t *, const int16_t *, const int16_t *);

#define mlk_poly_reduce_asm MLK_NAMESPACE(poly_reduce_asm)
void mlk_poly_reduce_asm(int16_t *);

#define mlk_poly_tomont_asm MLK_NAMESPACE(poly_tomont_asm)
void mlk_poly_tomont_asm(int16_t *);

#define mlk_poly_mulcache_compute_asm MLK_NAMESPACE(poly_mulcache_compute_asm)
void mlk_poly_mulcache_compute_asm(int16_t *, const int16_t *, const int16_t *,
                                   const int16_t *);

#define mlk_poly_tobytes_asm MLK_NAMESPACE(poly_tobytes_asm)
void mlk_poly_tobytes_asm(uint8_t *r, const int16_t *a);

#define mlk_polyvec_basemul_acc_montgomery_cached_asm_k2 \
  MLK_NAMESPACE(polyvec_basemul_acc_montgomery_cached_asm_k2)
void mlk_polyvec_basemul_acc_montgomery_cached_asm_k2(int16_t *r,
                                                      const int16_t *a,
                                                      const int16_t *b,
                                                      const int16_t *b_cache);

#define mlk_polyvec_basemul_acc_montgomery_cached_asm_k3 \
  MLK_NAMESPACE(polyvec_basemul_acc_montgomery_cached_asm_k3)
void mlk_polyvec_basemul_acc_montgomery_cached_asm_k3(int16_t *r,
                                                      const int16_t *a,
                                                      const int16_t *b,
                                                      const int16_t *b_cache);

#define mlk_polyvec_basemul_acc_montgomery_cached_asm_k4 \
  MLK_NAMESPACE(polyvec_basemul_acc_montgomery_cached_asm_k4)
void mlk_polyvec_basemul_acc_montgomery_cached_asm_k4(int16_t *r,
                                                      const int16_t *a,
                                                      const int16_t *b,
                                                      const int16_t *b_cache);

#define mlk_rej_uniform_asm MLK_NAMESPACE(rej_uniform_asm)
unsigned mlk_rej_uniform_asm(int16_t *r, const uint8_t *buf, unsigned buflen,
                             const uint8_t *table);

#endif /* MLK_NATIVE_AARCH64_SRC_ARITH_NATIVE_AARCH64_H */
