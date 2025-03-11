// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_sse.h
/// @brief Inlined functions for implementing basic linear algebra functions for SSSE3 arch.
///

#ifndef _BLAS_SSE_H_
#define _BLAS_SSE_H_

#include "gf16.h"


#include "emmintrin.h"
#include "tmmintrin.h"

#include "config.h"

#include "gf16_sse.h"

#include "gf16_tabs.h"
#include "utils_malloc.h"

//#include "assert.h"








static inline
__m128i _load_xmm( const uint8_t *a, unsigned _num_byte ) {
    PQOV_ALIGN uint8_t temp[32];
    //assert( 16 >= _num_byte );
    //assert( 0 < _num_byte );
    for (unsigned i = 0; i < _num_byte; i++) {
        temp[i] = a[i];
    }
    return _mm_load_si128((__m128i *)temp);
}

static inline
void loadu_xmm( __m128i *xmm_a, const uint8_t *a, unsigned _num_byte ) {
    unsigned n_16 = (_num_byte >> 4);
    unsigned n_16_rem = _num_byte & 0xf;
    while ( n_16-- ) {
        xmm_a[0] = _mm_loadu_si128( (__m128i *)(a) );
        xmm_a++;
        a += 16;
    }
    if ( n_16_rem ) {
        xmm_a[0] = _load_xmm( a, n_16_rem );
    }
}

static inline
void _store_xmm( uint8_t *a, unsigned _num_byte, __m128i data ) {
    PQOV_ALIGN uint8_t temp[32];
    //assert( 16 >= _num_byte );
    //assert( 0 < _num_byte );
    _mm_store_si128((__m128i *)temp, data);
    for (unsigned i = 0; i < _num_byte; i++) {
        a[i] = temp[i];
    }
}

static inline
void storeu_xmm( uint8_t *a, unsigned _num_byte, __m128i *xmm_a ) {
    unsigned n_16 = (_num_byte >> 4);
    unsigned n_16_rem = _num_byte & 0xf;
    while ( n_16-- ) {
        _mm_storeu_si128( (__m128i *)a, xmm_a[0] );
        xmm_a++;
        a += 16;
    }
    if ( n_16_rem ) {
        _store_xmm( a, n_16_rem, xmm_a[0] );
    }
}







static inline
void linearmap_8x8_sse( uint8_t *a, __m128i ml, __m128i mh, __m128i mask, unsigned _num_byte ) {
    unsigned n_16 = _num_byte >> 4;
    unsigned rem = _num_byte & 15;
    while ( n_16--) {
        __m128i inp = _mm_loadu_si128( (__m128i *)(a) );
        __m128i r0 = linear_transform_8x8_128b( ml, mh, inp, mask );
        _mm_storeu_si128( (__m128i *)(a), r0 );
        a += 16;
    }
    if ( rem ) {
        __m128i inp = _load_xmm( a, rem );
        __m128i r0 = linear_transform_8x8_128b( ml, mh, inp, mask );
        _store_xmm( a, rem, r0 );
    }
}


static inline
void linearmap_8x8_accu_sse( uint8_t *accu_c, const uint8_t *a, __m128i ml, __m128i mh, __m128i mask, unsigned _num_byte ) {
    unsigned n_16 = _num_byte >> 4;
    unsigned rem = _num_byte & 15;
    while ( n_16-- ) {
        __m128i inp = _mm_loadu_si128( (__m128i *)(a) );
        __m128i out = _mm_loadu_si128( (__m128i *)(accu_c) );
        __m128i r0 = linear_transform_8x8_128b( ml, mh, inp, mask );
        r0 ^= out;
        _mm_storeu_si128( (__m128i *)(accu_c), r0 );
        a += 16;
        accu_c += 16;
    }
    if ( rem ) {
        __m128i inp = _load_xmm( a, rem );
        __m128i out = _load_xmm( accu_c, rem );
        __m128i r0 = linear_transform_8x8_128b( ml, mh, inp, mask );
        r0 ^= out;
        _store_xmm( accu_c, rem, r0 );
    }
}





//////////////////////   basic functions  ///////////////////////////////////////////////




