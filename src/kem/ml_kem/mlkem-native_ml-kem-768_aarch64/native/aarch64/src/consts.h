/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#if !defined(MLKEM_NATIVE_AARCH64_CONSTS)
#define MLKEM_NATIVE_AARCH64_CONSTS

#include <stdint.h>
#include "../../../common.h"

#define zetas_mulcache_native MLKEM_NAMESPACE(zetas_mulcache_native)
extern const int16_t zetas_mulcache_native[256];

#define zetas_mulcache_twisted_native \
  MLKEM_NAMESPACE(zetas_mulcache_twisted_native)
extern const int16_t zetas_mulcache_twisted_native[256];

#endif /* MLKEM_NATIVE_AARCH64_CONSTS */
