#ifndef CRYPTO_HASH_SHA256_H
#define CRYPTO_HASH_SHA256_H

static int crypto_hashblocks_sha256(unsigned char *statebytes,const unsigned char *in,unsigned long long inlen);

static int crypto_hash_sha256(unsigned char *out,const unsigned char *in,unsigned long long inlen);static

#define crypto_hash_sha256_BYTES 32

#endif