static inline
void gf256v_add_sse( uint8_t *accu_b, const uint8_t *a, unsigned _num_byte ) {
    while ( _num_byte >= 16 ) {
        _mm_storeu_si128( (__m128i *) (accu_b), _mm_loadu_si128((__m128i *)(a))^_mm_loadu_si128((__m128i *)(accu_b)) );
        a += 16;
        accu_b += 16;
        _num_byte -= 16;
    }
    for (unsigned j = 0; j < _num_byte; j++) {
        accu_b[j] ^= a[j];
    }
}




///////////////////////////////





static inline
void gf16v_mul_scalar_sse( uint8_t *a, uint8_t gf16_b, unsigned _num_byte ) {
    //unsigned b = gf16_b&0xf;
    //__m128i ml = _mm_load_si128( (__m128i*) (__gf16_mul + 32*b) );
    __m128i ml = tbl_gf16_multab( gf16_b & 0xf );
    __m128i mh = _mm_slli_epi16( ml, 4 );
    __m128i mask = _mm_set1_epi8(0xf);

    linearmap_8x8_sse( a, ml, mh, mask, _num_byte );
}



static inline
void gf16v_madd_multab_sse( uint8_t *accu_c, const uint8_t *a, const uint8_t *multab, unsigned _num_byte ) {
    __m128i ml = _mm_load_si128( (__m128i *) multab );
    __m128i mh = _mm_slli_epi16( ml, 4 );
    __m128i mask = _mm_set1_epi8(0xf);

    linearmap_8x8_accu_sse( accu_c, a, ml, mh, mask, _num_byte );
}


static inline
void gf16v_madd_sse( uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned _num_byte ) {
    //unsigned b = gf16_b&0xf;
    //__m128i ml = _mm_load_si128( (__m128i*) (__gf16_mul + 32*b) );
    __m128i ml = tbl_gf16_multab( gf16_b & 0xf );
    __m128i mh = _mm_slli_epi16( ml, 4 );
    __m128i mask = _mm_set1_epi8(0xf);

    linearmap_8x8_accu_sse( accu_c, a, ml, mh, mask, _num_byte );
}


///////////////////////////


static inline
void gf256v_mul_scalar_sse( uint8_t *a, uint8_t _b, unsigned _num_byte ) {
    //unsigned b = _b;
    //__m128i ml = _mm_load_si128( (__m128i*) (__gf256_mul + 32*b) );
    //__m128i mh = _mm_load_si128( (__m128i*) (__gf256_mul + 32*b + 16) );
    __m128ix2_t tab = tbl_gf256_multab(_b);
    __m128i mask = _mm_set1_epi8(0xf);

    linearmap_8x8_sse( a, tab.v0, tab.v1, mask, _num_byte );
}


static inline
void gf256v_madd_multab_sse( uint8_t *accu_c, const uint8_t *a, const uint8_t *multab, unsigned _num_byte ) {
    __m128i ml = _mm_load_si128( (__m128i *) multab );
    __m128i mh = _mm_load_si128( (__m128i *) (multab + 16) );
    __m128i mask = _mm_set1_epi8(0xf);

    linearmap_8x8_accu_sse( accu_c, a, ml, mh, mask, _num_byte );
}

static inline
void gf256v_madd_sse( uint8_t *accu_c, const uint8_t *a, uint8_t _b, unsigned _num_byte ) {
    //unsigned b = _b;
    //__m128i ml = _mm_load_si128( (__m128i*) (__gf256_mul + 32*b) );
    //__m128i mh = _mm_load_si128( (__m128i*) (__gf256_mul + 32*b + 16) );
    __m128ix2_t tab = tbl_gf256_multab(_b);
    __m128i mask = _mm_set1_epi8(0xf);

    linearmap_8x8_accu_sse( accu_c, a, tab.v0, tab.v1, mask, _num_byte );
}










/////////////////////////////////////////////////////////////////////////////////


