/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef SLH_SYS_H
#define SLH_SYS_H

/*
 * C90 does not have the inline compiler directive yet.
 * We don't use it in C90 builds.
 * However, in that case the compiler warns about some inline functions in
 * header files not being used in every compilation unit that includes that
 * header. To work around it we silence that warning in that case using
 * __attribute__((unused)).
 */

/* Do not use inline for C90 builds*/
#if !defined(SLH_INLINE)
#if !defined(inline)
#if defined(_MSC_VER)
#define SLH_INLINE __inline
/* Don't combine __inline and __forceinline */
#define SLH_ALWAYS_INLINE __forceinline
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define SLH_INLINE inline
#define SLH_ALWAYS_INLINE SLH_INLINE __attribute__((always_inline))
#else
#define SLH_INLINE __attribute__((unused))
#define SLH_ALWAYS_INLINE SLH_INLINE
#endif

#else /* !inline */
#define SLH_INLINE inline
#define SLH_ALWAYS_INLINE SLH_INLINE __attribute__((always_inline))
#endif /* inline */
#endif /* !SLH_INLINE */

#endif /* SLH_SYS_H */
