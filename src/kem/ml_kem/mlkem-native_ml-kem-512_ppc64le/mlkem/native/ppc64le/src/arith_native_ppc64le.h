/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLK_NATIVE_PPC64LE_SRC_ARITH_NATIVE_PPC64LE_H
#define MLK_NATIVE_PPC64LE_SRC_ARITH_NATIVE_PPC64LE_H

#include <stdint.h>
#include "../../../common.h"

void mlk_ntt_ppc(int16_t *);

void mlk_intt_ppc(int16_t *);

void mlk_reduce_ppc(int16_t *r);

void mlk_poly_tomont_ppc(int16_t *);

#endif /* MLK_NATIVE_PPC64LE_SRC_ARITH_NATIVE_PPC64LE_H */
