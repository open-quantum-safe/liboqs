#ifndef __ENC_MUPQ_H__
#define __ENC_MUPQ_H__

/*************************************/
/********** MUPQ API *****************/
/*************************************/
/* For 128 bits security for MUPQ when local Rijndael is not defined, we use the AES-128 dedicated "private" API */
#include "aes.h"
#include "aes-publicinputs.h"
/* XXX: NOTE: when possible, we use the faster LUT based AES for key schedule
 * and the constant-time "fixsliced" AES for encryption.
 * Hence the following union.
 * See: https://github.com/mupq/mupq/issues/171 for the discussion
 * */
typedef union {
	aes128ctx_publicinputs ctx_pub;
	aes128ctx ctx_priv;
} enc_ctx;
typedef struct {
        enc_ctx ctx1;
        enc_ctx ctx2;
} enc_ctx_x2;
typedef struct {
        enc_ctx_x2 ctx1;
        enc_ctx_x2 ctx2;
} enc_ctx_x4;
typedef struct {
        enc_ctx_x4 ctx1;
        enc_ctx_x4 ctx2;
} enc_ctx_x8;

#define enc_ctx_pub aes128ctx_publicinputs
typedef struct {
        enc_ctx_pub ctx1;
        enc_ctx_pub ctx2;
} enc_ctx_pub_x2;
typedef struct {
        enc_ctx_pub_x2 ctx1;
        enc_ctx_pub_x2 ctx2;
} enc_ctx_pub_x4;
typedef struct {
        enc_ctx_pub_x4 ctx1;
        enc_ctx_pub_x4 ctx2;
} enc_ctx_pub_x8;

/* Helpers to interleave two round keys, stolen from https://github.com/aadomn/aes/blob/master/opt32/fixslicing/aes_encrypt.c */
#define SWAPMOVE(a, b, mask, n) ({                                                      \
        tmp = (b ^ (a >> n)) & mask;                                                    \
        b ^= tmp;                                                                       \
        a ^= (tmp << n);                                                                \
})
#define LE_LOAD_32(x)                                                                   \
    ((((uint32_t)((x)[3])) << 24) |                                                     \
     (((uint32_t)((x)[2])) << 16) |                                                     \
     (((uint32_t)((x)[1])) << 8) |                                                      \
      ((uint32_t)((x)[0])))

/******************************************************************************
* Applies ShiftRows^(-1) on a round key to match the fixsliced representation.
******************************************************************************/
static inline void inv_shiftrows_1(uint32_t* rkey) {
	uint32_t tmp;
	for (int i = 0; i < 8; i++) {
		SWAPMOVE(rkey[i], rkey[i], 0x0c0f0300, 4);
		SWAPMOVE(rkey[i], rkey[i], 0x33003300, 2);
	}
}
/******************************************************************************
* Applies ShiftRows^(-2) on a round key to match the fixsliced representation.
******************************************************************************/
static inline void inv_shiftrows_2(uint32_t* rkey) {
	uint32_t tmp;
	for (int i = 0; i < 8; i++) {
		SWAPMOVE(rkey[i], rkey[i], 0x0f000f00, 4);
	}
}

/******************************************************************************
* Applies ShiftRows^(-3) on a round key to match the fixsliced representation.
******************************************************************************/
static inline void inv_shiftrows_3(uint32_t* rkey) {
	uint32_t tmp;
	for (int i = 0; i < 8; i++) {
		SWAPMOVE(rkey[i], rkey[i], 0x030f0c00, 4);
		SWAPMOVE(rkey[i], rkey[i], 0x33003300, 2);
	}
}

static inline void keys_packing(uint32_t* out, const unsigned char* in0,
                                const unsigned char *in1, unsigned int i) {
	MQOM2_SYM_MEASURE_PRE
	uint32_t tmp;
	out[0] = LE_LOAD_32(in0);
	out[1] = LE_LOAD_32(in1);
	out[2] = LE_LOAD_32(in0 + 4);
	out[3] = LE_LOAD_32(in1 + 4);
	out[4] = LE_LOAD_32(in0 + 8);
	out[5] = LE_LOAD_32(in1 + 8);
	out[6] = LE_LOAD_32(in0 + 12);
	out[7] = LE_LOAD_32(in1 + 12);
	SWAPMOVE(out[1], out[0], 0x55555555, 1);
	SWAPMOVE(out[3], out[2], 0x55555555, 1);
	SWAPMOVE(out[5], out[4], 0x55555555, 1);
	SWAPMOVE(out[7], out[6], 0x55555555, 1);
	SWAPMOVE(out[2], out[0], 0x33333333, 2);
	SWAPMOVE(out[3], out[1], 0x33333333, 2);
	SWAPMOVE(out[6], out[4], 0x33333333, 2);
	SWAPMOVE(out[7], out[5], 0x33333333, 2);
	SWAPMOVE(out[4], out[0], 0x0f0f0f0f, 4);
	SWAPMOVE(out[5], out[1], 0x0f0f0f0f, 4);
	SWAPMOVE(out[6], out[2], 0x0f0f0f0f, 4);
	SWAPMOVE(out[7], out[3], 0x0f0f0f0f, 4);
	/* Apply inverse shiftrows on some round keys to match fixslicing */
	switch (i) {
	case 3:
	case 7:
		inv_shiftrows_3(out);
		break;
	case 1:
	case 5:
	case 9:
		inv_shiftrows_1(out);
		break;
	case 2:
	case 6:
		inv_shiftrows_2(out);
		break;
	default:
		break;
	}
	if (i > 0) {
		/* Apply the xor with 0xffffffff since it is expected by the bitslice
		 * encryption to speedup the SBox */
		out[1] ^= 0xffffffff;
		out[2] ^= 0xffffffff;
		out[6] ^= 0xffffffff;
		out[7] ^= 0xffffffff;
	}
	MQOM2_SYM_MEASURE_POST
}

