#ifndef __RIJNDAEL_AES_NI_H__
#define __RIJNDAEL_AES_NI_H__

/* First of all check for the support of AES-NI */
#ifndef __AES__
#error "Error: asking for RIJNDAEL_AES_NI while AES-NI support is not detected ... Please check your platform and/or compilation options!"
#endif /* __AES__ */

#include "rijndael_common.h"

/* Deal with namespacing */
#define aes128_aes_ni_setkey_enc MQOM_NAMESPACE(aes128_aes_ni_setkey_enc)
#define aes256_aes_ni_setkey_enc MQOM_NAMESPACE(aes256_aes_ni_setkey_enc)
#define rijndael256_aes_ni_setkey_enc MQOM_NAMESPACE(rijndael256_aes_ni_setkey_enc)
#define aes128_aes_ni_enc MQOM_NAMESPACE(aes128_aes_ni_enc)
#define aes256_aes_ni_enc MQOM_NAMESPACE(aes256_aes_ni_enc)
#define rijndael256_aes_ni_enc MQOM_NAMESPACE(rijndael256_aes_ni_enc)
#define aes128_aes_ni_enc_x2 MQOM_NAMESPACE(aes128_aes_ni_enc_x2)
#define aes128_aes_ni_enc_x4 MQOM_NAMESPACE(aes128_aes_ni_enc_x4)
#define aes128_aes_ni_enc_x8 MQOM_NAMESPACE(aes128_aes_ni_enc_x8)
#define aes256_aes_ni_enc_x2 MQOM_NAMESPACE(aes256_aes_ni_enc_x2)
#define aes256_aes_ni_enc_x4 MQOM_NAMESPACE(aes256_aes_ni_enc_x4)
#define aes256_aes_ni_enc_x8 MQOM_NAMESPACE(aes256_aes_ni_enc_x8)
#define rijndael256_aes_ni_enc_x2 MQOM_NAMESPACE(rijndael256_aes_ni_enc_x2)
#define rijndael256_aes_ni_enc_x4 MQOM_NAMESPACE(rijndael256_aes_ni_enc_x4)
#define rijndael256_aes_ni_enc_x8 MQOM_NAMESPACE(rijndael256_aes_ni_enc_x8)
/**/
#define aes128_aes_ni_setkey_enc_x2 MQOM_NAMESPACE(aes128_aes_ni_setkey_enc_x2)
#define aes256_aes_ni_setkey_enc_x2 MQOM_NAMESPACE(aes256_aes_ni_setkey_enc_x2)
#define rijndael256_aes_ni_setkey_enc_x2 MQOM_NAMESPACE(rijndael256_aes_ni_setkey_enc_x2)
#define aes128_aes_ni_setkey_enc_x4 MQOM_NAMESPACE(aes128_aes_ni_setkey_enc_x4)
#define aes256_aes_ni_setkey_enc_x4 MQOM_NAMESPACE(aes256_aes_ni_setkey_enc_x4)
#define rijndael256_aes_ni_setkey_enc_x4 MQOM_NAMESPACE(rijndael256_aes_ni_setkey_enc_x4)
#define aes128_aes_ni_setkey_enc_x8 MQOM_NAMESPACE(aes128_aes_ni_setkey_enc_x8)
#define aes256_aes_ni_setkey_enc_x8 MQOM_NAMESPACE(aes256_aes_ni_setkey_enc_x8)
#define rijndael256_aes_ni_setkey_enc_x8 MQOM_NAMESPACE(rijndael256_aes_ni_setkey_enc_x8)
#define aes128_aes_ni_enc_x2_x2 MQOM_NAMESPACE(aes128_aes_ni_enc_x2_x2)
#define aes128_aes_ni_enc_x4_x4 MQOM_NAMESPACE(aes128_aes_ni_enc_x4_x4)
#define aes128_aes_ni_enc_x8_x8 MQOM_NAMESPACE(aes128_aes_ni_enc_x8_x8)
#define aes256_aes_ni_enc_x2_x2 MQOM_NAMESPACE(aes256_aes_ni_enc_x2_x2)
#define aes256_aes_ni_enc_x4_x4 MQOM_NAMESPACE(aes256_aes_ni_enc_x4_x4)
#define aes256_aes_ni_enc_x8_x8 MQOM_NAMESPACE(aes256_aes_ni_enc_x8_x8)
#define rijndael256_aes_ni_enc_x2_x2 MQOM_NAMESPACE(rijndael256_aes_ni_enc_x2_x2)
#define rijndael256_aes_ni_enc_x4_x4 MQOM_NAMESPACE(rijndael256_aes_ni_enc_x4_x4)
#define rijndael256_aes_ni_enc_x8_x8 MQOM_NAMESPACE(rijndael256_aes_ni_enc_x8_x8)

/* Header for AES-NI intrinsics */
#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

