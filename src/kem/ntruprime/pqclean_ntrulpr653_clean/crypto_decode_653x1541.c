#include "crypto_decode_653x1541.h"

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

void PQCLEAN_NTRULPR653_CLEAN_crypto_decode_653x1541(void *v, const unsigned char *s) {
    int16 *R0 = v;
    uint16 R1[327], R2[164], R3[82], R4[41], R5[21], R6[11], R7[6], R8[3], R9[2], R10[1];
    long long i;
    uint16 r0;
    uint32 r1, r2;

    s += PQCLEAN_NTRULPR653_CLEAN_crypto_decode_653x1541_STRBYTES;
    r1 = 0;
    r1 = (r1 << 8) | *--s;
    r1 = (r1 << 8) | *--s;
    r1 = uint32_mod_uint14(r1, 2608); /* needed only for invalid inputs */
    R10[0] = (uint16) r1;

    r2 = R10[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 71);
    R9[0] = r0;
    r1 = uint32_mod_uint14(r1, 9402); /* needed only for invalid inputs */
    R9[1] = (uint16) r1;

    R8[2] = R9[1];
    r2 = R9[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 134);
    R8[0] = r0;
    r1 = uint32_mod_uint14(r1, 134); /* needed only for invalid inputs */
    R8[1] = (uint16) r1;

    r2 = R8[2];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 2953);
    R7[4] = r0;
    r1 = uint32_mod_uint14(r1, 815); /* needed only for invalid inputs */
    R7[5] = (uint16) r1;
    for (i = 1; i >= 0; --i) {
        r2 = R8[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 2953);
        R7[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 2953); /* needed only for invalid inputs */
        R7[2 * i + 1] = (uint16) r1;
    }

    R6[10] = R7[5];
    for (i = 4; i >= 0; --i) {
        r2 = R7[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 13910);
        R6[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 13910); /* needed only for invalid inputs */
        R6[2 * i + 1] = (uint16) r1;
    }

    R5[20] = R6[10];
    for (i = 9; i >= 0; --i) {
        r2 = R6[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1887);
        R5[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 1887); /* needed only for invalid inputs */
        R5[2 * i + 1] = (uint16) r1;
    }

    R4[40] = R5[20];
    for (i = 19; i >= 0; --i) {
        r2 = R5[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 695);
        R4[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 695); /* needed only for invalid inputs */
        R4[2 * i + 1] = (uint16) r1;
    }

    r2 = R4[40];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 6745);
    R3[80] = r0;
    r1 = uint32_mod_uint14(r1, 7910); /* needed only for invalid inputs */
    R3[81] = (uint16) r1;
    for (i = 39; i >= 0; --i) {
        r2 = R4[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 6745);
        R3[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 6745); /* needed only for invalid inputs */
        R3[2 * i + 1] = (uint16) r1;
    }

    r2 = R3[81];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 1314);
    R2[162] = r0;
    r1 = uint32_mod_uint14(r1, 1541); /* needed only for invalid inputs */
    R2[163] = (uint16) r1;
    for (i = 80; i >= 0; --i) {
        r2 = R3[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1314);
        R2[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 1314); /* needed only for invalid inputs */
        R2[2 * i + 1] = (uint16) r1;
    }

    R1[326] = R2[163];
    for (i = 162; i >= 0; --i) {
        r2 = R2[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 9277);
        R1[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 9277); /* needed only for invalid inputs */
        R1[2 * i + 1] = (uint16) r1;
    }

    R0[652] = (int16) (3 * R1[326] - 2310);
    for (i = 325; i >= 0; --i) {
        r2 = R1[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1541);
        R0[2 * i] = (int16) (3 * r0 - 2310);
        r1 = uint32_mod_uint14(r1, 1541); /* needed only for invalid inputs */
        R0[2 * i + 1] = (int16) (3 * r1 - 2310);
    }
}
