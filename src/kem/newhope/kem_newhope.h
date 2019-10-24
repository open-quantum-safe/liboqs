#ifndef OQS_KEM_NEWHOPE_H
#define OQS_KEM_NEWHOPE_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_newhope_512cca
#define OQS_KEM_newhope_512cca_length_public_key 928
#define OQS_KEM_newhope_512cca_length_secret_key 1888
#define OQS_KEM_newhope_512cca_length_ciphertext 1120
#define OQS_KEM_newhope_512cca_length_shared_secret 32
OQS_KEM *OQS_KEM_newhope_512cca_new(void);
OQS_API OQS_STATUS OQS_KEM_newhope_512cca_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_newhope_512cca_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_newhope_512cca_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_newhope_1024cca
#define OQS_KEM_newhope_1024cca_length_public_key 1824
#define OQS_KEM_newhope_1024cca_length_secret_key 3680
#define OQS_KEM_newhope_1024cca_length_ciphertext 2208
#define OQS_KEM_newhope_1024cca_length_shared_secret 32
OQS_KEM *OQS_KEM_newhope_1024cca_new(void);
OQS_API OQS_STATUS OQS_KEM_newhope_1024cca_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_newhope_1024cca_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_newhope_1024cca_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif // OQS_KEM_NEWHOPE_H
