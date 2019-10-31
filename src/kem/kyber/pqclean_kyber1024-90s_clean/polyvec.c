#include "polyvec.h"

#include "poly.h"

#include <stddef.h>
#include <stdint.h>
/*************************************************
* Name:        polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_compress(uint8_t *r, polyvec *a) {
    PQCLEAN_KYBER102490S_CLEAN_polyvec_csubq(a);

    uint16_t t[8];
    for (size_t i = 0; i < KYBER_K; i++) {
        for (size_t j = 0; j < KYBER_N / 8; j++) {
            for (size_t k = 0; k < 8; k++) {
                t[k] = ((((uint32_t)a->vec[i].coeffs[8 * j + k] << 11) + KYBER_Q / 2) / KYBER_Q) & 0x7ff;
            }

            r[11 * j + 0] = (uint8_t)t[0];
            r[11 * j + 1] = (uint8_t)((t[0] >>  8) | ((t[1] & 0x1f) << 3));
            r[11 * j + 2] = (uint8_t)((t[1] >>  5) | ((t[2] & 0x03) << 6));
            r[11 * j + 3] = (uint8_t)((t[2] >>  2));
            r[11 * j + 4] = (uint8_t)((t[2] >> 10) | ((t[3] & 0x7f) << 1));
            r[11 * j + 5] = (uint8_t)((t[3] >>  7) | ((t[4] & 0x0f) << 4));
            r[11 * j + 6] = (uint8_t)((t[4] >>  4) | ((t[5] & 0x01) << 7));
            r[11 * j + 7] = (uint8_t)((t[5] >>  1));
            r[11 * j + 8] = (uint8_t)((t[5] >>  9) | ((t[6] & 0x3f) << 2));
            r[11 * j + 9] = (uint8_t)((t[6] >>  6) | ((t[7] & 0x07) << 5));
            r[11 * j + 10] = (uint8_t)((t[7] >>  3));
        }
        r += 352;
    }
}

/*************************************************
* Name:        polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - uint8_t *a: pointer to input byte array (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_decompress(polyvec *r, const uint8_t *a) {
    for (size_t i = 0; i < KYBER_K; i++) {
        for (size_t j = 0; j < KYBER_N / 8; j++) {
            r->vec[i].coeffs[8 * j + 0] = (int16_t)( (((a[11 * j + 0]       | (((uint32_t)a[11 * j + 1] & 0x07) << 8)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 1] = (int16_t)(((((a[11 * j + 1] >> 3) | (((uint32_t)a[11 * j + 2] & 0x3f) << 5)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 2] = (int16_t)(((((a[11 * j + 2] >> 6) | (((uint32_t)a[11 * j + 3] & 0xff) << 2) | (((uint32_t)a[11 * j + 4] & 0x01) << 10)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 3] = (int16_t)(((((a[11 * j + 4] >> 1) | (((uint32_t)a[11 * j + 5] & 0x0f) << 7)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 4] = (int16_t)(((((a[11 * j + 5] >> 4) | (((uint32_t)a[11 * j + 6] & 0x7f) << 4)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 5] = (int16_t)(((((a[11 * j + 6] >> 7) | (((uint32_t)a[11 * j + 7] & 0xff) << 1) | (((uint32_t)a[11 * j + 8] & 0x03) <<  9)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 6] = (int16_t)(((((a[11 * j + 8] >> 2) | (((uint32_t)a[11 * j + 9] & 0x1f) << 6)) * KYBER_Q) + 1024) >> 11);
            r->vec[i].coeffs[8 * j + 7] = (int16_t)(((((a[11 * j + 9] >> 5) | (((uint32_t)a[11 * j + 10] & 0xff) << 3)) * KYBER_Q) + 1024) >> 11);
        }
        a += 352;
    }
}

/*************************************************
* Name:        polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_tobytes(uint8_t *r, polyvec *a) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_tobytes(r + i * KYBER_POLYBYTES, &a->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of polyvec_tobytes
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials (of length KYBER_POLYVECBYTES)
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_frombytes(polyvec *r, const uint8_t *a) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_frombytes(&r->vec[i], a + i * KYBER_POLYBYTES);
    }
}

/*************************************************
* Name:        polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_ntt(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_ntt(&r->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_invntt
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_invntt(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_invntt(&r->vec[i]);
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
void PQCLEAN_KYBER102490S_CLEAN_polyvec_pointwise_acc(poly *r, const polyvec *a, const polyvec *b) {
    poly t;

    PQCLEAN_KYBER102490S_CLEAN_poly_basemul(r, &a->vec[0], &b->vec[0]);
    for (size_t i = 1; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_basemul(&t, &a->vec[i], &b->vec[i]);
        PQCLEAN_KYBER102490S_CLEAN_poly_add(r, r, &t);
    }

    PQCLEAN_KYBER102490S_CLEAN_poly_reduce(r);
}

/*************************************************
* Name:        polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_reduce(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_reduce(&r->vec[i]);
    }
}

/*************************************************
* Name:        polyvec_csubq
*
* Description: Applies conditional subtraction of q to each coefficient
*              of each element of a vector of polynomials
*              for details of conditional subtraction of q see comments in reduce.c
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_polyvec_csubq(polyvec *r) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_csubq(&r->vec[i]);
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
void PQCLEAN_KYBER102490S_CLEAN_polyvec_add(polyvec *r, const polyvec *a, const polyvec *b) {
    for (size_t i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER102490S_CLEAN_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
