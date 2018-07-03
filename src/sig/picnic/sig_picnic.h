#ifndef __OQS_SIG_SIKE_H
#define __OQS_SIG_SIKE_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_picnic_L1_FS

#define OQS_SIG_picnic_L1_FS_length_public_key 33
#define OQS_SIG_picnic_L1_FS_length_secret_key 50
#define OQS_SIG_picnic_L1_FS_length_signature 34004

OQS_SIG *OQS_SIG_picnic_L1_FS_new();

extern OQS_STATUS OQS_SIG_picnic_L1_FS_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L1_FS_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L1_FS_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L1_UR

#define OQS_SIG_picnic_L1_UR_length_public_key 33
#define OQS_SIG_picnic_L1_UR_length_secret_key 50
#define OQS_SIG_picnic_L1_UR_length_signature 53933

OQS_SIG *OQS_SIG_picnic_L1_UR_new();

extern OQS_STATUS OQS_SIG_picnic_L1_UR_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L1_UR_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L1_UR_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_FS

#define OQS_SIG_picnic_L3_FS_length_public_key 49
#define OQS_SIG_picnic_L3_FS_length_secret_key 74
#define OQS_SIG_picnic_L3_FS_length_signature 76744

OQS_SIG *OQS_SIG_picnic_L3_FS_new();

extern OQS_STATUS OQS_SIG_picnic_L3_FS_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L3_FS_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L3_FS_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_UR

#define OQS_SIG_picnic_L3_UR_length_public_key 49
#define OQS_SIG_picnic_L3_UR_length_secret_key 74
#define OQS_SIG_picnic_L3_UR_length_signature 121817

OQS_SIG *OQS_SIG_picnic_L3_UR_new();

extern OQS_STATUS OQS_SIG_picnic_L3_UR_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L3_UR_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L3_UR_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_FS

#define OQS_SIG_picnic_L5_FS_length_public_key 65
#define OQS_SIG_picnic_L5_FS_length_secret_key 98
#define OQS_SIG_picnic_L5_FS_length_signature 132828

OQS_SIG *OQS_SIG_picnic_L5_FS_new();

extern OQS_STATUS OQS_SIG_picnic_L5_FS_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L5_FS_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L5_FS_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_UR

#define OQS_SIG_picnic_L5_UR_length_public_key 65 
#define OQS_SIG_picnic_L5_UR_length_secret_key 98
#define OQS_SIG_picnic_L5_UR_length_signature 209478

OQS_SIG *OQS_SIG_picnic_L5_UR_new();

extern OQS_STATUS OQS_SIG_picnic_L5_UR_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L5_UR_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
extern OQS_STATUS OQS_SIG_picnic_L5_UR_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#endif
