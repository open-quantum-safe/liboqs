/*
 * Constant time implementation of the Haraka hash function.
 *
 * The bit-sliced implementation of the AES round functions are
 * based on the AES implementation in BearSSL written
 * by Thomas Pornin <pornin@bolet.org>
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "haraka.h"

#define HARAKAS_RATE 32

static const uint64_t haraka512_rc64[10][8] = {
    {0x24cf0ab9086f628b, 0xbdd6eeecc83b8382, 0xd96fb0306cdad0a7, 0xaace082ac8f95f89, 0x449d8e8870d7041f, 0x49bb2f80b2b3e2f8, 0x0569ae98d93bb258, 0x23dc9691e7d6a4b1},
    {0xd8ba10ede0fe5b6e, 0x7ecf7dbe424c7b8e, 0x6ea9949c6df62a31, 0xbf3f3c97ec9c313e, 0x241d03a196a1861e, 0xead3a51116e5a2ea, 0x77d479fcad9574e3, 0x18657a1af894b7a0},
    {0x10671e1a7f595522, 0xd9a00ff675d28c7b, 0x2f1edf0d2b9ba661, 0xb8ff58b8e3de45f9, 0xee29261da9865c02, 0xd1532aa4b50bdf43, 0x8bf858159b231bb1, 0xdf17439d22d4f599},
    {0xdd4b2f0870b918c0, 0x757a81f3b39b1bb6, 0x7a5c556898952e3f, 0x7dd70a16d915d87a, 0x3ae61971982b8301, 0xc3ab319e030412be, 0x17c0033ac094a8cb, 0x5a0630fc1a8dc4ef},
    {0x17708988c1632f73, 0xf92ddae090b44f4f, 0x11ac0285c43aa314, 0x509059941936b8ba, 0xd03e152fa2ce9b69, 0x3fbcbcb63a32998b, 0x6204696d692254f7, 0x915542ed93ec59b4},
    {0xf4ed94aa8879236e, 0xff6cb41cd38e03c0, 0x069b38602368aeab, 0x669495b820f0ddba, 0xf42013b1b8bf9e3d, 0xcf935efe6439734d, 0xbc1dcf42ca29e3f8, 0x7e6d3ed29f78ad67},
    {0xf3b0f6837ffcddaa, 0x3a76faef934ddf41, 0xcec7ae583a9c8e35, 0xe4dd18c68f0260af, 0x2c0e5df1ad398eaa, 0x478df5236ae22e8c, 0xfb944c46fe865f39, 0xaa48f82f028132ba},
    {0x231b9ae2b76aca77, 0x292a76a712db0b40, 0x5850625dc8134491, 0x73137dd469810fb5, 0x8a12a6a202a474fd, 0xd36fd9daa78bdb80, 0xb34c5e733505706f, 0xbaf1cdca818d9d96},
    {0x2e99781335e8c641, 0xbddfe5cce47d560e, 0xf74e9bf32e5e040c, 0x1d7a709d65996be9, 0x670df36a9cf66cdd, 0xd05ef84a176a2875, 0x0f888e828cb1c44e, 0x1a79e9c9727b052c},
    {0x83497348628d84de, 0x2e9387d51f22a754, 0xb000068da2f852d6, 0x378c9e1190fd6fe5, 0x870027c316de7293, 0xe51a9d4462e047bb, 0x90ecf7f8c6251195, 0x655953bfbed90a9c},
};

static inline uint32_t br_dec32le(const unsigned char *src) {
    return (uint32_t)src[0]
           | ((uint32_t)src[1] << 8)
           | ((uint32_t)src[2] << 16)
           | ((uint32_t)src[3] << 24);
}

static void br_range_dec32le(uint32_t *v, size_t num, const unsigned char *src) {
    while (num-- > 0) {
        *v ++ = br_dec32le(src);
        src += 4;
    }
}

static inline void br_enc32le(unsigned char *dst, uint32_t x) {
    dst[0] = (unsigned char)x;
    dst[1] = (unsigned char)(x >> 8);
    dst[2] = (unsigned char)(x >> 16);
    dst[3] = (unsigned char)(x >> 24);
}


static void br_range_enc32le(unsigned char *dst, const uint32_t *v, size_t num) {
    while (num-- > 0) {
        br_enc32le(dst, *v ++);
        dst += 4;
    }
}

static void br_aes_ct64_bitslice_Sbox(uint64_t *q) {
    /*
     * This S-box implementation is a straightforward translation of
     * the circuit described by Boyar and Peralta in "A new
     * combinational logic minimization technique with applications
     * to cryptology" (https://eprint.iacr.org/2009/191.pdf).
     *
     * Note that variables x* (input) and s* (output) are numbered
     * in "reverse" order (x0 is the high bit, x7 is the low bit).
     */

    uint64_t x0, x1, x2, x3, x4, x5, x6, x7;
    uint64_t y1, y2, y3, y4, y5, y6, y7, y8, y9;
    uint64_t y10, y11, y12, y13, y14, y15, y16, y17, y18, y19;
    uint64_t y20, y21;
    uint64_t z0, z1, z2, z3, z4, z5, z6, z7, z8, z9;
    uint64_t z10, z11, z12, z13, z14, z15, z16, z17;
    uint64_t t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    uint64_t t10, t11, t12, t13, t14, t15, t16, t17, t18, t19;
    uint64_t t20, t21, t22, t23, t24, t25, t26, t27, t28, t29;
    uint64_t t30, t31, t32, t33, t34, t35, t36, t37, t38, t39;
    uint64_t t40, t41, t42, t43, t44, t45, t46, t47, t48, t49;
    uint64_t t50, t51, t52, t53, t54, t55, t56, t57, t58, t59;
    uint64_t t60, t61, t62, t63, t64, t65, t66, t67;
    uint64_t s0, s1, s2, s3, s4, s5, s6, s7;

    x0 = q[7];
    x1 = q[6];
    x2 = q[5];
    x3 = q[4];
    x4 = q[3];
    x5 = q[2];
    x6 = q[1];
    x7 = q[0];

    /*
     * Top linear transformation.
     */
    y14 = x3 ^ x5;
    y13 = x0 ^ x6;
    y9 = x0 ^ x3;
    y8 = x0 ^ x5;
    t0 = x1 ^ x2;
    y1 = t0 ^ x7;
    y4 = y1 ^ x3;
    y12 = y13 ^ y14;
    y2 = y1 ^ x0;
    y5 = y1 ^ x6;
    y3 = y5 ^ y8;
    t1 = x4 ^ y12;
    y15 = t1 ^ x5;
    y20 = t1 ^ x1;
    y6 = y15 ^ x7;
    y10 = y15 ^ t0;
    y11 = y20 ^ y9;
    y7 = x7 ^ y11;
    y17 = y10 ^ y11;
    y19 = y10 ^ y8;
    y16 = t0 ^ y11;
    y21 = y13 ^ y16;
    y18 = x0 ^ y16;

    /*
     * Non-linear section.
     */
    t2 = y12 & y15;
    t3 = y3 & y6;
    t4 = t3 ^ t2;
    t5 = y4 & x7;
    t6 = t5 ^ t2;
    t7 = y13 & y16;
    t8 = y5 & y1;
    t9 = t8 ^ t7;
    t10 = y2 & y7;
    t11 = t10 ^ t7;
    t12 = y9 & y11;
    t13 = y14 & y17;
    t14 = t13 ^ t12;
    t15 = y8 & y10;
    t16 = t15 ^ t12;
    t17 = t4 ^ t14;
    t18 = t6 ^ t16;
    t19 = t9 ^ t14;
    t20 = t11 ^ t16;
    t21 = t17 ^ y20;
    t22 = t18 ^ y19;
    t23 = t19 ^ y21;
    t24 = t20 ^ y18;

    t25 = t21 ^ t22;
    t26 = t21 & t23;
    t27 = t24 ^ t26;
    t28 = t25 & t27;
    t29 = t28 ^ t22;
    t30 = t23 ^ t24;
    t31 = t22 ^ t26;
    t32 = t31 & t30;
    t33 = t32 ^ t24;
    t34 = t23 ^ t33;
    t35 = t27 ^ t33;
    t36 = t24 & t35;
    t37 = t36 ^ t34;
    t38 = t27 ^ t36;
    t39 = t29 & t38;
    t40 = t25 ^ t39;

    t41 = t40 ^ t37;
    t42 = t29 ^ t33;
    t43 = t29 ^ t40;
    t44 = t33 ^ t37;
    t45 = t42 ^ t41;
    z0 = t44 & y15;
    z1 = t37 & y6;
    z2 = t33 & x7;
    z3 = t43 & y16;
    z4 = t40 & y1;
    z5 = t29 & y7;
    z6 = t42 & y11;
    z7 = t45 & y17;
    z8 = t41 & y10;
    z9 = t44 & y12;
    z10 = t37 & y3;
    z11 = t33 & y4;
    z12 = t43 & y13;
    z13 = t40 & y5;
    z14 = t29 & y2;
    z15 = t42 & y9;
    z16 = t45 & y14;
    z17 = t41 & y8;

    /*
     * Bottom linear transformation.
     */
    t46 = z15 ^ z16;
    t47 = z10 ^ z11;
    t48 = z5 ^ z13;
    t49 = z9 ^ z10;
    t50 = z2 ^ z12;
    t51 = z2 ^ z5;
    t52 = z7 ^ z8;
    t53 = z0 ^ z3;
    t54 = z6 ^ z7;
    t55 = z16 ^ z17;
    t56 = z12 ^ t48;
    t57 = t50 ^ t53;
    t58 = z4 ^ t46;
    t59 = z3 ^ t54;
    t60 = t46 ^ t57;
    t61 = z14 ^ t57;
    t62 = t52 ^ t58;
    t63 = t49 ^ t58;
    t64 = z4 ^ t59;
    t65 = t61 ^ t62;
    t66 = z1 ^ t63;
    s0 = t59 ^ t63;
    s6 = t56 ^ ~t62;
    s7 = t48 ^ ~t60;
    t67 = t64 ^ t65;
    s3 = t53 ^ t66;
    s4 = t51 ^ t66;
    s5 = t47 ^ t65;
    s1 = t64 ^ ~s3;
    s2 = t55 ^ ~t67;

    q[7] = s0;
    q[6] = s1;
    q[5] = s2;
    q[4] = s3;
    q[3] = s4;
    q[2] = s5;
    q[1] = s6;
    q[0] = s7;
}

