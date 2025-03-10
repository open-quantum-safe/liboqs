// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_comm_avx2.c
/// @brief Implementations for blas_comm_avx2.h
///

#include "gf16.h"

#include "config.h"

#include "gf16_avx2.h"

#include "blas_avx2.h"

#include "blas_comm.h"

#include "blas_matrix_sse.h"
#include "blas_matrix_avx2.h"

#include <immintrin.h>

#include "string.h"

#include "params.h"  // for macro _USE_GF16 and _V
#include "utils_malloc.h"

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  matrix-vector multiplication, GF( 16 ) ////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


#if defined(_USE_GF16)


static inline
void gf16mat_prod_multab_64x_avx2( uint8_t *c, const uint8_t *matA, unsigned n_ele, const __m256i *multab_b ) {
    __m256i mask_f = _mm256_set1_epi8(0xf);
    __m256i a0, a1;
    __m256i r0 = _mm256_setzero_si256();
    __m256i r1 = _mm256_setzero_si256();
    if ( n_ele & 1 ) {
        a0 = _mm256_loadu_si256((const __m256i *) (matA) );
        r0 ^= _mm256_shuffle_epi8( multab_b[0], a0 & mask_f )
              ^_mm256_shuffle_epi8( _mm256_slli_epi16(multab_b[0], 4), _mm256_srli_epi16(a0, 4)&mask_f );
        matA += 32;
        multab_b += 1;
        n_ele -= 1;
    }
    while ( n_ele ) {
        a0 = _mm256_loadu_si256((const __m256i *) (matA) );
        a1 = _mm256_loadu_si256((const __m256i *) (matA + 32) );

        r0 ^= _mm256_shuffle_epi8( multab_b[0], a0 & mask_f )
              ^_mm256_shuffle_epi8( _mm256_slli_epi16(multab_b[0], 4), _mm256_srli_epi16(a0, 4)&mask_f );
        r1 ^= _mm256_shuffle_epi8( multab_b[1], a1 & mask_f )
              ^_mm256_shuffle_epi8( _mm256_slli_epi16(multab_b[1], 4), _mm256_srli_epi16(a1, 4)&mask_f );

        matA += 64;
        multab_b += 2;
        n_ele -= 2;
    }
    _mm256_storeu_si256( (__m256i *)c, r0 ^ r1 );
}

static inline
void gf16mat_prod_multab_96x_avx2( uint8_t *c, const uint8_t *matA, unsigned n_ele, const __m256i *multab_b ) {
    __m256i mask_f = _mm256_set1_epi8(0xf);
    __m256i a0, a1;
    __m256i r0 = _mm256_setzero_si256();
    __m256i r1 = _mm256_setzero_si256();
    __m128i aa0, aa1;
    __m128i rr0 = _mm_setzero_si128();
    __m128i rr1 = _mm_setzero_si128();

    if ( n_ele & 1 ) {
        a0 = _mm256_loadu_si256((const __m256i *) (matA) );
        aa0 = _mm_loadu_si128((const __m128i *) (matA+32) );
        __m256i tab0_l = multab_b[0];
        __m256i tab0_h = _mm256_slli_epi16(tab0_l,4);
        r0  ^= _mm256_shuffle_epi8( tab0_l, a0 & mask_f )
              ^_mm256_shuffle_epi8( tab0_h, _mm256_srli_epi16(a0, 4)&mask_f );
        rr0 ^= _mm_shuffle_epi8( _mm256_castsi256_si128(tab0_l), aa0 & _mm256_castsi256_si128(mask_f) )
              ^_mm_shuffle_epi8( _mm256_castsi256_si128(tab0_h), _mm_srli_epi16(aa0, 4)&_mm256_castsi256_si128(mask_f) );
        matA += 48;
        multab_b += 1;
        n_ele -= 1;
    }
    while ( n_ele ) {
        a0  = _mm256_loadu_si256((const __m256i *) (matA) );
        aa0 = _mm_loadu_si128((const __m128i *) (matA+32) );
        a1  = _mm256_loadu_si256((const __m256i *) (matA + 48) );
        aa1 = _mm_loadu_si128((const __m128i *) (matA+80) );
        __m256i tab0_l = multab_b[0];
        __m256i tab0_h = _mm256_slli_epi16(tab0_l,4);
        __m256i tab1_l = multab_b[1];
        __m256i tab1_h = _mm256_slli_epi16(tab1_l,4);
        r0  ^= _mm256_shuffle_epi8( tab0_l, a0 & mask_f )
              ^_mm256_shuffle_epi8( tab0_h, _mm256_srli_epi16(a0, 4)&mask_f );
        rr0 ^= _mm_shuffle_epi8( _mm256_castsi256_si128(tab0_l), aa0 & _mm256_castsi256_si128(mask_f) )
              ^_mm_shuffle_epi8( _mm256_castsi256_si128(tab0_h), _mm_srli_epi16(aa0, 4)&_mm256_castsi256_si128(mask_f) );
        r1  ^= _mm256_shuffle_epi8( tab1_l, a1 & mask_f )
              ^_mm256_shuffle_epi8( tab1_h, _mm256_srli_epi16(a1, 4)&mask_f );
        rr1 ^= _mm_shuffle_epi8( _mm256_castsi256_si128(tab1_l), aa1 & _mm256_castsi256_si128(mask_f) )
              ^_mm_shuffle_epi8( _mm256_castsi256_si128(tab1_h), _mm_srli_epi16(aa1, 4)&_mm256_castsi256_si128(mask_f) );
        matA += 96;
        multab_b += 2;
        n_ele -= 2;
    }
    _mm256_storeu_si256( (__m256i *)c, r0 ^ r1 );
    _mm_storeu_si128( (__m128i *)(c+32), rr0 ^ rr1 );
}

