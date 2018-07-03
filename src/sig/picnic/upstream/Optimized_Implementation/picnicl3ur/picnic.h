#ifndef PICNIC_H
#define PICNIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct picnic_instance_s picnic_instance_t;

typedef enum {
  PARAMETER_SET_INVALID,
  Picnic_L1_FS, // 1
  Picnic_L1_UR, // 2
  Picnic_L3_FS, // 3
  Picnic_L3_UR, // 4
  Picnic_L5_FS, // 5
  Picnic_L5_UR, // 6
  PARAMETER_SET_MAX_INDEX
} picnic_params_t;

#define LOWMC_BLOCK_SIZE(p) ((((unsigned int)(p) + 3) / 2) * 8)

#define MAX_LOWMC_ROUNDS 38
#define MAX_LOWMC_SBOXES 10
#define MAX_ROUNDS 438

#define PICNIC_PRIVATE_KEY_SIZE(p) (1 + 2 * LOWMC_BLOCK_SIZE(p))
#define PICNIC_PUBLIC_KEY_SIZE(p) (1 + 2 * LOWMC_BLOCK_SIZE(p))

#define PICNIC_SIGNATURE_SIZE_Picnic_L1_FS 34000
#define PICNIC_SIGNATURE_SIZE_Picnic_L1_UR 53929
#define PICNIC_SIGNATURE_SIZE_Picnic_L3_FS 76740
#define PICNIC_SIGNATURE_SIZE_Picnic_L3_UR 121813
#define PICNIC_SIGNATURE_SIZE_Picnic_L5_FS 132824
#define PICNIC_SIGNATURE_SIZE_Picnic_L5_UR 209474

#define PICNIC_CONCAT2(a, b) a ## _ ## b
#define PICNIC_CONCAT(a, b) PICNIC_CONCAT2(a, b)

#define PICNIC_SIGNATURE_SIZE(p) PICNIC_CONCAT(PICNIC_SIGNATURE_SIZE, p)

#define MAX_LOWMC_BLOCK_SIZE LOWMC_BLOCK_SIZE(Picnic_L5_UR)
#define PICNIC_MAX_PRIVATE_KEY_SIZE PICNIC_PRIVATE_KEY_SIZE(Picnic_L5_UR)
#define PICNIC_MAX_PUBLIC_KEY_SIZE PICNIC_PUBLIC_KEY_SIZE(Picnic_L5_UR)
#define PICNIC_MAX_SIGNATURE_SIZE PICNIC_SIGNATURE_SIZE(Picnic_L5_UR)

size_t picnic_get_max_sig_size(picnic_params_t param);
size_t picnic_get_private_key_size(picnic_params_t param);
size_t picnic_get_public_key_size(picnic_params_t param);
const char* picnic_get_param_name(picnic_params_t parameters);

bool picnic_create_key(picnic_params_t param, uint8_t* private_key, size_t* private_size,
                       uint8_t* public_key, size_t* public_size);
bool picnic_sk_to_pk(const uint8_t* private_key, size_t private_size, uint8_t* public_key,
                     size_t* public_size);

bool picnic_verify_key(const uint8_t* private_key, size_t private_size, const uint8_t* public_key,
                       size_t public_size);

bool picnic_sign(const uint8_t* private_key, size_t private_size, const uint8_t* public_key,
                 size_t public_key_size, const uint8_t* msg, size_t msglen, uint8_t* sig,
                 size_t* siglen);
bool picnic_verify(const uint8_t* public_key, size_t public_key_size, const uint8_t* msg,
                   size_t msglen, const uint8_t* sig, size_t siglen);

void picnic_visualize(FILE* out, const uint8_t* public_key, size_t public_key_size,
                      const uint8_t* msg, size_t msglen, const uint8_t* sig, size_t siglen);

#endif