static void br_aes_ct_bitslice_Sbox(uint32_t *q) {
    /*
     * This S-box implementation is a straightforward translation of
     * the circuit described by Boyar and Peralta in "A new
     * combinational logic minimization technique with applications
     * to cryptology" (https://eprint.iacr.org/2009/191.pdf).
     *
     * Note that variables x* (input) and s* (output) are numbered
     * in "reverse" order (x0 is the high bit, x7 is the low bit).
     */

    uint32_t x0, x1, x2, x3, x4, x5, x6, x7;
    uint32_t y1, y2, y3, y4, y5, y6, y7, y8, y9;
    uint32_t y10, y11, y12, y13, y14, y15, y16, y17, y18, y19;
    uint32_t y20, y21;
    uint32_t z0, z1, z2, z3, z4, z5, z6, z7, z8, z9;
    uint32_t z10, z11, z12, z13, z14, z15, z16, z17;
    uint32_t t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    uint32_t t10, t11, t12, t13, t14, t15, t16, t17, t18, t19;
    uint32_t t20, t21, t22, t23, t24, t25, t26, t27, t28, t29;
    uint32_t t30, t31, t32, t33, t34, t35, t36, t37, t38, t39;
    uint32_t t40, t41, t42, t43, t44, t45, t46, t47, t48, t49;
    uint32_t t50, t51, t52, t53, t54, t55, t56, t57, t58, t59;
    uint32_t t60, t61, t62, t63, t64, t65, t66, t67;
    uint32_t s0, s1, s2, s3, s4, s5, s6, s7;

    x0 = q[7];
    x1 = q[6];
    x2 = q[5];
    x3 = q[4];
    x4 = q[3];
    x5 = q[2];
    x6 = q[1];
    x7 = q[0];

    /*
     * Top linear transformation.
     */
    y14 = x3 ^ x5;
    y13 = x0 ^ x6;
    y9 = x0 ^ x3;
    y8 = x0 ^ x5;
    t0 = x1 ^ x2;
    y1 = t0 ^ x7;
    y4 = y1 ^ x3;
    y12 = y13 ^ y14;
    y2 = y1 ^ x0;
    y5 = y1 ^ x6;
    y3 = y5 ^ y8;
    t1 = x4 ^ y12;
    y15 = t1 ^ x5;
    y20 = t1 ^ x1;
    y6 = y15 ^ x7;
    y10 = y15 ^ t0;
    y11 = y20 ^ y9;
    y7 = x7 ^ y11;
    y17 = y10 ^ y11;
    y19 = y10 ^ y8;
    y16 = t0 ^ y11;
    y21 = y13 ^ y16;
    y18 = x0 ^ y16;

    /*
     * Non-linear section.
     */
    t2 = y12 & y15;
    t3 = y3 & y6;
    t4 = t3 ^ t2;
    t5 = y4 & x7;
    t6 = t5 ^ t2;
    t7 = y13 & y16;
    t8 = y5 & y1;
    t9 = t8 ^ t7;
    t10 = y2 & y7;
    t11 = t10 ^ t7;
    t12 = y9 & y11;
    t13 = y14 & y17;
    t14 = t13 ^ t12;
    t15 = y8 & y10;
    t16 = t15 ^ t12;
    t17 = t4 ^ t14;
    t18 = t6 ^ t16;
    t19 = t9 ^ t14;
    t20 = t11 ^ t16;
    t21 = t17 ^ y20;
    t22 = t18 ^ y19;
    t23 = t19 ^ y21;
    t24 = t20 ^ y18;

    t25 = t21 ^ t22;
    t26 = t21 & t23;
    t27 = t24 ^ t26;
    t28 = t25 & t27;
    t29 = t28 ^ t22;
    t30 = t23 ^ t24;
    t31 = t22 ^ t26;
    t32 = t31 & t30;
    t33 = t32 ^ t24;
    t34 = t23 ^ t33;
    t35 = t27 ^ t33;
    t36 = t24 & t35;
    t37 = t36 ^ t34;
    t38 = t27 ^ t36;
    t39 = t29 & t38;
    t40 = t25 ^ t39;

    t41 = t40 ^ t37;
    t42 = t29 ^ t33;
    t43 = t29 ^ t40;
    t44 = t33 ^ t37;
    t45 = t42 ^ t41;
    z0 = t44 & y15;
    z1 = t37 & y6;
    z2 = t33 & x7;
    z3 = t43 & y16;
    z4 = t40 & y1;
    z5 = t29 & y7;
    z6 = t42 & y11;
    z7 = t45 & y17;
    z8 = t41 & y10;
    z9 = t44 & y12;
    z10 = t37 & y3;
    z11 = t33 & y4;
    z12 = t43 & y13;
    z13 = t40 & y5;
    z14 = t29 & y2;
    z15 = t42 & y9;
    z16 = t45 & y14;
    z17 = t41 & y8;

    /*
     * Bottom linear transformation.
     */
    t46 = z15 ^ z16;
    t47 = z10 ^ z11;
    t48 = z5 ^ z13;
    t49 = z9 ^ z10;
    t50 = z2 ^ z12;
    t51 = z2 ^ z5;
    t52 = z7 ^ z8;
    t53 = z0 ^ z3;
    t54 = z6 ^ z7;
    t55 = z16 ^ z17;
    t56 = z12 ^ t48;
    t57 = t50 ^ t53;
    t58 = z4 ^ t46;
    t59 = z3 ^ t54;
    t60 = t46 ^ t57;
    t61 = z14 ^ t57;
    t62 = t52 ^ t58;
    t63 = t49 ^ t58;
    t64 = z4 ^ t59;
    t65 = t61 ^ t62;
    t66 = z1 ^ t63;
    s0 = t59 ^ t63;
    s6 = t56 ^ ~t62;
    s7 = t48 ^ ~t60;
    t67 = t64 ^ t65;
    s3 = t53 ^ t66;
    s4 = t51 ^ t66;
    s5 = t47 ^ t65;
    s1 = t64 ^ ~s3;
    s2 = t55 ^ ~t67;

    q[7] = s0;
    q[6] = s1;
    q[5] = s2;
    q[4] = s3;
    q[3] = s4;
    q[2] = s5;
    q[1] = s6;
    q[0] = s7;
}

