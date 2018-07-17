#ifndef FIPS202_H
#define FIPS202_H

#define SHAKE128_RATE 168
#define SHA3_256_RATE 136
#define SHAKE256_RATE 136

void shake128_absorb(uint64_t *s, const unsigned char *input, unsigned int inputByteLen);
void shake128_squeezeblocks(unsigned char *output, unsigned long long nblocks, uint64_t *s);
void shake128(unsigned char *output, unsigned long long outputByteLen, const unsigned char *input, unsigned long long inputByteLen);
void shake256(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen);
void sha3256(unsigned char *output, const unsigned char *input, unsigned int inputByteLen);
void cshake128_simple(unsigned char *output, unsigned long long outlen, uint16_t cstm, const unsigned char *in, unsigned long long inlen);
void cshake256_simple(unsigned char *output, unsigned long long outlen, uint16_t cstm, const unsigned char *in, unsigned long long inlen);

#endif
