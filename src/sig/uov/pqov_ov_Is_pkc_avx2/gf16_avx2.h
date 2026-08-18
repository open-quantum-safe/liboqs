// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf16_avx2.h
/// @brief Inlined functions for implementing GF arithmetics for AVX2 instruction sets.
///
#ifndef _GF16_AVX2_H_
#define _GF16_AVX2_H_


#include "gf16_sse.h"

#include "gf16_tabs.h"


// AVX2
#include <immintrin.h>



// 6 instructions
static inline __m256i linear_transform_8x8_256b( __m256i tab_l, __m256i tab_h, __m256i v, __m256i mask_f ) {
    return _mm256_shuffle_epi8(tab_l, v & mask_f)^_mm256_shuffle_epi8(tab_h, _mm256_srli_epi16(v, 4)&mask_f);
}


static inline __m256i __tbl32( const uint8_t *ymm_aligned_table, __m256i a ) {
    return _mm256_shuffle_epi8(_mm256_load_si256((__m256i const *)ymm_aligned_table), a);
}


//////////////  GF(16)  /////////////////////////////



static inline __m256i tbl32_gf16_squ( __m256i a ) {
    return __tbl32(__gf16_squ, a);
}

static inline __m256i tbl32_gf16_inv( __m256i a ) {
    return __tbl32(__gf16_inv, a);
}





/////////////// multiplications with multiplication tables ////////////////////////////////////////


//
// Caution: variable 'a' should not be a secret value
//
static inline __m256i tbl32_gf16_mul_const( unsigned char a, __m256i b ) {
    return __tbl32( __gf16_mul + ((unsigned)a) * 32, b );
}


//
// generate multiplication table for '4-bit' variable 'b'
//
static inline __m256i tbl32_gf16_multab( uint8_t b ) {
    __m256i bx = _mm256_set1_epi16( b & 0xf );
    __m256i b1 = _mm256_srli_epi16( bx, 1 );

    __m256i tab0 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 0));
    __m256i tab1 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 1));
    __m256i tab2 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 2));
    __m256i tab3 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 3));

    __m256i mask_1  = _mm256_set1_epi16(1);
    __m256i mask_4  = _mm256_set1_epi16(4);
    __m256i mask_0  = _mm256_setzero_si256();

    return ( tab0 & _mm256_cmpgt_epi16( bx & mask_1, mask_0) )
           ^ ( tab1 & _mm256_cmpgt_epi16( b1 & mask_1, mask_0) )
           ^ ( tab2 & _mm256_cmpgt_epi16( bx & mask_4, mask_0) )
           ^ ( tab3 & _mm256_cmpgt_epi16( b1 & mask_4, mask_0) );
}


//
// multiplication by generating multiplication tables
//
static inline __m256i gf16v_mul_avx2( __m256i a, uint8_t b ) {
    __m256i multab_l = tbl32_gf16_multab( b );
    __m256i multab_h = _mm256_slli_epi16( multab_l, 4 );

    return linear_transform_8x8_256b( multab_l, multab_h, a, _mm256_set1_epi8(0xf) );
}




///////////////////////////////////////////////////////////////////


//
// gf16 multiplication with log/exp tables
//


static inline __m256i tbl32_gf16_log( __m256i a ) {
    return __tbl32(__gf16_log, a);
}

static inline __m256i tbl32_gf16_exp( __m256i a ) {
    return __tbl32(__gf16_exp, a);
}

static inline __m256i tbl32_gf16_mul_log_log( __m256i loga, __m256i logb, __m256i mask_f ) {
    __m256i la_lb = _mm256_add_epi8(loga, logb);
    return tbl32_gf16_exp( _mm256_sub_epi8(la_lb, mask_f & _mm256_cmpgt_epi8(la_lb, mask_f) ) );
}

