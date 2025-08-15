#include <quaternion.h>
#include "lll_internals.h"
#include "internal.h"

#include <math.h>
#include <float.h>


// Access entry of symmetric matrix
#define SYM(M, i, j) (i < j ? &M[j][i] : &M[i][j])

typedef struct fp_num {
    double s;
    int e;
} fp_num;

static void
copy(fp_num *x, fp_num *r)
{
    r->s = x->s;
    r->e = x->e;
}

static void
normalize(fp_num *x)
{
  if (x->s == 0.0 || isfinite(x->s) == 0) {
      if (x->s == 0.0) {
        x->e = INT_MIN;
      }
  } else {
      int e;
      x->s = frexp(x->s, &e);
      x->e += e;
  }
}

static void
to_one(fp_num *x)
{
    x->s = 1;
    x->e = 0;
}

static void
to_deltabar(fp_num *x)
{
    x->s = DELTABAR;
    x->e = 0;
}

static void
to_etabar(fp_num *x)
{
    x->s = ETABAR;
    x->e = 0;
}

static void
from_mpz(const mpz_t x, fp_num *r)
{
    long exp = 0;
    r->s = mpz_get_d_2exp(&exp, x);
    r->e = exp;
}

static void
to_mpz(const fp_num *x, mpz_t r)
{
    if (x->e >= DBL_MANT_DIG) {
        double s = x->s * 0x1P53;
        mpz_set_d(r, s);
        mpz_mul_2exp(r, r, x->e - DBL_MANT_DIG);
    } else if (x->e < 0) {
            mpz_set_ui(r, 0);
    } else {
            double s = ldexp(x->s, x->e);
            mpz_set_d(r, round(s));
    }
}

static void
fp_mul(const fp_num *x, const fp_num *y, fp_num *r)
{
    r->s = x->s * y->s;
    r->e = x->e + y->e;
    normalize(r);

}

static void
fp_div(const fp_num *x, const fp_num *y, fp_num *r)
{
    r->s = x->s / y->s;
    r->e = x->e - y->e;
    normalize(r);
}

static void
fp_sub(const fp_num *x, const fp_num *y, fp_num *r)
{
    if (x->e > y->e + DBL_MANT_DIG) {
        r->s = x->s;
        r->e = x->e;
    } else if (y->e > x->e + DBL_MANT_DIG) {
        r->s = -y->s;
        r->e = y->e;
    } else {
      int e = x->e - y->e;

      if (e >= 0) {
          r->s = x->s - ldexp(y->s, -e);
          r->e = x->e;
      } else {
          r->s = ldexp(x->s, e) - y->s;
          r->e = y->e;
      }

      normalize(r);
    }
}

static inline int
sign(const fp_num *x)
{
    if (x->s < 0.0)
        return -1;
    return 1;
}

static int
fp_cmp(const fp_num *x, const fp_num *y)
{
    int sign_x = sign(x);
    int sign_y = sign(y);

    if (sign_x != sign_y)
        return sign_x - sign_y;
    else if (x->e > y->e)
        return sign_x;
    else if (y->e > x->e)
        return -sign_x;
    else if (x->s > y->s)
        return 1;
    else if (x->s < y->s)
        return -1;
    else
        return 0;
}

static void
fp_round(fp_num *x)
{
    if (x->e < 0) {
        x->s = 0;
        x->e = 0;
    } else if (x->e >= DBL_MANT_DIG) {
        return;
    } else {
        double tmp;
        tmp = ldexp(x->s, x->e);
        x->s = round(tmp);
        x->e = 0;
        normalize(x);
    }
}

static void
fp_abs(const fp_num *x, fp_num *y) {
    if (x->s < 0.0) {
        y->s = -x->s;
    } else {
        y->s = x->s;
    }
    y->e = x->e;
}

