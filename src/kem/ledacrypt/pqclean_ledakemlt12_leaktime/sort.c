#include "sort.h"

/*
    Constant-time uint32_t sorting by Daniel J. Bernstein
    Source: https://sorting.cr.yp.to
*/

#define int32_MINMAX(a,b) \
    do { \
        int32 ab = (b) ^ (a); \
        int32 c = (b) - (a); \
        c ^= ab & (c ^ (b)); \
        c >>= 31; \
        c &= ab; \
        (a) ^= c; \
        (b) ^= c; \
    } while(0)

static void int32_sort(int32 *x, size_t n) {
    size_t top, p, q, r, i, j;

    if (n < 2) {
        return;
    }
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

/* can save time by integrating xor loops with int32_sort */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_uint32_sort(uint32_t *x, size_t n) {
    size_t j;
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
    int32_sort((int32_t *) x, n);
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
}
