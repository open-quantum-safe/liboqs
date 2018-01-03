#if !defined(WINDOWS)
#define NIX
#endif
#define _X86_
#define _FAST_GENERIC_
#define _AES128_FOR_A_ 1

#include <oqs/aes.h>
#define AES128_load_schedule(x, y) OQS_AES128_load_schedule((x), (void **) (y), 1)
#define AES128_ECB_enc_sch OQS_AES128_ECB_enc_sch
#define AES128_free_schedule OQS_AES128_free_schedule

#include <oqs/sha3.h>
#define cshake128_simple OQS_SHA3_cshake128_simple

#include <oqs/rand.h>

// fix global namespacing
#define frodo_add oqs_kex_lwe_frodokem_640_aes_frodo_add
#define frodo_key_decode oqs_kex_lwe_frodokem_640_aes_frodo_key_decode
#define frodo_key_encode oqs_kex_lwe_frodokem_640_aes_frodo_key_encode
#define frodo_mul_add_as_plus_e oqs_kex_lwe_frodokem_640_aes_frodo_mul_add_as_plus_e
#define frodo_mul_add_sa_plus_e oqs_kex_lwe_frodokem_640_aes_frodo_mul_add_sa_plus_e
#define frodo_mul_add_sb_plus_e oqs_kex_lwe_frodokem_640_aes_frodo_mul_add_sb_plus_e
#define frodo_mul_bs oqs_kex_lwe_frodokem_640_aes_frodo_mul_bs
#define frodo_pack oqs_kex_lwe_frodokem_640_aes_frodo_pack
#define frodo_sample_n oqs_kex_lwe_frodokem_640_aes_frodo_sample_n
#define frodo_sub oqs_kex_lwe_frodokem_640_aes_frodo_sub
#define frodo_unpack oqs_kex_lwe_frodokem_640_aes_frodo_unpack
#define clear_words oqs_kex_lwe_frodokem_640_aes_clear_words
#define crypto_kem_keypair_Frodo640 oqs_kex_lwe_frodokem_640_aes_keypair
#define crypto_kem_enc_Frodo640 oqs_kex_lwe_frodokem_640_aes_enc
#define crypto_kem_dec_Frodo640 oqs_kex_lwe_frodokem_640_aes_dec
#define CDF_TABLE oqs_kex_lwe_frodokem_640_aes_CDF_TABLE
#define CDF_TABLE_LEN oqs_kex_lwe_frodokem_640_aes_CDF_TABLE_LEN

#include "upstream/api_frodo640.h"
#include "upstream/frodo640.c"
#include "upstream/util.c"
