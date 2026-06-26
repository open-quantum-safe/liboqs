#include "rijndael_platform.h"

#if defined(RIJNDAEL_EXTERNAL)
#include "rijndael_external.h"


/* ==== Public APIs ===== */

/* XXX: NOTE: the following implementations are "weak" placeholders for
 * external implementations provided by the user */

#ifdef PANIC
#undef PANIC
#endif
#define PANIC() do { \
	while(1){}; \
} while(0)

WEAK int aes128_external_setkey_enc(rijndael_external_ctx_aes128 *ctx, const uint8_t key[16]) {
	(void)ctx;
	(void)key;
	PANIC();
	return -1;
}

WEAK int aes256_external_setkey_enc(rijndael_external_ctx_aes256 *ctx, const uint8_t key[32]) {
	(void)ctx;
	(void)key;
	PANIC();
	return -1;
}

WEAK int rijndael256_external_setkey_enc(rijndael_external_ctx_rijndael256 *ctx, const uint8_t key[32]) {
	(void)ctx;
	(void)key;
	PANIC();
	return -1;
}

WEAK int aes128_external_enc(const rijndael_external_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	(void)ctx;
	(void)data_in;
	(void)data_out;
	PANIC();
	return -1;
}

WEAK int aes128_external_enc_x2(const rijndael_external_ctx_aes128 *ctx1, const rijndael_external_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	(void)ctx1;
	(void)ctx2;
	(void)plainText1;
	(void)plainText2;
	(void)cipherText1;
	(void)cipherText2;
	PANIC();
	return -1;
}

WEAK int aes128_external_enc_x4(const rijndael_external_ctx_aes128 *ctx1, const rijndael_external_ctx_aes128 *ctx2, const rijndael_external_ctx_aes128 *ctx3, const rijndael_external_ctx_aes128 *ctx4,
                             const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                             uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	(void)ctx1;
	(void)ctx2;
	(void)ctx3;
	(void)ctx4;
	(void)plainText1;
	(void)plainText2;
	(void)plainText3;
	(void)plainText4;
	(void)cipherText1;
	(void)cipherText2;
	(void)cipherText3;
	(void)cipherText4;
	PANIC();
	return -1;
}

WEAK int aes128_external_enc_x8(const rijndael_external_ctx_aes128 *ctx1, const rijndael_external_ctx_aes128 *ctx2, const rijndael_external_ctx_aes128 *ctx3, const rijndael_external_ctx_aes128 *ctx4,
                             const rijndael_external_ctx_aes128 *ctx5, const rijndael_external_ctx_aes128 *ctx6, const rijndael_external_ctx_aes128 *ctx7, const rijndael_external_ctx_aes128 *ctx8,
                             const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                             const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                             uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                             uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	(void)ctx1;
	(void)ctx2;
	(void)ctx3;
	(void)ctx4;
	(void)ctx5;
	(void)ctx6;
	(void)ctx7;
	(void)ctx8;
	(void)plainText1;
	(void)plainText2;
	(void)plainText3;
	(void)plainText4;
	(void)plainText5;
	(void)plainText6;
	(void)plainText7;
	(void)plainText8;
	(void)cipherText1;
	(void)cipherText2;
	(void)cipherText3;
	(void)cipherText4;
	(void)cipherText5;
	(void)cipherText6;
	(void)cipherText7;
	(void)cipherText8;
	PANIC();
	return -1;
}

WEAK int aes256_external_enc(const rijndael_external_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]) {
	(void)ctx;
	(void)data_in;
	(void)data_out;
	PANIC();
	return -1;
}

WEAK int aes256_external_enc_x2(const rijndael_external_ctx_aes256 *ctx1, const rijndael_external_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	(void)ctx1;
	(void)ctx2;
	(void)plainText1;
	(void)plainText2;
	(void)cipherText1;
	(void)cipherText2;
	PANIC();
	return -1;
}

WEAK int aes256_external_enc_x4(const rijndael_external_ctx_aes256 *ctx1, const rijndael_external_ctx_aes256 *ctx2, const rijndael_external_ctx_aes256 *ctx3, const rijndael_external_ctx_aes256 *ctx4,
                             const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                             uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	(void)ctx1;
	(void)ctx2;
	(void)ctx3;
	(void)ctx4;
	(void)plainText1;
	(void)plainText2;
	(void)plainText3;
	(void)plainText4;
	(void)cipherText1;
	(void)cipherText2;
	(void)cipherText3;
	(void)cipherText4;
	PANIC();
	return -1;
}