static void br_aes_ct_ortho(uint32_t *q) {
#define SWAPN_32(cl, ch, s, x, y)   do { \
        uint32_t a, b; \
        a = (x); \
        b = (y); \
        (x) = (a & (uint32_t)(cl)) | ((b & (uint32_t)(cl)) << (s)); \
        (y) = ((a & (uint32_t)(ch)) >> (s)) | (b & (uint32_t)(ch)); \
    } while (0)

#define SWAP2_32(x, y)   SWAPN_32(0x55555555, 0xAAAAAAAA, 1, x, y)
#define SWAP4_32(x, y)   SWAPN_32(0x33333333, 0xCCCCCCCC, 2, x, y)
#define SWAP8_32(x, y)   SWAPN_32(0x0F0F0F0F, 0xF0F0F0F0, 4, x, y)

    SWAP2_32(q[0], q[1]);
    SWAP2_32(q[2], q[3]);
    SWAP2_32(q[4], q[5]);
    SWAP2_32(q[6], q[7]);

    SWAP4_32(q[0], q[2]);
    SWAP4_32(q[1], q[3]);
    SWAP4_32(q[4], q[6]);
    SWAP4_32(q[5], q[7]);

    SWAP8_32(q[0], q[4]);
    SWAP8_32(q[1], q[5]);
    SWAP8_32(q[2], q[6]);
    SWAP8_32(q[3], q[7]);
}

