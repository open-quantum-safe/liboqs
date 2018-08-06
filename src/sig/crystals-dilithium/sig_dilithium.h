#ifndef __OQS_SIG_DILITHIUM_H
#define __OQS_SIG_DILITHIUM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_Dilithium_II_medium

#define OQS_SIG_Dilithium_II_medium_length_public_key 1184
#define OQS_SIG_Dilithium_II_medium_length_secret_key 2800
#define OQS_SIG_Dilithium_II_medium_length_sig_overhead 2044

OQS_SIG *OQS_SIG_Dilithium_II_medium_new();

extern OQS_STATUS OQS_SIG_Dilithium_II_medium_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_II_medium_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_II_medium_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_Dilithium_III_recommended

#define OQS_SIG_Dilithium_III_recommended_length_public_key 1472
#define OQS_SIG_Dilithium_III_recommended_length_secret_key 3504
#define OQS_SIG_Dilithium_III_recommended_length_sig_overhead 2701

OQS_SIG *OQS_SIG_Dilithium_III_recommended_new();

extern OQS_STATUS OQS_SIG_Dilithium_III_recommended_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_III_recommended_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_III_recommended_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_Dilithium_IV_very_high

#define OQS_SIG_Dilithium_IV_very_high_length_public_key 1760
#define OQS_SIG_Dilithium_IV_very_high_length_secret_key 3856
#define OQS_SIG_Dilithium_IV_very_high_length_sig_overhead 3366

OQS_SIG *OQS_SIG_Dilithium_IV_very_high_new();

extern OQS_STATUS OQS_SIG_Dilithium_IV_very_high_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_IV_very_high_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_IV_very_high_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif
#endif
