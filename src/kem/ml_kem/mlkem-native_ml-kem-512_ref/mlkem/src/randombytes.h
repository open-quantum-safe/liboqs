/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLK_RANDOMBYTES_H
#define MLK_RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

#include "cbmc.h"
#include "common.h"

#if !defined(MLK_CONFIG_CUSTOM_RANDOMBYTES)
void randombytes(uint8_t *out, size_t outlen);
static MLK_INLINE void mlk_randombytes(uint8_t *out, size_t outlen)
__contract__(
  requires(memory_no_alias(out, outlen))
  assigns(memory_slice(out, outlen))) { randombytes(out, outlen); }
#endif /* !MLK_CONFIG_CUSTOM_RANDOMBYTES */

#endif /* !MLK_RANDOMBYTES_H */
