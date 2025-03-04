/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLK_SYMMETRIC_H
#define MLK_SYMMETRIC_H

#include <stddef.h>
#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include MLK_FIPS202_HEADER_FILE
#include MLK_FIPS202X4_HEADER_FILE

/* Macros denoting FIPS 203 specific Hash functions */

/* Hash function H, [FIPS 203, Section 4.1, Eq (4.4)] */
#define mlk_hash_h(OUT, IN, INBYTES) mlk_sha3_256(OUT, IN, INBYTES)

/* Hash function G, [FIPS 203, Section 4.1, Eq (4.5)] */
#define mlk_hash_g(OUT, IN, INBYTES) mlk_sha3_512(OUT, IN, INBYTES)

/* Hash function J, [FIPS 203, Section 4.1, Eq (4.4)] */
#define mlk_hash_j(OUT, IN, INBYTES) \
  mlk_shake256(OUT, MLKEM_SYMBYTES, IN, INBYTES)

/* PRF function, [FIPS 203, Section 4.1, Eq (4.3)]
 * Referring to (eq 4.3), `OUT` is assumed to contain `s || b`. */
#define mlk_prf_eta(ETA, OUT, IN) \
  mlk_shake256(OUT, (ETA) * MLKEM_N / 4, IN, MLKEM_SYMBYTES + 1)
#define mlk_prf_eta1(OUT, IN) mlk_prf_eta(MLKEM_ETA1, OUT, IN)
#define mlk_prf_eta2(OUT, IN) mlk_prf_eta(MLKEM_ETA2, OUT, IN)
#define mlk_prf_eta1_x4(OUT0, OUT1, OUT2, OUT3, IN0, IN1, IN2, IN3)            \
  mlk_shake256x4(OUT0, OUT1, OUT2, OUT3, (MLKEM_ETA1 * MLKEM_N / 4), IN0, IN1, \
                 IN2, IN3, MLKEM_SYMBYTES + 1)

/* XOF function, FIPS 203 4.1 */
#define mlk_xof_ctx mlk_shake128ctx
#define mlk_xof_x4_ctx mlk_shake128x4ctx
#define mlk_xof_init(CTX) mlk_shake128_init((CTX))
#define mlk_xof_absorb(CTX, IN, INBYTES) \
  mlk_shake128_absorb_once((CTX), (IN), (INBYTES))
#define mlk_xof_squeezeblocks(BUF, NBLOCKS, CTX) \
  mlk_shake128_squeezeblocks((BUF), (NBLOCKS), (CTX))
#define mlk_xof_release(CTX) mlk_shake128_release((CTX))

#define mlk_xof_x4_init(CTX) mlk_shake128x4_init((CTX))
#define mlk_xof_x4_absorb(CTX, IN0, IN1, IN2, IN3, INBYTES) \
  mlk_shake128x4_absorb_once((CTX), (IN0), (IN1), (IN2), (IN3), (INBYTES))
#define mlk_xof_x4_squeezeblocks(BUF0, BUF1, BUF2, BUF3, NBLOCKS, CTX) \
  mlk_shake128x4_squeezeblocks((BUF0), (BUF1), (BUF2), (BUF3), (NBLOCKS), (CTX))
#define mlk_xof_x4_release(CTX) mlk_shake128x4_release((CTX))

#define MLK_XOF_RATE SHAKE128_RATE

#endif /* MLK_SYMMETRIC_H */
