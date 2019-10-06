// XXX: Temporary placeholder for a faster sort.
// Copied from supercop-20190110/crypto_sort/int32/portable3

#include <stdint.h>

#include "crypto_sort.h"

#define int32_MINMAX(a,b) \
    do { \
        int32_t ab = (b) ^ (a); \
        int32_t c = (int32_t)((int64_t)(b) - (int64_t)(a)); \
        c ^= ab & (c ^ (b)); \
        c >>= 31; \
        c &= ab; \
        (a) ^= c; \
        (b) ^= c; \
    } while(0)

void PQCLEAN_NTRUHPS4096821_CLEAN_crypto_sort(void *array, long long n) {
    long long top, p, q, r, i;
    int32_t *x = array;

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
