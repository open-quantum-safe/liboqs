
// SPDX-License-Identifier: Apache-2.0

/* MAYO's AES entry points backed by the AES liboqs already links, so the copied
   sources need no patch to aes_ctr.h. Only listed in the META source lists, so it
   is built solely by the liboqs integration; aes_c.c serves every other build.
   Semantics follow aes_c.c: CTR keys on the seed and counts from an all-zero IV,
   ECB encrypts whole blocks under key. */

#include <stddef.h>
#include <stdint.h>

#include <aes.h>        // pqclean shim: aes128ctr_prf
#include <oqs/aes.h>    // OQS_AES128_ECB_enc
#include <aes_ctr.h>

int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen) {
    uint8_t nonce[12] = { 0 };
    (void) inputByteLen;
    aes128ctr_prf(output, outputByteLen, input, nonce);
    return (int) outputByteLen;
}

int AES_128_ECB_C(unsigned char *output, const unsigned char *input, size_t nblocks,
                  const unsigned char *key) {
    OQS_AES128_ECB_enc(input, nblocks * 16, key, output);
    return (int) (nblocks * 16);
}
