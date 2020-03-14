#include "vec.h"

#include "params.h"

vec PQCLEAN_MCELIECE8192128F_VEC_vec_setbits(vec b) {
    vec ret = -b;

    return ret;
}

vec PQCLEAN_MCELIECE8192128F_VEC_vec_set1_16b(uint16_t v) {
    vec ret;

    ret = v;
    ret |= ret << 16;
    ret |= ret << 32;

    return ret;
}

void PQCLEAN_MCELIECE8192128F_VEC_vec_copy(vec *out, const vec *in) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        out[i] = in[i];
    }
}

vec PQCLEAN_MCELIECE8192128F_VEC_vec_or_reduce(const vec *a) {
    int i;
    vec ret;

    ret = a[0];
    for (i = 1; i < GFBITS; i++) {
        ret |= a[i];
    }

    return ret;
}

int PQCLEAN_MCELIECE8192128F_VEC_vec_testz(vec a) {
    a |= a >> 32;
    a |= a >> 16;
    a |= a >> 8;
    a |= a >> 4;
    a |= a >> 2;
    a |= a >> 1;

    return (int)(a & 1) ^ 1;
}

void PQCLEAN_MCELIECE8192128F_VEC_vec_mul(vec *h, const vec *f, const vec *g) {
    int i, j;
    vec buf[ 2 * GFBITS - 1 ];

    for (i = 0; i < 2 * GFBITS - 1; i++) {
        buf[i] = 0;
    }

    for (i = 0; i < GFBITS; i++) {
        for (j = 0; j < GFBITS; j++) {
            buf[i + j] ^= f[i] & g[j];
        }
    }

    for (i = 2 * GFBITS - 2; i >= GFBITS; i--) {
        buf[i - GFBITS + 4] ^= buf[i];
        buf[i - GFBITS + 3] ^= buf[i];
        buf[i - GFBITS + 1] ^= buf[i];
        buf[i - GFBITS + 0] ^= buf[i];
    }

    for (i = 0; i < GFBITS; i++) {
        h[i] = buf[i];
    }
}

/* bitsliced field squarings */
void PQCLEAN_MCELIECE8192128F_VEC_vec_sq(vec *out, const vec *in) {
    int i;
    vec result[GFBITS], t;

    t = in[11] ^ in[12];

    result[0] = in[0] ^ in[11];
    result[1] = in[7] ^ t;
    result[2] = in[1] ^ in[7];
    result[3] = in[8] ^ t;
    result[4] = in[2] ^ in[7];
    result[4] = result[4] ^ in[8];
    result[4] = result[4] ^ t;
    result[5] = in[7] ^ in[9];
    result[6] = in[3] ^ in[8];
    result[6] = result[6] ^ in[9];
    result[6] = result[6] ^ in[12];
    result[7] = in[8] ^ in[10];
    result[8] = in[4] ^ in[9];
    result[8] = result[8] ^ in[10];
    result[9] = in[9] ^ in[11];
    result[10] = in[5] ^ in[10];
    result[10] = result[10] ^ in[11];
    result[11] = in[10] ^ in[12];
    result[12] = in[6] ^ t;

    for (i = 0; i < GFBITS; i++) {
        out[i] = result[i];
    }
}

/* bitsliced field inverses */
void PQCLEAN_MCELIECE8192128F_VEC_vec_inv(vec *out, const vec *in) {
    vec tmp_11[ GFBITS ];
    vec tmp_1111[ GFBITS ];

    PQCLEAN_MCELIECE8192128F_VEC_vec_copy(out, in);

    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_mul(tmp_11, out, in); // ^11

    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, tmp_11);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_mul(tmp_1111, out, tmp_11); // ^1111

    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, tmp_1111);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_mul(out, out, tmp_1111); // ^11111111

    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out);
    PQCLEAN_MCELIECE8192128F_VEC_vec_mul(out, out, tmp_1111); // ^111111111111

    PQCLEAN_MCELIECE8192128F_VEC_vec_sq(out, out); // ^1111111111110
}

