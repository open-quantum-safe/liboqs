// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_SIKE_H
#define OQS_KEM_SIKE_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_sike_p434

#define OQS_KEM_sike_p434_length_public_key 330
#define OQS_KEM_sike_p434_length_secret_key 374
#define OQS_KEM_sike_p434_length_ciphertext 346
#define OQS_KEM_sike_p434_length_shared_secret 16

OQS_KEM *OQS_KEM_sike_p434_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p434_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p434_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p434_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p434_compressed

#define OQS_KEM_sike_p434_compressed_length_public_key 197
#define OQS_KEM_sike_p434_compressed_length_secret_key 350
#define OQS_KEM_sike_p434_compressed_length_ciphertext 236
#define OQS_KEM_sike_p434_compressed_length_shared_secret 16

OQS_KEM *OQS_KEM_sike_p434_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p434_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p434_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p434_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p503

#define OQS_KEM_sike_p503_length_public_key 378
#define OQS_KEM_sike_p503_length_secret_key 434
#define OQS_KEM_sike_p503_length_ciphertext 402
#define OQS_KEM_sike_p503_length_shared_secret 24

OQS_KEM *OQS_KEM_sike_p503_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p503_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p503_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p503_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p503_compressed

#define OQS_KEM_sike_p503_compressed_length_public_key 225
#define OQS_KEM_sike_p503_compressed_length_secret_key 407
#define OQS_KEM_sike_p503_compressed_length_ciphertext 280
#define OQS_KEM_sike_p503_compressed_length_shared_secret 24

OQS_KEM *OQS_KEM_sike_p503_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p503_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p503_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p503_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p610

#define OQS_KEM_sike_p610_length_public_key 462
#define OQS_KEM_sike_p610_length_secret_key 524
#define OQS_KEM_sike_p610_length_ciphertext 486
#define OQS_KEM_sike_p610_length_shared_secret 24

OQS_KEM *OQS_KEM_sike_p610_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p610_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p610_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p610_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p610_compressed

#define OQS_KEM_sike_p610_compressed_length_public_key 274
#define OQS_KEM_sike_p610_compressed_length_secret_key 491
#define OQS_KEM_sike_p610_compressed_length_ciphertext 336
#define OQS_KEM_sike_p610_compressed_length_shared_secret 24

OQS_KEM *OQS_KEM_sike_p610_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p610_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p610_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p610_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p751

#define OQS_KEM_sike_p751_length_public_key 564
#define OQS_KEM_sike_p751_length_secret_key 644
#define OQS_KEM_sike_p751_length_ciphertext 596
#define OQS_KEM_sike_p751_length_shared_secret 32

OQS_KEM *OQS_KEM_sike_p751_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p751_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p751_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p751_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p751_compressed

#define OQS_KEM_sike_p751_compressed_length_public_key 335
#define OQS_KEM_sike_p751_compressed_length_secret_key 602
#define OQS_KEM_sike_p751_compressed_length_ciphertext 410
#define OQS_KEM_sike_p751_compressed_length_shared_secret 32

OQS_KEM *OQS_KEM_sike_p751_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sike_p751_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p751_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p751_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p434

#define OQS_KEM_sidh_p434_length_public_key 330
#define OQS_KEM_sidh_p434_length_secret_key 28
#define OQS_KEM_sidh_p434_length_ciphertext 330
#define OQS_KEM_sidh_p434_length_shared_secret 110

OQS_KEM *OQS_KEM_sidh_p434_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p434_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p434_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p434_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p434_compressed

#define OQS_KEM_sidh_p434_compressed_length_public_key 197
#define OQS_KEM_sidh_p434_compressed_length_secret_key 28
#define OQS_KEM_sidh_p434_compressed_length_ciphertext 197
#define OQS_KEM_sidh_p434_compressed_length_shared_secret 110

OQS_KEM *OQS_KEM_sidh_p434_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p434_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p434_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p434_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p503

#define OQS_KEM_sidh_p503_length_public_key 378
#define OQS_KEM_sidh_p503_length_secret_key 32
#define OQS_KEM_sidh_p503_length_ciphertext 378
#define OQS_KEM_sidh_p503_length_shared_secret 126

OQS_KEM *OQS_KEM_sidh_p503_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p503_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p503_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p503_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p503_compressed

#define OQS_KEM_sidh_p503_compressed_length_public_key 225
#define OQS_KEM_sidh_p503_compressed_length_secret_key 32
#define OQS_KEM_sidh_p503_compressed_length_ciphertext 225
#define OQS_KEM_sidh_p503_compressed_length_shared_secret 126

OQS_KEM *OQS_KEM_sidh_p503_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p503_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p503_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p503_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p610

#define OQS_KEM_sidh_p610_length_public_key 462
#define OQS_KEM_sidh_p610_length_secret_key 39
#define OQS_KEM_sidh_p610_length_ciphertext 462
#define OQS_KEM_sidh_p610_length_shared_secret 154

OQS_KEM *OQS_KEM_sidh_p610_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p610_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p610_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p610_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p610_compressed

#define OQS_KEM_sidh_p610_compressed_length_public_key 274
#define OQS_KEM_sidh_p610_compressed_length_secret_key 39
#define OQS_KEM_sidh_p610_compressed_length_ciphertext 274
#define OQS_KEM_sidh_p610_compressed_length_shared_secret 154

OQS_KEM *OQS_KEM_sidh_p610_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p610_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p610_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p610_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p751

#define OQS_KEM_sidh_p751_length_public_key 564
#define OQS_KEM_sidh_p751_length_secret_key 48
#define OQS_KEM_sidh_p751_length_ciphertext 564
#define OQS_KEM_sidh_p751_length_shared_secret 188

OQS_KEM *OQS_KEM_sidh_p751_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p751_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p751_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p751_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p751_compressed

#define OQS_KEM_sidh_p751_compressed_length_public_key 335
#define OQS_KEM_sidh_p751_compressed_length_secret_key 48
#define OQS_KEM_sidh_p751_compressed_length_ciphertext 335
#define OQS_KEM_sidh_p751_compressed_length_shared_secret 188

OQS_KEM *OQS_KEM_sidh_p751_compressed_new(void);

OQS_API OQS_STATUS OQS_KEM_sidh_p751_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p751_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p751_compressed_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#endif // OQS_KEM_SIKE_H
