#ifndef LINSYS_H
#define LINSYS_H

#include <stdint.h>
#include "qruov.h"

typedef struct {
    uint8_t *col;
    uint8_t original_row_id;
} linsys_row;

typedef struct {
    linsys_row row[QRUOV_m];
    linsys_row *eqn[QRUOV_m];
    uint8_t consistency_cache[QRUOV_m * QRUOV_m];
    uint8_t index[QRUOV_m];
    int rank;
    int has_consistency_cache;
} linsys_echelon;

void linsys_lu_decompose(uint8_t *mat, linsys_echelon *echelon);
int linsys_check_consistency(linsys_echelon *echelon, const uint8_t *b);
void linsys_prepare_consistency(linsys_echelon *echelon, const uint8_t *u, uint8_t *rhs);
int linsys_check_prepared_consistency(linsys_echelon *echelon, const uint8_t *rhs, const uint8_t *t);
void linsys_sample_solution(const linsys_echelon *echelon, const uint8_t *b, const uint8_t *free_x, uint8_t *x);

#endif // LINSYS_H
