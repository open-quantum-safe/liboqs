/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 205 Stateless Hash-Based Digital Signature Standard. */
/* HashSLH functions */

#ifndef _SLH_PREHASH_H_
#define _SLH_PREHASH_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "slh_dsa.h"

  /* === Generates a pre-hash SLH-DSA signature. */
  /* Algorithm 23: Algorithm 23 hash_slh_sign(M, ctx, PH, SK) */

  size_t hash_slh_sign(uint8_t *sig, const uint8_t *m, size_t m_sz,
                       const uint8_t *ctx, size_t ctx_sz, const char *ph,
                       const uint8_t *sk, const uint8_t *addrnd,
                       const slh_param_t *prm);

  /* === Verifies a pre-hash SLH-DSA signature. */
  /* Algorithm 25: hash_slh_verify(M, SIG, ctx, PH, PK) */

  int hash_slh_verify(const uint8_t *m, size_t m_sz, const uint8_t *sig,
                      size_t sig_sz, const uint8_t *ctx, size_t ctx_sz,
                      const char *ph, const uint8_t *pk,
                      const slh_param_t *prm);

#ifdef __cplusplus
}
#endif

/* _SLH_PREHASH_H_ */
#endif
