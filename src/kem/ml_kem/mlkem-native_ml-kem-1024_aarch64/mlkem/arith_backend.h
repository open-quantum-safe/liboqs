/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MLK_ARITH_BACKEND_H
#define MLK_ARITH_BACKEND_H

#include "common.h"

#if defined(MLK_ARITH_BACKEND_IMPL)
#include MLK_ARITH_BACKEND_IMPL

/* Include to enforce consistency of API and implementation,
 * and conduct sanity checks on the backend.
 *
 * Keep this _after_ the inclusion of the backend; otherwise,
 * the sanity checks won't have an effect. */
#if defined(MLK_CHECK_APIS)
#include "native/api.h"
#endif
#endif

#endif /* MLK_ARITH_BACKEND_H */
