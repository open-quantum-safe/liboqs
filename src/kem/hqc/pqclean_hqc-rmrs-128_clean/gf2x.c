#include "gf2x.h"
#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include <stdint.h>
/**
 * \file gf2x.c
 * \brief Implementation of multiplication of two polynomials
 */


static inline void swap(uint16_t *tab, uint16_t elt1, uint16_t elt2);
static void reduce(uint64_t *o, const uint64_t *a);
static void fast_convolution_mult(uint8_t *o, const uint32_t *a1, const uint64_t *a2, uint16_t weight, AES_XOF_struct *ctx);

/**
 * @brief swap two elements in a table
 *
 * This function exchanges tab[elt1] with tab[elt2]
 *
 * @param[in] tab Pointer to the table
 * @param[in] elt1 Index of the first element
 * @param[in] elt2 Index of the second element
 */
static inline void swap(uint16_t *tab, uint16_t elt1, uint16_t elt2) {
    uint16_t tmp = tab[elt1];

    tab[elt1] = tab[elt2];
    tab[elt2] = tmp;
}



/**
 * @brief Compute o(x) = a(x) mod \f$ X^n - 1\f$
 *
 * This function computes the modular reduction of the polynomial a(x)
 *
 * @param[in] a Pointer to the polynomial a(x)
 * @param[out] o Pointer to the result
 */
static void reduce(uint64_t *o, const uint64_t *a) {
    size_t i;
    uint64_t r;
    uint64_t carry;

    for (i = 0; i < VEC_N_SIZE_64; i++) {
        r = a[i + VEC_N_SIZE_64 - 1] >> (PARAM_N & 63);
        carry = (uint64_t) (a[i + VEC_N_SIZE_64] << (64 - (PARAM_N & 63)));
        o[i] = a[i] ^ r ^ carry;
    }

    o[VEC_N_SIZE_64 - 1] &= RED_MASK;
}



/**
 * @brief computes product of the polynomial a1(x) with the sparse polynomial a2
 *
 *  o(x) = a1(x)a2(x)
 *
 * @param[out] o Pointer to the result
 * @param[in] a1 Pointer to the sparse polynomial a2 (list of degrees of the monomials which appear in a2)
 * @param[in] a2 Pointer to the polynomial a1(x)
 * @param[in] weight Hamming wifht of the sparse polynomial a2
 * @param[in] ctx Pointer to a seed expander used to randomize the multiplication process
 */
static void fast_convolution_mult(uint8_t *o, const uint32_t *a1, const uint64_t *a2, uint16_t weight, AES_XOF_struct *ctx) {
//static uint32_t fast_convolution_mult(const uint64_t *A, const uint32_t *vB, uint64_t *C, const uint16_t w, AES_XOF_struct *ctx)
    uint64_t carry;
    uint32_t dec, s;
    uint64_t table[16 * (VEC_N_SIZE_64 + 1)];
    uint16_t permuted_table[16];
    uint16_t permutation_table[16];
    uint16_t permuted_sparse_vect[PARAM_OMEGA_E];
    uint16_t permutation_sparse_vect[PARAM_OMEGA_E];
    uint64_t tmp;
    uint64_t *pt;
    uint8_t *res;
    size_t i, j;

    for (i = 0; i < 16; i++) {
        permuted_table[i] = (uint16_t) i;
    }

    seedexpander(ctx, (uint8_t *) permutation_table, 16 * sizeof(uint16_t));

    for (i = 0; i < 15; i++) {
        swap(permuted_table + i, 0, permutation_table[i] % (16 - i));
    }

    pt = table + (permuted_table[0] * (VEC_N_SIZE_64 + 1));
    for (j = 0; j < VEC_N_SIZE_64; j++) {
        pt[j] = a2[j];
    }
    pt[VEC_N_SIZE_64] = 0x0;

    for (i = 1; i < 16; i++) {
        carry = 0;
        pt = table + (permuted_table[i] * (VEC_N_SIZE_64 + 1));
        for (j = 0; j < VEC_N_SIZE_64; j++) {
            pt[j] = (a2[j] << i) ^ carry;
            carry = (a2[j] >> ((64 - i)));
        }
        pt[VEC_N_SIZE_64] = carry;
    }

    for (i = 0; i < weight; i++) {
        permuted_sparse_vect[i] = (uint16_t) i;
    }

    seedexpander(ctx, (uint8_t *) permutation_sparse_vect, weight * sizeof(uint16_t));

    for (i = 0; i + 1 < weight; i++) {
        swap(permuted_sparse_vect + i, 0, (uint16_t) (permutation_sparse_vect[i] % (weight - i)));
    }

    for (i = 0; i < weight; i++) {
        dec = a1[permuted_sparse_vect[i]] & 0xf;
        s = a1[permuted_sparse_vect[i]] >> 4;
        res = o + 2 * s;
        pt = table + (permuted_table[dec] * (VEC_N_SIZE_64 + 1));

        for (j = 0; j < VEC_N_SIZE_64 + 1; j++) {
            tmp = PQCLEAN_HQCRMRS128_CLEAN_load8(res);
            PQCLEAN_HQCRMRS128_CLEAN_store8(res, tmp ^ pt[j]);
            res += 8;
        }
    }
}



/**
 * @brief Multiply two polynomials modulo \f$ X^n - 1\f$.
 *
 * This functions multiplies a sparse polynomial <b>a1</b> (of Hamming weight equal to <b>weight</b>)
 * and a dense polynomial <b>a2</b>. The multiplication is done modulo \f$ X^n - 1\f$.
 *
 * @param[out] o Pointer to the result
 * @param[in] a1 Pointer to the sparse polynomial
 * @param[in] a2 Pointer to the dense polynomial
 * @param[in] weight Integer that is the weigt of the sparse polynomial
 * @param[in] ctx Pointer to the randomness context
 */
void PQCLEAN_HQCRMRS128_CLEAN_vect_mul(uint64_t *o, const uint32_t *a1, const uint64_t *a2, uint16_t weight, AES_XOF_struct *ctx) {
    uint64_t tmp[2 * VEC_N_SIZE_64 + 1] = {0};

    fast_convolution_mult((uint8_t *) tmp, a1, a2, weight, ctx);
    PQCLEAN_HQCRMRS128_CLEAN_load8_arr(tmp, 2 * VEC_N_SIZE_64 + 1, (uint8_t *) tmp, sizeof(tmp));
    reduce(o, tmp);
}
