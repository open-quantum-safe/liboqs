///  @file parallel_matrix_op.c
///  @brief the standard implementations for functions in parallel_matrix_op.h
///
///  the standard implementations for functions in parallel_matrix_op.h
///

#include "parallel_matrix_op.h"
#include "blas.h"
#include "blas_comm.h"

///
/// @brief  Calculate the corresponding index in an array for an upper-triangle(UT) matrix.
///
/// @param[in]  i_row     - the i-th row in an upper-triangle matrix.
/// @param[in]  j_col     - the j-th column in an upper-triangle matrix.
/// @param[in]  dim       - the dimension of the upper-triangle matrix, i.e., an dim x dim matrix.
/// @return  the corresponding index in an array storage.
///
unsigned int PQCLEAN_RAINBOWIACYCLIC_CLEAN_idx_of_trimat(unsigned int i_row, unsigned int j_col, unsigned int dim) {
    return (dim + dim - i_row + 1) * i_row / 2 + j_col - i_row;
}

///
/// @brief  Calculate the corresponding index in an array for an upper-triangle or lower-triangle matrix.
///
/// @param[in]  i_row     - the i-th row in a triangle matrix.
/// @param[in]  j_col     - the j-th column in a triangle matrix.
/// @param[in]  dim       - the dimension of the triangle matrix, i.e., an dim x dim matrix.
/// @return    the corresponding index in an array storage.
///
static inline unsigned int idx_of_2trimat(unsigned int i_row, unsigned int j_col, unsigned int n_var) {
    if (i_row > j_col) {
        return PQCLEAN_RAINBOWIACYCLIC_CLEAN_idx_of_trimat(j_col, i_row, n_var);
    }
    return PQCLEAN_RAINBOWIACYCLIC_CLEAN_idx_of_trimat(i_row, j_col, n_var);
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_UpperTrianglize(unsigned char *btriC, const unsigned char *bA, unsigned int Awidth, unsigned int size_batch) {
    unsigned char *runningC = btriC;
    unsigned int Aheight = Awidth;
    for (unsigned int i = 0; i < Aheight; i++) {
        for (unsigned int j = 0; j < i; j++) {
            unsigned int idx = PQCLEAN_RAINBOWIACYCLIC_CLEAN_idx_of_trimat(j, i, Aheight);
            gf256v_add(btriC + idx * size_batch, bA + size_batch * (i * Awidth + j), size_batch);
        }
        gf256v_add(runningC, bA + size_batch * (i * Awidth + i), size_batch * (Aheight - i));
        runningC += size_batch * (Aheight - i);
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_trimat_madd_gf16(unsigned char *bC, const unsigned char *btriA,
        const unsigned char *B, unsigned int Bheight, unsigned int size_Bcolvec, unsigned int Bwidth, unsigned int size_batch) {
    unsigned int Awidth = Bheight;
    unsigned int Aheight = Awidth;
    for (unsigned int i = 0; i < Aheight; i++) {
        for (unsigned int j = 0; j < Bwidth; j++) {
            for (unsigned int k = 0; k < Bheight; k++) {
                if (k < i) {
                    continue;
                }
                gf16v_madd(bC, &btriA[(k - i) * size_batch], PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(&B[j * size_Bcolvec], k), size_batch);
            }
            bC += size_batch;
        }
        btriA += (Aheight - i) * size_batch;
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_trimatTr_madd_gf16(unsigned char *bC, const unsigned char *btriA,
        const unsigned char *B, unsigned int Bheight, unsigned int size_Bcolvec, unsigned int Bwidth, unsigned int size_batch) {
    unsigned int Aheight = Bheight;
    for (unsigned int i = 0; i < Aheight; i++) {
        for (unsigned int j = 0; j < Bwidth; j++) {
            for (unsigned int k = 0; k < Bheight; k++) {
                if (i < k) {
                    continue;
                }
                gf16v_madd(bC, &btriA[size_batch * (PQCLEAN_RAINBOWIACYCLIC_CLEAN_idx_of_trimat(k, i, Aheight))], PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(&B[j * size_Bcolvec], k), size_batch);
            }
            bC += size_batch;
        }
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_2trimat_madd_gf16(unsigned char *bC, const unsigned char *btriA,
        const unsigned char *B, unsigned int Bheight, unsigned int size_Bcolvec, unsigned int Bwidth, unsigned int size_batch) {
    unsigned int Aheight = Bheight;
    for (unsigned int i = 0; i < Aheight; i++) {
        for (unsigned int j = 0; j < Bwidth; j++) {
            for (unsigned int k = 0; k < Bheight; k++) {
                if (i == k) {
                    continue;
                }
                gf16v_madd(bC, &btriA[size_batch * (idx_of_2trimat(i, k, Aheight))], PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(&B[j * size_Bcolvec], k), size_batch);
            }
            bC += size_batch;
        }
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_matTr_madd_gf16(unsigned char *bC, const unsigned char *A_to_tr, unsigned int Aheight, unsigned int size_Acolvec, unsigned int Awidth,
        const unsigned char *bB, unsigned int Bwidth, unsigned int size_batch) {
    unsigned int Atr_height = Awidth;
    unsigned int Atr_width = Aheight;
    for (unsigned int i = 0; i < Atr_height; i++) {
        for (unsigned int j = 0; j < Atr_width; j++) {
            gf16v_madd(bC, &bB[j * Bwidth * size_batch], PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(&A_to_tr[size_Acolvec * i], j), size_batch * Bwidth);
        }
        bC += size_batch * Bwidth;
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_bmatTr_madd_gf16(unsigned char *bC, const unsigned char *bA_to_tr, unsigned int Awidth_before_tr,
        const unsigned char *B, unsigned int Bheight, unsigned int size_Bcolvec, unsigned int Bwidth, unsigned int size_batch) {
    const unsigned char *bA = bA_to_tr;
    unsigned int Aheight = Awidth_before_tr;
    for (unsigned int i = 0; i < Aheight; i++) {
        for (unsigned int j = 0; j < Bwidth; j++) {
            for (unsigned int k = 0; k < Bheight; k++) {
                gf16v_madd(bC, &bA[size_batch * (i + k * Aheight)], PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(&B[j * size_Bcolvec], k), size_batch);
            }
            bC += size_batch;
        }
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_mat_madd_gf16(unsigned char *bC, const unsigned char *bA, unsigned int Aheight,
        const unsigned char *B, unsigned int Bheight, unsigned int size_Bcolvec, unsigned int Bwidth, unsigned int size_batch) {
    unsigned int Awidth = Bheight;
    for (unsigned int i = 0; i < Aheight; i++) {
        for (unsigned int j = 0; j < Bwidth; j++) {
            for (unsigned int k = 0; k < Bheight; k++) {
                gf16v_madd(bC, &bA[k * size_batch], PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(&B[j * size_Bcolvec], k), size_batch);
            }
            bC += size_batch;
        }
        bA += (Awidth) * size_batch;
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_quad_recmat_eval_gf16(unsigned char *z, const unsigned char *y, unsigned int dim_y, const unsigned char *mat,
        const unsigned char *x, unsigned int dim_x, unsigned int size_batch) {
    unsigned char tmp[128];

    unsigned char _x[128];
    for (unsigned int i = 0; i < dim_x; i++) {
        _x[i] = PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(x, i);
    }
    unsigned char _y[128];
    for (unsigned int i = 0; i < dim_y; i++) {
        _y[i] = PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(y, i);
    }

    PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf256v_set_zero(z, size_batch);
    for (unsigned int i = 0; i < dim_y; i++) {
        PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf256v_set_zero(tmp, size_batch);
        for (unsigned int j = 0; j < dim_x; j++) {
            gf16v_madd(tmp, mat, _x[j], size_batch);
            mat += size_batch;
        }
        gf16v_madd(z, tmp, _y[i], size_batch);
    }
}

void PQCLEAN_RAINBOWIACYCLIC_CLEAN_batch_quad_trimat_eval_gf16(unsigned char *y, const unsigned char *trimat, const unsigned char *x, unsigned int dim, unsigned int size_batch) {
    unsigned char tmp[256];

    unsigned char _x[256];
    for (unsigned int i = 0; i < dim; i++) {
        _x[i] = PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_get_ele(x, i);
    }

    PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf256v_set_zero(y, size_batch);
    for (unsigned int i = 0; i < dim; i++) {
        PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf256v_set_zero(tmp, size_batch);
        for (unsigned int j = i; j < dim; j++) {
            gf16v_madd(tmp, trimat, _x[j], size_batch);
            trimat += size_batch;
        }
        gf16v_madd(y, tmp, _x[i], size_batch);
    }
}
