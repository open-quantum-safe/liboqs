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

#include <string.h>

#include <rng.h>
#include "ctr_drbg.h"

#ifdef ENABLE_CT_TESTING
#include <valgrind/memcheck.h>
#endif

#define RNG_SUCCESS 0
#define RNG_BAD_MAXLEN -1
#define RNG_BAD_OUTBUF -2
#define RNG_BAD_REQ_LEN -3

CTR_DRBG_STATE drbg;

#ifndef CTRDRBG_TEST_BENCH
static
#endif
void
randombytes_init_aes_ni(unsigned char *entropy_input,
                        unsigned char *personalization_string,
                        int security_strength) {
  (void)security_strength; // fixed to 256
  CTR_DRBG_init(&drbg, entropy_input, personalization_string,
                (personalization_string == NULL) ? 0 : CTR_DRBG_ENTROPY_LEN);
}

#ifndef CTRDRBG_TEST_BENCH
static
#endif
int
randombytes_aes_ni(unsigned char *x, size_t xlen) {
  CTR_DRBG_generate(&drbg, x, xlen, NULL, 0);
  return RNG_SUCCESS;
}

#ifdef RANDOMBYTES_AES_NI
SQISIGN_API
int randombytes(unsigned char *random_array, unsigned long long nbytes) {
  int ret = randombytes_aes_ni(random_array, nbytes);
#ifdef ENABLE_CT_TESTING
  VALGRIND_MAKE_MEM_UNDEFINED(random_array, ret);
#endif
  return ret;
}

SQISIGN_API
void randombytes_init(unsigned char *entropy_input,
                      unsigned char *personalization_string,
                      int security_strength) {
  randombytes_init_aes_ni(entropy_input, personalization_string,
                          security_strength);
}
#endif
