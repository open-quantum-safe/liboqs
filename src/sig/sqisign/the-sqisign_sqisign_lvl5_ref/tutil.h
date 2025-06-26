#ifndef TUTIL_H
#define TUTIL_H

#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define BSWAP16(i) __builtin_bswap16((i))
#define BSWAP32(i) __builtin_bswap32((i))
#define BSWAP64(i) __builtin_bswap64((i))
#define UNUSED __attribute__((unused))
#else
#define BSWAP16(i) ((((i) >> 8) & 0xff) | (((i) & 0xff00) << 8))
#define BSWAP32(i)                                                                                 \
    ((((i) >> 24) & 0xff) | (((i) >> 8) & 0xff00) | (((i) & 0xff00) << 8) | ((i) << 24))
#define BSWAP64(i) ((BSWAP32((i) >> 32) & 0xffffffff) | (BSWAP32(i) << 32)
#define UNUSED
#endif

#if defined(RADIX_64)
#define digit_t uint64_t
#define sdigit_t int64_t
#define RADIX 64
#define LOG2RADIX 6
#define BSWAP_DIGIT(i) BSWAP64(i)
#elif defined(RADIX_32)
#define digit_t uint32_t
#define sdigit_t int32_t
#define RADIX 32
#define LOG2RADIX 5
#define BSWAP_DIGIT(i) BSWAP32(i)
#else
#error "Radix must be 32bit or 64 bit"
#endif

#endif