static inline void add_round_key32(uint32_t *q, const uint32_t *sk) {
    q[0] ^= sk[0];
    q[1] ^= sk[1];
    q[2] ^= sk[2];
    q[3] ^= sk[3];
    q[4] ^= sk[4];
    q[5] ^= sk[5];
    q[6] ^= sk[6];
    q[7] ^= sk[7];
}

static inline void shift_rows32(uint32_t *q) {
    int i;

    for (i = 0; i < 8; i++) {
        uint32_t x;

        x = q[i];
        q[i] = (x & 0x000000FF)
               | ((x & 0x0000FC00) >> 2) | ((x & 0x00000300) << 6)
               | ((x & 0x00F00000) >> 4) | ((x & 0x000F0000) << 4)
               | ((x & 0xC0000000) >> 6) | ((x & 0x3F000000) << 2);
    }
}

static inline uint32_t rotr16(uint32_t x) {
    return (x << 16) | (x >> 16);
}

static inline void mix_columns32(uint32_t *q) {
    uint32_t q0, q1, q2, q3, q4, q5, q6, q7;
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7;

    q0 = q[0];
    q1 = q[1];
    q2 = q[2];
    q3 = q[3];
    q4 = q[4];
    q5 = q[5];
    q6 = q[6];
    q7 = q[7];
    r0 = (q0 >> 8) | (q0 << 24);
    r1 = (q1 >> 8) | (q1 << 24);
    r2 = (q2 >> 8) | (q2 << 24);
    r3 = (q3 >> 8) | (q3 << 24);
    r4 = (q4 >> 8) | (q4 << 24);
    r5 = (q5 >> 8) | (q5 << 24);
    r6 = (q6 >> 8) | (q6 << 24);
    r7 = (q7 >> 8) | (q7 << 24);

    q[0] = q7 ^ r7 ^ r0 ^ rotr16(q0 ^ r0);
    q[1] = q0 ^ r0 ^ q7 ^ r7 ^ r1 ^ rotr16(q1 ^ r1);
    q[2] = q1 ^ r1 ^ r2 ^ rotr16(q2 ^ r2);
    q[3] = q2 ^ r2 ^ q7 ^ r7 ^ r3 ^ rotr16(q3 ^ r3);
    q[4] = q3 ^ r3 ^ q7 ^ r7 ^ r4 ^ rotr16(q4 ^ r4);
    q[5] = q4 ^ r4 ^ r5 ^ rotr16(q5 ^ r5);
    q[6] = q5 ^ r5 ^ r6 ^ rotr16(q6 ^ r6);
    q[7] = q6 ^ r6 ^ r7 ^ rotr16(q7 ^ r7);
}

