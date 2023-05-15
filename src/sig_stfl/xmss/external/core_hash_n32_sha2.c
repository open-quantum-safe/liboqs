#include <oqs/sha2.h>
#include "hash.h"

int core_hash(const xmss_params *params,
              unsigned char *out,
              const unsigned char *in, unsigned long long inlen)
{
    (void)params;
    OQS_SHA2_sha256(out, in, inlen);
    return 0;
}
