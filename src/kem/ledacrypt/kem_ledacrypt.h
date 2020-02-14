#ifndef _OQS_KEM_LEDACRYPT_H
#define _OQS_KEM_LEDACRYPT_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ledacrypt_ledakemlt12
#define OQS_KEM_ledacrypt_ledakemlt12_length_public_key 6520
#define OQS_KEM_ledacrypt_ledakemlt12_length_secret_key 50
#define OQS_KEM_ledacrypt_ledakemlt12_length_ciphertext 6544
#define OQS_KEM_ledacrypt_ledakemlt12_length_shared_secret 32
OQS_KEM *OQS_KEM_ledacrypt_ledakemlt12_new(void);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt12_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt12_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt12_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ledacrypt_ledakemlt32
#define OQS_KEM_ledacrypt_ledakemlt32_length_public_key 12032
#define OQS_KEM_ledacrypt_ledakemlt32_length_secret_key 66
#define OQS_KEM_ledacrypt_ledakemlt32_length_ciphertext 12064
#define OQS_KEM_ledacrypt_ledakemlt32_length_shared_secret 48
OQS_KEM *OQS_KEM_ledacrypt_ledakemlt32_new(void);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt32_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt32_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt32_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ledacrypt_ledakemlt52
#define OQS_KEM_ledacrypt_ledakemlt52_length_public_key 19040
#define OQS_KEM_ledacrypt_ledakemlt52_length_secret_key 82
#define OQS_KEM_ledacrypt_ledakemlt52_length_ciphertext 19080
#define OQS_KEM_ledacrypt_ledakemlt52_length_shared_secret 64
OQS_KEM *OQS_KEM_ledacrypt_ledakemlt52_new(void);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt52_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt52_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_ledakemlt52_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif

