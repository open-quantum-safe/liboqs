/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_SYMMETRIC_H
#define MLD_SYMMETRIC_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"

#include "fips202/fips202.h"

#define STREAM128_BLOCKBYTES SHAKE128_RATE
#define STREAM256_BLOCKBYTES SHAKE256_RATE

#define mld_xof256_ctx keccak_state
#define mld_xof256_init(CTX) shake256_init(CTX)
#define mld_xof256_absorb(CTX, IN, INBYTES) \
  do                                        \
  {                                         \
    shake256_absorb(CTX, IN, INBYTES);      \
    shake256_finalize(CTX);                 \
  } while (0)
#define mld_xof256_release(CTX) shake256_release(CTX)

#define mld_xof256_squeezeblocks(OUT, OUTBLOCKS, STATE) \
  shake256_squeezeblocks(OUT, OUTBLOCKS, STATE)

#define mld_xof128_ctx keccak_state
#define mld_xof128_init(CTX) shake128_init(CTX)
#define mld_xof128_absorb(CTX, IN, INBYTES) \
  do                                        \
  {                                         \
    shake128_absorb(CTX, IN, INBYTES);      \
    shake128_finalize(CTX);                 \
  } while (0)
#define mld_xof128_release(CTX) shake128_release(CTX)


#define mld_xof128_squeezeblocks(OUT, OUTBLOCKS, STATE) \
  shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)

#define mld_xof256_x4_ctx mld_shake256x4ctx
#define mld_xof256_x4_init(CTX) mld_shake256x4_init((CTX))
#define mld_xof256_x4_absorb(CTX, IN, INBYTES)                          \
  mld_shake256x4_absorb_once((CTX), (IN)[0], (IN)[1], (IN)[2], (IN)[3], \
                             (INBYTES))
#define mld_xof256_x4_squeezeblocks(BUF, NBLOCKS, CTX)                 \
  mld_shake256x4_squeezeblocks((BUF)[0], (BUF)[1], (BUF)[2], (BUF)[3], \
                               (NBLOCKS), (CTX))
#define mld_xof256_x4_release(CTX) mld_shake256x4_release((CTX))

#define mld_xof128_x4_ctx mld_shake128x4ctx
#define mld_xof128_x4_init(CTX) mld_shake128x4_init((CTX))
#define mld_xof128_x4_absorb(CTX, IN, INBYTES)                          \
  mld_shake128x4_absorb_once((CTX), (IN)[0], (IN)[1], (IN)[2], (IN)[3], \
                             (INBYTES))
#define mld_xof128_x4_squeezeblocks(BUF, NBLOCKS, CTX)                 \
  mld_shake128x4_squeezeblocks((BUF)[0], (BUF)[1], (BUF)[2], (BUF)[3], \
                               (NBLOCKS), (CTX))
#define mld_xof128_x4_release(CTX) mld_shake128x4_release((CTX))

#endif /* !MLD_SYMMETRIC_H */
