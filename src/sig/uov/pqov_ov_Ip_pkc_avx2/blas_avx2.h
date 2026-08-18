// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_avx2.h
/// @brief Inlined functions for implementing basic linear algebra functions for AVX2 arch.
///

#ifndef _BLAS_AVX2_H_
#define _BLAS_AVX2_H_

#include "gf16.h"

#include <immintrin.h>

#include "config.h"
//#include "assert.h"

#include "gf16_avx2.h"

#include "blas_sse.h"
#include "utils_malloc.h"





static inline
__m256i _load_ymm( const uint8_t *a, unsigned _num_byte ) {
    PQOV_ALIGN uint8_t temp[32];
    //assert( 32 >= _num_byte );
    //assert( 0 < _num_byte );
    for (unsigned i = 0; i < _num_byte; i++) {
        temp[i] = a[i];
    }
    return _mm256_load_si256((__m256i *)temp);
}

static inline
void loadu_ymm( __m256i *ymm_a, const uint8_t *a, unsigned _num_byte ) {
    unsigned n_32 = (_num_byte >> 5);
    unsigned n_32_rem = _num_byte & 0x1f;
    while ( n_32-- ) {
        ymm_a[0] = _mm256_loadu_si256( (__m256i *)(a) );
        ymm_a++;
        a += 32;
    }
    if ( n_32_rem ) {
        ymm_a[0] = _load_ymm( a, n_32_rem );
    }
}



static inline
void _store_ymm( uint8_t *a, unsigned _num_byte, __m256i data ) {
    PQOV_ALIGN uint8_t temp[32];
    //assert( 32 >= _num_byte );
    //assert( 0 < _num_byte );
    _mm256_store_si256((__m256i *)temp, data);
    for (unsigned i = 0; i < _num_byte; i++) {
        a[i] = temp[i];
    }
}


static inline
void storeu_ymm( uint8_t *a, unsigned _num_byte, __m256i *ymm_a ) {
    unsigned n_32 = (_num_byte >> 5);
    unsigned n_32_rem = _num_byte & 0x1f;
    while ( n_32-- ) {
        _mm256_storeu_si256( (__m256i *)a, ymm_a[0] );
        ymm_a++;
        a += 32;
    }
    if ( n_32_rem ) {
        _store_ymm( a, n_32_rem, ymm_a[0] );
    }
}




static inline
void linearmap_8x8_ymm( uint8_t *a, __m256i ml, __m256i mh, __m256i mask, unsigned _num_byte ) {
    unsigned n_32 = _num_byte >> 5;
    unsigned rem = _num_byte & 31;
    if ( rem ) {
        if ( n_32 ) {
            __m256i inp = _mm256_loadu_si256( (__m256i *)a );
            __m256i in1 = _mm256_loadu_si256( (__m256i *)(a+rem) );
            __m256i r0 = linear_transform_8x8_256b( ml, mh, inp, mask );
            _mm256_storeu_si256( (__m256i *)a, r0 );
            _mm256_storeu_si256( (__m256i *)(a+rem), in1 );
        } else {
            linearmap_8x8_sse( a, _mm256_castsi256_si128(ml), _mm256_castsi256_si128(mh), _mm256_castsi256_si128(mask), rem );
        }
        a += rem;
    }
    while (n_32--) {
        __m256i inp = _mm256_loadu_si256( (__m256i *)a );
        __m256i r0 = linear_transform_8x8_256b( ml, mh, inp, mask );
        _mm256_storeu_si256( (__m256i *)a, r0 );
        a += 32;
    }
}


static inline
void linearmap_8x8_accu_ymm( uint8_t *accu_c, const uint8_t *a,  __m256i ml, __m256i mh, __m256i mask, unsigned _num_byte ) {
    unsigned n_32 = _num_byte >> 5;
    unsigned rem = _num_byte & 31;
    if ( rem ) {
        if (n_32 ) {
            __m256i inp = _mm256_loadu_si256( (__m256i *)a );
            __m256i out = _mm256_loadu_si256( (__m256i *)accu_c );
            __m256i ou1 = _mm256_loadu_si256( (__m256i *)(accu_c+rem) );
            __m256i r0 = out ^ linear_transform_8x8_256b( ml, mh, inp, mask );
            _mm256_storeu_si256( (__m256i *)accu_c, r0 );
            _mm256_storeu_si256( (__m256i *)(accu_c+rem), ou1 );
        } else {
            linearmap_8x8_accu_sse( accu_c, a, _mm256_castsi256_si128(ml), _mm256_castsi256_si128(mh), _mm256_castsi256_si128(mask), rem );
        }
        a += rem;
        accu_c += rem;
    }
    while (n_32--) {
        __m256i inp = _mm256_loadu_si256( (__m256i *)a );
        __m256i out = _mm256_loadu_si256( (__m256i *)accu_c );
        __m256i r0 = out ^ linear_transform_8x8_256b( ml, mh, inp, mask );
        _mm256_storeu_si256( (__m256i *)accu_c, r0 );
        a += 32;
        accu_c += 32;
    }
}




