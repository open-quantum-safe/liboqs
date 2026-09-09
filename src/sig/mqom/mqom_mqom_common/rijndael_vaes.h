#ifndef __RIJNDAEL_VAES_H__
#define __RIJNDAEL_VAES_H__

#include "rijndael_platform.h"
#ifdef RIJNDAEL_VAES

/* VAES uses scalar AES-NI instructions for key schedule and single-block fallback.
 * Any CPU supporting VAES also supports AES-NI, so this is always satisfied. */
#ifndef __AES__
#error "Error: RIJNDAEL_VAES requires AES-NI support (__AES__) for key schedule. Please check your platform and/or compilation options!"
#endif

#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include "rijndael_common.h"

/* Deal with namespacing */
#define aes128_vaes_setkey_enc MQOM_NAMESPACE(aes128_vaes_setkey_enc)
#define aes256_vaes_setkey_enc MQOM_NAMESPACE(aes256_vaes_setkey_enc)
#define rijndael256_vaes_setkey_enc MQOM_NAMESPACE(rijndael256_vaes_setkey_enc)
#define aes128_vaes_enc MQOM_NAMESPACE(aes128_vaes_enc)
#define aes256_vaes_enc MQOM_NAMESPACE(aes256_vaes_enc)
#define rijndael256_vaes_enc MQOM_NAMESPACE(rijndael256_vaes_enc)
#define aes128_vaes_enc_x2 MQOM_NAMESPACE(aes128_vaes_enc_x2)
#define aes128_vaes_enc_x4 MQOM_NAMESPACE(aes128_vaes_enc_x4)
#define aes128_vaes_enc_x8 MQOM_NAMESPACE(aes128_vaes_enc_x8)
#define aes256_vaes_enc_x2 MQOM_NAMESPACE(aes256_vaes_enc_x2)
#define aes256_vaes_enc_x4 MQOM_NAMESPACE(aes256_vaes_enc_x4)
#define aes256_vaes_enc_x8 MQOM_NAMESPACE(aes256_vaes_enc_x8)
#define rijndael256_vaes_enc_x2 MQOM_NAMESPACE(rijndael256_vaes_enc_x2)
#define rijndael256_vaes_enc_x4 MQOM_NAMESPACE(rijndael256_vaes_enc_x4)
#define rijndael256_vaes_enc_x8 MQOM_NAMESPACE(rijndael256_vaes_enc_x8)
/**/
#define aes128_vaes_setkey_enc_x2 MQOM_NAMESPACE(aes128_vaes_setkey_enc_x2)
#define aes256_vaes_setkey_enc_x2 MQOM_NAMESPACE(aes256_vaes_setkey_enc_x2)
#define rijndael256_vaes_setkey_enc_x2 MQOM_NAMESPACE(rijndael256_vaes_setkey_enc_x2)
#define aes128_vaes_setkey_enc_x4 MQOM_NAMESPACE(aes128_vaes_setkey_enc_x4)
#define aes256_vaes_setkey_enc_x4 MQOM_NAMESPACE(aes256_vaes_setkey_enc_x4)
#define rijndael256_vaes_setkey_enc_x4 MQOM_NAMESPACE(rijndael256_vaes_setkey_enc_x4)
#define aes128_vaes_setkey_enc_x8 MQOM_NAMESPACE(aes128_vaes_setkey_enc_x8)
#define aes256_vaes_setkey_enc_x8 MQOM_NAMESPACE(aes256_vaes_setkey_enc_x8)
#define rijndael256_vaes_setkey_enc_x8 MQOM_NAMESPACE(rijndael256_vaes_setkey_enc_x8)
#define aes128_vaes_enc_x2_x2 MQOM_NAMESPACE(aes128_vaes_enc_x2_x2)
#define aes128_vaes_enc_x4_x4 MQOM_NAMESPACE(aes128_vaes_enc_x4_x4)
#define aes128_vaes_enc_x8_x8 MQOM_NAMESPACE(aes128_vaes_enc_x8_x8)
#define aes256_vaes_enc_x2_x2 MQOM_NAMESPACE(aes256_vaes_enc_x2_x2)
#define aes256_vaes_enc_x4_x4 MQOM_NAMESPACE(aes256_vaes_enc_x4_x4)
#define aes256_vaes_enc_x8_x8 MQOM_NAMESPACE(aes256_vaes_enc_x8_x8)
#define rijndael256_vaes_enc_x2_x2 MQOM_NAMESPACE(rijndael256_vaes_enc_x2_x2)
#define rijndael256_vaes_enc_x4_x4 MQOM_NAMESPACE(rijndael256_vaes_enc_x4_x4)
#define rijndael256_vaes_enc_x8_x8 MQOM_NAMESPACE(rijndael256_vaes_enc_x8_x8)
/**/
#define aes128_vaes_setkey_enc_ecb MQOM_NAMESPACE(aes128_vaes_setkey_enc_ecb)
#define aes256_vaes_setkey_enc_ecb MQOM_NAMESPACE(aes256_vaes_setkey_enc_ecb)
#define rijndael256_vaes_setkey_enc_ecb MQOM_NAMESPACE(rijndael256_vaes_setkey_enc_ecb)
#define aes128_vaes_enc_ecb MQOM_NAMESPACE(aes128_vaes_enc_ecb)
#define aes256_vaes_enc_ecb MQOM_NAMESPACE(aes256_vaes_enc_ecb)
#define rijndael256_vaes_enc_ecb MQOM_NAMESPACE(rijndael256_vaes_enc_ecb)

