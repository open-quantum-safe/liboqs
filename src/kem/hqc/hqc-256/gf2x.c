/**
 * @file gf2x.c
 * @brief AVX2 implementation of multiplication of two polynomials
 */

#include "gf2x.h"
#include "parameters.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <immintrin.h>


#define VEC_N_ARRAY_SIZE_VEC CEIL_DIVIDE(PARAM_N, 256) /*!< The number of needed vectors to store PARAM_N bits*/
#define WORD 64
#define LAST64 (PARAM_N >> 6)

//Parameters for Toom-Cook and UB_Karatsuba
#define T_TM3R_3W (PARAM_N_MULT / 3)
#define T_TM3R (PARAM_N_MULT + 384)
#define tTM3R ((T_TM3R) / WORD)
#define T_TM3R_3W_256 ((T_TM3R_3W + 128) / (4 * WORD))
#define T_TM3R_3W_64 (T_TM3R_3W_256 << 2)
#define T_5W 4096
#define T_5W_256 (T_5W >> 8)
#define T2_5W_256 (2 * T_5W_256)
#define t5 (5 * T_5W / WORD)

__m256i a1_times_a2[VEC_N_256_SIZE_64 >> 1];
__m256i o256[VEC_N_ARRAY_SIZE_VEC];
uint64_t *tmp_reduce = (uint64_t *) o256;
uint64_t bloc64[PARAM_OMEGA_R]; // Allocation with the biggest possible weight
uint64_t bit64[PARAM_OMEGA_R]; // Allocation with the biggest possible weight

