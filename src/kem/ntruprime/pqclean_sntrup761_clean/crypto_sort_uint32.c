#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"


/* can save time by vectorizing xor loops */
/* can save time by integrating xor loops with int32_sort */

void PQCLEAN_SNTRUP761_CLEAN_crypto_sort_uint32(void *array, long long n) {
    uint32_t *x = array;
    long long j;
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
    PQCLEAN_SNTRUP761_CLEAN_crypto_sort_int32((int32_t *)array, n);
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
}
