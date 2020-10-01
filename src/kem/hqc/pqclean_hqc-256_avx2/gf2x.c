#include "gf2x.h"
#include "parameters.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>

/**
 * \file gf2x.c
 * \brief AVX2 implementation of multiplication of two polynomials
 */


// sizes for Toom-Cook
#define T_TM3_3W_256 32
#define T_TM3_3W_64 128
// constants TOOM3REC
#define tTM3R 1116
#define T_TM3R_3W_256 93
#define T_TM3R_3W_64 (T_TM3R_3W_256<<2)

#define VEC_N_SIZE_256 CEIL_DIVIDE(PARAM_N, 256) /*!< The number of needed vectors to store PARAM_N bits*/
__m256i a1_times_a2[2 * VEC_N_SIZE_256 + 1];
uint64_t bloc64[PARAM_OMEGA_R]; // Allocation with the biggest possible weight
uint64_t bit64[PARAM_OMEGA_R]; // Allocation with the biggest possible weight


static inline void reduce(uint64_t *o, const uint64_t *a);
static inline void karat_mult_1(__m128i *C, __m128i *A, __m128i *B);
static inline void karat_mult_2(__m256i *C, __m256i *A, __m256i *B);
static inline void karat_mult_4(__m256i *C, __m256i *A, __m256i *B);
static inline void karat_mult_8(__m256i *C, __m256i *A, __m256i *B);
static inline void karat_mult_16(__m256i *C, __m256i *A, __m256i *B);
static inline void karat_mult_32(__m256i *C, __m256i *A, __m256i *B);
static inline void divByXplus1(__m256i *out, __m256i *in, int size);
static inline void divByXplus1_256(__m256i *out, __m256i *in, int size);
static void TOOM3Mult(__m256i *Out, const uint64_t *A, const uint64_t *B);
static void TOOM3RecMult(__m256i *Out, const uint64_t *A, const uint64_t *B);



/**
 * @brief Compute o(x) = a(x) mod \f$ X^n - 1\f$
 *
 * This function computes the modular reduction of the polynomial a(x)
 *
 * @param[out] o Pointer to the result
 * @param[in] a Pointer to the polynomial a(x)
 */
