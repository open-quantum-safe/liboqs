#include "poly.h"
#include "cbd.h"
#include "fips202.h"
#include "ntt.h"
#include "polyvec.h"
#include "reduce.h"
#include <stdio.h>

/*************************************************
 * Name:        poly_compress
 *
 * Description: Compression and subsequent serialization of a polynomial
 *
 * Arguments:   - unsigned char *r: pointer to output byte array
 *              - const poly *a:    pointer to input polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_compress(unsigned char *r, const poly *a) {
    uint32_t t[8];
    unsigned int i, j, k = 0;

    for (i = 0; i < KYBER_N; i += 8) {
        for (j = 0; j < 8; j++) {
            t[j] = (((PQCLEAN_KYBER768_CLEAN_freeze(a->coeffs[i + j]) << 3) + KYBER_Q / 2) / KYBER_Q) & 7;
        }

        r[k]     = (unsigned char)( t[0]       | (t[1] << 3) | (t[2] << 6));
        r[k + 1] = (unsigned char)((t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7));
        r[k + 2] = (unsigned char)((t[5] >> 1) | (t[6] << 2) | (t[7] << 5));
        k += 3;
    }
}

/*************************************************
 * Name:        poly_decompress
 *
 * Description: De-serialization and subsequent decompression of a polynomial;
 *              approximate inverse of poly_compress
 *
 * Arguments:   - poly *r:                pointer to output polynomial
 *              - const unsigned char *a: pointer to input byte array
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_decompress(poly *r, const unsigned char *a) {
    unsigned int i;
    for (i = 0; i < KYBER_N; i += 8) {
        r->coeffs[i + 0] =  (((a[0] & 7) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 1] = ((((a[0] >> 3) & 7) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 2] = ((((a[0] >> 6) | ((a[1] << 2) & 4)) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 3] = ((((a[1] >> 1) & 7) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 4] = ((((a[1] >> 4) & 7) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 5] = ((((a[1] >> 7) | ((a[2] << 1) & 6)) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 6] = ((((a[2] >> 2) & 7) * KYBER_Q) + 4) >> 3;
        r->coeffs[i + 7] = ((((a[2] >> 5)) * KYBER_Q) + 4) >> 3;
        a += 3;
    }
}

/*************************************************
 * Name:        poly_tobytes
 *
 * Description: Serialization of a polynomial
 *
 * Arguments:   - unsigned char *r: pointer to output byte array
 *              - const poly *a:    pointer to input polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_tobytes(unsigned char *r, const poly *a) {
    int i, j;
    uint16_t t[8];

    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            t[j] = PQCLEAN_KYBER768_CLEAN_freeze(a->coeffs[8 * i + j]);
        }

        r[13 * i +  0] = (unsigned char)( t[0] & 0xff);
        r[13 * i +  1] = (unsigned char)((t[0] >>  8) | ((t[1] & 0x07) << 5));
        r[13 * i +  2] = (unsigned char)((t[1] >>  3) & 0xff);
        r[13 * i +  3] = (unsigned char)((t[1] >> 11) | ((t[2] & 0x3f) << 2));
        r[13 * i +  4] = (unsigned char)((t[2] >>  6) | ((t[3] & 0x01) << 7));
        r[13 * i +  5] = (unsigned char)((t[3] >>  1) & 0xff);
        r[13 * i +  6] = (unsigned char)((t[3] >>  9) | ((t[4] & 0x0f) << 4));
        r[13 * i +  7] = (unsigned char)((t[4] >>  4) & 0xff);
        r[13 * i +  8] = (unsigned char)((t[4] >> 12) | ((t[5] & 0x7f) << 1));
        r[13 * i +  9] = (unsigned char)((t[5] >>  7) | ((t[6] & 0x03) << 6));
        r[13 * i + 10] = (unsigned char)((t[6] >>  2) & 0xff);
        r[13 * i + 11] = (unsigned char)((t[6] >> 10) | ((t[7] & 0x1f) << 3));
        r[13 * i + 12] = (unsigned char)((t[7] >>  5));
    }
}

/*************************************************
 * Name:        poly_frombytes
 *
 * Description: De-serialization of a polynomial;
 *              inverse of poly_tobytes
 *
 * Arguments:   - poly *r:                pointer to output polynomial
 *              - const unsigned char *a: pointer to input byte array
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_frombytes(poly *r, const unsigned char *a) {
    int i;
    for (i = 0; i < KYBER_N / 8; i++) {
        r->coeffs[8 * i + 0]  = a[13 * i + 0]        | (((uint16_t)a[13 * i +  1] & 0x1f) << 8);
        r->coeffs[8 * i + 1] = (a[13 * i +  1] >> 5) | (((uint16_t)a[13 * i +  2]) << 3) | (((uint16_t)a[13 * i +  3] & 0x03) << 11);
        r->coeffs[8 * i + 2] = (a[13 * i +  3] >> 2) | (((uint16_t)a[13 * i +  4] & 0x7f) << 6);
        r->coeffs[8 * i + 3] = (a[13 * i +  4] >> 7) | (((uint16_t)a[13 * i +  5]) << 1) | (((uint16_t)a[13 * i +  6] & 0x0f) << 9);
        r->coeffs[8 * i + 4] = (a[13 * i +  6] >> 4) | (((uint16_t)a[13 * i +  7]) << 4) | (((uint16_t)a[13 * i +  8] & 0x01) << 12);
        r->coeffs[8 * i + 5] = (a[13 * i +  8] >> 1) | (((uint16_t)a[13 * i +  9] & 0x3f) << 7);
        r->coeffs[8 * i + 6] = (a[13 * i +  9] >> 6) | (((uint16_t)a[13 * i + 10]) << 2) | (((uint16_t)a[13 * i + 11] & 0x07) << 10);
        r->coeffs[8 * i + 7] = (a[13 * i + 11] >> 3) | (((uint16_t)a[13 * i + 12]) << 5);
    }
}

/*************************************************
 * Name:        poly_getnoise
 *
 * Description: Sample a polynomial deterministically from a seed and a nonce,
 *              with output polynomial close to centered binomial distribution
 *              with parameter KYBER_ETA
 *
 * Arguments:   - poly *r:                   pointer to output polynomial
 *              - const unsigned char *seed: pointer to input seed
 *              - unsigned char nonce:       one-byte input nonce
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_getnoise(poly *r, const unsigned char *seed, unsigned char nonce) {
    unsigned char buf[KYBER_ETA * KYBER_N / 4];
    unsigned char extseed[KYBER_SYMBYTES + 1];
    int i;

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        extseed[i] = seed[i];
    }
    extseed[KYBER_SYMBYTES] = nonce;

    shake256(buf, KYBER_ETA * KYBER_N / 4, extseed, KYBER_SYMBYTES + 1);

    PQCLEAN_KYBER768_CLEAN_cbd(r, buf);
}

/*************************************************
 * Name:        poly_ntt
 *
 * Description: Computes negacyclic number-theoretic transform (NTT) of
 *              a polynomial in place;
 *              inputs assumed to be in normal order, output in bitreversed
 *order
 *
 * Arguments:   - uint16_t *r: pointer to in/output polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_ntt(poly *r) {
    PQCLEAN_KYBER768_CLEAN_ntt(r->coeffs);
}

/*************************************************
 * Name:        poly_invntt
 *
 * Description: Computes inverse of negacyclic number-theoretic transform (NTT)
 *of a polynomial in place; inputs assumed to be in bitreversed order, output in
 *normal order
 *
 * Arguments:   - uint16_t *a: pointer to in/output polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_invntt(poly *r) {
    PQCLEAN_KYBER768_CLEAN_invntt(r->coeffs);
}

/*************************************************
 * Name:        poly_add
 *
 * Description: Add two polynomials
 *
 * Arguments: - poly *r:       pointer to output polynomial
 *            - const poly *a: pointer to first input polynomial
 *            - const poly *b: pointer to second input polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_add(poly *r, const poly *a, const poly *b) {
    int i;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = PQCLEAN_KYBER768_CLEAN_barrett_reduce(a->coeffs[i] + b->coeffs[i]);
    }
}

/*************************************************
 * Name:        poly_sub
 *
 * Description: Subtract two polynomials
 *
 * Arguments: - poly *r:       pointer to output polynomial
 *            - const poly *a: pointer to first input polynomial
 *            - const poly *b: pointer to second input polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_sub(poly *r, const poly *a, const poly *b) {
    int i;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = PQCLEAN_KYBER768_CLEAN_barrett_reduce(a->coeffs[i] + 3 * KYBER_Q - b->coeffs[i]);
    }
}

/*************************************************
 * Name:        poly_frommsg
 *
 * Description: Convert 32-byte message to polynomial
 *
 * Arguments:   - poly *r:                  pointer to output polynomial
 *              - const unsigned char *msg: pointer to input message
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_frommsg(poly *r, const unsigned char msg[KYBER_SYMBYTES]) {
    uint16_t i, j, mask;

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        for (j = 0; j < 8; j++) {
            mask = -((msg[i] >> j) & 1);
            r->coeffs[8 * i + j] = mask & ((KYBER_Q + 1) / 2);
        }
    }
}

/*************************************************
 * Name:        poly_tomsg
 *
 * Description: Convert polynomial to 32-byte message
 *
 * Arguments:   - unsigned char *msg: pointer to output message
 *              - const poly *a:      pointer to input polynomial
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_poly_tomsg(unsigned char msg[KYBER_SYMBYTES], const poly *a) {
    uint16_t t;
    int i, j;

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        msg[i] = 0;
        for (j = 0; j < 8; j++) {
            t = (((PQCLEAN_KYBER768_CLEAN_freeze(a->coeffs[8 * i + j]) << 1) + KYBER_Q / 2) / KYBER_Q) & 1;
            msg[i] |= t << j;
        }
    }
}
