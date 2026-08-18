#include <immintrin.h>

#include "poly_r2_inv.h"
#include "poly.h"

// Using pdep/pext for these two functions is faster but not a lot since they work on uint64_t which means
// we can only do 4 coefficients at a time. Per byte (where we store 8 coefficients) we will thus need 2 pdeps/pexts
// and an additional shift. In the case of tobytes we also need a logical or.
// On AMD Ryzen pdep/pext are quite slow and the naive solution (looping through and setting each bit individually)
// is preferred.
void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a) {
    // Since pext works on a uint64_t we view the coefficient pointer as a 64-bit pointer
    // so that we can extract 4 coefficient at a time. It also makes arithmetic a little easier.
    uint64_t *coeff_pointer = (void *) a->coeffs;

    int i;
    for (i = 0; i < 63; i++) {
        out[i] = _pext_u64(coeff_pointer[2 * i], 0x1000100010001);
        out[i] |= _pext_u64(coeff_pointer[2 * i + 1], 0x1000100010001) << 4;
    }
    out[i] = _pext_u64(coeff_pointer[2 * 63], 0x1000100010001);
    out[i] |= _pext_u64(coeff_pointer[2 * 63 + 1], 0x1) << 4;
}

void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in) {
    // Since pdep results in a uint64_t we view the coefficient pointer as a 64-bit pointer
    // so that we can store 4 coefficient at a time. It also makes arithmetic a little easier.
    uint64_t *coeff_pointer = (void *) a->coeffs;

    int i;
    for (i = 0; i < 63; i++) {
        coeff_pointer[2 * i] = _pdep_u64(in[i], 0x1000100010001);
        coeff_pointer[2 * i + 1] = _pdep_u64(in[i] >> 4, 0x1000100010001);
    }
    // From the last byte we only want 5 bits (since we have 509 total, not 512).
    coeff_pointer[2 * 63] = _pdep_u64(in[i], 0x1000100010001);
    coeff_pointer[2 * 63 + 1] = _pdep_u64(in[i] >> 4, 0x1);
}

void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_inv(poly *r, const poly *a) {
    union {
        unsigned char s[64];
        __m256i s_x32[2];
    } squares[13];
#define s(x) squares[(x)].s

    // This relies on the following addition chain:
    // 1, 2, 3, 6, 12, 15, 30, 60, 63, 126, 252, 504, 507

    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_tobytes(s(0), a); // TODO alignment

    PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(s(1), s(0));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(1), s(1), s(0));
    PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(s(2), s(1));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(2), s(2), s(0));
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(s(3), s(2));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(3), s(3), s(2));
    PQCLEAN_NTRUHPS2048509_AVX2_square_6_509(s(4), s(3));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(4), s(4), s(3));
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(s(5), s(4));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(5), s(5), s(2));
    PQCLEAN_NTRUHPS2048509_AVX2_square_15_509(s(6), s(5));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(6), s(6), s(5));
    PQCLEAN_NTRUHPS2048509_AVX2_square_30_509(s(7), s(6));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(7), s(7), s(6));
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(s(8), s(7));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(8), s(8), s(2));
    PQCLEAN_NTRUHPS2048509_AVX2_square_63_509(s(9), s(8));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(9), s(9), s(8));
    PQCLEAN_NTRUHPS2048509_AVX2_square_126_509(s(10), s(9));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(10), s(10), s(9));
    PQCLEAN_NTRUHPS2048509_AVX2_square_252_509(s(11), s(10));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(11), s(11), s(10));
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(s(12), s(11));
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(s(12), s(12), s(2));
    PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(s(0), s(12));

    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_frombytes(r, s(0));
#undef s
}
