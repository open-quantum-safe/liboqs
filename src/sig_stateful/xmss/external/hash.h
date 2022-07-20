#ifndef XMSS_HASH_H
#define XMSS_HASH_H

#include <stdint.h>
#include "params.h"

void addr_to_bytes(unsigned char *bytes, const uint32_t addr[8]);

int prf(const xmss_params *params,
        unsigned char *out, const unsigned char in[32],
        const unsigned char *key);

int h_msg(const xmss_params *params,
          unsigned char *out,
          const unsigned char *in, unsigned long long inlen,
          const unsigned char *key, const unsigned int keylen);

int thash_h(const xmss_params *params,
            unsigned char *out, const unsigned char *in,
            const unsigned char *pub_seed, uint32_t addr[8]);

int thash_f(const xmss_params *params,
            unsigned char *out, const unsigned char *in,
            const unsigned char *pub_seed, uint32_t addr[8]);

int hash_message(const xmss_params *params, unsigned char *out,
                 const unsigned char *R, const unsigned char *root,
                 unsigned long long idx,
                 unsigned char *m_with_prefix, unsigned long long mlen);

#ifdef FORWARD_SECURE
int hash_prg(const xmss_params *params, unsigned char *out, unsigned char *next_seed,
              const unsigned char *in, const unsigned char *pk, const uint32_t addr[8]);
#endif
#endif