// this function is slow. It's for the completeness of blas libs and sould not be reached in the UOV implementation.
static inline
void gf16mat_remaining_madd_avx2( uint8_t *dest, const uint8_t *mat, unsigned mat_vec_byte, unsigned rem_byte,
                                  const __m256i *multab_vec_ele, unsigned n_vec_ele ) {
    __m256i mask_f = _mm256_set1_epi8(0xf);
    __m256i dd = _load_ymm(dest,rem_byte);

    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        __m256i tab_l = multab_vec_ele[0];
        __m256i tab_h = _mm256_slli_epi16(tab_l, 4);
        multab_vec_ele ++;
        __m256i mi = _load_ymm( mat , rem_byte );
        dd ^= linear_transform_8x8_256b( tab_l , tab_h , mi , mask_f );
        mat += mat_vec_byte;
    }
    _store_ymm( dest, rem_byte , dd );
}

static inline
void gf16mat_blockmat_madd_avx2( __m256i *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_st_idx, unsigned blk_vec_ymm,
                                 const __m256i *multab_vec_ele, unsigned n_vec_ele ) {
    __m256i mask_f = _mm256_set1_epi8(0xf);

    org_mat += blk_st_idx;
    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        __m256i tab_l = multab_vec_ele[0];
        __m256i tab_h = _mm256_slli_epi16(tab_l, 4);
        multab_vec_ele ++;

        for (unsigned j = 0; j < blk_vec_ymm; j++) {
            __m256i mj = _mm256_loadu_si256( (__m256i *)(org_mat + j * 32) );
            dest[j] ^= linear_transform_8x8_256b( tab_l, tab_h, mj, mask_f );
        }
        org_mat += mat_vec_byte;
    }
}

#define _VEC_YMM_BUF_  (8)

static
void gf16mat_madd_multab_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    const __m256i *multabs = (const __m256i *)multab_b;
    __m256i blockmat_vec[_VEC_YMM_BUF_];
    while (matA_n_vec) {
        unsigned n_ele = (matA_n_vec >= _V) ? _V : matA_n_vec; // _V = 96 in current param for GF(16)
        unsigned vec_len_to_go = matA_vec_byte;
        if ( vec_len_to_go&31 ) {
            unsigned rem = vec_len_to_go&31;
            gf16mat_remaining_madd_avx2( c, matA, matA_vec_byte, rem, multabs, n_ele );
            vec_len_to_go -= rem;
        }

        while ( vec_len_to_go ) {
            unsigned block_len = (vec_len_to_go >= _VEC_YMM_BUF_ * 32) ? _VEC_YMM_BUF_ * 32 : vec_len_to_go;
            unsigned block_st_idx = matA_vec_byte - vec_len_to_go;

            loadu_ymm( blockmat_vec, c + block_st_idx, block_len );
            gf16mat_blockmat_madd_avx2( blockmat_vec, matA, matA_vec_byte, block_st_idx, block_len>>5, multabs, n_ele );
            storeu_ymm( c + block_st_idx, block_len, blockmat_vec );

            vec_len_to_go -= block_len;
        }

        matA_n_vec -= n_ele;
        multabs += n_ele;
        matA += n_ele * matA_vec_byte;
    }
}

#undef _VEC_YMM_BUF_

// public functions

void gf16mat_prod_multab_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    if (32 == matA_vec_byte) {
        gf16mat_prod_multab_64x_avx2(c, matA, matA_n_vec, (const __m256i *)multab_b);
    } else if (48 == matA_vec_byte) {
        gf16mat_prod_multab_96x_avx2(c, matA, matA_n_vec, (const __m256i *)multab_b);
    } else {
        gf256v_set_zero(c, matA_vec_byte);
        gf16mat_madd_multab_avx2(c, matA, matA_vec_byte, matA_n_vec, multab_b);
    }
}