/* VAES context types: own structs, independent from AES-NI (same layout) */
typedef struct {
	rijndael_type rtype;
	__m128i rk[11];
} rijndael_vaes_ctx_aes128;
typedef struct {
	rijndael_type rtype;
	__m128i rk[15];
} rijndael_vaes_ctx_aes256;
typedef struct {
	rijndael_type rtype;
	__m128i rk[30];
} rijndael_vaes_ctx_rijndael256;

/* x2, x4 and x8 contexts */
#ifdef RIJNDAEL_VAES256
/* Round keys for two independent AES-128 keys, packed one key per YMM
 * lane. Keep the same size and minimum alignment as two generic scalar
 * contexts, so this remains an internal representation change. */
typedef struct RIJNDAEL_ALIGN(16) {
	rijndael_type rtype;
	uint8_t padding[28];
	uint8_t rk[11][32];
} rijndael_vaes_ctx_aes128_x2;
typedef struct {
	rijndael_vaes_ctx_aes128_x2 ctx[2];
} rijndael_vaes_ctx_aes128_x4;
typedef struct {
	rijndael_vaes_ctx_aes128_x2 ctx[4];
} rijndael_vaes_ctx_aes128_x8;
#else
MAKE_GENERIC_CTX_XX(aes128, vaes)
#endif
MAKE_GENERIC_CTX_XX(aes256, vaes)
#ifdef RIJNDAEL_VAES256
/* Pairwise transposed round keys for independent-key Rijndael-256.
 * Each rk[i] contains the same 128-bit round-key half for two keys. Byte
 * storage plus explicit unaligned vector accesses retain the former
 * context's 16-byte alignment as well as its size. */
typedef struct RIJNDAEL_ALIGN(16) {
	rijndael_type rtype;
	uint8_t padding[28];
	uint8_t rk[30][32];
} rijndael_vaes_ctx_rijndael256_x2;
typedef struct {
	rijndael_vaes_ctx_rijndael256_x2 ctx[2];
} rijndael_vaes_ctx_rijndael256_x4;
typedef struct {
	rijndael_vaes_ctx_rijndael256_x2 ctx[4];
} rijndael_vaes_ctx_rijndael256_x8;

/* Byte indices for the optional AVX-512 VBMI ShiftRows compensation.
 * Kept as shared lists so their scalar equivalence can be unit-tested even
 * on VAES-256 machines without AVX-512. */
#define RIJNDAEL256_VBMI_AOS_IDX \
	 0, 17, 22, 23,  4,  5, 26, 27,  8,  9, 14, 31, 12, 13, 18, 19, \
	16,  1,  6,  7, 20, 21, 10, 11, 24, 25, 30, 15, 28, 29,  2,  3
