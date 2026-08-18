/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === Localization affecting all implementations can be inserted here. */

#ifndef _PLAT_LOCAL_H_
#define _PLAT_LOCAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include "slh_sys.h"

#ifndef PLAT_VERS_STR
#define PLAT_VERS_STR "dev version"
#define PLAT_VERS_MAJ 0
#define PLAT_VERS_MIN 0
#endif

  /* === platform detection logic */

#ifndef PLAT_XLEN

#if defined(__riscv)

#define PLAT_XLEN __riscv_xlen
#if defined(__riscv_vector)
#define PLAT_ARCH_RVV
#define PLAT_ARCH_STR "PLAT_ARCH_RVV"
#elif (__riscv_xlen == 32)
#define PLAT_ARCH_RV32
#define PLAT_ARCH_STR "PLAT_ARCH_RV32"
#else
#define PLAT_ARCH_RV64
#define PLAT_ARCH_STR "PLAT_ARCH_RV64"
#endif

#elif defined(__arm__)
#define PLAT_XLEN 32
#define PLAT_ARCH_STR "PLAT_ARCH_ARM32"
#define PLAT_ARCH_ARM32

#elif defined(__aarch64__)
#define PLAT_XLEN 64
#define PLAT_ARCH_STR "PLAT_ARCH_ARM64"
#define PLAT_ARCH_ARM64

#elif defined(__i386__)
#define PLAT_XLEN 32
#define PLAT_ARCH_STR "PLAT_ARCH_IA32"
#define PLAT_ARCH_IA32

#elif defined(__x86_64__)
#define PLAT_XLEN 64
#define PLAT_ARCH_STR "PLAT_ARCH_X64"
#define PLAT_ARCH_X64

#elif (__WORDSIZE == 32)
#define PLAT_XLEN 32
#define PLAT_ARCH_STR "PLAT_ARCH_GEN32"
#define PLAT_ARCH_GEN32

#else
/* fallback for everything */
#define PLAT_XLEN 64
#define PLAT_ARCH_STR "PLAT_ARCH_GEN64"
#define PLAT_ARCH_GEN64
#endif

/* PLAT_XLEN signals that the rest of the macros are defined too */
#endif

/* === Assume-Assert checks */

/* No-op for production */
#ifndef __CPROVER__

#ifndef XDEBUG
#define XASSERT(x)
#define XASSUME(x)
#define XTMPVAR(x)

/* Runtime checks in debug build */
#else
#include <assert.h>
#define XASSERT(x) assert(x)
#define XASSUME(x) assert(x)
#define XTMPVAR(x) x
/* XDEBUG */
#endif

/* For CBMC (formal checks) */
#else
#include <assert.h>
#include <stdio.h>
#define XASSERT(x) assert(x)
#define XASSUME(x) __CPROVER_assume(x)
#define XTMPVAR(x) x
#define XPROOFS
#endif /* __CPROVER__ */

  /* revert if not big endian */

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
/* nop */
#define rev8_be32(x) (x)
#else
/* RISC-V: grev(x, 0x18) or rev8 */

