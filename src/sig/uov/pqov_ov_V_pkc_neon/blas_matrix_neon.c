// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_matrix_neon.c
/// @brief Implementations for blas_matrix_neon.h
///

#include "gf16.h"

#include "config.h"

#include "gf16_neon.h"

#include "blas_neon.h"

#include "blas_comm.h"

#include "blas_matrix_neon.h"

#include "string.h"
#include "utils_malloc.h"

//////////// specialized functions  /////////////////////

static inline
void gf16mat_prod_32x_multab_neon(uint8_t *c, const uint8_t *matA, unsigned width_A, const uint8_t *multabs) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t cc = _gf16_tbl_x2( vld1q_u8(matA), vld1q_u8(multabs), mask_f );
    for (int i = ((int)width_A) - 1; i > 0; i--) {
        matA += 16;
        multabs += 16;
        cc ^= _gf16_tbl_x2( vld1q_u8(matA), vld1q_u8(multabs), mask_f );
    }
    vst1q_u8(c, cc);
}

static inline
void gf16mat_prod_64x_multab_neon(uint8_t *c, const uint8_t *matA, unsigned width_A, const uint8_t *multabs) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tab = vld1q_u8(multabs);
    multabs += 16;
    uint8x16_t cc0 = _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
    matA += 16;
    uint8x16_t cc1 = _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
    matA += 16;
    for (int i = ((int)width_A) - 1; i > 0; i--) {
        tab = vld1q_u8(multabs);
        multabs += 16;
        cc0 ^= _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
        matA += 16;
        cc1 ^= _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
        matA += 16;
    }
    vst1q_u8(c, cc0);
    vst1q_u8(c + 16, cc1);
}

static inline
void gf16mat_prod_96x_multab_neon(uint8_t *c, const uint8_t *matA, unsigned width_A, const uint8_t *multabs) {
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tab = vld1q_u8(multabs);
    multabs += 16;
    uint8x16_t cc0 = _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
    matA += 16;
    uint8x16_t cc1 = _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
    matA += 16;
    uint8x16_t cc2 = _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
    matA += 16;
    for (int i = ((int)width_A) - 1; i > 0; i--) {
        tab = vld1q_u8(multabs);
        multabs += 16;
        cc0 ^= _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
        matA += 16;
        cc1 ^= _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
        matA += 16;
        cc2 ^= _gf16_tbl_x2( vld1q_u8(matA), tab, mask_f );
        matA += 16;
    }
    vst1q_u8(c, cc0);
    vst1q_u8(c + 16, cc1);
    vst1q_u8(c + 32, cc2);
}

#define BLOCK_LEN 4


static
void gf16mat_remblockmat_prod_multab_neon( uint8_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_vec_byte,
                                        const uint8x16_t *multab_vec_ele, unsigned n_vec_ele ) {
    unsigned n_full_xmm = blk_vec_byte >> 4;
    unsigned rem_byte = blk_vec_byte & 15;
    uint8x16_t mask_f = vdupq_n_u8(0xf);

    uint8x16_t tmp[BLOCK_LEN];
    for (int i = 0; i < BLOCK_LEN; i++) {
        tmp[i] = vdupq_n_u8(0);
    }
    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        uint8x16_t tab = multab_vec_ele[0];
        multab_vec_ele += 1;

        tmp[0] ^= _gf16_tbl_x2( vld1q_u8( org_mat ), tab, mask_f );
        for (unsigned j = 0; j < n_full_xmm; j++) {
            uint8x16_t mj = vld1q_u8( org_mat + rem_byte + j * 16 );
            tmp[1+j] ^= _gf16_tbl_x2( mj, tab, mask_f );
        }
        org_mat += mat_vec_byte;
    }
    vst1q_u8( dest , tmp[0] );
    for (unsigned i = 0; i < n_full_xmm; i++) {
        vst1q_u8(dest + rem_byte + i * 16, tmp[i+1]);
    }
}

static
void gf16mat_blockmat_prod_multab_neon( uint8_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_vec_byte,
                                        const uint8x16_t *multab_vec_ele, unsigned n_vec_ele ) {
    unsigned n_full_xmm = blk_vec_byte >> 4;
    uint8x16_t mask_f = vdupq_n_u8(0xf);

    uint8x16_t tmp[BLOCK_LEN];
    for (int i = 0; i < BLOCK_LEN; i++) {
        tmp[i] = vdupq_n_u8(0);
    }
    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        uint8x16_t tab = multab_vec_ele[0];
        multab_vec_ele += 1;

        for (unsigned j = 0; j < n_full_xmm; j++) {
            uint8x16_t mj = vld1q_u8( org_mat + j * 16 );
            tmp[j] ^= _gf16_tbl_x2( mj, tab, mask_f );
        }
        org_mat += mat_vec_byte;
    }
    for (unsigned i = 0; i < n_full_xmm; i++) {
        vst1q_u8(dest + i * 16, tmp[i]);
    }
}



void gf16mat_prod_multab_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    if ( (32 == matA_vec_byte) ) {
        gf16mat_prod_64x_multab_neon(c, matA, matA_n_vec, multab_b);
        return;
    }
    if ( (16 == matA_vec_byte) ) {
        gf16mat_prod_32x_multab_neon(c, matA, matA_n_vec, multab_b);
        return;
    }
    if ( (48 == matA_vec_byte) ) {
        gf16mat_prod_96x_multab_neon(c, matA, matA_n_vec, multab_b);
        return;
    }

    const uint8x16_t *multabs = (const uint8x16_t *)multab_b;
    while (matA_n_vec) {
        unsigned n_ele = matA_n_vec;
        unsigned vec_len_to_go = matA_vec_byte;
        if (vec_len_to_go&15) {
            unsigned rem = vec_len_to_go&15;
            unsigned vec_len_fullreg = vec_len_to_go - rem;
            unsigned block_len = (vec_len_fullreg >= (BLOCK_LEN-1)*16) ? (BLOCK_LEN-1)*16 : vec_len_fullreg;
            gf16mat_remblockmat_prod_multab_neon( c , matA, matA_vec_byte, block_len+rem, multabs, n_ele);
            vec_len_to_go -= (rem + block_len);
        }
        while ( vec_len_to_go ) {
            unsigned block_len = (vec_len_to_go >= BLOCK_LEN * 16) ? BLOCK_LEN * 16 : vec_len_to_go;
            unsigned block_st_idx = matA_vec_byte - vec_len_to_go;
            gf16mat_blockmat_prod_multab_neon( c + block_st_idx, matA + block_st_idx, matA_vec_byte, block_len, multabs, n_ele );
            vec_len_to_go -= block_len;
        }
        matA_n_vec -= n_ele;
        matA += n_ele * matA_vec_byte;
        multabs += n_ele;
    }
}


#undef BLOCK_LEN


//////////////////  end of multab   //////////////////////////////



static inline
void gf16mat_prod_32x_neon(uint8_t *c, const uint8_t *matA, unsigned width_A, const uint8_t *b) {
    uint8x16_t r0 = vdupq_n_u8(0);
    uint8x16_t r1 = vdupq_n_u8(0);
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    for(unsigned i=0;i<width_A;i++) {
        uint8x16_t bb = vdupq_n_u8( gf16v_get_ele(b,i) );
        uint8x16_t tmp = vld1q_u8( matA );
        r0 ^= clmul_8x8( tmp&mask_f , bb );
        r1 ^= clmul_8x8( vshrq_n_u8(tmp,4) , bb );
        matA += 16;
    }
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    r0 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r0, 4) );
    r1 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r1, 4) );
    vst1q_u8(c, vsliq_n_u8(r0,r1,4));
}

