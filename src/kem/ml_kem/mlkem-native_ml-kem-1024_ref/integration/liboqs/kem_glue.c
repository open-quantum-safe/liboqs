/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*
 * liboqs glue for mlkem-native.
 *
 * liboqs's OQS_STATUS is a closed enum of OQS_SUCCESS / OQS_ERROR, and the
 * generated ML-KEM wrapper casts the result of the linked implementation
 * straight through. mlkem-native returns a wider error set, so these thin
 * shims collapse any nonzero result to OQS_ERROR. They are referenced from
 * the per-level META.yml files.
 *
 * The shims are compiled once per build (parameter set and backend), so the
 * namespaced symbol names match the names liboqs links against.
 */

#include <stdint.h>

#include <oqs/common.h>

#include "mlkem/mlkem_native.h"

/*
 * mlkem-native's public symbols are prefixed with MLK_CONFIG_NAMESPACE_PREFIX.
 * mlkem_native.h builds these names via MLK_API_NAMESPACE(), but #undef's
 * MLK_API_NAMESPACE_PREFIX at the end of the header, so MLK_API_NAMESPACE()
 * cannot be reused here. Replicate the concatenation for both the shim symbols
 * and their callees.
 */
#define MLK_OQS_CONCAT_(x, y) x##y
#define MLK_OQS_CONCAT(x, y) MLK_OQS_CONCAT_(x, y)
#define MLK_OQS_NS(sym) \
  MLK_OQS_CONCAT(MLK_OQS_CONCAT(MLK_CONFIG_NAMESPACE_PREFIX, _), sym)

#define mlk_oqs_keypair MLK_OQS_NS(keypair_oqs)
#define mlk_oqs_keypair_derand MLK_OQS_NS(keypair_derand_oqs)
#define mlk_oqs_enc MLK_OQS_NS(enc_oqs)
#define mlk_oqs_enc_derand MLK_OQS_NS(enc_derand_oqs)
#define mlk_oqs_dec MLK_OQS_NS(dec_oqs)

/* Collapse mlkem-native's error set to OQS_SUCCESS / OQS_ERROR. liboqs declares
 * these shims as returning int and casts the result to OQS_STATUS itself. */
static int mlk_oqs_status(int ret)
{
  return ret == 0 ? OQS_SUCCESS : OQS_ERROR;
}

int mlk_oqs_keypair(uint8_t *pk, uint8_t *sk)
{
  return mlk_oqs_status(MLK_OQS_NS(keypair)(pk, sk));
}

int mlk_oqs_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t *coins)
{
  return mlk_oqs_status(MLK_OQS_NS(keypair_derand)(pk, sk, coins));
}

int mlk_oqs_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk)
{
  return mlk_oqs_status(MLK_OQS_NS(enc)(ct, ss, pk));
}

int mlk_oqs_enc_derand(uint8_t *ct, uint8_t *ss, const uint8_t *pk,
                       const uint8_t *coins)
{
  return mlk_oqs_status(MLK_OQS_NS(enc_derand)(ct, ss, pk, coins));
}

int mlk_oqs_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk)
{
  return mlk_oqs_status(MLK_OQS_NS(dec)(ss, ct, sk));
}

#undef mlk_oqs_keypair
#undef mlk_oqs_keypair_derand
#undef mlk_oqs_enc
#undef mlk_oqs_enc_derand
#undef mlk_oqs_dec
#undef MLK_OQS_NS
#undef MLK_OQS_CONCAT
#undef MLK_OQS_CONCAT_
