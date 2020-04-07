#include "cbd.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "reduce.h"
#include "symmetric.h"

#include <immintrin.h>
#include <stdint.h>

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const poly *a:    pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_compress(uint8_t *r, poly *a) {
    uint8_t t[8];
    size_t i, j, k = 0;

    PQCLEAN_KYBER51290S_AVX2_poly_csubq(a);

    for (i = 0; i < KYBER_N; i += 8) {
        for (j = 0; j < 8; j++) {
            t[j] = (uint8_t)(((((uint16_t)a->coeffs[i + j] << 3) + KYBER_Q / 2) / KYBER_Q) & 7);
        }

        r[k]     = (uint8_t)( t[0]       | (t[1] << 3) | (t[2] << 6));
        r[k + 1] = (uint8_t)((t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7));
        r[k + 2] = (uint8_t)((t[5] >> 1) | (t[6] << 2) | (t[7] << 5));
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
*              - const uint8_t *a: pointer to input byte array
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_decompress(poly *r, const uint8_t *a) {
    size_t i;
    for (i = 0; i < KYBER_N; i += 8) {
        r->coeffs[i + 0] = (int16_t)((((a[0] & 7) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 1] = (int16_t)(((((a[0] >> 3) & 7) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 2] = (int16_t)(((((a[0] >> 6) | ((a[1] << 2) & 4)) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 3] = (int16_t)(((((a[1] >> 1) & 7) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 4] = (int16_t)(((((a[1] >> 4) & 7) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 5] = (int16_t)(((((a[1] >> 7) | ((a[2] << 1) & 6)) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 6] = (int16_t)(((((a[2] >> 2) & 7) * KYBER_Q) + 4) >> 3);
        r->coeffs[i + 7] = (int16_t)(((((a[2] >> 5)) * KYBER_Q) + 4) >> 3);
        a += 3;
    }
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const poly *a:    pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_tobytes(uint8_t *r, poly *a) {
    PQCLEAN_KYBER51290S_AVX2_ntttobytes_avx(r, a->coeffs);
    PQCLEAN_KYBER51290S_AVX2_ntttobytes_avx(r + 192, a->coeffs + 128);
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r:                pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_frombytes(poly *r, const uint8_t *a) {
    PQCLEAN_KYBER51290S_AVX2_nttfrombytes_avx(r->coeffs, a);
    PQCLEAN_KYBER51290S_AVX2_nttfrombytes_avx(r->coeffs + 128, a + 192);
}

/*************************************************
* Name:        poly_getnoise
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA
*
* Arguments:   - poly *r:                   pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*              - uint8_t nonce:       one-byte input nonce
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_getnoise(poly *r, const uint8_t *seed, uint8_t nonce) {
    uint8_t buf[KYBER_ETA * KYBER_N / 4];

    prf(buf, KYBER_ETA * KYBER_N / 4, seed, nonce);
    PQCLEAN_KYBER51290S_AVX2_cbd(r, buf);
}


/*************************************************
* Name:        poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_ntt(poly *r) {
    PQCLEAN_KYBER51290S_AVX2_ntt_level0_avx(r->coeffs, PQCLEAN_KYBER51290S_AVX2_zetas_exp);
    PQCLEAN_KYBER51290S_AVX2_ntt_level0_avx(r->coeffs + 64, PQCLEAN_KYBER51290S_AVX2_zetas_exp);
    PQCLEAN_KYBER51290S_AVX2_ntt_levels1t6_avx(r->coeffs, PQCLEAN_KYBER51290S_AVX2_zetas_exp + 4);
    PQCLEAN_KYBER51290S_AVX2_ntt_levels1t6_avx(r->coeffs + 128, PQCLEAN_KYBER51290S_AVX2_zetas_exp + 200);
}

/*************************************************
* Name:        poly_invntt
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_invntt(poly *r) {
    PQCLEAN_KYBER51290S_AVX2_invntt_levels0t5_avx(r->coeffs, PQCLEAN_KYBER51290S_AVX2_zetas_inv_exp);
    PQCLEAN_KYBER51290S_AVX2_invntt_levels0t5_avx(r->coeffs + 128, PQCLEAN_KYBER51290S_AVX2_zetas_inv_exp + 196);
    PQCLEAN_KYBER51290S_AVX2_invntt_level6_avx(r->coeffs, PQCLEAN_KYBER51290S_AVX2_zetas_inv_exp + 392);
}

// FIXME
void PQCLEAN_KYBER51290S_AVX2_poly_nttunpack(poly *r) {
    PQCLEAN_KYBER51290S_AVX2_nttunpack_avx(r->coeffs);
    PQCLEAN_KYBER51290S_AVX2_nttunpack_avx(r->coeffs + 128);
}

//XXX Add comment
void PQCLEAN_KYBER51290S_AVX2_poly_basemul(poly *r, const poly *a, const poly *b) {
    PQCLEAN_KYBER51290S_AVX2_basemul_avx(r->coeffs,
                                         a->coeffs,
                                         b->coeffs,
                                         PQCLEAN_KYBER51290S_AVX2_zetas_exp + 152);
    PQCLEAN_KYBER51290S_AVX2_basemul_avx(r->coeffs + 64,
                                         a->coeffs + 64,
                                         b->coeffs + 64,
                                         PQCLEAN_KYBER51290S_AVX2_zetas_exp + 184);
    PQCLEAN_KYBER51290S_AVX2_basemul_avx(r->coeffs + 128,
                                         a->coeffs + 128,
                                         b->coeffs + 128,
                                         PQCLEAN_KYBER51290S_AVX2_zetas_exp + 348);
    PQCLEAN_KYBER51290S_AVX2_basemul_avx(r->coeffs + 192,
                                         a->coeffs + 192,
                                         b->coeffs + 192,
                                         PQCLEAN_KYBER51290S_AVX2_zetas_exp + 380);
}

// FIXME
void PQCLEAN_KYBER51290S_AVX2_poly_frommont(poly *r) {
    PQCLEAN_KYBER51290S_AVX2_frommont_avx(r->coeffs);
    PQCLEAN_KYBER51290S_AVX2_frommont_avx(r->coeffs + 128);
}

// FIXME
void PQCLEAN_KYBER51290S_AVX2_poly_reduce(poly *r) {
    PQCLEAN_KYBER51290S_AVX2_reduce_avx(r->coeffs);
    PQCLEAN_KYBER51290S_AVX2_reduce_avx(r->coeffs + 128);
}

// FIXME
void PQCLEAN_KYBER51290S_AVX2_poly_csubq(poly *r) {
    PQCLEAN_KYBER51290S_AVX2_csubq_avx(r->coeffs);
    PQCLEAN_KYBER51290S_AVX2_csubq_avx(r->coeffs + 128);
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
void PQCLEAN_KYBER51290S_AVX2_poly_add(poly *r, const poly *a, const poly *b) {
    __m256i vec0, vec1;

    for (size_t i = 0; i < KYBER_N; i += 16) {
        vec0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
        vec1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
        vec0 = _mm256_add_epi16(vec0, vec1);
        _mm256_store_si256((__m256i *)&r->coeffs[i], vec0);
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
void PQCLEAN_KYBER51290S_AVX2_poly_sub(poly *r, const poly *a, const poly *b) {
    __m256i vec0, vec1;

    for (size_t i = 0; i < KYBER_N; i += 16) {
        vec0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
        vec1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
        vec0 = _mm256_sub_epi16(vec0, vec1);
        _mm256_store_si256((__m256i *)&r->coeffs[i], vec0);
    }
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r:                  pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_frommsg(poly *r, const uint8_t msg[KYBER_SYMBYTES]) {
    __m128i tmp;
    __m256i a[4], d0, d1, d2, d3;
    const __m256i shift = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    const __m256i zeros = _mm256_setzero_si256();
    const __m256i ones = _mm256_set1_epi32(1);
    const __m256i hqs = _mm256_set1_epi32((KYBER_Q + 1) / 2);

    tmp = _mm_loadu_si128((__m128i *)msg);
    for (size_t i = 0; i < 4; i++) {
        a[i] = _mm256_broadcastd_epi32(tmp);
        tmp = _mm_srli_si128(tmp, 4);
    }

    for (size_t i = 0; i < 4; i++) {
        d0 = _mm256_srlv_epi32(a[i], shift);
        d1 = _mm256_srli_epi32(d0, 8);
        d2 = _mm256_srli_epi32(d0, 16);
        d3 = _mm256_srli_epi32(d0, 24);

        d0 = _mm256_and_si256(d0, ones);
        d1 = _mm256_and_si256(d1, ones);
        d2 = _mm256_and_si256(d2, ones);
        d3 = _mm256_and_si256(d3, ones);

        d0 = _mm256_sub_epi32(zeros, d0);
        d1 = _mm256_sub_epi32(zeros, d1);
        d2 = _mm256_sub_epi32(zeros, d2);
        d3 = _mm256_sub_epi32(zeros, d3);

        d0 = _mm256_and_si256(hqs, d0);
        d1 = _mm256_and_si256(hqs, d1);
        d2 = _mm256_and_si256(hqs, d2);
        d3 = _mm256_and_si256(hqs, d3);

        d0 = _mm256_packus_epi32(d0, d1);
        d2 = _mm256_packus_epi32(d2, d3);
        d0 = _mm256_permute4x64_epi64(d0, 0xD8);
        d2 = _mm256_permute4x64_epi64(d2, 0xD8);
        _mm256_store_si256((__m256i *)&r->coeffs[32 * i + 0], d0);
        _mm256_store_si256((__m256i *)&r->coeffs[32 * i + 16], d2);
    }

    tmp = _mm_loadu_si128((__m128i *)&msg[16]);
    for (size_t i = 0; i < 4; i++) {
        a[i] = _mm256_broadcastd_epi32(tmp);
        tmp = _mm_srli_si128(tmp, 4);
    }

    for (size_t i = 0; i < 4; i++) {
        d0 = _mm256_srlv_epi32(a[i], shift);
        d1 = _mm256_srli_epi32(d0, 8);
        d2 = _mm256_srli_epi32(d0, 16);
        d3 = _mm256_srli_epi32(d0, 24);

        d0 = _mm256_and_si256(d0, ones);
        d1 = _mm256_and_si256(d1, ones);
        d2 = _mm256_and_si256(d2, ones);
        d3 = _mm256_and_si256(d3, ones);

        d0 = _mm256_sub_epi32(zeros, d0);
        d1 = _mm256_sub_epi32(zeros, d1);
        d2 = _mm256_sub_epi32(zeros, d2);
        d3 = _mm256_sub_epi32(zeros, d3);

        d0 = _mm256_and_si256(hqs, d0);
        d1 = _mm256_and_si256(hqs, d1);
        d2 = _mm256_and_si256(hqs, d2);
        d3 = _mm256_and_si256(hqs, d3);

        d0 = _mm256_packus_epi32(d0, d1);
        d2 = _mm256_packus_epi32(d2, d3);
        d0 = _mm256_permute4x64_epi64(d0, 0xD8);
        d2 = _mm256_permute4x64_epi64(d2, 0xD8);
        _mm256_store_si256((__m256i *)&r->coeffs[128 + 32 * i + 0], d0);
        _mm256_store_si256((__m256i *)&r->coeffs[128 + 32 * i + 16], d2);
    }
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a:      pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_AVX2_poly_tomsg(uint8_t msg[KYBER_SYMBYTES], poly *a) {
    uint32_t small;
    __m256i vec, tmp;
    const __m256i hqs = _mm256_set1_epi16((KYBER_Q - 1) / 2);
    const __m256i hhqs = _mm256_set1_epi16((KYBER_Q - 5) / 4);

    for (size_t i = 0; i < KYBER_N / 16; i++) {
        vec = _mm256_load_si256((__m256i *)&a->coeffs[16 * i]);
        vec = _mm256_sub_epi16(hqs, vec);
        tmp = _mm256_srai_epi16(vec, 15);
        vec = _mm256_xor_si256(vec, tmp);
        vec = _mm256_sub_epi16(hhqs, vec);
        small = (uint32_t)_mm256_movemask_epi8(vec);
        small = _pext_u32(small, 0xAAAAAAAA);
        small = ~small;
        msg[2 * i + 0] = (uint8_t)small;
        msg[2 * i + 1] = (uint8_t)(small >> 8);
    }
}
