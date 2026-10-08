#ifndef FQL_H
#define FQL_H

#include <stdint.h>
#include <string.h>
#include "qruov.h"
#include "gf.h"

static inline void fql_zero(uint8_t z[QRUOV_L])
{
    memset(z, 0, QRUOV_L);
}

static inline void fql_add(uint8_t z[QRUOV_L], const uint8_t x[QRUOV_L], const uint8_t y[QRUOV_L])
{
    for (int i = 0; i < QRUOV_L; i++) {
        z[i] = gf_add(x[i], y[i]);
    }
}

static inline void fql_sub(uint8_t z[QRUOV_L], const uint8_t x[QRUOV_L], const uint8_t y[QRUOV_L])
{
    for (int i = 0; i < QRUOV_L; i++) {
        z[i] = gf_sub(x[i], y[i]);
    }
}

static inline void fql_perm(uint8_t dst[QRUOV_L], const uint8_t src[QRUOV_L])
{
    for (int l = 0; l < QRUOV_L; l++) {
        dst[QRUOV_perm(l)] = src[l];
    }
}

static inline void fql_add_M(uint8_t *dst, const uint8_t *x, const uint8_t *y)
{
    for (int i = 0; i < QRUOV_M; i++) {
        fql_add(&dst[i * QRUOV_L], &x[i * QRUOV_L], &y[i * QRUOV_L]);
    }
}

static inline void fql_sub_M(uint8_t *dst, const uint8_t *x, const uint8_t *y)
{
    for (int i = 0; i < QRUOV_M; i++) {
        fql_sub(&dst[i * QRUOV_L], &x[i * QRUOV_L], &y[i * QRUOV_L]);
    }
}

static inline void fql_perm_M(uint8_t *dst, const uint8_t *src)
{
    for (int i = 0; i < QRUOV_M; i++) {
        fql_perm(&dst[i * QRUOV_L], &src[i * QRUOV_L]);
    }
}

static inline void fql_mul(uint8_t z[QRUOV_L], const uint8_t x[QRUOV_L], const uint8_t y[QRUOV_L])
{
    uint32_t acc[2 * QRUOV_L - 1];
    memset(acc, 0, sizeof(acc));
    for (int i = 0; i < QRUOV_L; i++) {
        for (int j = 0; j < QRUOV_L; j++) {
            acc[i + j] += (uint32_t)x[i] * y[j];
        }
    }
    for (int t = 2 * QRUOV_L - 2; t >= QRUOV_L; t--) {
        acc[t - QRUOV_L] += (uint32_t)QRUOV_fc0 * acc[t];
        acc[t - QRUOV_L + QRUOV_fe] += (uint32_t)QRUOV_fc * acc[t];
    }
    for (int i = 0; i < QRUOV_L; i++) {
        z[i] = gf_reduce(acc[i]);
    }
}

static inline void fql_dot(uint8_t z[QRUOV_L], const uint8_t *a, const uint8_t *b, int len)
{
    uint32_t acc[2 * QRUOV_L - 1];
    memset(acc, 0, sizeof(acc));
    for (int i = 0; i < len; i++) {
        const uint8_t *ai = &a[i * QRUOV_L];
        const uint8_t *bi = &b[i * QRUOV_L];
        for (int j = 0; j < QRUOV_L; j++) {
            const uint32_t aij = ai[j];
            for (int k = 0; k < QRUOV_L; k++) {
                acc[j + k] += aij * bi[k];
            }
        }
    }
    for (int t = 2 * QRUOV_L - 2; t >= QRUOV_L; t--) {
        acc[t - QRUOV_L] += (uint32_t)QRUOV_fc0 * acc[t];
        acc[t - QRUOV_L + QRUOV_fe] += (uint32_t)QRUOV_fc * acc[t];
    }
    for (int i = 0; i < QRUOV_L; i++) {
        z[i] = gf_reduce(acc[i]);
    }
}

static inline void fql_dot_V(uint8_t z[QRUOV_L], const uint8_t *a, const uint8_t *b)
{
    fql_dot(z, a, b, QRUOV_V);
}

static inline void fql_dot_M(uint8_t z[QRUOV_L], const uint8_t *a, const uint8_t *b)
{
    fql_dot(z, a, b, QRUOV_M);
}

static inline void fql_mul_VxV_V(uint8_t *dst, const uint8_t *A_VxV, const uint8_t *x_V)
{
    for (int i = 0; i < QRUOV_V; i++) {
        fql_dot_V(&dst[i * QRUOV_L], &A_VxV[i * QRUOV_V * QRUOV_L], x_V);
    }
}

static inline void fql_mul_MxV_V(uint8_t *dst, const uint8_t *A_MxV, const uint8_t *x_V)
{
    for (int i = 0; i < QRUOV_M; i++) {
        fql_dot_V(&dst[i * QRUOV_L], &A_MxV[i * QRUOV_V * QRUOV_L], x_V);
    }
}

static inline void fql_mul_MxV_VxVt(uint8_t *dst, const uint8_t *A_MxV, const uint8_t *B_VxVt)
{
    for (int i = 0; i < QRUOV_M; i++) {
        for (int j = 0; j < QRUOV_V; j++) {
            fql_dot_V(&dst[(i * QRUOV_V + j) * QRUOV_L],
                      &A_MxV[i * QRUOV_V * QRUOV_L],
                      &B_VxVt[j * QRUOV_V * QRUOV_L]);
        }
    }
}

static inline void fql_mul_MxV_MxVt(uint8_t *dst, const uint8_t *A_MxV, const uint8_t *B_MxV)
{
    for (int i = 0; i < QRUOV_M; i++) {
        for (int j = 0; j < QRUOV_M; j++) {
            fql_dot_V(&dst[(i * QRUOV_M + j) * QRUOV_L],
                      &A_MxV[i * QRUOV_V * QRUOV_L],
                      &B_MxV[j * QRUOV_V * QRUOV_L]);
        }
    }
}

static inline void fql_mul_VxM_M(uint8_t *dst, const uint8_t *A_VxM, const uint8_t *x_M)
{
    for (int i = 0; i < QRUOV_V; i++) {
        fql_dot_M(&dst[i * QRUOV_L], &A_VxM[i * QRUOV_M * QRUOV_L], x_M);
    }
}

static inline void fql_mul_MxM_M(uint8_t *dst, const uint8_t *A_MxM, const uint8_t *x_M)
{
    for (int i = 0; i < QRUOV_M; i++) {
        fql_dot_M(&dst[i * QRUOV_L], &A_MxM[i * QRUOV_M * QRUOV_L], x_M);
    }
}

#endif // FQL_H
