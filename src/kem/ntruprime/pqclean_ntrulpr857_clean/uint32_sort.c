#include <stdint.h>

#include "uint32.h"

static void minmax(uint32_t *x, uint32_t *y) {
    uint32_t xi = *x;
    uint32_t yi = *y;
    uint32_t xy = xi ^ yi;
    uint32_t c = yi - xi;
    c ^= xy & (c ^ yi ^ 0x80000000);
    c >>= 31;
    c = ~c + 1; // two's complement
    c &= xy;
    *x = xi ^ c;
    *y = yi ^ c;
}

void PQCLEAN_NTRULPR857_CLEAN_uint32_sort(uint32_t *x, int n) {
    int top, p, q, i;

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
                minmax(x + i, x + i + p);
            }
        }
        for (q = top; q > p; q >>= 1) {
            for (i = 0; i < n - q; ++i) {
                if (!(i & p)) {
                    minmax(x + i + p, x + i + q);
                }
            }
        }
    }
}
