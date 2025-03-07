// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf16_neon.h
/// @brief Inlined functions for implementing GF arithmetics for NEON instruction sets.
///

#ifndef _GF16_NEON_H_
#define _GF16_NEON_H_


#include "gf16_tabs_neon.h"
#include "stdint.h"


#include <arm_neon.h>


static inline uint8x16_t clmul_8x8( uint8x16_t a , uint8x16_t b ) { return vreinterpretq_u8_p8( vmulq_p8( vreinterpretq_p8_u8(a) , vreinterpretq_p8_u8(b) ) ); }

//////////////  GF(16)  /////////////////////////////


static inline
uint8_t gf16_inv_neon(uint8_t a) {
    uint8x16_t tinv = vld1q_u8( __gf16_inv );
    uint8x8_t aa  = vdup_n_u8( a );
    uint8x8_t r   = vqtbl1_u8( tinv, aa );
    return vget_lane_u8(r, 0);
}

// gf16 := gf2[x]/(x^4+x+1)
static inline
uint8x16_t _gf16v_reduce_tbl_neon( uint8x16_t abl, uint8x16_t abh, uint8x16_t tab_reduce ) {
    uint8x16_t rl = abl ^ vqtbl1q_u8( tab_reduce, vshrq_n_u8(abl, 4) );
    uint8x16_t rh = abh ^ vqtbl1q_u8( tab_reduce, vshrq_n_u8(abh, 4) );

    return vsliq_n_u8( rl, rh, 4 );
}

// gf16 := gf2[x]/(x^4+x+1)
static inline
uint8x16_t _gf16v_reduce_pmul_neon( uint8x16_t abl, uint8x16_t abh, uint8x16_t mask_3 ) {
    uint8x16_t rl = abl ^ clmul_8x8( vshrq_n_u8(abl, 4), mask_3 );
    uint8x16_t rh = abh ^ clmul_8x8( vshrq_n_u8(abh, 4), mask_3 );

    return vsliq_n_u8( rl, rh, 4 );
}

// Favor VTBL more than PMUL:
// For cortex-A72, VTBL  (Latency:3,Throughput:2) is faster than PMUL(L:5,T:0.5)
//

#define _GF16_REDUCE_WITH_TBL_

static inline
uint8x16_t _gf16v_mul_neon( uint8x16_t a, uint8x16_t bp, uint8x16_t mask_f, uint8x16_t mask_3 ) {
    uint8x16_t al0 = a & mask_f;
    uint8x16_t ah0 = vshrq_n_u8( a, 4 );
// mul
    uint8x16_t abl = clmul_8x8( al0, bp );
    uint8x16_t abh = clmul_8x8( ah0, bp );
// reduce
    #if defined(_GF16_REDUCE_WITH_TBL_)
    return _gf16v_reduce_tbl_neon( abl, abh, mask_3 );
    #else
    return _gf16v_reduce_pmul_neon( abl, abh, mask_3 );
    #endif
}

static inline
uint8x16_t gf16v_mul_neon( uint8x16_t a, uint8_t b ) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t bp = vdupq_n_u8(b);
    #if defined( _GF16_REDUCE_WITH_TBL_)
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    #else
    //uint8x16_t mask_3 = vdupq_n_u8( 3 );
    uint8x16_t tab_reduce = vdupq_n_u8( 3 );
    #endif
    return _gf16v_mul_neon( a, bp, mask_f, tab_reduce );
}

static inline
uint8x16_t _gf16v_mul_unpack_neon( uint8x16_t a0, uint8x16_t b0, uint8x16_t tab_reduce ) {
    uint8x16_t ab = clmul_8x8( a0, b0 );
    return ab ^ vqtbl1q_u8( tab_reduce, vshrq_n_u8(ab, 4) );
}

static inline
uint8x16_t _gf16v_get_multab_neon( uint8x16_t b, uint8x16_t tab_reduce, uint8x16_t tab_0_f ) {
    return _gf16v_mul_unpack_neon(b, tab_0_f, tab_reduce);
}

static inline
uint8x16_t gf16v_get_multab_neon( uint8_t b ) {
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    uint8x16_t tab_0_f = vld1q_u8(__0_f);
    uint8x16_t bb = vdupq_n_u8(b);
    return _gf16v_get_multab_neon(bb, tab_reduce, tab_0_f);
}

