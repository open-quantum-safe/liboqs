#include <string.h>
#include "qruov.h"
#include "util.h"
#include "linsys.h"
#include "gf.h"
#include "fastop.h"

static void echelon_init(uint8_t *mat, linsys_echelon *echelon)
{
    for (int i = 0; i < QRUOV_m; i++) {
        echelon->row[i].col = &mat[i * QRUOV_m];
        echelon->row[i].original_row_id = (uint8_t)i;
        echelon->eqn[i] = &echelon->row[i];
        echelon->index[i] = 0xff;
    }
    echelon->rank = 0;
    echelon->has_consistency_cache = 0;
}

static void row_swap(linsys_row *eqn[QRUOV_m], int i, int j)
{
    linsys_row *tmp = eqn[i];
    eqn[i] = eqn[j];
    eqn[j] = tmp;
}

#define EQN(E, I, J) ((E)->eqn[(I)]->col[(J)])

void linsys_lu_decompose(uint8_t *mat, linsys_echelon *echelon)
{
    echelon_init(mat, echelon);
    int c = -1;
    for (int i = 0; i < QRUOV_m; i++) {
        c++;
        if (c >= QRUOV_m) return;

        int j = i;
        while (EQN(echelon, j, c) == 0) {
            j++;
            if (j >= QRUOV_m) {
                c++;
                if (c >= QRUOV_m) return;
                j = i;
            }
        }

        row_swap(echelon->eqn, i, j);
        echelon->index[echelon->rank++] = (uint8_t)c;

        uint8_t pivot = EQN(echelon, i, c);
        uint8_t inv = gf_inv(pivot);
        EQN(echelon, i, i) = pivot;
        for (int k = c + 1; k < QRUOV_m; k++) {
            EQN(echelon, i, k) = gf_mul(inv, EQN(echelon, i, k));
        }

        for (int r = i + 1; r < QRUOV_m; r++) {
            uint8_t mul = EQN(echelon, r, c);
            EQN(echelon, r, i) = mul;
            uint8_t m_mul = (uint8_t)(mul ^ QRUOV_q);
            fastop_row_elim(&EQN(echelon, r, c + 1),
                            &EQN(echelon, i, c + 1),
                            m_mul, QRUOV_m - (c + 1));
        }
    }
}

static void linsys_build_consistency_cache(linsys_echelon *echelon)
{
    uint8_t *cache = echelon->consistency_cache;
    uint8_t pivot_inv[QRUOV_m];
    uint8_t qv[QRUOV_m];

    for (int k = 0; k < echelon->rank; k++) {
        pivot_inv[k] = gf_inv(EQN(echelon, k, k));
    }

    for (int h = 0; h < QRUOV_m - echelon->rank; h++) {
        int s = echelon->rank + h;
        uint8_t *row = &cache[h * QRUOV_m];
        memset(qv, 0, sizeof(qv));

        /*
         * The decomposition stores P A = L U.  For a dependent row s, this
         * builds the lower row of L^{-1} P directly in original-row order, so
         * consistency of A x = b is exactly row . b == 0.
         */
        for (int k = echelon->rank - 1; k >= 0; k--) {
            uint32_t acc = EQN(echelon, s, k);
            for (int p = k + 1; p < echelon->rank; p++) {
                acc += (uint32_t)qv[p] * EQN(echelon, p, k);
            }
            uint8_t tmp = gf_reduce(acc);
            qv[k] = gf_sub(0, gf_mul(tmp, pivot_inv[k]));
        }

        memset(row, 0, QRUOV_m);
        for (int k = 0; k < echelon->rank; k++) {
            row[echelon->eqn[k]->original_row_id] = qv[k];
        }
        row[echelon->eqn[s]->original_row_id] = 1;
    }

    secure_zero(qv, sizeof(qv));
    secure_zero(pivot_inv, sizeof(pivot_inv));
    echelon->has_consistency_cache = 1;
}

int linsys_check_consistency(linsys_echelon *echelon, const uint8_t *b)
{
    if (echelon->rank == QRUOV_m) return 1;
    if (!echelon->has_consistency_cache) linsys_build_consistency_cache(echelon);

    for (int h = 0; h < QRUOV_m - echelon->rank; h++) {
        const uint8_t *row = &echelon->consistency_cache[h * QRUOV_m];
        if (fastop_dotprod(QRUOV_m, row, b) != 0) return 0;
    }
    return 1;
}

void linsys_prepare_consistency(linsys_echelon *echelon, const uint8_t *u, uint8_t *rhs)
{
    memset(rhs, 0, QRUOV_m);
    if (echelon->rank == QRUOV_m) return;
    if (!echelon->has_consistency_cache) linsys_build_consistency_cache(echelon);

    for (int h = 0; h < QRUOV_m - echelon->rank; h++) {
        const uint8_t *row = &echelon->consistency_cache[h * QRUOV_m];
        rhs[h] = fastop_dotprod(QRUOV_m, row, u);
    }
}

int linsys_check_prepared_consistency(linsys_echelon *echelon, const uint8_t *rhs, const uint8_t *t)
{
    if (echelon->rank == QRUOV_m) return 1;
    if (!echelon->has_consistency_cache) linsys_build_consistency_cache(echelon);

    for (int h = 0; h < QRUOV_m - echelon->rank; h++) {
        const uint8_t *row = &echelon->consistency_cache[h * QRUOV_m];
        if (fastop_dotprod(QRUOV_m, row, t) != rhs[h]) return 0;
    }
    return 1;
}

void linsys_sample_solution(const linsys_echelon *echelon, const uint8_t *b, const uint8_t *free_x, uint8_t *x)
{
    uint8_t b2[QRUOV_m];
    memset(b2, 0, sizeof(b2));

    for (int i = 0; i < echelon->rank; i++) {
        uint8_t rhs = b[echelon->eqn[i]->original_row_id];
        uint8_t tmp = gf_sub(rhs, fastop_dotprod((unsigned int)i, &EQN(echelon, i, 0), b2));
        b2[i] = gf_mul(tmp, gf_inv(EQN(echelon, i, i)));
    }

    int pivot_idx = echelon->rank - 1;
    int free_pos = 0;
    for (int col = QRUOV_m - 1; col >= 0; col--) {
        if (pivot_idx >= 0 && col == echelon->index[pivot_idx]) {
            x[col] = gf_sub(b2[pivot_idx],
                            fastop_dotprod((unsigned int)(QRUOV_m - (col + 1)),
                                           &EQN(echelon, pivot_idx, col + 1),
                                           &x[col + 1]));
            pivot_idx--;
        } else {
            x[col] = free_x[free_pos++];
        }
    }

    secure_zero(b2, sizeof(b2));
}
