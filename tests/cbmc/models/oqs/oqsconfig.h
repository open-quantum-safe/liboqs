// SPDX-License-Identifier: MIT

#ifndef OQS_CONFIG_H
#define OQS_CONFIG_H

/*
 * CBMC only needs the public type surface for the liboqs-owned dispatch and
 * utility translation units.  No imported algorithm implementation is enabled.
 */
#define OQS_VERSION_TEXT "cbmc-model"
#define OQS_VERSION_MAJOR 0
#define OQS_VERSION_MINOR 0
#define OQS_VERSION_PATCH 0

/* Keep the full stateful-signature object layout visible to the harnesses. */
#define OQS_ALLOW_STFL_KEY_AND_SIG_GEN 1

/* Avoid the non-POSIX malloc.h fallback while parsing common.c on macOS. */
#define OQS_HAVE_POSIX_MEMALIGN 1

#endif /* OQS_CONFIG_H */
