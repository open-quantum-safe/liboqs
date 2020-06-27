// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_PICNIC_H
#define OQS_SIG_PICNIC_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_picnic_L1_FS

#define OQS_SIG_picnic_L1_FS_length_public_key 33
#define OQS_SIG_picnic_L1_FS_length_secret_key 49
#define OQS_SIG_picnic_L1_FS_length_signature 34036

OQS_SIG *OQS_SIG_picnic_L1_FS_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L1_FS_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L1_FS_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L1_FS_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L1_UR

#define OQS_SIG_picnic_L1_UR_length_public_key 33
#define OQS_SIG_picnic_L1_UR_length_secret_key 49
#define OQS_SIG_picnic_L1_UR_length_signature 53965

OQS_SIG *OQS_SIG_picnic_L1_UR_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L1_UR_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L1_UR_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L1_UR_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L1_full

#define OQS_SIG_picnic_L1_full_length_public_key 35
#define OQS_SIG_picnic_L1_full_length_secret_key 52
#define OQS_SIG_picnic_L1_full_length_signature 32065

OQS_SIG *OQS_SIG_picnic_L1_full_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L1_full_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L1_full_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L1_full_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_FS

#define OQS_SIG_picnic_L3_FS_length_public_key 49
#define OQS_SIG_picnic_L3_FS_length_secret_key 73
#define OQS_SIG_picnic_L3_FS_length_signature 76776

OQS_SIG *OQS_SIG_picnic_L3_FS_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L3_FS_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L3_FS_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L3_FS_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_UR

#define OQS_SIG_picnic_L3_UR_length_public_key 49
#define OQS_SIG_picnic_L3_UR_length_secret_key 73
#define OQS_SIG_picnic_L3_UR_length_signature 121849

OQS_SIG *OQS_SIG_picnic_L3_UR_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L3_UR_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L3_UR_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L3_UR_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_full

#define OQS_SIG_picnic_L3_full_length_public_key 49
#define OQS_SIG_picnic_L3_full_length_secret_key 73
#define OQS_SIG_picnic_L3_full_length_signature 71183

OQS_SIG *OQS_SIG_picnic_L3_full_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L3_full_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L3_full_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L3_full_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_FS

#define OQS_SIG_picnic_L5_FS_length_public_key 65
#define OQS_SIG_picnic_L5_FS_length_secret_key 97
#define OQS_SIG_picnic_L5_FS_length_signature 132860

OQS_SIG *OQS_SIG_picnic_L5_FS_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L5_FS_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L5_FS_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L5_FS_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_UR

#define OQS_SIG_picnic_L5_UR_length_public_key 65
#define OQS_SIG_picnic_L5_UR_length_secret_key 97
#define OQS_SIG_picnic_L5_UR_length_signature 209510

OQS_SIG *OQS_SIG_picnic_L5_UR_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L5_UR_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L5_UR_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L5_UR_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_full

#define OQS_SIG_picnic_L5_full_length_public_key 65
#define OQS_SIG_picnic_L5_full_length_secret_key 97
#define OQS_SIG_picnic_L5_full_length_signature 126290

OQS_SIG *OQS_SIG_picnic_L5_full_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic_L5_full_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L5_full_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic_L5_full_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic3_L1

#define OQS_SIG_picnic3_L1_length_public_key 35
#define OQS_SIG_picnic3_L1_length_secret_key 52
#define OQS_SIG_picnic3_L1_length_signature 14612

OQS_SIG *OQS_SIG_picnic3_L1_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic3_L1_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic3_L1_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic3_L1_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic3_L3

#define OQS_SIG_picnic3_L3_length_public_key 49
#define OQS_SIG_picnic3_L3_length_secret_key 73
#define OQS_SIG_picnic3_L3_length_signature 35028

OQS_SIG *OQS_SIG_picnic3_L3_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic3_L3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic3_L3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic3_L3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic3_L5

#define OQS_SIG_picnic3_L5_length_public_key 65
#define OQS_SIG_picnic3_L5_length_secret_key 97
#define OQS_SIG_picnic3_L5_length_signature 61028

OQS_SIG *OQS_SIG_picnic3_L5_new(void);

OQS_API OQS_STATUS OQS_SIG_picnic3_L5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic3_L5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_picnic3_L5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#endif // OQS_SIG_PICNIC_H
