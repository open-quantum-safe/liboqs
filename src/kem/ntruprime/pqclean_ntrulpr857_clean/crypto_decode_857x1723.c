#include "crypto_decode_857x1723.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t

/*
CPU division instruction typically takes time depending on x.
This software is designed to take time independent of x.
Time still varies depending on m; user must ensure that m is constant.
Time also varies on CPUs where multiplication is variable-time.
There could be more CPU issues.
There could also be compiler issues.
*/

static void uint32_divmod_uint14(uint32 *q, uint16 *r, uint32 x, uint16 m) {
    uint32 v = 0x80000000;
    uint32 qpart;
    uint32 mask;

    v /= m;

    /* caller guarantees m > 0 */
    /* caller guarantees m < 16384 */
    /* vm <= 2^31 <= vm+m-1 */
    /* xvm <= 2^31 x <= xvm+x(m-1) */

    *q = 0;

    qpart = (uint32) ((x * (uint64)v) >> 31);
    /* 2^31 qpart <= xv <= 2^31 qpart + 2^31-1 */
    /* 2^31 qpart m <= xvm <= 2^31 qpart m + (2^31-1)m */
    /* 2^31 qpart m <= 2^31 x <= 2^31 qpart m + (2^31-1)m + x(m-1) */
    /* 0 <= 2^31 newx <= (2^31-1)m + x(m-1) */
    /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
    /* 0 <= newx <= (1-1/2^31)(2^14-1) + (2^32-1)((2^14-1)-1)/2^31 */

    x -= qpart * m;
    *q += qpart;
    /* x <= 49146 */

    qpart = (uint32) ((x * (uint64)v) >> 31);
    /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
    /* 0 <= newx <= m + 49146(2^14-1)/2^31 */
    /* 0 <= newx <= m + 0.4 */
    /* 0 <= newx <= m */

    x -= qpart * m;
    *q += qpart;
    /* x <= m */

    x -= m;
    *q += 1;
    mask = (~(x >> 31) + 1);
    x += mask & (uint32)m;
    *q += mask;
    /* x < m */

    *r = (uint16) x;
}

static uint16 uint32_mod_uint14(uint32 x, uint16 m) {
    uint32 q;
    uint16 r;
    uint32_divmod_uint14(&q, &r, x, m);
    return r;
}

void PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857x1723(void *v, const unsigned char *s) {
    int16 *R0 = v;
    uint16 R1[429], R2[215], R3[108], R4[54], R5[27], R6[14], R7[7], R8[4], R9[2], R10[1];
    long long i;
    uint16 r0;
    uint32 r1, r2;

    s += PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857x1723_STRBYTES;
    r1 = 0;
    r1 = (r1 << 8) | *--s;
    r1 = uint32_mod_uint14(r1, 160); /* needed only for invalid inputs */
    R10[0] = (uint16) r1;

    r2 = R10[0];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 743);
    R9[0] = r0;
    r1 = uint32_mod_uint14(r1, 14044); /* needed only for invalid inputs */
    R9[1] = (uint16) r1;

    r2 = R9[1];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 436);
    R8[2] = r0;
    r1 = uint32_mod_uint14(r1, 8246); /* needed only for invalid inputs */
    R8[3] = (uint16) r1;
    r2 = R9[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 436);
    R8[0] = r0;
    r1 = uint32_mod_uint14(r1, 436); /* needed only for invalid inputs */
    R8[1] = (uint16) r1;

    R7[6] = R8[3];
    for (i = 2; i >= 0; --i) {
        r2 = R8[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 334);
        R7[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 334); /* needed only for invalid inputs */
        R7[2 * i + 1] = (uint16) r1;
    }

    r2 = R7[6];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 292);
    R6[12] = r0;
    r1 = uint32_mod_uint14(r1, 7229); /* needed only for invalid inputs */
    R6[13] = (uint16) r1;
    for (i = 5; i >= 0; --i) {
        r2 = R7[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 292);
        R6[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 292); /* needed only for invalid inputs */
        R6[2 * i + 1] = (uint16) r1;
    }

    R5[26] = R6[13];
    for (i = 12; i >= 0; --i) {
        r2 = R6[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 273);
        R5[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 273); /* needed only for invalid inputs */
        R5[2 * i + 1] = (uint16) r1;
    }

    r2 = R5[26];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 4225);
    R4[52] = r0;
    r1 = uint32_mod_uint14(r1, 438); /* needed only for invalid inputs */
    R4[53] = (uint16) r1;
    for (i = 25; i >= 0; --i) {
        r2 = R5[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 4225);
        R4[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 4225); /* needed only for invalid inputs */
        R4[2 * i + 1] = (uint16) r1;
    }

    r2 = R4[53];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 65);
    R3[106] = r0;
    r1 = uint32_mod_uint14(r1, 1723); /* needed only for invalid inputs */
    R3[107] = (uint16) r1;
    for (i = 52; i >= 0; --i) {
        r2 = R4[i];
        uint32_divmod_uint14(&r1, &r0, r2, 65);
        R3[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 65); /* needed only for invalid inputs */
        R3[2 * i + 1] = (uint16) r1;
    }

    R2[214] = R3[107];
    for (i = 106; i >= 0; --i) {
        r2 = R3[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 2053);
        R2[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 2053); /* needed only for invalid inputs */
        R2[2 * i + 1] = (uint16) r1;
    }

    R1[428] = R2[214];
    for (i = 213; i >= 0; --i) {
        r2 = R2[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 11597);
        R1[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 11597); /* needed only for invalid inputs */
        R1[2 * i + 1] = (uint16) r1;
    }

    R0[856] = (int16) (3 * R1[428] - 2583);
    for (i = 427; i >= 0; --i) {
        r2 = R1[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1723);
        R0[2 * i] = (int16) (3 * r0 - 2583);
        r1 = uint32_mod_uint14(r1, 1723); /* needed only for invalid inputs */
        R0[2 * i + 1] = (int16) (3 * r1 - 2583);
    }
}