static inline
void gf16mat_prod_64x_neon(uint8_t *c, const uint8_t *matA, unsigned width_A, const uint8_t *b) {
    uint8x16_t r0 = vdupq_n_u8(0);
    uint8x16_t r1 = vdupq_n_u8(0);
    uint8x16_t r2 = vdupq_n_u8(0);
    uint8x16_t r3 = vdupq_n_u8(0);
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    for(unsigned i=0;i<width_A;i++) {
        uint8x16_t bb = vdupq_n_u8( gf16v_get_ele(b,i) );
        uint8x16_t tmp0 = vld1q_u8( matA );
        uint8x16_t tmp1 = vld1q_u8( matA+16 );
        r0 ^= clmul_8x8( tmp0&mask_f , bb );
        r1 ^= clmul_8x8( vshrq_n_u8(tmp0,4) , bb );
        r2 ^= clmul_8x8( tmp1&mask_f , bb );
        r3 ^= clmul_8x8( vshrq_n_u8(tmp1,4) , bb );
        matA += 32;
    }
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    r0 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r0, 4) );
    r1 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r1, 4) );
    r2 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r2, 4) );
    r3 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r3, 4) );
    vst1q_u8(c, vsliq_n_u8(r0,r1,4));
    vst1q_u8(c+16, vsliq_n_u8(r2,r3,4));
}

static inline
void gf16mat_prod_96x_neon(uint8_t *c, const uint8_t *matA, unsigned width_A, const uint8_t *b) {
    uint8x16_t r0 = vdupq_n_u8(0);
    uint8x16_t r1 = vdupq_n_u8(0);
    uint8x16_t r2 = vdupq_n_u8(0);
    uint8x16_t r3 = vdupq_n_u8(0);
    uint8x16_t r4 = vdupq_n_u8(0);
    uint8x16_t r5 = vdupq_n_u8(0);
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    for(unsigned i=0;i<width_A;i++) {
        uint8x16_t bb = vdupq_n_u8( gf16v_get_ele(b,i) );
        uint8x16_t tmp0 = vld1q_u8( matA );
        uint8x16_t tmp1 = vld1q_u8( matA+16 );
        uint8x16_t tmp2 = vld1q_u8( matA+32 );
        r0 ^= clmul_8x8( tmp0&mask_f , bb );
        r1 ^= clmul_8x8( vshrq_n_u8(tmp0,4) , bb );
        r2 ^= clmul_8x8( tmp1&mask_f , bb );
        r3 ^= clmul_8x8( vshrq_n_u8(tmp1,4) , bb );
        r4 ^= clmul_8x8( tmp2&mask_f , bb );
        r5 ^= clmul_8x8( vshrq_n_u8(tmp2,4) , bb );
        matA += 48;
    }
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    r0 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r0, 4) );
    r1 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r1, 4) );
    r2 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r2, 4) );
    r3 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r3, 4) );
    r4 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r4, 4) );
    r5 ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(r5, 4) );
    vst1q_u8(c, vsliq_n_u8(r0,r1,4));
    vst1q_u8(c+16, vsliq_n_u8(r2,r3,4));
    vst1q_u8(c+32, vsliq_n_u8(r4,r5,4));
}


//////////// end of specialized functions  /////////////////////



#define BLOCK_LEN 4


static
void gf16mat_remblockmat_prod_neon( uint8_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_vec_byte, const uint8_t *b, unsigned n_vec_ele ) {
    unsigned n_full_xmm = blk_vec_byte >> 4;
    unsigned rem_byte = blk_vec_byte & 15;
    uint8x16_t mask_f = vdupq_n_u8(0xf);

    uint8x16_t tmp0[BLOCK_LEN];
    uint8x16_t tmp1[BLOCK_LEN];
    for (int i = 0; i < BLOCK_LEN; i++) { tmp0[i] = vdupq_n_u8(0); }
    for (int i = 0; i < BLOCK_LEN; i++) { tmp1[i] = vdupq_n_u8(0); }

    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        uint8x16_t bb = vdupq_n_u8( gf16v_get_ele(b,i) );
        uint8x16_t tt = vld1q_u8(org_mat);
        tmp0[0] ^= clmul_8x8( tt&mask_f, bb );
        tmp1[0] ^= clmul_8x8( vshrq_n_u8(tt,4), bb );
        for (unsigned j = 0; j < n_full_xmm; j++) {
            uint8x16_t mj = vld1q_u8( org_mat + rem_byte + j * 16 );
            tmp0[1+j] ^= clmul_8x8( mj&mask_f, bb );
            tmp1[1+j] ^= clmul_8x8( vshrq_n_u8(mj,4), bb );
        }
        org_mat += mat_vec_byte;
    }
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    for (int i = 0; i < BLOCK_LEN; i++) { tmp0[i] ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(tmp0[i], 4) ); }
    for (int i = 0; i < BLOCK_LEN; i++) { tmp1[i] ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(tmp1[i], 4) ); }

    vst1q_u8( dest , vsliq_n_u8(tmp0[0],tmp1[0],4) );
    for (unsigned i = 0; i < n_full_xmm; i++) {
        vst1q_u8( dest + rem_byte +i * 16 , vsliq_n_u8(tmp0[1+i],tmp1[1+i],4) );
    }
}

static
void gf16mat_blockmat_prod_neon( uint8_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_vec_byte, const uint8_t *b, unsigned n_vec_ele ) {
    unsigned n_full_xmm = blk_vec_byte >> 4;
    uint8x16_t mask_f = vdupq_n_u8(0xf);

    uint8x16_t tmp0[BLOCK_LEN];
    uint8x16_t tmp1[BLOCK_LEN];
    for (int i = 0; i < BLOCK_LEN; i++) { tmp0[i] = vdupq_n_u8(0); }
    for (int i = 0; i < BLOCK_LEN; i++) { tmp1[i] = vdupq_n_u8(0); }

    for (unsigned i = 0; i < n_vec_ele; i++ ) {
        uint8x16_t bb = vdupq_n_u8( gf16v_get_ele(b,i) );
        for (unsigned j = 0; j < n_full_xmm; j++) {
            uint8x16_t mj = vld1q_u8( org_mat + j * 16 );
            tmp0[j] ^= clmul_8x8( mj&mask_f, bb );
            tmp1[j] ^= clmul_8x8( vshrq_n_u8(mj,4), bb );
        }
        org_mat += mat_vec_byte;
    }
    uint8x16_t tab_reduce = vld1q_u8(__gf16_reduce);
    for (int i = 0; i < BLOCK_LEN; i++) { tmp0[i] ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(tmp0[i], 4) ); }
    for (int i = 0; i < BLOCK_LEN; i++) { tmp1[i] ^= vqtbl1q_u8( tab_reduce, vshrq_n_u8(tmp1[i], 4) ); }

    for (unsigned i = 0; i < n_full_xmm; i++) {
        vst1q_u8( dest + i * 16 , vsliq_n_u8(tmp0[i],tmp1[i],4) );
    }
}