void gf16mat_prod_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    __m256i multabs[_V]; // _V = 96 in current param for GF(16)
    if (32 == matA_vec_byte && matA_n_vec <= _V) {
        _gf16v_generate_multabs_avx2( multabs, b, matA_n_vec );
        gf16mat_prod_multab_64x_avx2(c, matA, matA_n_vec, multabs);
    } else if (48 == matA_vec_byte && matA_n_vec <= _V) {
        _gf16v_generate_multabs_avx2( multabs, b, matA_n_vec );
        gf16mat_prod_multab_96x_avx2(c, matA, matA_n_vec, multabs);
    } else {
        gf256v_set_zero( c, matA_vec_byte );
        while( matA_n_vec ) {
            unsigned n_ele = ( matA_n_vec >= _V)? _V : matA_n_vec;
            _gf16v_generate_multabs_avx2(multabs, b, n_ele );
            gf16mat_madd_multab_avx2(c, matA, matA_vec_byte, matA_n_vec, (const uint8_t *)multabs);
            b += (n_ele >> 1);
            matA += matA_vec_byte * n_ele;
            matA_n_vec -= n_ele;
        }
    }
}


#else  // defined(_USE_GF16)


////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  matrix-vector multiplication, GF( 256 ) ///////////////////
////////////////////////////////////////////////////////////////////////////////////////////


#define BLOCKLENYMM (7)

static
void gf256mat_blockmat_madd_avx2( uint8_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_st_idx, unsigned blk_vec_byte,
                                  const __m256i *multab_vec_ele, unsigned n_vec_ele ) {
    unsigned n_full_ymm = blk_vec_byte >> 5;
    __m256i mask_f = _mm256_set1_epi8(0xf);
    __m256i buf[BLOCKLENYMM];
    for(unsigned i = 0; i < n_full_ymm; i++ ) { buf[i] = _mm256_loadu_si256((__m256i*)(dest+i*32)); }

    org_mat += blk_st_idx;
    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        __m256i m_tab = multab_vec_ele[0];
        multab_vec_ele ++;
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

         for (unsigned j = 0; j < n_full_ymm; j++) {
            __m256i mj = _mm256_loadu_si256( (__m256i *)(org_mat + j * 32) );
            buf[j] ^= linear_transform_8x8_256b( ml, mh, mj, mask_f );
        }
        org_mat += mat_vec_byte;
    }

    for(unsigned i = 0; i < n_full_ymm; i++ ) { _mm256_storeu_si256((__m256i*)(dest+i*32), buf[i]); }
}

static
void gf256mat_remblockmat_madd_avx2( uint8_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_vec_byte,
                                  const __m256i *multab_vec_ele, unsigned n_vec_ele ) {
    unsigned n_full_ymm = blk_vec_byte >> 5;
    unsigned rem = blk_vec_byte & 31;
    __m256i mask_f = _mm256_set1_epi8(0xf);
    __m256i hbuf = _mm256_loadu_si256((__m256i*)dest);
    __m256i buf[BLOCKLENYMM];
    for(unsigned i = 0; i < n_full_ymm; i++ ) { buf[i] = _mm256_loadu_si256((__m256i*)(dest+rem+i*32)); }

    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        __m256i m_tab = multab_vec_ele[0];
        multab_vec_ele ++;
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        hbuf ^= linear_transform_8x8_256b( ml, mh, _mm256_loadu_si256((__m256i*)org_mat), mask_f );
        for (unsigned j = 0; j < n_full_ymm; j++) {
            __m256i mj = _mm256_loadu_si256((__m256i *)(org_mat + rem + j * 32) );
            buf[j] ^= linear_transform_8x8_256b( ml, mh, mj, mask_f );
        }
        org_mat += mat_vec_byte;
    }

    _mm256_storeu_si256((__m256i*)dest,hbuf);
    for(unsigned i = 0; i < n_full_ymm; i++ ) { _mm256_storeu_si256((__m256i*)(dest+rem+i*32), buf[i]); }
}

// presuming matA_vec_byte >= 96
static void gf256mat_madd_multab_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const __m256i *multabs ) {
    unsigned vec_len_to_go = matA_vec_byte;
    if ( vec_len_to_go&31 ) {
        unsigned rem = vec_len_to_go&31;
        vec_len_to_go -= rem;
        unsigned block_len = (vec_len_to_go >= BLOCKLENYMM * 32) ? BLOCKLENYMM * 32 : vec_len_to_go;
        gf256mat_remblockmat_madd_avx2(c,matA,matA_vec_byte,block_len+rem,multabs,matA_n_vec);
        vec_len_to_go -= block_len;
        c += block_len+rem;
    }
    while ( vec_len_to_go ) {
        unsigned block_len = (vec_len_to_go >= BLOCKLENYMM * 32) ? BLOCKLENYMM * 32 : vec_len_to_go;
        unsigned block_st_idx = matA_vec_byte - vec_len_to_go;
        gf256mat_blockmat_madd_avx2( c, matA, matA_vec_byte, block_st_idx, block_len, multabs, matA_n_vec );
        vec_len_to_go -= block_len;
        c += block_len;
    }
}

#undef BLOCKLENYMM

