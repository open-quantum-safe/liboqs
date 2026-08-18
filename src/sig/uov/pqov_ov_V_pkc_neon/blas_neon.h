// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_neon.h
/// @brief Inlined functions for implementing basic linear algebra functions for NEON.
///

#ifndef _BLAS_NEON_H_
#define _BLAS_NEON_H_

#include "gf16.h"

#include "config.h"

#include "gf16_neon.h"

#include <arm_neon.h>
//#include "assert.h"




static inline
uint8x16_t _load_Qreg( const uint8_t *v, unsigned n ) {
    //uint8_t temp[16];
    uint8_t temp[16] = {0}; // XXX: for gcc-12 warning: 'temp' may be used uninitialized
    for (unsigned i = 0; i < n; i++) {
        temp[i] = v[i];
    }
    return vld1q_u8( temp );
}

static inline
void load_Qregs( uint8x16_t *r, const uint8_t *v, unsigned n ) {
    while ( n >= 16 ) {
        r[0] = vld1q_u8( v );
        r++;
        v += 16;
        n -= 16;
    }
    if (n) {
        r[0] = _load_Qreg(v, n);
    }
}

static inline
void _store_Qreg( uint8_t *v, unsigned n, uint8x16_t a ) {
    uint8_t temp[16];
    vst1q_u8( temp, a );
    for (unsigned i = 0; i < n; i++) {
        v[i] = temp[i];
    }
}

static inline
void store_Qregs( uint8_t *v, unsigned n, const uint8x16_t *r ) {
    while ( n >= 16 ) {
        vst1q_u8( v, r[0] );
        r++;
        v += 16;
        n -= 16;
    }
    if (n) {
        _store_Qreg(v, n, r[0]);
    }
}



//////////////////////   basic functions  ///////////////////////////////////////////////

static inline
void gf256v_add_neon( uint8_t *accu_b, const uint8_t *a, unsigned _num_byte ) {
    if( _num_byte & 15 ) {
        unsigned len = _num_byte & 15;
        if (_num_byte < 16 ) {
            while ( _num_byte-- ) {
                *accu_b ^= *a;
                accu_b++;
                a++;
            }
            return;
        } else {
            uint8x16_t b0 = vld1q_u8(accu_b);
            uint8x16_t b1 = vld1q_u8(accu_b+len);
            vst1q_u8( accu_b, vld1q_u8(a)^b0 );
            vst1q_u8( accu_b+len, b1 );
            _num_byte -= len;
            a += len;
            accu_b += len;
        }
    }
    while ( _num_byte >= 48 ) {
        uint8x16_t a0 = vld1q_u8(a);
        uint8x16_t a1 = vld1q_u8(a + 16);
        uint8x16_t a2 = vld1q_u8(a + 32);
        uint8x16_t b0 = vld1q_u8(accu_b);
        uint8x16_t b1 = vld1q_u8(accu_b + 16);
        uint8x16_t b2 = vld1q_u8(accu_b + 32);
        vst1q_u8( accu_b, a0 ^ b0 );
        vst1q_u8( accu_b + 16, a1 ^ b1 );
        vst1q_u8( accu_b + 32, a2 ^ b2 );
        _num_byte -= 48;
        a += 48;
        accu_b += 48;
    }
    while ( _num_byte >= 16 ) {
        vst1q_u8( accu_b, vld1q_u8(a)^vld1q_u8(accu_b) );
        _num_byte -= 16;
        a += 16;
        accu_b += 16;
    }
}

static inline
void gf256v_conditional_add_neon( uint8_t *accu_b, uint8_t predicate, const uint8_t *a, unsigned _num_byte ) {
    uint8x16_t mm = vdupq_n_u8(-predicate);
    while ( _num_byte >= 16 ) {
        vst1q_u8( accu_b, (mm & vld1q_u8(a))^vld1q_u8(accu_b) );
        _num_byte -= 16;
        a += 16;
        accu_b += 16;
    }
    //for(unsigned j=0;j<_num_byte;j++) { accu_b[j] ^= a[j]*predicate; }
    while ( _num_byte-- ) {
        *accu_b ^= a[0] * predicate;
        accu_b++;
        a++;
    }
}


