// Based on supercop-20190110/crypto_sort/int32/x86

#include "crypto_sort_int32.h"

#include <stdint.h>
#define int32 int32_t

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

/* assume 2 <= n <= 0x40000000 */
void PQCLEAN_NTRUHPS40961229_CLEAN_crypto_sort_int32(int32 *array, size_t n) {
    size_t top, p, q, r, i, j;
    int32 *x = array;

    top = 1;
    while (top < n - top) {
        top += top;
    }

    for (p = top; p >= 1; p >>= 1) {
        i = 0;
        while (i + 2 * p <= n) {
            for (j = i; j < i + p; ++j) {
                int32_MINMAX(x[j], x[j + p]);
            }
            i += 2 * p;
        }
        for (j = i; j < n - p; ++j) {
            int32_MINMAX(x[j], x[j + p]);
        }

        i = 0;
        j = 0;
        for (q = top; q > p; q >>= 1) {
            if (j != i) {
                for (;;) {
                    if (j == n - q) {
                        goto done;
                    }
                    int32 a = x[j + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_MINMAX(a, x[j + r]);
                    }
                    x[j + p] = a;
                    ++j;
                    if (j == i + p) {
                        i += 2 * p;
                        break;
                    }
                }
            }
            while (i + p <= n - q) {
                for (j = i; j < i + p; ++j) {
                    int32 a = x[j + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_MINMAX(a, x[j + r]);
                    }
                    x[j + p] = a;
                }
                i += 2 * p;
            }
            /* now i + p > n - q */
            j = i;
            while (j < n - q) {
                int32 a = x[j + p];
                for (r = q; r > p; r >>= 1) {
                    int32_MINMAX(a, x[j + r]);
                }
                x[j + p] = a;
                ++j;
            }

done:
            ;
        }
    }
}