static inline __m256i tbl32_gf16_mul_log( __m256i a, __m256i logb, __m256i mask_f ) {
    __m256i la = tbl32_gf16_log( a );
    __m256i la_lb = _mm256_add_epi8(la, logb);
    return tbl32_gf16_exp( _mm256_sub_epi8(la_lb, mask_f & _mm256_cmpgt_epi8(la_lb, mask_f) ) );
}

static inline __m256i tbl32_gf16_mulx2_log( __m256i a, __m256i logb, __m256i mask_f ) {
    __m256i a_lo = a & mask_f;
    __m256i a_hi = _mm256_srli_epi16( a, 4 )&mask_f;
    return tbl32_gf16_mul_log( a_lo, logb, mask_f ) ^ _mm256_slli_epi16( tbl32_gf16_mul_log( a_hi, logb, mask_f ), 4 );
}

static inline __m256i tbl32_gf16_mul( __m256i a, __m256i b ) {
    return tbl32_gf16_mul_log_log( tbl32_gf16_log(a), tbl32_gf16_log(b), _mm256_set1_epi8(0xf) );
}






/////////////////////////////  GF(256) ////////////////////////////////////////



// load memory with address depended on the value of a. unsafe if a is a secret.
static inline __m256i tbl32_gf256_mul_const( unsigned char a, __m256i b ) {
    __m256i tab = _mm256_load_si256((__m256i const *) (__gf256_mul +  ((unsigned)a) * 32 ));
    __m256i tab_l = _mm256_permute2x128_si256( tab, tab, 0 );
    __m256i tab_h = _mm256_permute2x128_si256( tab, tab, 0x11 );

    return linear_transform_8x8_256b( tab_l, tab_h, b, _mm256_set1_epi8(0xf) );
}


static inline __m256i tbl32_gf256_squ( __m256i a ) {
    __m256i tab = _mm256_load_si256((__m256i const *) (__gf256_squ));
    __m256i tab_l = _mm256_permute2x128_si256( tab, tab, 0 );
    __m256i tab_h = _mm256_permute2x128_si256( tab, tab, 0x11 );

    return linear_transform_8x8_256b( tab_l, tab_h, a, _mm256_set1_epi8(0xf) );
}


//
// generate multiplication table
//
static inline __m256i tbl32_gf256_multab( uint8_t b ) {
    __m256i bx = _mm256_set1_epi16( b );
    __m256i b1 = _mm256_srli_epi16( bx, 1 );

    __m256i tab0 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 0));
    __m256i tab1 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 1));
    __m256i tab2 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 2));
    __m256i tab3 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 3));
    __m256i tab4 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 4));
    __m256i tab5 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 5));
    __m256i tab6 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 6));
    __m256i tab7 = _mm256_load_si256((__m256i const *) (__gf256_mulbase + 32 * 7));

    __m256i mask_1  = _mm256_set1_epi16(1);
    __m256i mask_4  = _mm256_set1_epi16(4);
    __m256i mask_16 = _mm256_set1_epi16(16);
    __m256i mask_64 = _mm256_set1_epi16(64);
    __m256i mask_0  = _mm256_setzero_si256();

    return ( tab0 & _mm256_cmpgt_epi16( bx & mask_1, mask_0) )
           ^ ( tab1 & _mm256_cmpgt_epi16( b1 & mask_1, mask_0) )
           ^ ( tab2 & _mm256_cmpgt_epi16( bx & mask_4, mask_0) )
           ^ ( tab3 & _mm256_cmpgt_epi16( b1 & mask_4, mask_0) )
           ^ ( tab4 & _mm256_cmpgt_epi16( bx & mask_16, mask_0) )
           ^ ( tab5 & _mm256_cmpgt_epi16( b1 & mask_16, mask_0) )
           ^ ( tab6 & _mm256_cmpgt_epi16( bx & mask_64, mask_0) )
           ^ ( tab7 & _mm256_cmpgt_epi16( b1 & mask_64, mask_0) );
}





//static inline __m256i tbl32_gf256_inv( __m256i a )
// Call gf256_inv_sse()




#endif // _GF16_AVX2_H_