static void br_aes_ct64_ortho(uint64_t *q) {
#define SWAPN(cl, ch, s, x, y)   do { \
        uint64_t a, b; \
        a = (x); \
        b = (y); \
        (x) = (a & (uint64_t)(cl)) | ((b & (uint64_t)(cl)) << (s)); \
        (y) = ((a & (uint64_t)(ch)) >> (s)) | (b & (uint64_t)(ch)); \
    } while (0)

#define SWAP2(x, y)    SWAPN(0x5555555555555555, 0xAAAAAAAAAAAAAAAA,  1, x, y)
#define SWAP4(x, y)    SWAPN(0x3333333333333333, 0xCCCCCCCCCCCCCCCC,  2, x, y)
#define SWAP8(x, y)    SWAPN(0x0F0F0F0F0F0F0F0F, 0xF0F0F0F0F0F0F0F0,  4, x, y)

    SWAP2(q[0], q[1]);
    SWAP2(q[2], q[3]);
    SWAP2(q[4], q[5]);
    SWAP2(q[6], q[7]);

    SWAP4(q[0], q[2]);
    SWAP4(q[1], q[3]);
    SWAP4(q[4], q[6]);
    SWAP4(q[5], q[7]);

    SWAP8(q[0], q[4]);
    SWAP8(q[1], q[5]);
    SWAP8(q[2], q[6]);
    SWAP8(q[3], q[7]);
}


static void br_aes_ct64_interleave_in(uint64_t *q0, uint64_t *q1, const uint32_t *w) {
    uint64_t x0, x1, x2, x3;

    x0 = w[0];
    x1 = w[1];
    x2 = w[2];
    x3 = w[3];
    x0 |= (x0 << 16);
    x1 |= (x1 << 16);
    x2 |= (x2 << 16);
    x3 |= (x3 << 16);
    x0 &= (uint64_t)0x0000FFFF0000FFFF;
    x1 &= (uint64_t)0x0000FFFF0000FFFF;
    x2 &= (uint64_t)0x0000FFFF0000FFFF;
    x3 &= (uint64_t)0x0000FFFF0000FFFF;
    x0 |= (x0 << 8);
    x1 |= (x1 << 8);
    x2 |= (x2 << 8);
    x3 |= (x3 << 8);
    x0 &= (uint64_t)0x00FF00FF00FF00FF;
    x1 &= (uint64_t)0x00FF00FF00FF00FF;
    x2 &= (uint64_t)0x00FF00FF00FF00FF;
    x3 &= (uint64_t)0x00FF00FF00FF00FF;
    *q0 = x0 | (x2 << 8);
    *q1 = x1 | (x3 << 8);
}


static void br_aes_ct64_interleave_out(uint32_t *w, uint64_t q0, uint64_t q1) {
    uint64_t x0, x1, x2, x3;

    x0 = q0 & (uint64_t)0x00FF00FF00FF00FF;
    x1 = q1 & (uint64_t)0x00FF00FF00FF00FF;
    x2 = (q0 >> 8) & (uint64_t)0x00FF00FF00FF00FF;
    x3 = (q1 >> 8) & (uint64_t)0x00FF00FF00FF00FF;
    x0 |= (x0 >> 8);
    x1 |= (x1 >> 8);
    x2 |= (x2 >> 8);
    x3 |= (x3 >> 8);
    x0 &= (uint64_t)0x0000FFFF0000FFFF;
    x1 &= (uint64_t)0x0000FFFF0000FFFF;
    x2 &= (uint64_t)0x0000FFFF0000FFFF;
    x3 &= (uint64_t)0x0000FFFF0000FFFF;
    w[0] = (uint32_t)x0 | (uint32_t)(x0 >> 16);
    w[1] = (uint32_t)x1 | (uint32_t)(x1 >> 16);
    w[2] = (uint32_t)x2 | (uint32_t)(x2 >> 16);
    w[3] = (uint32_t)x3 | (uint32_t)(x3 >> 16);
}

