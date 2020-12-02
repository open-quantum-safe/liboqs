#include "align.h"
#include "cbd.h"
#include "consts.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "reduce.h"
#include "symmetric.h"
#include <immintrin.h>
#include <stdint.h>

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_compress
*
* Description: Compression and subsequent serialization of a polynomial.
*              The coefficients of the input polynomial are assumed to
*              lie in the invertal [0,q], i.e. the polynomial must be reduced
*              by PQCLEAN_KYBER512_AVX2_poly_reduce().
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - const poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_compress(uint8_t r[128], const poly *restrict a) {
    unsigned int i;
    __m256i f0, f1, f2, f3;
    const __m256i v = _mm256_load_si256(&PQCLEAN_KYBER512_AVX2_qdata.vec[_16XV / 16]);
    const __m256i shift1 = _mm256_set1_epi16(1 << 9);
    const __m256i mask = _mm256_set1_epi16(15);
    const __m256i shift2 = _mm256_set1_epi16((16 << 8) + 1);
    const __m256i permdidx = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);

    for (i = 0; i < KYBER_N / 64; i++) {
        f0 = _mm256_load_si256(&a->vec[4 * i + 0]);
        f1 = _mm256_load_si256(&a->vec[4 * i + 1]);
        f2 = _mm256_load_si256(&a->vec[4 * i + 2]);
        f3 = _mm256_load_si256(&a->vec[4 * i + 3]);
        f0 = _mm256_mulhi_epi16(f0, v);
        f1 = _mm256_mulhi_epi16(f1, v);
        f2 = _mm256_mulhi_epi16(f2, v);
        f3 = _mm256_mulhi_epi16(f3, v);
        f0 = _mm256_mulhrs_epi16(f0, shift1);
        f1 = _mm256_mulhrs_epi16(f1, shift1);
        f2 = _mm256_mulhrs_epi16(f2, shift1);
        f3 = _mm256_mulhrs_epi16(f3, shift1);
        f0 = _mm256_and_si256(f0, mask);
        f1 = _mm256_and_si256(f1, mask);
        f2 = _mm256_and_si256(f2, mask);
        f3 = _mm256_and_si256(f3, mask);
        f0 = _mm256_packus_epi16(f0, f1);
        f2 = _mm256_packus_epi16(f2, f3);
        f0 = _mm256_maddubs_epi16(f0, shift2);
        f2 = _mm256_maddubs_epi16(f2, shift2);
        f0 = _mm256_packus_epi16(f0, f2);
        f0 = _mm256_permutevar8x32_epi32(f0, permdidx);
        _mm256_storeu_si256((__m256i *)&r[32 * i], f0);
    }
}

void PQCLEAN_KYBER512_AVX2_poly_decompress(poly *restrict r, const uint8_t a[128]) {
    unsigned int i;
    __m128i t;
    __m256i f;
    const __m256i q = _mm256_load_si256(&PQCLEAN_KYBER512_AVX2_qdata.vec[_16XQ / 16]);
    const __m256i shufbidx = _mm256_set_epi8(7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4,
                             3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0);
    const __m256i mask = _mm256_set1_epi32(0x00F0000F);
    const __m256i shift = _mm256_set1_epi32((128 << 16) + 2048);

    for (i = 0; i < KYBER_N / 16; i++) {
        t = _mm_loadl_epi64((__m128i *)&a[8 * i]);
        f = _mm256_broadcastsi128_si256(t);
        f = _mm256_shuffle_epi8(f, shufbidx);
        f = _mm256_and_si256(f, mask);
        f = _mm256_mullo_epi16(f, shift);
        f = _mm256_mulhrs_epi16(f, q);
        _mm256_store_si256(&r->vec[i], f);
    }
}