static void gf256mat_prod_multab_1ymm_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const __m256i *multabs ) {
    __m256i tmp = _mm256_setzero_si256();
    __m256i mask_f = _mm256_set1_epi8(0xf);
    for (unsigned i = 0; i < matA_n_vec - 1; i++) {
        __m256i m_tab = multabs[i];
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        __m256i mj = _mm256_loadu_si256( (__m256i *)matA );
        tmp ^= linear_transform_8x8_256b( ml, mh, mj, mask_f );
        matA += matA_vec_byte;
    }{
        unsigned i = matA_n_vec - 1;
        __m256i m_tab = multabs[i];
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        __m256i mj = (32 == matA_vec_byte) ? _mm256_loadu_si256((const __m256i *)matA) : _load_ymm( matA, matA_vec_byte );
        tmp ^= linear_transform_8x8_256b( ml, mh, mj, mask_f );
    }
    _store_ymm( c, matA_vec_byte, tmp );
}
static void gf256mat_prod_multab_2ymm_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const __m256i *multabs ) {
    __m256i tmp0 = _mm256_setzero_si256();
    __m256i tmp1 = _mm256_setzero_si256();
    __m256i mask_f = _mm256_set1_epi8(0xf);
    for (unsigned i = 0; i < matA_n_vec - 1; i++) {
        __m256i m_tab = multabs[i];
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        __m256i mj0 = _mm256_loadu_si256( (__m256i *)matA );
        __m256i mj1 = _mm256_loadu_si256( (__m256i *)(matA + 32) );
        tmp0 ^= linear_transform_8x8_256b( ml, mh, mj0, mask_f );
        tmp1 ^= linear_transform_8x8_256b( ml, mh, mj1, mask_f );
        matA += matA_vec_byte;
    }{
        unsigned i = matA_n_vec - 1;
        __m256i m_tab = multabs[i];
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        __m256i mj0 = _mm256_loadu_si256( (__m256i *)matA );
        __m256i mj1 = (64 == matA_vec_byte) ? _mm256_loadu_si256( (__m256i *)(matA + 32) ) : _load_ymm( matA + 32, matA_vec_byte - 32 );
        tmp0 ^= linear_transform_8x8_256b( ml, mh, mj0, mask_f );
        tmp1 ^= linear_transform_8x8_256b( ml, mh, mj1, mask_f );
    }
    _mm256_storeu_si256( (__m256i *)c, tmp0 );
    _store_ymm( c + 32, matA_vec_byte - 32, tmp1 );
}
static void gf256mat_prod_multab_3ymm_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const __m256i *multabs ) {
    __m256i tmp0 = _mm256_setzero_si256();
    __m256i tmp1 = _mm256_setzero_si256();
    __m256i tmp2 = _mm256_setzero_si256();
    __m256i mask_f = _mm256_set1_epi8(0xf);
    for (unsigned i = 0; i < matA_n_vec - 1; i++) {
        __m256i m_tab = multabs[i];
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        __m256i mj0 = _mm256_loadu_si256( (__m256i *)matA );
        __m256i mj1 = _mm256_loadu_si256( (__m256i *)(matA + 32) );
        __m256i mj2 = _mm256_loadu_si256( (__m256i *)(matA + 64) );
        tmp0 ^= linear_transform_8x8_256b( ml, mh, mj0, mask_f );
        tmp1 ^= linear_transform_8x8_256b( ml, mh, mj1, mask_f );
        tmp2 ^= linear_transform_8x8_256b( ml, mh, mj2, mask_f );
        matA += matA_vec_byte;
    }{
        unsigned i = matA_n_vec - 1;
        __m256i m_tab = multabs[i];
        __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
        __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

        __m256i mj0 = _mm256_loadu_si256( (__m256i *)matA );
        __m256i mj1 = _mm256_loadu_si256( (__m256i *)(matA + 32) );
        __m256i mj2 = (96 == matA_vec_byte) ? _mm256_loadu_si256( (__m256i *)(matA + 64) ) : _load_ymm( matA + 64, matA_vec_byte - 64 );
        tmp0 ^= linear_transform_8x8_256b( ml, mh, mj0, mask_f );
        tmp1 ^= linear_transform_8x8_256b( ml, mh, mj1, mask_f );
        tmp2 ^= linear_transform_8x8_256b( ml, mh, mj2, mask_f );
    }
    _mm256_storeu_si256( (__m256i *)c, tmp0 );
    _mm256_storeu_si256( (__m256i *)(c + 32), tmp1 );
    _store_ymm( c + 64, matA_vec_byte - 64, tmp2 );
}

static void gf256mat_prod_multab_small_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const __m256i *multabs ) {
    // XXX: if(matA_vec_byte <16 ) exit(-1);
    if     (matA_vec_byte <= 32) {
        gf256mat_prod_multab_1ymm_avx2(c, matA, matA_vec_byte, matA_n_vec, multabs);
        return;
    } else if (matA_vec_byte <= 64) {
        gf256mat_prod_multab_2ymm_avx2(c, matA, matA_vec_byte, matA_n_vec, multabs);
        return;
    } else {
        gf256mat_prod_multab_3ymm_avx2(c, matA, matA_vec_byte, matA_n_vec, multabs);
        return;
    }
}


