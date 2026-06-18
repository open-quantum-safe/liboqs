#ifndef __RIJNDAEL_CT64_ENC_H__
#define __RIJNDAEL_CT64_ENC_H__

/************************************************************************/
/* XXX: taken and adapted from BearSSL by the MQOM team to also support
 * Rijndael-256 (see https://bearssl.org/constanttime.html for explanations
 * on constant-time bitsliced AES)
 */
/************************************************************************/

/*
 * Copyright (c) 2016 Thomas Pornin <pornin@bolet.org>
 *
 * XXX: Modified by the MQOM team to include multiple keys and Rijndael-256
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "rijndael_ct64.h"

/*
 * Detect support for unaligned accesses with known endianness.
 *
 *  x86 (both 32-bit and 64-bit) is little-endian and allows unaligned
 *  accesses.
 *
 *  POWER/PowerPC allows unaligned accesses when big-endian. POWER8 and
 *  later also allow unaligned accesses when little-endian.
 */
#if !defined BR_LE_UNALIGNED && !defined BR_BE_UNALIGNED

#if __i386 || __i386__ || __x86_64__ || _M_IX86 || _M_X64
#define BR_LE_UNALIGNED   1
#elif BR_POWER8_BE
#define BR_BE_UNALIGNED   1
#elif BR_POWER8_LE
#define BR_LE_UNALIGNED   1
#elif (__powerpc__ || __powerpc64__ || _M_PPC || _ARCH_PPC || _ARCH_PPC64) \
        && __BIG_ENDIAN__
#define BR_BE_UNALIGNED   1
#endif

#endif

typedef union {
	uint32_t u;
	unsigned char b[sizeof(uint32_t)];
} br_union_u32;

static inline uint32_t
br_dec32le(const void *src) {
#if BR_LE_UNALIGNED
	return ((const br_union_u32 *)src)->u;
#else
	const unsigned char *buf;

	buf = src;
	return (uint32_t)buf[0]
	       | ((uint32_t)buf[1] << 8)
	       | ((uint32_t)buf[2] << 16)
	       | ((uint32_t)buf[3] << 24);
#endif
}

static inline void
br_enc32le(void *dst, uint32_t x) {
#if BR_LE_UNALIGNED
	((br_union_u32 *)dst)->u = x;
#else
	unsigned char *buf;

	buf = dst;
	buf[0] = (unsigned char)x;
	buf[1] = (unsigned char)(x >> 8);
	buf[2] = (unsigned char)(x >> 16);
	buf[3] = (unsigned char)(x >> 24);
#endif
}

static inline void
br_range_dec32le(uint32_t *v, size_t num, const void *src) {
	const unsigned char *buf;

	buf = src;
	while (num -- > 0) {
		*v ++ = br_dec32le(buf);
		buf += 4;
	}
}



static inline void
br_range_enc32le(void *dst, const uint32_t *v, size_t num) {
	unsigned char *buf;

	buf = dst;
	while (num -- > 0) {
		br_enc32le(buf, *v ++);
		buf += 4;
	}
}

