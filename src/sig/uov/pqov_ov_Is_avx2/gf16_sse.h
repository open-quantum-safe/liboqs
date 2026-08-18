// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf16_sse.h
/// @brief Inlined functions for implementing GF arithmetics for SSSE3 instruction sets.
///

#ifndef _GF16_SSE_H_
#define _GF16_SSE_H_

#include "gf16.h"
#include "gf16_tabs.h"

// tables:



// SSE2
#include <emmintrin.h>

// SSSE3
#include <tmmintrin.h>





// 6 instructions
static inline __m128i linear_transform_8x8_128b( __m128i tab_l, __m128i tab_h, __m128i v, __m128i mask_f ) {
    return _mm_shuffle_epi8(tab_l, v & mask_f)^_mm_shuffle_epi8(tab_h, _mm_srli_epi16(v, 4)&mask_f);
}



static inline __m128i __tbl16( const uint8_t *xmm_aligned_table, __m128i a ) {
    return _mm_shuffle_epi8(_mm_load_si128((__m128i const *)xmm_aligned_table), a);
}



//////////////  GF(16)  /////////////////////////////


static inline __m128i tbl_gf16_squ( __m128i a ) {
    return _mm_shuffle_epi8(_mm_load_si128((__m128i const *) __gf16_squ ), a);
}

static inline __m128i tbl_gf16_inv( __m128i a ) {
    return _mm_shuffle_epi8(_mm_load_si128((__m128i const *) __gf16_inv ), a);
}




/////////////// multiplications with multiplication tables ////////////////////////////////////////


//
// Caution: variable 'a' should not be a secret value
//
static inline __m128i tbl_gf16_mul_const( unsigned char a, __m128i b ) {
    return _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf16_mul +  ((unsigned)a) * 32 )), b);
}


//
// generate multiplication table for '4-bit' variable 'b'
//
static inline __m128i tbl_gf16_multab( uint8_t b ) {
    __m128i bx = _mm_set1_epi16( b & 0xf );
    __m128i b1 = _mm_srli_epi16( bx, 1 );
    __m128i tab0 = _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 * 0));
    __m128i tab1 = _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 * 1));
    __m128i tab2 = _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 * 2));
    __m128i tab3 = _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 * 3));
    __m128i mask_1  = _mm_set1_epi16(1);
    __m128i mask_4  = _mm_set1_epi16(4);
    __m128i mask_0  = _mm_setzero_si128();
    return ( tab0 & _mm_cmpgt_epi16( bx & mask_1, mask_0) )
           ^ ( tab1 & _mm_cmpgt_epi16( b1 & mask_1, mask_0) )
           ^ ( tab2 & _mm_cmpgt_epi16( bx & mask_4, mask_0) )
           ^ ( tab3 & _mm_cmpgt_epi16( b1 & mask_4, mask_0) );
}


//
// multiplication by generating multiplication tables
//
static inline __m128i gf16v_mul_sse( __m128i a, uint8_t b ) {
    __m128i mask_f = _mm_set1_epi8( 0xf );
    __m128i multab_l = tbl_gf16_multab( b );
    __m128i multab_h = _mm_slli_epi16( multab_l, 4 );

    return linear_transform_8x8_128b( multab_l, multab_h, a, mask_f );
}


static inline uint8_t gf16_inv_sse(uint8_t a) {
    __m128i tmp0 = _mm_insert_epi16(_mm_setzero_si128(), a, 0);
    __m128i tmp1 = tbl_gf16_inv( tmp0 );
    return _mm_extract_epi16(tmp1, 0) & 0xf;
}



/////////////////////////////////////////////////////////////////////////////////////////////




//
// gf16 multiplication with log/exp tables
//

static inline __m128i tbl_gf16_log( __m128i a ) {
    return _mm_shuffle_epi8(_mm_load_si128((__m128i const *) __gf16_log ), a);
}

static inline __m128i tbl_gf16_exp( __m128i a ) {
    return _mm_shuffle_epi8(_mm_load_si128((__m128i const *) __gf16_exp ), a);
}

static inline __m128i tbl_gf16_mul_log( __m128i a, __m128i logb, __m128i mask_f ) {
    __m128i la = tbl_gf16_log( a );
    __m128i la_lb = _mm_add_epi8(la, logb);
    return tbl_gf16_exp( _mm_sub_epi8(la_lb, mask_f & _mm_cmpgt_epi8(la_lb, mask_f) ) );
}

