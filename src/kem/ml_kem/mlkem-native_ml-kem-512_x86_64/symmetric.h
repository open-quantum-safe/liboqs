/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <stddef.h>
#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "fips202.h"
#include "fips202x4.h"

/* Macros denoting FIPS-203 specific Hash functions */

/* Hash function H, FIPS-203 4.1 (eq 4.4) */
#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)

/* Hash function G, FIPS-203 4.1 (eq 4.5) */
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)

/* Hash function J, FIPS-203 4.1 (eq 4.4) */
#define hash_j(OUT, IN, INBYTES) shake256(OUT, MLKEM_SYMBYTES, IN, INBYTES)

/* PRF function, FIPS-203 4.1 (eq 4.3)
 * Referring to (eq 4.3), `OUT` is assumed to contain `s || b`. */
#define prf_eta(ETA, OUT, IN) \
  shake256(OUT, (ETA) * MLKEM_N / 4, IN, MLKEM_SYMBYTES + 1)
#define prf_eta1(OUT, IN) prf_eta(MLKEM_ETA1, OUT, IN)
#define prf_eta2(OUT, IN) prf_eta(MLKEM_ETA2, OUT, IN)
#define prf_eta1_x4(OUT0, OUT1, OUT2, OUT3, IN0, IN1, IN2, IN3)            \
  shake256x4(OUT0, OUT1, OUT2, OUT3, (MLKEM_ETA1 * MLKEM_N / 4), IN0, IN1, \
             IN2, IN3, MLKEM_SYMBYTES + 1)

/* XOF function, FIPS-203 4.1 */
#define xof_ctx shake128ctx
#define xof_x4_ctx shake128x4ctx
#define xof_init(CTX) shake128_init((CTX))
#define xof_absorb(CTX, IN, INBYTES) \
  shake128_absorb_once((CTX), (IN), (INBYTES))
#define xof_squeezeblocks(BUF, NBLOCKS, CTX) \
  shake128_squeezeblocks((BUF), (NBLOCKS), (CTX))
#define xof_release(CTX) shake128_release((CTX))

#define xof_x4_init(CTX) shake128x4_init((CTX))
#define xof_x4_absorb(CTX, IN0, IN1, IN2, IN3, INBYTES) \
  shake128x4_absorb_once((CTX), (IN0), (IN1), (IN2), (IN3), (INBYTES))
#define xof_x4_squeezeblocks(BUF0, BUF1, BUF2, BUF3, NBLOCKS, CTX) \
  shake128x4_squeezeblocks((BUF0), (BUF1), (BUF2), (BUF3), (NBLOCKS), (CTX))
#define xof_x4_release(CTX) shake128x4_release((CTX))

#define XOF_RATE SHAKE128_RATE

#endif /* SYMMETRIC_H */
