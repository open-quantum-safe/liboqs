#ifndef XMSS_FIPS202_H
#define XMSS_FIPS202_H

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136

/* Evaluates SHAKE-128 on `inlen' bytes in `in', according to FIPS-202.
 * Writes the first `outlen` bytes of output to `out`.
 */
void shake128(unsigned char *out, unsigned long long outlen,
              const unsigned char *in, unsigned long long inlen);

/* Evaluates SHAKE-256 on `inlen' bytes in `in', according to FIPS-202.
 * Writes the first `outlen` bytes of output to `out`.
 */
void shake256(unsigned char *out, unsigned long long outlen,
              const unsigned char *in, unsigned long long inlen);

#endif