void gf256mat_prod_multab_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    if ( 96 >= matA_vec_byte ) {
        gf256mat_prod_multab_small_avx2(c, matA, matA_vec_byte, matA_n_vec, (const __m256i*) multab_b);
    } else {
        gf256v_set_zero( c, matA_vec_byte );
        gf256mat_madd_multab_avx2( c, matA, matA_vec_byte, matA_n_vec, (const __m256i *)multab_b );
    }
}

static
void gf256mat_prod_small_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    __m256i multabs[32];
    __m256i cc0[3];
    __m256i cc1[3];
    unsigned n_ymm = matA_vec_byte >>5;
    n_ymm += ((matA_vec_byte&31) + 31)>>5;

    for(unsigned i=0;i<n_ymm;i++) cc0[i] = _mm256_setzero_si256();
    while (matA_n_vec) {
        unsigned n_ele = (matA_n_vec >= 32) ? 32 : matA_n_vec;
        gf256v_generate_multabs_avx2( (uint8_t *)multabs, b, n_ele );
        gf256mat_prod_multab_small_avx2( (uint8_t *)cc1, matA, matA_vec_byte, n_ele, multabs );
        for(unsigned i=0;i<n_ymm;i++) cc0[i] ^= cc1[i];
        matA_n_vec -= n_ele;
        b += n_ele;
        matA += n_ele * matA_vec_byte;
    }
    storeu_ymm( c, matA_vec_byte, cc0 );
}


void gf256mat_prod_avx2( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    if ( 96 >= matA_vec_byte ) {
        gf256mat_prod_small_avx2(c, matA, matA_vec_byte, matA_n_vec, b);
        return;
    }

    __m256i multabs[32];
    gf256v_set_zero( c, matA_vec_byte );
    while (matA_n_vec) {
        unsigned n_ele = (matA_n_vec >= 32) ? 32 : matA_n_vec;
        gf256v_generate_multabs_avx2( (uint8_t *)multabs, b, n_ele );
        gf256mat_madd_multab_avx2( c, matA, matA_vec_byte, n_ele, multabs );
        matA_n_vec -= n_ele;
        b += n_ele;
        matA += n_ele * matA_vec_byte;
    }
}


#endif  //  defined(_USE_GF16)




/////////////////////////////////////////////////////////////////////////////////////
///////////////////  code for solving linear equations,  GF(16) /////////////////////
/////////////////////////////////////////////////////////////////////////////////////


#if defined(_USE_GF16)

////////////////  matrix tranpose  ///////////////////////////////


// nainv code
//for (unsigned i = 0; i < 32; i++) {
//    for (unsigned j = i + 1; j < 32; j++) {
//        uint8_t tmp = mat[j * 64 + i];
//        mat[j * 64 + i] = mat[i * 64 + j];
//        mat[i * 64 + j] = tmp;
//    }
//}
static
void gf256mat_transpose_32x32_even( uint8_t *mat_8 ) {
    __m256i mat[32];
    // load
    for (size_t i = 0; i < 32; i++) {
        mat[i] = _mm256_load_si256( (__m256i *)(mat_8 + i * 64) );
    }
    // swap 16x16 blocks
    for (size_t i = 0; i < 16; i++) {
        __m256i tmp = _mm256_permute2x128_si256(mat[i], mat[i + 16], 0x20);
        mat[i + 16] = _mm256_permute2x128_si256(mat[i], mat[i + 16], 0x31);
        mat[i] = tmp;
    }
    // swap 8x8 blocks
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 8; j++) {
            size_t r = 16 * i + j;
            __m256i tmp = _mm256_unpacklo_epi64(mat[r], mat[r + 8]);
            mat[r + 8] = _mm256_unpackhi_epi64(mat[r], mat[r + 8]);
            mat[r] = tmp;
        }
    }
    // swap 4x4 blocks
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            size_t r = 8 * i + j;
            __m256i tmp = _mm256_blend_epi32(mat[r], _mm256_bslli_epi128(mat[r + 4], 4), 0xaa);
            mat[r + 4] = _mm256_blend_epi32(_mm256_bsrli_epi128(mat[r], 4), mat[r + 4], 0xaa);
            mat[r] = tmp;
        }
    }
    // swap 2x2 blocks
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 2; j++) {
            size_t r = 4 * i + j;
            __m256i tmp = _mm256_blend_epi16(mat[r], _mm256_bslli_epi128(mat[r + 2], 2), 0xaa);
            mat[r + 2] = _mm256_blend_epi16(_mm256_bsrli_epi128(mat[r], 2), mat[r + 2], 0xaa);
            mat[r] = tmp;
        }
    }
    // swap last bytes
    for (size_t i = 0; i < 16; i++) {
        size_t r = 2 * i;
        __m256i blend_mask = _mm256_set1_epi16(-256);   // 0xff00
        __m256i tmp = _mm256_blendv_epi8(mat[r], _mm256_bslli_epi128(mat[r + 1], 1), blend_mask);
        mat[r + 1] = _mm256_blendv_epi8(_mm256_bsrli_epi128(mat[r], 1), mat[r + 1], blend_mask);
        mat[r] = tmp;
    }
    // store result
    for (size_t i = 0; i < 32; i++) {
        _mm256_store_si256( (__m256i *)(mat_8 + i * 64), mat[i]);
    }
}

