// SPDX-License-Identifier: CC0 OR Apache-2.0
//// @file blas_matrix.c
/// @brief The standard implementations for blas_matrix.h
///

#include "blas.h"
#include "blas_matrix_ref.h"
#include <stdint.h>
#include <string.h>

#include "config.h"   /// for the macro: _GE_CONST_TIME_CADD_EARLY_STOP_

#include "params.h"   /// for macro  _USE_GF16

/// This implementation depends on these vector functions :
///   0.  gf16v_mul_scalar
///       gf16v_madd
///       gf256v_add
///       gf256v_mul_scalar
///       gf256v_madd
///
///   1.  gf256v_conditional_add     for _gf(16/256)mat_gauss_elim()
///  these functions have to be defined in blas.h


#ifdef _USE_GF16

///////////  matrix-vector  multiplications  ////////////////////////////////

void gf16mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    gf256v_set_zero(c, n_A_vec_byte);
    for (unsigned i = 0; i < n_A_width; i++) {
        uint8_t bb = gf16v_get_ele(b, i);
        gf16v_madd(c, matA, bb, n_A_vec_byte);
        matA += n_A_vec_byte;
    }
}

//////////////////    Gaussian elimination + Back substitution for solving linear equations  //////////////////

#define _GF16_TRANSPOSE_

#ifdef _GF16_TRANSPOSE_
static
void gf256mat_transpose_32x32( uint8_t *mat, unsigned vec_len, const uint8_t *src_mat, unsigned src_vec_len ) {
    for (unsigned i = 0; i < 32; i++) {
        for (unsigned j = 0; j < 32; j++) {
            mat[i * vec_len + j] = src_mat[j * src_vec_len + i];
        }
    }
}

static
void gf16mat_64x64_sqmat_transpose(uint8_t *dest_mat, unsigned dest_vec_len, const uint8_t *src_sqmat ) {
    gf256mat_transpose_32x32( dest_mat, dest_vec_len * 2, src_sqmat, 64 );                   // transpose even rows
    gf256mat_transpose_32x32( dest_mat + dest_vec_len, dest_vec_len * 2, src_sqmat + 32, 64 ); // transpose odd rows
    // transpose 2x2 4-bit blocks
    for (int i = 0; i < 64; i += 2) {
        uint32_t *row1 = (uint32_t *)(dest_mat + i    * dest_vec_len);
        uint32_t *row2 = (uint32_t *)(dest_mat + (i + 1) * dest_vec_len);
        for (int j = 0; j < 8; j++) {
            uint32_t in1 = row1[j];
            uint32_t in2 = row2[j];
            row1[j] = (in1 & 0x0f0f0f0f) ^ ((in2 & 0x0f0f0f0f) << 4);
            row2[j] = (in2 & 0xf0f0f0f0) ^ ((in1 & 0xf0f0f0f0) >> 4);
        }
    }
}

static
void gf16mat_sqmat_transpose(uint8_t *dest_mat, unsigned dest_vec_len, const uint8_t *src_sqmat, unsigned src_vec_len, unsigned n_vec ) {
    if ( 64 == n_vec && 32 == src_vec_len ) {
        gf16mat_64x64_sqmat_transpose(dest_mat, dest_vec_len, src_sqmat);
        return;
    }

    for (unsigned i = 0; i < n_vec; i++) {
        uint8_t *ai = dest_mat + i * dest_vec_len;
        for (unsigned j = 0; j < n_vec; j++) {
            gf16v_set_ele( ai, j, gf16v_get_ele(src_sqmat + j * src_vec_len, i) );
        }
    }
}
#endif  // #ifdef _GF16_TRANSPOSE_

static
unsigned gf16mat_gauss_elim_row_echolen( uint8_t *mat, unsigned h, unsigned w_byte ) {
    unsigned r8 = 1;

    for (unsigned i = 0; i < h; i++) {
        uint8_t *ai = mat + w_byte * i;
        //unsigned i_start = i-(i&(_BLAS_UNIT_LEN_-1));
        unsigned i_start = i >> 1;
#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF16_ < h) ? i + _GE_EARLY_STOP_STEPS_GF16_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            uint8_t *aj = mat + w_byte * j;
            gf256v_conditional_add( ai + i_start, !gf16_is_nonzero(gf16v_get_ele(ai, i)), aj + i_start, w_byte - i_start );
        }
        uint8_t pivot = gf16v_get_ele(ai, i);
        r8 &= gf16_is_nonzero(pivot);
        pivot = gf16_inv( pivot );
        gf16v_mul_scalar( ai + i_start, pivot, w_byte - i_start );
        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *aj = mat + w_byte * j;
            gf16v_madd( aj + i_start, ai + i_start, gf16v_get_ele(aj, i), w_byte - i_start );
        }
    }
    return r8;
}

