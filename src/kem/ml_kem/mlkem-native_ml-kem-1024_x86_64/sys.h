/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLKEM_NATIVE_SYS_H
#define MLKEM_NATIVE_SYS_H

/* Check if we're running on an AArch64 little endian system. _M_ARM64 is set by
 * MSVC. */
#if defined(__AARCH64EL__) || defined(_M_ARM64)
#define SYS_AARCH64
#endif

/* Check if we're running on an AArch64 big endian system. */
#if defined(__AARCH64EB__)
#define SYS_AARCH64_EB
#endif

#if defined(__x86_64__)
#define SYS_X86_64
#if defined(__AVX2__)
#define SYS_X86_64_AVX2
#endif
#endif /* __x86_64__ */

/* Try to find endianness, if not forced through CFLAGS already */
#if !defined(SYS_LITTLE_ENDIAN) && !defined(SYS_BIG_ENDIAN)
#if defined(__BYTE_ORDER__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define SYS_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define SYS_BIG_ENDIAN
#else /* __BYTE_ORER__ */
#error "__BYTE_ORDER__ defined, but don't recognize value."
#endif /* __BYTE_ORER__ */
#endif /* !defined(__BYTE_ORER__) */
#endif /* defined(SYS_LITTLE_ENDIAN) || defined(SYS_BIG_ENDIAN) */

/* If FORCE_AARCH64 is set, assert that we're indeed on an AArch64 system. */
#if defined(FORCE_AARCH64) && !defined(SYS_AARCH64)
#error "FORCE_AARCH64 is set, but we don't seem to be on an AArch64 system."
#endif

/* If FORCE_AARCH64_EB is set, assert that we're indeed on a big endian AArch64
 * system. */
#if defined(FORCE_AARCH64_EB) && !defined(SYS_AARCH64_EB)
#error "FORCE_AARCH64_EB is set, but we don't seem to be on an AArch64 system."
#endif

/* If FORCE_X86_64 is set, assert that we're indeed on an X86_64 system. */
#if defined(FORCE_X86_64) && !defined(SYS_X86_64)
#error "FORCE_X86_64 is set, but we don't seem to be on an X86_64 system."
#endif

/*
 * C90 does not have the inline compiler directive yet.
 * We don't use it in C90 builds.
 * However, in that case the compiler warns about some inline functions in
 * header files not being used in every compilation unit that includes that
 * header. To work around it we silence that warning in that case using
 * __attribute__((unused)).
 */

/* Do not use inline for C90 builds*/
#if !defined(INLINE)
#if !defined(inline)
#if defined(_MSC_VER)
#define INLINE __inline
#define ALWAYS_INLINE __forceinline
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define INLINE inline
#define ALWAYS_INLINE __attribute__((always_inline))
#else
#define INLINE __attribute__((unused))
#define ALWAYS_INLINE
#endif

#else
#define INLINE inline
#define ALWAYS_INLINE __attribute__((always_inline))
#endif
#endif

/*
 * C90 does not have the restrict compiler directive yet.
 * We don't use it in C90 builds.
 */
#if !defined(restrict)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define RESTRICT restrict
#else
#define RESTRICT
#endif

#else

#define RESTRICT restrict
#endif

#define DEFAULT_ALIGN 32
#if defined(_WIN32)
#define ALIGN __declspec(align(DEFAULT_ALIGN))
#define asm __asm
#else
#define asm __asm__
#define ALIGN __attribute__((aligned(DEFAULT_ALIGN)))
#endif

#endif /* MLKEM_NATIVE_SYS_H */
