/// @file blas_comm.c
/// @brief The standard implementations for blas_comm.h
///

#include "blas_comm.h"
#include "blas.h"
#include "gf.h"
#include "rainbow_config.h"

#include <stdint.h>
#include <string.h>

void PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_set_zero(uint8_t *b, unsigned int _num_byte) {
    for (size_t i = 0; i < _num_byte; i++) {
        b[i] = 0;
    }
}
/// @brief get an element from GF(256) vector .
///
/// @param[in]  a         - the input vector a.
/// @param[in]  i         - the index in the vector a.
/// @return  the value of the element.
///
uint8_t PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_get_ele(const uint8_t *a, unsigned int i) {
    return a[i];
}

unsigned int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_is_zero(const uint8_t *a, unsigned int _num_byte) {
    uint8_t r = 0;
    while (_num_byte--) {
        r |= a[0];
        a++;
    }
    return (0 == r);
}

/// polynomial multplication
/// School boook
void PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_polymul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned int _num) {
    PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_set_zero(c, _num * 2 - 1);
    for (unsigned int i = 0; i < _num; i++) {
        PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_madd(c + i, a, b[i], _num);
    }
}

static void gf256mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned int n_A_vec_byte, unsigned int n_A_width, const uint8_t *b) {
    PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_set_zero(c, n_A_vec_byte);
    for (unsigned int i = 0; i < n_A_width; i++) {
        PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_madd(c, matA, b[i], n_A_vec_byte);
        matA += n_A_vec_byte;
    }
}

void PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_mul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned int len_vec) {
    unsigned int n_vec_byte = len_vec;
    for (unsigned int k = 0; k < len_vec; k++) {
        PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_set_zero(c, n_vec_byte);
        const uint8_t *bk = b + n_vec_byte * k;
        for (unsigned int i = 0; i < len_vec; i++) {
            PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_madd(c, a + n_vec_byte * i, bk[i], n_vec_byte);
        }
        c += n_vec_byte;
    }
}

static unsigned int gf256mat_gauss_elim_ref(uint8_t *mat, unsigned int h, unsigned int w) {
    unsigned int r8 = 1;

    for (unsigned int i = 0; i < h; i++) {
        uint8_t *ai = mat + w * i;
        unsigned int skip_len_align4 = i & ((unsigned int)~0x3);

        for (unsigned int j = i + 1; j < h; j++) {
            uint8_t *aj = mat + w * j;
            PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_predicated_add(ai + skip_len_align4, !PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256_is_nonzero(ai[i]), aj + skip_len_align4, w - skip_len_align4);
        }
        r8 &= PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256_is_nonzero(ai[i]);
        uint8_t pivot = ai[i];
        pivot = PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256_inv(pivot);
        PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_mul_scalar(ai + skip_len_align4, pivot, w - skip_len_align4);
        for (unsigned int j = 0; j < h; j++) {
            if (i == j) {
                continue;
            }
            uint8_t *aj = mat + w * j;
            PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_madd(aj + skip_len_align4, ai + skip_len_align4, aj[i], w - skip_len_align4);
        }
    }

    return r8;
}

static unsigned int gf256mat_solve_linear_eq_ref(uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned int n) {
    uint8_t mat[64 * 64];
    for (unsigned int i = 0; i < n; i++) {
        memcpy(mat + i * (n + 1), inp_mat + i * n, n);
        mat[i * (n + 1) + n] = c_terms[i];
    }
    unsigned int r8 = PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_gauss_elim(mat, n, n + 1);
    for (unsigned int i = 0; i < n; i++) {
        sol[i] = mat[i * (n + 1) + n];
    }
    return r8;
}

static inline void gf256mat_submat(uint8_t *mat2, unsigned int w2, unsigned int st, const uint8_t *mat, unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; i++) {
        for (unsigned int j = 0; j < w2; j++) {
            mat2[i * w2 + j] = mat[i * w + st + j];
        }
    }
}

unsigned int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_inv(uint8_t *inv_a, const uint8_t *a, unsigned int H, uint8_t *buffer) {
    uint8_t *aa = buffer;
    for (unsigned int i = 0; i < H; i++) {
        uint8_t *ai = aa + i * 2 * H;
        PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_set_zero(ai, 2 * H);
        PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256v_add(ai, a + i * H, H);
        ai[H + i] = 1;
    }
    unsigned int r8 = PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_gauss_elim(aa, H, 2 * H);
    gf256mat_submat(inv_a, H, H, aa, 2 * H, H);
    return r8;
}


// choosing the implementations depends on the macros _BLAS_AVX2_ and _BLAS_SSE

#define gf256mat_prod_impl gf256mat_prod_ref
#define gf256mat_gauss_elim_impl gf256mat_gauss_elim_ref
#define gf256mat_solve_linear_eq_impl gf256mat_solve_linear_eq_ref
void PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_prod(uint8_t *c, const uint8_t *matA, unsigned int n_A_vec_byte, unsigned int n_A_width, const uint8_t *b) {
    gf256mat_prod_impl(c, matA, n_A_vec_byte, n_A_width, b);
}

unsigned int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_gauss_elim(uint8_t *mat, unsigned int h, unsigned int w) {
    return gf256mat_gauss_elim_impl(mat, h, w);
}

unsigned int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256mat_solve_linear_eq(uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned int n) {
    return gf256mat_solve_linear_eq_impl(sol, inp_mat, c_terms, n);
}

