// SPDX-License-Identifier: Apache-2.0

#ifndef FIPS202_H
#define FIPS202_H

#include <oqs/sha3.h>

#define shake256incctx OQS_SHA3_shake256_inc_ctx
#define shake256_inc_init OQS_SHA3_shake256_inc_init
#define shake256_inc_absorb OQS_SHA3_shake256_inc_absorb
#define shake256_inc_finalize OQS_SHA3_shake256_inc_finalize
#define shake256_inc_squeeze OQS_SHA3_shake256_inc_squeeze
#define shake256_inc_ctx_release OQS_SHA3_shake256_inc_ctx_release
#define shake256_inc_ctx_reset OQS_SHA3_shake256_inc_ctx_reset

#endif
