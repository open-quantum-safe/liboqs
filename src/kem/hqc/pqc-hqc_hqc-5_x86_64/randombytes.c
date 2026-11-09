#include <stdint.h>

#include "randombytes.h"
#include "oqs/rand.h"

int randombytes(uint8_t *out, size_t outlen) {
  OQS_randombytes(out, outlen);
  return 0;
}

