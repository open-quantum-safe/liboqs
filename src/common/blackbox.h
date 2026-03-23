
/**
 * Value barrier: prevent compiler from optimizing on secret values
 */
#if defined(OQS_DISABLE_MEM_BLACK_BOX)

/** DANGER!
 * __OQS_DISABLE_MEM_BLACK_BOX will disable OQS's value barrier, which can lead
 * to timing side channels introduced by aggressive compiler optimization. This
 * is defined for internal testing only and should NEVER be used externally.
 */
#define OQS_MEM_BLACK_BOX(v) (void)v

#elif defined(__GNUC__) || defined(__clang__)

#define OQS_MEM_BLACK_BOX(v)                                                   \
    do {                                                                       \
        __asm__ volatile("" : "+r"(v) :);                                      \
    } while (0)

#elif defined(_MSC_VER)

#include <intrin.h>

static __forceinline void oqs_black_box_impl(volatile void *p, size_t sz) {
    volatile unsigned char *q = (volatile unsigned char *)p;
    unsigned char tmp;
    size_t i;
    _ReadWriteBarrier();
    for (i = 0; i < sz; i++) {
        tmp = q[i]; /* volatile read  */
        q[i] = tmp; /* volatile write */
    }
    _ReadWriteBarrier();
}

#define OQS_MEM_BLACK_BOX(v)                                                   \
    oqs_black_box_impl((volatile void *)&(v), sizeof(v))

#else

#pragma message("OQS_MEM_BLACK_BOX: unrecognised compiler. "                   \
                "Falling back to volatile round-trip. "                        \
                "Verify generated assembly for constant-time correctness.")

static inline void oqs_black_box_fallback(volatile void *p, size_t sz)
    __attribute__((unused));

static inline void oqs_black_box_fallback(volatile void *p, size_t sz)
    __attribute__((unused)) {
    volatile unsigned char *q = (volatile unsigned char *)p;
    unsigned char tmp;
    size_t i;
    for (i = 0; i < sz; i++) {
        tmp = q[i];
        q[i] = tmp;
    }
}

#define OQS_MEM_BLACK_BOX(v)                                                   \
    oqs_black_box_fallback((volatile void *)&(v), sizeof(v))
#endif