void gf16mat_prod_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    if ( (32 == matA_vec_byte) ) {
        gf16mat_prod_64x_neon(c, matA, matA_n_vec, b);
        return;
    }
    if ( (16 == matA_vec_byte) ) {
        gf16mat_prod_32x_neon(c, matA, matA_n_vec, b);
        return;
    }
    if ( (48 == matA_vec_byte) ) {
        gf16mat_prod_96x_neon(c, matA, matA_n_vec, b);
        return;
    }

    while (matA_n_vec) {
        unsigned n_ele = matA_n_vec;

        unsigned vec_len_to_go = matA_vec_byte;
        if (vec_len_to_go&15) {
            unsigned rem = vec_len_to_go&15;
            unsigned vec_len_fullreg = vec_len_to_go - rem;
            unsigned block_len = (vec_len_fullreg >= (BLOCK_LEN-1)*16) ? (BLOCK_LEN-1)*16 : vec_len_fullreg;
            gf16mat_remblockmat_prod_neon( c , matA, matA_vec_byte, block_len+rem, b, n_ele);
            vec_len_to_go -= (rem + block_len);
        }
        while ( vec_len_to_go ) {
            unsigned block_len = (vec_len_to_go >= BLOCK_LEN * 16) ? BLOCK_LEN * 16 : vec_len_to_go;
            unsigned block_st_idx = matA_vec_byte - vec_len_to_go;
            gf16mat_blockmat_prod_neon( c + block_st_idx, matA + block_st_idx, matA_vec_byte, block_len, b, n_ele );
            vec_len_to_go -= block_len;
        }
        matA_n_vec -= n_ele;
        b += (n_ele + 1) >> 1;
        matA += n_ele * matA_vec_byte;
    }
}


#undef BLOCK_LEN


///////////////////  matrix transpose ///////////////////

static inline uint8x16_t _vtrn1q_u64( uint8x16_t a, uint8x16_t b) { return vreinterpretq_u8_u64(vtrn1q_u64(vreinterpretq_u64_u8(a),vreinterpretq_u64_u8(b))); }
static inline uint8x16_t _vtrn2q_u64( uint8x16_t a, uint8x16_t b) { return vreinterpretq_u8_u64(vtrn2q_u64(vreinterpretq_u64_u8(a),vreinterpretq_u64_u8(b))); }
static inline uint8x16_t _vtrn1q_u32( uint8x16_t a, uint8x16_t b) { return vreinterpretq_u8_u32(vtrn1q_u32(vreinterpretq_u32_u8(a),vreinterpretq_u32_u8(b))); }
static inline uint8x16_t _vtrn2q_u32( uint8x16_t a, uint8x16_t b) { return vreinterpretq_u8_u32(vtrn2q_u32(vreinterpretq_u32_u8(a),vreinterpretq_u32_u8(b))); }
static inline uint8x16_t _vtrn1q_u16( uint8x16_t a, uint8x16_t b) { return vreinterpretq_u8_u16(vtrn1q_u16(vreinterpretq_u16_u8(a),vreinterpretq_u16_u8(b))); }
static inline uint8x16_t _vtrn2q_u16( uint8x16_t a, uint8x16_t b) { return vreinterpretq_u8_u16(vtrn2q_u16(vreinterpretq_u16_u8(a),vreinterpretq_u16_u8(b))); }


static
void byte_transpose_16x16_neon( uint8_t *dest, unsigned dest_vec_len, const uint8_t *src, unsigned src_vec_len ) {
    uint8x16_t r0 = _vtrn1q_u64( vld1q_u8( src + 0 * src_vec_len), vld1q_u8( src + 8 * src_vec_len) );
    uint8x16_t r8 = _vtrn2q_u64( vld1q_u8( src + 0 * src_vec_len), vld1q_u8( src + 8 * src_vec_len) );
    uint8x16_t r1 = _vtrn1q_u64( vld1q_u8( src + 1 * src_vec_len), vld1q_u8( src + 9 * src_vec_len) );
    uint8x16_t r9 = _vtrn2q_u64( vld1q_u8( src + 1 * src_vec_len), vld1q_u8( src + 9 * src_vec_len) );
    uint8x16_t r2 = _vtrn1q_u64( vld1q_u8( src + 2 * src_vec_len), vld1q_u8( src + 10 * src_vec_len) );
    uint8x16_t r10 = _vtrn2q_u64( vld1q_u8( src + 2 * src_vec_len), vld1q_u8( src + 10 * src_vec_len) );
    uint8x16_t r3 = _vtrn1q_u64( vld1q_u8( src + 3 * src_vec_len), vld1q_u8( src + 11 * src_vec_len) );
    uint8x16_t r11 = _vtrn2q_u64( vld1q_u8( src + 3 * src_vec_len), vld1q_u8( src + 11 * src_vec_len) );
    uint8x16_t r4 = _vtrn1q_u64( vld1q_u8( src + 4 * src_vec_len), vld1q_u8( src + 12 * src_vec_len) );
    uint8x16_t r12 = _vtrn2q_u64( vld1q_u8( src + 4 * src_vec_len), vld1q_u8( src + 12 * src_vec_len) );
    uint8x16_t r5 = _vtrn1q_u64( vld1q_u8( src + 5 * src_vec_len), vld1q_u8( src + 13 * src_vec_len) );
    uint8x16_t r13 = _vtrn2q_u64( vld1q_u8( src + 5 * src_vec_len), vld1q_u8( src + 13 * src_vec_len) );
    uint8x16_t r6 = _vtrn1q_u64( vld1q_u8( src + 6 * src_vec_len), vld1q_u8( src + 14 * src_vec_len) );
    uint8x16_t r14 = _vtrn2q_u64( vld1q_u8( src + 6 * src_vec_len), vld1q_u8( src + 14 * src_vec_len) );
    uint8x16_t r7 = _vtrn1q_u64( vld1q_u8( src + 7 * src_vec_len), vld1q_u8( src + 15 * src_vec_len) );
    uint8x16_t r15 = _vtrn2q_u64( vld1q_u8( src + 7 * src_vec_len), vld1q_u8( src + 15 * src_vec_len) );

    uint8x16_t s0 = _vtrn1q_u32( r0, r4 );
    uint8x16_t s4 = _vtrn2q_u32( r0, r4 );
    uint8x16_t s1 = _vtrn1q_u32( r1, r5 );
    uint8x16_t s5 = _vtrn2q_u32( r1, r5 );
    uint8x16_t s2 = _vtrn1q_u32( r2, r6 );
    uint8x16_t s6 = _vtrn2q_u32( r2, r6 );
    uint8x16_t s3 = _vtrn1q_u32( r3, r7 );
    uint8x16_t s7 = _vtrn2q_u32( r3, r7 );
    uint8x16_t s8 = _vtrn1q_u32( r8, r12 );
    uint8x16_t s12 = _vtrn2q_u32( r8, r12 );
    uint8x16_t s9 = _vtrn1q_u32( r9, r13 );
    uint8x16_t s13 = _vtrn2q_u32( r9, r13 );
    uint8x16_t s10 = _vtrn1q_u32( r10, r14 );
    uint8x16_t s14 = _vtrn2q_u32( r10, r14 );
    uint8x16_t s11 = _vtrn1q_u32( r11, r15 );
    uint8x16_t s15 = _vtrn2q_u32( r11, r15 );

    r0 = _vtrn1q_u16( s0, s2 );
    r2 = _vtrn2q_u16( s0, s2 );
    r1 = _vtrn1q_u16( s1, s3 );
    r3 = _vtrn2q_u16( s1, s3 );
    r4 = _vtrn1q_u16( s4, s6 );
    r6 = _vtrn2q_u16( s4, s6 );
    r5 = _vtrn1q_u16( s5, s7 );
    r7 = _vtrn2q_u16( s5, s7 );
    r8 = _vtrn1q_u16( s8, s10 );
    r10 = _vtrn2q_u16( s8, s10 );
    r9 = _vtrn1q_u16( s9, s11 );
    r11 = _vtrn2q_u16( s9, s11 );
    r12 = _vtrn1q_u16( s12, s14 );
    r14 = _vtrn2q_u16( s12, s14 );
    r13 = _vtrn1q_u16( s13, s15 );
    r15 = _vtrn2q_u16( s13, s15 );

    vst1q_u8( dest + 0 * dest_vec_len, vtrn1q_u8( r0, r1 ) );
    vst1q_u8( dest + 1 * dest_vec_len, vtrn2q_u8( r0, r1 ) );
    vst1q_u8( dest + 2 * dest_vec_len, vtrn1q_u8( r2, r3 ) );
    vst1q_u8( dest + 3 * dest_vec_len, vtrn2q_u8( r2, r3 ) );
    vst1q_u8( dest + 4 * dest_vec_len, vtrn1q_u8( r4, r5 ) );
    vst1q_u8( dest + 5 * dest_vec_len, vtrn2q_u8( r4, r5 ) );
    vst1q_u8( dest + 6 * dest_vec_len, vtrn1q_u8( r6, r7 ) );
    vst1q_u8( dest + 7 * dest_vec_len, vtrn2q_u8( r6, r7 ) );
    vst1q_u8( dest + 8 * dest_vec_len, vtrn1q_u8( r8, r9 ) );
    vst1q_u8( dest + 9 * dest_vec_len, vtrn2q_u8( r8, r9 ) );
    vst1q_u8( dest + 10 * dest_vec_len, vtrn1q_u8( r10, r11 ) );
    vst1q_u8( dest + 11 * dest_vec_len, vtrn2q_u8( r10, r11 ) );
    vst1q_u8( dest + 12 * dest_vec_len, vtrn1q_u8( r12, r13 ) );
    vst1q_u8( dest + 13 * dest_vec_len, vtrn2q_u8( r12, r13 ) );
    vst1q_u8( dest + 14 * dest_vec_len, vtrn1q_u8( r14, r15 ) );
    vst1q_u8( dest + 15 * dest_vec_len, vtrn2q_u8( r14, r15 ) );
}


