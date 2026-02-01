#ifndef __RIJNDAEL_EXTERNAL_H__
#define __RIJNDAEL_EXTERNAL_H__

#include "rijndael_common.h"

/* Deal with namespacing */
#define aes128_external_setkey_enc MQOM_NAMESPACE(aes128_external_setkey_enc)
#define aes256_external_setkey_enc MQOM_NAMESPACE(aes256_external_setkey_enc)
#define rijndael256_external_setkey_enc MQOM_NAMESPACE(rijndael256_external_setkey_enc)
#define aes128_external_enc MQOM_NAMESPACE(aes128_external_enc)
#define aes256_external_enc MQOM_NAMESPACE(aes256_external_enc)
#define rijndael256_external_enc MQOM_NAMESPACE(rijndael256_external_enc)
#define aes128_external_enc_x2 MQOM_NAMESPACE(aes128_external_enc_x2)
#define aes128_external_enc_x4 MQOM_NAMESPACE(aes128_external_enc_x4)
#define aes128_external_enc_x8 MQOM_NAMESPACE(aes128_external_enc_x8)
#define aes256_external_enc_x2 MQOM_NAMESPACE(aes256_external_enc_x2)
#define aes256_external_enc_x4 MQOM_NAMESPACE(aes256_external_enc_x4)
#define aes256_external_enc_x8 MQOM_NAMESPACE(aes256_external_enc_x8)
#define rijndael256_external_enc_x2 MQOM_NAMESPACE(rijndael256_external_enc_x2)
#define rijndael256_external_enc_x4 MQOM_NAMESPACE(rijndael256_external_enc_x4)
#define rijndael256_external_enc_x8 MQOM_NAMESPACE(rijndael256_external_enc_x8)
/**/
#define aes128_external_setkey_enc_x2 MQOM_NAMESPACE(aes128_external_setkey_enc_x2)
#define aes256_external_setkey_enc_x2 MQOM_NAMESPACE(aes256_external_setkey_enc_x2)
#define rijndael256_external_setkey_enc_x2 MQOM_NAMESPACE(rijndael256_external_setkey_enc_x2)
#define aes128_external_setkey_enc_x4 MQOM_NAMESPACE(aes128_external_setkey_enc_x4)
#define aes256_external_setkey_enc_x4 MQOM_NAMESPACE(aes256_external_setkey_enc_x4)
#define rijndael256_external_setkey_enc_x4 MQOM_NAMESPACE(rijndael256_external_setkey_enc_x4)
#define aes128_external_setkey_enc_x8 MQOM_NAMESPACE(aes128_external_setkey_enc_x8)
#define aes256_external_setkey_enc_x8 MQOM_NAMESPACE(aes256_external_setkey_enc_x8)
#define rijndael256_external_setkey_enc_x8 MQOM_NAMESPACE(rijndael256_external_setkey_enc_x8)
#define aes128_external_enc_x2_x2 MQOM_NAMESPACE(aes128_external_enc_x2_x2)
#define aes128_external_enc_x4_x4 MQOM_NAMESPACE(aes128_external_enc_x4_x4)
#define aes128_external_enc_x8_x8 MQOM_NAMESPACE(aes128_external_enc_x8_x8)
#define aes256_external_enc_x2_x2 MQOM_NAMESPACE(aes256_external_enc_x2_x2)
#define aes256_external_enc_x4_x4 MQOM_NAMESPACE(aes256_external_enc_x4_x4)
#define aes256_external_enc_x8_x8 MQOM_NAMESPACE(aes256_external_enc_x8_x8)
#define rijndael256_external_enc_x2_x2 MQOM_NAMESPACE(rijndael256_external_enc_x2_x2)
#define rijndael256_external_enc_x4_x4 MQOM_NAMESPACE(rijndael256_external_enc_x4_x4)
#define rijndael256_external_enc_x8_x8 MQOM_NAMESPACE(rijndael256_external_enc_x8_x8)

/* The general Rijndael core context structure: this contains a pointer to an
 * "opaque" structure defined elsewhere */
typedef struct {
	void *opaque;
} rijndael_external_ctx_aes128;
typedef struct {
	void *opaque;
} rijndael_external_ctx_aes256;
typedef struct {
	void *opaque;
} rijndael_external_ctx_rijndael256;

/* x2, x4 and x8 contexts */
MAKE_GENERIC_CTX_XX(aes128, external)
MAKE_GENERIC_CTX_XX(aes256, external)
MAKE_GENERIC_CTX_XX(rijndael256, external)

