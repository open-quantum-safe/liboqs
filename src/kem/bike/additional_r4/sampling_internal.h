/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker, Shay Gueron and Dusan Kostic,
 * AWS Cryptographic Algorithms Group.
 */

#pragma once

#include "cpu_features.h"
#include "defs.h"
#include "types.h"

void secure_set_bits_port(OUT pad_r_t *r,
                          IN size_t    first_pos,
                          IN const idx_t *wlist,
                          IN size_t       w_size);

#if defined(X86_64)
void secure_set_bits_avx2(OUT pad_r_t *r,
                          IN size_t    first_pos,
                          IN const idx_t *wlist,
                          IN size_t       w_size);

void secure_set_bits_avx512(OUT pad_r_t *r,
                            IN size_t    first_pos,
                            IN const idx_t *wlist,
                            IN size_t       w_size);
#endif

typedef struct sampling_ctx_st {
  void (*secure_set_bits)(OUT pad_r_t *r,
                          IN size_t    first_pos,
                          IN const idx_t *wlist,
                          IN size_t       w_size);
} sampling_ctx;

_INLINE_ void sampling_ctx_init(sampling_ctx *ctx)
{
#if defined(X86_64)
#if defined(OQS_DIST_X86_64_BUILD) || defined(OQS_USE_AVX512_INSTRUCTIONS)
  if(is_avx512_enabled()) {
    ctx->secure_set_bits = secure_set_bits_avx512;
  } else
#endif
#if defined(OQS_DIST_X86_64_BUILD) || defined(OQS_USE_AVX2_INSTRUCTIONS)
  if(is_avx2_enabled()) {
    ctx->secure_set_bits = secure_set_bits_avx2;
  } else
#endif
#endif
  {
    ctx->secure_set_bits = secure_set_bits_port;
  }
}
