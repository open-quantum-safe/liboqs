#ifndef __OQS_KEM_BIKE_H
#define __OQS_KEM_BIKE_H

#include <oqs/oqs.h>

//Determine whether the additional implementation is being used.
#ifdef OQS_ENABLE_KEM_bike1_l1

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike1_l1_length_secret_key 4670
#else
#define OQS_KEM_bike1_l1_length_secret_key 3606
#endif
#else
#define OQS_KEM_bike1_l1_length_secret_key 2542
#endif
#define OQS_KEM_bike1_l1_length_public_key 2542
#define OQS_KEM_bike1_l1_length_ciphertext 2542
#define OQS_KEM_bike1_l1_length_shared_secret 32

OQS_KEM *OQS_KEM_bike1_l1_new();

OQS_API OQS_STATUS OQS_KEM_bike1_l1_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike1_l3

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike1_l3_length_secret_key 8116
#else
#define OQS_KEM_bike1_l3_length_secret_key 7050
#endif
#else
#define OQS_KEM_bike1_l3_length_secret_key 4964
#endif
#define OQS_KEM_bike1_l3_length_public_key 4964
#define OQS_KEM_bike1_l3_length_ciphertext 4964
#define OQS_KEM_bike1_l3_length_shared_secret 32

OQS_KEM *OQS_KEM_bike1_l3_new();

OQS_API OQS_STATUS OQS_KEM_bike1_l3_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike1_l5

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike1_l5_length_secret_key 12364
#else
#define OQS_KEM_bike1_l5_length_secret_key 10276
#endif
#else
#define OQS_KEM_bike1_l5_length_secret_key 8188
#endif
#define OQS_KEM_bike1_l5_length_public_key 8188
#define OQS_KEM_bike1_l5_length_ciphertext 8188
#define OQS_KEM_bike1_l5_length_shared_secret 32

OQS_KEM *OQS_KEM_bike1_l5_new();

OQS_API OQS_STATUS OQS_KEM_bike1_l5_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l5_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l5_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike2_l1

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike2_l1_length_secret_key 5941
#else
#define OQS_KEM_bike2_l1_length_secret_key 4964
#endif
#define OQS_KEM_bike2_l1_length_public_key 1271
#define OQS_KEM_bike2_l1_length_ciphertext 1271
#else
#define OQS_KEM_bike2_l1_length_secret_key 2542
#define OQS_KEM_bike2_l1_length_public_key 2542
#define OQS_KEM_bike2_l1_length_ciphertext 2542
#endif
#define OQS_KEM_bike2_l1_length_shared_secret 32

OQS_KEM *OQS_KEM_bike2_l1_new();

OQS_API OQS_STATUS OQS_KEM_bike2_l1_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike2_l1_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike2_l1_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike2_l3

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike2_l3_length_secret_key 10598
#else
#define OQS_KEM_bike2_l3_length_secret_key 9787
#endif

#define OQS_KEM_bike2_l3_length_public_key 2482
#define OQS_KEM_bike2_l3_length_ciphertext 2482
#else
#define OQS_KEM_bike2_l3_length_secret_key 4964
#define OQS_KEM_bike2_l3_length_public_key 4964
#define OQS_KEM_bike2_l3_length_ciphertext 4964
#endif
#define OQS_KEM_bike2_l3_length_shared_secret 32

OQS_KEM *OQS_KEM_bike2_l3_new();

OQS_API OQS_STATUS OQS_KEM_bike2_l3_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike2_l3_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike2_l3_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike2_l5

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike2_l5_length_secret_key 16458
#else
#define OQS_KEM_bike2_l5_length_secret_key 14370
#endif

#define OQS_KEM_bike2_l5_length_public_key 4094
#define OQS_KEM_bike2_l5_length_ciphertext 4094
#else
#define OQS_KEM_bike2_l5_length_public_key 8188
#define OQS_KEM_bike2_l5_length_secret_key 8188
#define OQS_KEM_bike2_l5_length_ciphertext 8188
#endif
#define OQS_KEM_bike2_l5_length_shared_secret 32

OQS_KEM *OQS_KEM_bike2_l5_new();

OQS_API OQS_STATUS OQS_KEM_bike2_l5_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike2_l5_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike2_l5_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike3_l1

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike3_l1_length_secret_key 7628
#else
#define OQS_KEM_bike3_l1_length_secret_key 6572
#endif

#else
#define OQS_KEM_bike3_l1_length_secret_key 2758
#endif
#define OQS_KEM_bike3_l1_length_public_key 2758
#define OQS_KEM_bike3_l1_length_ciphertext 2758
#define OQS_KEM_bike3_l1_length_shared_secret 32

OQS_KEM *OQS_KEM_bike3_l1_new();

OQS_API OQS_STATUS OQS_KEM_bike3_l1_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike3_l1_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike3_l1_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike3_l3

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike3_l3_length_secret_key 13996
#else
#define OQS_KEM_bike3_l3_length_secret_key 12420
#endif
#else
#define OQS_KEM_bike3_l3_length_secret_key 5422
#endif
#define OQS_KEM_bike3_l3_length_public_key 5422
#define OQS_KEM_bike3_l3_length_ciphertext 5422
#define OQS_KEM_bike3_l3_length_shared_secret 32

OQS_KEM *OQS_KEM_bike3_l3_new();

OQS_API OQS_STATUS OQS_KEM_bike3_l3_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike3_l3_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike3_l3_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_bike3_l5

#ifdef OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION
#ifdef CONSTANT_TIME
#define OQS_KEM_bike3_l5_length_secret_key 22244
#else
#define OQS_KEM_bike3_l5_length_secret_key 20156
#endif
#else
#define OQS_KEM_bike3_l5_length_secret_key 9034
#endif
#define OQS_KEM_bike3_l5_length_public_key 9034
#define OQS_KEM_bike3_l5_length_ciphertext 9034
#define OQS_KEM_bike3_l5_length_shared_secret 32

OQS_KEM *OQS_KEM_bike3_l5_new();

OQS_API OQS_STATUS OQS_KEM_bike3_l5_keypair(uint8_t *public_key,
                                            uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike3_l5_encaps(uint8_t *ciphertext,
                                           uint8_t *shared_secret,
                                           const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike3_l5_decaps(uint8_t *shared_secret,
                                           const unsigned char *ciphertext,
                                           const uint8_t *secret_key);

#endif

#endif