// nainv code:
//for (unsigned i = 0; i < 32; i++) {
//    for (unsigned j = 0; j < 32; j++) {
//        mat[i * vec_len + j] = src_mat[j * src_vec_len + i];
//    }
//}
static
void gf256mat_transpose_32x32_neon( uint8_t *mat, unsigned vec_len, const uint8_t *src_mat, unsigned src_vec_len ) {
    byte_transpose_16x16_neon( mat, vec_len, src_mat, src_vec_len );
    byte_transpose_16x16_neon( mat + 16, vec_len, src_mat + 16 * src_vec_len, src_vec_len );
    byte_transpose_16x16_neon( mat + 16 * vec_len, vec_len, src_mat + 16, src_vec_len );
    byte_transpose_16x16_neon( mat + 16 * vec_len + 16, vec_len, src_mat + 16 * src_vec_len + 16, src_vec_len );
}

static
void gf16mat_64x64_sqmat_transpose_neon(uint8_t *dest_mat, unsigned dest_vec_len, const uint8_t *src_sqmat ) {
    gf256mat_transpose_32x32_neon( dest_mat, dest_vec_len * 2, src_sqmat, 64 );                   // transpose even rows
    gf256mat_transpose_32x32_neon( dest_mat + dest_vec_len, dest_vec_len * 2, src_sqmat + 32, 64 ); // transpose odd rows
    // transpose 2x2 4-bit blocks
    uint8x16_t mask_0f = vdupq_n_u8(0x0f);
    uint8x16_t mask_f0 = vdupq_n_u8(0xf0);
    for (int i = 0; i < 64; i += 2) {
        uint8x16_t row1_0 = vld1q_u8( dest_mat + i * dest_vec_len );
        uint8x16_t row1_1 = vld1q_u8( dest_mat + i * dest_vec_len + 16);
        uint8x16_t row2_0 = vld1q_u8( dest_mat + (i + 1) * dest_vec_len );
        uint8x16_t row2_1 = vld1q_u8( dest_mat + (i + 1) * dest_vec_len + 16);

        uint8x16_t out1_0 = (row1_0 & mask_0f)^vshlq_n_u8(row2_0 & mask_0f, 4);
        uint8x16_t out1_1 = (row1_1 & mask_0f)^vshlq_n_u8(row2_1 & mask_0f, 4);
        uint8x16_t out2_0 = (row2_0 & mask_f0)^vshrq_n_u8(row1_0 & mask_f0, 4);
        uint8x16_t out2_1 = (row2_1 & mask_f0)^vshrq_n_u8(row1_1 & mask_f0, 4);

        vst1q_u8( dest_mat + i * dest_vec_len, out1_0 );
        vst1q_u8( dest_mat + i * dest_vec_len + 16, out1_1 );
        vst1q_u8( dest_mat + (i + 1)*dest_vec_len, out2_0 );
        vst1q_u8( dest_mat + (i + 1)*dest_vec_len + 16, out2_1 );
    }
}


static
void gf16mat_sqmat_transpose(uint8_t *dest_mat, unsigned dest_vec_len, const uint8_t *src_sqmat, unsigned src_vec_len, unsigned n_vec ) {
    if ( 64 == n_vec && 32 == src_vec_len ) {
        gf16mat_64x64_sqmat_transpose_neon(dest_mat, dest_vec_len, src_sqmat);
        return;
    }

    for (unsigned i = 0; i < n_vec; i++) {
        uint8_t *ai = dest_mat + i * dest_vec_len;
        for (unsigned j = 0; j < n_vec; j++) {
            gf16v_set_ele( ai, j, gf16v_get_ele(src_sqmat + j * src_vec_len, i) );
        }
    }
}

//////////////////    Gaussian elimination + Back substitution for solving linear equations  //////////////////


static
unsigned gf16mat_gauss_elim_row_echolen( uint8_t *mat, unsigned h, unsigned w_byte, unsigned offset ) {
    PQOV_ALIGN uint8_t pivots[96];
    unsigned w_2 = w_byte;

    uint8_t r8 = 1;
    for (unsigned i = 0; i < h; i++) {
        unsigned idx = (offset << 1) + i;
        unsigned offset_16 = idx >> 5;
        unsigned st_idx = offset_16 << 4;
        uint8_t *ai = mat + w_2 * i;
        for (unsigned j = i; j < h; j++) {
            pivots[j] = gf16v_get_ele( mat + w_2 * j, idx );
        }

#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF16_ < h) ? i + _GE_EARLY_STOP_STEPS_GF16_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            uint8_t m8 = !gf16_is_nonzero( pivots[i] );
            m8 = -m8;
            uint8x16_t m128 = vdupq_n_u8( m8 );

            pivots[i] ^= pivots[j] & m8;
            uint8_t *aj = mat + w_2 * j;
            for (unsigned k = st_idx; k < w_2; k += 16) {
                vst1q_u8( ai + k, vld1q_u8(ai + k) ^ (m128 & vld1q_u8(aj + k)) );
            }
        }
        r8 &= gf16_is_nonzero( pivots[i] );
        pivots[i] = gf16_inv_neon( pivots[i] );

        gf16v_mul_scalar_neon( ai + st_idx, pivots[i], w_2 - st_idx ); // pivot row
        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *aj = mat + w_2 * j;
            gf16v_madd_neon( aj + st_idx, ai + st_idx, pivots[j], w_2 - st_idx );
        }
    }
    return r8;
}



