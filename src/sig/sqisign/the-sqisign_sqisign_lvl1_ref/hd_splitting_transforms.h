#ifndef HD_SPLITTING_H
#define HD_SPLITTING_H

#include <hd.h>
#include <stdint.h>

typedef struct precomp_basis_change_matrix {
    uint8_t m[4][4];
} precomp_basis_change_matrix_t;

extern const int EVEN_INDEX[10][2];
extern const int CHI_EVAL[4][4];
extern const fp2_t FP2_CONSTANTS[5];
extern const precomp_basis_change_matrix_t SPLITTING_TRANSFORMS[10];
extern const precomp_basis_change_matrix_t NORMALIZATION_TRANSFORMS[6];

#endif

