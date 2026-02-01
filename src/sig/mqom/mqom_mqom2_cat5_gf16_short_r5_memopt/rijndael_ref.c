#include "rijndael_platform.h"

#if defined(RIJNDAEL_CONSTANT_TIME_REF)
#include "rijndael_ref.h"

/* Non-tabulated and constant time Rijndael: slow but constant time (using circuit based S-Box) */

static const uint8_t rcon[256] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

/* Rijndael primitives *******************************************************/
/************************************************************************/
/* Multiplication and squaring over the Rijndael Galois field */
#define RIJNDAEL_MODULUS 0x1B /* The Rijndael field GF(2^8) modulus */
static inline uint8_t gmul(uint8_t x, uint8_t y) {
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t res;
	volatile uint8_t mask = 1;

	res = (-(y >> 7 & mask) & x);
	res = (-(y >> 6 & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);
	res = (-(y >> 5 & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);
	res = (-(y >> 4 & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);
	res = (-(y >> 3 & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);
	res = (-(y >> 2 & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);
	res = (-(y >> 1 & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);
	res = (-(y      & mask) & x) ^ (-(res >> 7) & RIJNDAEL_MODULUS) ^ (res << 1);

	return res;
}
static inline uint8_t gsquare(uint8_t x) {
	return gmul(x, x);
}

/* Sbox computed as a circuit for constant time */
#define SBOX_BIT_EXTRACT(s, a, b, c, d, e) (((s >> a) & 1) ^ ((s >> b) & 1) ^ ((s >> c) & 1) ^ ((s >> d) & 1) ^ ((s >> e) & 1))
static inline uint8_t sbox(uint8_t s) {
	uint8_t out;
	/* First, compute the inverse of s as s^254  */
	uint8_t s2   = gsquare(s);     /* s^2 */
	uint8_t s3   = gmul(s, s2);    /* s^3 = s * s^2*/
	uint8_t s5   = gmul(s3, s2);   /* s^5 = s^3 * s^2 */
	uint8_t s7   = gmul(s5, s2);   /* s^7 = s^5 * s^2 */
	uint8_t s14  = gsquare(s7);    /* s^14 = (s^7)^2 */
	uint8_t s28  = gsquare(s14);   /* s^28 = (s^14)^2 */
	uint8_t s56  = gsquare(s28);   /* s^56 = (s^28)^2 */
	uint8_t s63  = gmul(s56, s7);  /* s^63 = s^56 * s^7 */
	uint8_t s126 = gsquare(s63);   /* s^126 = (s^63)^2 */
	uint8_t s252 = gsquare(s126);  /* s^252 = (s^126)^2 */
	uint8_t sinv = gmul(s252, s2); /* This is s^254 = s^-1 in GF(2^8) */
	/* Secondly, compute the affine part of the SBox: A * sinv */
	uint8_t out0 = SBOX_BIT_EXTRACT(sinv, 0, 4, 5, 6, 7);
	uint8_t out1 = SBOX_BIT_EXTRACT(sinv, 0, 1, 5, 6, 7);
	uint8_t out2 = SBOX_BIT_EXTRACT(sinv, 0, 1, 2, 6, 7);
	uint8_t out3 = SBOX_BIT_EXTRACT(sinv, 0, 1, 2, 3, 7);
	uint8_t out4 = SBOX_BIT_EXTRACT(sinv, 0, 1, 2, 3, 4);
	uint8_t out5 = SBOX_BIT_EXTRACT(sinv, 1, 2, 3, 4, 5);
	uint8_t out6 = SBOX_BIT_EXTRACT(sinv, 2, 3, 4, 5, 6);
	uint8_t out7 = SBOX_BIT_EXTRACT(sinv, 3, 4, 5, 6, 7);
	/* Put all the bits in out and add the constant part */
	out = (out0 | (out1 << 1) | (out2 << 2) | (out3 << 3) | (out4 << 4) | (out5 << 5) | (out6 << 6) | (out7 << 7)) ^ 0x63;
	/* Return the value */
	return out;
}

#define SUB_BYTES_SR(ctx, state) do { \
    uint8_t s[32];\
    memcpy(s, state, 4 * ctx->Nb);\
\
    if(ctx->Nb == 4){\
        /* Subbytes then Shiftrows */\
        state[0]  = sbox(s[0]);\
        state[1]  = sbox(s[5]);\
        state[2]  = sbox(s[10]);\
        state[3]  = sbox(s[15]);\
        /**/\
        state[4]  = sbox(s[4]);\
        state[5]  = sbox(s[9]);\
        state[6]  = sbox(s[14]);\
        state[7]  = sbox(s[3]);\
        /**/\
        state[8]  = sbox(s[8]);\
        state[9]  = sbox(s[13]);\
        state[10] = sbox(s[2]);\
        state[11] = sbox(s[7]);\
        /**/\
        state[12] = sbox(s[12]);\
        state[13] = sbox(s[1]);\
        state[14] = sbox(s[6]);\
        state[15] = sbox(s[11]);\
    }\
    else{\
        /* Subbytes then Shiftrows */\
        state[0]  = sbox(s[0]);\
        state[1]  = sbox(s[5]);\
        state[2]  = sbox(s[14]);\
        state[3]  = sbox(s[19]);\
        /**/\
        state[4]  = sbox(s[4]);\
        state[5]  = sbox(s[9]);\
        state[6]  = sbox(s[18]);\
        state[7]  = sbox(s[23]);\
        /**/\
        state[8]  = sbox(s[8]);\
        state[9]  = sbox(s[13]);\
        state[10] = sbox(s[22]);\
        state[11] = sbox(s[27]);\
        /**/\
        state[12] = sbox(s[12]);\
        state[13] = sbox(s[17]);\
        state[14] = sbox(s[26]);\
        state[15] = sbox(s[31]);\
        /**/\
        state[16 + 0]  = sbox(s[16]);\
        state[16 + 1]  = sbox(s[21]);\
        state[16 + 2]  = sbox(s[30]);\
        state[16 + 3]  = sbox(s[3]);\
        /**/\
        state[16 + 4]  = sbox(s[20]);\
        state[16 + 5]  = sbox(s[25]);\
        state[16 + 6]  = sbox(s[2]);\
        state[16 + 7]  = sbox(s[7]);\
        /**/\
        state[16 + 8]  = sbox(s[24]);\
        state[16 + 9]  = sbox(s[29]);\
        state[16 + 10] = sbox(s[6]);\
        state[16 + 11] = sbox(s[11]);\
        /**/\
        state[16 + 12] = sbox(s[28]);\
        state[16 + 13] = sbox(s[1]);\
        state[16 + 14] = sbox(s[10]);\
        state[16 + 15] = sbox(s[15]);\
    }\
} while(0);

/* Optimized gmul for MixColumns constants: thanks to the mixcolumns constants,
 * we can simplify gmul
 */
#define xtime(x) ((uint8_t)(((x)<<1) ^ ((((x)>>7) & 1) * 0x1b)))
static inline uint8_t gmul_mc(uint8_t x, uint8_t y) {
	return ((uint8_t)(((y & 1) * x) ^ (((y >> 1) & 1) * xtime(x)) ^ (((y >> 2) & 1) * xtime(xtime(x))) ^ (((y >> 3) & 1) * xtime(xtime(xtime(x)))) ^ (((y >> 4) & 1) * xtime(xtime(xtime(xtime(x))))))) & 0xff;
}

#define MIX_COLUMNS(ctx, state) do { \
    uint8_t s[32];\
    memcpy(s, state, 4 * ctx->Nb);\
\
    state[0]  = gmul_mc(s[0], 2) ^ gmul_mc(s[3], 1) ^ gmul_mc(s[2], 1) ^ gmul_mc(s[1], 3);\
    state[1]  = gmul_mc(s[1], 2) ^ gmul_mc(s[0], 1) ^ gmul_mc(s[3], 1) ^ gmul_mc(s[2], 3);\
    state[2]  = gmul_mc(s[2], 2) ^ gmul_mc(s[1], 1) ^ gmul_mc(s[0], 1) ^ gmul_mc(s[3], 3);\
    state[3]  = gmul_mc(s[3], 2) ^ gmul_mc(s[2], 1) ^ gmul_mc(s[1], 1) ^ gmul_mc(s[0], 3);\
    /**/\
    state[4]  = gmul_mc(s[4], 2) ^ gmul_mc(s[7], 1) ^ gmul_mc(s[6], 1) ^ gmul_mc(s[5], 3);\
    state[5]  = gmul_mc(s[5], 2) ^ gmul_mc(s[4], 1) ^ gmul_mc(s[7], 1) ^ gmul_mc(s[6], 3);\
    state[6]  = gmul_mc(s[6], 2) ^ gmul_mc(s[5], 1) ^ gmul_mc(s[4], 1) ^ gmul_mc(s[7], 3);\
    state[7]  = gmul_mc(s[7], 2) ^ gmul_mc(s[6], 1) ^ gmul_mc(s[5], 1) ^ gmul_mc(s[4], 3);\
    /**/\
    state[8]  = gmul_mc(s[8], 2) ^ gmul_mc(s[11], 1) ^ gmul_mc(s[10], 1) ^ gmul_mc(s[9], 3);\
    state[9]  = gmul_mc(s[9], 2) ^ gmul_mc(s[8], 1) ^ gmul_mc(s[11], 1) ^ gmul_mc(s[10], 3);\
    state[10] = gmul_mc(s[10], 2) ^ gmul_mc(s[9], 1) ^ gmul_mc(s[8], 1) ^ gmul_mc(s[11], 3);\
    state[11] = gmul_mc(s[11], 2) ^ gmul_mc(s[10], 1) ^ gmul_mc(s[9], 1) ^ gmul_mc(s[8], 3);\
    /**/\
    state[12] = gmul_mc(s[12], 2) ^ gmul_mc(s[15], 1) ^ gmul_mc(s[14], 1) ^ gmul_mc(s[13], 3);\
    state[13] = gmul_mc(s[13], 2) ^ gmul_mc(s[12], 1) ^ gmul_mc(s[15], 1) ^ gmul_mc(s[14], 3);\
    state[14] = gmul_mc(s[14], 2) ^ gmul_mc(s[13], 1) ^ gmul_mc(s[12], 1) ^ gmul_mc(s[15], 3);\
    state[15] = gmul_mc(s[15], 2) ^ gmul_mc(s[14], 1) ^ gmul_mc(s[13], 1) ^ gmul_mc(s[12], 3);\
    /**/\
    if(ctx->Nb == 8){\
        state[16 + 0]  = gmul_mc(s[16 + 0], 2) ^ gmul_mc(s[16 + 3], 1) ^ gmul_mc(s[16 + 2], 1) ^ gmul_mc(s[16 + 1], 3);\
        state[16 + 1]  = gmul_mc(s[16 + 1], 2) ^ gmul_mc(s[16 + 0], 1) ^ gmul_mc(s[16 + 3], 1) ^ gmul_mc(s[16 + 2], 3);\
        state[16 + 2]  = gmul_mc(s[16 + 2], 2) ^ gmul_mc(s[16 + 1], 1) ^ gmul_mc(s[16 + 0], 1) ^ gmul_mc(s[16 + 3], 3);\
        state[16 + 3]  = gmul_mc(s[16 + 3], 2) ^ gmul_mc(s[16 + 2], 1) ^ gmul_mc(s[16 + 1], 1) ^ gmul_mc(s[16 + 0], 3);\
        /**/\
        state[16 + 4]  = gmul_mc(s[16 + 4], 2) ^ gmul_mc(s[16 + 7], 1) ^ gmul_mc(s[16 + 6], 1) ^ gmul_mc(s[16 + 5], 3);\
        state[16 + 5]  = gmul_mc(s[16 + 5], 2) ^ gmul_mc(s[16 + 4], 1) ^ gmul_mc(s[16 + 7], 1) ^ gmul_mc(s[16 + 6], 3);\
        state[16 + 6]  = gmul_mc(s[16 + 6], 2) ^ gmul_mc(s[16 + 5], 1) ^ gmul_mc(s[16 + 4], 1) ^ gmul_mc(s[16 + 7], 3);\
        state[16 + 7]  = gmul_mc(s[16 + 7], 2) ^ gmul_mc(s[16 + 6], 1) ^ gmul_mc(s[16 + 5], 1) ^ gmul_mc(s[16 + 4], 3);\
        /**/\
        state[16 + 8]  = gmul_mc(s[16 + 8], 2) ^ gmul_mc(s[16 + 11], 1) ^ gmul_mc(s[16 + 10], 1) ^ gmul_mc(s[16 + 9], 3);\
        state[16 + 9]  = gmul_mc(s[16 + 9], 2) ^ gmul_mc(s[16 + 8], 1) ^ gmul_mc(s[16 + 11], 1) ^ gmul_mc(s[16 + 10], 3);\
        state[16 + 10] = gmul_mc(s[16 + 10], 2) ^ gmul_mc(s[16 + 9], 1) ^ gmul_mc(s[16 + 8], 1) ^ gmul_mc(s[16 + 11], 3);\
        state[16 + 11] = gmul_mc(s[16 + 11], 2) ^ gmul_mc(s[16 + 10], 1) ^ gmul_mc(s[16 + 9], 1) ^ gmul_mc(s[16 + 8], 3);\
        /**/\
        state[16 + 12] = gmul_mc(s[16 + 12], 2) ^ gmul_mc(s[16 + 15], 1) ^ gmul_mc(s[16 + 14], 1) ^ gmul_mc(s[16 + 13], 3);\
        state[16 + 13] = gmul_mc(s[16 + 13], 2) ^ gmul_mc(s[16 + 12], 1) ^ gmul_mc(s[16 + 15], 1) ^ gmul_mc(s[16 + 14], 3);\
        state[16 + 14] = gmul_mc(s[16 + 14], 2) ^ gmul_mc(s[16 + 13], 1) ^ gmul_mc(s[16 + 12], 1) ^ gmul_mc(s[16 + 15], 3);\
        state[16 + 15] = gmul_mc(s[16 + 15], 2) ^ gmul_mc(s[16 + 14], 1) ^ gmul_mc(s[16 + 13], 1) ^ gmul_mc(s[16 + 12], 3);\
    }\
} while(0);

#define ADD_RKEY(ctx, state, rkey) do { \
    uint32_t i; \
    for(i = 0; i < (4 * ctx->Nb); i++){ \
        state[i] ^= (rkey)[i]; \
    } \
} while(0);

static inline void sched(uint8_t *in, uint8_t n) {
	/* Rotate word, apply sbox and rcon */
	uint8_t t = in[0];
	in[0] = sbox(in[1]) ^ rcon[n];
	in[1] = sbox(in[2]);
	in[2] = sbox(in[3]);
	in[3] = sbox(t);

	return;
}

/* Encryption key schedule */
#define RIJNDAEL_SETKEY_ENC(ctx, key, rtype_) do {\
    uint32_t i, offset;\
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
    memcpy(&ctx->rk, key, 4 * ctx->Nk);\
    for(i = ctx->Nk; i < ctx->Nb * (ctx->Nr + 1); i++){\
        uint8_t t[4];\
        offset = 4 * (i - 1);\
        memcpy(t, &ctx->rk[offset], 4);\
        if(i % ctx->Nk == 0){\
            sched(t, i / ctx->Nk);\
        }\
        else if(i % ctx->Nk == 4){\
            if(ctx->Nk > 6){\
                /* Extra SBox for Nk > 6 */\
                t[0] = sbox(t[0]); t[1] = sbox(t[1]);\
                t[2] = sbox(t[2]); t[3] = sbox(t[3]);\
            }\
        }\
        offset += 4;\
        ctx->rk[offset]     = ctx->rk[offset - (4 * ctx->Nk)]     ^ t[0];\
        ctx->rk[offset + 1] = ctx->rk[offset - (4 * ctx->Nk) + 1] ^ t[1];\
        ctx->rk[offset + 2] = ctx->rk[offset - (4 * ctx->Nk) + 2] ^ t[2];\
        ctx->rk[offset + 3] = ctx->rk[offset - (4 * ctx->Nk) + 3] ^ t[3];\
    }\
} while(0);

/* Encryption primitive */
#define RIJNDAEL_ENC(ctx, data_in, data_out) do {\
    uint32_t r;\
    /* Our local state (maximum 32 bytes) */\
    uint8_t state[32];\
\
    if((ctx == NULL) || (data_in == NULL) || (data_out == NULL)){\
        goto err;\
    }\
    /* Sanity check for array access */\
    if((4 * ctx->Nb * (ctx->Nr + 1)) > sizeof(ctx->rk)){\
        goto err;\
    }\
    memcpy(state, data_in, 4 * ctx->Nb);\
\
    /* Initial add round key */\
    ADD_RKEY(ctx, state, &ctx->rk[0]);\
\
    /* All our rounds except the last one */\
    for(r = 1; r < ctx->Nr; r++){\
        SUB_BYTES_SR(ctx, state);\
        MIX_COLUMNS(ctx, state);\
        ADD_RKEY(ctx, state, &ctx->rk[4 * ctx->Nb * r]);\
    }\
    /* Last round without mixcolumns */\
    SUB_BYTES_SR(ctx, state);\
    ADD_RKEY(ctx, state, &ctx->rk[4 * ctx->Nb * r]);\
\
    /* Copy back state to ciphertext */\
    memcpy(data_out, state, 4 * ctx->Nb);\
\
    /* Clean stuff */\
    memset(state, 0, sizeof(state));\
} while(0);

/* ==== Public APIs ===== */

WEAK int aes128_ref_setkey_enc(rijndael_ref_ctx_aes128 *ctx, const uint8_t key[16]) {
	int ret = -1;

	RIJNDAEL_SETKEY_ENC(ctx, key, AES128);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ref_setkey_enc(rijndael_ref_ctx_aes256 *ctx, const uint8_t key[32]) {
	int ret = -1;

	RIJNDAEL_SETKEY_ENC(ctx, key, AES256);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ref_setkey_enc(rijndael_ref_ctx_rijndael256 *ctx, const uint8_t key[32]) {
	int ret = -1;

	RIJNDAEL_SETKEY_ENC(ctx, key, RIJNDAEL_256_256);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ref_enc(const rijndael_ref_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	int ret = -1;

	if (ctx->rtype != AES128) {
		goto err;
	}

	RIJNDAEL_ENC(ctx, data_in, data_out);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ref_enc_x2(const rijndael_ref_ctx_aes128 *ctx1, const rijndael_ref_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret;

	ret  = aes128_ref_enc(ctx1, plainText1, cipherText1);
	ret |= aes128_ref_enc(ctx2, plainText2, cipherText2);

	return ret;
}

WEAK int aes128_ref_enc_x4(const rijndael_ref_ctx_aes128 *ctx1, const rijndael_ref_ctx_aes128 *ctx2, const rijndael_ref_ctx_aes128 *ctx3, const rijndael_ref_ctx_aes128 *ctx4,
                           const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                           uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret;

	ret  = aes128_ref_enc(ctx1, plainText1, cipherText1);
	ret |= aes128_ref_enc(ctx2, plainText2, cipherText2);
	ret |= aes128_ref_enc(ctx3, plainText3, cipherText3);
	ret |= aes128_ref_enc(ctx4, plainText4, cipherText4);

	return ret;
}

WEAK int aes128_ref_enc_x8(const rijndael_ref_ctx_aes128 *ctx1, const rijndael_ref_ctx_aes128 *ctx2, const rijndael_ref_ctx_aes128 *ctx3, const rijndael_ref_ctx_aes128 *ctx4,
                           const rijndael_ref_ctx_aes128 *ctx5, const rijndael_ref_ctx_aes128 *ctx6, const rijndael_ref_ctx_aes128 *ctx7, const rijndael_ref_ctx_aes128 *ctx8,
                           const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                           const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                           uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                           uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes128_ref_enc_x4(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= aes128_ref_enc_x4(ctx5, ctx6, ctx7, ctx8, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

WEAK int aes256_ref_enc(const rijndael_ref_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	int ret = -1;

	if (ctx->rtype != AES256) {
		goto err;
	}

	RIJNDAEL_ENC(ctx, data_in, data_out);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ref_enc_x2(const rijndael_ref_ctx_aes256 *ctx1, const rijndael_ref_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret;

	ret  = aes256_ref_enc(ctx1, plainText1, cipherText1);
	ret |= aes256_ref_enc(ctx2, plainText2, cipherText2);

	return ret;
}


WEAK int aes256_ref_enc_x4(const rijndael_ref_ctx_aes256 *ctx1, const rijndael_ref_ctx_aes256 *ctx2, const rijndael_ref_ctx_aes256 *ctx3, const rijndael_ref_ctx_aes256 *ctx4,
                           const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                           uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret;

	ret  = aes256_ref_enc(ctx1, plainText1, cipherText1);
	ret |= aes256_ref_enc(ctx2, plainText2, cipherText2);
	ret |= aes256_ref_enc(ctx3, plainText3, cipherText3);
	ret |= aes256_ref_enc(ctx4, plainText4, cipherText4);

	return ret;
}

WEAK int aes256_ref_enc_x8(const rijndael_ref_ctx_aes256 *ctx1, const rijndael_ref_ctx_aes256 *ctx2, const rijndael_ref_ctx_aes256 *ctx3, const rijndael_ref_ctx_aes256 *ctx4,
                           const rijndael_ref_ctx_aes256 *ctx5, const rijndael_ref_ctx_aes256 *ctx6, const rijndael_ref_ctx_aes256 *ctx7, const rijndael_ref_ctx_aes256 *ctx8,
                           const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                           const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                           uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                           uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes256_ref_enc_x4(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= aes256_ref_enc_x4(ctx5, ctx6, ctx7, ctx8, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

WEAK int rijndael256_ref_enc(const rijndael_ref_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]) {
	int ret = -1;

	if (ctx->rtype != RIJNDAEL_256_256) {
		goto err;
	}

	RIJNDAEL_ENC(ctx, data_in, data_out);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ref_enc_x2(const rijndael_ref_ctx_rijndael256 *ctx1, const rijndael_ref_ctx_rijndael256 *ctx2,
                                const uint8_t plainText1[32], const uint8_t plainText2[32],
                                uint8_t cipherText1[32], uint8_t cipherText2[32]) {
	int ret;

	ret  = rijndael256_ref_enc(ctx1, plainText1, cipherText1);
	ret |= rijndael256_ref_enc(ctx2, plainText2, cipherText2);

	return ret;

}


WEAK int rijndael256_ref_enc_x4(const rijndael_ref_ctx_rijndael256 *ctx1, const rijndael_ref_ctx_rijndael256 *ctx2, const rijndael_ref_ctx_rijndael256 *ctx3, const rijndael_ref_ctx_rijndael256 *ctx4,
                                const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]) {
	int ret;

	ret  = rijndael256_ref_enc(ctx1, plainText1, cipherText1);
	ret |= rijndael256_ref_enc(ctx2, plainText2, cipherText2);
	ret |= rijndael256_ref_enc(ctx3, plainText3, cipherText3);
	ret |= rijndael256_ref_enc(ctx4, plainText4, cipherText4);

	return ret;
}

WEAK int rijndael256_ref_enc_x8(const rijndael_ref_ctx_rijndael256 *ctx1, const rijndael_ref_ctx_rijndael256 *ctx2, const rijndael_ref_ctx_rijndael256 *ctx3, const rijndael_ref_ctx_rijndael256 *ctx4,
                                const rijndael_ref_ctx_rijndael256 *ctx5, const rijndael_ref_ctx_rijndael256 *ctx6, const rijndael_ref_ctx_rijndael256 *ctx7, const rijndael_ref_ctx_rijndael256 *ctx8,
                                const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                                uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                                uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]) {
	int ret = 0;
	ret |= rijndael256_ref_enc_x4(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= rijndael256_ref_enc_x4(ctx5, ctx6, ctx7, ctx8, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

MAKE_GENERIC_FUNCS_XX_IMPL(aes128, ref, 16, 16)
MAKE_GENERIC_FUNCS_XX_IMPL(aes256, ref, 32, 16)
MAKE_GENERIC_FUNCS_XX_IMPL(rijndael256, ref, 32, 32)

#else /* !RIJNDAEL_CONSTANT_TIME_REF, */
/*
 * Dummy definition to avoid the empty translation unit ISO C warning
 */
typedef int dummy;
#endif
