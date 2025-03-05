// SPDX-License-Identifier: CC0 OR Apache-2.0
///  @file parallel_matrix_op.c
///  @brief the standard implementations for functions in parallel_matrix_op.h
///
///  the standard implementations for functions in parallel_matrix_op.h
///

#include "blas_comm.h"
#include "blas.h"
#include "params.h"
#include "utils_malloc.h"

#include "parallel_matrix_op.h"



#ifdef _USE_GF16
/////////////////  Section: matrix multiplications  ///////////////////////////////

void batch_trimat_madd_gf16( unsigned char *bC, const unsigned char *btriA,
                             const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_V       (96)
#define MAX_O_BYTE  (32)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t B2[MAX_V * MAX_O_BYTE];
#undef MAX_V
#undef MAX_O_BYTE

    for (unsigned i = 0; i < Bwidth; i++) {
        for (unsigned j = 0; j < size_Bcolvec - 1; j++) {
            B2[i * size_Bcolvec + j] = (B[i * size_Bcolvec + j] >> 4) | (B[i * size_Bcolvec + j + 1] << 4);
        }
        B2[i * size_Bcolvec + size_Bcolvec - 1] = B[i * size_Bcolvec + size_Bcolvec - 1] >> 4;
    }

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i += 2) {
        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod( tmp_c, btriA, size_batch, Aheight - i, B + j * size_Bcolvec + (i / 2) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
        btriA += size_batch * (Aheight - i);
        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod( tmp_c, btriA, size_batch, Aheight - i - 1, B2 + j * size_Bcolvec + (i / 2) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
        btriA += size_batch * (Aheight - i - 1);
    }
}

// This function is only used in calssic mode.
void batch_trimatTr_madd_gf16( unsigned char *bC, const unsigned char *btriA,
                               const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (32)
#define MAX_V      (96)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memcpy( tmp_Arow + i * size_batch, ptr, size_batch );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod( tmp_c, tmp_Arow, size_batch, i + 1, B + (j * size_Bcolvec) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

void batch_2trimat_madd_gf16( unsigned char *bC, const unsigned char *btriA,
                              const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (32)
#define MAX_V      (96)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memset( tmp_Arow + i * size_batch, 0, size_batch );
        ptr += size_batch;
        memcpy( tmp_Arow + (i + 1)*size_batch, ptr, size_batch * (Aheight - i - 1) );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod( tmp_c, tmp_Arow, size_batch, Aheight, B + (j * size_Bcolvec) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

////////////////



void batch_upper_matTr_x_mat_gf16( unsigned char *bC, const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                                   const unsigned char *bB, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O  (64)
#define MAX_O_BYTE  (32)
    PQOV_ALIGN uint8_t row[MAX_O * MAX_O_BYTE]; /// XXX: buffer for maximum row
#undef MAX_O_BYTE
#undef MAX_O
    unsigned Atr_height = Awidth;
    unsigned Atr_width  = Aheight;
    for (unsigned i = 0; i < Atr_height; i++) {
        gf16mat_prod( row, bB, Bwidth * size_batch, Atr_width, A_to_tr + size_Acolvec * i );
        uint8_t *ptr = bC + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            gf256v_add( ptr, row + size_batch * j, size_batch );
            ptr += (Bwidth - j - 1) * size_batch;
        }
        memcpy( ptr, row + size_batch * i, size_batch * (Bwidth - i) );
    }
}


////////////////////  Section: "quadratric" matrix evaluation  ///////////////////////////////

#if defined(_MUL_WITH_MULTAB_)
static void batch_quad_trimat_eval_multab_gf16( unsigned char *y, const unsigned char *trimat, const unsigned char *multabs_x, unsigned dim, unsigned size_batch );

void batch_quad_trimat_eval_gf16( unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned dim, unsigned size_batch ) {
    #define MAX_V      (96)
    PQOV_ALIGN uint8_t multabs[(MAX_V) * 32];
    #undef MAX_V
    gf16v_generate_multabs( multabs, x, dim );
    batch_quad_trimat_eval_multab_gf16( y , trimat , multabs , dim , size_batch );
}

#else

void batch_quad_trimat_eval_gf16( unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned dim, unsigned size_batch ) {
#define MAX_O_BYTE      (64/2)
#define MAX_V_BYTE      (96/2)
    PQOV_ALIGN uint8_t x2[MAX_V_BYTE];
    PQOV_ALIGN uint8_t tmp[MAX_O_BYTE];
#undef  MAX_O_BYTE
#undef  MAX_V_BYTE
    for (unsigned j = 0; j < (dim / 2) - 1; j++) {
        x2[j] = (x[j] >> 4) | (x[j + 1] << 4);
    }
    x2[(dim / 2) - 1] = x[(dim / 2) - 1] >> 4;

    gf256v_set_zero( y, size_batch );
    for (unsigned i = 0; i < dim; i += 2) {
        gf16mat_prod( tmp, trimat, size_batch, dim - i, x + (i / 2) );
        gf16v_madd( y, tmp, gf16v_get_ele(x, i), size_batch );
        trimat += (dim - i) * size_batch;

        gf16mat_prod( tmp, trimat, size_batch, dim - i - 1, x2 + (i / 2) );
        gf16v_madd( y, tmp, gf16v_get_ele(x, i + 1), size_batch );
        trimat += (dim - i - 1) * size_batch;
    }
}

#endif


#if defined(_MUL_WITH_MULTAB_)

void batch_trimat_madd_multab_gf16( unsigned char *bC, const unsigned char *btriA,
                                    const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
    (void)size_Bcolvec; // un-used variable
    #if defined(_BLAS_NEON_)
    const unsigned w_multab = 4;
    #else
    const unsigned w_multab = 5;
    #endif

#define MAX_O_BYTE  (32)
    uint8_t tmp_c[MAX_O_BYTE];
#undef MAX_O_BYTE

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod_multab( tmp_c, btriA, size_batch, Aheight - i, B + ((j * Bheight + i) << w_multab) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
        btriA += size_batch * (Aheight - i);
    }
}

// This function is only used in calssic mode.
void batch_trimatTr_madd_multab_gf16( unsigned char *bC, const unsigned char *btriA,
                                      const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
    (void)size_Bcolvec; // un-used variable
    #if defined(_BLAS_NEON_)
    const unsigned w_multab = 4;
    #else
    const unsigned w_multab = 5;
    #endif

#define MAX_O_BYTE  (32)
#define MAX_V      (96)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memcpy( tmp_Arow + i * size_batch, ptr, size_batch );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod_multab( tmp_c, tmp_Arow, size_batch, i + 1, B + (j * Bheight << w_multab) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

void batch_2trimat_madd_multab_gf16( unsigned char *bC, const unsigned char *btriA,
                                     const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {

    (void)size_Bcolvec; // un-used variable
    #if defined(_BLAS_NEON_)
    const unsigned w_multab = 4;
    #else
    const unsigned w_multab = 5;
    #endif

#define MAX_O_BYTE  (32)
#define MAX_V      (96)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memset( tmp_Arow + i * size_batch, 0, size_batch );
        ptr += size_batch;
        memcpy( tmp_Arow + (i + 1)*size_batch, ptr, size_batch * (Aheight - i - 1) );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf16mat_prod_multab( tmp_c, tmp_Arow, size_batch, Aheight, B + (j * Bheight << w_multab) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

////////////////


void batch_upper_matTr_x_mat_multab_gf16( unsigned char *bC, const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
        const unsigned char *bB, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O  (64)
#define MAX_O_BYTE  (32)
    PQOV_ALIGN uint8_t row[MAX_O * MAX_O_BYTE]; /// XXX: buffer for maximum row
#undef MAX_O_BYTE
#undef MAX_O
    (void)size_Acolvec; // un-used variable
    #if defined(_BLAS_NEON_)
    const unsigned w_multab = 4;
    #else
    const unsigned w_multab = 5;
    #endif
    unsigned Atr_height = Awidth;
    unsigned Atr_width  = Aheight;
    for (unsigned i = 0; i < Atr_height; i++) {
        gf16mat_prod_multab( row, bB, Bwidth * size_batch, Atr_width, A_to_tr + (i * Aheight << w_multab) );
        uint8_t *ptr = bC + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            gf256v_add( ptr, row + size_batch * j, size_batch );
            ptr += (Bwidth - j - 1) * size_batch;
        }
        memcpy( ptr, row + size_batch * i, size_batch * (Bwidth - i) );
    }
}


static
void batch_quad_trimat_eval_multab_gf16( unsigned char *y, const unsigned char *trimat, const unsigned char *multab_x, unsigned dim, unsigned size_batch ) {
///    assert( dim <= 128 );
///    assert( size_batch <= 128 );
    PQOV_ALIGN unsigned char tmp[256];

    gf256v_set_zero( y, size_batch );
    for (unsigned i = 0; i < dim; i++) {
        gf16mat_prod_multab( tmp, trimat, size_batch, dim - i, multab_x );
        gf16v_madd_multab( y, tmp, multab_x, size_batch );

        trimat += (dim - i) * size_batch;
// XXX: base function has to know behaivors of derived functions.
        #if defined(_BLAS_NEON_)
        multab_x += 16;
        #else
        multab_x += 32;
        #endif
    }
}

#endif  // #if defined(_MUL_WITH_MULTAB_)

#else  //#ifdef _USE_GF16


/////////////////  Section: matrix multiplications  ///////////////////////////////
void batch_trimat_madd_gf256( unsigned char *bC, const unsigned char *btriA,
                              const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (96)
    uint8_t tmp_c[MAX_O_BYTE];
#undef MAX_O_BYTE
// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        for (unsigned j = 0; j < Bwidth; j++) {
            gf256mat_prod( tmp_c, btriA, size_batch, Aheight - i, B + j * size_Bcolvec + i );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
        btriA += size_batch * (Aheight - i);
    }
}

// This function is only used in calssic mode.
void batch_trimatTr_madd_gf256( unsigned char *bC, const unsigned char *btriA,
                                const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (96)
#define MAX_V      (148)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memcpy( tmp_Arow + i * size_batch, ptr, size_batch );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf256mat_prod( tmp_c, tmp_Arow, size_batch, i + 1, B + j * size_Bcolvec );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}



void batch_2trimat_madd_gf256( unsigned char *bC, const unsigned char *btriA,
                               const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (96)
#define MAX_V      (148)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memset( tmp_Arow + i * size_batch, 0, size_batch );
        ptr += size_batch;
        memcpy( tmp_Arow + (i + 1)*size_batch, ptr, size_batch * (Aheight - i - 1) );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf256mat_prod( tmp_c, tmp_Arow, size_batch, Aheight, B + j * size_Bcolvec );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

////////////////



void batch_upper_matTr_x_mat_gf256( unsigned char *bC, const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
                                    const unsigned char *bB, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O  (96)
    PQOV_ALIGN uint8_t row[MAX_O * MAX_O]; /// XXX: buffer for maximum row
#undef MAX_O

    unsigned Atr_height = Awidth;
    unsigned Atr_width  = Aheight;
    for (unsigned i = 0; i < Atr_height; i++) {
        gf256mat_prod( row, bB, Bwidth * size_batch, Atr_width, A_to_tr + size_Acolvec * i );
        uint8_t *ptr = bC + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            gf256v_add( ptr, row + size_batch * j, size_batch );
            ptr += (Bwidth - j - 1) * size_batch;
        }
        memcpy( ptr, row + size_batch * i, size_batch * (Bwidth - i) );
    }
}


////////////////////  Section: "quadratric" matrix evaluation  ///////////////////////////////

#if defined(_MUL_WITH_MULTAB_)
static void batch_quad_trimat_eval_multab_gf256( unsigned char *y, const unsigned char *trimat, const unsigned char *multab_x, unsigned dim, unsigned size_batch );

void batch_quad_trimat_eval_gf256( unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned dim, unsigned size_batch ) {
    #define MAX_V      (148)
    PQOV_ALIGN uint8_t multabs[(MAX_V) * 32];
    #undef MAX_V
    gf256v_generate_multabs( multabs, x, dim );
    batch_quad_trimat_eval_multab_gf256( y , trimat , multabs , dim , size_batch );
}

#else

void batch_quad_trimat_eval_gf256( unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned dim, unsigned size_batch ) {
///
///    assert( dim <= 256 );
///    assert( size_batch <= 256 );
    unsigned char tmp[256];

    gf256v_set_zero( y, size_batch );
    for (unsigned i = 0; i < dim - 15; i++) {
        gf256mat_prod( tmp, trimat, size_batch, dim - i, x + i );
        gf256v_madd( y, tmp, x[i], size_batch );
        trimat += (dim - i) * size_batch;
    }
    uint8_t quad_terms[128] = {0};
    int idx = 0;
    for (unsigned i = dim - 15; i < dim; i++) {
        gf256v_madd(quad_terms + idx, x + i, x[i], dim - i );
        idx += dim - i;
    }
    gf256mat_prod( tmp, trimat, size_batch, 120, quad_terms );      // 1 + 2 + ... + 15 = 120
    gf256v_add( y, tmp, size_batch );
}
#endif

#if defined(_MUL_WITH_MULTAB_)

void batch_trimat_madd_multab_gf256( unsigned char *bC, const unsigned char *btriA,
                                     const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (96)
    uint8_t tmp_c[MAX_O_BYTE];
#undef MAX_V

// access fixed positions of destination matrix C
    (void)size_Bcolvec; // un-used variable
    const unsigned w_multab = 5;
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        for (unsigned j = 0; j < Bwidth; j++) {
            gf256mat_prod_multab( tmp_c, btriA, size_batch, Aheight - i, B + ((j * Bheight + i) << w_multab) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
        btriA += size_batch * (Aheight - i);
    }
}

// This function is only used in calssic mode.
void batch_trimatTr_madd_multab_gf256( unsigned char *bC, const unsigned char *btriA,
                                       const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (96)
#define MAX_V      (148)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    (void)size_Bcolvec; // un-used variable
    const unsigned w_multab = 5;
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memcpy( tmp_Arow + i * size_batch, ptr, size_batch );
        for (unsigned j = 0; j < Bwidth; j++) {
            gf256mat_prod_multab( tmp_c, tmp_Arow, size_batch, i + 1, B + (j * Bheight << w_multab) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

void batch_2trimat_madd_multab_gf256( unsigned char *bC, const unsigned char *btriA,
                                      const unsigned char *B, unsigned Bheight, unsigned size_Bcolvec, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O_BYTE  (96)
#define MAX_V      (148)
    uint8_t tmp_c[MAX_O_BYTE];
    uint8_t tmp_Arow[MAX_V * MAX_O_BYTE];
#undef MAX_O_BYTE
#undef MAX_V

// access fixed positions of destination matrix C
    (void)size_Bcolvec; // un-used variable
    const unsigned w_multab = 5;
    unsigned Aheight = Bheight;
    for (unsigned i = 0; i < Aheight; i++) {
        const uint8_t *ptr = btriA + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            memcpy( tmp_Arow + j * size_batch, ptr, size_batch );
            ptr += (Aheight - j - 1) * size_batch;
        }
        memset( tmp_Arow + i * size_batch, 0, size_batch );
        ptr += size_batch;
        memcpy( tmp_Arow + (i + 1)*size_batch, ptr, size_batch * (Aheight - i - 1) );

        for (unsigned j = 0; j < Bwidth; j++) {
            gf256mat_prod_multab( tmp_c, tmp_Arow, size_batch, Aheight, B + (j * Bheight << w_multab) );
            gf256v_add( bC, tmp_c, size_batch);
            bC += size_batch;
        }
    }
}

////////////////


void batch_upper_matTr_x_mat_multab_gf256( unsigned char *bC, const unsigned char *A_to_tr, unsigned Aheight, unsigned size_Acolvec, unsigned Awidth,
        const unsigned char *bB, unsigned Bwidth, unsigned size_batch ) {
#define MAX_O  (96)
    PQOV_ALIGN uint8_t row[MAX_O * MAX_O]; /// XXX: buffer for maximum row
#undef MAX_O
    (void)size_Acolvec; // un-used variable
    const unsigned w_multab = 5;
    unsigned Atr_height = Awidth;
    unsigned Atr_width  = Aheight;
    for (unsigned i = 0; i < Atr_height; i++) {
        gf256mat_prod_multab( row, bB, Bwidth * size_batch, Atr_width, A_to_tr + (i * Aheight << w_multab) );

        uint8_t *ptr = bC + i * size_batch;
        for (unsigned j = 0; j < i; j++) {
            gf256v_add( ptr, row + size_batch * j, size_batch );
            ptr += (Bwidth - j - 1) * size_batch;
        }
        memcpy( ptr, row + size_batch * i, size_batch * (Bwidth - i) );
    }
}

static
void batch_quad_trimat_eval_multab_gf256( unsigned char *y, const unsigned char *trimat, const unsigned char *multab_x, unsigned dim, unsigned size_batch ) {
///
///    assert( dim <= 256 );
///    assert( size_batch <= 256 );
    PQOV_ALIGN unsigned char tmp[256];

    gf256v_set_zero( y, size_batch );
    for (unsigned i = 0; i < dim; i++) {
        gf256mat_prod_multab( tmp, trimat, size_batch, dim - i, multab_x );
        gf256v_madd_multab( y, tmp, multab_x, size_batch );

        multab_x += 32;
        trimat += (dim - i) * size_batch;
    }
}

#endif  // #if defined(_MUL_WITH_MULTAB_)
#endif  // #if defined(_USE_GF16_)
