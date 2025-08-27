/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 205 Stateless Hash-Based Digital Signature Standard. */
/* API fo internal and "pure" variants. */

#ifndef _SLH_DSA_H_
#define _SLH_DSA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <oqs/oqs.h>

#ifndef _SLH_PARAM_H_
  typedef struct slh_param_s slh_param_t;
#endif

  /* === SLH-DSA parameter sets */
  extern const slh_param_t slh_dsa_sha2_128s;
  extern const slh_param_t slh_dsa_shake_128s;
  extern const slh_param_t slh_dsa_sha2_128f;
  extern const slh_param_t slh_dsa_shake_128f;
  extern const slh_param_t slh_dsa_sha2_192s;
  extern const slh_param_t slh_dsa_shake_192s;
  extern const slh_param_t slh_dsa_sha2_192f;
  extern const slh_param_t slh_dsa_shake_192f;
  extern const slh_param_t slh_dsa_sha2_256s;
  extern const slh_param_t slh_dsa_shake_256s;
  extern const slh_param_t slh_dsa_sha2_256f;
  extern const slh_param_t slh_dsa_shake_256f;

  /* === SLH_DSA API (_internal and _pure functions.) */
  /* see slh_prehash.h for HashSLH functions */

  /* Return standard identifier string for parameter set *prm, or NULL. */
  const char *slh_alg_id(const slh_param_t *prm);

  /* Return public (verification) key size in bytes for parameter set *prm. */
  size_t slh_pk_sz(const slh_param_t *prm);

  /* Return private (signing) key size in bytes for parameter set *prm. */
  size_t slh_sk_sz(const slh_param_t *prm);

  /* Return signature size in bytes for parameter set *prm. */
  size_t slh_sig_sz(const slh_param_t *prm);

  /* Generate an SLH-DSA key pair; needs an RBG (a "randombytes()" function) */
  int slh_keygen(uint8_t *sk, uint8_t *pk, int (*rbg)(uint8_t *x, size_t xlen),
                 const slh_param_t *prm);

  int slh_keygen_internal(uint8_t *sk, uint8_t *pk, const uint8_t *sk_seed,
                          const uint8_t *sk_prf, const uint8_t *pk_seed,
                          const slh_param_t *prm);

  /* Generate an SLH-DSA signature. */
  OQS_API size_t slh_sign_internal(uint8_t *sig, const uint8_t *m, size_t m_sz,
                           const uint8_t *sk, const uint8_t *addrnd,
                           const slh_param_t *prm);

  size_t slh_sign(uint8_t *sig, const uint8_t *m, size_t m_sz,
                  const uint8_t *ctx, size_t ctx_sz, const uint8_t *sk,
                  const uint8_t *addrnd, const slh_param_t *prm);

  /* Verify an SLH-DSA signature. */
  /* return 0 on verification failure, 1 on success */
  OQS_API int slh_verify_internal(const uint8_t *m, size_t m_sz, const uint8_t *sig,
                          size_t sig_sz, const uint8_t *pk,
                          const slh_param_t *prm);

  int slh_verify(const uint8_t *m, size_t m_sz, const uint8_t *sig,
                 size_t sig_sz, const uint8_t *ctx, size_t ctx_sz,
                 const uint8_t *pk, const slh_param_t *prm);

#ifdef __cplusplus
}
#endif

/* _SLH_DSA_H_ */
#endif