/* XXX: NOTE: on a x86 host, we force MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE since
 * we cannot mix public and private AES .... */
#if defined(__i386__) || defined(__x86_64__)
#define MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE
#endif

static inline int enc_key_sched(enc_ctx *ctx, const uint8_t key[16]) {
#if defined(MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE)
	aes128_ecb_keyexp(&ctx->ctx_priv, key);
#else
	/* By default, and since our key schedule only deals with public data, we
	 * use the non constant-time LUT based key schedule for performance
	 * */
	aes128_ecb_keyexp_publicinputs(&ctx->ctx_pub, key);
#endif
	return 0;
}
static inline int enc_key_sched_pub(enc_ctx_pub *ctx, const uint8_t key[16]) {
	aes128_ecb_keyexp_publicinputs(ctx, key);
	return 0;
}
static inline int enc_key_sched_x2(enc_ctx_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret;
	ret  = enc_key_sched(&ctx->ctx1, key1);
	ret |= enc_key_sched(&ctx->ctx2, key2);
	return ret;
}
static inline int enc_key_sched_x4(enc_ctx_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret;
	ret  = enc_key_sched_x2(&ctx->ctx1, key1, key2);
	ret |= enc_key_sched_x2(&ctx->ctx2, key3, key4);
	return ret;
}
static inline int enc_key_sched_x8(enc_ctx_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret;
	ret  = enc_key_sched_x4(&ctx->ctx1, key1, key2, key3, key4);
	ret |= enc_key_sched_x4(&ctx->ctx2, key5, key6, key7, key8);
	return ret;
}
static inline int enc_key_sched_pub_x2(enc_ctx_pub_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret;
	ret  = enc_key_sched_pub(&ctx->ctx1, key1);
	ret |= enc_key_sched_pub(&ctx->ctx2, key2);
	return ret;
}
static inline int enc_key_sched_pub_x4(enc_ctx_pub_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret;
	ret  = enc_key_sched_pub_x2(&ctx->ctx1, key1, key2);
	ret |= enc_key_sched_pub_x2(&ctx->ctx2, key3, key4);
	return ret;
}
static inline int enc_key_sched_pub_x8(enc_ctx_pub_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret;
	ret  = enc_key_sched_pub_x4(&ctx->ctx1, key1, key2, key3, key4);
	ret |= enc_key_sched_pub_x4(&ctx->ctx2, key5, key6, key7, key8);
	return ret;
}

static inline int enc_encrypt(const enc_ctx *ctx, const uint8_t pt[16], uint8_t ct[16]) {
#if !defined(MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE)
	unsigned int i;
	uint32_t *rk = (uint32_t*)(&ctx->ctx_pub);
	uint32_t interleaved_keys[88];
	/* Transfer the keys from LUT to bitslice */
	for (i = 0; i < 11; i++) {
		keys_packing(&interleaved_keys[8 * i], (uint8_t*)&rk[4 * i], (uint8_t*)&rk[4 * i], i);
	}
	aes128_ecb(ct, pt, 1, (aes128ctx*)&interleaved_keys);
#else
	aes128_ecb(ct, pt, 1, &ctx->ctx_priv);
#endif
	return 0;
}
static inline int enc_encrypt_pub(const enc_ctx_pub *ctx, const uint8_t pt[16], uint8_t ct[16]) {
	aes128_ecb_publicinputs(ct, pt, 1, ctx);
	return 0;
}