static inline void add_round_key(uint64_t *q, const uint64_t *sk) {
    q[0] ^= sk[0];
    q[1] ^= sk[1];
    q[2] ^= sk[2];
    q[3] ^= sk[3];
    q[4] ^= sk[4];
    q[5] ^= sk[5];
    q[6] ^= sk[6];
    q[7] ^= sk[7];
}

static inline void shift_rows(uint64_t *q) {
    int i;

    for (i = 0; i < 8; i++) {
        uint64_t x;

        x = q[i];
        q[i] = (x & (uint64_t)0x000000000000FFFF)
               | ((x & (uint64_t)0x00000000FFF00000) >> 4)
               | ((x & (uint64_t)0x00000000000F0000) << 12)
               | ((x & (uint64_t)0x0000FF0000000000) >> 8)
               | ((x & (uint64_t)0x000000FF00000000) << 8)
               | ((x & (uint64_t)0xF000000000000000) >> 12)
               | ((x & (uint64_t)0x0FFF000000000000) << 4);
    }
}

static inline uint64_t rotr32(uint64_t x) {
    return (x << 32) | (x >> 32);
}

static inline void mix_columns(uint64_t *q) {
    uint64_t q0, q1, q2, q3, q4, q5, q6, q7;
    uint64_t r0, r1, r2, r3, r4, r5, r6, r7;

    q0 = q[0];
    q1 = q[1];
    q2 = q[2];
    q3 = q[3];
    q4 = q[4];
    q5 = q[5];
    q6 = q[6];
    q7 = q[7];
    r0 = (q0 >> 16) | (q0 << 48);
    r1 = (q1 >> 16) | (q1 << 48);
    r2 = (q2 >> 16) | (q2 << 48);
    r3 = (q3 >> 16) | (q3 << 48);
    r4 = (q4 >> 16) | (q4 << 48);
    r5 = (q5 >> 16) | (q5 << 48);
    r6 = (q6 >> 16) | (q6 << 48);
    r7 = (q7 >> 16) | (q7 << 48);

    q[0] = q7 ^ r7 ^ r0 ^ rotr32(q0 ^ r0);
    q[1] = q0 ^ r0 ^ q7 ^ r7 ^ r1 ^ rotr32(q1 ^ r1);
    q[2] = q1 ^ r1 ^ r2 ^ rotr32(q2 ^ r2);
    q[3] = q2 ^ r2 ^ q7 ^ r7 ^ r3 ^ rotr32(q3 ^ r3);
    q[4] = q3 ^ r3 ^ q7 ^ r7 ^ r4 ^ rotr32(q4 ^ r4);
    q[5] = q4 ^ r4 ^ r5 ^ rotr32(q5 ^ r5);
    q[6] = q5 ^ r5 ^ r6 ^ rotr32(q6 ^ r6);
    q[7] = q6 ^ r6 ^ r7 ^ rotr32(q7 ^ r7);
}

static void interleave_constant(uint64_t *out, const unsigned char *in) {
    uint32_t tmp_32_constant[16];
    int i;

    br_range_dec32le(tmp_32_constant, 16, in);
    for (i = 0; i < 4; i++) {
        br_aes_ct64_interleave_in(&out[i], &out[i + 4], tmp_32_constant + (i << 2));
    }
    br_aes_ct64_ortho(out);
}

