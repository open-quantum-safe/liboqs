/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLK_INTEGRATION_LIBOQS_FIPS202X4_GLUE_H
#define MLK_INTEGRATION_LIBOQS_FIPS202X4_GLUE_H

/* Include OQS's own FIPS202_X4 header */
#include "fips202x4.h"

/* OQS's FIPS202_X4 is as-is compatible with the one expected
 * by mlkem-native, so just remove the mlk_xxx prefix. */
#define mlk_shake128x4ctx shake128x4ctx
#define mlk_shake128x4_absorb_once shake128x4_absorb_once
#define mlk_shake128x4_squeezeblocks shake128x4_squeezeblocks
#define mlk_shake128x4_init shake128x4_init
#define mlk_shake128x4_release shake128x4_release
#define mlk_shake256x4 shake256x4

#endif /* !MLK_INTEGRATION_LIBOQS_FIPS202X4_GLUE_H */