static inline int enc_encrypt_x2(const enc_ctx *ctx1, const enc_ctx *ctx2, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
#if !defined(MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE)
	unsigned int i;
	uint32_t *rk1 = (uint32_t*)(&ctx1->ctx_pub);
	uint32_t *rk2 = (uint32_t*)(&ctx2->ctx_pub);
	uint32_t interleaved_keys[88];
	/* Transfer the keys from LUT to bitslice */
	for (i = 0; i < 11; i++) {
		keys_packing(&interleaved_keys[8 * i], (uint8_t*)&rk1[4 * i], (uint8_t*)&rk2[4 * i], i);
	}
	{
		uint8_t pt[32];
		uint8_t ct[32];

		memcpy(&pt[0], pt1, 16);
		memcpy(&pt[16], pt2, 16);

		aes128_ecb(ct, pt, 2, (aes128ctx*)&interleaved_keys);

		memcpy(ct1, &ct[0], 16);
		memcpy(ct2, &ct[16], 16);
	}
#else
	if (ctx1 == ctx2) {
		uint8_t pt[32];
		uint8_t ct[32];

		memcpy(&pt[0], pt1, 16);
		memcpy(&pt[16], pt2, 16);

		aes128_ecb(ct, pt, 2, &ctx1->ctx_priv);

		memcpy(ct1, &ct[0], 16);
		memcpy(ct2, &ct[16], 16);
	} else {
		aes128_ecb(ct1, pt1, 1, &ctx1->ctx_priv);
		aes128_ecb(ct2, pt2, 1, &ctx2->ctx_priv);
	}
#endif
	return 0;
}
static inline int enc_encrypt_x2_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	if (ctx1 == ctx2) {
		uint8_t pt[32];
		uint8_t ct[32];

		memcpy(&pt[0], pt1, 16);
		memcpy(&pt[16], pt2, 16);

		aes128_ecb_publicinputs(ct, pt, 2, ctx1);

		memcpy(ct1, &ct[0], 16);
		memcpy(ct2, &ct[16], 16);
	} else {
		aes128_ecb_publicinputs(ct1, pt1, 1, ctx1);
		aes128_ecb_publicinputs(ct2, pt2, 1, ctx2);
	}

	return 0;
}
static inline int enc_encrypt_x2_x2(const enc_ctx_x2 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret;
	ret  = enc_encrypt(&ctx->ctx1, pt1, ct1);
	ret |= enc_encrypt(&ctx->ctx2, pt2, ct2);
	return ret;
}
static inline int enc_encrypt_x2_pub_x2(const enc_ctx_pub_x2 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret;
	ret  = enc_encrypt_pub(&ctx->ctx1, pt1, ct1);
	ret |= enc_encrypt_pub(&ctx->ctx2, pt2, ct2);
	return ret;
}


static inline int enc_encrypt_x4(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
#if !defined(MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE)
	int ret = 0;
	ret |= enc_encrypt_x2(ctx1, ctx2, pt1, pt2, ct1, ct2);
	ret |= enc_encrypt_x2(ctx3, ctx4, pt3, pt4, ct3, ct4);
	return ret;
#else
	if ((ctx1 == ctx2) && (ctx2 == ctx3) && (ctx3 == ctx4)) {
		uint8_t pt[64];
		uint8_t ct[64];

		memcpy(&pt[0],  pt1, 16);
		memcpy(&pt[16], pt2, 16);
		memcpy(&pt[32], pt3, 16);
		memcpy(&pt[48], pt4, 16);

		aes128_ecb(ct, pt, 4, &ctx1->ctx_priv);

		memcpy(ct1, &ct[0], 16);
		memcpy(ct2, &ct[16], 16);
		memcpy(ct3, &ct[32], 16);
		memcpy(ct4, &ct[48], 16);
	} else {
		enc_encrypt_x2(ctx1, ctx2, pt1, pt2, ct1, ct2);
		enc_encrypt_x2(ctx3, ctx4, pt3, pt4, ct3, ct4);
	}
	return 0;
#endif
}
static inline int enc_encrypt_x4_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	if ((ctx1 == ctx2) && (ctx2 == ctx3) && (ctx3 == ctx4)) {
		uint8_t pt[64];
		uint8_t ct[64];

		memcpy(&pt[0],  pt1, 16);
		memcpy(&pt[16], pt2, 16);
		memcpy(&pt[32], pt3, 16);
		memcpy(&pt[48], pt4, 16);

		aes128_ecb_publicinputs(ct, pt, 4, ctx1);

		memcpy(ct1, &ct[0], 16);
		memcpy(ct2, &ct[16], 16);
		memcpy(ct3, &ct[32], 16);
		memcpy(ct4, &ct[48], 16);
	} else {
		enc_encrypt_x2_pub(ctx1, ctx2, pt1, pt2, ct1, ct2);
		enc_encrypt_x2_pub(ctx3, ctx4, pt3, pt4, ct3, ct4);
	}
	return 0;
}
static inline int enc_encrypt_x4_x4(const enc_ctx_x4 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret;
	ret  = enc_encrypt_x2_x2(&ctx->ctx1, pt1, pt2, ct1, ct2);
	ret |= enc_encrypt_x2_x2(&ctx->ctx2, pt3, pt4, ct3, ct4);
	return ret;
}
static inline int enc_encrypt_x4_pub_x4(const enc_ctx_pub_x4 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret;
	ret  = enc_encrypt_x2_pub_x2(&ctx->ctx1, pt1, pt2, ct1, ct2);
	ret |= enc_encrypt_x2_pub_x2(&ctx->ctx2, pt3, pt4, ct3, ct4);
	return ret;
}

