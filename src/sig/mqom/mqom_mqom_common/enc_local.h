#ifndef __ENC_LOCAL_H__
#define __ENC_LOCAL_H__

/*************************************/
/********** Local ********************/
/*************************************/

/* ===================================== */
/* ===================================== */
/* === 128 bits security === */
#if (MQOM2_PARAM_SECURITY == 128) && !defined(MQOM2_SEC128_OVERRIDE)
/* Our block encryption is based on Rijndael variants */
#include "rijndael.h"
/* For 128 bits security, we transparently use AES-128 */
#define enc_ctx rijndael_ctx_aes128
#define enc_ctx_pub rijndael_ctx_aes128_pub
#define enc_ctx_x2 rijndael_ctx_aes128_x2
#define enc_ctx_pub_x2 rijndael_ctx_aes128_pub_x2
#define enc_ctx_x4 rijndael_ctx_aes128_x4
#define enc_ctx_pub_x4 rijndael_ctx_aes128_pub_x4
#define enc_ctx_x8 rijndael_ctx_aes128_x8
#define enc_ctx_pub_x8 rijndael_ctx_aes128_pub_x8

static inline int enc_key_sched(enc_ctx *ctx, const uint8_t key[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc(ctx, key);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub(enc_ctx_pub *ctx, const uint8_t key[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_pub(ctx, key);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_x2(enc_ctx_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_x2(ctx, key1, key2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x2(enc_ctx_pub_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_pub_x2(ctx, key1, key2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_x4(enc_ctx_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_x4(ctx, key1, key2, key3, key4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x4(enc_ctx_pub_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_pub_x4(ctx, key1, key2, key3, key4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_x8(enc_ctx_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_x8(ctx, key1, key2, key3, key4, key5, key6, key7, key8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x8(enc_ctx_pub_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16], const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_setkey_enc_pub_x8(ctx, key1, key2, key3, key4, key5, key6, key7, key8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}


static inline int enc_encrypt(const enc_ctx *ctx, const uint8_t pt[16], uint8_t ct[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc(ctx, pt, ct);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_pub(const enc_ctx_pub *ctx, const uint8_t pt[16], uint8_t ct[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_pub(ctx, pt, ct);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x2(const enc_ctx *ctx1, const enc_ctx *ctx2, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x2(ctx1, ctx2, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x2_pub(ctx1, ctx2, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_x2(const enc_ctx_x2 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x2_x2(ctx, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_pub_x2(const enc_ctx_pub_x2 *ctx, const uint8_t pt1[16], const uint8_t pt2[16], uint8_t ct1[16], uint8_t ct2[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x2_pub_x2(ctx, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x4(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x4(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x4_pub(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_x4(const enc_ctx_x4 *ctx,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x4_x4(ctx, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_pub_x4(const enc_ctx_pub_x4 *ctx,
                                     const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x4_pub_x4(ctx, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x8(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const enc_ctx *ctx5, const enc_ctx *ctx6, const enc_ctx *ctx7, const enc_ctx *ctx8,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                 uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x8(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const enc_ctx_pub *ctx5, const enc_ctx_pub *ctx6, const enc_ctx_pub *ctx7, const enc_ctx_pub *ctx8,
                                     const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                     uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = aes128_enc_x8_pub(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_x8(const enc_ctx_x8 *ctx,
                                 const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                 const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                 uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                 uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
        int ret;
        MQOM2_SYM_MEASURE_PRE
        ret = aes128_enc_x8_x8(ctx, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
        MQOM2_SYM_MEASURE_POST
        return ret;
}
static inline int enc_encrypt_x8_pub_x8(const enc_ctx_pub_x8 *ctx,
                                     const uint8_t pt1[16], const uint8_t pt2[16], const uint8_t pt3[16], const uint8_t pt4[16],
                                     const uint8_t pt5[16], const uint8_t pt6[16], const uint8_t pt7[16], const uint8_t pt8[16],
                                     uint8_t ct1[16], uint8_t ct2[16], uint8_t ct3[16], uint8_t ct4[16],
                                     uint8_t ct5[16], uint8_t ct6[16], uint8_t ct7[16], uint8_t ct8[16]) {
        int ret;
        MQOM2_SYM_MEASURE_PRE
        ret = aes128_enc_x8_pub_x8(ctx, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
        MQOM2_SYM_MEASURE_POST
        return ret;
}


/* ===================================== */
/* ===================================== */
/* === 192 bits security === */
#elif MQOM2_PARAM_SECURITY == 192
/* Our block encryption is based on Rijndael variants */
#include "rijndael.h"

#define enc_ctx rijndael_ctx_rijndael256
#define enc_ctx_pub rijndael_ctx_rijndael256_pub
#define enc_ctx_x2 rijndael_ctx_rijndael256_x2
#define enc_ctx_pub_x2 rijndael_ctx_rijndael256_pub_x2
#define enc_ctx_x4 rijndael_ctx_rijndael256_x4
#define enc_ctx_pub_x4 rijndael_ctx_rijndael256_pub_x4
#define enc_ctx_x8 rijndael_ctx_rijndael256_x8
#define enc_ctx_pub_x8 rijndael_ctx_rijndael256_pub_x8

/* Specifically for 192 bits, we need to adapt the underlying calls
 * as we use Rijndael-256-256 with expansion and truncation
 * */
static inline int enc_key_sched(enc_ctx *ctx, const uint8_t key[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key[32] = { 0 };
	memcpy(exp_key, key, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc(ctx, exp_key);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub(enc_ctx_pub *ctx, const uint8_t key[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key[32] = { 0 };
	memcpy(exp_key, key, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_pub(ctx, exp_key);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_key_sched_x2(enc_ctx_x2 *ctx, const uint8_t key1[24], const uint8_t key2[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key1[32] = { 0 };
	uint8_t exp_key2[32] = { 0 };
	memcpy(exp_key1, key1, 24);
	memcpy(exp_key2, key2, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_x2(ctx, exp_key1, exp_key2);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x2(enc_ctx_pub_x2 *ctx, const uint8_t key1[24], const uint8_t key2[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key1[32] = { 0 };
	uint8_t exp_key2[32] = { 0 };
	memcpy(exp_key1, key1, 24);
	memcpy(exp_key2, key2, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_pub_x2(ctx, exp_key1, exp_key2);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}


static inline int enc_key_sched_x4(enc_ctx_x4 *ctx, const uint8_t key1[24], const uint8_t key2[24], const uint8_t key3[24], const uint8_t key4[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key1[32] = { 0 };
	uint8_t exp_key2[32] = { 0 };
	uint8_t exp_key3[32] = { 0 };
	uint8_t exp_key4[32] = { 0 };
	memcpy(exp_key1, key1, 24);
	memcpy(exp_key2, key2, 24);
	memcpy(exp_key3, key3, 24);
	memcpy(exp_key4, key4, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_x4(ctx, exp_key1, exp_key2, exp_key3, exp_key4);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x4(enc_ctx_pub_x4 *ctx, const uint8_t key1[24], const uint8_t key2[24], const uint8_t key3[24], const uint8_t key4[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key1[32] = { 0 };
	uint8_t exp_key2[32] = { 0 };
	uint8_t exp_key3[32] = { 0 };
	uint8_t exp_key4[32] = { 0 };
	memcpy(exp_key1, key1, 24);
	memcpy(exp_key2, key2, 24);
	memcpy(exp_key3, key3, 24);
	memcpy(exp_key4, key4, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_pub_x4(ctx, exp_key1, exp_key2, exp_key3, exp_key4);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_key_sched_x8(enc_ctx_x8 *ctx, const uint8_t key1[24], const uint8_t key2[24], const uint8_t key3[24], const uint8_t key4[24], const uint8_t key5[24], const uint8_t key6[24], const uint8_t key7[24], const uint8_t key8[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key1[32] = { 0 };
	uint8_t exp_key2[32] = { 0 };
	uint8_t exp_key3[32] = { 0 };
	uint8_t exp_key4[32] = { 0 };
	uint8_t exp_key5[32] = { 0 };
	uint8_t exp_key6[32] = { 0 };
	uint8_t exp_key7[32] = { 0 };
	uint8_t exp_key8[32] = { 0 };
	memcpy(exp_key1, key1, 24);
	memcpy(exp_key2, key2, 24);
	memcpy(exp_key3, key3, 24);
	memcpy(exp_key4, key4, 24);
	memcpy(exp_key5, key5, 24);
	memcpy(exp_key6, key6, 24);
	memcpy(exp_key7, key7, 24);
	memcpy(exp_key8, key8, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_x8(ctx, exp_key1, exp_key2, exp_key3, exp_key4, exp_key5, exp_key6, exp_key7, exp_key8);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x8(enc_ctx_pub_x8 *ctx, const uint8_t key1[24], const uint8_t key2[24], const uint8_t key3[24], const uint8_t key4[24], const uint8_t key5[24], const uint8_t key6[24], const uint8_t key7[24], const uint8_t key8[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Key expansion key || O^64 */
	uint8_t exp_key1[32] = { 0 };
	uint8_t exp_key2[32] = { 0 };
	uint8_t exp_key3[32] = { 0 };
	uint8_t exp_key4[32] = { 0 };
	uint8_t exp_key5[32] = { 0 };
	uint8_t exp_key6[32] = { 0 };
	uint8_t exp_key7[32] = { 0 };
	uint8_t exp_key8[32] = { 0 };
	memcpy(exp_key1, key1, 24);
	memcpy(exp_key2, key2, 24);
	memcpy(exp_key3, key3, 24);
	memcpy(exp_key4, key4, 24);
	memcpy(exp_key5, key5, 24);
	memcpy(exp_key6, key6, 24);
	memcpy(exp_key7, key7, 24);
	memcpy(exp_key8, key8, 24);

	/* Execute the Rijndael-256-256 key schedule */
	ret = rijndael256_setkey_enc_pub_x8(ctx, exp_key1, exp_key2, exp_key3, exp_key4, exp_key5, exp_key6, exp_key7, exp_key8);
	ERR(ret, err);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt(const enc_ctx *ctx, const uint8_t pt[24], uint8_t ct[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[32] = { 0 }, exp_ct[32];
	memcpy(exp_pt, pt, 24);

	/* Encrypt */
	ret = rijndael256_enc(ctx, exp_pt, exp_ct);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct, exp_ct, 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_pub(const enc_ctx_pub *ctx, const uint8_t pt[24], uint8_t ct[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[32] = { 0 }, exp_ct[32];
	memcpy(exp_pt, pt, 24);

	/* Encrypt */
	ret = rijndael256_enc_pub(ctx, exp_pt, exp_ct);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct, exp_ct, 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}


static inline int enc_encrypt_x2(const enc_ctx *ctx1, const enc_ctx *ctx2, const uint8_t pt1[24], const uint8_t pt2[24], uint8_t ct1[24], uint8_t ct2[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[2][32] = { 0 }, exp_ct[2][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	/* Encrypt */
	ret = rijndael256_enc_x2(ctx1, ctx2, exp_pt[0], exp_pt[1], exp_ct[0], exp_ct[1]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const uint8_t pt1[24], const uint8_t pt2[24], uint8_t ct1[24], uint8_t ct2[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[2][32] = { 0 }, exp_ct[2][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	/* Encrypt */
	ret = rijndael256_enc_x2_pub(ctx1, ctx2, exp_pt[0], exp_pt[1], exp_ct[0], exp_ct[1]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_x2(const enc_ctx_x2 *ctx, const uint8_t pt1[24], const uint8_t pt2[24], uint8_t ct1[24], uint8_t ct2[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[2][32] = { 0 }, exp_ct[2][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	/* Encrypt */
	ret = rijndael256_enc_x2_x2(ctx, exp_pt[0], exp_pt[1], exp_ct[0], exp_ct[1]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_pub_x2(const enc_ctx_pub_x2 *ctx, const uint8_t pt1[24], const uint8_t pt2[24], uint8_t ct1[24], uint8_t ct2[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[2][32] = { 0 }, exp_ct[2][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	/* Encrypt */
	ret = rijndael256_enc_x2_pub_x2(ctx, exp_pt[0], exp_pt[1], exp_ct[0], exp_ct[1]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x4(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                 uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[4][32] = { 0 }, exp_ct[4][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);

	/* Encrypt */
	ret = rijndael256_enc_x4(ctx1, ctx2, ctx3, ctx4, exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                     uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[4][32] = { 0 }, exp_ct[4][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);

	/* Encrypt */
	ret = rijndael256_enc_x4_pub(ctx1, ctx2, ctx3, ctx4, exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_x4(const enc_ctx_x4 *ctx,
                                 const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                 uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24]) {
	int ret = -1;

	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[4][32] = { 0 }, exp_ct[4][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);

	/* Encrypt */
	ret = rijndael256_enc_x4_x4(ctx, exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_pub_x4(const enc_ctx_pub_x4 *ctx,
                                     const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                     uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[4][32] = { 0 }, exp_ct[4][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);

	/* Encrypt */
	ret = rijndael256_enc_x4_pub_x4(ctx, exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x8(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const enc_ctx *ctx5, const enc_ctx *ctx6, const enc_ctx *ctx7, const enc_ctx *ctx8,
                                 const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                 const uint8_t pt5[24], const uint8_t pt6[24], const uint8_t pt7[24], const uint8_t pt8[24],
                                 uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24],
                                 uint8_t ct5[24], uint8_t ct6[24], uint8_t ct7[24], uint8_t ct8[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[8][32] = { 0 }, exp_ct[8][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);
	memcpy(exp_pt[4], pt5, 24);
	memcpy(exp_pt[5], pt6, 24);
	memcpy(exp_pt[6], pt7, 24);
	memcpy(exp_pt[7], pt8, 24);

	/* Encrypt */
	ret = rijndael256_enc_x8(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8,
	                         exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_pt[4], exp_pt[5], exp_pt[6], exp_pt[7],
	                         exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3], exp_ct[4], exp_ct[5], exp_ct[6], exp_ct[7]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);
	memcpy(ct5, exp_ct[4], 24);
	memcpy(ct6, exp_ct[5], 24);
	memcpy(ct7, exp_ct[6], 24);
	memcpy(ct8, exp_ct[7], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const enc_ctx_pub *ctx5, const enc_ctx_pub *ctx6, const enc_ctx_pub *ctx7, const enc_ctx_pub *ctx8,
                                     const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                     const uint8_t pt5[24], const uint8_t pt6[24], const uint8_t pt7[24], const uint8_t pt8[24],
                                     uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24],
                                     uint8_t ct5[24], uint8_t ct6[24], uint8_t ct7[24], uint8_t ct8[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[8][32] = { 0 }, exp_ct[8][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);
	memcpy(exp_pt[4], pt5, 24);
	memcpy(exp_pt[5], pt6, 24);
	memcpy(exp_pt[6], pt7, 24);
	memcpy(exp_pt[7], pt8, 24);

	/* Encrypt */
	ret = rijndael256_enc_x8_pub(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8,
	                             exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_pt[4], exp_pt[5], exp_pt[6], exp_pt[7],
	                             exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3], exp_ct[4], exp_ct[5], exp_ct[6], exp_ct[7]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);
	memcpy(ct5, exp_ct[4], 24);
	memcpy(ct6, exp_ct[5], 24);
	memcpy(ct7, exp_ct[6], 24);
	memcpy(ct8, exp_ct[7], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_x8(const enc_ctx_x8 *ctx,
                                 const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                 const uint8_t pt5[24], const uint8_t pt6[24], const uint8_t pt7[24], const uint8_t pt8[24],
                                 uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24],
                                 uint8_t ct5[24], uint8_t ct6[24], uint8_t ct7[24], uint8_t ct8[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[8][32] = { 0 }, exp_ct[8][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);
	memcpy(exp_pt[4], pt5, 24);
	memcpy(exp_pt[5], pt6, 24);
	memcpy(exp_pt[6], pt7, 24);
	memcpy(exp_pt[7], pt8, 24);

	/* Encrypt */
	ret = rijndael256_enc_x8_x8(ctx,
	                         exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_pt[4], exp_pt[5], exp_pt[6], exp_pt[7],
	                         exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3], exp_ct[4], exp_ct[5], exp_ct[6], exp_ct[7]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);
	memcpy(ct5, exp_ct[4], 24);
	memcpy(ct6, exp_ct[5], 24);
	memcpy(ct7, exp_ct[6], 24);
	memcpy(ct8, exp_ct[7], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_pub_x8(const enc_ctx_pub_x8 *ctx,
                                     const uint8_t pt1[24], const uint8_t pt2[24], const uint8_t pt3[24], const uint8_t pt4[24],
                                     const uint8_t pt5[24], const uint8_t pt6[24], const uint8_t pt7[24], const uint8_t pt8[24],
                                     uint8_t ct1[24], uint8_t ct2[24], uint8_t ct3[24], uint8_t ct4[24],
                                     uint8_t ct5[24], uint8_t ct6[24], uint8_t ct7[24], uint8_t ct8[24]) {
	int ret = -1;
	MQOM2_SYM_MEASURE_PRE
	/* Plaintext expansion pt || 0^64 */
	uint8_t exp_pt[8][32] = { 0 }, exp_ct[8][32];
	memcpy(exp_pt[0], pt1, 24);
	memcpy(exp_pt[1], pt2, 24);
	memcpy(exp_pt[2], pt3, 24);
	memcpy(exp_pt[3], pt4, 24);
	memcpy(exp_pt[4], pt5, 24);
	memcpy(exp_pt[5], pt6, 24);
	memcpy(exp_pt[6], pt7, 24);
	memcpy(exp_pt[7], pt8, 24);

	/* Encrypt */
	ret = rijndael256_enc_x8_pub_x8(ctx,
	                             exp_pt[0], exp_pt[1], exp_pt[2], exp_pt[3], exp_pt[4], exp_pt[5], exp_pt[6], exp_pt[7],
	                             exp_ct[0], exp_ct[1], exp_ct[2], exp_ct[3], exp_ct[4], exp_ct[5], exp_ct[6], exp_ct[7]);
	ERR(ret, err);

	/* Truncate the output */
	memcpy(ct1, exp_ct[0], 24);
	memcpy(ct2, exp_ct[1], 24);
	memcpy(ct3, exp_ct[2], 24);
	memcpy(ct4, exp_ct[3], 24);
	memcpy(ct5, exp_ct[4], 24);
	memcpy(ct6, exp_ct[5], 24);
	memcpy(ct7, exp_ct[6], 24);
	memcpy(ct8, exp_ct[7], 24);

	ret = 0;
err:
	MQOM2_SYM_MEASURE_POST
	return ret;
}

/* ===================================== */
/* ===================================== */
/* === 256 bits security === */
#elif MQOM2_PARAM_SECURITY == 256
/* Our block encryption is based on Rijndael variants */
#include "rijndael.h"

#define enc_ctx rijndael_ctx_rijndael256
#define enc_ctx_pub rijndael_ctx_rijndael256_pub
#define enc_ctx_x2 rijndael_ctx_rijndael256_x2
#define enc_ctx_pub_x2 rijndael_ctx_rijndael256_pub_x2
#define enc_ctx_x4 rijndael_ctx_rijndael256_x4
#define enc_ctx_pub_x4 rijndael_ctx_rijndael256_pub_x4
#define enc_ctx_x8 rijndael_ctx_rijndael256_x8
#define enc_ctx_pub_x8 rijndael_ctx_rijndael256_pub_x8

/* For 256 bits security, we tyransparently use Rijndael-256-256 */
static inline int enc_key_sched(enc_ctx *ctx, const uint8_t key[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc(ctx, key);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub(enc_ctx_pub *ctx, const uint8_t key[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_pub(ctx, key);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_key_sched_x2(enc_ctx_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_x2(ctx, key1, key2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x2(enc_ctx_pub_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_pub_x2(ctx, key1, key2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_key_sched_x4(enc_ctx_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_x4(ctx, key1, key2, key3, key4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x4(enc_ctx_pub_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_pub_x4(ctx, key1, key2, key3, key4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_key_sched_x8(enc_ctx_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32], const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_x8(ctx, key1, key2, key3, key4, key5, key6, key7, key8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_key_sched_pub_x8(enc_ctx_pub_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32], const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_setkey_enc_pub_x8(ctx, key1, key2, key3, key4, key5, key6, key7, key8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt(const enc_ctx *ctx, const uint8_t pt[32], uint8_t ct[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc(ctx, pt, ct);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_pub(const enc_ctx_pub *ctx, const uint8_t pt[32], uint8_t ct[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_pub(ctx, pt, ct);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x2(const enc_ctx *ctx1, const enc_ctx *ctx2, const uint8_t pt1[32], const uint8_t pt2[32], uint8_t ct1[32], uint8_t ct2[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x2(ctx1, ctx2, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const uint8_t pt1[32], const uint8_t pt2[32], uint8_t ct1[32], uint8_t ct2[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x2_pub(ctx1, ctx2, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_x2(const enc_ctx_x2 *ctx, const uint8_t pt1[32], const uint8_t pt2[32], uint8_t ct1[32], uint8_t ct2[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x2_x2(ctx, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x2_pub_x2(const enc_ctx_pub_x2 *ctx, const uint8_t pt1[32], const uint8_t pt2[32], uint8_t ct1[32], uint8_t ct2[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x2_pub_x2(ctx, pt1, pt2, ct1, ct2);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x4(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                 uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x4(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                     uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x4_pub(ctx1, ctx2, ctx3, ctx4, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_x4(const enc_ctx_x4 *ctx,
                                 const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                 uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x4_x4(ctx, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x4_pub_x4(const enc_ctx_pub_x4 *ctx,
                                     const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                     uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x4_pub_x4(ctx, pt1, pt2, pt3, pt4, ct1, ct2, ct3, ct4);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

static inline int enc_encrypt_x8(const enc_ctx *ctx1, const enc_ctx *ctx2, const enc_ctx *ctx3, const enc_ctx *ctx4,
                                 const enc_ctx *ctx5, const enc_ctx *ctx6, const enc_ctx *ctx7, const enc_ctx *ctx8,
                                 const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                 const uint8_t pt5[32], const uint8_t pt6[32], const uint8_t pt7[32], const uint8_t pt8[32],
                                 uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32],
                                 uint8_t ct5[32], uint8_t ct6[32], uint8_t ct7[32], uint8_t ct8[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x8(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_pub(const enc_ctx_pub *ctx1, const enc_ctx_pub *ctx2, const enc_ctx_pub *ctx3, const enc_ctx_pub *ctx4,
                                     const enc_ctx_pub *ctx5, const enc_ctx_pub *ctx6, const enc_ctx_pub *ctx7, const enc_ctx_pub *ctx8,
                                     const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                     const uint8_t pt5[32], const uint8_t pt6[32], const uint8_t pt7[32], const uint8_t pt8[32],
                                     uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32],
                                     uint8_t ct5[32], uint8_t ct6[32], uint8_t ct7[32], uint8_t ct8[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x8_pub(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_x8(const enc_ctx_x8 *ctx,
                                 const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                 const uint8_t pt5[32], const uint8_t pt6[32], const uint8_t pt7[32], const uint8_t pt8[32],
                                 uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32],
                                 uint8_t ct5[32], uint8_t ct6[32], uint8_t ct7[32], uint8_t ct8[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x8_x8(ctx, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}
static inline int enc_encrypt_x8_pub_x8(const enc_ctx_pub_x8 *ctx,
                                     const uint8_t pt1[32], const uint8_t pt2[32], const uint8_t pt3[32], const uint8_t pt4[32],
                                     const uint8_t pt5[32], const uint8_t pt6[32], const uint8_t pt7[32], const uint8_t pt8[32],
                                     uint8_t ct1[32], uint8_t ct2[32], uint8_t ct3[32], uint8_t ct4[32],
                                     uint8_t ct5[32], uint8_t ct6[32], uint8_t ct7[32], uint8_t ct8[32]) {
	int ret;
	MQOM2_SYM_MEASURE_PRE
	ret = rijndael256_enc_x8_pub_x8(ctx, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
	MQOM2_SYM_MEASURE_POST
	return ret;
}

#else

/* The only case where we can legitimately end here is if we have 128 bits security
 * level and override it in another header ... If this is not the case, trigger a
 * compilation error. */
#if !defined(MQOM2_SEC128_OVERRIDE)
#error "Sorry, unsupported security parameters: must be one of 128, 192, 256"
#endif

#endif

#endif /* __ENC_LOCAL_H__ */
