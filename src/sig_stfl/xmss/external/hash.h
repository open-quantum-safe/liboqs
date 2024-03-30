// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0
#ifndef XMSS_HASH_H
#define XMSS_HASH_H

#include <stdint.h>
#include "params.h"
#include "core_hash.h"

#define addr_to_bytes XMSS_INNER_NAMESPACE(addr_to_bytes)
void addr_to_bytes(unsigned char *bytes, const uint32_t addr[8]);

#define prf XMSS_INNER_NAMESPACE(prf)
int prf(const xmss_params *params,
        unsigned char *out, const unsigned char in[32],
        const unsigned char *key,
        unsigned char *buf);

#define prf_keygen XMSS_INNER_NAMESPACE(prf_keygen)
int prf_keygen(const xmss_params *params,
        unsigned char *out, const unsigned char *in,
        const unsigned char *key,
        unsigned char *buf);

#define h_msg XMSS_INNER_NAMESPACE(h_msg)
int h_msg(const xmss_params *params,
          unsigned char *out,
          const unsigned char *in, unsigned long long inlen,
          const unsigned char *key, const unsigned int keylen);

#define thash_h XMSS_INNER_NAMESPACE(thash_h)
int thash_h(const xmss_params *params,
            unsigned char *out, const unsigned char *in,
            const unsigned char *pub_seed, uint32_t addr[8],
            unsigned char *buf);

#define thash_f XMSS_INNER_NAMESPACE(thash_f)
int thash_f(const xmss_params *params,
            unsigned char *out, const unsigned char *in,
            const unsigned char *pub_seed, uint32_t addr[8],
            unsigned char *buf);

#define hash_message XMSS_INNER_NAMESPACE(hash_message)
int hash_message(const xmss_params *params, unsigned char *out,
                 const unsigned char *R, const unsigned char *root,
                 unsigned long long idx,
                 unsigned char *m_with_prefix, unsigned long long mlen);

#endif
