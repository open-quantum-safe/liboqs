/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MLK_NATIVE_PPC64LE_SRC_DEFAULT_IMPL_H
#define MLK_NATIVE_PPC64LE_SRC_DEFAULT_IMPL_H
#ifdef MLK_ARITH_PROFILE_IMPL_H
#error Only one MLKEM_ARITH assembly profile can be defined -- did you include multiple profiles?
#else
#define MLK_ARITH_PROFILE_IMPL_H

#include "../../../params.h"
#include "arith_native_ppc64le.h"

/* Set of primitives that this backend replaces */
#define MLK_USE_NATIVE_NTT
#define MLK_USE_NATIVE_INTT
#define MLK_USE_NATIVE_POLY_REDUCE
#define MLK_USE_NATIVE_POLY_TOMONT

static MLK_INLINE void mlk_ntt_native(int16_t data[MLKEM_N]) {
	mlk_ntt_ppc(data);
}

static MLK_INLINE void mlk_intt_native(int16_t data[MLKEM_N]) {
	mlk_intt_ppc(data);
}

static MLK_INLINE void mlk_poly_reduce_native(int16_t data[MLKEM_N]) {
	mlk_reduce_ppc(data);
}

static MLK_INLINE void mlk_poly_tomont_native(int16_t data[MLKEM_N]) {
	mlk_poly_tomont_ppc(data);
}

#endif /* MLK_ARITH_PROFILE_IMPL_H */

#endif /* MLK_NATIVE_PPC64LE_SRC_DEFAULT_IMPL_H */
