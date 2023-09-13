#ifndef PQCLEAN_MCELIECE348864_CLEAN_CRYPTO_HASH_H
#define PQCLEAN_MCELIECE348864_CLEAN_CRYPTO_HASH_H

#include "fips202.h"

#define crypto_hash_32b(out, in, inlen) \
    shake256(out, 32, in, inlen)

#define shake(out,outlen,in,inlen) \
    shake256(out, outlen, in, inlen)

#endif
