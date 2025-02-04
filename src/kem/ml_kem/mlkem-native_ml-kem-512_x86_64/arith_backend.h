/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#if !defined(MLKEM_NATIVE_ARITH_IMPL_H)
#define MLKEM_NATIVE_ARITH_IMPL_H

#include "common.h"

#if defined(MLKEM_NATIVE_ARITH_BACKEND_IMPL)
#include MLKEM_NATIVE_ARITH_BACKEND_IMPL

/* Include to enforce consistency of API and implementation,
 * and conduct sanity checks on the backend.
 *
 * Keep this _after_ the inclusion of the backend; otherwise,
 * the sanity checks won't have an effect. */
#if defined(MLKEM_NATIVE_CHECK_APIS)
#include "native/api.h"
#endif
#endif

#endif /* MLKEM_NATIVE_ARITH_IMPL_H */