// input a:          0x12 0x34 0x56 0x78 ......
// output x_align:   0x02 0x01 0x04 0x03 0x06 0x05 .........
static inline
void gf16v_split_16to32_sse( __m128i *x_align, __m128i a ) {
    __m128i mask_f = _mm_set1_epi8(0xf);
    __m128i al = a & mask_f;
    __m128i ah = _mm_srli_epi16( a, 4 )&mask_f;

    __m128i a0 = _mm_unpacklo_epi8( al, ah );
    __m128i a1 = _mm_unpackhi_epi8( al, ah );

    _mm_store_si128( x_align, a0 );
    _mm_store_si128( x_align + 1, a1 );
}

typedef struct __xmm_x2 {
    __m128i v0;
    __m128i v1;
} xmmx2_t;
static inline
xmmx2_t gf16v_split_16to32_sse2( __m128i a ) {
    __m128i mask_f = _mm_set1_epi8(0xf);
    __m128i al = a & mask_f;
    __m128i ah = _mm_srli_epi16( a, 4 )&mask_f;
    xmmx2_t r;
    r.v0 = _mm_unpacklo_epi8( al, ah );
    r.v1 = _mm_unpackhi_epi8( al, ah );
    return r;
}


//////////////////   Generating multiplication tables  /////////////////////////////////////






static inline
void gf16v_generate_multab_16_sse( __m128i *multabs, __m128i a, unsigned len ) {
    __m128i broadcast_x1 = _mm_set_epi8( 0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16 );
    __m128i broadcast_x2 = _mm_set_epi8( 0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16 );
    __m128i broadcast_x4 = _mm_set_epi8( 0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16 );
    __m128i broadcast_x8 = _mm_set_epi8( 0, 0, 0, 0, 0, 0, 0, 0,  -16, -16, -16, -16, -16, -16, -16, -16 );

    __m128i bx1 = a;
    __m128i bx2 = _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 )), bx1);
    __m128i bx4 = _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 * 2 )), bx1);
    __m128i bx8 = _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf16_mulbase + 32 * 3 )), bx1);


    multabs[0] =  _mm_shuffle_epi8(bx1, broadcast_x1) ^ _mm_shuffle_epi8(bx2, broadcast_x2)
                  ^ _mm_shuffle_epi8(bx4, broadcast_x4) ^ _mm_shuffle_epi8(bx8, broadcast_x8);
    multabs[1] = _mm_slli_epi16(multabs[0], 4);
    for (unsigned i = 1; i < len; i++) {
        bx1 = _mm_srli_si128( bx1, 1 );
        bx2 = _mm_srli_si128( bx2, 1 );
        bx4 = _mm_srli_si128( bx4, 1 );
        bx8 = _mm_srli_si128( bx8, 1 );

        multabs[i * 2] =  _mm_shuffle_epi8(bx1, broadcast_x1) ^ _mm_shuffle_epi8(bx2, broadcast_x2)
                          ^ _mm_shuffle_epi8(bx4, broadcast_x4) ^ _mm_shuffle_epi8(bx8, broadcast_x8);
        multabs[i * 2 + 1] = _mm_slli_epi16(multabs[i * 2], 4);
    }
}



static inline
void gf16v_generate_multabs_sse( uint8_t *multabs, const uint8_t *v, unsigned n_ele ) {
    __m128i x[2];
    while (n_ele >= 32) {
        x[0] = _mm_loadu_si128( (__m128i *)v );
        gf16v_split_16to32_sse( x, x[0] );
        gf16v_generate_multab_16_sse( (__m128i *)multabs, x[0], 16 );
        gf16v_generate_multab_16_sse( (__m128i *)(multabs + 16 * 32), x[1], 16 );
        multabs += 32 * 32;
        v += 16;
        n_ele -= 32;
    }
    if (n_ele) {
        x[0] = _load_xmm( v, (n_ele + 1) >> 1 );
        gf16v_split_16to32_sse( x, x[0] );
        if (n_ele <= 16) {
            gf16v_generate_multab_16_sse( (__m128i *)multabs, x[0], n_ele );
        } else {
            gf16v_generate_multab_16_sse( (__m128i *)multabs, x[0], 16 );
            gf16v_generate_multab_16_sse( (__m128i *)(multabs + 16 * 32), x[1], n_ele - 16 );
        }
    }
}



