
#include "picnic.h"

#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "lowmc.h"
#include "picnic_impl.h"
#include "randomness.h"

const picnic_instance_t* picnic_instance_get(picnic_params_t param) {
  return get_instance(param);
}

size_t picnic_get_max_sig_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return instance->max_signature_size;
}

size_t picnic_get_private_key_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return 1 + instance->input_size + instance->output_size;
}

size_t picnic_get_public_key_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return 1 + (instance->output_size << 1);
}

bool picnic_create_key(picnic_params_t param, uint8_t* private_key, size_t* private_size,
                       uint8_t* public_key, size_t* public_size) {
  if (!private_key || !private_size || !public_key || !public_size) {
    return false;
  }

  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return false;
  }

  const size_t private_key_size = picnic_get_private_key_size(param);
  if (*private_size < private_key_size) {
    return false;
  }

  const size_t input_size = instance->input_size;
  const size_t output_size = instance->output_size;

  uint8_t* sk = private_key + 1;
  uint8_t* pt = private_key + 1 + input_size;

  // generate private key
  private_key[0] = param;
  // random secret key
  if (!rand_bytes(sk, input_size)) {
    return false;
  }
  // random plain text
  if (!rand_bytes(pt, output_size)) {
    return false;
  }
  // encrypt plaintext under secret key
  if (!picnic_sk_to_pk(private_key, private_key_size, public_key, public_size)) {
    return false;
  }

  *private_size = private_key_size;
  return true;
}

bool picnic_sk_to_pk(const uint8_t* private_key, size_t private_size, uint8_t* public_key,
                     size_t* public_size) {

  if (!private_key || !private_size || !public_key || !public_size) {
    return false;
  }

  const picnic_params_t param       = private_key[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return false;
  }

  const size_t private_key_size = picnic_get_private_key_size(param);
  const size_t public_key_size  = picnic_get_public_key_size(param);

  if (private_size != private_key_size || *public_size < public_key_size) {
    return false;
  }

  const uint8_t* sk = private_key + 1;
  const uint8_t* pt = private_key + 1 + instance->input_size;

  *public_size  = public_key_size;
  public_key[0] = param;

  // copy plaintext
  memcpy(public_key + 1, pt, instance->output_size);

  mzd_local_t* plaintext = mzd_local_init_ex(1, instance->lowmc.n, false);
  mzd_local_t* privkey   = mzd_local_init_ex(1, instance->lowmc.k, false);

  mzd_from_char_array(plaintext, pt, instance->output_size);
  mzd_from_char_array(privkey, sk, instance->input_size);

  // compute public key
  mzd_local_t* ciphertext = lowmc_call(&instance->lowmc, privkey, plaintext);
  mzd_to_char_array(public_key + 1 + instance->output_size, ciphertext, instance->output_size);

  mzd_local_free(ciphertext);
  mzd_local_free(privkey);
  mzd_local_free(plaintext);

  return true;
}

bool picnic_verify_key(const uint8_t* private_key, size_t private_size, const uint8_t* public_key,
                       size_t public_size) {
  if (!private_key || !private_size || !public_key) {
    return false;
  }

  const picnic_params_t param       = private_key[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return false;
  }

  const size_t private_key_size = picnic_get_private_key_size(param);
  const size_t public_key_size  = picnic_get_public_key_size(param);
  if (private_size != private_key_size || public_size != public_key_size) {
    return false;
  }

  const uint8_t* sk = private_key + 1;
  const uint8_t* pt = private_key + 1 + instance->input_size;
  const uint8_t* pk_pt = public_key + 1;
  const uint8_t* pk_pk = public_key + 1 + instance->output_size;

  // check param and plaintext
  if (param != public_key[0] || memcmp(pt, pk_pt, instance->output_size) != 0) {
    return false;
  }

  mzd_local_t* plaintext = mzd_local_init_ex(1, instance->lowmc.n, false);
  mzd_local_t* privkey   = mzd_local_init_ex(1, instance->lowmc.k, false);

  mzd_from_char_array(plaintext, pt, instance->output_size);
  mzd_from_char_array(privkey, sk, instance->input_size);

  // compute public key
  mzd_local_t* ciphertext = lowmc_call(&instance->lowmc, privkey, plaintext);

  uint8_t buffer[MAX_LOWMC_BLOCK_SIZE];
  mzd_to_char_array(buffer, ciphertext, instance->output_size);

  mzd_local_free(ciphertext);
  mzd_local_free(privkey);
  mzd_local_free(plaintext);

  return memcmp(buffer, pk_pk, instance->output_size) == 0;
}

bool picnic_sign(const uint8_t* private_key, size_t private_size, const uint8_t* public_key,
                 size_t public_size, const uint8_t* msg, size_t msglen, uint8_t* sig,
                 size_t* siglen) {
  if (!private_key || !private_size) {
    return false;
  }

  const picnic_params_t param       = private_key[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return false;
  }

  const size_t private_key_size = picnic_get_private_key_size(param);
  const size_t public_key_size  = picnic_get_public_key_size(param);

  // if we do not have the public key, we need the full private key
  if ((!public_size || public_key_size != public_size) && (private_size != private_key_size)) {
    return false;
  }

  if (!public_key || !public_size) {
    // no public key given ... recompute it
    uint8_t pk[PICNIC_MAX_PUBLIC_KEY_SIZE];
    size_t pk_size = sizeof(pk);

    if (!picnic_sk_to_pk(private_key, private_size, pk, &pk_size)) {
      return false;
    }

    const uint8_t* sk = private_key + 1;
    const uint8_t* pt = private_key + 1 + instance->input_size;
    const uint8_t* pk_pk = public_key + 1 + instance->output_size;

    return fis_sign(instance, pt, sk, pk_pk, msg, msglen, sig, siglen);
  }

  const uint8_t* sk = private_key + 1;
  const uint8_t* pt = public_key + 1;
  const uint8_t* pk = public_key + 1 + instance->output_size;

  return fis_sign(instance, pt, sk, pk, msg, msglen, sig, siglen);
}

bool picnic_verify(const uint8_t* public_key, size_t public_size, const uint8_t* msg, size_t msglen,
                   const uint8_t* sig, size_t siglen) {
  if (!public_key || !public_size || !msg || !sig || !siglen) {
    return false;
  }

  const picnic_params_t param       = public_key[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return false;
  }

  const size_t public_key_size = picnic_get_public_key_size(param);
  if (public_size != public_key_size) {
    return false;
  }

  const uint8_t* pt = public_key + 1;
  const uint8_t* pk = public_key + 1 + instance->output_size;

  return fis_verify(instance, pt, pk, msg, msglen, sig, siglen);
}

void picnic_visualize(FILE* out, const uint8_t* public_key, size_t public_key_size,
                      const uint8_t* msg, size_t msglen, const uint8_t* sig, size_t siglen) {
  if (!public_key || !public_key_size) {
    return;
  }

  const picnic_params_t param       = public_key[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return;
  }

  visualize_signature(out, instance, msg, msglen, sig, siglen);
}

const char* picnic_get_param_name(picnic_params_t parameters) {
    switch (parameters) {
    case Picnic_L1_FS:
        return "Picnic_L1_FS";
    case Picnic_L1_UR:
        return "Picnic_L1_UR";
    case Picnic_L3_FS:
        return "Picnic_L3_FS";
    case Picnic_L3_UR:
        return "Picnic_L3_UR";
    case Picnic_L5_FS:
        return "Picnic_L5_FS";
    case Picnic_L5_UR:
        return "Picnic_L5_UR";
    default:
        return "Unknown parameter set";
    }
}