static inline void reduce(uint64_t *o, const uint64_t *a) {
    uint64_t r;
    uint64_t carry;

    for (uint32_t i = 0; i < VEC_N_SIZE_64; i++) {
        r = a[i + VEC_N_SIZE_64 - 1] >> (PARAM_N & 63);
        carry = (uint64_t) (a[i + VEC_N_SIZE_64] << (64 - (PARAM_N & 63)));
        o[i] = a[i] ^ r ^ carry;
    }

    o[VEC_N_SIZE_64 - 1] &= RED_MASK;
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
static inline void karat_mult_1(__m128i *C, __m128i *A, __m128i *B) {
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
static inline void karat_mult_2(__m256i *C, __m256i *A, __m256i *B) {
    __m256i D0[2], D1[2], D2[2], SAA, SBB;
    __m128i *A128 = (__m128i *)A, *B128 = (__m128i *)B;

    karat_mult_1((__m128i *) D0, A128, B128);
    karat_mult_1((__m128i *) D2, A128 + 2, B128 + 2);

    SAA = A[0] ^ A[1];
    SBB = B[0] ^ B[1];

    karat_mult_1((__m128i *) D1, (__m128i *) &SAA, (__m128i *) &SBB);
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
static inline void karat_mult_4(__m256i *C, __m256i *A, __m256i *B) {
    __m256i D0[4], D1[4], D2[4], SAA[2], SBB[2];

    karat_mult_2( D0, A, B);
    karat_mult_2(D2, A + 2, B + 2);

    SAA[0] = A[0] ^ A[2];
    SBB[0] = B[0] ^ B[2];
    SAA[1] = A[1] ^ A[3];
    SBB[1] = B[1] ^ B[3];

    karat_mult_2( D1, SAA, SBB);

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
static inline void karat_mult_8(__m256i *C, __m256i *A, __m256i *B) {
    __m256i D0[8], D1[8], D2[8], SAA[4], SBB[4];

    karat_mult_4( D0, A, B);
    karat_mult_4(D2, A + 4, B + 4);

    for (int32_t i = 0; i < 4; i++) {
        int is = i + 4;
        SAA[i] = A[i] ^ A[is];
        SBB[i] = B[i] ^ B[is];
    }

    karat_mult_4(D1, SAA, SBB);

    for (int32_t i = 0; i < 4; i++) {
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
static inline void karat_mult_16(__m256i *C, __m256i *A, __m256i *B) {
    int32_t i, is, is2, is3;
    __m256i D0[16], D1[16], D2[16], SAA[8], SBB[8];

    karat_mult_8( D0, A, B);
    karat_mult_8(D2, A + 8, B + 8);

    for (i = 0; i < 8; i++) {
        is = i + 8;
        SAA[i] = A[i] ^ A[is];
        SBB[i] = B[i] ^ B[is];
    }

    karat_mult_8( D1, SAA, SBB);

    for (i = 0; i < 8; i++) {
        is = i + 8;
        is2 = is + 8;
        is3 = is2 + 8;

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
static inline void karat_mult_32(__m256i *C, __m256i *A, __m256i *B) {
    __m256i D0[32], D1[32], D2[32], SAA[16], SBB[16];
    int32_t i, is, is2, is3;

    karat_mult_16( D0, A, B);
    karat_mult_16(D2, A + 16, B + 16);

    for (i = 0; i < 16; i++) {
        is = i + 16;
        SAA[i] = A[i] ^ A[is];
        SBB[i] = B[i] ^ B[is];
    }

    karat_mult_16( D1, SAA, SBB);

    for (i = 0; i < 16; i++) {
        is = i + 16;
        is2 = is + 16;
        is3 = is2 + 16;

        __m256i middle = _mm256_xor_si256(D0[is], D2[i]);

        C[i]   = D0[i];
        C[is]  = middle ^ D0[i] ^ D1[i];
        C[is2] = middle ^ D1[is] ^ D2[is];
        C[is3] = D2[is];
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
static inline void divByXplus1(__m256i *out, __m256i *in, int size) {
    uint64_t *A = (uint64_t *) in;
    uint64_t *B = (uint64_t *) out;

    B[0] = A[0];

    for (int32_t i = 1; i < 2 * (size << 2); i++) {
        B[i] = B[i - 1] ^ A[i];
    }
}



/**
 * @brief Compute C(x) = A(x)*B(x) using TOOM3Mult
 *
 * This function computes A(x)*B(x) using TOOM-COOK3 Multiplication
 * last multiplication are done using Karatsuba
 * @param[out] Out Pointer to the result
 * @param[in] A Pointer to the polynomial A(x)
 * @param[in] B Pointer to the polynomial B(x)
 */
static void TOOM3Mult(__m256i *Out, const uint64_t *A, const uint64_t *B) {
    static __m256i U0[T_TM3_3W_256], V0[T_TM3_3W_256], U1[T_TM3_3W_256], V1[T_TM3_3W_256], U2[T_TM3_3W_256], V2[T_TM3_3W_256];
    static __m256i W0[2 * (T_TM3_3W_256)], W1[2 * (T_TM3_3W_256)], W2[2 * (T_TM3_3W_256)], W3[2 * (T_TM3_3W_256)], W4[2 * (T_TM3_3W_256)];
    static __m256i tmp[2 * (T_TM3_3W_256)];
    static __m256i ro256[6 * (T_TM3_3W_256)];
    const __m256i zero = _mm256_setzero_si256();
    int64_t *U1_64;
    int64_t *U2_64;
    int64_t *V1_64;
    int64_t *V2_64;
    int32_t T2 = T_TM3_3W_64 << 1;
    int32_t i, i4, i41, i42;

    for (i = 0; i < T_TM3_3W_256 - 1; i++) {
        i4 = i << 2;
        i42 = i4 - 2;
        U0[i] = _mm256_lddqu_si256((__m256i const *)(& A[i4]));
        V0[i] = _mm256_lddqu_si256((__m256i const *)(& B[i4]));
        U1[i] = _mm256_lddqu_si256((__m256i const *)(& A[i42 + T_TM3_3W_64]));
        V1[i] = _mm256_lddqu_si256((__m256i const *)(& B[i42 + T_TM3_3W_64]));
        U2[i] = _mm256_lddqu_si256((__m256i const *)(& A[i4 + T2 - 4]));
        V2[i] = _mm256_lddqu_si256((__m256i const *)(& B[i4 + T2 - 4]));
    }

    for (i = T_TM3_3W_256 - 1; i < T_TM3_3W_256; i++) {
        i4 = i << 2;
        i41 = i4 + 1;
        U0[i] = _mm256_set_epi64x(0, 0, A[i41], A[i4]);
        V0[i] = _mm256_set_epi64x(0, 0, B[i41], B[i4]);
        U1[i] = _mm256_set_epi64x(0, 0, A[i41 + T_TM3_3W_64 - 2], A[i4 + T_TM3_3W_64 - 2]);
        V1[i] = _mm256_set_epi64x(0, 0, B[i41 + T_TM3_3W_64 - 2], B[i4 + T_TM3_3W_64 - 2]);
        U2[i] = _mm256_set_epi64x(0, 0, A[i4 - 3 + T2], A[i4 - 4 + T2]);
        V2[i] = _mm256_set_epi64x(0, 0, B[i4 - 3 + T2], B[i4 - 4 + T2]);
    }

    // Evaluation phase : x= X^64
    // P(X): P0=(0); P1=(1); P2=(x); P3=(1+x); P4=(\infty)
    // Evaluation: 5*2 add, 2*2 shift; 5 mul (n)
    //W3 = U2 + U1 + U0; W2 = V2 + V1 + V0
    for (i = 0; i < T_TM3_3W_256; i++) {
        W3[i] = U0[i] ^ U1[i] ^ U2[i];
        W2[i] = V0[i] ^ V1[i] ^ V2[i];
    }

    //W1 = W2 * W3
    karat_mult_32( W1, W2, W3);

    //W0 =(U1 + U2*x)*x; W4 =(V1 + V2*x)*x (SIZE = T_TM3_3W_256 !)
    U1_64 = ((int64_t *) U1);
    U2_64 = ((int64_t *) U2);

    V1_64 = ((int64_t *) V1);
    V2_64 = ((int64_t *) V2);

    W0[0] = _mm256_set_epi64x(U1_64[2] ^ U2_64[1], U1_64[1] ^ U2_64[0], U1_64[0], 0);
    W4[0] = _mm256_set_epi64x(V1_64[2] ^ V2_64[1], V1_64[1] ^ V2_64[0], V1_64[0], 0);

    for (i = 1; i < T_TM3_3W_256; i++) {
        i4 = i << 2;
        W0[i] = _mm256_lddqu_si256((__m256i const *)(& U1_64[i4 - 1]));
        W0[i] ^= _mm256_lddqu_si256((__m256i const *)(& U2_64[i4 - 2]));

        W4[i] = _mm256_lddqu_si256((__m256i const *)(& V1_64[i4 - 1]));
        W4[i] ^= _mm256_lddqu_si256((__m256i const *)(& V2_64[i4 - 2]));
    }

    //W3 = W3 + W0     ; W2 = W2 + W4
    for (i = 0; i < T_TM3_3W_256; i++) {
        W3[i] ^= W0[i];
        W2[i] ^= W4[i];
    }

    //W0 = W0 + U0     ; W4 = W4 + V0
    for (i = 0; i < T_TM3_3W_256; i++) {
        W0[i] ^= U0[i];
        W4[i] ^= V0[i];
    }

    //W3 = W3 * W2     ; W2 = W0 * W4
    karat_mult_32(tmp, W3, W2);

    for (i = 0; i < 2 * (T_TM3_3W_256); i++) {
        W3[i] = tmp[i];
    }

    karat_mult_32(W2, W0, W4);
    //W4 = U2 * V2     ; W0 = U0 * V0
    karat_mult_32(W4, U2, V2);
    karat_mult_32(W0, U0, V0);

    // Interpolation phase
    // 9 add, 1 shift, 1 Smul, 2 Sdiv (2n)
    //W3 = W3 + W2
    for (i = 0; i < 2 * (T_TM3_3W_256); i++) {
        W3[i] ^= W2[i];
    }

    //W1 = W1 + W0
    for (i = 0; i < 2 * (T_TM3_3W_256); i++) {
        W1[i] ^= W0[i];
    }

    //W2 =(W2 + W0)/x -> x = X^64
    U1_64 = ((int64_t *) W2);
    U2_64 = ((int64_t *) W0);
    for (i = 0; i < (T_TM3_3W_256 << 1); i++) {
        i4 = i << 2;
        W2[i] = _mm256_lddqu_si256((__m256i const *)(& U1_64[i4 + 1]));
        W2[i] ^= _mm256_lddqu_si256((__m256i const *)(& U2_64[i4 + 1]));
    }

    //W2 =(W2 + W3 + W4*(x^3+1))/(x+1)
    U1_64 = ((int64_t *) W4);
    __m256i *U1_256 = (__m256i *) (U1_64 + 1);
    tmp[0] = W2[0] ^ W3[0] ^ W4[0] ^ _mm256_set_epi64x(U1_64[0], 0, 0, 0);

    for (i = 1; i < (T_TM3_3W_256 << 1) - 1; i++) {
        tmp[i] = W2[i] ^ W3[i] ^ W4[i] ^ _mm256_lddqu_si256(&U1_256[i - 1]);
    }

    divByXplus1(W2, tmp, T_TM3_3W_256);
    W2[2 * (T_TM3_3W_256) - 1] = zero;

    //W3 =(W3 + W1)/(x*(x+1))
    U1_64 = (int64_t *) W3;
    U1_256 = (__m256i *) (U1_64 + 1);

    U2_64 = (int64_t *) W1;
    __m256i *U2_256 = (__m256i *) (U2_64 + 1);

    for (i = 0; i < 2 * (T_TM3_3W_256) - 1; i++) {
        tmp[i] = _mm256_lddqu_si256(&U1_256[i]) ^ _mm256_lddqu_si256(&U2_256[i]);
    }

    divByXplus1(W3, tmp, T_TM3_3W_256);
    W3[2 * (T_TM3_3W_256) - 1] = zero;

    //W1 = W1 + W4 + W2
    for (i = 0; i < 2 * (T_TM3_3W_256); i++) {
        W1[i] ^= W2[i] ^ W4[i];
    }

    //W2 = W2 + W3
    for (i = 0; i < 2 * (T_TM3_3W_256); i++) {
        W2[i] ^= W3[i];
    }

    // Recomposition
    //W  = W0+ W1*x+ W2*x^2+ W3*x^3 + W4*x^4
    //W0, W1, W4 of size 2*T_TM3_3W_256, W2 and W3 of size 2*(T_TM3_3W_256)
    for (i = 0; i < (T_TM3_3W_256 << 1) - 1; i++) {
        ro256[i] = W0[i];
        ro256[i + 2 * T_TM3_3W_256 - 1] = W2[i];
        ro256[i + 4 * T_TM3_3W_256 - 2] = W4[i];
    }

    ro256[(T_TM3_3W_256 << 1) - 1] = W0[(T_TM3_3W_256 << 1) - 1] ^ W2[0];
    ro256[(T_TM3_3W_256 << 2) - 2] = W2[(T_TM3_3W_256 << 1) - 1] ^ W4[0];
    ro256[(T_TM3_3W_256 * 6) - 3] = W4[(T_TM3_3W_256 << 1) - 1];

    U1_64 = ((int64_t *) &ro256[T_TM3_3W_256]);
    U1_256 = (__m256i *) (U1_64 - 2);

    U2_64 = ((int64_t *) &ro256[3 * T_TM3_3W_256 - 1]);
    U2_256 = (__m256i *) (U2_64 - 2);

    for (i = 0; i < T_TM3_3W_256 << 1; i++) {
        _mm256_storeu_si256(&U1_256[i], W1[i] ^ _mm256_lddqu_si256(&U1_256[i]));
        _mm256_storeu_si256(&U2_256[i], W3[i] ^ _mm256_loadu_si256(&U2_256[i]));
    }

    for (i = 0; i < 6 * T_TM3_3W_256 - 2; i++) {
        _mm256_storeu_si256(&Out[i], ro256[i]);
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
static inline void divByXplus1_256(__m256i *out, __m256i *in, int32_t size) {
    out[0] = in[0];
    for (int32_t i = 1; i < 2 * (size + 2); i++) {
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
static void TOOM3RecMult(__m256i *Out, const uint64_t *A, const uint64_t *B) {
    __m256i U0[T_TM3R_3W_256 + 2], V0[T_TM3R_3W_256 + 2], U1[T_TM3R_3W_256 + 2], V1[T_TM3R_3W_256 + 2], U2[T_TM3R_3W_256 + 2], V2[T_TM3R_3W_256 + 2];
    __m256i W0[2 * (T_TM3R_3W_256 + 2)], W1[2 * (T_TM3R_3W_256 + 2)], W2[2 * (T_TM3R_3W_256 + 2)], W3[2 * (T_TM3R_3W_256 + 2)], W4[2 * (T_TM3R_3W_256 + 2)];
    __m256i tmp[2 * (T_TM3R_3W_256 + 2) + 3];
    __m256i ro256[tTM3R / 2];
    const __m256i zero = _mm256_setzero_si256();
    int32_t T2 = T_TM3R_3W_64 << 1;
    int32_t i, i1, i4;

    for (i = 0; i < T_TM3R_3W_256; i++) {
        i4 = i << 2;
        U0[i] = _mm256_lddqu_si256((__m256i const *)(& A[i4]));
        V0[i] = _mm256_lddqu_si256((__m256i const *)(& B[i4]));
        U1[i] = _mm256_lddqu_si256((__m256i const *)(& A[i4 + T_TM3R_3W_64]));
        V1[i] = _mm256_lddqu_si256((__m256i const *)(& B[i4 + T_TM3R_3W_64]));
        U2[i] = _mm256_lddqu_si256((__m256i const *)(& A[i4 + T2]));
        V2[i] = _mm256_lddqu_si256((__m256i const *)(& B[i4 + T2]));
    }

    for (i = T_TM3R_3W_256; i < T_TM3R_3W_256 + 2; i++) {
        U0[i] = zero;
        V0[i] = zero;
        U1[i] = zero;
        V1[i] = zero;
        U2[i] = zero;
        V2[i] = zero;
    }

    // Evaluation phase : x= X^256
    // P(X): P0=(0); P1=(1); P2=(x); P3=(1+x); P4=(\infty)
    // Evaluation: 5*2 add, 2*2 shift; 5 mul (n)
    //W3 = U2 + U1 + U0; W2 = V2 + V1 + V0

    for (i = 0; i < T_TM3R_3W_256; i++) {
        W3[i] = U0[i] ^ U1[i] ^ U2[i];
        W2[i] = V0[i] ^ V1[i] ^ V2[i];
    }

    for (i = T_TM3R_3W_256; i < T_TM3R_3W_256 + 2; i++) {
        W2[i] = zero;
        W3[i] = zero;
    }

    //W1 = W2 * W3
    TOOM3Mult(W1, (uint64_t *) W2, (uint64_t *) W3);
    //W0 =(U1 + U2*x)*x; W4 =(V1 + V2*x)*x (SIZE = T_TM3_3W_256 + 2 !)
    W0[0] = zero;
    W4[0] = zero;
    W0[1] = U1[0];
    W4[1] = V1[0];

    for (i = 1; i < T_TM3R_3W_256 + 1; i++) {
        W0[i + 1] = U1[i] ^ U2[i - 1];
        W4[i + 1] = V1[i] ^ V2[i - 1];
    }

    W0[T_TM3R_3W_256 + 1] = U2[T_TM3R_3W_256 - 1];
    W4[T_TM3R_3W_256 + 1] = V2[T_TM3R_3W_256 - 1];

    //W3 = W3 + W0     ; W2 = W2 + W4
    for (i = 0; i < T_TM3R_3W_256 + 2; i++) {
        W3[i] ^= W0[i];
        W2[i] ^= W4[i];
    }

    //W0 = W0 + U0     ; W4 = W4 + V0
    for (i = 0; i < T_TM3R_3W_256 + 2; i++) {
        W0[i] ^= U0[i];
        W4[i] ^= V0[i];
    }

    //W3 = W3 * W2     ; W2 = W0 * W4
    TOOM3Mult(tmp, (uint64_t *) W3, (uint64_t *) W2);

    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2); i++) {
        W3[i] = tmp[i];
    }

    TOOM3Mult(W2, (uint64_t *) W0, (uint64_t *) W4);

    //W4 = U2 * V2     ; W0 = U0 * V0
    TOOM3Mult(W4, (uint64_t *) U2, (uint64_t *) V2);
    TOOM3Mult(W0, (uint64_t *) U0, (uint64_t *) V0);

    //Interpolation phase
    //9 add, 1 shift, 1 Smul, 2 Sdiv (2n)

    //W3 = W3 + W2
    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2); i++) {
        W3[i] ^= W2[i];
    }

    //W1 = W1 + W0
    for (i = 0; i < 2 * (T_TM3R_3W_256); i++) {
        W1[i] ^= W0[i];
    }

    //W2 =(W2 + W0)/x
    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2) - 1; i++) {
        i1 = i + 1;
        W2[i] = W2[i1] ^ W0[i1];
    }

    W2[2 * (T_TM3R_3W_256 + 2) - 1] = zero;

    //W2 =(W2 + W3 + W4*(x^3+1))/(x+1)
    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2); i++) {
        tmp[i] = W2[i] ^ W3[i] ^ W4[i];
    }

    tmp[2 * (T_TM3R_3W_256 + 2)] = zero;
    tmp[2 * (T_TM3R_3W_256 + 2) + 1] = zero;
    tmp[2 * (T_TM3R_3W_256 + 2) + 2] = zero;

    for (i = 0; i < 2 * (T_TM3R_3W_256); i++) {
        tmp[i + 3] ^= W4[i];
    }

    divByXplus1_256(W2, tmp, T_TM3R_3W_256);

    //W3 =(W3 + W1)/(x*(x+1))
    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2) - 1; i++) {
        i1 = i + 1;
        tmp[i] = W3[i1] ^ W1[i1];
    }

    tmp[ 2 * (T_TM3R_3W_256 + 2) - 1] = zero;
    divByXplus1_256(W3, tmp, T_TM3R_3W_256);

    //W1 = W1 + W4 + W2
    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2); i++) {
        W1[i] ^= W2[i] ^ W4[i];
    }
    //W2 = W2 + W3
    for (i = 0; i < 2 * (T_TM3R_3W_256 + 2); i++) {
        W2[i] ^= W3[i];
    }

    // Recomposition
    //W  = W0+ W1*x+ W2*x^2+ W3*x^3 + W4*x^4
    //W0, W1, W4 of size 2*T_TM3_3W_256, W2 and W3 of size 2*(T_TM3_3W_256+2)
    for (i = 0; i < T_TM3R_3W_256; i++) {
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

    ro256[2 + 4 * T_TM3R_3W_256] ^= W2[2 + 2  * T_TM3R_3W_256];
    ro256[2 + 5 * T_TM3R_3W_256] ^= W3[2 + 2 * T_TM3R_3W_256];

    ro256[3 + 4 * T_TM3R_3W_256] ^= W2[3 + 2 * T_TM3R_3W_256];
    ro256[3 + 5 * T_TM3R_3W_256] ^= W3[3 + 2 * T_TM3R_3W_256];


    for (i = 0; i < 2 * VEC_N_SIZE_256 + 1; i++) {
        _mm256_storeu_si256(&Out[i], ro256[i]);
    }
}



/**
 * @brief Multiply two polynomials modulo \f$ X^n - 1\f$.
 *
 * This functions multiplies a sparse polynomial <b>a1</b> (of Hamming weight equal to <b>weight</b>)
 * and a dense polynomial <b>a2</b>. The multiplication is done modulo \f$ X^n - 1\f$.
 *
 * @param[out] o Pointer to the result
 * @param[in] a1 Pointer to a polynomial
 * @param[in] a2 Pointer to a polynomial
 */
void PQCLEAN_HQC256_AVX2_vect_mul(uint64_t *o, const uint64_t *a1, const uint64_t *a2) {
    TOOM3RecMult(a1_times_a2, a1, a2);
    reduce(o, (uint64_t *)a1_times_a2);

    // clear all
    memset(a1_times_a2, 0, (2 * VEC_N_SIZE_256 + 1) * sizeof(__m256i));
}