unsigned gf16mat_gaussian_elim_neon(uint8_t *sqmat_a, uint8_t *constant, unsigned len) {
#define MAX_H  (64)
    uint8_t mat[MAX_H * ((MAX_H / 2) + 16)];
#undef MAX_H

    unsigned height = len;
    unsigned width_o  = len / 2;
    unsigned width_n  = ((width_o + 1 + 15) >> 4) << 4;
    unsigned offset   = width_n - width_o - 1;

    gf16mat_sqmat_transpose(mat + offset, width_n, sqmat_a, width_o, height );
    for (unsigned i = 0; i < height; i++) {
        mat[i * width_n + width_o + offset] = gf16v_get_ele(constant, i);
    }

    unsigned char r8 = gf16mat_gauss_elim_row_echolen( mat, height, width_n, offset );

    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width_n + offset;
        memcpy( sqmat_a + i * width_o, ai, width_o ); // output a row-major matrix
        gf16v_set_ele(constant, i, ai[width_o] );
    }
    return r8;
}

void gf16mat_back_substitute_neon( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len) {
#define MAX_H  (64)
    uint8_t mat[MAX_H * (MAX_H / 2)];
#undef MAX_H
    unsigned width_byte = (len + 1) / 2;
    gf16mat_sqmat_transpose( mat, width_byte, sq_row_mat_a, width_byte, len );
    for (unsigned i = len - 1; i > 0; i--) {
        uint8_t *col = mat + i * width_byte;
        gf16v_set_ele( col, i, 0 );
        gf16v_madd_neon( constant, col, gf16v_get_ele(constant, i), (i + 1) / 2 );
    }
}





///////////////////////////////  GF( 256 ) ////////////////////////////////////////////////////



static
void gf256mat_blockmat_madd_multab_neon( uint8x16_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_st_idx, unsigned blk_vec_byte,
        const uint8x16_t *multab_vec_ele, unsigned n_vec_ele ) {
    unsigned n_full_xmm = blk_vec_byte >> 4;
    unsigned n_rem_byte = blk_vec_byte & 15;
    uint8x16_t mask_f = vdupq_n_u8(0xf);

    org_mat += blk_st_idx;
    if ( !n_rem_byte) {
        for (unsigned i = 0; i < n_vec_ele; i++ ) {
            uint8x16_t tab_l = multab_vec_ele[0];
            uint8x16_t tab_h = multab_vec_ele[1];
            multab_vec_ele += 2;

            for (unsigned j = 0; j < n_full_xmm; j++) {
                uint8x16_t mj = vld1q_u8( org_mat + j * 16 );
                dest[j] ^= _gf256_tbl( mj, tab_l, tab_h, mask_f );
            }
            org_mat += mat_vec_byte;
        }
    } else {  // n_rem_byte != 0
        for (unsigned i = 0; i < n_vec_ele - 1; i++ ) {
            uint8x16_t tab_l = multab_vec_ele[0];
            uint8x16_t tab_h = multab_vec_ele[1];
            multab_vec_ele += 2;

            for (unsigned j = 0; j < n_full_xmm + 1; j++) {
                uint8x16_t mj = vld1q_u8( org_mat + j * 16 );
                dest[j] ^= _gf256_tbl( mj, tab_l, tab_h, mask_f );
            }
            org_mat += mat_vec_byte;
        }{  //unsigned i=n_vec_ele-1;
            uint8x16_t tab_l = multab_vec_ele[0];
            uint8x16_t tab_h = multab_vec_ele[1];
            multab_vec_ele += 2;

            for (unsigned j = 0; j < n_full_xmm; j++) {
                uint8x16_t mj = vld1q_u8( org_mat + j * 16 );
                dest[j] ^= _gf256_tbl( mj, tab_l, tab_h, mask_f );
            } //if( n_rem_byte )
            {
                // unsigned j=n_full_xmm;
                uint8x16_t mj = _load_Qreg( org_mat + (n_full_xmm * 16), n_rem_byte );
                dest[n_full_xmm] ^= _gf256_tbl( mj, tab_l, tab_h, mask_f );
            }
        }
    }
}


// need to specialize matrix with vector lengths 22, 36, 44, 48, 72, 96


static
void gf256mat_prod_multab_17_32_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    uint8x16_t c0 = vdupq_n_u8(0);
    uint8x16_t c1 = vdupq_n_u8(0);
    uint8x16_t mask_f = vdupq_n_u8(0xf);
    unsigned rem = matA_vec_byte - 16;
    int counter = (int)matA_n_vec - 1;
    while (counter--) {
        uint8x16_t a0 = vld1q_u8(matA);
        matA += 16;
        uint8x16_t a1 = vld1q_u8(matA);
        matA += rem;
        uint8x16_t ml = vld1q_u8(multab_b);
        multab_b += 16;
        uint8x16_t mh = vld1q_u8(multab_b);
        multab_b += 16;
        c0 ^= _gf256_tbl( a0, ml, mh, mask_f );
        c1 ^= _gf256_tbl( a1, ml, mh, mask_f );
    }
    {
        uint8x16_t a0 = vld1q_u8(matA);
        matA += 16;
        uint8x16_t a1 = _load_Qreg( matA, rem );
        uint8x16_t ml = vld1q_u8(multab_b);
        multab_b += 16;
        uint8x16_t mh = vld1q_u8(multab_b);
        multab_b += 16;
        c0 ^= _gf256_tbl( a0, ml, mh, mask_f );
        c1 ^= _gf256_tbl( a1, ml, mh, mask_f );
    }
    vst1q_u8(c, c0);
    _store_Qreg( c + 16, rem, c1 );
}
static
void gf256mat_prod_multab_33_48_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    uint8x16_t c0 = vdupq_n_u8(0);
    uint8x16_t mask_f = vdupq_n_u8(0xf);
    uint8x16_t c1 = c0;
    uint8x16_t c2 = c0;
    unsigned rem = matA_vec_byte - 32;
    int counter = (int)matA_n_vec - 1;
    while (counter--) {
        uint8x16_t a0 = vld1q_u8(matA);
        matA += 16;
        uint8x16_t a1 = vld1q_u8(matA);
        matA += 16;
        uint8x16_t a2 = vld1q_u8(matA);
        matA += rem;
        uint8x16_t ml = vld1q_u8(multab_b);
        multab_b += 16;
        uint8x16_t mh = vld1q_u8(multab_b);
        multab_b += 16;
        c0 ^= _gf256_tbl( a0, ml, mh, mask_f );
        c1 ^= _gf256_tbl( a1, ml, mh, mask_f );
        c2 ^= _gf256_tbl( a2, ml, mh, mask_f );
    }
    {
        uint8x16_t a0 = vld1q_u8(matA);
        matA += 16;
        uint8x16_t a1 = vld1q_u8(matA);
        matA += 16;
        uint8x16_t a2 = _load_Qreg( matA, rem );
        uint8x16_t ml = vld1q_u8(multab_b);
        multab_b += 16;
        uint8x16_t mh = vld1q_u8(multab_b);
        multab_b += 16;
        c0 ^= _gf256_tbl( a0, ml, mh, mask_f );
        c1 ^= _gf256_tbl( a1, ml, mh, mask_f );
        c2 ^= _gf256_tbl( a2, ml, mh, mask_f );
    }
    vst1q_u8(c, c0);
    vst1q_u8(c + 16, c1);
    _store_Qreg( c + 32, rem, c2 );
}

