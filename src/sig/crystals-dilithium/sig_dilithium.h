#ifndef __OQS_SIG_DILITHIUM_H
#define __OQS_SIG_DILITHIUM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_DILITHIUM_II

#define OQS_SIG_Dilithium_II_length_public_key 1184U
#define OQS_SIG_Dilithium_II_length_secret_key 2800U
#define OQS_SIG_Dilithium_II_length_sig_overhead 2044U

OQS_SIG *OQS_SIG_Dilithium_II_new();

extern OQS_STATUS OQS_SIG_Dilithium_II_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_II_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_II_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_DILITHIUM_III

#define OQS_SIG_Dilithium_III_length_public_key 1472U
#define OQS_SIG_Dilithium_III_length_secret_key 3504U
#define OQS_SIG_Dilithium_III_length_sig_overhead 2701U

OQS_SIG *OQS_SIG_Dilithium_III_new();

extern OQS_STATUS OQS_SIG_Dilithium_III_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_III_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_III_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_DILITHIUM_IV

#define OQS_SIG_Dilithium_IV_length_public_key 1760U
#define OQS_SIG_Dilithium_IV_length_secret_key 3856U
#define OQS_SIG_Dilithium_IV_length_sig_overhead 3366U

OQS_SIG *OQS_SIG_Dilithium_IV_new();

extern OQS_STATUS OQS_SIG_Dilithium_IV_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_IV_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_Dilithium_IV_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif
#endif