static inline
uint8x16_t _gf16_tbl_x2( uint8x16_t a, uint8x16_t tbl, uint8x16_t mask_f ) {
    return vsliq_n_u8( vqtbl1q_u8( tbl, a & mask_f ), vqtbl1q_u8( tbl, vshrq_n_u8( a, 4 ) ), 4 );
}




/////////////////////////////  GF(256) ////////////////////////////////////////


// 0x11b GF(256).  0x1b = 3 + (3<<3)
static inline
uint8x16_t _gf256v_reduce_pmul_neon( uint8x16_t ab0, uint8x16_t ab1, uint8x16_t mask_3, uint8x16_t mask_0x1b ) {
    uint8x16_t abl = vuzp1q_u8(ab0, ab1);
    uint8x16_t abh = vuzp2q_u8(ab0, ab1);
// reduce
    uint8x16_t abhx3 = clmul_8x8( abh, mask_3 );
    uint8x16_t r = abl ^ abhx3 ^ vshlq_n_u8( abhx3, 3 ) ^ clmul_8x8( mask_0x1b, vshrq_n_u8( abhx3, 5 ) );

    return r;
}


static inline
uint8x16_t _gf256v_reduce_tbl_neon( uint8x16_t ab0, uint8x16_t ab1, uint8x16_t mask_f, uint8x16_t tab_rd0, uint8x16_t tab_rd1 ) {
    uint8x16_t abl = vuzp1q_u8(ab0, ab1);
    uint8x16_t abh = vuzp2q_u8(ab0, ab1);
// reduce
    return abl ^ vqtbl1q_u8( tab_rd0, abh & mask_f ) ^ vqtbl1q_u8( tab_rd1, vshrq_n_u8(abh, 4) );
}


#define _GF256_REDUCE_WITH_TBL_


#if defined(_GF256_REDUCE_WITH_TBL_)

// 0x11b GF(256).  0x1b = 3 + (3<<3)
static inline
uint8x16_t _gf256v_mul_neon( uint8x16_t _a, uint8x16_t _b, uint8x16_t mask_f, uint8x16_t tab_rd0, uint8x16_t tab_rd1 ) {
    poly8x16_t a = vreinterpretq_p8_u8(_a);
    poly8x16_t b = vreinterpretq_p8_u8(_b);
    poly16x8_t ab0 = vmull_p8( vget_low_p8(a), vget_low_p8(b) );
    poly16x8_t ab1 = vmull_high_p8( a, b );
    return _gf256v_reduce_tbl_neon( vreinterpretq_u8_p16(ab0), vreinterpretq_u8_p16(ab1), mask_f, tab_rd0, tab_rd1 );
}

static inline
uint8x16_t gf256v_mul_neon( uint8x16_t a, uint8_t b ) {
    uint8x16_t bb = vdupq_n_u8(b);
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tab_rd0 = vld1q_u8(__gf256_bit8_11_reduce);
    uint8x16_t tab_rd1 = vld1q_u8(__gf256_bit12_15_reduce);
    return _gf256v_mul_neon( a, bb, mask_f, tab_rd0, tab_rd1 );
}

#else

// 0x11b GF(256).  0x1b = 3 + (3<<3)
static inline
uint8x16_t _gf256v_mul_neon( uint8x16_t a, uint8x16_t b, uint8x16_t mask_3, uint8x16_t mask_0x1b ) {
// mul
    poly16x8_t ab0 = vmull_p8( vget_low_p8(a), vget_low_p8(b) );
    poly16x8_t ab1 = vmull_high_p8( a, b );

    return _gf256v_reduce_pmul_neon( ab0, ab1, mask_3, mask_0x1b );
}

static inline
uint8x16_t gf256v_mul_neon( uint8x16_t a, uint8_t b ) {
    uint8x16_t bb = vdupq_n_u8(b);

    uint8x16_t mask_3 = vdupq_n_u8( 3 );
    uint8x16_t mask_0x1b = vdupq_n_u8( 0x1b );

    return _gf256v_mul_neon( a, bb, mask_3, mask_0x1b );
}

#endif