static void interleave_constant32(uint32_t *out, const unsigned char *in) {
    int i;
    for (i = 0; i < 4; i++) {
        out[2 * i] = br_dec32le(in + 4 * i);
        out[2 * i + 1] = br_dec32le(in + 4 * i + 16);
    }
    br_aes_ct_ortho(out);
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_tweak_constants(
    harakactx *state,
    const unsigned char *pk_seed, const unsigned char *sk_seed,
    unsigned long long seed_length) {
    unsigned char buf[40 * 16];
    int i;

    /* Use the standard constants to generate tweaked ones. */
    memcpy((uint8_t *)state->tweaked512_rc64, (uint8_t *)haraka512_rc64, 40 * 16);

    /* Constants for sk.seed */
    if (sk_seed != NULL) {
        PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S(
            buf, 40 * 16, sk_seed, seed_length, state);

        /* Interleave constants */
        for (i = 0; i < 10; i++) {
            interleave_constant32(state->tweaked256_rc32_sseed[i], buf + 32 * i);
        }
    }

    /* Constants for pk.seed */
    PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S(
        buf, 40 * 16, pk_seed, seed_length, state);
    for (i = 0; i < 10; i++) {
        interleave_constant32(state->tweaked256_rc32[i], buf + 32 * i);
        interleave_constant(state->tweaked512_rc64[i], buf + 64 * i);
    }
}

static void haraka_S_absorb(unsigned char *s,
                            const unsigned char *m, unsigned long long mlen,
                            unsigned char p, const harakactx *state) {
    unsigned long long i;
    unsigned char t[HARAKAS_RATE];

    while (mlen >= HARAKAS_RATE) {
        /* XOR block to state */
        for (i = 0; i < HARAKAS_RATE; ++i) {
            s[i] ^= m[i];
        }
        PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(s, s, state);
        mlen -= HARAKAS_RATE;
        m += HARAKAS_RATE;
    }

    for (i = 0; i < HARAKAS_RATE; ++i) {
        t[i] = 0;
    }
    for (i = 0; i < mlen; ++i) {
        t[i] = m[i];
    }
    t[i] = p;
    t[HARAKAS_RATE - 1] |= 128;
    for (i = 0; i < HARAKAS_RATE; ++i) {
        s[i] ^= t[i];
    }
}

static void haraka_S_squeezeblocks(unsigned char *h, unsigned long long nblocks,
                                   unsigned char *s, const harakactx *state) {
    while (nblocks > 0) {
        PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(s, s, state);
        memcpy(h, s, HARAKAS_RATE);
        h += HARAKAS_RATE;
        nblocks--;
    }
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_init(uint8_t *s_inc) {
    size_t i;

    for (i = 0; i < 64; i++) {
        s_inc[i] = 0;
    }
    s_inc[64] = 0;
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_absorb(uint8_t *s_inc, const uint8_t *m, size_t mlen, const harakactx *state) {
    size_t i;

    /* Recall that s_inc[64] is the non-absorbed bytes xored into the state */
    while (mlen + s_inc[64] >= HARAKAS_RATE) {
        for (i = 0; i < (size_t)(HARAKAS_RATE - s_inc[64]); i++) {
            /* Take the i'th byte from message
               xor with the s_inc[64] + i'th byte of the state */
            s_inc[s_inc[64] + i] ^= m[i];
        }
        mlen -= (size_t)(HARAKAS_RATE - s_inc[64]);
        m += HARAKAS_RATE - s_inc[64];
        s_inc[64] = 0;

        PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(s_inc, s_inc, state);
    }

    for (i = 0; i < mlen; i++) {
        s_inc[s_inc[64] + i] ^= m[i];
    }
    s_inc[64] = (uint8_t)(mlen + s_inc[64]);
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_finalize(uint8_t *s_inc) {
    /* After haraka_S_inc_absorb, we are guaranteed that s_inc[64] < HARAKAS_RATE,
       so we can always use one more byte for p in the current state. */
    s_inc[s_inc[64]] ^= 0x1F;
    s_inc[HARAKAS_RATE - 1] ^= 128;
    s_inc[64] = 0;
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_squeeze(uint8_t *out, size_t outlen, uint8_t *s_inc, const harakactx *state) {
    uint8_t i;

    /* First consume any bytes we still have sitting around */
    for (i = 0; i < outlen && i < s_inc[64]; i++) {
        /* There are s_inc[64] bytes left, so r - s_inc[64] is the first
           available byte. We consume from there, i.e., up to r. */
        out[i] = s_inc[(HARAKAS_RATE - s_inc[64] + i)];
    }
    out += i;
    outlen -= i;
    s_inc[64] = (uint8_t)(s_inc[64] - i);

    /* Then squeeze the remaining necessary blocks */
    while (outlen > 0) {
        PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(s_inc, s_inc, state);

        for (i = 0; i < outlen && i < HARAKAS_RATE; i++) {
            out[i] = s_inc[i];
        }
        out += i;
        outlen -= i;
        s_inc[64] = (uint8_t)(HARAKAS_RATE - i);
    }
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S(unsigned char *out, unsigned long long outlen, const unsigned char *in, unsigned long long inlen, const harakactx *state) {
    unsigned long long i;
    unsigned char s[64];
    unsigned char d[32];

    for (i = 0; i < 64; i++) {
        s[i] = 0;
    }
    haraka_S_absorb(s, in, inlen, 0x1F, state);

    haraka_S_squeezeblocks(out, outlen / 32, s, state);
    out += (outlen / 32) * 32;

    if (outlen % 32) {
        haraka_S_squeezeblocks(d, 1, s, state);
        for (i = 0; i < outlen % 32; i++) {
            out[i] = d[i];
        }
    }
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(unsigned char *out, const unsigned char *in, const harakactx *state) {
    uint32_t w[16];
    uint64_t q[8], tmp_q;
    unsigned int i, j;

    br_range_dec32le(w, 16, in);
    for (i = 0; i < 4; i++) {
        br_aes_ct64_interleave_in(&q[i], &q[i + 4], w + (i << 2));
    }
    br_aes_ct64_ortho(q);

    /* AES rounds */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            br_aes_ct64_bitslice_Sbox(q);
            shift_rows(q);
            mix_columns(q);
            add_round_key(q, state->tweaked512_rc64[2 * i + j]);
        }
        /* Mix states */
        for (j = 0; j < 8; j++) {
            tmp_q = q[j];
            q[j] = (tmp_q & 0x0001000100010001) << 5 |
                   (tmp_q & 0x0002000200020002) << 12 |
                   (tmp_q & 0x0004000400040004) >> 1 |
                   (tmp_q & 0x0008000800080008) << 6 |
                   (tmp_q & 0x0020002000200020) << 9 |
                   (tmp_q & 0x0040004000400040) >> 4 |
                   (tmp_q & 0x0080008000800080) << 3 |
                   (tmp_q & 0x2100210021002100) >> 5 |
                   (tmp_q & 0x0210021002100210) << 2 |
                   (tmp_q & 0x0800080008000800) << 4 |
                   (tmp_q & 0x1000100010001000) >> 12 |
                   (tmp_q & 0x4000400040004000) >> 10 |
                   (tmp_q & 0x8400840084008400) >> 3;
        }
    }

    br_aes_ct64_ortho(q);
    for (i = 0; i < 4; i ++) {
        br_aes_ct64_interleave_out(w + (i << 2), q[i], q[i + 4]);
    }
    br_range_enc32le(out, w, 16);
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512(unsigned char *out, const unsigned char *in, const harakactx *state) {
    int i;

    unsigned char buf[64];

    PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(buf, in, state);
    /* Feed-forward */
    for (i = 0; i < 64; i++) {
        buf[i] = buf[i] ^ in[i];
    }

    /* Truncated */
    memcpy(out,      buf + 8, 8);
    memcpy(out + 8,  buf + 24, 8);
    memcpy(out + 16, buf + 32, 8);
    memcpy(out + 24, buf + 48, 8);
}


void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka256(unsigned char *out, const unsigned char *in, const harakactx *state) {
    uint32_t q[8], tmp_q;
    int i, j;

    for (i = 0; i < 4; i++) {
        q[2 * i] = br_dec32le(in + 4 * i);
        q[2 * i + 1] = br_dec32le(in + 4 * i + 16);
    }
    br_aes_ct_ortho(q);

    /* AES rounds */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            br_aes_ct_bitslice_Sbox(q);
            shift_rows32(q);
            mix_columns32(q);
            add_round_key32(q, state->tweaked256_rc32[2 * i + j]);
        }

        /* Mix states */
        for (j = 0; j < 8; j++) {
            tmp_q = q[j];
            q[j] = (tmp_q & 0x81818181) |
                   (tmp_q & 0x02020202) << 1 |
                   (tmp_q & 0x04040404) << 2 |
                   (tmp_q & 0x08080808) << 3 |
                   (tmp_q & 0x10101010) >> 3 |
                   (tmp_q & 0x20202020) >> 2 |
                   (tmp_q & 0x40404040) >> 1;
        }
    }

    br_aes_ct_ortho(q);
    for (i = 0; i < 4; i++) {
        br_enc32le(out + 4 * i, q[2 * i]);
        br_enc32le(out + 4 * i + 16, q[2 * i + 1]);
    }

    for (i = 0; i < 32; i++) {
        out[i] ^= in[i];
    }
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka256_sk(unsigned char *out, const unsigned char *in, const harakactx *state) {
    uint32_t q[8], tmp_q;
    int i, j;

    for (i = 0; i < 4; i++) {
        q[2 * i] = br_dec32le(in + 4 * i);
        q[2 * i + 1] = br_dec32le(in + 4 * i + 16);
    }
    br_aes_ct_ortho(q);

    /* AES rounds */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            br_aes_ct_bitslice_Sbox(q);
            shift_rows32(q);
            mix_columns32(q);
            add_round_key32(q, state->tweaked256_rc32_sseed[2 * i + j]);
        }

        /* Mix states */
        for (j = 0; j < 8; j++) {
            tmp_q = q[j];
            q[j] = (tmp_q & 0x81818181) |
                   (tmp_q & 0x02020202) << 1 |
                   (tmp_q & 0x04040404) << 2 |
                   (tmp_q & 0x08080808) << 3 |
                   (tmp_q & 0x10101010) >> 3 |
                   (tmp_q & 0x20202020) >> 2 |
                   (tmp_q & 0x40404040) >> 1;
        }
    }

    br_aes_ct_ortho(q);
    for (i = 0; i < 4; i++) {
        br_enc32le(out + 4 * i, q[2 * i]);
        br_enc32le(out + 4 * i + 16, q[2 * i + 1]);
    }

    for (i = 0; i < 32; i++) {
        out[i] ^= in[i];
    }
}
