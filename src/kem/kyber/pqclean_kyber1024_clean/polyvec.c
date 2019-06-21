#include "polyvec.h"

#include "poly.h"

#include <stdint.h>

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - unsigned char *r: pointer to output byte array (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_compress(unsigned char *r, polyvec *a) {
    int i, j, k;

    PQCLEAN_KYBER1024_CLEAN_polyvec_csubq(a);

    uint16_t t[8];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 8; j++) {
            for (k = 0; k < 8; k++) {
                t[k] = ((((uint32_t)a->vec[i].coeffs[8 * j + k] << 11) + KYBER_Q / 2) / KYBER_Q) & 0x7ff;
            }

            r[11 * j + 0] =  t[0] & 0xff;
            r[11 * j + 1] = ((t[0] >>  8) | ((t[1] & 0x1f) << 3)) & 0xff;
            r[11 * j + 2] = ((t[1] >>  5) | ((t[2] & 0x03) << 6)) & 0xff;
            r[11 * j + 3] =  (t[2] >>  2) & 0xff;
            r[11 * j + 4] = ((t[2] >> 10) | ((t[3] & 0x7f) << 1)) & 0xff;
            r[11 * j + 5] = ((t[3] >>  7) | ((t[4] & 0x0f) << 4)) & 0xff;
            r[11 * j + 6] = ((t[4] >>  4) | ((t[5] & 0x01) << 7)) & 0xff;
            r[11 * j + 7] =  (t[5] >>  1) & 0xff;
            r[11 * j + 8] = ((t[5] >>  9) | ((t[6] & 0x3f) << 2)) & 0xff;
            r[11 * j + 9] = ((t[6] >>  6) | ((t[7] & 0x07) << 5)) & 0xff;
            r[11 * j + 10] = (t[7] >>  3) & 0xff;
        }
        r += 352;
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of PQCLEAN_KYBER1024_CLEAN_polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - unsigned char *a: pointer to input byte array (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_decompress(polyvec *r, const unsigned char *a) {
    int i, j;
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 8; j++) {
            r->vec[i].coeffs[8 * j + 0] =  (((a[11 * j + 0]       | (((uint32_t)a[11 * j + 1] & 0x07) << 8)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 1] = ((((a[11 * j + 1] >> 3) | (((uint32_t)a[11 * j + 2] & 0x3f) << 5)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 2] = ((((a[11 * j + 2] >> 6) | (((uint32_t)a[11 * j + 3] & 0xff) << 2) | (((uint32_t)a[11 * j + 4] & 0x01) << 10)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 3] = ((((a[11 * j + 4] >> 1) | (((uint32_t)a[11 * j + 5] & 0x0f) << 7)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 4] = ((((a[11 * j + 5] >> 4) | (((uint32_t)a[11 * j + 6] & 0x7f) << 4)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 5] = ((((a[11 * j + 6] >> 7) | (((uint32_t)a[11 * j + 7] & 0xff) << 1) | (((uint32_t)a[11 * j + 8] & 0x03) <<  9)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 6] = ((((a[11 * j + 8] >> 2) | (((uint32_t)a[11 * j + 9] & 0x1f) << 6)) * KYBER_Q) + 1024) >> 11;
            r->vec[i].coeffs[8 * j + 7] = ((((a[11 * j + 9] >> 5) | (((uint32_t)a[11 * j + 10] & 0xff) << 3)) * KYBER_Q) + 1024) >> 11;
        }
        a += 352;
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - unsigned char *r: pointer to output byte array (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_tobytes(unsigned char *r, polyvec *a) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_tobytes(r + i * KYBER_POLYBYTES, &a->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of PQCLEAN_KYBER1024_CLEAN_polyvec_tobytes
*
* Arguments:   - unsigned char *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials (of length KYBER_POLYVECBYTES)
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_frombytes(polyvec *r, const unsigned char *a) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_frombytes(&r->vec[i], a + i * KYBER_POLYBYTES);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_ntt(polyvec *r) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_ntt(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_invntt
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_invntt(polyvec *r) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_invntt(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_pointwise_acc
*
* Description: Pointwise multiply elements of a and b and accumulate into r
*
* Arguments: - poly *r:          pointer to output polynomial
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_pointwise_acc(poly *r, const polyvec *a, const polyvec *b) {
    int i;
    poly t;

    PQCLEAN_KYBER1024_CLEAN_poly_basemul(r, &a->vec[0], &b->vec[0]);
    for (i = 1; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_basemul(&t, &a->vec[i], &b->vec[i]);
        PQCLEAN_KYBER1024_CLEAN_poly_add(r, r, &t);
    }

    PQCLEAN_KYBER1024_CLEAN_poly_reduce(r);
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_reduce(polyvec *r) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_reduce(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_csubq
*
* Description: Applies conditional subtraction of q to each coefficient
*              of each element of a vector of polynomials
*              for details of conditional subtraction of q see comments in reduce.c
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_csubq(polyvec *r) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_csubq(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: - polyvec *r:       pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_KYBER1024_CLEAN_polyvec_add(polyvec *r, const polyvec *a, const polyvec *b) {
    int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER1024_CLEAN_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
