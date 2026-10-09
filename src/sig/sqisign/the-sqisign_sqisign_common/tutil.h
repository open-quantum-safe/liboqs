#ifndef TUTIL_H
#define TUTIL_H

#include <stddef.h>
#include <stdint.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

// Count leading zeros in a nonzero 64-bit word.
static inline int
sqisign_clz64(uint64_t x)
{
#if defined(_MSC_VER)
    unsigned long index;
#if defined(_M_X64) || defined(_M_ARM64)
    _BitScanReverse64(&index, x);
    return 63 - (int)index;
#else
    if (_BitScanReverse(&index, (unsigned long)(x >> 32)))
        return 31 - (int)index;
    _BitScanReverse(&index, (unsigned long)x);
    return 63 - (int)index;
#endif
#else
    return __builtin_clzll(x);
#endif
}

static inline int
sqisign_clrsb64(int64_t x)
{
#if defined(_MSC_VER)
    uint64_t u = (uint64_t)x;
    u ^= 0 - (u >> 63);
    return sqisign_clz64((u << 1) | 1);
#else
    return __builtin_clrsbll(x);
#endif
}

#if defined(_MSC_VER)
#define SQISIGN_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define SQISIGN_NOINLINE __attribute__((noinline))
#else
#define SQISIGN_NOINLINE
#endif

#if defined(__GNUC__) || defined(__clang__)
#define BSWAP16(i) __builtin_bswap16((i))
#define BSWAP32(i) __builtin_bswap32((i))
#define BSWAP64(i) __builtin_bswap64((i))
#define UNUSED __attribute__((unused))
#else
#define BSWAP16(i) ((((i) >> 8) & 0xff) | (((i) & 0xff00) << 8))
#define BSWAP32(i) ((((i) >> 24) & 0xff) | (((i) >> 8) & 0xff00) | (((i) & 0xff00) << 8) | ((i) << 24))
#define BSWAP64(i) ((BSWAP32((i) >> 32) & 0xffffffff) | (BSWAP32(i) << 32))
#define UNUSED
#endif

// Thread-storage-duration qualifier for state that must not be shared between concurrent operations.
//
// Define SQISIGN_SINGLE_THREADED to compile it away: the C11 thread-local machinery is too expensive on embedded
// targets, which are usually single-threaded anyway. Opt-out: if not defined, it is multithreaded-safe.
#if defined(SQISIGN_SINGLE_THREADED)
#define SQISIGN_THREAD_LOCAL
#elif defined(_MSC_VER)
#define SQISIGN_THREAD_LOCAL __declspec(thread)
#else
#define SQISIGN_THREAD_LOCAL _Thread_local
#endif

// fallback when the build system sets no radix: 64-bit limbs need a 128-bit type
#if !defined(RADIX_32) && !defined(RADIX_64)
#if defined(__SIZEOF_INT128__)
#define RADIX_64
#else
#define RADIX_32
#endif
#endif

#if defined(RADIX_64)
typedef uint64_t digit_t;
typedef int64_t sdigit_t;
__extension__ typedef __uint128_t ddigit_t;
__extension__ typedef __int128_t sddigit_t;
#define RADIX 64
#define LOG2RADIX 6
#define BSWAP_DIGIT(i) BSWAP64(i)
#elif defined(RADIX_32)
typedef uint32_t digit_t;
typedef int32_t sdigit_t;
typedef uint64_t ddigit_t;
typedef int64_t sddigit_t;
#define RADIX 32
#define LOG2RADIX 5
#define BSWAP_DIGIT(i) BSWAP32(i)
#else
#error "Radix must be 32bit or 64 bit"
#endif

#endif
