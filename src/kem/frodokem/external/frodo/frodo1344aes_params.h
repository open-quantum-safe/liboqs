#include <oqs/sha3.h> // for shake macro

#define CRYPTO_PUBLICKEYBYTES OQS_KEM_frodokem_1344_aes_length_public_key
#define CRYPTO_SECRETKEYBYTES OQS_KEM_frodokem_1344_aes_length_secret_key
#define CRYPTO_CIPHERTEXTBYTES OQS_KEM_frodokem_1344_aes_length_ciphertext
#define CRYPTO_BYTES OQS_KEM_frodokem_1344_aes_length_shared_secret
#define CRYPTO_SEEDBYTES OQS_KEM_frodokem_1344_aes_length_keypair_seed
#define CRYPTO_ENCSEEDBYTES OQS_KEM_frodokem_1344_aes_length_encaps_seed

// Parameters for "FrodoKEM-1344"
#define PARAMS_N 1344
#define PARAMS_NBAR 8
#define PARAMS_LOGQ 16
#define PARAMS_Q (1 << PARAMS_LOGQ)
#define PARAMS_EXTRACTED_BITS 4
#define PARAMS_STRIPE_STEP 8
#define PARAMS_PARALLEL 4
#define BYTES_SEED_A 16
#define BYTES_MU (PARAMS_EXTRACTED_BITS*PARAMS_NBAR*PARAMS_NBAR)/8
#define BYTES_SALT (2*CRYPTO_BYTES)
#define BYTES_SEED_SE (2*CRYPTO_BYTES)
#define BYTES_PKHASH CRYPTO_BYTES

// CDF table
#define CDF_TABLE {9142, 23462, 30338, 32361, 32725, 32765, 32767}
#define CDF_TABLE_LEN 7

#define randombytes OQS_randombytes
#define clear_bytes OQS_MEM_cleanse

#define shake     OQS_SHA3_shake256
#define USE_AES128_FOR_A

#define crypto_kem_keypair            OQS_KEM_frodokem_1344_aes_keypair
#define crypto_kem_keypair_derand     OQS_KEM_frodokem_1344_aes_keypair_derand
#define crypto_kem_enc                OQS_KEM_frodokem_1344_aes_encaps
#define crypto_kem_enc_derand         OQS_KEM_frodokem_1344_aes_encaps_derand
#define crypto_kem_dec                OQS_KEM_frodokem_1344_aes_decaps

#define frodo_sample_n oqs_kem_frodokem_1344_aes_sample_n
#define frodo_pack oqs_kem_frodokem_1344_aes_pack
#define frodo_unpack oqs_kem_frodokem_1344_aes_unpack
#define ct_select oqs_kem_frodokem_1344_aes_ct_select
#define ct_verify oqs_kem_frodokem_1344_aes_ct_verify

#define frodo_mul_add_as_plus_e oqs_kem_frodokem_1344_aes_mul_add_as_plus_e
#define frodo_mul_add_as_plus_e_portable oqs_kem_frodokem_1344_aes_mul_add_as_plus_e_portable
#define frodo_mul_add_as_plus_e_avx2 oqs_kem_frodokem_1344_aes_mul_add_as_plus_e_avx2
#define frodo_mul_add_sa_plus_e oqs_kem_frodokem_1344_aes_mul_add_sa_plus_e
#define frodo_mul_add_sa_plus_e_aes_portable oqs_kem_frodokem_1344_aes_mul_add_sa_plus_e_portable
#define frodo_mul_add_sa_plus_e_aes_avx2 oqs_kem_frodokem_1344_aes_mul_add_sa_plus_e_avx2
#define frodo_mul_add_sb_plus_e oqs_kem_frodokem_1344_aes_mul_add_sb_plus_e
#define frodo_mul_bs oqs_kem_frodokem_1344_aes_mul_bs

#define frodo_add oqs_kem_frodokem_1344_aes_add
#define frodo_sub oqs_kem_frodokem_1344_aes_sub
#define frodo_key_encode oqs_kem_frodokem_1344_aes_key_encode
#define frodo_key_decode oqs_kem_frodokem_1344_aes_key_decode
