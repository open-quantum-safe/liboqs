/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLK_NATIVE_META_H
#define MLK_NATIVE_META_H

/*
 * Default arithmetic backend
 */
#include "../sys.h"

#ifdef MLK_SYS_AARCH64
#include "aarch64/meta.h"
#endif

#ifdef MLK_SYS_X86_64_AVX2
#include "x86_64/meta.h"
#endif

#endif /* !MLK_NATIVE_META_H */
