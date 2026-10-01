#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <string.h>
#include <stdint.h>

#define ROUND_UP4(x)  (((x) + 3) & ~3)
#define ROUND_UP16(x) (((x) + 15) & ~15)

static inline void store_u16_be(uint8_t out[2], uint16_t x)
{
    out[0] = (uint8_t)(x >> 8);
    out[1] = (uint8_t)x;
}

static inline void store_u64_be(uint8_t out[8], uint64_t x)
{
    out[0] = (uint8_t)(x >> 56);
    out[1] = (uint8_t)(x >> 48);
    out[2] = (uint8_t)(x >> 40);
    out[3] = (uint8_t)(x >> 32);
    out[4] = (uint8_t)(x >> 24);
    out[5] = (uint8_t)(x >> 16);
    out[6] = (uint8_t)(x >> 8);
    out[7] = (uint8_t)x;
}

static inline void secure_zero(void *ptr, size_t len)
{
#if defined(__APPLE__)
    (void)memset_s(ptr, len, 0, len);
#elif defined(__GLIBC__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    explicit_bzero(ptr, len);
#else
    static void *(*const volatile memset_ptr)(void *, int, size_t) = memset;
    memset_ptr(ptr, 0, len);
#endif
}

#endif // UTIL_H