static inline __m128i tbl_gf16_mul_log_log( __m128i loga, __m128i logb, __m128i mask_f ) {
    __m128i la_lb = _mm_add_epi8(loga, logb);
    return tbl_gf16_exp( _mm_sub_epi8(la_lb, mask_f & _mm_cmpgt_epi8(la_lb, mask_f) ) );
}

static inline __m128i tbl_gf16_mul( __m128i a, __m128i b ) {
    __m128i mask_f = _mm_set1_epi8( 0xf );
    __m128i log_16 = _mm_load_si128((__m128i const *) __gf16_log);
    __m128i exp_16 = _mm_load_si128((__m128i const *) __gf16_exp);

    __m128i la = _mm_shuffle_epi8(log_16, a);
    __m128i lb = _mm_shuffle_epi8(log_16, b);
    __m128i la_lb = _mm_add_epi8(la, lb);

    __m128i r0 = _mm_shuffle_epi8(exp_16, _mm_sub_epi8(la_lb, mask_f & _mm_cmpgt_epi8(la_lb, mask_f) ) );
    return r0;
}








/////////////////////////////  GF(256) ////////////////////////////////////////




static inline __m128i tbl_gf256_mul_const( unsigned char a, __m128i b ) {
    __m128i mask_f = _mm_set1_epi8( 0xf );
    __m128i tab_l = _mm_load_si128((__m128i const *) (__gf256_mul +  ((unsigned)a) * 32 ));
    __m128i tab_h = _mm_load_si128((__m128i const *) (__gf256_mul +  ((unsigned)a) * 32 + 16 ));

    return linear_transform_8x8_128b( tab_l, tab_h, b, mask_f );
}



static inline __m128i tbl_gf256_squ( __m128i a ) {
    __m128i mask_f = _mm_set1_epi8( 0xf );
    __m128i tab_l = _mm_load_si128((__m128i const *) (__gf256_squ) );
    __m128i tab_h = _mm_load_si128((__m128i const *) (__gf256_squ + 16) );

    return linear_transform_8x8_128b( tab_l, tab_h, a, mask_f );
}




//
// generate multiplication table
//
typedef struct __m128ix2 {
    __m128i v0;
    __m128i v1;
} __m128ix2_t;

static inline
__m128ix2_t tbl_gf256_multab( uint8_t b ) {
    __m128i bx   = _mm_set1_epi8( b );
    __m128i mask = _mm_set1_epi8(1);

    __m128i m0 = _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 0));
    __m128i m1 = _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 0 + 16));
    __m128i bi = _mm_cmpeq_epi8(mask, bx & mask);
    __m128ix2_t r;
    r.v0 = m0 & bi;
    r.v1 = m1 & bi;
    for (unsigned i = 1; i < 8; i++) {
        m0 = _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * i));
        m1 = _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * i + 16));
        bx = _mm_srli_epi16(bx, 1);
        bi = _mm_cmpeq_epi8(mask, bx & mask);
        r.v0 ^= m0 & bi;
        r.v1 ^= m1 & bi;
    }
    return r;
}


static inline __m128i tbl_gf256_mul( __m128i a, unsigned char b ) {
    __m128i mask_f = _mm_set1_epi8( 0xf );
    __m128ix2_t tab = tbl_gf256_multab(b);
    return linear_transform_8x8_128b( tab.v0, tab.v1, a, mask_f );
}




static inline __m128i tbl_gf256_mul_vv( __m128i a, __m128i b ) {
    __m128i red    = _mm_set1_epi8(0x1b);
    __m128i mask_0x80 = _mm_set1_epi8((char)0x80);
    __m128i mask_0xff = _mm_cmpeq_epi8(red, red);
    __m128i not_b = b ^ mask_0xff;
    __m128i r = a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 7));
    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 6));

    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 5));

    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 4));

    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 3));

    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 2));

    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, _mm_slli_epi16(not_b, 1));

    a = _mm_add_epi8(a, a)^ _mm_shuffle_epi8(red, a ^ mask_0x80);
    r ^= a & _mm_shuffle_epi8(mask_0xff, not_b);
    return r;
}




static inline uint8_t gf256_inv_sse(uint8_t a) {
    return gf256_inv(a);
}






#endif // _GF16_SSE_H_

