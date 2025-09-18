#include "poly_r2_inv.h"
#include "poly.h"

// TODO this costs 1764 cycles.. (implementing as S3_to_bytes results in 2108)
// This can be implemented nicely in assembly using pdep / pext functions
void PQCLEAN_NTRUHRSS701_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a) {
    int i, j, k;
    for (i = 0; i < 12; i++) {
        for (k = 0; k < 8; k++) {
            out[i * 8 + k] = 0;
            for (j = 0; j < 8; j++) {
                if ((i * 8 + k) * 8 + j < NTRU_N) {
                    out[i * 8 + k] |= (a->coeffs[(i * 8 + k) * 8 + j] & 1) << j;
                }
            }
        }
    }
}

void PQCLEAN_NTRUHRSS701_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in) {
    int i, j, k;
    for (i = 0; i < 12; i++) {
        for (k = 0; k < 8; k++) {
            for (j = 0; j < 8; j++) {
                if ((i * 8 + k) * 8 + j < NTRU_N) {
                    a->coeffs[(i * 8 + k) * 8 + j] = (in[i * 8 + k] >> j) & 1;
                }
            }
        }
    }
}

void PQCLEAN_NTRUHRSS701_AVX2_poly_R2_inv(poly *r, const poly *a) {
    union {
        unsigned char s[96];
        __m256i s_x32[3];
    } squares[13];
#define s(x) squares[(x)].s

    // This relies on the following addition chain:
    // 1, 2, 3, 6, 12, 15, 27, 42, 84, 168, 336, 672, 699

    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_tobytes(s(0), a); // TODO alignment

    PQCLEAN_NTRUHRSS701_AVX2_square_1_701(s(1), s(0));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(1), s(1), s(0));
    PQCLEAN_NTRUHRSS701_AVX2_square_1_701(s(2), s(1));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(2), s(2), s(0));
    PQCLEAN_NTRUHRSS701_AVX2_square_3_701(s(3), s(2));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(3), s(3), s(2));
    PQCLEAN_NTRUHRSS701_AVX2_square_6_701(s(4), s(3));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(4), s(4), s(3));
    PQCLEAN_NTRUHRSS701_AVX2_square_3_701(s(5), s(4));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(5), s(5), s(2));
    PQCLEAN_NTRUHRSS701_AVX2_square_12_701(s(6), s(5));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(6), s(6), s(4));
    PQCLEAN_NTRUHRSS701_AVX2_square_15_701(s(7), s(6));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(7), s(7), s(5));
    PQCLEAN_NTRUHRSS701_AVX2_square_42_701(s(8), s(7));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(8), s(8), s(7));
    PQCLEAN_NTRUHRSS701_AVX2_square_84_701(s(9), s(8));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(9), s(9), s(8));
    PQCLEAN_NTRUHRSS701_AVX2_square_168_701(s(10), s(9));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(10), s(10), s(9));
    PQCLEAN_NTRUHRSS701_AVX2_square_336_701(s(11), s(10));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(11), s(11), s(10));
    PQCLEAN_NTRUHRSS701_AVX2_square_27_701(s(12), s(11));
    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(s(12), s(12), s(6));
    PQCLEAN_NTRUHRSS701_AVX2_square_1_701(s(0), s(12));

    PQCLEAN_NTRUHRSS701_AVX2_poly_R2_frombytes(r, s(0));
#undef s
}