#define BLOCK_LEN 8

// XXX: matA_vec_byte has to be >= 8
void gf256mat_prod_multab_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *multab_b ) {
    if ( (32 >= matA_vec_byte) && (16 < matA_vec_byte) ) {
        gf256mat_prod_multab_17_32_neon(c, matA, matA_vec_byte, matA_n_vec, multab_b);
        return;
    }
    if ( (48 >= matA_vec_byte) && (32 < matA_vec_byte) ) {
        gf256mat_prod_multab_33_48_neon(c, matA, matA_vec_byte, matA_n_vec, multab_b);
        return;
    }
    //if( 96 >= matA_vec_byte ) { gf256mat_prod_multab_96_neon(c,matA,matA_vec_byte,matA_n_vec,multab_b); return; }

    const uint8x16_t *multabs = (const uint8x16_t *)multab_b;
    gf256v_set_zero_neon( c, matA_vec_byte );

    uint8x16_t blockmat_vec[BLOCK_LEN];

    while (matA_n_vec) {
        unsigned n_ele = (matA_n_vec >= 16) ? 16 : matA_n_vec;
        unsigned vec_len_to_go = matA_vec_byte;
        while ( vec_len_to_go ) {
            unsigned block_len = (vec_len_to_go >= BLOCK_LEN * 16) ? BLOCK_LEN * 16 : vec_len_to_go;
            unsigned block_st_idx = matA_vec_byte - vec_len_to_go;

            load_Qregs( blockmat_vec, c + block_st_idx, block_len );
            gf256mat_blockmat_madd_multab_neon( blockmat_vec, matA, matA_vec_byte, block_st_idx, block_len, multabs, n_ele );
            store_Qregs( c + block_st_idx, block_len, blockmat_vec );

            vec_len_to_go -= block_len;
        }

        matA_n_vec -= n_ele;
        matA += n_ele * matA_vec_byte;
        multabs += n_ele * 2;
    }
}

#undef BLOCK_LEN


#define _PMULL_INLINEASM_

#define _GF256_LAZY_REDUCE_

#if defined(_GF256_LAZY_REDUCE_)

static
void gf256mat_block1_prod_lazy(uint8_t *c, const uint8_t *mat, unsigned mat_vec_len,
                               const uint8_t *b, unsigned mat_n_vec ) {
    if ( 0 == mat_n_vec ) {
        memset(c, 0, 16);
        return;
    }

    uint8x16_t rl0;
    uint8x16_t rh0;
    rl0 = vdupq_n_u8(0);
    rh0 = vdupq_n_u8(0);

    const uint8_t *ptr = mat;
    for (unsigned j = 0; j < mat_n_vec; j++) {
        register uint8x16_t cc0 __asm__ ("v10") = vld1q_u8(ptr);
        ptr += mat_vec_len;
        register uint8x16_t bb  __asm__ ("v9") = vld1q_dup_u8(b+j);
        #ifdef  _PMULL_INLINEASM_
        register uint8x16_t tmp0 __asm__("v13");
        //rl0 ^= vmull_p8( vget_low_p8(cc0) , vget_low_p8(bb) );
        __asm__ volatile ( "pmull   v13.8h, v10.8b , v9.8b"   : "=w"(tmp0) : "w"(cc0), "w"(bb) );
        __asm__ volatile ( "pmull2  v10.8h, v10.16b , v9.16b" : "+w"(cc0) : "w"(bb) );
        rl0 ^= tmp0;
        rh0 ^= cc0;
        #else
        rl0 ^= vmull_p8( vget_low_p8(cc0), vget_low_p8(bb) );
        rh0 ^= vmull_high_p8( cc0, bb );
        #endif
    }
    // reduce
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tab_rd0 = vld1q_u8(__gf256_bit8_11_reduce);
    uint8x16_t tab_rd1 = vld1q_u8(__gf256_bit12_15_reduce);
    uint8x16_t r0 = _gf256v_reduce_tbl_neon( rl0, rh0, mask_f, tab_rd0, tab_rd1 );
    vst1q_u8( c, r0 );
}

// assert(16>=matA_vec_byte)&&(8<=matA_vec_byte)
static
void gf256mat_prod_16_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    if (16 == matA_vec_byte) {
        gf256mat_block1_prod_lazy(c, matA, matA_vec_byte, b, matA_n_vec);
        return;
    }
    uint8x16_t tmp[1];
    uint8_t *ptr = (uint8_t *)tmp;
    gf256mat_block1_prod_lazy(ptr, matA, matA_vec_byte, b, matA_n_vec - 1);
    gf256v_madd_neon( ptr, matA + matA_vec_byte * (matA_n_vec - 1), b[matA_n_vec - 1], matA_vec_byte );
    memcpy(c, ptr, matA_vec_byte);
}

static
void gf256mat_block2_prod_lazy(uint8_t *c, const uint8_t *mat, unsigned mat_vec_len,
                               const uint8_t *b, unsigned mat_n_vec ) {
    if ( 0 == mat_n_vec ) {
        memset(c, 0, 32);
        return;
    }

    uint8x16_t rl0, rl1;
    uint8x16_t rh0, rh1;
    rl0 = vdupq_n_u8(0);
    rl1 = vdupq_n_u8(0);
    rh0 = vdupq_n_u8(0);
    rh1 = vdupq_n_u8(0);

    const uint8_t *ptr = mat;
    for (unsigned j = 0; j < mat_n_vec; j++) {
        register uint8x16_t cc0 __asm__ ("v10") = vld1q_u8(ptr);
        register uint8x16_t cc1 __asm__ ("v11") = vld1q_u8(ptr+16);
        ptr += mat_vec_len;
        register uint8x16_t bb  __asm__ ("v9") = vld1q_dup_u8(b+j);
        #ifdef  _PMULL_INLINEASM_
        register uint8x16_t tmp0 __asm__("v13");
        //rl0 ^= vmull_p8( vget_low_p8(cc0) , vget_low_p8(bb) );
        __asm__ volatile ( "pmull   v13.8h, v10.8b , v9.8b"   : "=w"(tmp0) : "w"(cc0), "w"(bb) );
        __asm__ volatile ( "pmull2  v10.8h, v10.16b , v9.16b" : "+w"(cc0) : "w"(bb) );
        rl0 ^= tmp0;
        rh0 ^= cc0;
        //rl1 ^= vmull_p8( vget_low_p8(cc1) , vget_low_p8(bb) );
        __asm__ volatile ( "pmull   v13.8h, v11.8b , v9.8b"   : "=w"(tmp0) : "w"(cc1), "w"(bb) );
        __asm__ volatile ( "pmull2  v10.8h, v11.16b , v9.16b" : "=w"(cc0) : "w"(cc1), "w"(bb) );
        rl1 ^= tmp0;
        rh1 ^= cc0;
        #else
        rl0 ^= vmull_p8( vget_low_p8(cc0), vget_low_p8(bb) );
        rh0 ^= vmull_high_p8( cc0, bb );
        rl1 ^= vmull_p8( vget_low_p8(cc1), vget_low_p8(bb) );
        rh1 ^= vmull_high_p8( cc1, bb );
        #endif
    }
    // reduce
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tab_rd0 = vld1q_u8(__gf256_bit8_11_reduce);
    uint8x16_t tab_rd1 = vld1q_u8(__gf256_bit12_15_reduce);

    uint8x16_t r0 = _gf256v_reduce_tbl_neon( rl0, rh0, mask_f, tab_rd0, tab_rd1 );
    uint8x16_t r1 = _gf256v_reduce_tbl_neon( rl1, rh1, mask_f, tab_rd0, tab_rd1 );

    vst1q_u8( c, r0 );
    vst1q_u8( c + 16, r1 );
}

