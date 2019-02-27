#ifndef crypto_hash_blocks_H
#define crypto_hash_blocks_H

#define crypto_hashblocks_sha512 crypto_hashblocks

int crypto_hashblocks(unsigned char *statebytes, const unsigned char *in, unsigned long long inlen);

#endif
