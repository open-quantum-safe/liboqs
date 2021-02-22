#include "uint32_sort.h"

#include "int32_sort.h"


/* can save time by vectorizing xor loops */
/* can save time by integrating xor loops with int32_sort */

void PQCLEAN_MCELIECE6688128F_AVX_uint32_sort(uint32_t *x, size_t n) {
    size_t j;
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
    PQCLEAN_MCELIECE6688128F_AVX_int32_sort((int32_t *) x, n);
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
}