static inline void reduce(__m256i *o, const __m256i *a);
static inline void karat_mult_1(__m128i *C, const __m128i *A, const __m128i *B);
static inline void karat_mult_2(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult_4(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult_8(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult_16(__m256i *C, const __m256i *A, const __m256i *B);
static inline void karat_mult_5(__m256i *C, const __m256i *A, const __m256i *B);
static inline void divide_by_x_plus_one_256(__m256i *in, __m256i *out, int32_t size);
static void toom_3_mult(__m256i *Out, const __m256i *A, const __m256i *B);


/**
 * @brief Compute o(x) = a(x) mod \f$ X^n - 1\f$
 *
 * This function computes the modular reduction of the polynomial a(x)
 *
 * @param[out] o Pointer to the result
 * @param[in] a Pointer to the polynomial a(x)
 */
static inline void reduce(__m256i *o, const __m256i *a256) {
    __m256i r256, carry256;
    uint64_t *a = (uint64_t *) a256;
    static const int32_t dec64 = PARAM_N & 0x3f;
    static int32_t d0;
    int32_t i, i2;

    d0 = WORD - dec64;
    for (i = LAST64 ; i < (PARAM_N >> 5) - 4 ; i += 4) {
        r256 = _mm256_lddqu_si256((__m256i const *) (& a[i]));
        r256 = _mm256_srli_epi64(r256, dec64);
        carry256 = _mm256_lddqu_si256((__m256i const *) (& a[i + 1]));
        carry256 = _mm256_slli_epi64(carry256, d0);
        r256 ^= carry256;
        i2 = (i - LAST64) >> 2;
        o256[i2] = a256[i2] ^ r256;
    }

    i = i - LAST64;

    for (; i < LAST64 + 1 ; i++) {
        uint64_t r = a[i + LAST64] >> dec64;
        uint64_t carry = a[i + LAST64 + 1] << d0;
        r ^= carry;
        tmp_reduce[i] = a[i] ^ r;
    }

    tmp_reduce[LAST64] &= RED_MASK;
    memcpy(o, tmp_reduce, VEC_N_SIZE_BYTES);
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

    // Computation of Al.Bl=D0 
    __m128i DD0 = _mm_clmulepi64_si128(Al, Bl, 0);
    __m128i DD2 = _mm_clmulepi64_si128(Al, Bl, 0x11);
    __m128i AAlpAAh = _mm_xor_si128(Al, _mm_shuffle_epi32(Al, 0x4e));
    __m128i BBlpBBh = _mm_xor_si128(Bl, _mm_shuffle_epi32(Bl, 0x4e));
    __m128i DD1 = _mm_xor_si128(_mm_xor_si128(DD0, DD2), _mm_clmulepi64_si128(AAlpAAh, BBlpBBh, 0));
    D0[0] = _mm_xor_si128(DD0, _mm_unpacklo_epi64(_mm_setzero_si128(), DD1));
    D0[1] = _mm_xor_si128(DD2, _mm_unpackhi_epi64(DD1, _mm_setzero_si128()));

    //	Computation of Ah.Bh=D2
    DD0 = _mm_clmulepi64_si128(Ah, Bh, 0);
    DD2 = _mm_clmulepi64_si128(Ah, Bh, 0x11);
    AAlpAAh = _mm_xor_si128(Ah, _mm_shuffle_epi32(Ah, 0x4e));
    BBlpBBh = _mm_xor_si128(Bh, _mm_shuffle_epi32(Bh, 0x4e));
    DD1 = _mm_xor_si128(_mm_xor_si128(DD0, DD2), _mm_clmulepi64_si128(AAlpAAh, BBlpBBh, 0));
    D2[0] = _mm_xor_si128(DD0, _mm_unpacklo_epi64(_mm_setzero_si128(), DD1));
    D2[1] = _mm_xor_si128(DD2, _mm_unpackhi_epi64(DD1, _mm_setzero_si128()));

    // Computation of AlpAh.BlpBh=D1
    // initialisation of AlpAh and BlpBh
    __m128i AlpAh = _mm_xor_si128(Al, Ah);
    __m128i BlpBh = _mm_xor_si128(Bl, Bh);

    DD0 = _mm_clmulepi64_si128(AlpAh, BlpBh, 0);
    DD2 = _mm_clmulepi64_si128(AlpAh, BlpBh, 0x11);
    AAlpAAh = _mm_xor_si128(AlpAh, _mm_shuffle_epi32(AlpAh, 0x4e));
    BBlpBBh = _mm_xor_si128(BlpBh, _mm_shuffle_epi32(BlpBh, 0x4e));
    DD1 = _mm_xor_si128(_mm_xor_si128(DD0, DD2), _mm_clmulepi64_si128(AAlpAAh, BBlpBBh, 0));
    D1[0] = _mm_xor_si128(DD0, _mm_unpacklo_epi64(_mm_setzero_si128(), DD1));
    D1[1] = _mm_xor_si128(DD2, _mm_unpackhi_epi64(DD1, _mm_setzero_si128()));

    // Computation of C
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
    __m128i *A128 = (__m128i *)A, *B128 = (__m128i *)B;

    karat_mult_1((__m128i *) D0, A128, B128);
    karat_mult_1((__m128i *) D2, A128 + 2, B128 + 2);
    SAA = A[0] ^ A[1];
    SBB = B[0] ^ B[1];
    karat_mult_1((__m128i *) D1,(__m128i *) &SAA,(__m128i *) &SBB);

    __m256i middle = _mm256_xor_si256(D0[1], D2[0]);

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

    karat_mult_2(D0, A,B);
    karat_mult_2(D2, A + 2, B + 2);
    SAA[0] = A[0] ^ A[2];
    SBB[0] = B[0] ^ B[2];
    SAA[1] = A[1] ^ A[3];
    SBB[1] = B[1] ^ B[3];
    karat_mult_2(D1, SAA, SBB);

    __m256i middle0 = _mm256_xor_si256(D0[2], D2[0]);
    __m256i middle1 = _mm256_xor_si256(D0[3], D2[1]);

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
    __m256i D0[8], D1[8], D2[8], SAA[4], SBB[4];

    karat_mult_4(D0, A, B);
    karat_mult_4(D2, A + 4, B + 4);
    for(int32_t i = 0 ; i < 4 ; i++) {
        int32_t is = i + 4; 
        SAA[i] = A[i] ^ A[is];
        SBB[i] = B[i] ^ B[is];
    }

    karat_mult_4(D1, SAA, SBB);

    for(int32_t i = 0 ; i < 4 ; i++) {
        int32_t is = i + 4;
        int32_t is2 = is + 4;
        int32_t is3 = is2 + 4;

        __m256i middle = _mm256_xor_si256(D0[is], D2[i]);

        C[i]   = D0[i];
        C[is]  = middle ^ D0[i] ^ D1[i];
        C[is2] = middle ^ D1[is] ^ D2[is];
        C[is3] = D2[is];
    }
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
static inline void karat_mult_16(__m256i *C, const __m256i *A, const __m256i *B) {
    __m256i D0[16], D1[16], D2[16], SAA[8], SBB[8];

    karat_mult_8(D0, A, B);
    karat_mult_8(D2, A + 8, B + 8);

    for(int32_t i = 0 ; i < 8 ; i++) {
        int32_t is = i + 8; 
        SAA[i] = A[i] ^ A[is];
        SBB[i] = B[i] ^ B[is];
    }

    karat_mult_8(D1, SAA, SBB);

    for(int32_t i = 0 ; i < 8 ; i++) {
        int32_t is = i + 8;
        int32_t is2 = is + 8;
        int32_t is3 = is2 + 8;

        __m256i middle = _mm256_xor_si256(D0[is], D2[i]);

        C[i]   = D0[i];
        C[is]  = middle ^ D0[i] ^ D1[i];
        C[is2] = middle ^ D1[is] ^ D2[is];
        C[is3] = D2[is];
    }
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
static inline void karat_mult_5(__m256i *Out, const __m256i *A, const __m256i *B) {
    const __m256i *a0, *b0, *a1, *b1, *a2, *b2, * a3, * b3, *a4, *b4;

    static __m256i aa01[T_5W_256], bb01[T_5W_256], aa02[T_5W_256], bb02[T_5W_256], aa03[T_5W_256], bb03[T_5W_256], aa04[T_5W_256], bb04[T_5W_256], aa12[T_5W_256], bb12[T_5W_256], aa13[T_5W_256], bb13[T_5W_256], aa14[T_5W_256], bb14[T_5W_256], aa23[T_5W_256], bb23[T_5W_256], aa24[T_5W_256], bb24[T_5W_256], aa34[T_5W_256], bb34[T_5W_256];

    static __m256i D0[T2_5W_256], D1[T2_5W_256], D2[T2_5W_256], D3[T2_5W_256], D4[T2_5W_256], D01[T2_5W_256], D02[T2_5W_256], D03[T2_5W_256], D04[T2_5W_256], D12[T2_5W_256], D13[T2_5W_256], D14[T2_5W_256], D23[T2_5W_256], D24[T2_5W_256], D34[T2_5W_256];

    __m256i ro256[t5 >> 1];

    a0 = A;
    a1 = a0 + T_5W_256;
    a2 = a1 + T_5W_256;
    a3 = a2 + T_5W_256;
    a4 = a3 + T_5W_256;
    b0 = B;
    b1 = b0 + T_5W_256;
    b2 = b1 + T_5W_256;
    b3 = b2 + T_5W_256;
    b4 = b3 + T_5W_256;

    for (int32_t i = 0 ; i < T_5W_256 ; i++) {
        aa01[i] = a0[i] ^ a1[i];
        bb01[i] = b0[i] ^ b1[i];

        aa02[i] = a0[i] ^ a2[i];
        bb02[i] = b0[i] ^ b2[i];

        aa03[i] = a0[i] ^ a3[i];
        bb03[i] = b0[i] ^ b3[i];

        aa04[i] = a0[i] ^ a4[i];
        bb04[i] = b0[i] ^ b4[i];

        aa12[i] = a2[i] ^ a1[i];
        bb12[i] = b2[i] ^ b1[i];

        aa13[i] = a3[i] ^ a1[i];
        bb13[i] = b3[i] ^ b1[i];

        aa14[i] = a4[i] ^ a1[i];
        bb14[i] = b4[i] ^ b1[i];

        aa23[i] = a2[i] ^ a3[i];
        bb23[i] = b2[i] ^ b3[i];

        aa24[i] = a2[i] ^ a4[i];
        bb24[i] = b2[i] ^ b4[i];

        aa34[i] = a3[i] ^ a4[i];
        bb34[i] = b3[i] ^ b4[i];
    }

    karat_mult_16(D0, a0, b0);
    karat_mult_16(D1, a1, b1);
    karat_mult_16(D2, a2, b2);
    karat_mult_16(D3, a3, b3);
    karat_mult_16(D4, a4, b4);

    karat_mult_16(D01, aa01, bb01);
    karat_mult_16(D02, aa02, bb02);
    karat_mult_16(D03, aa03, bb03);
    karat_mult_16(D04, aa04, bb04);

    karat_mult_16(D12, aa12, bb12);
    karat_mult_16(D13, aa13, bb13);
    karat_mult_16(D14, aa14, bb14);

    karat_mult_16(D23, aa23, bb23);
    karat_mult_16(D24, aa24, bb24);

    karat_mult_16(D34, aa34, bb34);

    for (int32_t i = 0 ; i < T_5W_256 ; i++) {
        ro256[i] = D0[i];
        ro256[i + T_5W_256] = D0[i + T_5W_256] ^ D01[i] ^ D0[i] ^ D1[i];
        ro256[i + 2 * T_5W_256] = D1[i] ^ D02[i] ^ D0[i] ^ D2[i] ^ D01[i + T_5W_256] ^ D0[i + T_5W_256] ^ D1[i + T_5W_256];
        ro256[i + 3 * T_5W_256] = D1[i + T_5W_256] ^ D03[i] ^ D0[i] ^ D3[i] ^ D12[i] ^ D1[i] ^ D2[i] ^ D02[i + T_5W_256] ^ D0[i + T_5W_256] ^ D2[i + T_5W_256];
        ro256[i + 4 * T_5W_256] = D2[i] ^ D04[i] ^ D0[i] ^ D4[i] ^ D13[i] ^ D1[i] ^ D3[i] ^ D03[i + T_5W_256] ^ D0[i + T_5W_256] ^ D3[i + T_5W_256] ^ D12[i + T_5W_256] ^ D1[i + T_5W_256] ^ D2[i + T_5W_256];
        ro256[i + 5 * T_5W_256] = D2[i + T_5W_256] ^ D14[i] ^ D1[i] ^ D4[i] ^ D23[i] ^ D2[i] ^ D3[i] ^ D04[i + T_5W_256] ^ D0[i + T_5W_256] ^ D4[i + T_5W_256] ^ D13[i + T_5W_256] ^ D1[i + T_5W_256] ^ D3[i + T_5W_256];
        ro256[i + 6 * T_5W_256] = D3[i] ^ D24[i] ^ D2[i] ^ D4[i] ^ D14[i + T_5W_256] ^ D1[i + T_5W_256] ^ D4[i + T_5W_256] ^ D23[i + T_5W_256] ^ D2[i + T_5W_256] ^ D3[i + T_5W_256];
        ro256[i + 7 * T_5W_256] = D3[i + T_5W_256] ^ D34[i] ^ D3[i] ^ D4[i] ^ D24[i + T_5W_256] ^ D2[i + T_5W_256] ^ D4[i + T_5W_256];
        ro256[i + 8 * T_5W_256] = D4[i] ^ D34[i + T_5W_256] ^ D3[i + T_5W_256] ^ D4[i + T_5W_256];
        ro256[i + 9 * T_5W_256] = D4[i + T_5W_256];
    }

    for(int32_t i = 0 ; i < T_5W_256 * 10 ; i++) {
        Out[i] = ro256[i];
    }
}



/**
 * @brief Compute B(x) = A(x)/(x+1) 
 *
 * This function computes A(x)/(x+1) using a Quercia like algorithm
 * @param[out] out Pointer to the result
 * @param[in] in Pointer to the polynomial A(x)
 * @param[in] size used to define the number of coeeficients of A
 */
static inline void divide_by_x_plus_one_256(__m256i *in, __m256i *out, int32_t size) {
    out[0] = in[0];	
    for(int32_t i = 1 ; i < 2 * (size + 2) ; i++) {
        out[i] = out[i - 1] ^ in[i];
    }
}



/**
 * @brief Compute C(x) = A(x)*B(x) using TOOM3Mult with recursive call 
 *
 * This function computes A(x)*B(x) using recursive TOOM-COOK3 Multiplication
 * @param[out] Out Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static void toom_3_mult(__m256i *Out, const __m256i *A, const __m256i *B) {
    static __m256i U0[T_TM3R_3W_256 + 2], V0[T_TM3R_3W_256 + 2], U1[T_TM3R_3W_256 + 2], V1[T_TM3R_3W_256 + 2], U2[T_TM3R_3W_256 + 2], V2[T_TM3R_3W_256 + 2];
    static __m256i W0[2 * (T_TM3R_3W_256 + 2)], W1[2 * (T_TM3R_3W_256 + 2)], W2[2 * (T_TM3R_3W_256 + 2)], W3[2 * (T_TM3R_3W_256 + 2)], W4[2 * (T_TM3R_3W_256 + 2)];
    static __m256i tmp[2 * (T_TM3R_3W_256 + 2) + 3];
    static __m256i ro256[tTM3R / 2];
    static const __m256i zero = {0ul, 0ul, 0ul, 0ul};
    int32_t T2 = T_TM3R_3W_256 << 1;

    for (int32_t i = 0 ; i < T_TM3R_3W_256 ; i++) {
        U0[i]= A[i];
        V0[i]= B[i];
        U1[i]= A[i + T_TM3R_3W_256];
        V1[i]= B[i + T_TM3R_3W_256];
        U2[i]= A[i + T2];
        V2[i]= B[i + T2];
    }

    for (int32_t i = T_TM3R_3W_256 ; i < T_TM3R_3W_256 + 2 ; i++)	{
        U0[i] = zero;
        V0[i] = zero;
        U1[i] = zero;
        V1[i] = zero;
        U2[i] = zero;
        V2[i] = zero;
    }

    // EVALUATION PHASE : x= X^256
    // P(X): P0=(0); P1=(1); P2=(x); P3=(1+x); P4=(\infty)
    // Evaluation: 5*2 add, 2*2 shift; 5 mul (n)
    //W3 = U2 + U1 + U0 ; W2 = V2 + V1 + V0

    for (int32_t i = 0 ; i < T_TM3R_3W_256 ; i++) {
        W3[i] = U0[i] ^ U1[i] ^ U2[i];
        W2[i] = V0[i] ^ V1[i] ^ V2[i];
    }

    for (int32_t i = T_TM3R_3W_256 ; i < T_TM3R_3W_256 + 2 ; i++) {
        W2[i] = zero;
        W3[i] = zero;
    }

    //W1 = W2 * W3
    karat_mult_5(W1, W2, W3);

    //W0 =(U1 + U2*x)*x ; W4 =(V1 + V2*x)*x (SIZE = T_TM3_3W_256 + 2 !)
    W0[0] = zero;
    W4[0] = zero;

    W0[1] = U1[0];
    W4[1] = V1[0];

    for (int32_t i = 1 ; i < T_TM3R_3W_256 + 1 ; i++) {
        W0[i + 1] = U1[i] ^ U2[i - 1];
        W4[i + 1] = V1[i] ^ V2[i - 1];
    }

    W0[T_TM3R_3W_256 + 1] = U2[T_TM3R_3W_256 - 1];
    W4[T_TM3R_3W_256 + 1] = V2[T_TM3R_3W_256 - 1];

    //W3 = W3 + W0      ; W2 = W2 + W4
    for (int32_t i = 0 ; i < T_TM3R_3W_256 + 2 ; i++) {
        W3[i] ^= W0[i];
        W2[i] ^= W4[i];
    }

    //W0 = W0 + U0      ; W4 = W4 + V0
    for (int32_t i = 0 ; i < T_TM3R_3W_256 + 2 ; i++) {
        W0[i] ^= U0[i];
        W4[i] ^= V0[i];
    }

    //W3 = W3 * W2      ; W2 = W0 * W4
    karat_mult_5(tmp, W3, W2);
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) ; i++) {
        W3[i] = tmp[i];
    }

    karat_mult_5(W2, W0, W4);

    //W4 = U2 * V2      ; W0 = U0 * V0
    karat_mult_5(W4, U2, V2);
    karat_mult_5(W0, U0, V0);

    //INTERPOLATION PHASE
    //9 add, 1 shift, 1 Smul, 2 Sdiv (2n)
    //W3 = W3 + W2
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) ; i++) {
        W3[i] ^= W2[i];
    }

    //W1 = W1 + W0
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256) ; i++) {
        W1[i] ^= W0[i];
    }

    //W2 =(W2 + W0)/x
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) - 1 ; i++) {
        int32_t i1 = i + 1;
        W2[i] = W2[i1] ^ W0[i1];
    }

    W2[2 * (T_TM3R_3W_256 + 2) - 1] = zero;

    //W2 =(W2 + W3 + W4*(x^3+1))/(x+1)
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) ; i++) {
        tmp[i] = W2[i] ^ W3[i] ^ W4[i];
    }

    tmp[2 * (T_TM3R_3W_256 + 2)] = zero;
    tmp[2 * (T_TM3R_3W_256 + 2) + 1] = zero;
    tmp[2 * (T_TM3R_3W_256 + 2) + 2] = zero;

    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256) ; i++) {
        tmp[i + 3] ^= W4[i];
    }

    divide_by_x_plus_one_256(tmp, W2, T_TM3R_3W_256);

    //W3 =(W3 + W1)/(x*(x+1))
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) - 1 ; i++) {
        int32_t i1 = i + 1;
        tmp[i] = W3[i1] ^ W1[i1];
    }

    tmp[2 * (T_TM3R_3W_256 + 2) - 1] = (__m256i){0ul, 0ul, 0ul, 0ul};
    divide_by_x_plus_one_256(tmp, W3, T_TM3R_3W_256);

    //W1 = W1 + W4 + W2
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) ; i++) {
        W1[i] ^= W2[i] ^ W4[i];
    }

    //W2 = W2 + W3
    for (int32_t i = 0 ; i < 2 * (T_TM3R_3W_256 + 2) ; i++) {
        W2[i] ^= W3[i];
    }

    //Recomposition
    //W  = W0+ W1*x+ W2*x^2+ W3*x^3 + W4*x^4
    //Note that : W0, W1, W4 of size 2*T_TM3_3W_256, W2 and W3 of size 2*(T_TM3_3W_256+2)
    for (int32_t i = 0 ; i < T_TM3R_3W_256 ; i++) {
        ro256[i] = W0[i];
        ro256[i + T_TM3R_3W_256] = W0[i + T_TM3R_3W_256] ^ W1[i];
        ro256[i + 2 * T_TM3R_3W_256] = W1[i + T_TM3R_3W_256] ^ W2[i];
        ro256[i + 3 * T_TM3R_3W_256] = W2[i + T_TM3R_3W_256] ^ W3[i];
        ro256[i + 4 * T_TM3R_3W_256] = W3[i + T_TM3R_3W_256] ^ W4[i];
        ro256[i + 5 * T_TM3R_3W_256] = W4[i + T_TM3R_3W_256];
    }

    ro256[4 * T_TM3R_3W_256] ^= W2[2 * T_TM3R_3W_256];
    ro256[5 * T_TM3R_3W_256] ^= W3[2 * T_TM3R_3W_256];

    ro256[1 + 4 * T_TM3R_3W_256] ^= W2[1 + 2 * T_TM3R_3W_256];
    ro256[1 + 5 * T_TM3R_3W_256] ^= W3[1 + 2 * T_TM3R_3W_256];

    ro256[2 + 4 * T_TM3R_3W_256] ^= W2[2 + 2 * T_TM3R_3W_256];
    ro256[2 + 5 * T_TM3R_3W_256] ^= W3[2 + 2 * T_TM3R_3W_256];

    ro256[3 + 4 * T_TM3R_3W_256] ^= W2[3 + 2 * T_TM3R_3W_256];
    ro256[3 + 5 * T_TM3R_3W_256] ^= W3[3 + 2 * T_TM3R_3W_256];

    uint64_t *ro64 = (uint64_t *) ro256;
    uint64_t *Out64 = (uint64_t *) Out;
    for (int32_t i = 0 ; i < VEC_N_256_SIZE_64 << 1 ; i++) {
        Out64[i] = ro64[i];
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
void vect_mul(__m256i *o, const __m256i *a1, const __m256i *a2) {	
    toom_3_mult(a1_times_a2, a1, a2);
    reduce(o, a1_times_a2);

    // clear all
    #ifdef __STDC_LIB_EXT1__
        memset_s(a1_times_a2, 0, (VEC_N_SIZE_64 >> 1) * sizeof(__m256i));
    #else
        memset(a1_times_a2, 0, (VEC_N_SIZE_64 >> 1) * sizeof(__m256i));
    #endif
}
