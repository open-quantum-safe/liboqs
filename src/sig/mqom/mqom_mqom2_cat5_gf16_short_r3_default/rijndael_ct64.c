#include "rijndael_platform.h"

#if defined(RIJNDAEL_BITSLICE)
#include "rijndael_ct64_enc.h"

#undef CT_KEYSCHED
#if defined(RIJNDAEL_CT64_CT_KEYSCHED)
#define CT_KEYSCHED 1
#else
#define CT_KEYSCHED 0
#endif

/****** API functions *******/
#if defined(RIJNDAEL_OPT_ARMV7M)
/* Sanity check for options */
#if defined(RIJNDAEL_CT64_CT_KEYSCHED)
#error "RIJNDAEL_CT64_CT_KEYSCHED and RIJNDAEL_OPT_ARMV7M are not compatible for now ..."
#endif
/* When RIJNDAEL_OPT_ARMV7M is defined, we use the assembly optimized with t-tables
 * from  https://eprint.iacr.org/2016/714.pdf
 * NOTE: we do not need a constant time key schedule as only public data is used */

/* Helper to interleave two round keys, stolen from https://github.com/aadomn/aes/blob/master/opt32/fixslicing/aes_encrypt.c */
#define SWAPMOVE(a, b, mask, n) ({                          \
    tmp = (b ^ (a >> n)) & mask;                            \
    b ^= tmp;                                   \
    a ^= (tmp << n);                                \
})
#define LE_LOAD_32(x)                                   \
    ((((uint32_t)((x)[3])) << 24) |                             \
     (((uint32_t)((x)[2])) << 16) |                             \
     (((uint32_t)((x)[1])) << 8) |                          \
      ((uint32_t)((x)[0])))

/******************************************************************************
* Applies ShiftRows^(-1) on a round key to match the fixsliced representation.
******************************************************************************/
static void inv_shiftrows_1(uint32_t* rkey) {
	uint32_t tmp;
	for (int i = 0; i < 8; i++) {
		SWAPMOVE(rkey[i], rkey[i], 0x0c0f0300, 4);
		SWAPMOVE(rkey[i], rkey[i], 0x33003300, 2);
	}
}
/******************************************************************************
* Applies ShiftRows^(-2) on a round key to match the fixsliced representation.
******************************************************************************/
static void inv_shiftrows_2(uint32_t* rkey) {
	uint32_t tmp;
	for (int i = 0; i < 8; i++) {
		SWAPMOVE(rkey[i], rkey[i], 0x0f000f00, 4);
	}
}

/******************************************************************************
* Applies ShiftRows^(-3) on a round key to match the fixsliced representation.
******************************************************************************/
static void inv_shiftrows_3(uint32_t* rkey) {
	uint32_t tmp;
	for (int i = 0; i < 8; i++) {
		SWAPMOVE(rkey[i], rkey[i], 0x030f0c00, 4);
		SWAPMOVE(rkey[i], rkey[i], 0x33003300, 2);
	}
}
/******************************************************************************
* Packs two 128-bit input blocs in0, in1 into the 256-bit internal state out
* where the bits are packed as follows:
* out[0] = b_24 b_56 b_88 b_120 || ... || b_0 b_32 b_64 b_96
* out[1] = b_25 b_57 b_89 b_121 || ... || b_1 b_33 b_65 b_97
* out[2] = b_26 b_58 b_90 b_122 || ... || b_2 b_34 b_66 b_98
* out[3] = b_27 b_59 b_91 b_123 || ... || b_3 b_35 b_67 b_99
* out[4] = b_28 b_60 b_92 b_124 || ... || b_4 b_36 b_68 b_100
* out[5] = b_29 b_61 b_93 b_125 || ... || b_5 b_37 b_69 b_101
* out[6] = b_30 b_62 b_94 b_126 || ... || b_6 b_38 b_70 b_102
* out[7] = b_31 b_63 b_95 b_127 || ... || b_7 b_39 b_71 b_103
******************************************************************************/
static void keys_packing(uint32_t* out, const unsigned char* in0,
                  const unsigned char *in1, unsigned int i) {
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
}