/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_tobytes
*
* Description: Serialization of a polynomial in NTT representation.
*              The coefficients of the input polynomial are assumed to
*              lie in the invertal [0,q], i.e. the polynomial must be reduced
*              by PQCLEAN_KYBER512_AVX2_poly_reduce(). The coefficients are orderd as output by
*              PQCLEAN_KYBER512_AVX2_poly_ntt(); the serialized output coefficients are in bitreversed
*              order.
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_tobytes(uint8_t r[KYBER_POLYBYTES], poly *a) {
    PQCLEAN_KYBER512_AVX2_ntttobytes_avx(r, a->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of PQCLEAN_KYBER512_AVX2_poly_tobytes
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_frombytes(poly *r, const uint8_t a[KYBER_POLYBYTES]) {
    PQCLEAN_KYBER512_AVX2_nttfrombytes_avx(r->vec, a, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_frommsg(poly *restrict r, const uint8_t msg[KYBER_INDCPA_MSGBYTES]) {
    __m256i f, g0, g1, g2, g3, h0, h1, h2, h3;
    const __m256i shift = _mm256_broadcastsi128_si256(_mm_set_epi32(0, 1, 2, 3));
    const __m256i idx = _mm256_broadcastsi128_si256(_mm_set_epi8(15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0));
    const __m256i hqs = _mm256_set1_epi16((KYBER_Q + 1) / 2);

#define FROMMSG64(i)                        \
    g3 = _mm256_shuffle_epi32(f,0x55*(i));                \
    g3 = _mm256_sllv_epi32(g3,shift);             \
    g3 = _mm256_shuffle_epi8(g3,idx);             \
    g0 = _mm256_slli_epi16(g3,12);                \
    g1 = _mm256_slli_epi16(g3,8);                 \
    g2 = _mm256_slli_epi16(g3,4);                 \
    g0 = _mm256_srai_epi16(g0,15);                \
    g1 = _mm256_srai_epi16(g1,15);                \
    g2 = _mm256_srai_epi16(g2,15);                \
    g3 = _mm256_srai_epi16(g3,15);                \
    g0 = _mm256_and_si256(g0,hqs);  /* 19 18 17 16  3  2  1  0 */ \
    g1 = _mm256_and_si256(g1,hqs);  /* 23 22 21 20  7  6  5  4 */ \
    g2 = _mm256_and_si256(g2,hqs);  /* 27 26 25 24 11 10  9  8 */ \
    g3 = _mm256_and_si256(g3,hqs);  /* 31 30 29 28 15 14 13 12 */ \
    h0 = _mm256_unpacklo_epi64(g0,g1);                \
    h2 = _mm256_unpackhi_epi64(g0,g1);                \
    h1 = _mm256_unpacklo_epi64(g2,g3);                \
    h3 = _mm256_unpackhi_epi64(g2,g3);                \
    g0 = _mm256_permute2x128_si256(h0,h1,0x20);           \
    g2 = _mm256_permute2x128_si256(h0,h1,0x31);           \
    g1 = _mm256_permute2x128_si256(h2,h3,0x20);           \
    g3 = _mm256_permute2x128_si256(h2,h3,0x31);           \
    _mm256_store_si256(&r->vec[0+2*(i)+0],g0);    \
    _mm256_store_si256(&r->vec[0+2*(i)+1],g1);    \
    _mm256_store_si256(&r->vec[8+2*(i)+0],g2);    \
    _mm256_store_si256(&r->vec[8+2*(i)+1],g3)

    f = _mm256_loadu_si256((__m256i *)msg);
    FROMMSG64(0);
    FROMMSG64(1);
    FROMMSG64(2);
    FROMMSG64(3);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_tomsg
*
* Description: Convert polynomial to 32-byte message.
*              The coefficients of the input polynomial are assumed to
*              lie in the invertal [0,q], i.e. the polynomial must be reduced
*              by PQCLEAN_KYBER512_AVX2_poly_reduce().
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], poly *restrict a) {
    unsigned int i;
    uint32_t small;
    __m256i f0, f1, g0, g1;
    const __m256i hq = _mm256_set1_epi16((KYBER_Q - 1) / 2);
    const __m256i hhq = _mm256_set1_epi16((KYBER_Q - 1) / 4);

    for (i = 0; i < KYBER_N / 32; i++) {
        f0 = _mm256_load_si256(&a->vec[2 * i + 0]);
        f1 = _mm256_load_si256(&a->vec[2 * i + 1]);
        f0 = _mm256_sub_epi16(hq, f0);
        f1 = _mm256_sub_epi16(hq, f1);
        g0 = _mm256_srai_epi16(f0, 15);
        g1 = _mm256_srai_epi16(f1, 15);
        f0 = _mm256_xor_si256(f0, g0);
        f1 = _mm256_xor_si256(f1, g1);
        f0 = _mm256_sub_epi16(f0, hhq);
        f1 = _mm256_sub_epi16(f1, hhq);
        f0 = _mm256_packs_epi16(f0, f1);
        small = _mm256_movemask_epi8(f0);
        msg[4 * i + 0] = small;
        msg[4 * i + 1] = small >> 16;
        msg[4 * i + 2] = small >>  8;
        msg[4 * i + 3] = small >> 24;
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_getnoise_eta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA1
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_getnoise_eta1(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce) {
    ALIGNED_UINT8(KYBER_ETA1 * KYBER_N / 4 + 32) buf; // +32 bytes as required by PQCLEAN_KYBER512_AVX2_poly_cbd_eta1
    prf(buf.coeffs, KYBER_ETA1 * KYBER_N / 4, seed, nonce);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r, buf.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_getnoise_eta2(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce) {
    ALIGNED_UINT8(KYBER_ETA2 * KYBER_N / 4) buf;
    prf(buf.coeffs, KYBER_ETA2 * KYBER_N / 4, seed, nonce);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta2(r, buf.vec);
}

#define NOISE_NBLOCKS ((KYBER_ETA1*KYBER_N/4+SHAKE256_RATE-1)/SHAKE256_RATE)
void PQCLEAN_KYBER512_AVX2_poly_getnoise_eta1_4x(poly *r0,
        poly *r1,
        poly *r2,
        poly *r3,
        const uint8_t seed[32],
        uint8_t nonce0,
        uint8_t nonce1,
        uint8_t nonce2,
        uint8_t nonce3) {
    ALIGNED_UINT8(NOISE_NBLOCKS * SHAKE256_RATE) buf[4];
    __m256i f;
    keccakx4_state state;

    f = _mm256_loadu_si256((__m256i *)seed);
    _mm256_store_si256(buf[0].vec, f);
    _mm256_store_si256(buf[1].vec, f);
    _mm256_store_si256(buf[2].vec, f);
    _mm256_store_si256(buf[3].vec, f);

    buf[0].coeffs[32] = nonce0;
    buf[1].coeffs[32] = nonce1;
    buf[2].coeffs[32] = nonce2;
    buf[3].coeffs[32] = nonce3;

    PQCLEAN_KYBER512_AVX2_shake256x4_absorb_once(&state, buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, 33);
    PQCLEAN_KYBER512_AVX2_shake256x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, NOISE_NBLOCKS, &state);

    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r0, buf[0].vec);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r1, buf[1].vec);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r2, buf[2].vec);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r3, buf[3].vec);
}

void PQCLEAN_KYBER512_AVX2_poly_getnoise_eta1122_4x(poly *r0,
        poly *r1,
        poly *r2,
        poly *r3,
        const uint8_t seed[32],
        uint8_t nonce0,
        uint8_t nonce1,
        uint8_t nonce2,
        uint8_t nonce3) {
    ALIGNED_UINT8(NOISE_NBLOCKS * SHAKE256_RATE) buf[4];
    __m256i f;
    keccakx4_state state;

    f = _mm256_loadu_si256((__m256i *)seed);
    _mm256_store_si256(buf[0].vec, f);
    _mm256_store_si256(buf[1].vec, f);
    _mm256_store_si256(buf[2].vec, f);
    _mm256_store_si256(buf[3].vec, f);

    buf[0].coeffs[32] = nonce0;
    buf[1].coeffs[32] = nonce1;
    buf[2].coeffs[32] = nonce2;
    buf[3].coeffs[32] = nonce3;

    PQCLEAN_KYBER512_AVX2_shake256x4_absorb_once(&state, buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, 33);
    PQCLEAN_KYBER512_AVX2_shake256x4_squeezeblocks(buf[0].coeffs, buf[1].coeffs, buf[2].coeffs, buf[3].coeffs, NOISE_NBLOCKS, &state);

    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r0, buf[0].vec);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta1(r1, buf[1].vec);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta2(r2, buf[2].vec);
    PQCLEAN_KYBER512_AVX2_poly_cbd_eta2(r3, buf[3].vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place.
*              Input coefficients assumed to be in normal order,
*              output coefficients are in special order that is natural
*              for the vectorization. Input coefficients are assumed to be
*              bounded by q in absolute value, output coefficients are bounded
*              by 16118 in absolute value.
*
* Arguments:   - poly *r: pointer to in/output polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_ntt(poly *r) {
    PQCLEAN_KYBER512_AVX2_ntt_avx(r->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              Input coefficients assumed to be in special order from vectorized
*              forward ntt, output in normal order. Input coefficients can be
*              arbitrary 16-bit integers, output coefficients are bounded by 14870
*              in absolute value.
*
* Arguments:   - poly *a: pointer to in/output polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_invntt_tomont(poly *r) {
    PQCLEAN_KYBER512_AVX2_invntt_avx(r->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

void PQCLEAN_KYBER512_AVX2_poly_nttunpack(poly *r) {
    PQCLEAN_KYBER512_AVX2_nttunpack_avx(r->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain.
*              One of the input polynomials needs to have coefficients
*              bounded by q, the other polynomial can have arbitrary
*              coefficients. Output coefficients are bounded by 6656.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_basemul_montgomery(poly *r, const poly *a, const poly *b) {
    PQCLEAN_KYBER512_AVX2_basemul_avx(r->vec, a->vec, b->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_tomont(poly *r) {
    PQCLEAN_KYBER512_AVX2_tomont_avx(r->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_reduce(poly *r) {
    PQCLEAN_KYBER512_AVX2_reduce_avx(r->vec, PQCLEAN_KYBER512_AVX2_qdata.vec);
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_add
*
* Description: Add two polynomials. No modular reduction
*              is performed.
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_add(poly *r, const poly *a, const poly *b) {
    unsigned int i;
    __m256i f0, f1;

    for (i = 0; i < KYBER_N / 16; i++) {
        f0 = _mm256_load_si256(&a->vec[i]);
        f1 = _mm256_load_si256(&b->vec[i]);
        f0 = _mm256_add_epi16(f0, f1);
        _mm256_store_si256(&r->vec[i], f0);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER512_AVX2_poly_sub
*
* Description: Subtract two polynomials. No modular reduction
*              is performed.
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_KYBER512_AVX2_poly_sub(poly *r, const poly *a, const poly *b) {
    unsigned int i;
    __m256i f0, f1;

    for (i = 0; i < KYBER_N / 16; i++) {
        f0 = _mm256_load_si256(&a->vec[i]);
        f1 = _mm256_load_si256(&b->vec[i]);
        f0 = _mm256_sub_epi16(f0, f1);
        _mm256_store_si256(&r->vec[i], f0);
    }
}
