#include "fastrandombytes.h"
#include "keccak.h"
#include "param.h"
#include "littleendian.h"
#include <string.h>

#define IV_SIZE 4 /* 32-bit IV */

static unsigned char key[CRYPTO_RANDOMBYTES];
static unsigned char iv[IV_SIZE];

/* r <-- KMAC256(key, iv, rlen)
 * iv++ */
void fastrandombytes(unsigned char *r, unsigned long long rlen)
{
	KMAC256(key, 8 * CRYPTO_RANDOMBYTES, iv, 8 * IV_SIZE, r, 8 * rlen, NULL, 0);
	store_32(iv, load_32(iv) + 1);
}

/* key <-- randomness 
 * iv <-- 0 */
void fastrandombytes_setseed(const unsigned char *randomness)
{
	memcpy(key, randomness, CRYPTO_RANDOMBYTES);
	memset(iv, 0, IV_SIZE);
}



