#ifndef uint64_sort_h
#define uint64_sort_h

#include "namespace.h"

#define int64_sort CRYPTO_NAMESPACE(int64_sort)

#include <inttypes.h>

#define uint64_MINMAX(a,b) \
    do { \
        uint64_t c = (b) - (a); \
        c >>= 63; \
        c = -c; \
        c &= (a) ^ (b); \
        (a) ^= c; \
        (b) ^= c; \
    } while(0)

static void uint64_sort(uint64_t *x, long long n) {
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
                uint64_MINMAX(x[i], x[i + p]);
            }
        }
        i = 0;
        for (q = top; q > p; q >>= 1) {
            for (; i < n - q; ++i) {
                if (!(i & p)) {
                    uint64_t a = x[i + p];
                    for (r = q; r > p; r >>= 1) {
                        uint64_MINMAX(a, x[i + r]);
                    }
                    x[i + p] = a;
                }
            }
        }
    }
}

#endif