// assert(32>=matA_vec_byte)&&(16<matA_vec_byte)
static
void gf256mat_prod_32_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    if (32 == matA_vec_byte) {
        gf256mat_block2_prod_lazy(c, matA, matA_vec_byte, b, matA_n_vec);
        return;
    }
    uint8x16_t tmp[2];
    uint8_t *ptr = (uint8_t *)tmp;
    gf256mat_block2_prod_lazy(ptr, matA, matA_vec_byte, b, matA_n_vec - 1);
    gf256v_madd_neon( ptr, matA + matA_vec_byte * (matA_n_vec - 1), b[matA_n_vec - 1], matA_vec_byte );
    memcpy(c, ptr, matA_vec_byte);

}

static
void gf256mat_block3_prod_lazy(uint8_t *c, const uint8_t *mat, unsigned mat_vec_len,
                               const uint8_t *b, unsigned mat_n_vec ) {
    if ( 0 == mat_n_vec ) {
        memset(c, 0, 48);
        return;
    }

    uint8x16_t rl0, rl1, rl2;
    uint8x16_t rh0, rh1, rh2;
    rl0 = vdupq_n_u8(0);
    rl1 = vdupq_n_u8(0);
    rl2 = vdupq_n_u8(0);
    rh0 = vdupq_n_u8(0);
    rh1 = vdupq_n_u8(0);
    rh2 = vdupq_n_u8(0);

    const uint8_t *ptr = mat;
    for (unsigned j = 0; j < mat_n_vec; j++) {
        register uint8x16_t cc0 __asm__ ("v10") = vld1q_u8(ptr);
        register uint8x16_t cc1 __asm__ ("v11") = vld1q_u8(ptr+16);
        register uint8x16_t cc2 __asm__ ("v12") = vld1q_u8(ptr+32);
        ptr += mat_vec_len;
        register uint8x16_t bb  __asm__ ("v9") = vld1q_dup_u8(b+j);
        #ifdef   _PMULL_INLINEASM_
        register uint8x16_t tmp0 __asm__("v13");
        //rl0 ^= vmull_p8( vget_low_p8(cc0) , vget_low_p8(bb) );
        __asm__ volatile ( "pmull   v13.8h, v10.8b , v9.8b"   : "=w"(tmp0) : "w"(cc0), "w"(bb) );
        __asm__ volatile ( "pmull2  v10.8h, v10.16b , v9.16b" : "+w"(cc0) : "w"(bb) );
        rl0 ^= tmp0;
        rh0 ^= cc0;
        //rl1 ^= vmull_p8( vget_low_p8(cc1) , vget_low_p8(bb) );
        __asm__ volatile ( "pmull   v13.8h, v11.8b , v9.8b"   : "=w"(tmp0) : "w"(cc1), "w"(bb) );
        __asm__ volatile ( "pmull2  v10.8h, v11.16b , v9.16b" : "=w"(cc0) : "w"(cc1), "w"(bb) );
        rl1 ^= tmp0;
        rh1 ^= cc0;
        //rl2 ^= vmull_p8( vget_low_p8(cc2) , vget_low_p8(bb) );
        __asm__ volatile ( "pmull   v13.8h, v12.8b , v9.8b"   : "=w"(tmp0) : "w"(cc2), "w"(bb) );
        __asm__ volatile ( "pmull2  v10.8h, v12.16b , v9.16b" : "=w"(cc0) : "w"(cc2), "w"(bb) );
        rl2 ^= tmp0;
        rh2 ^= cc0;
        #else
        rl0 ^= vmull_p8( vget_low_p8(cc0), vget_low_p8(bb) );
        rh0 ^= vmull_high_p8( cc0, bb );
        rl1 ^= vmull_p8( vget_low_p8(cc1), vget_low_p8(bb) );
        rh1 ^= vmull_high_p8( cc1, bb );
        rl2 ^= vmull_p8( vget_low_p8(cc2), vget_low_p8(bb) );
        rh2 ^= vmull_high_p8( cc2, bb );
        #endif
    }
    // reduce
    uint8x16_t mask_f = vdupq_n_u8( 0xf );
    uint8x16_t tab_rd0 = vld1q_u8(__gf256_bit8_11_reduce);
    uint8x16_t tab_rd1 = vld1q_u8(__gf256_bit12_15_reduce);

    uint8x16_t r0 = _gf256v_reduce_tbl_neon( rl0, rh0, mask_f, tab_rd0, tab_rd1 );
    uint8x16_t r1 = _gf256v_reduce_tbl_neon( rl1, rh1, mask_f, tab_rd0, tab_rd1 );
    uint8x16_t r2 = _gf256v_reduce_tbl_neon( rl2, rh2, mask_f, tab_rd0, tab_rd1 );

    vst1q_u8( c, r0 );
    vst1q_u8( c + 16, r1 );
    vst1q_u8( c + 32, r2 );
}

#define BLOCK_LEN 3

#if 3 == BLOCK_LEN

static
void gf256mat_blockmat_prod_lazy( uint8x16_t *dest, const uint8_t *org_mat, unsigned mat_vec_byte, unsigned blk_vec_byte,
                                  const uint8_t *b, unsigned n_vec_ele ) {
    unsigned n_full_xmm = blk_vec_byte >> 4;
    unsigned n_rem_byte = blk_vec_byte & 15;

    if ( 0 == n_rem_byte) {
        if (1 == n_full_xmm) {
            gf256mat_block1_prod_lazy((uint8_t *)dest, org_mat, mat_vec_byte, b, n_vec_ele);
        } else if (2 == n_full_xmm) {
            gf256mat_block2_prod_lazy((uint8_t *)dest, org_mat, mat_vec_byte, b, n_vec_ele);
        } else {
            gf256mat_block3_prod_lazy((uint8_t *)dest, org_mat, mat_vec_byte, b, n_vec_ele);
        }
    } else {  // n_rem_byte != 0
        uint8x16_t temp0[BLOCK_LEN];
        uint8_t *ptr = (uint8_t *)temp0;
        if (0 == n_full_xmm) {
            gf256mat_block1_prod_lazy(ptr, org_mat, mat_vec_byte, b, n_vec_ele - 1);
        } else if (1 == n_full_xmm) {
            gf256mat_block2_prod_lazy(ptr, org_mat, mat_vec_byte, b, n_vec_ele - 1);
        } else {
            gf256mat_block3_prod_lazy(ptr, org_mat, mat_vec_byte, b, n_vec_ele - 1);
        }
        gf256v_madd_neon(ptr, org_mat + (n_vec_ele - 1)*mat_vec_byte, b[n_vec_ele - 1], blk_vec_byte);
        memcpy((uint8_t *)dest, ptr, blk_vec_byte);
    }
}