///////////////////////   basic functions   /////////////////////////////////////


static inline
void gf256v_add_avx2( uint8_t *accu_b, const uint8_t *a, unsigned _num_byte ) {
    if ( _num_byte & 31 ) {
        unsigned rem = _num_byte & 31;
        if ( _num_byte < 32 ) {
            gf256v_add_sse( accu_b, a, _num_byte );
        } else {
            __m256i aa = _mm256_loadu_si256((__m256i *)a);
            __m256i ab = _mm256_loadu_si256((__m256i *)accu_b);
            __m256i ab1 = _mm256_loadu_si256((__m256i *)(accu_b+rem));
            _mm256_storeu_si256( (__m256i *)accu_b, aa^ab );
            _mm256_storeu_si256( (__m256i *)(accu_b+rem), ab1 );
        }
        a += rem;
        accu_b += rem;
        _num_byte -= rem;
    }
    while ( _num_byte ) {
        _mm256_storeu_si256( (__m256i *)accu_b, _mm256_loadu_si256((__m256i *)a) ^ _mm256_loadu_si256((__m256i *)accu_b) );
        accu_b += 32;
        a += 32;
        _num_byte -= 32;
    }
}



//////////////////////////////////////////////////////////////////////////////////



static inline
void gf16v_mul_scalar_avx2( uint8_t *a, uint8_t gf16_b, unsigned _num_byte ) {
    __m256i ml = tbl32_gf16_multab( gf16_b );
    __m256i mh = _mm256_slli_epi16( ml, 4 );
    __m256i mask = _mm256_set1_epi8(0xf);
    linearmap_8x8_ymm( a, ml, mh, mask, _num_byte );
}



static inline
void gf16v_madd_avx2( uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned _num_byte ) {
    __m256i ml = tbl32_gf16_multab( gf16_b );
    __m256i mh = _mm256_slli_epi16( ml, 4 );
    __m256i mask = _mm256_set1_epi8(0xf);
    linearmap_8x8_accu_ymm( accu_c, a, ml, mh, mask, _num_byte );
}

static inline
void gf16v_madd_multab_avx2( uint8_t *accu_c, const uint8_t *a, const uint8_t *multab, unsigned _num_byte ) {
    __m256i ml = _mm256_load_si256( (__m256i *) (multab) );
    __m256i mh = _mm256_slli_epi16( ml, 4 );
    __m256i mask = _mm256_set1_epi8(0xf);

    linearmap_8x8_accu_ymm( accu_c, a, ml, mh, mask, _num_byte );
}


//////////////////////////////////////////////////////////////////////////////


static inline
void gf256v_mul_scalar_avx2( uint8_t *a, uint8_t _b, unsigned _num_byte ) {
    __m256i m_tab = tbl32_gf256_multab( _b );
    __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
    __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );
    __m256i mask = _mm256_set1_epi8(0xf);

    linearmap_8x8_ymm( a, ml, mh, mask, _num_byte );
}


static inline
void gf256v_madd_multab_avx2( uint8_t *accu_c, const uint8_t *a, const uint8_t *multab, unsigned _num_byte ) {
    __m256i m_tab = _mm256_load_si256( (__m256i *) (multab) );
    __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
    __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );
    __m256i mask = _mm256_set1_epi8(0xf);

    linearmap_8x8_accu_ymm( accu_c, a, ml, mh, mask, _num_byte );
}



static inline
void gf256v_madd_avx2( uint8_t *accu_c, const uint8_t *a, uint8_t _b, unsigned _num_byte ) {
    __m256i m_tab = tbl32_gf256_multab( _b );
    __m256i ml = _mm256_permute2x128_si256( m_tab, m_tab, 0 );
    __m256i mh = _mm256_permute2x128_si256( m_tab, m_tab, 0x11 );
    __m256i mask = _mm256_set1_epi8(0xf);

    linearmap_8x8_accu_ymm( accu_c, a, ml, mh, mask, _num_byte );
}





//////////////////   Generating multiplication tables  /////////////////////////////////////