WEAK int aes256_external_enc_x8(const rijndael_external_ctx_aes256 *ctx1, const rijndael_external_ctx_aes256 *ctx2, const rijndael_external_ctx_aes256 *ctx3, const rijndael_external_ctx_aes256 *ctx4,
                             const rijndael_external_ctx_aes256 *ctx5, const rijndael_external_ctx_aes256 *ctx6, const rijndael_external_ctx_aes256 *ctx7, const rijndael_external_ctx_aes256 *ctx8,
                             const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                             const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                             uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                             uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	(void)ctx1;
	(void)ctx2;
	(void)ctx3;
	(void)ctx4;
	(void)ctx5;
	(void)ctx6;
	(void)ctx7;
	(void)ctx8;
	(void)plainText1;
	(void)plainText2;
	(void)plainText3;
	(void)plainText4;
	(void)plainText5;
	(void)plainText6;
	(void)plainText7;
	(void)plainText8;
	(void)cipherText1;
	(void)cipherText2;
	(void)cipherText3;
	(void)cipherText4;
	(void)cipherText5;
	(void)cipherText6;
	(void)cipherText7;
	(void)cipherText8;
	PANIC();
	return -1;
}

WEAK int rijndael256_external_enc(const rijndael_external_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]) {
	(void)ctx;
	(void)data_in;
	(void)data_out;
	PANIC();
	return -1;
}

WEAK int rijndael256_external_enc_x2(const rijndael_external_ctx_rijndael256 *ctx1, const rijndael_external_ctx_rijndael256 *ctx2,
                                  const uint8_t plainText1[32], const uint8_t plainText2[32],
                                  uint8_t cipherText1[32], uint8_t cipherText2[32]) {
	(void)ctx1;
	(void)ctx2;
	(void)plainText1;
	(void)plainText2;
	(void)cipherText1;
	(void)cipherText2;
	PANIC();
	return -1;
}


WEAK int rijndael256_external_enc_x4(const rijndael_external_ctx_rijndael256 *ctx1, const rijndael_external_ctx_rijndael256 *ctx2, const rijndael_external_ctx_rijndael256 *ctx3, const rijndael_external_ctx_rijndael256 *ctx4,
                                  const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                  uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]) {
	(void)ctx1;
	(void)ctx2;
	(void)ctx3;
	(void)ctx4;
	(void)plainText1;
	(void)plainText2;
	(void)plainText3;
	(void)plainText4;
	(void)cipherText1;
	(void)cipherText2;
	(void)cipherText3;
	(void)cipherText4;
	PANIC();
	return -1;
}

WEAK int rijndael256_external_enc_x8(const rijndael_external_ctx_rijndael256 *ctx1, const rijndael_external_ctx_rijndael256 *ctx2, const rijndael_external_ctx_rijndael256 *ctx3, const rijndael_external_ctx_rijndael256 *ctx4,
                                  const rijndael_external_ctx_rijndael256 *ctx5, const rijndael_external_ctx_rijndael256 *ctx6, const rijndael_external_ctx_rijndael256 *ctx7, const rijndael_external_ctx_rijndael256 *ctx8,
                                  const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                                  const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                                  uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                                  uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]) {
	(void)ctx1;
	(void)ctx2;
	(void)ctx3;
	(void)ctx4;
	(void)ctx5;
	(void)ctx6;
	(void)ctx7;
	(void)ctx8;
	(void)plainText1;
	(void)plainText2;
	(void)plainText3;
	(void)plainText4;
	(void)plainText5;
	(void)plainText6;
	(void)plainText7;
	(void)plainText8;
	(void)cipherText1;
	(void)cipherText2;
	(void)cipherText3;
	(void)cipherText4;
	(void)cipherText5;
	(void)cipherText6;
	(void)cipherText7;
	(void)cipherText8;
	PANIC();
	return -1;
}

MAKE_GENERIC_FUNCS_XX_IMPL(aes128, external, 16, 16)
MAKE_GENERIC_FUNCS_XX_IMPL(aes256, external, 32, 16)
MAKE_GENERIC_FUNCS_XX_IMPL(rijndael256, external, 32, 32)

#else /* */
/*
 * Dummy definition to avoid the empty translation unit ISO C warning
 */
typedef int dummy;
#endif