void
quat_lll_core(ibz_mat_4x4_t *G, ibz_mat_4x4_t *basis)
{
    fp_num const_one = {0};
    fp_num delta_bar = {0};
    fp_num eta_bar = {0};
    fp_num neg_eta_bar = {0};
    to_one(&const_one);
    to_deltabar(&delta_bar);
    eta_bar.s = ETABAR;
    eta_bar.e = 0;
    neg_eta_bar.s = -ETABAR;
    neg_eta_bar.e = 0;
    normalize(&eta_bar);
    normalize(&neg_eta_bar);

    fp_num r[4][4] = {0};
    fp_num u[4][4] = {0};
    fp_num lovasz[4] = {0};

    fp_num Xf = {0};
    fp_num tmpF = {0};

    ibz_t X, tmpI;
    ibz_init(&X);
    ibz_init(&tmpI);

    // Main L² loop
    from_mpz((*G)[0][0], &r[0][0]);
    int kappa = 1;
    while (kappa < 4) {
        // size reduce b_κ
        int done = 0;
        while (!done) {
            // Recompute the κ-th row of the Choleski Factorisation
            // Loop invariant:
            //     r[κ][j] ≈ u[κ][j] ‖b_j*‖² ≈ 〈b_κ, b_j*〉
            for (int j = 0; j <= kappa; j++) {
                from_mpz((*G)[kappa][j], &r[kappa][j]);
                for (int k = 0; k < j; k++) {
                    fp_mul(&r[kappa][k], &u[j][k], &tmpF);
                    fp_sub(&r[kappa][j], &tmpF, &r[kappa][j]);
                }
                if (j < kappa)
                    fp_div(&r[kappa][j], &r[j][j], &u[kappa][j]);
            }

            done = 1;
            // size reduce
            for (int i = kappa - 1; i >= 0; i--) {
                if (fp_cmp(&u[kappa][i], &eta_bar) > 0 || fp_cmp(&u[kappa][i], &neg_eta_bar) < 0) {
                    done = 0;
                    copy(&u[kappa][i], &Xf);
                    fp_round(&Xf);
                    to_mpz(&Xf, X);
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
                        fp_mul(&Xf, &u[i][j], &tmpF);
                        fp_sub(&u[kappa][j], &tmpF, &u[kappa][j]);
                    }
                }
            }
        }

        // Check Lovasz' conditions
        // lovasz[0] = ‖b_κ‖²
        from_mpz((*G)[kappa][kappa], &lovasz[0]);
        // lovasz[i] = lovasz[i-1] - u[κ][i-1]·r[κ][i-1]
        for (int i = 1; i < kappa; i++) {
            fp_mul(&u[kappa][i - 1], &r[kappa][i - 1], &tmpF);
            fp_sub(&lovasz[i - 1], &tmpF, &lovasz[i]);
        }
        int swap;
        for (swap = kappa; swap > 0; swap--) {
            fp_mul(&delta_bar, &r[swap - 1][swap - 1], &tmpF);
            if (fp_cmp(&tmpF, &lovasz[swap - 1]) < 0)
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
                copy(&u[kappa][i], &u[swap][i]);
                copy(&r[kappa][i], &r[swap][i]);
            }
            copy(&lovasz[swap], &r[swap][swap]);
            // swap complete
            kappa = swap;
        }

        kappa += 1;
    }

#ifndef NDEBUG
    // Check size-reducedness
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < i; j++) {
            fp_abs(&u[i][j], &u[i][j]);
            assert(fp_cmp(&u[i][j], &eta_bar) <= 0);
        }
    // Check Lovasz' conditions
    for (int i = 1; i < 4; i++) {
        fp_mul(&u[i][i - 1], &u[i][i - 1], &tmpF);
        fp_sub(&delta_bar, &tmpF, &tmpF);
        fp_mul(&tmpF, &r[i - 1][i - 1], &tmpF);
        assert(fp_cmp(&tmpF, &r[i][i]) <= 0);
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
