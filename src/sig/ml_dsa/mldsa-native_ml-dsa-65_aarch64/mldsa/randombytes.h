/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_RANDOMBYTES_H
#define MLD_RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

#include "cbmc.h"
#include "common.h"

#if !defined(MLD_CONFIG_CUSTOM_RANDOMBYTES)
void randombytes(uint8_t *out, size_t outlen);
static MLD_INLINE void mld_randombytes(uint8_t *out, size_t outlen)
__contract__(
  requires(memory_no_alias(out, outlen))
  assigns(memory_slice(out, outlen))
) { randombytes(out, outlen); }
#endif /* !MLD_CONFIG_CUSTOM_RANDOMBYTES */
#endif /* !MLD_RANDOMBYTES_H */