//
// Caution: multabs are different from ssse3 version
// ssse3:  [multab_low] [ multab_high ]
//         <-   16  ->  <-    16     ->
// avx2:   [         multab_low       ]
//         <---        32          --->
static inline
void gf16v_generate_multab_16_avx2( __m256i *multabs, __m128i a, unsigned len ) {
    __m256i broadcast_x1 = _mm256_set_epi8( 0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16 );
    __m256i broadcast_x2 = _mm256_set_epi8( 0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16 );
    __m256i broadcast_x4 = _mm256_set_epi8( 0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16 );
    __m256i broadcast_x8 = _mm256_set_epi8( 0, 0, 0, 0, 0, 0, 0, 0,  -16, -16, -16, -16, -16, -16, -16, -16,  0, 0, 0, 0, 0, 0, 0, 0,  -16, -16, -16, -16, -16, -16, -16, -16 );

    //__m256i bx1 = _mm256_inserti128_si256( _mm256_castsi128_si256(a), a , 1 );
    __m256i bx1 = _mm256_setr_m128i( a, a );
    __m256i bx2 = _mm256_shuffle_epi8( _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 )), bx1 );
    __m256i bx4 = _mm256_shuffle_epi8( _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 2 )), bx1 );
    __m256i bx8 = _mm256_shuffle_epi8( _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 3 )), bx1 );


    multabs[0] =  _mm256_shuffle_epi8(bx1, broadcast_x1) ^ _mm256_shuffle_epi8(bx2, broadcast_x2)
                  ^ _mm256_shuffle_epi8(bx4, broadcast_x4) ^ _mm256_shuffle_epi8(bx8, broadcast_x8);
    for (unsigned i = 1; i < len; i++) {
        bx1 = _mm256_srli_si256( bx1, 1 );
        bx2 = _mm256_srli_si256( bx2, 1 );
        bx4 = _mm256_srli_si256( bx4, 1 );
        bx8 = _mm256_srli_si256( bx8, 1 );

        multabs[i] =  _mm256_shuffle_epi8(bx1, broadcast_x1) ^ _mm256_shuffle_epi8(bx2, broadcast_x2)
                      ^ _mm256_shuffle_epi8(bx4, broadcast_x4) ^ _mm256_shuffle_epi8(bx8, broadcast_x8);
    }
}

static inline
void _gf16v_generate_multabs_avx2( __m256i *multabs, const uint8_t *v, unsigned n_ele ) {
    __m128i x[2];
    while (n_ele >= 32) {
        x[0] = _mm_loadu_si128( (__m128i *)v );
        gf16v_split_16to32_sse( x, x[0] );
        gf16v_generate_multab_16_avx2( multabs, x[0], 16 );
        gf16v_generate_multab_16_avx2( multabs + 16, x[1], 16 );
        multabs += 32;
        v += 16;
        n_ele -= 32;
    }
    if (n_ele) {
        x[0] = _load_xmm( v, (n_ele + 1) >> 1 );
        gf16v_split_16to32_sse( x, x[0] );
        if (n_ele <= 16) {
            gf16v_generate_multab_16_avx2( multabs, x[0], n_ele );
        } else {
            gf16v_generate_multab_16_avx2( multabs, x[0], 16 );
            gf16v_generate_multab_16_avx2( multabs + 16, x[1], n_ele - 16 );
        }
    }
}

static inline
void gf16v_generate_multabs_avx2( uint8_t *multabs, const uint8_t *v, unsigned n_ele ) {
    _gf16v_generate_multabs_avx2((__m256i *)multabs, v, n_ele);
}


#include "gf256_tabs.h"