static inline
uint8x16_t _gf256v_mul_4bits_neon( uint8x16_t a, uint8x16_t b_4bits, uint8x16_t mask_f, uint8x16_t tab_rd0 ) {
    uint8x16_t abl = clmul_8x8( a & mask_f, b_4bits );
    uint8x16_t abh = clmul_8x8( vshrq_n_u8(a, 4), b_4bits );
    uint8x16_t abr = vqtbl1q_u8( tab_rd0, vshrq_n_u8(abh, 4) );
    return abl ^ abr ^ vshlq_n_u8(abh, 4);
}

static inline
uint8x16_t _gf256v_mul_h4bits_neon( uint8x16_t a, uint8x16_t b_4bits, uint8x16_t mask_f, uint8x16_t tab_rd0, uint8x16_t tab_rd1 ) {
    uint8x16_t abl = clmul_8x8( a & mask_f, b_4bits );
    uint8x16_t abh = clmul_8x8( vshrq_n_u8(a, 4), b_4bits );
    uint8x16_t abr0 = vqtbl1q_u8( tab_rd0, vshrq_n_u8(abl, 4) ^ (abh & mask_f) );
    uint8x16_t abr1 = vqtbl1q_u8( tab_rd1, vshrq_n_u8(abh, 4 ));

    return abr0 ^ abr1 ^ vshlq_n_u8(abl, 4);
}


#ifdef _APPLE_SILICON_
static inline
uint8x16x2_t _gf256v_get_multab_neon( uint8_t v, uint8x16_t mask_f, uint8x16_t mask_0_f, uint8x16_t tab_rd0 ) {
    uint8x16_t a = vdupq_n_u8(v);

    uint8x16x2_t r;
    uint8x16_t abl = clmul_8x8( a & mask_f, mask_0_f );
    uint8x16_t abh = clmul_8x8( vshrq_n_u8(a, 4), mask_0_f );
    uint8x16_t abr = vqtbl1q_u8( tab_rd0, vshrq_n_u8(abh, 4) );
    r.val[0] = abl ^ abr ^ vshlq_n_u8(abh, 4);
    r.val[1] = vqtbl1q_u8( tab_rd0, vshrq_n_u8(r.val[0], 4) ) ^ vshlq_n_u8(r.val[0], 4);
    return r;
}
#else
static inline
uint8x16x2_t _gf256v_get_multab_neon( uint8_t v, uint8x16_t mask_f, uint8x16_t mask_0_f, uint8x16_t tab_rd0, uint8x16_t tab_rd1 ) {
    uint8x16_t a = vdupq_n_u8(v);
    uint8x16x2_t r;
    uint8x16_t abl = clmul_8x8( a & mask_f, mask_0_f );
    uint8x16_t abh = clmul_8x8( vshrq_n_u8(a, 4), mask_0_f );
    uint8x16_t abr = vqtbl1q_u8( tab_rd0, vshrq_n_u8(abh, 4) );
    uint8x16_t abr0 = vqtbl1q_u8( tab_rd0, vshrq_n_u8(abl, 4) ^ (abh & mask_f) );
    uint8x16_t abr1 = vqtbl1q_u8( tab_rd1, vshrq_n_u8(abh, 4 ));

    r.val[0] = abl ^ abr ^ vshlq_n_u8(abh, 4);
    r.val[1] = abr0 ^ abr1 ^ vshlq_n_u8(abl, 4);
    return r;
}
#endif

static inline
uint8x16x2_t gf256v_get_multab_neon( uint8_t v ) {
    #ifdef _APPLE_SILICON_
    uint8x16_t mask_f   = vdupq_n_u8(0xf);
    uint8x16_t mask_0_f = vld1q_u8(__0_f);
    uint8x16_t tab_rd0  = vld1q_u8(__gf256_bit8_11_reduce);

    return _gf256v_get_multab_neon( v, mask_f, mask_0_f, tab_rd0 );
    #else
    uint8x16_t mask_f   = vdupq_n_u8(0xf);
    uint8x16_t mask_0_f = vld1q_u8(__0_f);
    uint8x16_t tab_rd0  = vld1q_u8(__gf256_bit8_11_reduce);
    uint8x16_t tab_rd1  = vld1q_u8(__gf256_bit12_15_reduce);

    return _gf256v_get_multab_neon( v, mask_f, mask_0_f, tab_rd0, tab_rd1 );
    #endif
}

#endif // _GF16_NEON_H_