static SLH_INLINE uint32_t rev8_be32(uint32_t x)
{
  return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) |
         ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
}
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define rev8_be64(x) (x)
#else
/* RISC-V: grev(x, 0x38) or rev8(x) */
static SLH_INLINE uint64_t rev8_be64(uint64_t x)
{
  return (x << 56) | ((x & 0x000000000000FF00LL) << 40) |
         ((x & 0x0000000000FF0000LL) << 24) |
         ((x & 0x00000000FF000000LL) << 8) | ((x & 0x000000FF00000000LL) >> 8) |
         ((x & 0x0000FF0000000000LL) >> 24) |
         ((x & 0x00FF000000000000LL) >> 40) | (x >> 56);
}
#endif

  /* rotate left */

  static SLH_INLINE uint32_t rol32(uint32_t x, uint32_t n)
  {
    return (x << n) | (x >> (32 - n));
  }

  static SLH_INLINE uint64_t rol64(uint64_t x, uint64_t n)
  {
    return (x << n) | (x >> (64 - n));
  }

  /* rotate right (RISC-V ROR or RORI) */

  static SLH_INLINE uint32_t ror32(uint32_t x, uint32_t n)
  {
    return (x >> n) | (x << (32 - n));
  }

  static SLH_INLINE uint64_t ror64(uint64_t x, uint64_t n)
  {
    return (x >> n) | (x << (64 - n));
  }

  /* and with negate (RISC-V ANDN) */

  static SLH_INLINE uint32_t andn32(uint32_t x, uint32_t y) { return x & ~y; }

  static SLH_INLINE uint64_t andn64(uint64_t x, uint64_t y) { return x & ~y; }

  /* little-endian loads and stores (unaligned) */

  static SLH_INLINE uint16_t get16u_le(const uint8_t *v)
  {
    return (((uint16_t)v[1]) << 8) | ((uint16_t)v[0]);
  }

  static SLH_INLINE void put16u_le(uint8_t *v, uint16_t x)
  {
    v[0] = x;
    v[1] = x >> 8;
  }

  static SLH_INLINE uint32_t get32u_le(const uint8_t *v)
  {
    return ((uint32_t)v[0]) | (((uint32_t)v[1]) << 8) |
           (((uint32_t)v[2]) << 16) | (((uint32_t)v[3]) << 24);
  }

  static SLH_INLINE void put32u_le(uint8_t *v, uint32_t x)
  {
    v[0] = x;
    v[1] = x >> 8;
    v[2] = x >> 16;
    v[3] = x >> 24;
  }

  static SLH_INLINE uint64_t get64u_le(const uint8_t *v)
  {
    return ((uint64_t)v[0]) | (((uint64_t)v[1]) << 8) |
           (((uint64_t)v[2]) << 16) | (((uint64_t)v[3]) << 24) |
           (((uint64_t)v[4]) << 32) | (((uint64_t)v[5]) << 40) |
           (((uint64_t)v[6]) << 48) | (((uint64_t)v[7]) << 56);
  }

  static SLH_INLINE void put64u_le(uint8_t *v, uint64_t x)
  {
    v[0] = x;
    v[1] = x >> 8;
    v[2] = x >> 16;
    v[3] = x >> 24;
    v[4] = x >> 32;
    v[5] = x >> 40;
    v[6] = x >> 48;
    v[7] = x >> 56;
  }

  /* big-endian loads and stores (unaligned) */

  static SLH_INLINE uint16_t get16u_be(const uint8_t *v)
  {
    return (((uint16_t)v[0]) << 8) | ((uint16_t)v[1]);
  }

  static SLH_INLINE void put16u_be(uint8_t *v, uint16_t x)
  {
    v[0] = x >> 8;
    v[1] = x;
  }

  static SLH_INLINE uint32_t get32u_be(const uint8_t *v)
  {
    return (((uint32_t)v[0]) << 24) | (((uint32_t)v[1]) << 16) |
           (((uint32_t)v[2]) << 8) | ((uint32_t)v[3]);
  }

  static SLH_INLINE void put32u_be(uint8_t *v, uint32_t x)
  {
    v[0] = x >> 24;
    v[1] = x >> 16;
    v[2] = x >> 8;
    v[3] = x;
  }

  static SLH_INLINE uint64_t get64u_be(const uint8_t *v)
  {
    return (((uint64_t)v[0]) << 56) | (((uint64_t)v[1]) << 48) |
           (((uint64_t)v[2]) << 40) | (((uint64_t)v[3]) << 32) |
           (((uint64_t)v[4]) << 24) | (((uint64_t)v[5]) << 16) |
           (((uint64_t)v[6]) << 8) | ((uint64_t)v[7]);
  }

  static SLH_INLINE void put64u_be(uint8_t *v, uint64_t x)
  {
    v[0] = x >> 56;
    v[1] = x >> 48;
    v[2] = x >> 40;
    v[3] = x >> 32;
    v[4] = x >> 24;
    v[5] = x >> 16;
    v[6] = x >> 8;
    v[7] = x;
  }

#ifdef __cplusplus
}
#endif

/* _PLAT_LOCAL_H_ */
#endif
