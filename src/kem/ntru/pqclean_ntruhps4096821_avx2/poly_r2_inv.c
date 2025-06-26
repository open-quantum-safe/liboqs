#include "poly.h"
#include "poly_r2_inv.h"

// TODO this costs 1764 cycles.. (implementing as S3_to_bytes results in 2108)
// This can be implemented nicely in assembly using pdep / pext functions
void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a) {
    int i, j, k;
    for (i = 0; i < 16; i++) {
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

void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in) {
    int i, j, k;
    for (i = 0; i < 16; i++) {
        for (k = 0; k < 8; k++) {
            for (j = 0; j < 8; j++) {
                if ((i * 8 + k) * 8 + j < NTRU_N) {
                    a->coeffs[(i * 8 + k) * 8 + j] = (in[i * 8 + k] >> j) & 1;
                }
            }
        }
    }
}


static void _poly_R2_mul(unsigned char r[128], unsigned char a[128], unsigned char b[128]) {
    // TODO: Try to avoid copying input
    int i;
    union {
        unsigned char b[128];
        __m256i b_x32[4];
    } rt;

    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul(rt.b, a, b);
    for (i = 0; i < 128; i++) {
        r[i] = rt.b[i];
    }
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_inv(poly *r, const poly *a) {
    union {
        unsigned char s[128];
        __m256i s_x32[4];
    } squares[13];
#define s(x) squares[(x)].s

    // This relies on the following addition chain:
    // 1, 2, 3, 6, 12, 24, 48, 51, 102, 204, 408, 816, 819

    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_tobytes(s(0), a); // TODO alignment

    PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(s(1), s(0));
    _poly_R2_mul(s(1), s(1), s(0));
    PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(s(2), s(1));
    _poly_R2_mul(s(2), s(2), s(0));
    PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(s(3), s(2));
    _poly_R2_mul(s(3), s(3), s(2));
    PQCLEAN_NTRUHPS4096821_AVX2_square_6_821(s(4), s(3));
    _poly_R2_mul(s(4), s(4), s(3));
    PQCLEAN_NTRUHPS4096821_AVX2_square_12_821(s(5), s(4));
    _poly_R2_mul(s(5), s(5), s(4));
    PQCLEAN_NTRUHPS4096821_AVX2_square_24_821(s(6), s(5));
    _poly_R2_mul(s(6), s(6), s(5));
    PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(s(7), s(6));
    _poly_R2_mul(s(7), s(7), s(2));
    PQCLEAN_NTRUHPS4096821_AVX2_square_51_821(s(8), s(7));
    _poly_R2_mul(s(8), s(8), s(7));
    PQCLEAN_NTRUHPS4096821_AVX2_square_102_821(s(9), s(8));
    _poly_R2_mul(s(9), s(9), s(8));
    PQCLEAN_NTRUHPS4096821_AVX2_square_204_821(s(10), s(9));
    _poly_R2_mul(s(10), s(10), s(9));
    PQCLEAN_NTRUHPS4096821_AVX2_square_408_821(s(11), s(10));
    _poly_R2_mul(s(11), s(11), s(10));
    PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(s(12), s(11));
    _poly_R2_mul(s(12), s(12), s(2));
    PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(s(0), s(12));


    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_frombytes(r, s(0));
#undef s
}