static inline
void gf256v_set_zero_neon( uint8_t *vec, unsigned _num_byte ) {
    uint8x16_t zero = vdupq_n_u8(0);
    while (_num_byte >= 16) {
        vst1q_u8( vec, zero );
        _num_byte -= 16;
        vec += 16;
    }
    while (_num_byte--) {
        *vec = 0;
        vec++;
    }
}

///////////////////////////////


static inline
void gf16v_mul_scalar_neon( uint8_t *a, uint8_t gf16_b, unsigned _num_byte ) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t bp = vdupq_n_u8(gf16_b & 0xf);
    #if defined(_GF16_REDUCE_WITH_TBL_)
    uint8x16_t mask_3 = vld1q_u8(__gf16_reduce);
    #else
    uint8x16_t mask_3 = vdupq_n_u8( 3 );
    #endif
    if (_num_byte&15) {
        unsigned len = _num_byte & 15;
        if ( _num_byte < 16 ){
            uint8x16_t aa = _load_Qreg(a,len);
            uint8x16_t ab = _gf16v_mul_neon(aa,bp,mask_f,mask_3);
            _store_Qreg(a, len, ab);
            return;
        } else {
            uint8x16_t a0 = vld1q_u8(a);
            uint8x16_t a1 = vld1q_u8(a+len);
            uint8x16_t ab = _gf16v_mul_neon(a0,bp,mask_f,mask_3);
            vst1q_u8(a,ab);
            vst1q_u8(a+len,a1);
            _num_byte -= len;
            a += len;
        }
    }
    while ( _num_byte ) {
        uint8x16_t aa = vld1q_u8(a);
        vst1q_u8( a, _gf16v_mul_neon(aa, bp, mask_f, mask_3) );
        _num_byte -= 16;
        a += 16;
    }
}



static inline
void gf16v_madd_neon( uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned _num_byte ) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t bp = vdupq_n_u8(gf16_b & 0xf);
    #if defined(_GF16_REDUCE_WITH_TBL_)
    uint8x16_t mask_3 = vld1q_u8(__gf16_reduce);
    #else
    uint8x16_t mask_3 = vdupq_n_u8( 3 );
    #endif
    if (_num_byte&15) {
        unsigned len = _num_byte & 15;
        if ( _num_byte < 16 ){
            uint8x16_t aa = _load_Qreg(a,len);
            uint8x16_t cc = _load_Qreg(accu_c,len);
            uint8x16_t ab = cc ^ _gf16v_mul_neon(aa,bp,mask_f,mask_3);
            _store_Qreg(accu_c, len, ab);
            return;
        } else {
            uint8x16_t a0 = vld1q_u8(a);
            uint8x16_t c0 = vld1q_u8(accu_c);
            uint8x16_t c1 = vld1q_u8(accu_c+len);
            uint8x16_t ab = c0 ^ _gf16v_mul_neon(a0,bp,mask_f,mask_3);
            vst1q_u8(accu_c,ab);
            vst1q_u8(accu_c+len,c1);
            _num_byte -= len;
            a += len;
            accu_c += len;
        }
    }
    while ( _num_byte ) {
        uint8x16_t aa = vld1q_u8(a);
        uint8x16_t cc = vld1q_u8(accu_c);
        vst1q_u8( accu_c, cc ^ _gf16v_mul_neon(aa, bp, mask_f, mask_3) );
        _num_byte -= 16;
        a += 16;
        accu_c += 16;
    }
}



static inline
void gf16v_madd_multab_neon( uint8_t *accu_c, const uint8_t *a, const uint8_t *multab, unsigned _num_byte ) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tbl = vld1q_u8( multab );

    if (_num_byte&15) {
        unsigned len = _num_byte & 15;
        if ( _num_byte < 16 ){
            uint8x16_t aa = _load_Qreg( a, _num_byte );
            uint8x16_t cc = _load_Qreg( accu_c, _num_byte );
            _store_Qreg( accu_c, _num_byte, cc ^ _gf16_tbl_x2(aa, tbl, mask_f) );
            return;
        } else {
            uint8x16_t aa = vld1q_u8(a);
            uint8x16_t cc = vld1q_u8(accu_c);
            uint8x16_t c1 = vld1q_u8(accu_c+len);
            vst1q_u8(accu_c, cc ^ _gf16_tbl_x2(aa, tbl, mask_f) );
            vst1q_u8(accu_c+len,c1);
            _num_byte -= len;
            a += len;
            accu_c += len;
        }
    }
    while ( _num_byte ) {
        uint8x16_t aa = vld1q_u8(a);
        uint8x16_t cc = vld1q_u8(accu_c);
        vst1q_u8( accu_c, cc ^ _gf16_tbl_x2(aa, tbl, mask_f) );
        _num_byte -= 16;
        a += 16;
        accu_c += 16;
    }
}