/* Deal with namespacing */
#define AES_128_keyschedule MQOM_NAMESPACE(AES_128_keyschedule)
extern void AES_128_keyschedule(const uint8_t *, uint8_t *);
WEAK int aes128_ct64_setkey_enc(rijndael_ct64_ctx_aes128 *ctx, const uint8_t key[16]) {
	int ret = -1;
	uint8_t *rk;

	if (ctx == NULL) {
		goto err;
	}
	ctx->rtype = AES128;
	rk = (uint8_t*)(ctx->rk);

	memcpy(&rk[0], key, 16);
	AES_128_keyschedule(key, &rk[16]);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_setkey_enc_x2(rijndael_ct64_ctx_aes128_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret = -1;
	unsigned int i;
	rijndael_ct64_ctx_aes128 cctx0, cctx1;

	if(ctx == NULL){
		goto err;
	}

	ret = aes128_ct64_setkey_enc(&cctx0, key1);
	if(ret){
		goto err;
	}
	ret = aes128_ct64_setkey_enc(&cctx1, key2);
	if(ret){
		goto err;
	}
	ctx->rtype = AES128;

	/* The "fixsliced" implementation expects two interleaved keys */
	for (i = 0; i < 11; i++) {
		keys_packing(&ctx->interleaved_rkeys[8 * i], (uint8_t*)(&cctx0.rk[4 * i]), (uint8_t*)(&cctx1.rk[4 * i]), i);
	}

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_setkey_enc_x4(rijndael_ct64_ctx_aes128_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret = 0;
	ret |= aes128_ct64_setkey_enc_x2(&ctx->ctx1, key1, key2);
	ret |= aes128_ct64_setkey_enc_x2(&ctx->ctx2, key3, key4);
	return ret;
}

WEAK int aes128_ct64_setkey_enc_x8(rijndael_ct64_ctx_aes128_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16],
				   const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret = 0;
	ret |= aes128_ct64_setkey_enc_x4(&ctx->ctx1, key1, key2, key3, key4);
	ret |= aes128_ct64_setkey_enc_x4(&ctx->ctx2, key5, key6, key7, key8);
	return ret;
}

#else /* !RIJNDAEL_OPT_ARMV7M */
WEAK int aes128_ct64_setkey_enc(rijndael_ct64_ctx_aes128 *ctx, const uint8_t key[16]) {
	int ret = -1;

	BR_AES_CT64_KEYSCHED(ctx, key, AES128, CT_KEYSCHED);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_setkey_enc_x2(rijndael_ct64_ctx_aes128_x2 *ctx, const uint8_t key1[16], const uint8_t key2[16]) {
	int ret = -1;
	rijndael_ct64_ctx_aes128 ctx_[2];
	rijndael_ct64_ctx_aes128 *cctx0 = &ctx_[0];
	rijndael_ct64_ctx_aes128 *cctx1 = &ctx_[1];
	rijndael_ct64_ctx_aes128 *dummy_ctx = NULL;

	if(ctx == NULL){
		goto err;
	}

	BR_AES_CT64_KEYSCHED(cctx0, key1, AES128, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx1, key2, AES128, CT_KEYSCHED);

	uint64_t *keys = &ctx->interleaved_rkeys[0];
	ctx->rtype = cctx0->rtype;
	ctx->Nr = cctx0->Nr;
	ctx->Nk = cctx0->Nk;
	ctx->Nb = cctx0->Nb;

	BITSLICE_KEYS(cctx0, cctx1, dummy_ctx, dummy_ctx, keys);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_setkey_enc_x4(rijndael_ct64_ctx_aes128_x4 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16]) {
	int ret = -1;
	rijndael_ct64_ctx_aes128 ctx_[4];
	rijndael_ct64_ctx_aes128 *cctx0 = &ctx_[0];
	rijndael_ct64_ctx_aes128 *cctx1 = &ctx_[1];
	rijndael_ct64_ctx_aes128 *cctx2 = &ctx_[2];
	rijndael_ct64_ctx_aes128 *cctx3 = &ctx_[3];

	if(ctx == NULL){
		goto err;
	}

	BR_AES_CT64_KEYSCHED(cctx0, key1, AES128, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx1, key2, AES128, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx2, key3, AES128, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx3, key4, AES128, CT_KEYSCHED);

	uint64_t *keys = &ctx->interleaved_rkeys[0];
	ctx->rtype = cctx0->rtype;
	ctx->Nr = cctx0->Nr;
	ctx->Nk = cctx0->Nk;
	ctx->Nb = cctx0->Nb;

	BITSLICE_KEYS(cctx0, cctx1, cctx2, cctx3, keys);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_setkey_enc_x8(rijndael_ct64_ctx_aes128_x8 *ctx, const uint8_t key1[16], const uint8_t key2[16], const uint8_t key3[16], const uint8_t key4[16],
				   const uint8_t key5[16], const uint8_t key6[16], const uint8_t key7[16], const uint8_t key8[16]) {
	int ret = 0;

	ret  = aes128_ct64_setkey_enc_x4(&ctx->ctx1, key1, key2, key3, key4);
	ret |= aes128_ct64_setkey_enc_x4(&ctx->ctx2, key5, key6, key7, key8);

	return ret;
}
#endif /* !RIJNDAEL_OPT_ARMV7M */

/*** AES-256 set key enc ***/
WEAK int aes256_ct64_setkey_enc(rijndael_ct64_ctx_aes256 *ctx, const uint8_t key[32]) {
	int ret = -1;

	BR_AES_CT64_KEYSCHED(ctx, key, AES256, CT_KEYSCHED);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_setkey_enc_x2(rijndael_ct64_ctx_aes256_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]) {
	int ret = -1;
	rijndael_ct64_ctx_aes256 ctx_[2];
	rijndael_ct64_ctx_aes256 *cctx0 = &ctx_[0];
	rijndael_ct64_ctx_aes256 *cctx1 = &ctx_[1];
	rijndael_ct64_ctx_aes256 *dummy_ctx = NULL;

	if(ctx == NULL){
		goto err;
	}

	BR_AES_CT64_KEYSCHED(cctx0, key1, AES256, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx1, key2, AES256, CT_KEYSCHED);

	uint64_t *keys = &ctx->interleaved_rkeys[0];
	ctx->rtype = cctx0->rtype;
	ctx->Nr = cctx0->Nr;
	ctx->Nk = cctx0->Nk;
	ctx->Nb = cctx0->Nb;

	BITSLICE_KEYS(cctx0, cctx1, dummy_ctx, dummy_ctx, keys);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_setkey_enc_x4(rijndael_ct64_ctx_aes256_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]) {
	int ret = -1;
	rijndael_ct64_ctx_aes256 ctx_[4];
	rijndael_ct64_ctx_aes256 *cctx0 = &ctx_[0];
	rijndael_ct64_ctx_aes256 *cctx1 = &ctx_[1];
	rijndael_ct64_ctx_aes256 *cctx2 = &ctx_[2];
	rijndael_ct64_ctx_aes256 *cctx3 = &ctx_[3];

	if(ctx == NULL){
		goto err;
	}

	BR_AES_CT64_KEYSCHED(cctx0, key1, AES256, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx1, key2, AES256, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx2, key3, AES256, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx3, key4, AES256, CT_KEYSCHED);

	uint64_t *keys = &ctx->interleaved_rkeys[0];
	ctx->rtype = cctx0->rtype;
	ctx->Nr = cctx0->Nr;
	ctx->Nk = cctx0->Nk;
	ctx->Nb = cctx0->Nb;

	BITSLICE_KEYS(cctx0, cctx1, cctx2, cctx3, keys);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_setkey_enc_x8(rijndael_ct64_ctx_aes256_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32],
				   const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]) {
	int ret = 0;

	ret  = aes256_ct64_setkey_enc_x4(&ctx->ctx1, key1, key2, key3, key4);
	ret |= aes256_ct64_setkey_enc_x4(&ctx->ctx2, key5, key6, key7, key8);

	return ret;
}

/*** RIJNDAEL-256 set key enc ***/
WEAK int rijndael256_ct64_setkey_enc(rijndael_ct64_ctx_rijndael256 *ctx, const uint8_t key[32]) {
	int ret = -1;

	BR_AES_CT64_KEYSCHED(ctx, key, RIJNDAEL_256_256, CT_KEYSCHED);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ct64_setkey_enc_x2(rijndael_ct64_ctx_rijndael256_x2 *ctx, const uint8_t key1[32], const uint8_t key2[32]) {
	int ret = -1;
	rijndael_ct64_ctx_rijndael256 ctx_[2];
	rijndael_ct64_ctx_rijndael256 *cctx0 = &ctx_[0];
	rijndael_ct64_ctx_rijndael256 *cctx1 = &ctx_[1];
	rijndael_ct64_ctx_rijndael256 *dummy_ctx = NULL;

	if(ctx == NULL){
		goto err;
	}

	BR_AES_CT64_KEYSCHED(cctx0, key1, RIJNDAEL_256_256, CT_KEYSCHED);
	BR_AES_CT64_KEYSCHED(cctx1, key2, RIJNDAEL_256_256, CT_KEYSCHED);

	uint64_t *keys = &ctx->interleaved_rkeys[0];
	ctx->rtype = cctx0->rtype;
	ctx->Nr = cctx0->Nr;
	ctx->Nk = cctx0->Nk;
	ctx->Nb = cctx0->Nb;

	BITSLICE_KEYS(cctx0, cctx1, dummy_ctx, dummy_ctx, keys);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ct64_setkey_enc_x4(rijndael_ct64_ctx_rijndael256_x4 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32]) {
	int ret = 0;

	ret  = rijndael256_ct64_setkey_enc_x2(&ctx->ctx1, key1, key2);
	ret |= rijndael256_ct64_setkey_enc_x2(&ctx->ctx2, key3, key4);

	return ret;
}

WEAK int rijndael256_ct64_setkey_enc_x8(rijndael_ct64_ctx_rijndael256_x8 *ctx, const uint8_t key1[32], const uint8_t key2[32], const uint8_t key3[32], const uint8_t key4[32],
				   const uint8_t key5[32], const uint8_t key6[32], const uint8_t key7[32], const uint8_t key8[32]) {
	int ret = 0;

	ret  = rijndael256_ct64_setkey_enc_x2(&ctx->ctx1, key1, key2);
	ret |= rijndael256_ct64_setkey_enc_x2(&ctx->ctx2, key3, key4);
	ret |= rijndael256_ct64_setkey_enc_x2(&ctx->ctx3, key5, key6);
	ret |= rijndael256_ct64_setkey_enc_x2(&ctx->ctx4, key7, key8);

	return ret;
}


// === AES-128 enc
#if defined(RIJNDAEL_OPT_ARMV7M)
/* When RIJNDAEL_OPT_ARMV7M is defined, we use the assembly optimized "fixsliced" based implementation
 * from https://eprint.iacr.org/2020/1123.pdf */
/* Deal with namespacing */
#define aes128_encrypt_ffs MQOM_NAMESPACE(aes128_encrypt_ffs)
extern void aes128_encrypt_ffs(uint8_t* ctext, uint8_t* ctext_bis, const uint8_t* ptext,
                               const uint8_t *ptext_bis, const uint32_t *rkey);

WEAK int aes128_ct64_enc(const rijndael_ct64_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	int ret = -1;
	unsigned int i;
	uint32_t interleaved_rkeys[88];

	if ((ctx == NULL) || (ctx->rtype != AES128)) {
		goto err;
	}

	/* The "fixsliced" implementation expects two interleaved keys: use the same key as a dummy one */
	for (i = 0; i < 11; i++) {
		keys_packing(&interleaved_rkeys[8 * i], (uint8_t*)(&ctx->rk[4 * i]), (uint8_t*)(&ctx->rk[4 * i]), i);
	}
	aes128_encrypt_ffs(data_out, data_out, data_in, data_in, interleaved_rkeys);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x2(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = -1;
	unsigned int i;
	uint32_t interleaved_rkeys[88];

	if ((ctx1 == NULL) || (ctx1->rtype != AES128)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != AES128)) {
		goto err;
	}

	/* The "fixsliced" implementation expects two interleaved keys */
	for (i = 0; i < 11; i++) {
		keys_packing(&interleaved_rkeys[8 * i], (uint8_t*)(&ctx1->rk[4 * i]), (uint8_t*)(&ctx2->rk[4 * i]), i);
	}
	aes128_encrypt_ffs(cipherText1, cipherText2, plainText1, plainText2, interleaved_rkeys);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x2_x2(const rijndael_ct64_ctx_aes128_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = -1;
	if(ctx->rtype != AES128){
		goto err;
	}

	aes128_encrypt_ffs(cipherText1, cipherText2, plainText1, plainText2, ctx->interleaved_rkeys);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x4(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const rijndael_ct64_ctx_aes128 *ctx3, const rijndael_ct64_ctx_aes128 *ctx4,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = 0;
	ret |= aes128_ct64_enc_x2(ctx1, ctx2, plainText1, plainText2, cipherText1, cipherText2);
	ret |= aes128_ct64_enc_x2(ctx3, ctx4, plainText3, plainText4, cipherText3, cipherText4);
	return ret;
}

WEAK int aes128_ct64_enc_x4_x4(const rijndael_ct64_ctx_aes128_x4 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = 0;
	ret |= aes128_ct64_enc_x2_x2(&ctx->ctx1, plainText1, plainText2, cipherText1, cipherText2);
	ret |= aes128_ct64_enc_x2_x2(&ctx->ctx2, plainText3, plainText4, cipherText3, cipherText4);
	return ret;
}

#else /* !RIJNDAEL_OPT_ARMV7M */
WEAK int aes128_ct64_enc(const rijndael_ct64_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	int ret = -1;

	rijndael_ct64_ctx_aes128 *dummy_ctx = NULL;

	if ((ctx == NULL) || (ctx->rtype != AES128)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx, dummy_ctx, dummy_ctx, dummy_ctx,
	                           data_in, NULL, NULL, NULL,
	                           data_out, NULL, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x2(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = -1;

	rijndael_ct64_ctx_aes128 *dummy_ctx = NULL;

	if ((ctx1 == NULL) || (ctx1->rtype != AES128)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != AES128)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, dummy_ctx, dummy_ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x2_x2(const rijndael_ct64_ctx_aes128_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = -1;

	if ((ctx == NULL) || (ctx->rtype != AES128)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT_ALREADY_BITSLICED(ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x4(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const rijndael_ct64_ctx_aes128 *ctx3, const rijndael_ct64_ctx_aes128 *ctx4,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = -1;

	if ((ctx1 == NULL) || (ctx1->rtype != AES128)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != AES128)) {
		goto err;
	}
	if ((ctx3 == NULL) || (ctx3->rtype != AES128)) {
		goto err;
	}
	if ((ctx4 == NULL) || (ctx4->rtype != AES128)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, ctx3, ctx4,
	                           plainText1, plainText2, plainText3, plainText4,
	                           cipherText1, cipherText2, cipherText3, cipherText4);

	ret = 0;
err:
	return ret;
}

WEAK int aes128_ct64_enc_x4_x4(const rijndael_ct64_ctx_aes128_x4 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = -1;

	if ((ctx == NULL) || (ctx->rtype != AES128)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT_ALREADY_BITSLICED(ctx,
	                           plainText1, plainText2, plainText3, plainText4,
	                           cipherText1, cipherText2, cipherText3, cipherText4);

	ret = 0;
err:
	return ret;
}
#endif /* !RIJNDAEL_OPT_ARMV7M */

WEAK int aes128_ct64_enc_x8(const rijndael_ct64_ctx_aes128 *ctx1, const rijndael_ct64_ctx_aes128 *ctx2, const rijndael_ct64_ctx_aes128 *ctx3, const rijndael_ct64_ctx_aes128 *ctx4,
                            const rijndael_ct64_ctx_aes128 *ctx5, const rijndael_ct64_ctx_aes128 *ctx6, const rijndael_ct64_ctx_aes128 *ctx7, const rijndael_ct64_ctx_aes128 *ctx8,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                            uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes128_ct64_enc_x4(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= aes128_ct64_enc_x4(ctx5, ctx6, ctx7, ctx8, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

WEAK int aes128_ct64_enc_x8_x8(const rijndael_ct64_ctx_aes128_x8 *ctx,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                            uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes128_ct64_enc_x4_x4(&ctx->ctx1, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= aes128_ct64_enc_x4_x4(&ctx->ctx2, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}


// === AES-256 enc
WEAK int aes256_ct64_enc(const rijndael_ct64_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	int ret = -1;

	rijndael_ct64_ctx_aes256 *dummy_ctx = NULL;

	if ((ctx == NULL) || (ctx->rtype != AES256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx, dummy_ctx, dummy_ctx, dummy_ctx,
	                           data_in, NULL, NULL, NULL,
	                           data_out, NULL, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_enc_x2(const rijndael_ct64_ctx_aes256 *ctx1, const rijndael_ct64_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = -1;

	rijndael_ct64_ctx_aes256 *dummy_ctx = NULL;

	if ((ctx1 == NULL) || (ctx1->rtype != AES256)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != AES256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, dummy_ctx, dummy_ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_enc_x2_x2(const rijndael_ct64_ctx_aes256_x2 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = -1;

	if ((ctx == NULL) || (ctx->rtype != AES256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT_ALREADY_BITSLICED(ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_enc_x4(const rijndael_ct64_ctx_aes256 *ctx1, const rijndael_ct64_ctx_aes256 *ctx2, const rijndael_ct64_ctx_aes256 *ctx3, const rijndael_ct64_ctx_aes256 *ctx4,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = -1;

	if ((ctx1 == NULL) || (ctx1->rtype != AES256)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != AES256)) {
		goto err;
	}
	if ((ctx3 == NULL) || (ctx3->rtype != AES256)) {
		goto err;
	}
	if ((ctx4 == NULL) || (ctx4->rtype != AES256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, ctx3, ctx4,
	                           plainText1, plainText2, plainText3, plainText4,
	                           cipherText1, cipherText2, cipherText3, cipherText4);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_enc_x4_x4(const rijndael_ct64_ctx_aes256_x4 *ctx, const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = -1;

	if ((ctx == NULL) || (ctx->rtype != AES256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT_ALREADY_BITSLICED(ctx,
	                           plainText1, plainText2, plainText3, plainText4,
	                           cipherText1, cipherText2, cipherText3, cipherText4);

	ret = 0;
err:
	return ret;
}

WEAK int aes256_ct64_enc_x8(const rijndael_ct64_ctx_aes256 *ctx1, const rijndael_ct64_ctx_aes256 *ctx2, const rijndael_ct64_ctx_aes256 *ctx3, const rijndael_ct64_ctx_aes256 *ctx4,
                            const rijndael_ct64_ctx_aes256 *ctx5, const rijndael_ct64_ctx_aes256 *ctx6, const rijndael_ct64_ctx_aes256 *ctx7, const rijndael_ct64_ctx_aes256 *ctx8,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                            uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes256_ct64_enc_x4(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= aes256_ct64_enc_x4(ctx5, ctx6, ctx7, ctx8, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

WEAK int aes256_ct64_enc_x8_x8(const rijndael_ct64_ctx_aes256_x8 *ctx,
                            const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                            const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                            uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                            uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes256_ct64_enc_x4_x4(&ctx->ctx1, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= aes256_ct64_enc_x4_x4(&ctx->ctx2, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

// === Rijndael-256 enc
WEAK int rijndael256_ct64_enc(const rijndael_ct64_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]) {
	int ret = -1;

	rijndael_ct64_ctx_rijndael256 *dummy_ctx = NULL;

	if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx, dummy_ctx, dummy_ctx, dummy_ctx,
	                           data_in, NULL, NULL, NULL,
	                           data_out, NULL, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ct64_enc_x2(const rijndael_ct64_ctx_rijndael256 *ctx1, const rijndael_ct64_ctx_rijndael256 *ctx2, const uint8_t plainText1[32], const uint8_t plainText2[32], uint8_t cipherText1[32], uint8_t cipherText2[32]) {
	int ret = -1;

	rijndael_ct64_ctx_rijndael256 *dummy_ctx = NULL;

	if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, dummy_ctx, dummy_ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ct64_enc_x2_x2(const rijndael_ct64_ctx_rijndael256_x2 *ctx, const uint8_t plainText1[32], const uint8_t plainText2[32], uint8_t cipherText1[32], uint8_t cipherText2[32]) {
	int ret = -1;

	if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) {
		goto err;
	}
	CORE_CT64_BITSLICE_ENCRYPT_ALREADY_BITSLICED(ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ct64_enc_x4(const rijndael_ct64_ctx_rijndael256 *ctx1, const rijndael_ct64_ctx_rijndael256 *ctx2, const rijndael_ct64_ctx_rijndael256 *ctx3, const rijndael_ct64_ctx_rijndael256 *ctx4,
                                 const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                 uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]) {
	int ret = -1;

	rijndael_ct64_ctx_rijndael256 *dummy_ctx = NULL;

	if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256)) {
		goto err;
	}
	if ((ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256)) {
		goto err;
	}
	if ((ctx3 == NULL) || (ctx3->rtype != RIJNDAEL_256_256)) {
		goto err;;
	}
	if ((ctx4 == NULL) || (ctx4->rtype != RIJNDAEL_256_256)) {
		goto err;
	}

	CORE_CT64_BITSLICE_ENCRYPT(ctx1, ctx2, dummy_ctx, dummy_ctx,
	                           plainText1, plainText2, NULL, NULL,
	                           cipherText1, cipherText2, NULL, NULL);

	CORE_CT64_BITSLICE_ENCRYPT(ctx3, ctx4, dummy_ctx, dummy_ctx,
	                           plainText3, plainText4, NULL, NULL,
	                           cipherText3, cipherText4, NULL, NULL);

	ret = 0;
err:
	return ret;
}

WEAK int rijndael256_ct64_enc_x4_x4(const rijndael_ct64_ctx_rijndael256_x4 *ctx, const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                 uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]) {
	int ret = 0;
	ret |= rijndael256_ct64_enc_x2_x2(&ctx->ctx1, plainText1, plainText2, cipherText1, cipherText2);
	ret |= rijndael256_ct64_enc_x2_x2(&ctx->ctx2, plainText3, plainText4, cipherText3, cipherText4);
	return ret;
}

WEAK int rijndael256_ct64_enc_x8(const rijndael_ct64_ctx_rijndael256 *ctx1, const rijndael_ct64_ctx_rijndael256 *ctx2, const rijndael_ct64_ctx_rijndael256 *ctx3, const rijndael_ct64_ctx_rijndael256 *ctx4,
                                 const rijndael_ct64_ctx_rijndael256 *ctx5, const rijndael_ct64_ctx_rijndael256 *ctx6, const rijndael_ct64_ctx_rijndael256 *ctx7, const rijndael_ct64_ctx_rijndael256 *ctx8,
                                 const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                 const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                                 uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                                 uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]) {
	int ret = 0;
	ret |= rijndael256_ct64_enc_x4(ctx1, ctx2, ctx3, ctx4, plainText1, plainText2, plainText3, plainText4, cipherText1, cipherText2, cipherText3, cipherText4);
	ret |= rijndael256_ct64_enc_x4(ctx5, ctx6, ctx7, ctx8, plainText5, plainText6, plainText7, plainText8, cipherText5, cipherText6, cipherText7, cipherText8);
	return ret;
}

WEAK int rijndael256_ct64_enc_x8_x8(const rijndael_ct64_ctx_rijndael256_x8 *ctx,
                                 const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                 const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                                 uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                                 uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]) {
	int ret = 0;
	ret |= rijndael256_ct64_enc_x2_x2(&ctx->ctx1, plainText1, plainText2, cipherText1, cipherText2);
	ret |= rijndael256_ct64_enc_x2_x2(&ctx->ctx2, plainText3, plainText4, cipherText3, cipherText4);
	ret |= rijndael256_ct64_enc_x2_x2(&ctx->ctx3, plainText5, plainText6, cipherText5, cipherText6);
	ret |= rijndael256_ct64_enc_x2_x2(&ctx->ctx4, plainText7, plainText8, cipherText7, cipherText8);
	return ret;
}


#else /* */
/*
 * Dummy definition to avoid the empty translation unit ISO C warning
 */
typedef int dummy;
#endif /* RIJNDAEL_BITSLICE */

