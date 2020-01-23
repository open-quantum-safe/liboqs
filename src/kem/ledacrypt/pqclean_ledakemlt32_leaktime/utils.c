#include "gf2x_arith.h"
#include "utils.h"

/* compares DIGIT sequences, returns 0 if they are equal */
int PQCLEAN_LEDAKEMLT32_LEAKTIME_gf2x_verify(const DIGIT *a, const DIGIT *b, size_t len) {
    DIGIT x = 0;
    for (size_t i = 0; i < len; i++) {
        x |= a[i] ^ b[i];
    }
    x = (~x) + 1;
    x >>= (DIGIT_SIZE_b - 1);
    return (int)x;
}

/* conditionally move a into r if cond */
void PQCLEAN_LEDAKEMLT32_LEAKTIME_cmov(uint8_t *r, const uint8_t *a, size_t len, int cond) {
    uint8_t mask = (uint8_t)(-cond);
    for (size_t i = 0; i < len; i++) {
        r[i] ^= mask & (r[i] ^ a[i]);
    }
}
