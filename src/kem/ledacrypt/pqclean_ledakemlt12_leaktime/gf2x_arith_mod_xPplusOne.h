#ifndef GF2X_ARITH_MOD_XPLUSONE_H
#define GF2X_ARITH_MOD_XPLUSONE_H

#include "qc_ldpc_parameters.h"

#include "gf2x_arith.h"
#include "rng.h"

#define NUM_BITS_GF2X_ELEMENT                   (P) // 52147
#define NUM_DIGITS_GF2X_ELEMENT                 ((P+DIGIT_SIZE_b-1)/DIGIT_SIZE_b)
#define MSb_POSITION_IN_MSB_DIGIT_OF_ELEMENT    ((P % DIGIT_SIZE_b) ? (P % DIGIT_SIZE_b)-1 : DIGIT_SIZE_b-1)
#define NUM_BITS_GF2X_MODULUS                   (P+1)
#define NUM_DIGITS_GF2X_MODULUS                 ((P+1+DIGIT_SIZE_b-1)/DIGIT_SIZE_b)
#define MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS    (P-DIGIT_SIZE_b*(NUM_DIGITS_GF2X_MODULUS-1))
#define INVALID_POS_VALUE                       (P)
#define P_BITS                                  (16) // log_2(p) = 15.6703

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_copy(DIGIT dest[], const DIGIT in[]);
DIGIT PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_get_coeff(const DIGIT poly[], size_t exponent);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_set_coeff(DIGIT poly[], size_t exponent, DIGIT value);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_toggle_coeff(DIGIT poly[], size_t exponent);
int PQCLEAN_LEDAKEMLT12_LEAKTIME_population_count(const DIGIT *poly);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_add(DIGIT Res[], const DIGIT A[], const DIGIT B[]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_transpose_in_place(DIGIT A[]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_circulant_sparse_block(POSITION_T *pos_ones, size_t countOnes, AES_XOF_struct *seed_expander_ctx);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_circulant_blocks_sequence(DIGIT *sequence, AES_XOF_struct *seed_expander_ctx);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_error_pos(POSITION_T errorPos[NUM_ERRORS_T], AES_XOF_struct *seed_expander_ctx);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_expand_error(DIGIT sequence[N0 * NUM_DIGITS_GF2X_ELEMENT], const POSITION_T errorPos[NUM_ERRORS_T]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_add_sparse(size_t sizeR, POSITION_T Res[], size_t sizeA, const POSITION_T A[], size_t sizeB, const POSITION_T B[]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_transpose_in_place_sparse(size_t sizeA, POSITION_T A[]);
int PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_inverse(DIGIT out[], const DIGIT in[]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_mul_sparse(size_t sizeR, POSITION_T Res[], size_t sizeA,  const POSITION_T A[], size_t sizeB, const POSITION_T B[]);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_mul_dense_to_sparse(DIGIT Res[], const DIGIT dense[], POSITION_T sparse[], size_t nPos);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_tobytes(uint8_t *bytes, const DIGIT *poly);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_frombytes(DIGIT *poly, const uint8_t *poly_bytes);


#endif