#define RIJNDAEL256_VBMI_PACKED_L_IDX \
	 0, 33, 38, 39,  4,  5, 42, 43,  8,  9, 14, 47, 12, 13, 34, 35, \
	16, 49, 54, 55, 20, 21, 58, 59, 24, 25, 30, 63, 28, 29, 50, 51
#define RIJNDAEL256_VBMI_PACKED_R_IDX \
	32,  1,  6,  7, 36, 37, 10, 11, 40, 41, 46, 15, 44, 45,  2,  3, \
	48, 17, 22, 23, 52, 53, 26, 27, 56, 57, 62, 31, 60, 61, 18, 19
#else
MAKE_GENERIC_CTX_XX(rijndael256, vaes)
#endif
/* ECB contexts */
MAKE_GENERIC_CTX_ECB(aes128, vaes)
MAKE_GENERIC_CTX_ECB(aes256, vaes)
MAKE_GENERIC_CTX_ECB(rijndael256, vaes)

/* ==== Public API ==== */
int aes128_vaes_setkey_enc(rijndael_vaes_ctx_aes128 *ctx, const uint8_t key[16]);
int aes256_vaes_setkey_enc(rijndael_vaes_ctx_aes256 *ctx, const uint8_t key[32]);
int rijndael256_vaes_setkey_enc(rijndael_vaes_ctx_rijndael256 *ctx, const uint8_t key[32]);
int aes128_vaes_enc(const rijndael_vaes_ctx_aes128 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int aes256_vaes_enc(const rijndael_vaes_ctx_aes256 *ctx, const uint8_t data_in[16], uint8_t data_out[16]);
int rijndael256_vaes_enc(const rijndael_vaes_ctx_rijndael256 *ctx, const uint8_t data_in[32], uint8_t data_out[32]);
/* enc_xN: N independent-key parallel encryptions (scalar AES-NI instructions) */
int aes128_vaes_enc_x2(const rijndael_vaes_ctx_aes128 *ctx1, const rijndael_vaes_ctx_aes128 *ctx2,
                       const uint8_t plainText1[16], const uint8_t plainText2[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes128_vaes_enc_x4(const rijndael_vaes_ctx_aes128 *ctx1, const rijndael_vaes_ctx_aes128 *ctx2,
                       const rijndael_vaes_ctx_aes128 *ctx3, const rijndael_vaes_ctx_aes128 *ctx4,
                       const uint8_t plainText1[16], const uint8_t plainText2[16],
                       const uint8_t plainText3[16], const uint8_t plainText4[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16],
                       uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes128_vaes_enc_x8(const rijndael_vaes_ctx_aes128 *ctx1, const rijndael_vaes_ctx_aes128 *ctx2,
                       const rijndael_vaes_ctx_aes128 *ctx3, const rijndael_vaes_ctx_aes128 *ctx4,
                       const rijndael_vaes_ctx_aes128 *ctx5, const rijndael_vaes_ctx_aes128 *ctx6,
                       const rijndael_vaes_ctx_aes128 *ctx7, const rijndael_vaes_ctx_aes128 *ctx8,
                       const uint8_t plainText1[16], const uint8_t plainText2[16],
                       const uint8_t plainText3[16], const uint8_t plainText4[16],
                       const uint8_t plainText5[16], const uint8_t plainText6[16],
                       const uint8_t plainText7[16], const uint8_t plainText8[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16],
                       uint8_t cipherText3[16], uint8_t cipherText4[16],
                       uint8_t cipherText5[16], uint8_t cipherText6[16],
                       uint8_t cipherText7[16], uint8_t cipherText8[16]);
int aes256_vaes_enc_x2(const rijndael_vaes_ctx_aes256 *ctx1, const rijndael_vaes_ctx_aes256 *ctx2,
                       const uint8_t plainText1[16], const uint8_t plainText2[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16]);
int aes256_vaes_enc_x4(const rijndael_vaes_ctx_aes256 *ctx1, const rijndael_vaes_ctx_aes256 *ctx2,
                       const rijndael_vaes_ctx_aes256 *ctx3, const rijndael_vaes_ctx_aes256 *ctx4,
                       const uint8_t plainText1[16], const uint8_t plainText2[16],
                       const uint8_t plainText3[16], const uint8_t plainText4[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16],
                       uint8_t cipherText3[16], uint8_t cipherText4[16]);
int aes256_vaes_enc_x8(const rijndael_vaes_ctx_aes256 *ctx1, const rijndael_vaes_ctx_aes256 *ctx2,
                       const rijndael_vaes_ctx_aes256 *ctx3, const rijndael_vaes_ctx_aes256 *ctx4,
                       const rijndael_vaes_ctx_aes256 *ctx5, const rijndael_vaes_ctx_aes256 *ctx6,
                       const rijndael_vaes_ctx_aes256 *ctx7, const rijndael_vaes_ctx_aes256 *ctx8,
                       const uint8_t plainText1[16], const uint8_t plainText2[16],
                       const uint8_t plainText3[16], const uint8_t plainText4[16],
                       const uint8_t plainText5[16], const uint8_t plainText6[16],
                       const uint8_t plainText7[16], const uint8_t plainText8[16],
                       uint8_t cipherText1[16], uint8_t cipherText2[16],
                       uint8_t cipherText3[16], uint8_t cipherText4[16],
                       uint8_t cipherText5[16], uint8_t cipherText6[16],
                       uint8_t cipherText7[16], uint8_t cipherText8[16]);
int rijndael256_vaes_enc_x2(const rijndael_vaes_ctx_rijndael256 *ctx1, const rijndael_vaes_ctx_rijndael256 *ctx2,
                            const uint8_t plainText1[32], const uint8_t plainText2[32],
                            uint8_t cipherText1[32], uint8_t cipherText2[32]);
int rijndael256_vaes_enc_x4(const rijndael_vaes_ctx_rijndael256 *ctx1, const rijndael_vaes_ctx_rijndael256 *ctx2,
                            const rijndael_vaes_ctx_rijndael256 *ctx3, const rijndael_vaes_ctx_rijndael256 *ctx4,
                            const uint8_t plainText1[32], const uint8_t plainText2[32],
                            const uint8_t plainText3[32], const uint8_t plainText4[32],
                            uint8_t cipherText1[32], uint8_t cipherText2[32],
                            uint8_t cipherText3[32], uint8_t cipherText4[32]);
int rijndael256_vaes_enc_x8(const rijndael_vaes_ctx_rijndael256 *ctx1, const rijndael_vaes_ctx_rijndael256 *ctx2,
                            const rijndael_vaes_ctx_rijndael256 *ctx3, const rijndael_vaes_ctx_rijndael256 *ctx4,
                            const rijndael_vaes_ctx_rijndael256 *ctx5, const rijndael_vaes_ctx_rijndael256 *ctx6,
                            const rijndael_vaes_ctx_rijndael256 *ctx7, const rijndael_vaes_ctx_rijndael256 *ctx8,
                            const uint8_t plainText1[32], const uint8_t plainText2[32],
                            const uint8_t plainText3[32], const uint8_t plainText4[32],
                            const uint8_t plainText5[32], const uint8_t plainText6[32],
                            const uint8_t plainText7[32], const uint8_t plainText8[32],
                            uint8_t cipherText1[32], uint8_t cipherText2[32],
                            uint8_t cipherText3[32], uint8_t cipherText4[32],
                            uint8_t cipherText5[32], uint8_t cipherText6[32],
                            uint8_t cipherText7[32], uint8_t cipherText8[32]);

MAKE_GENERIC_FUNCS_XX_DECL(aes128, vaes, 16, 16)
MAKE_GENERIC_FUNCS_XX_DECL(aes256, vaes, 32, 16)
MAKE_GENERIC_FUNCS_XX_DECL(rijndael256, vaes, 32, 32)

/* --- VAES-256 : 4xymm = 8 AES-128 or AES-256 blocks ------------------- */

/* AES-128 VAES-256 ECB x4-ymm (= 8 blocks): k[0..10] broadcast to 256-bit */
#define DO_ENC_BLOCK_128_128_VAES256_ECB_x4(m0,m1,m2,m3, k) \
    do { \
        __m256i _kr256; \
        _kr256 = _mm256_broadcastsi128_si256((k)[0]); \
        (m0) = _mm256_xor_si256((m0), _kr256); \
        (m1) = _mm256_xor_si256((m1), _kr256); \
        (m2) = _mm256_xor_si256((m2), _kr256); \
        (m3) = _mm256_xor_si256((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[1]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[2]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[3]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[4]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[5]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[6]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[7]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[8]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[9]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[10]); \
        (m0) = _mm256_aesenclast_epi128((m0), _kr256); \
        (m1) = _mm256_aesenclast_epi128((m1), _kr256); \
        (m2) = _mm256_aesenclast_epi128((m2), _kr256); \
        (m3) = _mm256_aesenclast_epi128((m3), _kr256); \
    } while(0)

/* AES-256 VAES-256 ECB x4-ymm (= 8 blocks): k[0..14] broadcast to 256-bit */
#define DO_ENC_BLOCK_128_256_VAES256_ECB_x4(m0,m1,m2,m3, k) \
    do { \
        __m256i _kr256; \
        _kr256 = _mm256_broadcastsi128_si256((k)[0]); \
        (m0) = _mm256_xor_si256((m0), _kr256); \
        (m1) = _mm256_xor_si256((m1), _kr256); \
        (m2) = _mm256_xor_si256((m2), _kr256); \
        (m3) = _mm256_xor_si256((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[1]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[2]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[3]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[4]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[5]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[6]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[7]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[8]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[9]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[10]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[11]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[12]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[13]); \
        (m0) = _mm256_aesenc_epi128((m0), _kr256); \
        (m1) = _mm256_aesenc_epi128((m1), _kr256); \
        (m2) = _mm256_aesenc_epi128((m2), _kr256); \
        (m3) = _mm256_aesenc_epi128((m3), _kr256); \
        _kr256 = _mm256_broadcastsi128_si256((k)[14]); \
        (m0) = _mm256_aesenclast_epi128((m0), _kr256); \
        (m1) = _mm256_aesenclast_epi128((m1), _kr256); \
        (m2) = _mm256_aesenclast_epi128((m2), _kr256); \
        (m3) = _mm256_aesenclast_epi128((m3), _kr256); \
    } while(0)

/* Rijndael-256 VAES-256: 4 ymm (2 pairs) = 4 blocks.
 * Layout: yml=[L0,L1] ymr=[R0,R1]  yml2=[L2,L3] ymr2=[R2,R3].
 * _mm256_blendv_epi8 / _mm256_shuffle_epi8 operate per 128-bit lane
 * independently  - exactly the same semantics as the xmm ShiftRows fix. */
#define RINJDAEL_COMPENSATE_SR_VAES256(yml, ymr, tyml, tymr, mb256, ms256) \
    do { \
        (tyml) = _mm256_blendv_epi8((yml), (ymr), (mb256)); \
        (tymr) = _mm256_blendv_epi8((ymr), (yml), (mb256)); \
        (tyml) = _mm256_shuffle_epi8((tyml), (ms256)); \
        (tymr) = _mm256_shuffle_epi8((tymr), (ms256)); \
    } while(0)

#define ENC_256_BLOCK_VAES256(yml, ymr, kl128, kr128, mb256, ms256) \
    do { \
        __m256i _tyml, _tymr; \
        RINJDAEL_COMPENSATE_SR_VAES256(yml, ymr, _tyml, _tymr, mb256, ms256); \
        (yml) = _mm256_aesenc_epi128(_tyml, _mm256_broadcastsi128_si256(kl128)); \
        (ymr) = _mm256_aesenc_epi128(_tymr, _mm256_broadcastsi128_si256(kr128)); \
    } while(0)

#define ENC_256_BLOCK_LAST_VAES256(yml, ymr, kl128, kr128, mb256, ms256) \
    do { \
        __m256i _tyml, _tymr; \
        RINJDAEL_COMPENSATE_SR_VAES256(yml, ymr, _tyml, _tymr, mb256, ms256); \
        (yml) = _mm256_aesenclast_epi128(_tyml, _mm256_broadcastsi128_si256(kl128)); \
        (ymr) = _mm256_aesenclast_epi128(_tymr, _mm256_broadcastsi128_si256(kr128)); \
    } while(0)

#define DO_ENC_BLOCK_256_256_VAES256_ECB_x4(yml, ymr, yml2, ymr2, k) \
    do { \
        const __m128i _mb = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000); \
        const __m128i _ms = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100); \
        __m256i _mb256 = _mm256_broadcastsi128_si256(_mb); \
        __m256i _ms256 = _mm256_broadcastsi128_si256(_ms); \
        __m256i _kl256 = _mm256_broadcastsi128_si256((k)[0]); \
        __m256i _kr256 = _mm256_broadcastsi128_si256((k)[1]); \
        (yml)  = _mm256_xor_si256((yml),  _kl256); \
        (ymr)  = _mm256_xor_si256((ymr),  _kr256); \
        (yml2) = _mm256_xor_si256((yml2), _kl256); \
        (ymr2) = _mm256_xor_si256((ymr2), _kr256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[2],  (k)[3],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[2],  (k)[3],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[4],  (k)[5],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[4],  (k)[5],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[6],  (k)[7],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[6],  (k)[7],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[8],  (k)[9],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[8],  (k)[9],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[10], (k)[11], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[10], (k)[11], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[12], (k)[13], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[12], (k)[13], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[14], (k)[15], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[14], (k)[15], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[16], (k)[17], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[16], (k)[17], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[18], (k)[19], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[18], (k)[19], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[20], (k)[21], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[20], (k)[21], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[22], (k)[23], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[22], (k)[23], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[24], (k)[25], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[24], (k)[25], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[26], (k)[27], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[26], (k)[27], _mb256, _ms256); \
        ENC_256_BLOCK_LAST_VAES256(yml,  ymr,  (k)[28], (k)[29], _mb256, _ms256); \
        ENC_256_BLOCK_LAST_VAES256(yml2, ymr2, (k)[28], (k)[29], _mb256, _ms256); \
    } while(0)

/* Rijndael-256/256 VAES-256 ECB x8 (8 blocks, 8 YMM pairs).
 * Each YMM pair (ymlN, ymrN) holds two blocks' left/right halves via _mm256_set_m128i. */
#define DO_ENC_BLOCK_256_256_VAES256_ECB_x8(yml, ymr, yml2, ymr2, yml3, ymr3, yml4, ymr4, k) \
    do { \
        const __m128i _mb = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000); \
        const __m128i _ms = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100); \
        __m256i _mb256 = _mm256_broadcastsi128_si256(_mb); \
        __m256i _ms256 = _mm256_broadcastsi128_si256(_ms); \
        __m256i _kl256 = _mm256_broadcastsi128_si256((k)[0]); \
        __m256i _kr256 = _mm256_broadcastsi128_si256((k)[1]); \
        (yml)  = _mm256_xor_si256((yml),  _kl256); \
        (ymr)  = _mm256_xor_si256((ymr),  _kr256); \
        (yml2) = _mm256_xor_si256((yml2), _kl256); \
        (ymr2) = _mm256_xor_si256((ymr2), _kr256); \
        (yml3) = _mm256_xor_si256((yml3), _kl256); \
        (ymr3) = _mm256_xor_si256((ymr3), _kr256); \
        (yml4) = _mm256_xor_si256((yml4), _kl256); \
        (ymr4) = _mm256_xor_si256((ymr4), _kr256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[2],  (k)[3],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[2],  (k)[3],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[2],  (k)[3],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[2],  (k)[3],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[4],  (k)[5],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[4],  (k)[5],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[4],  (k)[5],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[4],  (k)[5],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[6],  (k)[7],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[6],  (k)[7],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[6],  (k)[7],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[6],  (k)[7],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[8],  (k)[9],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[8],  (k)[9],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[8],  (k)[9],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[8],  (k)[9],  _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[10], (k)[11], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[10], (k)[11], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[10], (k)[11], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[10], (k)[11], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[12], (k)[13], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[12], (k)[13], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[12], (k)[13], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[12], (k)[13], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[14], (k)[15], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[14], (k)[15], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[14], (k)[15], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[14], (k)[15], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[16], (k)[17], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[16], (k)[17], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[16], (k)[17], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[16], (k)[17], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[18], (k)[19], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[18], (k)[19], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[18], (k)[19], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[18], (k)[19], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[20], (k)[21], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[20], (k)[21], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[20], (k)[21], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[20], (k)[21], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[22], (k)[23], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[22], (k)[23], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[22], (k)[23], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[22], (k)[23], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[24], (k)[25], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[24], (k)[25], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[24], (k)[25], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[24], (k)[25], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml,  ymr,  (k)[26], (k)[27], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml2, ymr2, (k)[26], (k)[27], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml3, ymr3, (k)[26], (k)[27], _mb256, _ms256); \
        ENC_256_BLOCK_VAES256(yml4, ymr4, (k)[26], (k)[27], _mb256, _ms256); \
        ENC_256_BLOCK_LAST_VAES256(yml,  ymr,  (k)[28], (k)[29], _mb256, _ms256); \
        ENC_256_BLOCK_LAST_VAES256(yml2, ymr2, (k)[28], (k)[29], _mb256, _ms256); \
        ENC_256_BLOCK_LAST_VAES256(yml3, ymr3, (k)[28], (k)[29], _mb256, _ms256); \
        ENC_256_BLOCK_LAST_VAES256(yml4, ymr4, (k)[28], (k)[29], _mb256, _ms256); \
    } while(0)

/* --- VAES-512 : 4xzmm = 16 AES-128 or AES-256 blocks ------------------ */
#ifdef RIJNDAEL_VAES512

/* AES-128 VAES-512 ECB x4-zmm (= 16 blocks): k[0..10] broadcast to 512-bit */
#define DO_ENC_BLOCK_128_128_VAES512_ECB_x4(m0,m1,m2,m3, k) \
    do { \
        __m512i _kr512; \
        _kr512 = _mm512_broadcast_i32x4((k)[0]); \
        (m0) = _mm512_xor_si512((m0), _kr512); \
        (m1) = _mm512_xor_si512((m1), _kr512); \
        (m2) = _mm512_xor_si512((m2), _kr512); \
        (m3) = _mm512_xor_si512((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[1]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[2]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[3]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[4]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[5]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[6]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[7]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[8]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[9]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[10]); \
        (m0) = _mm512_aesenclast_epi128((m0), _kr512); \
        (m1) = _mm512_aesenclast_epi128((m1), _kr512); \
        (m2) = _mm512_aesenclast_epi128((m2), _kr512); \
        (m3) = _mm512_aesenclast_epi128((m3), _kr512); \
    } while(0)

/* AES-256 VAES-512 ECB x4-zmm (= 16 blocks): k[0..14] */
#define DO_ENC_BLOCK_128_256_VAES512_ECB_x4(m0,m1,m2,m3, k) \
    do { \
        __m512i _kr512; \
        _kr512 = _mm512_broadcast_i32x4((k)[0]); \
        (m0) = _mm512_xor_si512((m0), _kr512); \
        (m1) = _mm512_xor_si512((m1), _kr512); \
        (m2) = _mm512_xor_si512((m2), _kr512); \
        (m3) = _mm512_xor_si512((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[1]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[2]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[3]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[4]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[5]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[6]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[7]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[8]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[9]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[10]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[11]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[12]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[13]); \
        (m0) = _mm512_aesenc_epi128((m0), _kr512); \
        (m1) = _mm512_aesenc_epi128((m1), _kr512); \
        (m2) = _mm512_aesenc_epi128((m2), _kr512); \
        (m3) = _mm512_aesenc_epi128((m3), _kr512); \
        _kr512 = _mm512_broadcast_i32x4((k)[14]); \
        (m0) = _mm512_aesenclast_epi128((m0), _kr512); \
        (m1) = _mm512_aesenclast_epi128((m1), _kr512); \
        (m2) = _mm512_aesenclast_epi128((m2), _kr512); \
        (m3) = _mm512_aesenclast_epi128((m3), _kr512); \
    } while(0)

/* Rijndael-256 VAES-512: 2xzmm = 4 blocks.
 * Layout: zml=[L0,L1,L2,L3]  zmr=[R0,R1,R2,R3].
 * AVX-512 has no blendv_epi8: use _mm512_movepi8_mask + _mm512_mask_blend_epi8.
 * _mm512_shuffle_epi8 operates per 128-bit lane (AVX-512BW). */
#define RINJDAEL_COMPENSATE_SR_VAES512(zml, zmr, tzml, tzmr, ms512, mk) \
    do { \
        (tzml) = _mm512_mask_blend_epi8(mk, (zml), (zmr)); \
        (tzmr) = _mm512_mask_blend_epi8(mk, (zmr), (zml)); \
        (tzml) = _mm512_shuffle_epi8((tzml), (ms512)); \
        (tzmr) = _mm512_shuffle_epi8((tzmr), (ms512)); \
    } while(0)

#define ENC_256_BLOCK_VAES512(zml, zmr, kl128, kr128, ms512, mk) \
    do { \
        __m512i _tzml, _tzmr; \
        RINJDAEL_COMPENSATE_SR_VAES512(zml, zmr, _tzml, _tzmr, ms512, mk); \
        (zml) = _mm512_aesenc_epi128(_tzml, _mm512_broadcast_i32x4(kl128)); \
        (zmr) = _mm512_aesenc_epi128(_tzmr, _mm512_broadcast_i32x4(kr128)); \
    } while(0)

#define ENC_256_BLOCK_LAST_VAES512(zml, zmr, kl128, kr128, ms512, mk) \
    do { \
        __m512i _tzml, _tzmr; \
        RINJDAEL_COMPENSATE_SR_VAES512(zml, zmr, _tzml, _tzmr, ms512, mk); \
        (zml) = _mm512_aesenclast_epi128(_tzml, _mm512_broadcast_i32x4(kl128)); \
        (zmr) = _mm512_aesenclast_epi128(_tzmr, _mm512_broadcast_i32x4(kr128)); \
    } while(0)

/* Process 4 Rijndael-256 blocks: [zml=(L0,L1,L2,L3), zmr=(R0,R1,R2,R3)] */
#define DO_ENC_BLOCK_256_256_VAES512_ECB_x4(zml, zmr, k) \
    do { \
        const __m128i _mb = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000); \
        const __m128i _ms = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100); \
        __m512i _ms512 = _mm512_broadcast_i32x4(_ms); \
        __mmask64 _mk  = _mm512_movepi8_mask(_mm512_broadcast_i32x4(_mb)); \
        (zml) = _mm512_xor_si512((zml), _mm512_broadcast_i32x4((k)[0])); \
        (zmr) = _mm512_xor_si512((zmr), _mm512_broadcast_i32x4((k)[1])); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[2],  (k)[3],  _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[4],  (k)[5],  _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[6],  (k)[7],  _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[8],  (k)[9],  _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[10], (k)[11], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[12], (k)[13], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[14], (k)[15], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[16], (k)[17], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[18], (k)[19], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[20], (k)[21], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[22], (k)[23], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[24], (k)[25], _ms512, _mk); \
        ENC_256_BLOCK_VAES512(zml, zmr, (k)[26], (k)[27], _ms512, _mk); \
        ENC_256_BLOCK_LAST_VAES512(zml, zmr, (k)[28], (k)[29], _ms512, _mk); \
    } while(0)

#endif /* RIJNDAEL_VAES512 */

/* ECB function declarations */
MAKE_GENERIC_FUNCS_ECB_DECL(aes128, vaes, 16, 16)
MAKE_GENERIC_FUNCS_ECB_DECL(aes256, vaes, 32, 16)
MAKE_GENERIC_FUNCS_ECB_DECL(rijndael256, vaes, 32, 32)

#endif /* RIJNDAEL_VAES */
#endif /* __RIJNDAEL_VAES_H__ */