static inline
void gf256v_generate_multab_16_sse( __m128i *multabs, __m128i a, unsigned len ) {
    __m128i broadcast_x1 = _mm_set_epi8( 0, -16, 0, -16, 0, -16, 0, -16,  0, -16, 0, -16, 0, -16, 0, -16 );
    __m128i broadcast_x2 = _mm_set_epi8( 0, 0, -16, -16, 0, 0, -16, -16,  0, 0, -16, -16, 0, 0, -16, -16 );
    __m128i broadcast_x4 = _mm_set_epi8( 0, 0, 0, 0, -16, -16, -16, -16,  0, 0, 0, 0, -16, -16, -16, -16 );
    __m128i broadcast_x8 = _mm_set_epi8( 0, 0, 0, 0, 0, 0, 0, 0,  -16, -16, -16, -16, -16, -16, -16, -16 );
    __m128i mask_f = _mm_set1_epi8( 0xf );

    __m128i a_lo = a & mask_f;
    __m128i a_hi = _mm_srli_epi16(a, 4)&mask_f;

    __m128i bx1 = a;
    __m128i bx2 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 1   )), a_lo)
                   ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 1 + 16)), a_hi);
    __m128i bx4 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 2   )), a_lo)
                   ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 2 + 16)), a_hi);
    __m128i bx8 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 3   )), a_lo)
                   ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 3 + 16)), a_hi);

    __m128i bx10 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 4   )), a_lo)
                    ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 4 + 16)), a_hi);
    __m128i bx20 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 5   )), a_lo)
                    ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 5 + 16)), a_hi);
    __m128i bx40 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 6   )), a_lo)
                    ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 6 + 16)), a_hi);
    __m128i bx80 =  _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 7   )), a_lo)
                    ^ _mm_shuffle_epi8( _mm_load_si128((__m128i const *) (__gf256_mulbase + 32 * 7 + 16)), a_hi);

    multabs[0] =  _mm_shuffle_epi8(bx1, broadcast_x1) ^ _mm_shuffle_epi8(bx2, broadcast_x2)
                  ^ _mm_shuffle_epi8(bx4, broadcast_x4) ^ _mm_shuffle_epi8(bx8, broadcast_x8);
    multabs[1] =  _mm_shuffle_epi8(bx10, broadcast_x1) ^ _mm_shuffle_epi8(bx20, broadcast_x2)
                  ^ _mm_shuffle_epi8(bx40, broadcast_x4) ^ _mm_shuffle_epi8(bx80, broadcast_x8);

    __m128i mask_1 = _mm_set1_epi8( 1 );
    for (unsigned i = 1; i < len; i++) {
        broadcast_x1 = _mm_add_epi8( broadcast_x1, mask_1 );
        broadcast_x2 = _mm_add_epi8( broadcast_x2, mask_1 );
        broadcast_x4 = _mm_add_epi8( broadcast_x4, mask_1 );
        broadcast_x8 = _mm_add_epi8( broadcast_x8, mask_1 );

        multabs[i * 2 + 0] =  _mm_shuffle_epi8(bx1, broadcast_x1) ^ _mm_shuffle_epi8(bx2, broadcast_x2)
                              ^ _mm_shuffle_epi8(bx4, broadcast_x4) ^ _mm_shuffle_epi8(bx8, broadcast_x8);
        multabs[i * 2 + 1] =  _mm_shuffle_epi8(bx10, broadcast_x1) ^ _mm_shuffle_epi8(bx20, broadcast_x2)
                              ^ _mm_shuffle_epi8(bx40, broadcast_x4) ^ _mm_shuffle_epi8(bx80, broadcast_x8);
    }
}


static inline
void gf256v_generate_multabs_sse( uint8_t *multabs, const uint8_t *v, unsigned n_ele ) {
    __m128i x;
    while (n_ele >= 16) {
        x = _mm_loadu_si128( (__m128i *)v );
        gf256v_generate_multab_16_sse( (__m128i *)multabs, x, 16 );
        multabs += 16 * 32;
        v += 16;
        n_ele -= 16;
    }
    if (n_ele) {
        x = _load_xmm( v, n_ele );
        gf256v_generate_multab_16_sse( (__m128i *)multabs, x, n_ele );
    }
}




#endif // _BLAS_SSE_H_

