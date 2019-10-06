#include "mq.h"
#include "params.h"

/* Computes all products x_i * x_j, returns in reduced form */
inline static
void generate_quadratic_terms( gf31 *xij, const gf31 *x ) {
    int i, j, k;
    k = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++) {
            xij[k] = PQCLEAN_MQDSS48_CLEAN_mod31((gf31)(x[i] * x[j]));
            k++;
        }
    }
}

/* Computes all terms (x_i * y_j) + (x_j * y_i), returns in reduced form */
inline static
void generate_xiyj_p_xjyi_terms( gf31 *xij, const gf31 *x, const gf31 *y ) {
    int i, j, k;
    k = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++) {
            xij[k] = PQCLEAN_MQDSS48_CLEAN_mod31((gf31)(x[i] * y[j] + x[j] * y[i]));
            k++;
        }
    }
}

/* Evaluates the MQ function on a vector of N gf31 elements x (expected to be
   in reduced 5-bit representation). Expects the coefficients in F to be in
   signed representation (i.e. [-15, 15], packed bytewise).
   Outputs M gf31 elements in unique 16-bit representation as fx. */
void PQCLEAN_MQDSS48_CLEAN_MQ(gf31 *fx, const gf31 *x, const signed char *F) {
    int i, j;
    gf31 _xij[N * (N + 1) >> 1];
    int r[M] = {0};

    generate_quadratic_terms(_xij, x);

    for (i = 0; i < N; i += 2) {
        for (j = 0; j < M; j++) {
            r[j] += ((int)x[i]) * ((int)F[i * M + 2 * j]) +
                    ((int)x[i + 1]) * ((int)F[i * M + 2 * j + 1]);
        }
    }

    for (i = 0; i < (N * (N + 1)) >> 1; i += 2) {
        for (j = 0; j < M; j++) {
            r[j] += ((int)_xij[i]) * ((int)F[N * M + i * M + 2 * j]) +
                    ((int)_xij[i + 1]) * ((int)F[N * M + i * M + 2 * j + 1]);
        }
    }

    for (i = 0; i < M; i++) {
        fx[i] = PQCLEAN_MQDSS48_CLEAN_mod31((gf31)((r[i] >> 15) + (r[i] & 0x7FFF)));
    }
}

/* Evaluates the bilinear polar form of the MQ function (i.e. G) on a vector of
   N gf31 elements x (expected to be in reduced 5-bit representation). Expects
   the coefficients in F to be in signed representation (i.e. [-15, 15], packed
   bytewise). Outputs M gf31 elements in unique 16-bit representation as fx. */
void PQCLEAN_MQDSS48_CLEAN_G(gf31 *fx, const gf31 *x, const gf31 *y, const signed char *F) {
    int i, j;
    gf31 _xij[N * (N + 1) >> 1];
    int r[M] = {0};

    generate_xiyj_p_xjyi_terms(_xij, x, y);

    for (i = 0; i < (N * (N + 1)) >> 1; i += 2) {
        for (j = 0; j < M; j++) {
            r[j] += ((int)_xij[i]) * ((int)F[N * M + i * M + 2 * j]) +
                    ((int)_xij[i + 1]) * ((int)F[N * M + i * M + 2 * j + 1]);
        }
    }

    for (i = 0; i < M; i++) {
        fx[i] = PQCLEAN_MQDSS48_CLEAN_mod31((gf31)((r[i] >> 15) + (r[i] & 0x7FFF)));
    }
}