static
void gf16mat_transpose_64x64(uint8_t *mat, const uint8_t *sqmat_a ) {
    for (size_t t = 0; t < 1; t++) {
        memcpy( mat, sqmat_a, 64 * 32 );
        gf256mat_transpose_32x32_even( mat );
        gf256mat_transpose_32x32_even( mat + 32 );
        __m256i mask_0f = _mm256_set1_epi8(0xf);
        __m256i mask_f0 = _mm256_set1_epi8(-16);  // 0xf0
        // transpose 2x2 4-bit blocks
        for (int i = 0; i < 64; i += 2) {
            __m256i in1 = _mm256_load_si256((__m256i *)(mat + i * 32));
            __m256i in2 = _mm256_load_si256((__m256i *)(mat + i * 32 + 32));
            __m256i in1_0f = in1 & mask_0f;
            __m256i in1_f0 = in1 & mask_f0;
            __m256i in2_0f = in2 & mask_0f;
            __m256i in2_f0 = in2 & mask_f0;
            _mm256_store_si256( (__m256i *)(mat + i * 32), in1_0f ^ _mm256_slli_epi16(in2_0f, 4) );
            _mm256_store_si256( (__m256i *)(mat + i * 32 + 32), in2_f0 ^ _mm256_srli_epi16(in1_f0, 4) );
        }
    }
}


//////////////////    Gaussian elimination + Back substitution for solving linear equations  //////////////////


// input -> output
// 0 -> 0xff
// 1 .. 15 -> 0
static inline
uint8_t _if_zero_then_0xff(uint8_t p ) {
    return (p - 1) >> 4; // signed shift. don't know why
}

static inline
unsigned _gf16mat_sol_64x64_avx2( uint8_t *mat, __m256i *vec) {
    //assert( 0==(w_byte&63) ); /// w_byte is a multiple of 64.
    //assert( 128 >= h );

    __m256i mask_f = _mm256_set1_epi8( 0xf );

    PQOV_ALIGN uint8_t pivots[64];
    __m256i multabs[64];

    const unsigned h = 64;
    const unsigned w_byte = 32;

    uint8_t rr8 = 1;
    for (unsigned i = 0; i < 1; i++) {
        if (i & 1) {
            for (unsigned j = 0; j < h; j++) {
                pivots[j] = mat[j * w_byte + (i >> 1)] >> 4;
            }
        } else {
            for (unsigned j = 0; j < h; j++) {
                pivots[j] = mat[j * w_byte + (i >> 1)] & 0xf;
            }
        }

        __m256i rowi = _mm256_load_si256( (__m256i *)(mat + i * w_byte) );

#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF16_ < h) ? i + _GE_EARLY_STOP_STEPS_GF16_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            uint8_t mask = _if_zero_then_0xff( pivots[i] );
            pivots[i] ^= mask & pivots[j];

            __m256i mask_zero = _mm256_set1_epi8( mask );
            vec[i] ^= mask_zero & vec[j];
            rowi   ^= mask_zero & _mm256_load_si256( (__m256i *)(mat + j * w_byte) );
            _mm256_store_si256( (__m256i *)(mat + i * w_byte), rowi );
        }
        uint8_t is_pi_nz = ~_if_zero_then_0xff(pivots[i]);
        rr8 &= is_pi_nz;

        pivots[i] = gf16_inv_sse( pivots[i] );

        for (unsigned j = 0; j < h; j += 16) {
            unsigned num = ((h - j) > 16) ? 16 : h - j;
            gf16v_generate_multab_16_avx2( multabs + j, _mm_load_si128((__m128i *)(pivots + j)), num );
        }

        {
            // pivot row
            __m256i ml = multabs[i];
            __m256i mh = _mm256_slli_epi16(multabs[i], 4);
            rowi   = linear_transform_8x8_256b( ml, mh, rowi, mask_f );
            _mm256_store_si256( (__m256i *)(mat + i * w_byte), rowi );
            vec[i] = _mm256_shuffle_epi8( ml, vec[i] );
        }
        for (unsigned j = i + 1; j < h; j++) {
            __m256i ml = multabs[j];
            __m256i mh = _mm256_slli_epi16(multabs[j], 4);
            __m256i rowj = _mm256_load_si256( (__m256i *)(mat + j * w_byte) );
            rowj ^= linear_transform_8x8_256b( ml, mh, rowi, mask_f );
            _mm256_store_si256( (__m256i *)(mat + j * w_byte), rowj );
            vec[j] ^= _mm256_shuffle_epi8( ml, vec[i] );
        }
    }

    // copy vec to mat
    do {  // i = 0
        __m256i temp = _mm256_insert_epi8( _mm256_setzero_si256(), 1, 0 );
        _mm256_store_si256( (__m256i *)mat, _mm256_load_si256( (__m256i *)mat )^temp ^ vec[0] );
    } while (0);
    for (unsigned i = 1; i < h; i++) {
        _mm256_store_si256( (__m256i *)(mat + i * w_byte), _mm256_load_si256( (__m256i *)(mat + i * w_byte) )^vec[i] );
    }

    for (unsigned i = 1; i < h; i++) {
        if (i & 1) {
            for (unsigned j = i; j < h; j++) {
                pivots[j] = mat[j * w_byte + (i >> 1)] >> 4;
            }
        } else {
            for (unsigned j = i; j < h; j++) {
                pivots[j] = mat[j * w_byte + (i >> 1)] & 0xf;
            }
        }

        __m256i rowi = _mm256_load_si256( (__m256i *)(mat + i * w_byte) );

#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF16_ < h) ? i + _GE_EARLY_STOP_STEPS_GF16_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            uint8_t mask = _if_zero_then_0xff( pivots[i] );
            pivots[i] ^= mask & pivots[j];
            __m256i mask_zero = _mm256_set1_epi8( mask );
            __m256i rowj = _mm256_load_si256( (__m256i *)(mat + j * w_byte) );
            rowi ^= mask_zero & rowj;
            _mm256_store_si256( (__m256i *)(mat + i * w_byte), rowi );
        }
        uint8_t is_pi_nz = ~_if_zero_then_0xff(pivots[i]);
        rr8 &= is_pi_nz;

        pivots[i] = gf16_inv_sse( pivots[i] );

        for (unsigned j = ((i) >> 4) << 4; j < h; j += 16) {
            unsigned num = ((h - j) > 16) ? 16 : h - j;
            gf16v_generate_multab_16_avx2( multabs + j, _mm_load_si128((__m128i *)(pivots + j)), num );
        }

        {
            // pivot row
            __m256i ml = multabs[i];
            __m256i mh = _mm256_slli_epi16(multabs[i], 4);
            rowi = linear_transform_8x8_256b( ml, mh, rowi, mask_f );
            _mm256_store_si256( (__m256i *)(mat + i * w_byte), rowi );
        }
        for (unsigned j = i + 1; j < h; j++) {
            __m256i ml = multabs[j];
            __m256i mh = _mm256_slli_epi16(multabs[j], 4);
            __m256i rowj = _mm256_load_si256( (__m256i *)(mat + j * w_byte) );
            rowj ^= linear_transform_8x8_256b( ml, mh, rowi, mask_f );
            _mm256_store_si256( (__m256i *)(mat + j * w_byte), rowj );
        }
    }

    return rr8;
}