/* ==== Public API ==== */
int aes128_external_setkey_enc(rijndael_external_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_external_setkey_enc(rijndael_external_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_external_setkey_enc(rijndael_external_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_external_enc(const rijndael_external_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_external_enc(const rijndael_external_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_external_enc(const rijndael_external_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* x2 and x4 encryption APIs */
int aes128_external_enc_x2(const rijndael_external_ctx_aes128 *ctx1, const rijndael_external_ctx_aes128 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_external_enc_x4(const rijndael_external_ctx_aes128 *ctx1, const rijndael_external_ctx_aes128 *ctx2, const rijndael_external_ctx_aes128 *ctx3, const rijndael_external_ctx_aes128 *ctx4,
                        const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                        uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_external_enc_x8(const rijndael_external_ctx_aes128 *ctx1, const rijndael_external_ctx_aes128 *ctx2, const rijndael_external_ctx_aes128 *ctx3, const rijndael_external_ctx_aes128 *ctx4,
                        const rijndael_external_ctx_aes128 *ctx5, const rijndael_external_ctx_aes128 *ctx6, const rijndael_external_ctx_aes128 *ctx7, const rijndael_external_ctx_aes128 *ctx8,
                        const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                        const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                        uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                        uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_external_enc_x2(const rijndael_external_ctx_aes256 *ctx1, const rijndael_external_ctx_aes256 *ctx2, const uint8_t plainText1[16], const uint8_t plainText2[16], uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_external_enc_x4(const rijndael_external_ctx_aes256 *ctx1, const rijndael_external_ctx_aes256 *ctx2, const rijndael_external_ctx_aes256 *ctx3, const rijndael_external_ctx_aes256 *ctx4,
                        const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                        uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_external_enc_x8(const rijndael_external_ctx_aes256 *ctx1, const rijndael_external_ctx_aes256 *ctx2, const rijndael_external_ctx_aes256 *ctx3, const rijndael_external_ctx_aes256 *ctx4,
                        const rijndael_external_ctx_aes256 *ctx5, const rijndael_external_ctx_aes256 *ctx6, const rijndael_external_ctx_aes256 *ctx7, const rijndael_external_ctx_aes256 *ctx8,
                        const uint8_t plainText1[16], const uint8_t plainText2[16], const uint8_t plainText3[16], const uint8_t plainText4[16],
                        const uint8_t plainText5[16], const uint8_t plainText6[16], const uint8_t plainText7[16], const uint8_t plainText8[16],
                        uint8_t cipherText1[16], uint8_t cipherText2[16], uint8_t cipherText3[16], uint8_t cipherText4[16],
                        uint8_t cipherText5[16], uint8_t cipherText6[16], uint8_t cipherText7[16], uint8_t cipherText8[16]);
int rijndael256_external_enc_x2(const rijndael_external_ctx_rijndael256 *ctx1, const rijndael_external_ctx_rijndael256 *ctx2,
                             const uint8_t plainText1[32], const uint8_t plainText2[32],
                             uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_external_enc_x4(const rijndael_external_ctx_rijndael256 *ctx1, const rijndael_external_ctx_rijndael256 *ctx2, const rijndael_external_ctx_rijndael256 *ctx3, const rijndael_external_ctx_rijndael256 *ctx4,
                             const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                             uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_external_enc_x8(const rijndael_external_ctx_rijndael256 *ctx1, const rijndael_external_ctx_rijndael256 *ctx2, const rijndael_external_ctx_rijndael256 *ctx3, const rijndael_external_ctx_rijndael256 *ctx4,
                             const rijndael_external_ctx_rijndael256 *ctx5, const rijndael_external_ctx_rijndael256 *ctx6, const rijndael_external_ctx_rijndael256 *ctx7, const rijndael_external_ctx_rijndael256 *ctx8,
                             const uint8_t plainText1[32], const uint8_t plainText2[32], const uint8_t plainText3[32], const uint8_t plainText4[32],
                             const uint8_t plainText5[32], const uint8_t plainText6[32], const uint8_t plainText7[32], const uint8_t plainText8[32],
                             uint8_t cipherText1[32], uint8_t cipherText2[32], uint8_t cipherText3[32], uint8_t cipherText4[32],
                             uint8_t cipherText5[32], uint8_t cipherText6[32], uint8_t cipherText7[32], uint8_t cipherText8[32]);

MAKE_GENERIC_FUNCS_XX_DECL(aes128, external, 16, 16)
MAKE_GENERIC_FUNCS_XX_DECL(aes256, external, 32, 16)
MAKE_GENERIC_FUNCS_XX_DECL(rijndael256, external, 32, 32)

#endif /* __RIJNDAEL_EXTERNAL_H__ */