/////////////////////   GF(256)  ///////////////////

static inline
uint8x16_t _gf256_tbl( uint8x16_t a, uint8x16_t tbl0, uint8x16_t tbl1, uint8x16_t mask_f ) {
    return vqtbl1q_u8( tbl0, a & mask_f ) ^ vqtbl1q_u8( tbl1, vshrq_n_u8( a, 4 ));
}

// WARNING: Assuming _num_byte >= 16
static inline
void _gf256v_madd_multab_neon( uint8_t *accu_c, const uint8_t *a,
                               uint8x16_t tbl0, uint8x16_t tbl1, unsigned _num_byte, uint8x16_t mask_f ) {
    if (15 & _num_byte) {
        unsigned len = 15 & _num_byte;
        uint8x16_t aa = vld1q_u8(a);
        uint8x16_t cc = vld1q_u8(accu_c);
        uint8x16_t c1 = vld1q_u8(accu_c+len);
        vst1q_u8( accu_c, cc ^ _gf256_tbl(aa, tbl0, tbl1, mask_f) );
        vst1q_u8( accu_c + len , c1 );
        _num_byte -= len;
        accu_c += len;
        a += len;
    }
    while ( _num_byte >= 48 ) {
        uint8x16_t a0 = vld1q_u8(a);
        uint8x16_t a1 = vld1q_u8(a + 16);
        uint8x16_t a2 = vld1q_u8(a + 32);
        uint8x16_t c0 = vld1q_u8(accu_c);
        uint8x16_t c1 = vld1q_u8(accu_c + 16);
        uint8x16_t c2 = vld1q_u8(accu_c + 32);
        vst1q_u8( accu_c, c0 ^ _gf256_tbl(a0, tbl0, tbl1, mask_f) );
        vst1q_u8( accu_c + 16, c1 ^ _gf256_tbl(a1, tbl0, tbl1, mask_f) );
        vst1q_u8( accu_c + 32, c2 ^ _gf256_tbl(a2, tbl0, tbl1, mask_f) );
        _num_byte -= 48;
        a += 48;
        accu_c += 48;
    }
    while ( _num_byte ) {
        uint8x16_t aa = vld1q_u8(a);
        uint8x16_t cc = vld1q_u8(accu_c);
        vst1q_u8( accu_c, cc ^ _gf256_tbl(aa, tbl0, tbl1, mask_f) );
        _num_byte -= 16;
        a += 16;
        accu_c += 16;
    }
}

// WARNING: Assuming _num_byte >= 16
static inline
void _gf256v_mul_scalar_multab_neon( uint8_t *c, uint8x16_t tbl0, uint8x16_t tbl1, unsigned _num_byte, uint8x16_t mask_f ) {
    if (15 & _num_byte) {
        unsigned len = 15 & _num_byte;
        uint8x16_t cc = vld1q_u8( c );
        uint8x16_t c1 = vld1q_u8( c+len );
        vst1q_u8( c, _gf256_tbl(cc, tbl0, tbl1, mask_f) );
        vst1q_u8( c + len , c1 );
        _num_byte -= len;
        c += len;
    }
    while ( _num_byte >= 48 ) {
        uint8x16_t c0 = vld1q_u8(c);
        uint8x16_t c1 = vld1q_u8(c + 16);
        uint8x16_t c2 = vld1q_u8(c + 32);
        uint8x16_t r0 = _gf256_tbl(c0, tbl0, tbl1, mask_f);
        uint8x16_t r1 = _gf256_tbl(c1, tbl0, tbl1, mask_f);
        uint8x16_t r2 = _gf256_tbl(c2, tbl0, tbl1, mask_f);
        vst1q_u8( c, r0 );
        vst1q_u8( c + 16, r1 );
        vst1q_u8( c + 32, r2 );
        _num_byte -= 48;
        c += 48;
    }
    while ( _num_byte ) {
        uint8x16_t cc = vld1q_u8(c);
        vst1q_u8( c, _gf256_tbl(cc, tbl0, tbl1, mask_f) );
        _num_byte -= 16;
        c += 16;
    }
}

