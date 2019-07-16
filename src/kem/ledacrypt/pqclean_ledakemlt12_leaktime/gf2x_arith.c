#include "gf2x_arith.h"

#include <string.h>  // memset(...)

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_add(DIGIT Res[], const DIGIT A[], const DIGIT B[], int nr) {
    for (int i = 0; i < nr; i++) {
        Res[i] = A[i] ^ B[i];
    }
}

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_right_bit_shift_n(int length, DIGIT in[], unsigned int amount) {
    if ( amount == 0 ) {
        return;
    }
    unsigned int j;
    DIGIT mask;
    mask = ((DIGIT)0x01 << amount) - 1;
    for (j = length - 1; j > 0 ; j--) {
        in[j] >>= amount;
        in[j] |=  (in[j - 1] & mask) << (DIGIT_SIZE_b - amount);
    }
    in[j] >>= amount;
}

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_left_bit_shift_n(int length, DIGIT in[], unsigned int amount) {
    if ( amount == 0 ) {
        return;
    }
    int j;
    DIGIT mask;
    mask = ~(((DIGIT)0x01 << (DIGIT_SIZE_b - amount)) - 1);
    for (j = 0 ; j < length - 1 ; j++) {
        in[j] <<= amount;
        in[j] |=  (in[j + 1] & mask) >> (DIGIT_SIZE_b - amount);
    }
    in[j] <<= amount;
}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mul_comb(int nr, DIGIT Res[],
        int na, const DIGIT A[],
        int nb, const DIGIT B[]) {
    int i, j, k;
    DIGIT u, h;

    memset(Res, 0x00, nr * sizeof(DIGIT));

    for (k = DIGIT_SIZE_b - 1; k > 0; k--) {
        for (i = na - 1; i >= 0; i--) {
            if ( A[i] & (((DIGIT)0x1) << k) ) {
                for (j = nb - 1; j >= 0; j--) {
                    Res[i + j + 1] ^= B[j];
                }
            }
        }

        u = Res[na + nb - 1];
        Res[na + nb - 1] = u << 0x1;
        for (j = 1; j < na + nb; ++j) {
            h = u >> (DIGIT_SIZE_b - 1);
            u = Res[na + nb - 1 - j];
            Res[na + nb - 1 - j] = h ^ (u << 0x1);
        }
    }
    for (i = na - 1; i >= 0; i--) {
        if ( A[i] & ((DIGIT)0x1) ) {
            for (j = nb - 1; j >= 0; j--) {
                Res[i + j + 1] ^= B[j];
            }
        }
    }
}