static inline
void gf256v_generate_multab_16_avx2( __m256i *multabs, __m128i a, unsigned len ) {
    __m256i tab0 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 0));
    __m256i tab1 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 1));
    __m256i tab2 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 2));
    __m256i tab3 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 3));
    __m256i tab4 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 4));
    __m256i tab5 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 5));
    __m256i tab6 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 6));
    __m256i tab7 = _mm256_load_si256((__m256i const *) (__gf256_mulbase_avx + 32 * 7));
    __m256i mask_f = _mm256_set1_epi8(0xf);

    __m256i aa = _mm256_setr_m128i( a, a );
    __m256i a_lo = aa & mask_f;
    __m256i a_hi = _mm256_srli_epi16(aa, 4)&mask_f;
    __m256i bx1 =  _mm256_shuffle_epi8( tab0, a_lo) ^ _mm256_shuffle_epi8( tab1, a_hi);
    __m256i bx2 =  _mm256_shuffle_epi8( tab2, a_lo) ^ _mm256_shuffle_epi8( tab3, a_hi);
    __m256i bx4 =  _mm256_shuffle_epi8( tab4, a_lo) ^ _mm256_shuffle_epi8( tab5, a_hi);
    __m256i bx8 =  _mm256_shuffle_epi8( tab6, a_lo) ^ _mm256_shuffle_epi8( tab7, a_hi);

    __m256i broadcast_x1 = _mm256_set_epi8( 0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16 );
    __m256i broadcast_x2 = _mm256_set_epi8( 0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16 );
    __m256i broadcast_x4 = _mm256_set_epi8( 0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16 );
    __m256i broadcast_x8 = _mm256_set_epi8( 0, 0, 0, 0, 0, 0, 0, 0,  -16, -16, -16, -16, -16, -16, -16, -16,  0, 0, 0, 0, 0, 0, 0, 0,  -16, -16, -16, -16, -16, -16, -16, -16 );
    __m256i broadcast_x1_2 = _mm256_set_epi8( 1, -16, 1, -16, 1, -16, 1, -16,  1, -16, 1, -16, 1, -16, 1, -16,  1, -16, 1, -16, 1, -16, 1, -16,  1, -16, 1, -16, 1, -16, 1, -16 );
    __m256i broadcast_x2_2 = _mm256_set_epi8( 1, 1, -16, -16, 1, 1, -16, -16,  1, 1, -16, -16, 1, 1, -16, -16,  1, 1, -16, -16, 1, 1, -16, -16,  1, 1, -16, -16, 1, 1, -16, -16 );
    __m256i broadcast_x4_2 = _mm256_set_epi8( 1, 1, 1, 1, -16, -16, -16, -16,  1, 1, 1, 1, -16, -16, -16, -16,  1, 1, 1, 1, -16, -16, -16, -16,  1, 1, 1, 1, -16, -16, -16, -16 );
    __m256i broadcast_x8_2 = _mm256_set_epi8( 1, 1, 1, 1, 1, 1, 1, 1,  -16, -16, -16, -16, -16, -16, -16, -16,  1, 1, 1, 1, 1, 1, 1, 1,  -16, -16, -16, -16, -16, -16, -16, -16 );

    if ( 0 == (len & 1) ) {
        multabs[0] =  _mm256_shuffle_epi8(bx1, broadcast_x1) ^ _mm256_shuffle_epi8(bx2, broadcast_x2)
                      ^ _mm256_shuffle_epi8(bx4, broadcast_x4) ^ _mm256_shuffle_epi8(bx8, broadcast_x8);
        multabs[1] =  _mm256_shuffle_epi8(bx1, broadcast_x1_2) ^ _mm256_shuffle_epi8(bx2, broadcast_x2_2)
                      ^ _mm256_shuffle_epi8(bx4, broadcast_x4_2) ^ _mm256_shuffle_epi8(bx8, broadcast_x8_2);

        for (unsigned i = 2; i < len; i += 2) {
            bx1 = _mm256_srli_si256( bx1, 2 );
            bx2 = _mm256_srli_si256( bx2, 2 );
            bx4 = _mm256_srli_si256( bx4, 2 );
            bx8 = _mm256_srli_si256( bx8, 2 );
            multabs[i] =  _mm256_shuffle_epi8(bx1, broadcast_x1) ^ _mm256_shuffle_epi8(bx2, broadcast_x2)
                          ^ _mm256_shuffle_epi8(bx4, broadcast_x4) ^ _mm256_shuffle_epi8(bx8, broadcast_x8);
            multabs[i + 1] =  _mm256_shuffle_epi8(bx1, broadcast_x1_2) ^ _mm256_shuffle_epi8(bx2, broadcast_x2_2)
                              ^ _mm256_shuffle_epi8(bx4, broadcast_x4_2) ^ _mm256_shuffle_epi8(bx8, broadcast_x8_2);
        }
    } else {
        multabs[0] =  _mm256_shuffle_epi8(bx1, broadcast_x1) ^ _mm256_shuffle_epi8(bx2, broadcast_x2)
                      ^ _mm256_shuffle_epi8(bx4, broadcast_x4) ^ _mm256_shuffle_epi8(bx8, broadcast_x8);

        for (unsigned i = 1; i < len; i++) {
            bx1 = _mm256_srli_si256( bx1, 1 );
            bx2 = _mm256_srli_si256( bx2, 1 );
            bx4 = _mm256_srli_si256( bx4, 1 );
            bx8 = _mm256_srli_si256( bx8, 1 );

            multabs[i] =  _mm256_shuffle_epi8(bx1, broadcast_x1) ^ _mm256_shuffle_epi8(bx2, broadcast_x2)
                          ^ _mm256_shuffle_epi8(bx4, broadcast_x4) ^ _mm256_shuffle_epi8(bx8, broadcast_x8);
        }
    }
}



static inline
void gf256v_generate_multabs_avx2( uint8_t *multabs, const uint8_t *v, unsigned n_ele ) {
    __m128i x;
    while (n_ele >= 16) {
        x = _mm_loadu_si128( (__m128i *)v );
        gf256v_generate_multab_16_avx2( (__m256i *)multabs, x, 16 );
        multabs += 16 * 32;
        v += 16;
        n_ele -= 16;
    }
    if (n_ele) {
        x = _load_xmm( v, n_ele );
        gf256v_generate_multab_16_avx2( (__m256i *)multabs, x, n_ele );
    }
}






#endif // _BLAS_AVX2_H_

