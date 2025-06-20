// SPDX-License-Identifier: Apache-2.0 and Unknown
//
/*
NIST-developed software is provided by NIST as a public service. You may use,
copy, and distribute copies of the software in any medium, provided that you
keep intact this entire notice. You may improve, modify, and create derivative
works of the software or any portion of the software, and you may copy and
distribute such modifications or works. Modified works should carry a notice
stating that you changed the software and should note the date and nature of any
such change. Please explicitly acknowledge the National Institute of Standards
and Technology as the source of the software.

NIST-developed software is expressly provided "AS IS." NIST MAKES NO WARRANTY OF
ANY KIND, EXPRESS, IMPLIED, IN FACT, OR ARISING BY OPERATION OF LAW, INCLUDING,
WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, NON-INFRINGEMENT, AND DATA ACCURACY. NIST NEITHER REPRESENTS
NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR
ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT WARRANT OR MAKE
ANY REPRESENTATIONS REGARDING THE USE OF THE SOFTWARE OR THE RESULTS THEREOF,
INCLUDING BUT NOT LIMITED TO THE CORRECTNESS, ACCURACY, RELIABILITY, OR
USEFULNESS OF THE SOFTWARE.

You are solely responsible for determining the appropriateness of using and
distributing the software and you assume all risks associated with its use,
including but not limited to the risks and costs of program errors, compliance
with applicable laws, damage to or loss of data, programs or equipment, and the
unavailability or interruption of operation. This software is not intended to be
used in any situation where a failure could cause risk of injury or damage to
property. The software developed by NIST employees is not subject to copyright
protection within the United States.
*/

#include <rng.h>
#include <string.h>

#include <aes.h>

#ifdef ENABLE_CT_TESTING
#include <valgrind/memcheck.h>
#endif

#define RNG_SUCCESS 0
#define RNG_BAD_MAXLEN -1
#define RNG_BAD_OUTBUF -2
#define RNG_BAD_REQ_LEN -3

static inline void AES256_ECB(const unsigned char *key,
                              const unsigned char *ctr, unsigned char *buffer) {
  AES_ECB_encrypt(ctr, key, buffer);
}

typedef struct {
  unsigned char Key[32];
  unsigned char V[16];
  int reseed_counter;
} AES256_CTR_DRBG_struct;

void AES256_CTR_DRBG_Update(const unsigned char *provided_data,
                            unsigned char *Key, unsigned char *V);

AES256_CTR_DRBG_struct DRBG_ctx;

#ifndef CTRDRBG_TEST_BENCH
static
#endif
    void
    randombytes_init_nist(unsigned char *entropy_input,
                          unsigned char *personalization_string,
                          int security_strength) {
  unsigned char seed_material[48];

  (void)security_strength; // Unused parameter
  memcpy(seed_material, entropy_input, 48);
  if (personalization_string)
    for (int i = 0; i < 48; i++) {
      seed_material[i] ^= personalization_string[i];
    }
  memset(DRBG_ctx.Key, 0x00, 32);
  memset(DRBG_ctx.V, 0x00, 16);
  AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
  DRBG_ctx.reseed_counter = 1;
}

#ifndef CTRDRBG_TEST_BENCH
static
#endif
    int
    randombytes_nist(unsigned char *x, size_t xlen) {
  unsigned char block[16];
  size_t i = 0;

  while (xlen > 0) {
    // increment V
    for (int j = 15; j >= 0; j--) {
      if (DRBG_ctx.V[j] == 0xff) {
        DRBG_ctx.V[j] = 0x00;
      } else {
        DRBG_ctx.V[j]++;
        break;
      }
    }
    AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
    if (xlen > 15) {
      memcpy(x + i, block, 16);
      i += 16;
      xlen -= 16;
    } else {
      memcpy(x + i, block, xlen);
      i += xlen;
      xlen = 0;
    }
  }
  AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
  DRBG_ctx.reseed_counter++;

  return 0;
}

void AES256_CTR_DRBG_Update(const unsigned char *provided_data,
                            unsigned char *Key, unsigned char *V) {
  unsigned char temp[48];

  for (int i = 0; i < 3; i++) {
    // increment V
    for (int j = 15; j >= 0; j--) {
      if (V[j] == 0xff) {
        V[j] = 0x00;
      } else {
        V[j]++;
        break;
      }
    }

    AES256_ECB(Key, V, temp + 16 * i);
  }
  if (provided_data != NULL)
    for (int i = 0; i < 48; i++) {
      temp[i] ^= provided_data[i];
    }
  memcpy(Key, temp, 32);
  memcpy(V, temp + 32, 16);
}

#ifdef RANDOMBYTES_C
SQISIGN_API
int randombytes(unsigned char *random_array, unsigned long long nbytes) {
  int ret = randombytes_nist(random_array, nbytes);
#ifdef ENABLE_CT_TESTING
  VALGRIND_MAKE_MEM_UNDEFINED(random_array, ret);
#endif
  return ret;
}

SQISIGN_API
void randombytes_init(unsigned char *entropy_input,
                      unsigned char *personalization_string,
                      int security_strength) {
  randombytes_init_nist(entropy_input, personalization_string,
                        security_strength);
}
#endif
