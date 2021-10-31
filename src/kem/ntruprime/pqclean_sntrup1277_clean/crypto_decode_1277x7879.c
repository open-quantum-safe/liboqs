#include "crypto_decode_1277x7879.h"

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

void PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x7879(void *v, const unsigned char *s) {
    int16 *R0 = v;
    uint16 R1[639], R2[320], R3[160], R4[80], R5[40], R6[20], R7[10], R8[5], R9[3], R10[2], R11[1];
    long long i;
    uint16 r0;
    uint32 r1, r2;

    s += PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x7879_STRBYTES;
    r1 = 0;
    r1 = (r1 << 8) | *--s;
    r1 = (r1 << 8) | *--s;
    r1 = uint32_mod_uint14(r1, 3696); /* needed only for invalid inputs */
    R11[0] = (uint16) r1;

    r2 = R11[0];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 376);
    R10[0] = r0;
    r1 = uint32_mod_uint14(r1, 2516); /* needed only for invalid inputs */
    R10[1] = (uint16) r1;

    R9[2] = R10[1];
    r2 = R10[0];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 4962);
    R9[0] = r0;
    r1 = uint32_mod_uint14(r1, 4962); /* needed only for invalid inputs */
    R9[1] = (uint16) r1;

    R8[4] = R9[2];
    for (i = 1; i >= 0; --i) {
        r2 = R9[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 1127);
        R8[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 1127); /* needed only for invalid inputs */
        R8[2 * i + 1] = (uint16) r1;
    }

    r2 = R8[4];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 537);
    R7[8] = r0;
    r1 = uint32_mod_uint14(r1, 1199); /* needed only for invalid inputs */
    R7[9] = (uint16) r1;
    for (i = 3; i >= 0; --i) {
        r2 = R8[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 537);
        R7[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 537); /* needed only for invalid inputs */
        R7[2 * i + 1] = (uint16) r1;
    }

    r2 = R7[9];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 5929);
    R6[18] = r0;
    r1 = uint32_mod_uint14(r1, 13244); /* needed only for invalid inputs */
    R6[19] = (uint16) r1;
    for (i = 8; i >= 0; --i) {
        r2 = R7[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 5929);
        R6[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 5929); /* needed only for invalid inputs */
        R6[2 * i + 1] = (uint16) r1;
    }

    r2 = R6[19];
    uint32_divmod_uint14(&r1, &r0, r2, 77);
    R5[38] = r0;
    r1 = uint32_mod_uint14(r1, 172); /* needed only for invalid inputs */
    R5[39] = (uint16) r1;
    for (i = 18; i >= 0; --i) {
        r2 = R6[i];
        uint32_divmod_uint14(&r1, &r0, r2, 77);
        R5[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 77); /* needed only for invalid inputs */
        R5[2 * i + 1] = (uint16) r1;
    }

    r2 = R5[39];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 140);
    R4[78] = r0;
    r1 = uint32_mod_uint14(r1, 313); /* needed only for invalid inputs */
    R4[79] = (uint16) r1;
    for (i = 38; i >= 0; --i) {
        r2 = R5[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 140);
        R4[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 140); /* needed only for invalid inputs */
        R4[2 * i + 1] = (uint16) r1;
    }

    r2 = R4[79];
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 189);
    R3[158] = r0;
    r1 = uint32_mod_uint14(r1, 423); /* needed only for invalid inputs */
    R3[159] = (uint16) r1;
    for (i = 78; i >= 0; --i) {
        r2 = R4[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 189);
        R3[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 189); /* needed only for invalid inputs */
        R3[2 * i + 1] = (uint16) r1;
    }

    r2 = R3[159];
    r2 = (r2 << 8) | *--s;
    r2 = (r2 << 8) | *--s;
    uint32_divmod_uint14(&r1, &r0, r2, 3511);
    R2[318] = r0;
    r1 = uint32_mod_uint14(r1, 7879); /* needed only for invalid inputs */
    R2[319] = (uint16) r1;
    for (i = 158; i >= 0; --i) {
        r2 = R3[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 3511);
        R2[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 3511); /* needed only for invalid inputs */
        R2[2 * i + 1] = (uint16) r1;
    }

    R1[638] = R2[319];
    for (i = 318; i >= 0; --i) {
        r2 = R2[i];
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 948);
        R1[2 * i] = r0;
        r1 = uint32_mod_uint14(r1, 948); /* needed only for invalid inputs */
        R1[2 * i + 1] = (uint16) r1;
    }

    R0[1276] = (int16) (R1[638] - 3939);
    for (i = 637; i >= 0; --i) {
        r2 = R1[i];
        r2 = (r2 << 8) | *--s;
        r2 = (r2 << 8) | *--s;
        uint32_divmod_uint14(&r1, &r0, r2, 7879);
        R0[2 * i] = (int16) (r0 - 3939);
        r1 = uint32_mod_uint14(r1, 7879); /* needed only for invalid inputs */
        R0[2 * i + 1] = (int16) (r1 - 3939);
    }
}
