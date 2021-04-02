#include "gf2x.h"
#include "parameters.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/**
 * \file gf2x.c
 * \brief AVX2 implementation of multiplication of two polynomials
 */



#define VEC_N_SPLIT_3x3 CEIL_DIVIDE(CEIL_DIVIDE(PARAM_N, 9), 256)
#define VEC_N_SPLIT_3 (3*VEC_N_SPLIT_3x3)

static inline void reduce(uint64_t *o, const __m256i *a);
static inline void karat_mult_1(__m128i *C, const __m128i *A, const __m128i *B);
static inline void karat_mult_2(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult_4(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult_8(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_three_way_mult(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult9(__m256i *C, const aligned_vec_t *A, const aligned_vec_t *B);


/**
 * @brief Compute o(x) = a(x) mod \f$ X^n - 1\f$
 *
 * This function computes the modular reduction of the polynomial a(x)
 *
 * @param[out] o Pointer to the result
 * @param[in] a Pointer to the polynomial a(x)
 */
static inline void reduce(uint64_t *o, const __m256i *a256) {
    size_t i, i2;
    __m256i r256, carry256;
    __m256i *o256 = (__m256i *)o;
    const uint64_t *a64 = (const uint64_t *)a256;
    uint64_t r, carry;

    i2 = 0;
    for (i = (PARAM_N >> 6); i < (PARAM_N >> 5) - 4; i += 4) {
        r256 = _mm256_lddqu_si256((const __m256i *) (& a64[i]));
        r256 = _mm256_srli_epi64(r256, PARAM_N & 63);
        carry256 = _mm256_lddqu_si256((const __m256i *) (& a64[i + 1]));
        carry256 = _mm256_slli_epi64(carry256, (-PARAM_N) & 63);
        r256 ^= carry256;
        _mm256_storeu_si256(&o256[i2], a256[i2] ^ r256);
        i2 += 1;
    }

    i = i - (PARAM_N >> 6);
    for (; i < (PARAM_N >> 6) + 1; i++) {
        r = a64[i + (PARAM_N >> 6)] >> (PARAM_N & 63);
        carry = a64[i + (PARAM_N >> 6) + 1] << ((-PARAM_N) & 63);
        r ^= carry;
        o[i] = a64[i] ^ r;
    }

    o[PARAM_N >> 6] &= RED_MASK;
}



/**
 * @brief Compute C(x) = A(x)*B(x)
 * A(x) and B(x) are stored in 128-bit registers
 * This function computes A(x)*B(x) using Karatsuba
 *
 * @param[out] C Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static inline void karat_mult_1(__m128i *C, const __m128i *A, const __m128i *B) {
    __m128i D1[2];
    __m128i D0[2], D2[2];
    __m128i Al = _mm_loadu_si128(A);
    __m128i Ah = _mm_loadu_si128(A + 1);
    __m128i Bl = _mm_loadu_si128(B);
    __m128i Bh = _mm_loadu_si128(B + 1);

    //  Compute Al.Bl=D0
    __m128i DD0 = _mm_clmulepi64_si128(Al, Bl, 0);
    __m128i DD2 = _mm_clmulepi64_si128(Al, Bl, 0x11);
    __m128i AAlpAAh = _mm_xor_si128(Al, _mm_shuffle_epi32(Al, 0x4e));
    __m128i BBlpBBh = _mm_xor_si128(Bl, _mm_shuffle_epi32(Bl, 0x4e));
    __m128i DD1 = _mm_xor_si128(_mm_xor_si128(DD0, DD2), _mm_clmulepi64_si128(AAlpAAh, BBlpBBh, 0));
    D0[0] = _mm_xor_si128(DD0, _mm_unpacklo_epi64(_mm_setzero_si128(), DD1));
    D0[1] = _mm_xor_si128(DD2, _mm_unpackhi_epi64(DD1, _mm_setzero_si128()));

    //  Compute Ah.Bh=D2
    DD0 = _mm_clmulepi64_si128(Ah, Bh, 0);
    DD2 = _mm_clmulepi64_si128(Ah, Bh, 0x11);
    AAlpAAh = _mm_xor_si128(Ah, _mm_shuffle_epi32(Ah, 0x4e));
    BBlpBBh = _mm_xor_si128(Bh, _mm_shuffle_epi32(Bh, 0x4e));
    DD1 = _mm_xor_si128(_mm_xor_si128(DD0, DD2), _mm_clmulepi64_si128(AAlpAAh, BBlpBBh, 0));
    D2[0] = _mm_xor_si128(DD0, _mm_unpacklo_epi64(_mm_setzero_si128(), DD1));
    D2[1] = _mm_xor_si128(DD2, _mm_unpackhi_epi64(DD1, _mm_setzero_si128()));

    // Compute AlpAh.BlpBh=D1
    // Initialisation of AlpAh and BlpBh
    __m128i AlpAh = _mm_xor_si128(Al, Ah);
    __m128i BlpBh = _mm_xor_si128(Bl, Bh);
    DD0 = _mm_clmulepi64_si128(AlpAh, BlpBh, 0);
    DD2 = _mm_clmulepi64_si128(AlpAh, BlpBh, 0x11);
    AAlpAAh = _mm_xor_si128(AlpAh, _mm_shuffle_epi32(AlpAh, 0x4e));
    BBlpBBh = _mm_xor_si128(BlpBh, _mm_shuffle_epi32(BlpBh, 0x4e));
    DD1 = _mm_xor_si128(_mm_xor_si128(DD0, DD2), _mm_clmulepi64_si128(AAlpAAh, BBlpBBh, 0));
    D1[0] = _mm_xor_si128(DD0, _mm_unpacklo_epi64(_mm_setzero_si128(), DD1));
    D1[1] = _mm_xor_si128(DD2, _mm_unpackhi_epi64(DD1, _mm_setzero_si128()));

    // Final comutation of C
    __m128i middle = _mm_xor_si128(D0[1], D2[0]);
    C[0] = D0[0];
    C[1] = middle ^ D0[0] ^ D1[0];
    C[2] = middle ^ D1[1] ^ D2[1];
    C[3] = D2[1];
}



/**
 * @brief Compute C(x) = A(x)*B(x)
 *
 * This function computes A(x)*B(x) using Karatsuba
 * A(x) and B(x) are stored in 256-bit registers
 * @param[out] C Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static inline void karat_mult_2(__m256i *C, const __m256i *A, const __m256i *B) {
    __m256i D0[2], D1[2], D2[2], SAA, SBB;
    const __m128i *A128 = (const __m128i *)A;
    const __m128i *B128 = (const __m128i *)B;
    __m256i middle;

    karat_mult_1((__m128i *) D0, A128, B128);
    karat_mult_1((__m128i *) D2, A128 + 2, B128 + 2);

    SAA = _mm256_xor_si256(A[0], A[1]);
    SBB = _mm256_xor_si256(B[0], B[1]);

    karat_mult_1((__m128i *) D1, (__m128i *) &SAA, (__m128i *) &SBB);
    middle = _mm256_xor_si256(D0[1], D2[0]);

    C[0] = D0[0];
    C[1] = middle ^ D0[0] ^ D1[0];
    C[2] = middle ^ D1[1] ^ D2[1];
    C[3] = D2[1];
}



/**
 * @brief Compute C(x) = A(x)*B(x)
 *
 * This function computes A(x)*B(x) using Karatsuba
 * A(x) and B(x) are stored in 256-bit registers
 * @param[out] C Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static inline void karat_mult_4(__m256i *C, const __m256i *A, const __m256i *B) {
    __m256i D0[4], D1[4], D2[4], SAA[2], SBB[2];
    __m256i middle0;
    __m256i middle1;

    karat_mult_2(D0, A, B);
    karat_mult_2(D2, A + 2, B + 2);

    SAA[0] = A[0] ^ A[2];
    SBB[0] = B[0] ^ B[2];
    SAA[1] = A[1] ^ A[3];
    SBB[1] = B[1] ^ B[3];

    karat_mult_2( D1, SAA, SBB);

    middle0 = _mm256_xor_si256(D0[2], D2[0]);
    middle1 = _mm256_xor_si256(D0[3], D2[1]);

    C[0] = D0[0];
    C[1] = D0[1];
    C[2] = middle0 ^ D0[0] ^ D1[0];
    C[3] = middle1 ^ D0[1] ^ D1[1];
    C[4] = middle0 ^ D1[2] ^ D2[2];
    C[5] = middle1 ^ D1[3] ^ D2[3];
    C[6] = D2[2];
    C[7] = D2[3];
}



/**
 * @brief Compute C(x) = A(x)*B(x)
 *
 * This function computes A(x)*B(x) using Karatsuba
 * A(x) and B(x) are stored in 256-bit registers
 * @param[out] C Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static inline void karat_mult_8(__m256i *C, const __m256i *A, const __m256i *B) {
    size_t i, is, is2, is3;
    __m256i D0[8], D1[8], D2[8], SAA[4], SBB[4];
    __m256i middle;

    karat_mult_4(D0, A, B);
    karat_mult_4(D2, A + 4, B + 4);

    for (i = 0; i < 4; i++) {
        is = i + 4;
        SAA[i] = A[i] ^ A[is];
        SBB[i] = B[i] ^ B[is];
    }

    karat_mult_4(D1, SAA, SBB);

    for (i = 0; i < 4; i++) {
        is = i + 4;
        is2 = is + 4;
        is3 = is2 + 4;

        middle = _mm256_xor_si256(D0[is], D2[i]);

        C[i]   = D0[i];
        C[is]  = middle ^ D0[i] ^ D1[i];
        C[is2] = middle ^ D1[is] ^ D2[is];
        C[is3] = D2[is];
    }
}



/**
 * @brief Compute C(x) = A(x)*B(x)
 *
 * This function computes A(x)*B(x) using Karatsuba 3 part split
 * A(x) and B(x) are stored in 256-bit registers
 * @param[out] C Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static inline void karat_three_way_mult(__m256i *C, const __m256i *A, const __m256i *B) {
    size_t i, j;
    const __m256i *a0, *b0, *a1, *b1, *a2, *b2;
    __m256i aa01[VEC_N_SPLIT_3x3], bb01[VEC_N_SPLIT_3x3], aa02[VEC_N_SPLIT_3x3], bb02[VEC_N_SPLIT_3x3], aa12[VEC_N_SPLIT_3x3], bb12[VEC_N_SPLIT_3x3];
    __m256i D0[2 * VEC_N_SPLIT_3x3], D1[2 * VEC_N_SPLIT_3x3], D2[2 * VEC_N_SPLIT_3x3], D3[2 * VEC_N_SPLIT_3x3], D4[2 * VEC_N_SPLIT_3x3], D5[2 * VEC_N_SPLIT_3x3];
    __m256i ro256[6 * VEC_N_SPLIT_3x3];
    __m256i middle0;

    a0 = A;
    a1 = A + VEC_N_SPLIT_3x3;
    a2 = A + (VEC_N_SPLIT_3x3 << 1);

    b0 = B;
    b1 = B + VEC_N_SPLIT_3x3;
    b2 = B + (VEC_N_SPLIT_3x3 << 1);

    for (i = 0; i < VEC_N_SPLIT_3x3; i++) {
        aa01[i] = a0[i] ^ a1[i];
        bb01[i] = b0[i] ^ b1[i];

        aa12[i] = a2[i] ^ a1[i];
        bb12[i] = b2[i] ^ b1[i];

        aa02[i] = a0[i] ^ a2[i];
        bb02[i] = b0[i] ^ b2[i];
    }

    karat_mult_8(D0, a0, b0);
    karat_mult_8(D1, a1, b1);
    karat_mult_8(D2, a2, b2);

    karat_mult_8(D3, aa01, bb01);
    karat_mult_8(D4, aa02, bb02);
    karat_mult_8(D5, aa12, bb12);

    for (i = 0; i < VEC_N_SPLIT_3x3; i++) {
        j = i + VEC_N_SPLIT_3x3;
        middle0 = D0[i] ^ D1[i] ^ D0[j];
        ro256[i] = D0[i];
        ro256[j]  = D3[i] ^ middle0;
        ro256[j + VEC_N_SPLIT_3x3] = D4[i] ^ D2[i] ^ D3[j] ^ D1[j] ^ middle0;
        middle0 = D1[j] ^ D2[i] ^ D2[j];
        ro256[j + (VEC_N_SPLIT_3x3 << 1)] = D5[i] ^ D4[j] ^ D0[j] ^ D1[i] ^ middle0;
        ro256[i + (VEC_N_SPLIT_3x3 << 2)] = D5[j] ^ middle0;
        ro256[j + (VEC_N_SPLIT_3x3 << 2)] = D2[j];
    }

    for (i = 0; i < 2 * VEC_N_SPLIT_3; i++) {
        C[i] = ro256[i];
    }
}



/**
 * @brief Compute C(x) = A(x)*B(x)
 *
 * This function computes A(x)*B(x) using Karatsuba 3 part split
 * A(x) and B(x) are stored in 256-bit registers
 * @param[out] C Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static inline void karat_mult9(__m256i *C, const aligned_vec_t *A, const aligned_vec_t *B) {
    size_t i, j;
    const __m256i *a0, *b0, *a1, *b1, *a2, *b2;
    __m256i aa01[VEC_N_SPLIT_3], bb01[VEC_N_SPLIT_3], aa02[VEC_N_SPLIT_3], bb02[VEC_N_SPLIT_3], aa12[VEC_N_SPLIT_3], bb12[VEC_N_SPLIT_3];
    __m256i D0[2 * VEC_N_SPLIT_3], D1[2 * VEC_N_SPLIT_3], D2[2 * VEC_N_SPLIT_3], D3[2 * VEC_N_SPLIT_3], D4[2 * VEC_N_SPLIT_3], D5[2 * VEC_N_SPLIT_3];
    __m256i middle0;

    a0 = (__m256i *)(A->arr64);
    a1 = a0 + VEC_N_SPLIT_3;
    a2 = a0 + (2 * VEC_N_SPLIT_3);

    b0 = (__m256i *)(B->arr64);
    b1 = b0 + VEC_N_SPLIT_3;
    b2 = b0 + (2 * VEC_N_SPLIT_3);

    for (i = 0; i < VEC_N_SPLIT_3; i++) {
        aa01[i] = a0[i] ^ a1[i];
        bb01[i] = b0[i] ^ b1[i];

        aa12[i] = a2[i] ^ a1[i];
        bb12[i] = b2[i] ^ b1[i];

        aa02[i] = a0[i] ^ a2[i];
        bb02[i] = b0[i] ^ b2[i];
    }

    karat_three_way_mult(D0, a0, b0);
    karat_three_way_mult(D1, a1, b1);
    karat_three_way_mult(D2, a2, b2);

    karat_three_way_mult(D3, aa01, bb01);
    karat_three_way_mult(D4, aa02, bb02);
    karat_three_way_mult(D5, aa12, bb12);

    for (i = 0; i < VEC_N_SPLIT_3; i++) {
        j = i + VEC_N_SPLIT_3;
        middle0 = D0[i] ^ D1[i] ^ D0[j];
        C[i] = D0[i];
        C[j]  = D3[i] ^ middle0;
        C[j + VEC_N_SPLIT_3] = D4[i] ^ D2[i] ^ D3[j] ^ D1[j] ^ middle0;
        middle0 = D1[j] ^ D2[i] ^ D2[j];
        C[j + (VEC_N_SPLIT_3 << 1)] = D5[i] ^ D4[j] ^ D0[j] ^ D1[i] ^ middle0;
        C[i + (VEC_N_SPLIT_3 << 2)] = D5[j] ^ middle0;
        C[j + (VEC_N_SPLIT_3 << 2)] = D2[j];
    }
}



/**
 * @brief Multiply two polynomials modulo \f$ X^n - 1\f$.
 *
 * This functions multiplies a dense polynomial <b>a1</b> (of Hamming weight equal to <b>weight</b>)
 * and a dense polynomial <b>a2</b>. The multiplication is done modulo \f$ X^n - 1\f$.
 *
 * @param[out] o Pointer to the result
 * @param[in] a1 Pointer to a polynomial
 * @param[in] a2 Pointer to a polynomial
 */
void PQCLEAN_HQCRMRS128_AVX2_vect_mul(uint64_t *o, const aligned_vec_t *a1, const aligned_vec_t *a2) {
    __m256i a1_times_a2[CEIL_DIVIDE(2 * PARAM_N_MULT + 1, 256)] = {0};
    karat_mult9(a1_times_a2, a1, a2);
    reduce(o, a1_times_a2);
}