static inline void
br_aes_ct64_bitslice_Sbox(uint64_t *q) {
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

static inline void
br_aes_ct64_ortho(uint64_t *q) {
#define SWAPN(cl, ch, s, x, y)   do { \
        uint64_t a, b; \
        a = (x); \
        b = (y); \
        (x) = (a & (uint64_t)cl) | ((b & (uint64_t)cl) << (s)); \
        (y) = ((a & (uint64_t)ch) >> (s)) | (b & (uint64_t)ch); \
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

/* Rijndael dedicated interleaving of inputs and outputs */
static inline void
br_rijndael_ct64_interleave_in(uint32_t w_out[8], const uint32_t w[8]) {
	/* Interleaving consists in columns shuffling of the state */
	w_out[0] = w[0];
	w_out[1] = w[2];
	w_out[2] = w[4];
	w_out[3] = w[6];
	w_out[4] = w[1];
	w_out[5] = w[3];
	w_out[6] = w[5];
	w_out[7] = w[7];
}

static inline void
br_rijndael_ct64_interleave_out(uint32_t w_out[8], const uint32_t w[8]) {
	/* Interleaving consists in columns shuffling of the state */
	w_out[0] = w[0];
	w_out[1] = w[4];
	w_out[2] = w[1];
	w_out[3] = w[5];
	w_out[4] = w[2];
	w_out[5] = w[6];
	w_out[6] = w[3];
	w_out[7] = w[7];
}


static inline void
br_aes_ct64_interleave_in(uint64_t *q0, uint64_t *q1, const uint32_t *w) {
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

static inline void
br_aes_ct64_interleave_out(uint32_t *w, uint64_t q0, uint64_t q1) {
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

static const unsigned char Rcon[] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36, 0x6c, 0xd8, 0xab, 0x4d
};

static inline uint32_t
sub_word(uint32_t x) {
	uint64_t q[8] = { 0 };

	q[0] = x;
	br_aes_ct64_ortho(q);
	br_aes_ct64_bitslice_Sbox(q);
	br_aes_ct64_ortho(q);
	return (uint32_t)q[0];
}

/* Key schedule for one key */
#define BR_AES_CT64_KEYSCHED_CT(ctx, key, rtype_) do { \
    unsigned num_rounds;\
    int i, j, k, nb, nk, nkf;\
    uint32_t tmp;\
    size_t key_len;\
    uint32_t *skey = ctx->rk;\
\
        if((ctx == NULL) || (key == NULL)){\
                goto err;\
        }\
    /* NOTE: for AES we have 4 keys, for Rijndael-256 we have 2 keys */\
        switch(rtype_){\
                case AES128:{\
                        ctx->Nr = num_rounds = 10;\
                        ctx->Nk = nk = 4;\
                        ctx->Nb = nb = 4;\
            key_len = 16;\
                        break;\
                }\
                case AES256:{\
                        ctx->Nr = num_rounds = 14;\
                        ctx->Nk = nk = 8;\
                        ctx->Nb = nb = 4;\
            key_len = 32;\
                        break;\
                }\
                case RIJNDAEL_256_256:{\
                        ctx->Nr = num_rounds = 14;\
                        ctx->Nk = nk = 8;\
                        ctx->Nb = nb = 8;\
            key_len = 32;\
                        break;\
                }\
                default:{\
                        ret = -1;\
                        goto err;\
                }\
        }\
    ctx->rtype = rtype_;\
\
    /* Perform the non-bitsliced key schedule */\
    nk = (int)(key_len >> 2);\
    nkf = (int)((num_rounds + 1) * nb);\
    br_range_dec32le(skey, (key_len >> 2), key);\
    tmp = skey[(key_len >> 2) - 1];\
    for (i = nk, j = 0, k = 0; i < nkf; i ++) {\
        if (j == 0) {\
            tmp = (tmp << 24) | (tmp >> 8);\
            tmp = sub_word(tmp) ^ Rcon[k];\
        } else if (nk > 6 && j == 4) {\
            tmp = sub_word(tmp);\
        }\
        tmp ^= skey[i - nk];\
        skey[i] = tmp;\
        if (++ j == nk) {\
            j = 0;\
            k ++;\
        }\
    }\
} while(0);

/* Non-constant time sub word */
/* S-Box */
static const uint8_t sbox[256] = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

/* RCON constants table */
static const uint32_t rcon[14] = {
        0x00000001, 0x00000002, 0x00000004, 0x00000008,
        0x00000010, 0x00000020, 0x00000040, 0x00000080,
        0x0000001b, 0x00000036, 0x0000006c, 0x000000d8,
        0x000000ab, 0x0000004d
};

#ifndef GET_UINT32_LE
#define GET_UINT32_LE(n,b,i)                            \
do {                                                    \
    (n) = ( (uint32_t) (b)[(i)    ]       )             \
    | ( (uint32_t) (b)[(i) + 1] <<  8 )     \
    | ( (uint32_t) (b)[(i) + 2] << 16 )     \
    | ( (uint32_t) (b)[(i) + 3] << 24 );        \
} while( 0 )
#endif

#ifndef PUT_UINT32_LE
#define PUT_UINT32_LE(n,b,i)                                    \
do {                                                            \
    (b)[(i)    ] = (uint8_t) ( ( (n)       ) & 0xFF );    \
    (b)[(i) + 1] = (uint8_t) ( ( (n) >>  8 ) & 0xFF );    \
    (b)[(i) + 2] = (uint8_t) ( ( (n) >> 16 ) & 0xFF );    \
    (b)[(i) + 3] = (uint8_t) ( ( (n) >> 24 ) & 0xFF );    \
} while( 0 )
#endif

#define BR_AES_CT64_KEYSCHED_NON_CT(ctx, key, rtype_) do { \
    uint32_t i;\
    /* Get the round keys pointer as uint32_t (NOTE: buffer should be 4 bytes aligned) */\
    uint32_t *RK = (uint32_t*)(ctx->rk);\
\
    if((ctx == NULL) || (key == NULL)){\
        goto err;\
    }\
    switch(rtype_){\
        case AES128:{\
            ctx->Nr = 10;\
            ctx->Nk = 4;\
            ctx->Nb = 4;\
            break;\
        }\
        case AES256:{\
            ctx->Nr = 14;\
            ctx->Nk = 8;\
            ctx->Nb = 4;\
            break;\
        }\
        case RIJNDAEL_256_256:{\
            ctx->Nr = 14;\
            ctx->Nk = 8;\
            ctx->Nb = 8;\
            break;\
        }\
        default:{\
            ret = -1;\
            goto err;\
        }\
    }\
    ctx->rtype = rtype_;\
\
    /* Perform the key schedule */\
    for(i = 0; i < ctx->Nk; i++){\
        GET_UINT32_LE(RK[i], key, (4*i));\
    }\
\
    switch(ctx->Nr){\
        case 10:{\
            for(i = 0; i < 10; i++){\
                RK[4]  = RK[0] ^ rcon[i] ^\
                    ( (uint32_t) sbox[ ( RK[3] >>  8 ) & 0xFF ]       ) ^\
                    ( (uint32_t) sbox[ ( RK[3] >> 16 ) & 0xFF ] <<  8 ) ^\
                    ( (uint32_t) sbox[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^\
                    ( (uint32_t) sbox[ ( RK[3]       ) & 0xFF ] << 24 );\
                RK[5]  = RK[1] ^ RK[4];\
                RK[6]  = RK[2] ^ RK[5];\
                RK[7]  = RK[3] ^ RK[6];\
                RK += 4;\
            }\
            break;\
        }\
        case 12:{\
            for(i = 0; i < 8; i++){\
                RK[6]  = RK[0] ^ rcon[i] ^\
                    ( (uint32_t) sbox[ ( RK[5] >>  8 ) & 0xFF ]       ) ^\
                    ( (uint32_t) sbox[ ( RK[5] >> 16 ) & 0xFF ] <<  8 ) ^\
                    ( (uint32_t) sbox[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^\
                    ( (uint32_t) sbox[ ( RK[5]       ) & 0xFF ] << 24 );\
                RK[7]  = RK[1] ^ RK[6];\
                RK[8]  = RK[2] ^ RK[7];\
                RK[9]  = RK[3] ^ RK[8];\
                RK[10] = RK[4] ^ RK[9];\
                RK[11] = RK[5] ^ RK[10];\
                RK += 6;\
            }\
            break;\
        }\
        case 14:{\
            unsigned int rcon_offset = 0;\
            for(i = 0; i < 7; i++){\
                RK[8]  = RK[0] ^ rcon[rcon_offset++] ^\
                    ( (uint32_t) sbox[ ( RK[7] >>  8 ) & 0xFF ]       ) ^\
                    ( (uint32_t) sbox[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^\
                    ( (uint32_t) sbox[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^\
                    ( (uint32_t) sbox[ ( RK[7]       ) & 0xFF ] << 24 );\
                RK[9]  = RK[1] ^ RK[8];\
                RK[10] = RK[2] ^ RK[9];\
                RK[11] = RK[3] ^ RK[10];\
                if((i == 6) && (ctx->Nb != 8)){ \
                    break; \
                } \
                RK[12] = RK[4] ^\
                    ( (uint32_t) sbox[ ( RK[11]       ) & 0xFF ]       ) ^\
                    ( (uint32_t) sbox[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^\
                    ( (uint32_t) sbox[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^\
                    ( (uint32_t) sbox[ ( RK[11] >> 24 ) & 0xFF ] << 24 );\
                RK[13] = RK[5] ^ RK[12];\
                RK[14] = RK[6] ^ RK[13];\
                RK[15] = RK[7] ^ RK[14];\
                RK += 8;\
                if(ctx->Nb == 8){\
                    /* Rijndael with 256 block size case */\
                    RK[8]  = RK[0] ^ rcon[rcon_offset++] ^\
                        ( (uint32_t) sbox[ ( RK[7] >>  8 ) & 0xFF ]       ) ^\
                        ( (uint32_t) sbox[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^\
                        ( (uint32_t) sbox[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^\
                        ( (uint32_t) sbox[ ( RK[7]       ) & 0xFF ] << 24 );\
                    RK[9]  = RK[1] ^ RK[8];\
                    RK[10] = RK[2] ^ RK[9];\
                    RK[11] = RK[3] ^ RK[10];\
                    RK[12] = RK[4] ^\
                        ( (uint32_t) sbox[ ( RK[11]       ) & 0xFF ]       ) ^\
                        ( (uint32_t) sbox[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^\
                        ( (uint32_t) sbox[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^\
                        ( (uint32_t) sbox[ ( RK[11] >> 24 ) & 0xFF ] << 24 );\
                    RK[13] = RK[5] ^ RK[12];\
                    RK[14] = RK[6] ^ RK[13];\
                    RK[15] = RK[7] ^ RK[14];\
                    RK += 8;\
                }\
            }\
            break;\
        }\
        default:{\
            goto err;\
        }\
    }\
} while(0);

#define BR_AES_CT64_KEYSCHED(ctx, key, rtype_, ct) do { \
	if(ct){ \
		BR_AES_CT64_KEYSCHED_CT(ctx, key, rtype_); \
	} \
	else{ \
		BR_AES_CT64_KEYSCHED_NON_CT(ctx, key, rtype_); \
	} \
} while(0);

/* Bitslice key scheduled keys */
#define BITSLICE_KEYS(ctx1, ctx2, ctx3, ctx4, comp_skey) do { \
    unsigned int i, j;\
\
    /* Sanity checks: at least one context  */\
    if(ctx1 == NULL){\
        ret = -1;\
        goto err;\
    }\
    /* We must have at most 4 contexts for AES, at most 2 contexts for Rijndael */\
    if((ctx1->rtype == RIJNDAEL_256_256) && ((ctx3 != NULL) || (ctx4 != NULL))){\
        ret = -1;\
        goto err;\
    }\
    if(ctx2 && (ctx2->rtype != ctx1->rtype)){\
        ret = -1;\
        goto err;\
    }\
    if(ctx3 && (ctx3->rtype != ctx1->rtype)){\
        ret = -1;\
        goto err;\
    }\
    if(ctx4 && (ctx4->rtype != ctx1->rtype)){\
        ret = -1;\
        goto err;\
    }\
\
    /* Bitslice the keys */\
    j = 0;\
    for(i = 0; i < ((ctx1->Nr + 1) * ctx1->Nb); i += ctx1->Nb){\
        uint64_t *q_ = &comp_skey[j];\
        uint64_t *q0_ = &q_[0];\
        uint64_t *q1_ = &q_[1];\
        uint64_t *q2_ = &q_[2];\
        uint64_t *q3_ = &q_[3];\
        uint64_t *q4_ = &q_[4];\
        uint64_t *q5_ = &q_[5];\
        uint64_t *q6_ = &q_[6];\
        uint64_t *q7_ = &q_[7];\
        j += 8;\
\
        /* Interleave the keys */\
        if(ctx1->rtype == RIJNDAEL_256_256){\
            uint32_t w_[16] = { 0 };\
            uint32_t *w0_ = &w_[0];\
            uint32_t *w1_ = &w_[4];\
            uint32_t *w2_ = &w_[8];\
            uint32_t *w3_ = &w_[12];\
\
            br_rijndael_ct64_interleave_in(w0_, &ctx1->rk[i]);\
            br_aes_ct64_interleave_in(q0_, q4_, w0_);\
                    br_aes_ct64_interleave_in(q2_, q6_, w1_);\
            if(ctx2 != NULL){\
                br_rijndael_ct64_interleave_in(w2_, &ctx2->rk[i]);\
                        br_aes_ct64_interleave_in(q1_, q5_, w2_);\
                        br_aes_ct64_interleave_in(q3_, q7_, w3_);\
            }\
        }\
        else{\
            br_aes_ct64_interleave_in(q0_, q4_, &ctx1->rk[i]);\
            if(ctx2 != NULL){\
                        br_aes_ct64_interleave_in(q1_, q5_, &ctx2->rk[i]);\
            }\
            if(ctx3 != NULL){\
                        br_aes_ct64_interleave_in(q2_, q6_, &ctx3->rk[i]);\
            }\
            if(ctx4 != NULL){\
                        br_aes_ct64_interleave_in(q3_, q7_, &ctx4->rk[i]);\
            }\
        }\
        /* Transpose */\
        br_aes_ct64_ortho(q_);\
    }\
} while(0);


static inline void
add_round_key(uint64_t *q, const uint64_t *sk) {
	q[0] ^= sk[0];
	q[1] ^= sk[1];
	q[2] ^= sk[2];
	q[3] ^= sk[3];
	q[4] ^= sk[4];
	q[5] ^= sk[5];
	q[6] ^= sk[6];
	q[7] ^= sk[7];
}

/* XXX: NOTE: shift rows is not the same for AES and Rijndael */
static inline void
shift_rows(uint64_t *q, bool is_rijndael_256) {
	int i;

	for (i = 0; i < 8; i ++) {
		uint64_t x;
		x = q[i];
		if (!is_rijndael_256) {
			/* For AES, shifts are 0, 1, 2, 3 per row respectively */
			q[i] =     (x & (uint64_t)0x000000000000FFFF)        /* Keep  first row untouched  */
			           | ((x & (uint64_t)0x00000000FFF00000) >> 4)  /* Rotate the second row by 1 */
			           | ((x & (uint64_t)0x00000000000F0000) << 12) /* Rotate the second row by 1 */
			           | ((x & (uint64_t)0x0000FF0000000000) >> 8)  /* Rotate the third row by 2  */
			           | ((x & (uint64_t)0x000000FF00000000) << 8)  /* Rotate the third row by 2  */
			           | ((x & (uint64_t)0xF000000000000000) >> 12) /* Rotate the fourth row by 3 */
			           | ((x & (uint64_t)0x0FFF000000000000) << 4); /* Rotate the fourth row by 3 */
		} else {
			/* For Rijndael-256, shifts are 0, 1, 3, 4 per row respectively */
			q[i] =     (x & (uint64_t)0x000000000000FFFF)         /* Keep  first row untouched  */
			           | ((x & (uint64_t)0x00000000FFFC0000) >> 2)   /* Rotate the second row by 1 */
			           | ((x & (uint64_t)0x0000000000030000) << 14)  /* Rotate the second row by 1 */
			           | ((x & (uint64_t)0x0000FFC000000000) >> 6)   /* Rotate the third row by 3  */
			           | ((x & (uint64_t)0x0000003F00000000) << 10)  /* Rotate the third row by 3  */
			           | ((x & (uint64_t)0xFF00000000000000) >> 8)   /* Rotate the fourth row by 4 */
			           | ((x & (uint64_t)0x00FF000000000000) << 8);  /* Rotate the fourth row by 4 */
		}
	}
}

static inline uint64_t
rotr32(uint64_t x) {
	return (x << 32) | (x >> 32);
}

static inline void
mix_columns(uint64_t *q) {
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

static inline void
br_ct64_bitslice_encrypt(const uint64_t *skey, rijndael_type rtype, unsigned int num_rounds, uint64_t *q) {
	unsigned int u;

	add_round_key(q, skey);
	for (u = 1; u < num_rounds; u ++) {
		br_aes_ct64_bitslice_Sbox(q);
		shift_rows(q, (rtype == RIJNDAEL_256_256) ? true : false);
		mix_columns(q);
		add_round_key(q, skey + (u << 3));
	}
	br_aes_ct64_bitslice_Sbox(q);
	shift_rows(q, (rtype == RIJNDAEL_256_256) ? true : false);
	add_round_key(q, skey + (num_rounds << 3));
}

/* Generic bitslice encryption */
#define CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4) do { \
    /* Maximum size for all the instances */\
    uint64_t comp_skey[120];\
    /* The bitsliced states */\
    uint64_t q[8] = { 0 };\
    rijndael_type rtype_;\
    uint32_t w[16] = { 0 };\
    uint32_t *w0 = &w[0];\
    uint32_t *w1 = &w[4];\
    uint32_t *w2 = &w[8];\
    uint32_t *w3 = &w[12];\
    uint64_t *q0 = &q[0];\
    uint64_t *q1 = &q[1];\
    uint64_t *q2 = &q[2];\
    uint64_t *q3 = &q[3];\
    uint64_t *q4 = &q[4];\
    uint64_t *q5 = &q[5];\
    uint64_t *q6 = &q[6];\
    uint64_t *q7 = &q[7];\
\
    /* Sanity checks */\
    if(ctx1 == NULL){\
        ret = -1;\
        goto err;\
    }\
    rtype_ = ctx1->rtype;\
    if(ctx2 && (ctx2->rtype != rtype_)){\
        ret = -1;\
        goto err;\
    }\
    if(ctx3 && (ctx3->rtype != rtype_)){\
        ret = -1;\
        goto err;\
    }\
    if(ctx4 && (ctx4->rtype != rtype_)){\
        ret = -1;\
        goto err;\
    }\
    if((rtype_ == RIJNDAEL_256_256) && ((ctx3 != NULL) || (ctx4 != NULL))){\
        ret = -1;\
        goto err;\
    }\
    if((rtype_ == RIJNDAEL_256_256) && ((plainText3 != NULL) || (plainText4 != NULL))){\
        ret = -1;\
        goto err;\
    }\
\
    /* The internal bitsliced round keys */\
    BITSLICE_KEYS(ctx1, ctx2, ctx3, ctx4, comp_skey);\
\
    /* Interleave the plaintexts */\
    if(rtype_ == RIJNDAEL_256_256){\
        uint32_t w_tmp[8];\
        if(plainText1 != NULL){\
            br_range_dec32le(w_tmp, 8, plainText1);\
            br_rijndael_ct64_interleave_in(w0, w_tmp);\
            br_aes_ct64_interleave_in(q0, q4, w0);\
            br_aes_ct64_interleave_in(q2, q6, w1);\
        }\
        if(plainText2 != NULL){\
            br_range_dec32le(w_tmp, 8, plainText2);\
            br_rijndael_ct64_interleave_in(w2, w_tmp);\
            br_aes_ct64_interleave_in(q1, q5, w2);\
            br_aes_ct64_interleave_in(q3, q7, w3);\
        }\
    }\
    else{\
        if(plainText1 != NULL){\
            br_range_dec32le(w0, 4, plainText1);\
            br_aes_ct64_interleave_in(q0, q4, w0);\
        }\
        if(plainText2 != NULL){\
            br_range_dec32le(w1, 4, plainText2);\
            br_aes_ct64_interleave_in(q1, q5, w1);\
        }\
        if(plainText3 != NULL){\
            br_range_dec32le(w2, 4, plainText3);\
            br_aes_ct64_interleave_in(q2, q6, w2);\
        }\
        if(plainText4 != NULL){\
            br_range_dec32le(w3, 4, plainText4);\
            br_aes_ct64_interleave_in(q3, q7, w3);\
        }\
    }\
\
    /* Transpose in */\
    br_aes_ct64_ortho(q);\
    /* Call the core encryption */\
    br_ct64_bitslice_encrypt(comp_skey, rtype_, ctx1->Nr, q);\
    /* Transpose out */\
    br_aes_ct64_ortho(q);\
\
    /* Now extract the output */\
    if(rtype_ == RIJNDAEL_256_256){\
        uint32_t w_tmp[8];\
        if(cipherText1 != NULL){\
            br_aes_ct64_interleave_out(w0, *q0, *q4);\
            br_aes_ct64_interleave_out(w1, *q2, *q6);\
            br_rijndael_ct64_interleave_out(w_tmp, w0);\
            br_range_enc32le(cipherText1, w_tmp, 8);\
        }\
        if(cipherText2 != NULL){\
            br_aes_ct64_interleave_out(w2, *q1, *q5);\
            br_aes_ct64_interleave_out(w3, *q3, *q7);\
            br_rijndael_ct64_interleave_out(w_tmp, w2);\
            br_range_enc32le(cipherText2, w_tmp, 8);\
        }\
    }\
    else{\
        if(cipherText1 != NULL){\
            br_aes_ct64_interleave_out(w0, *q0, *q4);\
            br_range_enc32le(cipherText1, w0, 4);\
        }\
        if(cipherText2 != NULL){\
            br_aes_ct64_interleave_out(w1, *q1, *q5);\
            br_range_enc32le(cipherText2, w1, 4);\
        }\
        if(cipherText3 != NULL){\
            br_aes_ct64_interleave_out(w2, *q2, *q6);\
            br_range_enc32le(cipherText3, w2, 4);\
        }\
        if(cipherText4 != NULL){\
            br_aes_ct64_interleave_out(w3, *q3, *q7);\
            br_range_enc32le(cipherText4, w3, 4);\
        }\
    }\
} while(0);

#define CORE_CT64_BITSLICE_ENCRYPT_ALREADY_BITSLICED(ctx, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4) do { \
    const uint64_t *comp_skey; \
    /* The bitsliced states */\
    uint64_t q[8] = { 0 };\
    rijndael_type rtype_;\
    uint32_t w[16] = { 0 };\
    uint32_t *w0 = &w[0];\
    uint32_t *w1 = &w[4];\
    uint32_t *w2 = &w[8];\
    uint32_t *w3 = &w[12];\
    uint64_t *q0 = &q[0];\
    uint64_t *q1 = &q[1];\
    uint64_t *q2 = &q[2];\
    uint64_t *q3 = &q[3];\
    uint64_t *q4 = &q[4];\
    uint64_t *q5 = &q[5];\
    uint64_t *q6 = &q[6];\
    uint64_t *q7 = &q[7];\
\
    /* Sanity checks */\
    if(ctx == NULL){\
        ret = -1;\
        goto err;\
    }\
    rtype_ = ctx->rtype;\
    comp_skey = ctx->interleaved_rkeys; \
    /* Interleave the plaintexts */\
    if(rtype_ == RIJNDAEL_256_256){\
        uint32_t w_tmp[8];\
        if(plainText1 != NULL){\
            br_range_dec32le(w_tmp, 8, plainText1);\
            br_rijndael_ct64_interleave_in(w0, w_tmp);\
            br_aes_ct64_interleave_in(q0, q4, w0);\
            br_aes_ct64_interleave_in(q2, q6, w1);\
        }\
        if(plainText2 != NULL){\
            br_range_dec32le(w_tmp, 8, plainText2);\
            br_rijndael_ct64_interleave_in(w2, w_tmp);\
            br_aes_ct64_interleave_in(q1, q5, w2);\
            br_aes_ct64_interleave_in(q3, q7, w3);\
        }\
    }\
    else{\
        if(plainText1 != NULL){\
            br_range_dec32le(w0, 4, plainText1);\
            br_aes_ct64_interleave_in(q0, q4, w0);\
        }\
        if(plainText2 != NULL){\
            br_range_dec32le(w1, 4, plainText2);\
            br_aes_ct64_interleave_in(q1, q5, w1);\
        }\
        if(plainText3 != NULL){\
            br_range_dec32le(w2, 4, plainText3);\
            br_aes_ct64_interleave_in(q2, q6, w2);\
        }\
        if(plainText4 != NULL){\
            br_range_dec32le(w3, 4, plainText4);\
            br_aes_ct64_interleave_in(q3, q7, w3);\
        }\
    }\
\
    /* Transpose in */\
    br_aes_ct64_ortho(q);\
    /* Call the core encryption */\
    br_ct64_bitslice_encrypt(comp_skey, rtype_, ctx->Nr, q);\
    /* Transpose out */\
    br_aes_ct64_ortho(q);\
\
    /* Now extract the output */\
    if(rtype_ == RIJNDAEL_256_256){\
        uint32_t w_tmp[8];\
        if(cipherText1 != NULL){\
            br_aes_ct64_interleave_out(w0, *q0, *q4);\
            br_aes_ct64_interleave_out(w1, *q2, *q6);\
            br_rijndael_ct64_interleave_out(w_tmp, w0);\
            br_range_enc32le(cipherText1, w_tmp, 8);\
        }\
        if(cipherText2 != NULL){\
            br_aes_ct64_interleave_out(w2, *q1, *q5);\
            br_aes_ct64_interleave_out(w3, *q3, *q7);\
            br_rijndael_ct64_interleave_out(w_tmp, w2);\
            br_range_enc32le(cipherText2, w_tmp, 8);\
        }\
    }\
    else{\
        if(cipherText1 != NULL){\
            br_aes_ct64_interleave_out(w0, *q0, *q4);\
            br_range_enc32le(cipherText1, w0, 4);\
        }\
        if(cipherText2 != NULL){\
            br_aes_ct64_interleave_out(w1, *q1, *q5);\
            br_range_enc32le(cipherText2, w1, 4);\
        }\
        if(cipherText3 != NULL){\
            br_aes_ct64_interleave_out(w2, *q2, *q6);\
            br_range_enc32le(cipherText3, w2, 4);\
        }\
        if(cipherText4 != NULL){\
            br_aes_ct64_interleave_out(w3, *q3, *q7);\
            br_range_enc32le(cipherText4, w3, 4);\
        }\
    }\
} while(0);

#endif /* __RIJNDAEL_CT64_ENC_H__ */