static inline
void gf256v_mul_scalar_neon( uint8_t *a, uint8_t b, unsigned _num_byte ) {
    if ( _num_byte < 16 ) {
        uint8x16_t aa = _load_Qreg( a , _num_byte );
        uint8x16_t rr = gf256v_mul_neon( aa , b );
        _store_Qreg( a , _num_byte , rr );
    } else {
        uint8x16_t mask_f = vdupq_n_u8( 0xf );
        uint8x16x2_t t = gf256v_get_multab_neon(b);
        uint8x16_t tbl0 = t.val[0];
        uint8x16_t tbl1 = t.val[1];
        _gf256v_mul_scalar_multab_neon( a, tbl0, tbl1, _num_byte, mask_f );
    }
}

static inline
void gf256v_madd_neon( uint8_t *accu_c, const uint8_t *a, uint8_t b, unsigned _num_byte ) {
    if ( _num_byte < 16 ) {
        uint8x16_t aa = _load_Qreg( a , _num_byte );
        uint8x16_t cc = _load_Qreg( accu_c , _num_byte );
        uint8x16_t rr = cc ^ gf256v_mul_neon( aa , b );
        _store_Qreg( accu_c , _num_byte , rr );
    } else {
        uint8x16_t mask_f = vdupq_n_u8( 0xf );
        uint8x16x2_t t = gf256v_get_multab_neon(b);
        uint8x16_t tbl0 = t.val[0];
        uint8x16_t tbl1 = t.val[1];
        _gf256v_madd_multab_neon( accu_c, a, tbl0, tbl1, _num_byte, mask_f );
    }
}



///////////////////////



static inline
void gf16v_generate_multabs_neon( uint8_t *multabs, const uint8_t *v, unsigned n_ele ) {
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    uint8x16_t tab_0_f = vld1q_u8(__0_f);

    while ( 2 <= n_ele ) {
        uint8x16_t b0 = vdupq_n_u8(v[0] & 0xf);
        uint8x16_t b1 = vdupq_n_u8(v[0] >> 4);
        uint8x16_t mm0 = _gf16v_get_multab_neon(b0, tab_reduce, tab_0_f);
        uint8x16_t mm1 = _gf16v_get_multab_neon(b1, tab_reduce, tab_0_f);
        vst1q_u8( multabs, mm0 );
        multabs += 16;
        vst1q_u8( multabs, mm1 );
        multabs += 16;
        n_ele -= 2;
        v++;
    }
    if ( n_ele ) {
        uint8x16_t b0 = vdupq_n_u8(v[0] & 0xf);
        uint8x16_t mm0 = _gf16v_get_multab_neon(b0, tab_reduce, tab_0_f);
        vst1q_u8( multabs, mm0 );
    }
}


static inline
void gf256v_generate_multabs_neon( uint8_t *multabs, const uint8_t *v, unsigned n_ele ) {
    uint8x16_t tab_0_f = vld1q_u8(__0_f);
    uint8x16_t tab_rd0 = vld1q_u8(__gf256_bit8_11_reduce);
    uint8x16_t mask_f  = vdupq_n_u8(0xf);
    #if !defined(_APPLE_SILICON_)
    uint8x16_t tab_rd1 = vld1q_u8(__gf256_bit12_15_reduce);
    #endif

    while (n_ele) {
        #ifdef _APPLE_SILICON_
        uint8x16x2_t tab = _gf256v_get_multab_neon( v[0], mask_f, tab_0_f, tab_rd0 );
        #else
        uint8x16x2_t tab = _gf256v_get_multab_neon( v[0], mask_f, tab_0_f, tab_rd0, tab_rd1 );
        #endif
        vst1q_u8(multabs, tab.val[0]);
        multabs += 16;
        vst1q_u8(multabs, tab.val[1]);
        multabs += 16;
        v++;
        n_ele--;
    }
}



#endif // _BLAS_NEON_H_

