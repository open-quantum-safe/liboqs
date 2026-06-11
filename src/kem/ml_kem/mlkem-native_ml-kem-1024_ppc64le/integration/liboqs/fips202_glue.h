/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLK_INTEGRATION_LIBOQS_FIPS202_GLUE_H
#define MLK_INTEGRATION_LIBOQS_FIPS202_GLUE_H

/* Include OQS's own FIPS202 header */
#include "fips202.h"

/* OQS's FIPS202 is as-is compatible with the one expected
 * by mlkem-native, so just remove the mlk_xxx prefix. */
#define mlk_shake128ctx shake128ctx
#define mlk_shake128_absorb_once shake128_absorb_once
#define mlk_shake128_squeezeblocks shake128_squeezeblocks
#define mlk_shake128_init shake128_init
#define mlk_shake128_release shake128_release
#define mlk_shake256 shake256
#define mlk_sha3_256 sha3_256
#define mlk_sha3_512 sha3_512

#endif /* !MLK_INTEGRATION_LIBOQS_FIPS202_GLUE_H */
