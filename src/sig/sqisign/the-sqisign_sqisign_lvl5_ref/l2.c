#include <quaternion.h>
#include "lll_internals.h"
#include "internal.h"

#include "dpe.h"

// Access entry of symmetric matrix
#define SYM(M, i, j) (i < j ? &M[j][i] : &M[i][j])

void
quat_lll_core(ibz_mat_4x4_t *G, ibz_mat_4x4_t *basis)
{
    dpe_t dpe_const_one, dpe_const_DELTABAR;

    dpe_init(dpe_const_one);
    dpe_set_ui(dpe_const_one, 1);

    dpe_init(dpe_const_DELTABAR);
    dpe_set_d(dpe_const_DELTABAR, DELTABAR);

    // fp variables for Gram-Schmidt orthogonalization and Lovasz' conditions
    dpe_t r[4][4], u[4][4], lovasz[4];
    for (int i = 0; i < 4; i++) {
        dpe_init(lovasz[i]);
        for (int j = 0; j <= i; j++) {
            dpe_init(r[i][j]);
            dpe_init(u[i][j]);
        }
    }

    // threshold for swaps
    dpe_t delta_bar;
    dpe_init(delta_bar);
    dpe_set_d(delta_bar, DELTABAR);

    // Other work variables
    dpe_t Xf, tmpF;
    dpe_init(Xf);
    dpe_init(tmpF);
    ibz_t X, tmpI;
    ibz_init(&X);
    ibz_init(&tmpI);

    // Main L² loop
    dpe_set_z(r[0][0], (*G)[0][0]);
    int kappa = 1;
    while (kappa < 4) {
        // size reduce b_κ
        int done = 0;
        while (!done) {
            // Recompute the κ-th row of the Choleski Factorisation
            // Loop invariant:
            //     r[κ][j] ≈ u[κ][j] ‖b_j*‖² ≈ 〈b_κ, b_j*〉
            for (int j = 0; j <= kappa; j++) {
                dpe_set_z(r[kappa][j], (*G)[kappa][j]);
                for (int k = 0; k < j; k++) {
                    dpe_mul(tmpF, r[kappa][k], u[j][k]);
                    dpe_sub(r[kappa][j], r[kappa][j], tmpF);
                }
                if (j < kappa)
                    dpe_div(u[kappa][j], r[kappa][j], r[j][j]);
            }

            done = 1;
            // size reduce
            for (int i = kappa - 1; i >= 0; i--) {
                if (dpe_cmp_d(u[kappa][i], ETABAR) > 0 || dpe_cmp_d(u[kappa][i], -ETABAR) < 0) {
                    done = 0;
                    dpe_set(Xf, u[kappa][i]);
                    dpe_round(Xf, Xf);
                    dpe_get_z(X, Xf);
                    // Update basis: b_κ ← b_κ - X·b_i
                    for (int j = 0; j < 4; j++) {
                        ibz_mul(&tmpI, &X, &(*basis)[j][i]);
                        ibz_sub(&(*basis)[j][kappa], &(*basis)[j][kappa], &tmpI);
                    }
                    // Update lower half of the Gram matrix
                    // <b_κ - X·b_i, b_κ - X·b_i> = <b_κ, b_κ> - 2X<b_κ, b_i> + X²<b_i, b_i> =
                    // <b_κ,b_κ> - X<b_κ,b_i> - X(<b_κ,b_i> - X·<b_i, b_i>)
                    //// 〈b_κ, b_κ〉 ← 〈b_κ, b_κ〉 - X·〈b_κ, b_i〉
                    ibz_mul(&tmpI, &X, &(*G)[kappa][i]);
                    ibz_sub(&(*G)[kappa][kappa], &(*G)[kappa][kappa], &tmpI);
                    for (int j = 0; j < 4; j++) { // works because i < κ
                        // 〈b_κ, b_j〉 ← 〈b_κ, b_j〉 - X·〈b_i, b_j〉
                        ibz_mul(&tmpI, &X, SYM((*G), i, j));
                        ibz_sub(SYM((*G), kappa, j), SYM((*G), kappa, j), &tmpI);
                    }
                    // After the loop:
                    //// 〈b_κ,b_κ〉 ← 〈b_κ,b_κ〉 - X·〈b_κ,b_i〉 - X·(〈b_κ,b_i〉 - X·〈b_i,
                    /// b_i〉) = 〈b_κ - X·b_i, b_κ - X·b_i〉
                    //
                    // Update u[kappa][j]
                    for (int j = 0; j < i; j++) {
                        dpe_mul(tmpF, Xf, u[i][j]);
                        dpe_sub(u[kappa][j], u[kappa][j], tmpF);
                    }
                }
            }
        }

        // Check Lovasz' conditions
        // lovasz[0] = ‖b_κ‖²
        dpe_set_z(lovasz[0], (*G)[kappa][kappa]);
        // lovasz[i] = lovasz[i-1] - u[κ][i-1]·r[κ][i-1]
        for (int i = 1; i < kappa; i++) {
            dpe_mul(tmpF, u[kappa][i - 1], r[kappa][i - 1]);
            dpe_sub(lovasz[i], lovasz[i - 1], tmpF);
        }
        int swap;
        for (swap = kappa; swap > 0; swap--) {
            dpe_mul(tmpF, delta_bar, r[swap - 1][swap - 1]);
            if (dpe_cmp(tmpF, lovasz[swap - 1]) < 0)
                break;
        }

        // Insert b_κ before b_swap
        if (kappa != swap) {
            // Insert b_κ before b_swap in the basis and in the lower half Gram matrix
            for (int j = kappa; j > swap; j--) {
                for (int i = 0; i < 4; i++) {
                    ibz_swap(&(*basis)[i][j], &(*basis)[i][j - 1]);
                    if (i == j - 1)
                        ibz_swap(&(*G)[i][i], &(*G)[j][j]);
                    else if (i != j)
                        ibz_swap(SYM((*G), i, j), SYM((*G), i, j - 1));
                }
            }
            // Copy row u[κ] and r[κ] in swap position, ignore what follows
            for (int i = 0; i < swap; i++) {
                dpe_set(u[swap][i], u[kappa][i]);
                dpe_set(r[swap][i], r[kappa][i]);
            }
            dpe_set(r[swap][swap], lovasz[swap]);
            // swap complete
            kappa = swap;
        }

        kappa += 1;
    }

#ifndef NDEBUG
    // Check size-reducedness
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < i; j++) {
            dpe_abs(u[i][j], u[i][j]);
            assert(dpe_cmp_d(u[i][j], ETABAR) <= 0);
        }
    // Check Lovasz' conditions
    for (int i = 1; i < 4; i++) {
        dpe_mul(tmpF, u[i][i - 1], u[i][i - 1]);
        dpe_sub(tmpF, dpe_const_DELTABAR, tmpF);
        dpe_mul(tmpF, tmpF, r[i - 1][i - 1]);
        assert(dpe_cmp(tmpF, r[i][i]) <= 0);
    }
#endif

    // Fill in the upper half of the Gram matrix
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++)
            ibz_copy(&(*G)[i][j], &(*G)[j][i]);
    }

    // Clearinghouse
    ibz_finalize(&X);
    ibz_finalize(&tmpI);
    dpe_clear(dpe_const_one);
    dpe_clear(dpe_const_DELTABAR);
    dpe_clear(Xf);
    dpe_clear(tmpF);
    dpe_clear(delta_bar);
    for (int i = 0; i < 4; i++) {
        dpe_clear(lovasz[i]);
        for (int j = 0; j <= i; j++) {
            dpe_clear(r[i][j]);
            dpe_clear(u[i][j]);
        }
    }
}

int
quat_lattice_lll(ibz_mat_4x4_t *red, const quat_lattice_t *lattice, const quat_alg_t *alg)
{
    ibz_mat_4x4_t G; // Gram Matrix
    ibz_mat_4x4_init(&G);
    quat_lattice_gram(&G, lattice, alg);
    ibz_mat_4x4_copy(red, &lattice->basis);
    quat_lll_core(&G, red);
    ibz_mat_4x4_finalize(&G);
    return 0;
}
