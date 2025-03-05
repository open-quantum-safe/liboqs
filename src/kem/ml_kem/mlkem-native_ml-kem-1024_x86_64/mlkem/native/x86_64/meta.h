/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MLK_NATIVE_X86_64_META_H
#define MLK_NATIVE_X86_64_META_H
/* ML-KEM arithmetic native profile for clean assembly */

#ifdef MLK_ARITH_PROFILE_H
#error Only one MLKEM_ARITH assembly profile can be defined -- did you include multiple profiles?
#else
#define MLK_ARITH_PROFILE_H

/* Identifier for this backend so that source and assembly files
 * in the build can be appropriately guarded. */
#define MLK_ARITH_BACKEND_X86_64_DEFAULT

#define MLK_ARITH_BACKEND_NAME X86_64_DEFAULT

/* Filename of the C backend implementation.
 * This is not inlined here because this header is included in assembly
 * files as well. */
#define MLK_ARITH_BACKEND_IMPL "native/x86_64/src/default_impl.h"

#endif /* MLK_ARITH_PROFILE_H */

#endif /* MLK_NATIVE_X86_64_META_H */