static inline int enc_encrypt_x8(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const enc_ctx *ctx5, const enc_ctx *ctx6, const enc_ctx *ctx7, const enc_ctx *ctx8,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                 uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
#if !defined(MQOM2_FOR_MUPQ_AES_GENERIC_KEYSCHEDULE)
	int ret = 0;
	ret |= enc_encrypt_x4(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	ret |= enc_encrypt_x4(ctx5, ctx6, ctx7, ctx8, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);
	return ret;
#else
	if ((ctx1 == ctx2) && (ctx2 == ctx3) && (ctx3 == ctx4) && (ctx4 == ctx5) && (ctx5 == ctx6) && (ctx6 == ctx7) && (ctx7 == ctx8)) {
		uint8_t pt[128];
		uint8_t ct[128];

		memcpy(&pt[0],   pt1, 16);
		memcpy(&pt[16],  pt2, 16);
		memcpy(&pt[32],  pt3, 16);
		memcpy(&pt[48],  pt4, 16);
		memcpy(&pt[64],  pt5, 16);
		memcpy(&pt[80],  pt6, 16);
		memcpy(&pt[96],  pt7, 16);
		memcpy(&pt[112], pt8, 16);

		aes128_ecb(ct, pt, 8, &ctx1->ctx_priv);

		memcpy(ct1, &ct[0],  16);
		memcpy(ct2, &ct[16],  16);
		memcpy(ct3, &ct[32],  16);
		memcpy(ct4, &ct[48],  16);
		memcpy(ct5, &ct[64],  16);
		memcpy(ct6, &ct[80],  16);
		memcpy(ct7, &ct[96],  16);
		memcpy(ct8, &ct[112], 16);
	} else {
		enc_encrypt_x4(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
		enc_encrypt_x4(ctx5, ctx6, ctx7, ctx8, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);
	}
	return 0;
#endif
}
static inline int enc_encrypt_x8_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const enc_ctx_pub *ctx5, const enc_ctx_pub *ctx6, const enc_ctx_pub *ctx7, const enc_ctx_pub *ctx8,
                                     const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                     uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
	if ((ctx1 == ctx2) && (ctx2 == ctx3) && (ctx3 == ctx4) && (ctx4 == ctx5) && (ctx5 == ctx6) && (ctx6 == ctx7) && (ctx7 == ctx8)) {
		uint8_t pt[128];
		uint8_t ct[128];

		memcpy(&pt[0],   pt1, 16);
		memcpy(&pt[16],  pt2, 16);
		memcpy(&pt[32],  pt3, 16);
		memcpy(&pt[48],  pt4, 16);
		memcpy(&pt[64],  pt5, 16);
		memcpy(&pt[80],  pt6, 16);
		memcpy(&pt[96],  pt7, 16);
		memcpy(&pt[112], pt8, 16);

		aes128_ecb_publicinputs(ct, pt, 8, ctx1);

		memcpy(ct1, &ct[0],  16);
		memcpy(ct2, &ct[16],  16);
		memcpy(ct3, &ct[32],  16);
		memcpy(ct4, &ct[48],  16);
		memcpy(ct5, &ct[64],  16);
		memcpy(ct6, &ct[80],  16);
		memcpy(ct7, &ct[96],  16);
		memcpy(ct8, &ct[112], 16);
	} else {
		enc_encrypt_x4_pub(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
		enc_encrypt_x4_pub(ctx5, ctx6, ctx7, ctx8, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);
	}
	return 0;
}
static inline int enc_encrypt_x8_x8(const enc_ctx_x8 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                     uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) { 
	int ret;
	ret  = enc_encrypt_x4_x4(&ctx->ctx1, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	ret |= enc_encrypt_x4_x4(&ctx->ctx2, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);
	return ret;
}
static inline int enc_encrypt_x8_pub_x8(const enc_ctx_pub_x8 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                     uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
	int ret;
	ret  = enc_encrypt_x4_pub_x4(&ctx->ctx1, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	ret |= enc_encrypt_x4_pub_x4(&ctx->ctx2, pt5, pt6, pt7, pt8, ct5, ct6, ct7, ct8);
	return ret;
}

#endif /* __ENC_MUPQ_H__ */
