#include "ntt.h"
#include "poly.h"
#include "polyvec.h"

#include <stdint.h>

/*************************************************
* Name:        polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_compress(uint8_t *r, polyvec *a) {
    PQCLEAN_KYBER76890S_AVX2_polyvec_csubq(a);

    uint16_t t[4];
    for (size_t i = 0; i < KYBER_K; i++) {
        for (size_t j = 0; j < KYBER_N / 4; j++) {
            for (size_t k = 0; k < 4; k++) {
                t[k] = ((((uint32_t)a->vec[i].coeffs[4 * j + k] << 10) + KYBER_Q / 2) / KYBER_Q) & 0x3ff;
            }

            r[5 * j + 0] = (uint8_t)t[0];
            r[5 * j + 1] = (uint8_t)((t[0] >>  8) | ((t[1] & 0x3f) << 2));
            r[5 * j + 2] = (uint8_t)((t[1] >>  6) | ((t[2] & 0x0f) << 4));
            r[5 * j + 3] = (uint8_t)((t[2] >>  4) | ((t[3] & 0x03) << 6));
            r[5 * j + 4] = (uint8_t)((t[3] >>  2));
        }
        r += 320;
    }
}

/*************************************************
* Name:        polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - uint8_t *a: pointer to input byte array
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_decompress(polyvec *r, const uint8_t *a) {
    for (size_t i = 0; i < KYBER_K; i++) {
        for (size_t j = 0; j < KYBER_N / 4; j++) {
            r->vec[i].coeffs[4 * j + 0] = (int16_t)( (((a[5 * j + 0]       | (((uint32_t)a[5 * j + 1] & 0x03) << 8)) * KYBER_Q) + 512) >> 10);
            r->vec[i].coeffs[4 * j + 1] = (int16_t)(((((a[5 * j + 1] >> 2) | (((uint32_t)a[5 * j + 2] & 0x0f) << 6)) * KYBER_Q) + 512) >> 10);
            r->vec[i].coeffs[4 * j + 2] = (int16_t)(((((a[5 * j + 2] >> 4) | (((uint32_t)a[5 * j + 3] & 0x3f) << 4)) * KYBER_Q) + 512) >> 10);
            r->vec[i].coeffs[4 * j + 3] = (int16_t)(((((a[5 * j + 3] >> 6) | (((uint32_t)a[5 * j + 4] & 0xff) << 2)) * KYBER_Q) + 512) >> 10);
        }
        a += 320;
    }
}

/*************************************************
* Name:        polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_tobytes(uint8_t *r, polyvec *a) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_tobytes(r + i * KYBER_POLYBYTES, &a->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of polyvec_tobytes
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_frombytes(polyvec *r, const uint8_t *a) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_frombytes(&r->vec[i], a + i * KYBER_POLYBYTES);
    }
}

/*************************************************
* Name:        polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_ntt(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_ntt(&r->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_invntt
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_invntt(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_invntt(&r->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_pointwise_acc
*
* Description: Pointwise multiply elements of a and b and accumulate into r
*
* Arguments: - poly *r:          pointer to output polynomial
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_pointwise_acc(poly *r, const polyvec *a, const polyvec *b) {
    PQCLEAN_KYBER76890S_AVX2_basemul_acc_avx(r->coeffs,
            a->vec->coeffs,
            b->vec->coeffs,
            PQCLEAN_KYBER76890S_AVX2_zetas_exp + 152);
    PQCLEAN_KYBER76890S_AVX2_basemul_acc_avx(r->coeffs + 64,
            a->vec->coeffs + 64,
            b->vec->coeffs + 64,
            PQCLEAN_KYBER76890S_AVX2_zetas_exp + 184);
    PQCLEAN_KYBER76890S_AVX2_basemul_acc_avx(r->coeffs + 128,
            a->vec->coeffs + 128,
            b->vec->coeffs + 128,
            PQCLEAN_KYBER76890S_AVX2_zetas_exp + 348);
    PQCLEAN_KYBER76890S_AVX2_basemul_acc_avx(r->coeffs + 192,
            a->vec->coeffs + 192,
            b->vec->coeffs + 192,
            PQCLEAN_KYBER76890S_AVX2_zetas_exp + 380);
}

// FIXME
void PQCLEAN_KYBER76890S_AVX2_polyvec_reduce(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_reduce(&r->vec[i]);
    }
}

// FIXME
void PQCLEAN_KYBER76890S_AVX2_polyvec_csubq(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_csubq(&r->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: - polyvec *r:       pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_polyvec_add(polyvec *r, const polyvec *a, const polyvec *b) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