// assert mat_vec_len >= 8
void gf256mat_prod_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    if ( (16 >= matA_vec_byte) && (8 <= matA_vec_byte) ) {
        gf256mat_prod_16_neon(c, matA, matA_vec_byte, matA_n_vec, b);
        return;
    }
    if ( (32 >= matA_vec_byte) && (16 < matA_vec_byte) ) {
        gf256mat_prod_32_neon(c, matA, matA_vec_byte, matA_n_vec, b);
        return;
    }

    gf256v_set_zero_neon( c, matA_vec_byte );
    uint8x16_t blockmat_vec[BLOCK_LEN];
    while (matA_n_vec) {
        unsigned n_ele = matA_n_vec;
        unsigned vec_len_to_go = matA_vec_byte;
        while ( vec_len_to_go ) {
            unsigned block_len = (vec_len_to_go >= BLOCK_LEN * 16) ? BLOCK_LEN * 16 : vec_len_to_go;
            unsigned block_st_idx = matA_vec_byte - vec_len_to_go;

            if ( 48 == block_len ) {
                gf256mat_block3_prod_lazy((uint8_t *)blockmat_vec, matA + block_st_idx, matA_vec_byte, b, n_ele);
            } else {
                gf256mat_blockmat_prod_lazy(blockmat_vec, matA + block_st_idx, matA_vec_byte, block_len, b, n_ele);
            }
            gf256v_add_neon(c + block_st_idx, (uint8_t *)blockmat_vec, block_len);
            vec_len_to_go -= block_len;
        }

        matA_n_vec -= n_ele;
        b += n_ele;
        matA += n_ele * matA_vec_byte;
    }
}

#endif  // if 3 == BLOCK_LEN

#undef BLOCK_LEN

#else  // defined(_GF256_LAZY_REDUCE_)

#define BLOCK_LEN  8

void gf256mat_prod_neon( uint8_t *c, const uint8_t *matA, unsigned matA_vec_byte, unsigned matA_n_vec, const uint8_t *b ) {
    if ( (16 >= matA_vec_byte) && (8 <= matA_vec_byte) ) {
        gf256mat_prod_16_neon(c, matA, matA_vec_byte, matA_n_vec, b);
        return;
    }
    if ( (32 >= matA_vec_byte) && (16 < matA_vec_byte) ) {
        gf256mat_prod_32_neon(c, matA, matA_vec_byte, matA_n_vec, b);
        return;
    }

    uint8_t multabs[16 * 16 * 2];
    gf256v_set_zero_neon( c, matA_vec_byte );

    uint8x16_t blockmat_vec[BLOCK_LEN];

    while (matA_n_vec) {

        unsigned n_ele = (matA_n_vec >= 16) ? 16 : matA_n_vec;
        gf256v_generate_multabs_neon( multabs, b, n_ele );

        unsigned vec_len_to_go = matA_vec_byte;
        while ( vec_len_to_go ) {
            unsigned block_len = (vec_len_to_go >= BLOCK_LEN * 16) ? BLOCK_LEN * 16 : vec_len_to_go;
            unsigned block_st_idx = matA_vec_byte - vec_len_to_go;

            load_Qregs( blockmat_vec, c + block_st_idx, block_len );
            gf256mat_blockmat_madd_multab_neon( blockmat_vec, matA, matA_vec_byte, block_st_idx, block_len, (uint8x16_t *)multabs, n_ele );
            store_Qregs( c + block_st_idx, block_len, blockmat_vec );

            vec_len_to_go -= block_len;
        }

        matA_n_vec -= n_ele;
        b += n_ele;
        matA += n_ele * matA_vec_byte;
    }
}

#undef BLOCK_LEN

#endif  // defined(_GF256_LAZY_REDUCE_)




///////////////////////////////////////////////////////////////////////////



// need to do (h=44,w=48), (h=72,w=80), (h=96,w=112)

static
unsigned gf256mat_gauss_elim_row_echolen( uint8_t *mat, unsigned h, unsigned w, unsigned offset ) {
    PQOV_ALIGN uint8_t pivots[96];

    uint8_t r8 = 1;
    for (unsigned i = 0; i < h; i++) {
        unsigned idx = i + offset;
        unsigned st_idx = (idx >> 4) << 4;
        uint8_t *ai = mat + w * i;
        for (unsigned j = i; j < h; j++) {
            pivots[j] = mat[ w * j + idx ];
        }

#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF256_ < h) ? i + _GE_EARLY_STOP_STEPS_GF256_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            uint8_t m8 = !gf256_is_nonzero( pivots[i] );
            m8 = -m8;
            pivots[i] ^= m8 & pivots[j];
            uint8x16_t m128 = vdupq_n_u8( m8 );
            uint8_t *aj = mat + w * j;
            for (unsigned k = st_idx; k < w; k += 16) {
                vst1q_u8( ai + k, vld1q_u8(ai + k) ^ (m128 & vld1q_u8(aj + k)) );
            }
        }
        r8 &= gf256_is_nonzero( pivots[i] );
        pivots[i] = gf256_inv( pivots[i] );

        gf256v_mul_scalar_neon( ai + st_idx, pivots[i], w - st_idx ); // pivot row
        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *aj = mat + w * j;
            gf256v_madd_neon( aj + st_idx, ai + st_idx, pivots[j], w - st_idx );
        }
    }
    return r8;
}


unsigned gf256mat_gaussian_elim_neon(uint8_t *sqmat_a, uint8_t *constant, unsigned len) {
#define MAX_H  96
    PQOV_ALIGN uint8_t mat[MAX_H * (MAX_H + 16)];
#undef MAX_H

    unsigned height = len;
    unsigned width = ((len + 1 + 15) >> 4) << 4;
    unsigned offset = width - (len + 1);
    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width;
        for (unsigned j = 0; j < height; j++) {
            ai[offset + j] = sqmat_a[j * len + i];    // transpose since sqmat_a is col-major
        }
        ai[width - 1] = constant[i];
    }
    unsigned char r8 = gf256mat_gauss_elim_row_echolen( mat, height, width, offset);

    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width;
        memcpy( sqmat_a + i * len, ai + offset, len );   // output a row-major matrix
        constant[i] = ai[width - 1];
    }
    return r8;
}

void gf256mat_back_substitute_neon( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len) {
#define  MAX_H  (96)
    uint8_t column[MAX_H] = {0};
#undef  MAX_H
    for (int i = len - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            column[j] = sq_row_mat_a[j * len + i];    // row-major -> column-major, i.e., transpose
        }
        column[i] = 0;
        gf256v_madd_neon( constant, column, constant[i], len);
    }
}