/* The general Rijndael core context structure */
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	/* AES-NI context using aligned __m128i */
	/* NOTE: __m128i type should align on 16 bytes and avoid unalignment issues with xmm memory accesses */
	__m128i rk[11]; /* Round keys */
} rijndael_aes_ni_ctx_aes128;
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	/* AES-NI context using aligned __m128i */
	/* NOTE: __m128i type should align on 16 bytes and avoid unalignment issues with xmm memory accesses */
	__m128i rk[15]; /* Round keys */
} rijndael_aes_ni_ctx_aes256;
typedef struct {
	rijndael_type rtype; /* Type of Rijndael */
	/* AES-NI context using aligned __m128i */
	/* NOTE: __m128i type should align on 16 bytes and avoid unalignment issues with xmm memory accesses */
	__m128i rk[30]; /* Round keys */
} rijndael_aes_ni_ctx_rijndael256;

/* x2, x4 and x8 contexts */
MAKE_GENERIC_CTX_XX(aes128, aes_ni)
MAKE_GENERIC_CTX_XX(aes256, aes_ni)
MAKE_GENERIC_CTX_XX(rijndael256, aes_ni)

/* ==== Public API ==== */
int aes128_aes_ni_setkey_enc(rijndael_aes_ni_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_aes_ni_setkey_enc(rijndael_aes_ni_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_aes_ni_setkey_enc(rijndael_aes_ni_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_aes_ni_enc(const rijndael_aes_ni_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_aes_ni_enc(const rijndael_aes_ni_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_aes_ni_enc(const rijndael_aes_ni_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2 and x4 encryption APIs */
int aes128_aes_ni_enc_x2(const rijndael_aes_ni_ctx_aes128 *ctx1, const rijndael_aes_ni_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_aes_ni_enc_x4(const rijndael_aes_ni_ctx_aes128 *ctx1, const rijndael_aes_ni_ctx_aes128 *ctx2, const rijndael_aes_ni_ctx_aes128 *ctx3, const rijndael_aes_ni_ctx_aes128 *ctx4,
                         const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                         uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_aes_ni_enc_x8(const rijndael_aes_ni_ctx_aes128 *ctx1, const rijndael_aes_ni_ctx_aes128 *ctx2, const rijndael_aes_ni_ctx_aes128 *ctx3, const rijndael_aes_ni_ctx_aes128 *ctx4,
                         const rijndael_aes_ni_ctx_aes128 *ctx5, const rijndael_aes_ni_ctx_aes128 *ctx6, const rijndael_aes_ni_ctx_aes128 *ctx7, const rijndael_aes_ni_ctx_aes128 *ctx8,
                         const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                         const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                         uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                         uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_aes_ni_enc_x2(const rijndael_aes_ni_ctx_aes256 *ctx1, const rijndael_aes_ni_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_aes_ni_enc_x4(const rijndael_aes_ni_ctx_aes256 *ctx1, const rijndael_aes_ni_ctx_aes256 *ctx2, const rijndael_aes_ni_ctx_aes256 *ctx3, const rijndael_aes_ni_ctx_aes256 *ctx4,
                         const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                         uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_aes_ni_enc_x8(const rijndael_aes_ni_ctx_aes256 *ctx1, const rijndael_aes_ni_ctx_aes256 *ctx2, const rijndael_aes_ni_ctx_aes256 *ctx3, const rijndael_aes_ni_ctx_aes256 *ctx4,
                         const rijndael_aes_ni_ctx_aes256 *ctx5, const rijndael_aes_ni_ctx_aes256 *ctx6, const rijndael_aes_ni_ctx_aes256 *ctx7, const rijndael_aes_ni_ctx_aes256 *ctx8,
                         const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                         const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                         uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                         uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int rijndael256_aes_ni_enc_x2(const rijndael_aes_ni_ctx_rijndael256 *ctx1, const rijndael_aes_ni_ctx_rijndael256 *ctx2,
                              const uint8_t plainText1[32], const uint8_t plainText2[32],
                              uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_aes_ni_enc_x4(const rijndael_aes_ni_ctx_rijndael256 *ctx1, const rijndael_aes_ni_ctx_rijndael256 *ctx2, const rijndael_aes_ni_ctx_rijndael256 *ctx3, const rijndael_aes_ni_ctx_rijndael256 *ctx4,
                              const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                              uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_aes_ni_enc_x8(const rijndael_aes_ni_ctx_rijndael256 *ctx1, const rijndael_aes_ni_ctx_rijndael256 *ctx2, const rijndael_aes_ni_ctx_rijndael256 *ctx3, const rijndael_aes_ni_ctx_rijndael256 *ctx4,
                              const rijndael_aes_ni_ctx_rijndael256 *ctx5, const rijndael_aes_ni_ctx_rijndael256 *ctx6, const rijndael_aes_ni_ctx_rijndael256 *ctx7, const rijndael_aes_ni_ctx_rijndael256 *ctx8,
                              const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                              const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                              uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                              uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);

MAKE_GENERIC_FUNCS_XX_DECL(aes128, aes_ni, 16, 16)
MAKE_GENERIC_FUNCS_XX_DECL(aes256, aes_ni, 32, 16)
MAKE_GENERIC_FUNCS_XX_DECL(rijndael256, aes_ni, 32, 32)

#endif /* __RIJNDAEL_AES_NI_H__ */