static
unsigned gf16mat_GE_64x64_avx2(uint8_t *sqmat_a, uint8_t *constant) {
    PQOV_ALIGN uint8_t mat[64 * 32];
    __m256i vec[64] = {0};

    const unsigned height = 64;
    //unsigned width  = len/2;

    gf16mat_transpose_64x64( mat, sqmat_a );

    uint8_t *ptr = (uint8_t *)vec;
    for (unsigned i = 0; i < height; i++) {
        ptr[32 * i] = gf16v_get_ele(constant, i);
    }

    unsigned char r8 = _gf16mat_sol_64x64_avx2( mat, vec );

    //for(unsigned i=0;i<height;i++) {  gf16v_set_ele(constant,i, mat[32*i]&0xf ); }    // This is only for the compatibility with ref version.
    memcpy(sqmat_a, mat, 64 * 32 );

    return r8;
}


static
void gf16mat_BS_64x64_avx2(uint8_t *constant, const uint8_t *sq_row_mat_a) {
    PQOV_ALIGN uint8_t mat[64 * 64];

    const unsigned height = 64;

    gf16mat_transpose_64x64( mat, sq_row_mat_a );

    __m256i vec = _mm256_load_si256( (__m256i *) mat ); // or should I follow the convension, loading it from constant ?

    for (unsigned i = 1; i < height; i++) {
        gf16v_set_ele( mat + i * 32, i, 0 );    // set diagonal terms to 0
    }
    for (unsigned i = height - 1; i > 0; i--) {
        __m256i col_i = _mm256_load_si256( (__m256i *)(mat + i * 32) );
        _mm256_store_si256( (__m256i *) mat, vec );
        uint8_t v = gf16v_get_ele( mat, i  );
        vec ^= gf16v_mul_avx2( col_i, v );
    }
    _mm256_storeu_si256( (__m256i *) constant, vec );
}


#include "blas_matrix_ref.h"

unsigned gf16mat_gaussian_elim_avx2(uint8_t *sqmat_a, uint8_t *constant, unsigned len) {
    if (64 == len ) {
        return gf16mat_GE_64x64_avx2( sqmat_a, constant );
    } else {
        return gf16mat_gaussian_elim_ref(sqmat_a, constant, len);
    }
}

void gf16mat_back_substitute_avx2( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len) {
    if (64 == len) {
        gf16mat_BS_64x64_avx2(constant, sq_row_mat_a);
    } else {
        gf16mat_back_substitute_ref(constant, sq_row_mat_a, len);
    }
}


#else  // defined(_USE_GF16)


//////////////////////////////////////////////////////////////////////////////////////
///////////////////  code for solving linear equations,  GF(256) /////////////////////
//////////////////////////////////////////////////////////////////////////////////////



