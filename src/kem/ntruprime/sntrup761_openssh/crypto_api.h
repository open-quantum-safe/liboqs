#define crypto_int8 int8_t
#define crypto_uint8 uint8_t
#define crypto_int16 int16_t
#define crypto_uint16 uint16_t
#define crypto_int32 int32_t
#define crypto_uint32 uint32_t
#define crypto_int64 int64_t
#define crypto_uint64 uint64_t
#define crypto_hash_sha512 OQS_SHA2_sha512
#define randombytes(buf,len) OQS_randombytes((uint8_t *)(buf),(len))
#define crypto_kem_sntrup761_keypair PQCLEAN_SNTRUP761_OPENSSH_crypto_kem_keypair
#define crypto_kem_sntrup761_enc PQCLEAN_SNTRUP761_OPENSSH_crypto_kem_enc
#define crypto_kem_sntrup761_dec PQCLEAN_SNTRUP761_OPENSSH_crypto_kem_dec
