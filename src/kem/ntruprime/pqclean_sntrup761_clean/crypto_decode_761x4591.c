#include "crypto_decode_761x4591.h"

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

void PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761x4591(void *v, const unsigned char *s) {
    int16 *R0 = v;
    uint16 R1[381], R2[191], R3[96], R4[48], R5[24], R6[12], R7[6], R8[3], R9[2], R10[1];
    long long i;
    uint16 r0;
    uint32 r1, r2;

    s += PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761x4591_STRBYTES;
    r1 = 0;
    r1 = (r1 << 8) | *--s;
    r1 = (r1 << 8) | *--s;
    r1 = uint32_mod_uint14(r1, 1608); /* needed only for invalid inputs */
    R10[0] = (uint16) r1;

    r2 = R10[0];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 9470);
    R9[0] = r0;
    r1 = uint32_mod_uint14(r1, 11127); /* needed only for invalid inputs */
    R9[1] = (uint16) r1;

    R8[2] = R9[1];
    r2 = R9[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 1557);
    R8[0] = r0;
    r1 = uint32_mod_uint14(r1, 1557); /* needed only for invalid inputs */
    R8[1] = (uint16) r1;

    r2 = R8[2];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 10101);
    R7[4] = r0;
    r1 = uint32_mod_uint14(r1, 282); /* needed only for invalid inputs */
    R7[5] = (uint16) r1;
    for (i = 1; i >= 0; --i) {
        r2 = R8[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 10101);
        R7[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 10101); /* needed only for invalid inputs */
        R7[2 * i + 1] = (uint16) r1;
    }

    r2 = R7[5];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 1608);
    R6[10] = r0;
    r1 = uint32_mod_uint14(r1, 11468); /* needed only for invalid inputs */
    R6[11] = (uint16) r1;
    for (i = 4; i >= 0; --i) {
        r2 = R7[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1608);
        R6[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 1608); /* needed only for invalid inputs */
        R6[2 * i + 1] = (uint16) r1;
    }

    r2 = R6[11];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 10265);
    R5[22] = r0;
    r1 = uint32_mod_uint14(r1, 286); /* needed only for invalid inputs */
    R5[23] = (uint16) r1;
    for (i = 10; i >= 0; --i) {
        r2 = R6[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 10265);
        R5[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 10265); /* needed only for invalid inputs */
        R5[2 * i + 1] = (uint16) r1;
    }

    r2 = R5[23];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 1621);
    R4[46] = r0;
    r1 = uint32_mod_uint14(r1, 11550); /* needed only for invalid inputs */
    R4[47] = (uint16) r1;
    for (i = 22; i >= 0; --i) {
        r2 = R5[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1621);
        R4[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 1621); /* needed only for invalid inputs */
        R4[2 * i + 1] = (uint16) r1;
    }

    r2 = R4[47];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 644);
    R3[94] = r0;
    r1 = uint32_mod_uint14(r1, 4591); /* needed only for invalid inputs */
    R3[95] = (uint16) r1;
    for (i = 46; i >= 0; --i) {
        r2 = R4[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 644);
        R3[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 644); /* needed only for invalid inputs */
        R3[2 * i + 1] = (uint16) r1;
    }

    R2[190] = R3[95];
    for (i = 94; i >= 0; --i) {
        r2 = R3[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 406);
        R2[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 406); /* needed only for invalid inputs */
        R2[2 * i + 1] = (uint16) r1;
    }

    R1[380] = R2[190];
    for (i = 189; i >= 0; --i) {
        r2 = R2[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 322);
        R1[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 322); /* needed only for invalid inputs */
        R1[2 * i + 1] = (uint16) r1;
    }

    R0[760] = (int16) (R1[380] - 2295);
    for (i = 379; i >= 0; --i) {
        r2 = R1[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 4591);
        R0[2 * i] = (int16) (r0 - 2295);
        r1 = uint32_mod_uint14(r1, 4591); /* needed only for invalid inputs */
        R0[2 * i + 1] = (int16) (r1 - 2295);
    }
}
