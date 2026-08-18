#ifndef PQCLEAN_MCELIECE348864_CLEAN_int32_sort_h
#define PQCLEAN_MCELIECE348864_CLEAN_int32_sort_h

#include "namespace.h"

#define int32_sort CRYPTO_NAMESPACE(int32_sort)

#include <inttypes.h>

#define int32_MINMAX(a,b) \
    do { \
        int32_t ab = (b) ^ (a); \
        int32_t c = (b) - (a); \
        c ^= ab & ((c) ^ (b)); \
        c >>= 31; \
        c &= ab; \
        (a) ^= c; \
        (b) ^= c; \
    } while(0)

static void int32_sort(int32_t *x, long long n) {
    long long top, p, q, r, i;

    if (n < 2) {
        return;
    }
    top = 1;
    while (top < n - top) {
        top += top;
    }

    for (p = top; p > 0; p >>= 1) {
        for (i = 0; i < n - p; ++i) {
            if (!(i & p)) {
                int32_MINMAX(x[i], x[i + p]);
            }
        }
        i = 0;
        for (q = top; q > p; q >>= 1) {
            for (; i < n - q; ++i) {
                if (!(i & p)) {
                    int32_t a = x[i + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_MINMAX(a, x[i + r]);
                    }
                    x[i + p] = a;
                }
            }
        }
    }
}

#endif
