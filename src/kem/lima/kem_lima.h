#ifndef __OQS_KEM_LIMA_H
#define __OQS_KEM_LIMA_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_lima_2p_1024_cca_kem

#define OQS_KEM_lima_2p_1024_cca_kem_length_public_key 6145
#define OQS_KEM_lima_2p_1024_cca_kem_length_secret_key 9217
#define OQS_KEM_lima_2p_1024_cca_kem_length_ciphertext 4227
#define OQS_KEM_lima_2p_1024_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_lima_2p_1024_cca_kem_new();

extern OQS_STATUS OQS_KEM_lima_2p_1024_cca_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_lima_2p_1024_cca_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_lima_2p_1024_cca_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_lima_2p_2048_cca_kem

#define OQS_KEM_lima_2p_2048_cca_kem_length_public_key 12289
#define OQS_KEM_lima_2p_2048_cca_kem_length_secret_key 18433
#define OQS_KEM_lima_2p_2048_cca_kem_length_ciphertext 7299
#define OQS_KEM_lima_2p_2048_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_lima_2p_2048_cca_kem_new();

extern OQS_STATUS OQS_KEM_lima_2p_2048_cca_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_lima_2p_2048_cca_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_lima_2p_2048_cca_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_1018_cca_kem

#define OQS_KEM_lima_sp_1018_cca_kem_length_public_key 6109
#define OQS_KEM_lima_sp_1018_cca_kem_length_secret_key 9163
#define OQS_KEM_lima_sp_1018_cca_kem_length_ciphertext 4209
#define OQS_KEM_lima_sp_1018_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_lima_sp_1018_cca_kem_new();

extern OQS_STATUS OQS_KEM_lima_sp_1018_cca_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_lima_sp_1018_cca_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_lima_sp_1018_cca_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_1306_cca_kem

#define OQS_KEM_lima_sp_1306_cca_kem_length_public_key 10449
#define OQS_KEM_lima_sp_1306_cca_kem_length_secret_key 15673
#define OQS_KEM_lima_sp_1306_cca_kem_length_ciphertext 6763
#define OQS_KEM_lima_sp_1306_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_lima_sp_1306_cca_kem_new();

extern OQS_STATUS OQS_KEM_lima_sp_1306_cca_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_lima_sp_1306_cca_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_lima_sp_1306_cca_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_1822_cca_kem

#define OQS_KEM_lima_sp_1822_cca_kem_length_public_key 14577
#define OQS_KEM_lima_sp_1822_cca_kem_length_secret_key 21865
#define OQS_KEM_lima_sp_1822_cca_kem_length_ciphertext 8827
#define OQS_KEM_lima_sp_1822_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_lima_sp_1822_cca_kem_new();

extern OQS_STATUS OQS_KEM_lima_sp_1822_cca_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_lima_sp_1822_cca_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_lima_sp_1822_cca_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_2062_cca_kem

#define OQS_KEM_lima_sp_2062_cca_kem_length_public_key 16497
#define OQS_KEM_lima_sp_2062_cca_kem_length_secret_key 24745
#define OQS_KEM_lima_sp_2062_cca_kem_length_ciphertext 9787
#define OQS_KEM_lima_sp_2062_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_lima_sp_2062_cca_kem_new();

extern OQS_STATUS OQS_KEM_lima_sp_2062_cca_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_lima_sp_2062_cca_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_lima_sp_2062_cca_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif // __OQS_KEM_LIMA_H
