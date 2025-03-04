/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLK_NATIVE_META_H
#define MLK_NATIVE_META_H

/*
 * Default arithmetic backend
 */
#include "../sys.h"

#ifdef MLK_SYS_AARCH64
#include "aarch64/meta.h"
#endif /* MLK_SYS_AARCH64 */

#ifdef MLK_SYS_X86_64_AVX2
#include "x86_64/meta.h"
#endif /* MLK_SYS_X86_64 */

#endif /* MLK_NATIVE_META_H */