//////////////////    Gaussian elimination + Back substitution for solving linear equations  //////////////////


static
unsigned _gf256mat_gauss_elim_row_echelon_avx2( uint8_t *mat, unsigned h, unsigned w, unsigned offset ) {
    // assert( (w&31)==0 );
    unsigned n_ymm = w >> 5;
    __m256i mask_f = _mm256_set1_epi8(0xf);
    __m128i mask_0 = _mm_setzero_si128();

    // assert( h <= 128 );
#define MAX_H  96
    PQOV_ALIGN uint8_t pivots[MAX_H];
    __m256i multabs[MAX_H];
#undef MAX_H

    uint8_t rr8 = 1;
    for (unsigned i = 0; i < h; i++) {
        unsigned idx = offset + i;
        unsigned char i_r16 = idx & 0xf;
        unsigned i_d16 = idx >> 4;
        unsigned i_d32 = idx >> 5;

        uint8_t *mi = mat + i * w;

#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF256_ < h) ? i + _GE_EARLY_STOP_STEPS_GF256_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            __m128i piv_i   = _mm_load_si128( (__m128i *)( mi + i_d16 * 16 ) );
            __m128i is_zero = _mm_cmpeq_epi8( piv_i, mask_0 );
            __m128i add_mask = _mm_shuffle_epi8( is_zero, _mm_set1_epi8(i_r16) );

            __m256i mask = _mm256_setr_m128i( add_mask, add_mask );
            uint8_t *mj = mat + j * w;

            for (unsigned k = i_d32; k < n_ymm; k++) {
                __m256i p_i = _mm256_load_si256( (__m256i *)( mi + k * 32 ) );
                __m256i p_j = _mm256_load_si256( (__m256i *)( mj + k * 32 ) );

                p_i ^= mask & p_j;
                _mm256_store_si256( (__m256i *)( mi + k * 32 ), p_i );
            }
        }
        for (unsigned j = i; j < h; j++) {
            pivots[j] = mat[j * w + idx];
        }
        rr8 &= gf256_is_nonzero( pivots[i] );
        pivots[i] = gf256_inv_sse( pivots[i] );

        for (unsigned j = ((i) >> 4) << 4; j < h; j += 16) {
            unsigned num = ((h - j) > 16) ? 16 : h - j;
            gf256v_generate_multab_16_avx2( multabs + j, _mm_load_si128((__m128i *)(pivots + j)), num );
        }

        {
            // pivot row
            __m256i m_tab = multabs[i];
            __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
            __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

            for (unsigned k = i_d32; k < n_ymm; k++) {
                __m256i rowi = _mm256_load_si256( (__m256i *)(mi + k * 32) );
                rowi = linear_transform_8x8_256b( ml, mh, rowi, mask_f );
                _mm256_store_si256( (__m256i *)(mi + k * 32), rowi );
            }
        }

        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *mj = mat + j * w;
            __m256i m_tab = multabs[j];
            __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
            __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );

            for (unsigned k = i_d32; k < n_ymm; k++) {
                __m256i rowi = _mm256_load_si256( (__m256i *)(mi + k * 32) );
                rowi = linear_transform_8x8_256b( ml, mh, rowi, mask_f ) ^ _mm256_load_si256( (__m256i *)(mj + k * 32) );
                _mm256_store_si256( (__m256i *)(mj + k * 32), rowi );
            }
        }
    }
    return rr8;
}



unsigned gf256mat_gaussian_elim_avx2(uint8_t *sqmat_a, uint8_t *constant, unsigned len) {
#define MAX_H  96
    PQOV_ALIGN uint8_t mat[MAX_H * (MAX_H + 32)];
#undef MAX_H

    unsigned height = len;
    unsigned width  = ((len + 1 + 31) >> 5) << 5;
    unsigned offset = width - (len + 1);
    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width;
        for (unsigned j = 0; j < height; j++) {
            ai[offset + j] = sqmat_a[j * len + i];    // transpose since sqmat_a is col-major
        }
        ai[offset + height] = constant[i];
    }
    unsigned char r8 =  _gf256mat_gauss_elim_row_echelon_avx2( mat, height, width, offset );

    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width;
        memcpy( sqmat_a + i * len, ai + offset, len );   // output a row-major matrix
        constant[i] = ai[offset + len];
    }
    return r8;
}


void gf256mat_back_substitute_avx2( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len) {
    //const unsigned MAX_H=96;
#define MAX_H  96
    PQOV_ALIGN uint8_t column[MAX_H] = {0};
    PQOV_ALIGN uint8_t temp[MAX_H];
#undef MAX_H
    memcpy( temp, constant, len );
    for (int i = len - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            column[j] = sq_row_mat_a[j * len + i];    // row-major -> column-major, i.e., transpose
        }
        column[i] = 0;
        unsigned len2 = ((i + 31) >> 5) << 5;
        gf256v_madd_avx2( temp, column, temp[i], len2 );
    }
    memcpy( constant, temp, len );
}



#endif //  defined(_USE_GF16)

