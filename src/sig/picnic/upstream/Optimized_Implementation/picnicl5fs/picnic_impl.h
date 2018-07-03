#ifndef PICNIC_IMPL_H
#define PICNIC_IMPL_H

#include "mpc_lowmc.h"
#include "picnic.h"

#define MAX_DIGEST_SIZE 64
#define MAX_NUM_ROUNDS 438

typedef enum { TRANSFORM_FS, TRANSFORM_UR } transform_t;

struct picnic_instance_s {
  picnic_params_t params;
  transform_t transform;

  lowmc_t lowmc;
  lowmc_implementation_f lowmc_impl;
  lowmc_verify_implementation_f lowmc_verify_impl;

  uint32_t security_level; /* bits */
  uint32_t digest_size;    /* bytes */
  uint32_t seed_size;      /* bytes */
  uint32_t num_rounds;

  uint32_t input_size;      /* bytes */
  uint32_t output_size;     /* bytes */
  uint32_t view_size;       /* bytes */
  uint32_t view_round_size; /* bits */

  uint32_t collapsed_challenge_size;       /* bytes */
  uint32_t unruh_without_input_bytes_size; /* bytes */
  uint32_t unruh_with_input_bytes_size;    /* bytes */
  uint32_t max_signature_size;             /* bytes */
};

picnic_instance_t* get_instance(picnic_params_t param);
const picnic_instance_t* picnic_instance_get(picnic_params_t param);

bool fis_sign(const picnic_instance_t* pp, const uint8_t* plaintext, const uint8_t* private_key,
              const uint8_t* public_key, const uint8_t* msg, size_t msglen, uint8_t* sig,
              size_t* siglen);

bool fis_verify(const picnic_instance_t* pp, const uint8_t* plaintext, const uint8_t* public_key,
                const uint8_t* msg, size_t msglen, const uint8_t* sig, size_t siglen);

void visualize_signature(FILE* out, const picnic_instance_t* pp, const uint8_t* msg, size_t msglen,
                         const uint8_t* sig, size_t siglen);

#endif