unsigned gf16mat_gaussian_elim_ref(uint8_t *sqmat_a, uint8_t *constant, unsigned len) {
    //const unsigned MAX_H=64;
#define MAX_H  (64)
    uint8_t mat[MAX_H * (MAX_H + 4)] = {0};
#undef MAX_H

    unsigned height = len;
    unsigned width_o  = len / 2;
    unsigned width_n  = width_o + 4;

    #ifdef _GF16_TRANSPOSE_
    gf16mat_sqmat_transpose(mat, width_n, sqmat_a, width_o, height );
    for (unsigned i = 0; i < height; i++) {
        mat[i * width_n + width_o] = gf16v_get_ele(constant, i);
    }
    #else
    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width_n;
        for (unsigned j = 0; j < height; j++) {
            // transpose since sqmat_a is col-major
            gf16v_set_ele( ai, j, gf16v_get_ele(sqmat_a + j * width_o, i) );
        }
        ai[width_o] = gf16v_get_ele(constant, i);
    }
    #endif

    unsigned char r8 = gf16mat_gauss_elim_row_echolen( mat, height, width_n );

    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width_n;
        memcpy( sqmat_a + i * width_o, ai, width_o ); // output a row-major matrix
        gf16v_set_ele(constant, i, ai[width_o] );
    }
    return r8;
}

void gf16mat_back_substitute_ref( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len) {
    #ifdef _GF16_TRANSPOSE_
#define MAX_H  (64)
    uint8_t mat[MAX_H * (MAX_H / 2)] = {0};
#undef MAX_H
    unsigned width_byte = (len + 1) / 2;
    gf16mat_sqmat_transpose( mat, width_byte, sq_row_mat_a, width_byte, len );
    for (unsigned i = len - 1; i > 0; i--) {
        uint8_t *col = mat + i * width_byte;
        gf16v_set_ele( col, i, 0 );
        gf16v_madd( constant, col, gf16v_get_ele(constant, i), (i + 1) / 2 );
    }
    #else
#define MAX_H  (64)
    uint8_t column[MAX_H] = {0};
#undef MAX_H
    unsigned width_byte = (len + 1) / 2;
    for (int i = len - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            // row-major -> column-major, i.e., transpose
            uint8_t ele = gf16v_get_ele( sq_row_mat_a + width_byte * j, i );
            gf16v_set_ele( column, j, ele );
        }
        gf16v_set_ele( column, i, 0 );    // pad to last byte
        gf16v_madd( constant, column, gf16v_get_ele(constant, i), (i + 1) / 2 );
    }
    #endif
}


#else   // #ifdef _USE_GF16


///////////  matrix-vector  multiplications  ////////////////////////////////

void gf256mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    gf256v_set_zero(c, n_A_vec_byte);
    for (unsigned i = 0; i < n_A_width; i++) {
        gf256v_madd(c, matA, b[i], n_A_vec_byte);
        matA += n_A_vec_byte;
    }
}

//////////////////    Gaussian elimination + Back substitution for solving linear equations  //////////////////

static
unsigned gf256mat_gauss_elim_row_echolen( uint8_t *mat, unsigned h, unsigned w ) {
    unsigned r8 = 1;

    for (unsigned i = 0; i < h; i++) {
        uint8_t *ai = mat + w * i;
        //unsigned i_start = i-(i&(_BLAS_UNIT_LEN_-1));
        unsigned i_start = i;

#if defined( _GE_CONST_TIME_CADD_EARLY_STOP_ )   // defined in config.h
        unsigned stop = (i + _GE_EARLY_STOP_STEPS_GF256_ < h) ? i + _GE_EARLY_STOP_STEPS_GF256_ : h;
        for (unsigned j = i + 1; j < stop; j++) {
#else
        for (unsigned j = i + 1; j < h; j++) {
#endif
            uint8_t *aj = mat + w * j;
            gf256v_conditional_add( ai + i_start, !gf256_is_nonzero(ai[i]), aj + i_start, w - i_start );
        }
        r8 &= gf256_is_nonzero(ai[i]);
        uint8_t pivot = ai[i];
        pivot = gf256_inv( pivot );
        gf256v_mul_scalar( ai + i_start, pivot, w - i_start );
        for (unsigned j = i + 1; j < h; j++) {
            uint8_t *aj = mat + w * j;
            gf256v_madd( aj + i_start, ai + i_start, aj[i], w - i_start );
        }
    }
    return r8;
}

unsigned gf256mat_gaussian_elim_ref(uint8_t *sqmat_a, uint8_t *constant, unsigned len) {
    #define MAX_H 96
    uint8_t mat[MAX_H * (MAX_H + 4)] = {0};
    #undef MAX_H

    unsigned height = len;
    unsigned width  = len + 4;

    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width;
        for (unsigned j = 0; j < height; j++) {
            ai[j] = sqmat_a[j * len + i];    // transpose since sqmat_a is col-major
        }
        ai[height] = constant[i];
    }
    unsigned char r8 = gf256mat_gauss_elim_row_echolen( mat, height, width );

    for (unsigned i = 0; i < height; i++) {
        uint8_t *ai = mat + i * width;
        memcpy( sqmat_a + i * len, ai, len );     // output a row-major matrix
        constant[i] = ai[len];
    }
    return r8;
}

void gf256mat_back_substitute_ref( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len) {
    #define MAX_H 96
    uint8_t column[MAX_H];
    #undef MAX_H
    for (int i = len - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            column[j] = sq_row_mat_a[j * len + i];    // row-major -> column-major, i.e., transpose
        }
        gf256v_madd( constant, column, constant[i], i );
    }
}


#endif  // #ifdef _USE_GF16



