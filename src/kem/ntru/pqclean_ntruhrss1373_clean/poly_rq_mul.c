#include "poly.h"

/* Polynomial multiplication using     */
/* four layers of Karatsuba.           */

#define L PAD32(NTRU_N)
#define M (L/4)
#define K (L/16)

static void k2x4_mul(uint16_t ab[2 * L], const uint16_t a[L], const uint16_t b[L]);

static void k2x4_eval_00(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_01(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_0inf(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_10(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_11(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_1inf(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_inf0(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_inf1(uint16_t r[9 * K], const uint16_t a[L]);
static void k2x4_eval_infinf(uint16_t r[9 * K], const uint16_t a[L]);
static inline void k2x2_eval(uint16_t r[9 * K]);

static void k2x4_basemul(uint16_t r[18 * K], const uint16_t a[9 * K], const uint16_t b[9 * K]);
static inline void schoolbook_KxK(uint16_t r[2 * K], const uint16_t a[K], const uint16_t b[K]);

static void k2x4_interpolate(uint16_t r[2 * L], const uint16_t a[81 * 2 * K]);
static inline void k2x2_interpolate(uint16_t r[2 * M], const uint16_t a[18 * K]);

void PQCLEAN_NTRUHRSS1373_CLEAN_poly_Rq_mul(poly *r, const poly *a, const poly *b) {
    size_t i;
    uint16_t ab[2 * L];

    for (i = 0; i < NTRU_N; i++) {
        ab[i] = a->coeffs[i];
        ab[L + i] = b->coeffs[i];
    }
    for (i = NTRU_N; i < L; i++) {
        ab[i] = 0;
        ab[L + i] = 0;
    }

    k2x4_mul(ab, ab, ab + L);

    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = ab[i] + ab[NTRU_N + i];
    }
}

static void k2x4_mul(uint16_t ab[2 * L], const uint16_t a[L], const uint16_t b[L]) {
    uint16_t tmpA[9 * K];
    uint16_t tmpB[9 * K];
    uint16_t eC[81 * 2 * K];

    k2x4_eval_00(tmpA, a);
    k2x4_eval_00(tmpB, b);
    k2x4_basemul(eC + 0 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_0inf(tmpA, a);
    k2x4_eval_0inf(tmpB, b);
    k2x4_basemul(eC + 1 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_inf0(tmpA, a);
    k2x4_eval_inf0(tmpB, b);
    k2x4_basemul(eC + 2 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_infinf(tmpA, a);
    k2x4_eval_infinf(tmpB, b);
    k2x4_basemul(eC + 3 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_01(tmpA, a);
    k2x4_eval_01(tmpB, b);
    k2x4_basemul(eC + 4 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_1inf(tmpA, a);
    k2x4_eval_1inf(tmpB, b);
    k2x4_basemul(eC + 5 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_10(tmpA, a);
    k2x4_eval_10(tmpB, b);
    k2x4_basemul(eC + 6 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_inf1(tmpA, a);
    k2x4_eval_inf1(tmpB, b);
    k2x4_basemul(eC + 7 * 9 * 2 * K, tmpA, tmpB);

    k2x4_eval_11(tmpA, a);
    k2x4_eval_11(tmpB, b);
    k2x4_basemul(eC + 8 * 9 * 2 * K, tmpA, tmpB);

    k2x4_interpolate(ab, eC);
}

static void k2x4_eval_00(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i] = a[i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_01(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i] = a[0 * M + i];
        r[i] += a[1 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_0inf(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i] = a[1 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_10(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i]  = a[0 * M + i];
        r[i] += a[2 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_11(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i]  = a[0 * M + i];
        r[i] += a[1 * M + i];
        r[i] += a[2 * M + i];
        r[i] += a[3 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_1inf(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i]  = a[1 * M + i];
        r[i] += a[3 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_inf0(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i] = a[2 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_inf1(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i]  = a[2 * M + i];
        r[i] += a[3 * M + i];
    }
    k2x2_eval(r);
}

static void k2x4_eval_infinf(uint16_t r[9 * K], const uint16_t a[L]) {
    for (size_t i = 0; i < M; i++) {
        r[i] = a[3 * M + i];
    }
    k2x2_eval(r);
}

static inline void k2x2_eval(uint16_t r[9 * K]) {
    /* Input:  e + f.Y + g.Y^2 + h.Y^3                              */
    /* Output: [ e | f | g | h | e+f | f+h | g+e | h+g | e+f+g+h ]  */

    size_t i;
    for (i = 0; i < 4 * K; i++) {
        r[4 * K + i] = r[i];
    }
    for (i = 0; i < K; i++) {
        r[4 * K + i] += r[1 * K + i];
        r[5 * K + i] += r[3 * K + i];
        r[6 * K + i] += r[0 * K + i];
        r[7 * K + i] += r[2 * K + i];
        r[8 * K + i] = r[5 * K + i];
        r[8 * K + i] += r[6 * K + i];
    }
}

static void k2x4_basemul(uint16_t r[18 * K], const uint16_t a[9 * K], const uint16_t b[9 * K]) {
    schoolbook_KxK(r + 0 * 2 * K, a + 0 * K, b + 0 * K);
    schoolbook_KxK(r + 1 * 2 * K, a + 1 * K, b + 1 * K);
    schoolbook_KxK(r + 2 * 2 * K, a + 2 * K, b + 2 * K);
    schoolbook_KxK(r + 3 * 2 * K, a + 3 * K, b + 3 * K);
    schoolbook_KxK(r + 4 * 2 * K, a + 4 * K, b + 4 * K);
    schoolbook_KxK(r + 5 * 2 * K, a + 5 * K, b + 5 * K);
    schoolbook_KxK(r + 6 * 2 * K, a + 6 * K, b + 6 * K);
    schoolbook_KxK(r + 7 * 2 * K, a + 7 * K, b + 7 * K);
    schoolbook_KxK(r + 8 * 2 * K, a + 8 * K, b + 8 * K);
}

static inline void schoolbook_KxK(uint16_t r[2 * K], const uint16_t a[K], const uint16_t b[K]) {
    size_t i, j;
    for (j = 0; j < K; j++) {
        r[j] = a[0] * (uint32_t)b[j];
    }
    for (i = 1; i < K; i++) {
        for (j = 0; j < K - 1; j++) {
            r[i + j] += a[i] * (uint32_t)b[j];
        }
        r[i + K - 1] = a[i] * (uint32_t)b[K - 1];
    }
    r[2 * K - 1] = 0;
}

static void k2x4_interpolate(uint16_t r[2 * L], const uint16_t a[81 * 2 * K]) {
    size_t i;

    uint16_t tmp[4 * M];

    uint16_t P1[2 * M];
    uint16_t P2[2 * M];
    uint16_t P3[2 * M];

    k2x2_interpolate(P1, a + 0 * 9 * 2 * K);
    k2x2_interpolate(P2, a + 1 * 9 * 2 * K);
    k2x2_interpolate(P3, a + 4 * 9 * 2 * K);
    for (i = 0; i < 2 * M; i++) {
        r[0 * M + i] = P1[i];
        r[2 * M + i] = P2[i];
    }

    for (i = 0; i < 2 * M; i++) {
        r[1 * M + i] += P3[i] - P1[i] - P2[i];
    }

    k2x2_interpolate(P1, a + 2 * 9 * 2 * K);
    k2x2_interpolate(P2, a + 3 * 9 * 2 * K);
    k2x2_interpolate(P3, a + 7 * 9 * 2 * K);
    for (i = 0; i < 2 * M; i++) {
        r[4 * M + i] = P1[i];
        r[6 * M + i] = P2[i];
    }

    for (i = 0; i < 2 * M; i++) {
        r[5 * M + i] += P3[i] - P1[i] - P2[i];
    }

    k2x2_interpolate(P1, a + 6 * 9 * 2 * K);
    k2x2_interpolate(P2, a + 5 * 9 * 2 * K);
    k2x2_interpolate(P3, a + 8 * 9 * 2 * K);
    for (i = 0; i < 2 * M; i++) {
        tmp[0 * M + i] = P1[i];
        tmp[2 * M + i] = P2[i];
    }

    for (i = 0; i < 2 * M; i++) {
        tmp[1 * M + i] += P3[i] - P1[i] - P2[i];
    }

    for (i = 0; i < 4 * M; i++) {
        tmp[0 * M + i] = tmp[0 * M + i] - r[0 * M + i] - r[4 * M + i];
    }

    for (i = 0; i < 4 * M; i++) {
        r[2 * M + i] += tmp[0 * M + i];
    }
}

static inline void k2x2_interpolate(uint16_t r[2 * M], const uint16_t a[18 * K]) {
    size_t i;
    uint16_t tmp[4 * K];

    for (i = 0; i < 2 * K; i++) {
        r[0 * K + i] = a[0 * K + i];
        r[2 * K + i] = a[2 * K + i];
    }

    for (i = 0; i < 2 * K; i++) {
        r[1 * K + i] += a[8 * K + i] - a[0 * K + i] - a[2 * K + i];
    }

    for (i = 0; i < 2 * K; i++) {
        r[4 * K + i] = a[4 * K + i];
        r[6 * K + i] = a[6 * K + i];
    }

    for (i = 0; i < 2 * K; i++) {
        r[5 * K + i] += a[14 * K + i] - a[4 * K + i] - a[6 * K + i];
    }

    for (i = 0; i < 2 * K; i++) {
        tmp[0 * K + i] = a[12 * K + i];
        tmp[2 * K + i] = a[10 * K + i];
    }

    for (i = 0; i < 2 * K; i++) {
        tmp[K + i] += a[16 * K + i] - a[12 * K + i] - a[10 * K + i];
    }

    for (i = 0; i < 4 * K; i++) {
        tmp[0 * K + i] = tmp[0 * K + i] - r[0 * K + i] - r[4 * K + i];
    }

    for (i = 0; i < 4 * K; i++) {
        r[2 * K + i] += tmp[0 * K + i];
    }
}

