#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "picnic.h"

static int picnic_sign_verify(const picnic_params_t param) {
  static const uint8_t m[] = "test message";

  const size_t max_signature_size = picnic_get_max_sig_size(param);
  if (!max_signature_size) {
    /* not supported */
    return -2;
  }

  uint8_t private_key[PICNIC_MAX_PRIVATE_KEY_SIZE];
  uint8_t public_key[PICNIC_MAX_PUBLIC_KEY_SIZE];

  size_t private_key_size = sizeof(private_key);
  size_t public_key_size  = sizeof(public_key);

  /* Create a key pair */
  printf("Creating key pair ... ");
  if (!picnic_create_key(param, private_key, &private_key_size, public_key, &public_key_size)) {
    printf("FAILED!\n");
    return -1;
  }
  printf("OK\n");

  /* Valid key pair */
  printf("Validating key pair ... ");
  if (!picnic_verify_key(private_key, private_key_size, public_key, public_key_size)) {
    printf("FAILED!\n");
    return -1;
  }
  printf("OK\n");

  uint8_t* sig  = malloc(max_signature_size);
  size_t siglen = max_signature_size;
  int ret       = 0;

  /* Sign a message */
  printf("Signing message ... ");
  if (picnic_sign(private_key, private_key_size, public_key, public_key_size, m, sizeof(m), sig,
                  &siglen)) {
    printf("OK\nVerifying signature ... ");
    /* Verify signature */
    if (!picnic_verify(public_key, public_key_size, m, sizeof(m), sig, siglen)) {
      ret = -1;
      printf("FAILED!\n");
    } else {
      printf("OK\n");
    }
  } else {
    ret = -1;
    printf("FAILED!\n");
  }

  free(sig);
  return ret;
}

int main() {
  int ret = 0;
  for (unsigned int param = Picnic_L1_FS; param < PARAMETER_SET_MAX_INDEX; ++param) {
    printf("testing: %d ... ", param);
    const int r = picnic_sign_verify(param);
    if (r == -2) {
      printf("SKIPPED\n");
    } else if (r) {
      printf("FAILED\n");
      ret = -1;
    } else {
      printf("OK\n");
    }
  }

  return ret;
}
