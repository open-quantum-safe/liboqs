#ifdef SUPERCOP
#include "crypto_sign.h"
#include "crypto_declassify.h"
#else
#include "api.h"
#endif

#include "sign.h"

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
#ifdef SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#else
extern int randombytes(unsigned char* x, unsigned long long xlen);
#endif
#else
#include "randombytes.h"
#endif

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
int crypto_sign_signature(uint8_t *sig,
                          unsigned long long *siglen,
                          const uint8_t *m,
                          unsigned long long mlen,
                          const uint8_t *sk)
#else
int
crypto_sign_signature(unsigned char  *sig, size_t *siglen,
                      const unsigned char  *m, size_t mlen,
                      const unsigned char  *sk)
#endif
{
	int ret = -1;

	// Sample mseed
	uint8_t mseed[MQOM2_PARAM_SEED_SIZE];
#if defined(SUPERCOP) || defined(MQOM2_FOR_LIBOQS)
	randombytes(mseed, MQOM2_PARAM_SEED_SIZE);
#else
	ret = randombytes(mseed, MQOM2_PARAM_SEED_SIZE);
	ERR(ret, err);
#endif
	// Sample salt
	uint8_t salt[MQOM2_PARAM_SALT_SIZE];

#if defined(SUPERCOP) || defined(MQOM2_FOR_LIBOQS)
	randombytes(salt, MQOM2_PARAM_SALT_SIZE);
#else
	ret = randombytes(salt, MQOM2_PARAM_SALT_SIZE);
	ERR(ret, err);
#endif

#ifdef SUPERCOP
        /* Salt declassification (as it is public) for SUPERCOP */
        crypto_declassify(salt, MQOM2_PARAM_SALT_SIZE);
#endif

	// Build the signature
	ret = Sign(sk, m, mlen, salt, mseed, sig);
	ERR(ret, err);
	if (siglen != NULL) {
		*siglen = (unsigned long long) MQOM2_SIG_SIZE;
	}

	ret = 0;
err:
	return ret;
}

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
int crypto_sign(
    unsigned char *sm, unsigned long long *smlen,
    const unsigned char *m, unsigned long long mlen,
    const unsigned char *sk)
#else
int
crypto_sign(unsigned char *sm, size_t *smlen,
            const unsigned char *m, size_t mlen,
            const unsigned char *sk)
#endif
{
	int ret = -1;

	uint8_t *message = sm;
	memmove(message, m, mlen);
	ret = crypto_sign_signature(sm + mlen, smlen, message, mlen, sk);
	ERR(ret, err);
	if (smlen != NULL) {
		*smlen += mlen;
	}

	ret = 0;
err:
	return ret;
}

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
int crypto_sign_verify(const uint8_t *sig,
                       unsigned long long siglen,
                       const uint8_t *m,
                       unsigned long long mlen,
                       const uint8_t *pk)
#else
int
crypto_sign_verify(const unsigned char  *sig, size_t siglen,
                   const unsigned char  *m, size_t mlen,
                   const unsigned char  *pk)
#endif
{
	if (siglen != (unsigned long long) MQOM2_SIG_SIZE) {
		return -1;
	}

	return Verify(pk, m, mlen, sig);
}

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
int crypto_sign_open(
    unsigned char *m, unsigned long long *mlen,
    const unsigned char *sm, unsigned long long smlen,
    const unsigned char *pk)
#else
int
crypto_sign_open(unsigned char *m, size_t *mlen,
                 const unsigned char *sm, size_t smlen,
                 const unsigned char *pk)
#endif
{

	if (smlen < (unsigned long long) MQOM2_SIG_SIZE) {
		return -1;
	}

	const uint8_t *message = sm;
	*mlen = smlen - (unsigned long long) MQOM2_SIG_SIZE;

	int ret = crypto_sign_verify(sm + (*mlen), MQOM2_SIG_SIZE, message, *mlen, pk);
	if (ret) {
		return ret;
	}

	memmove(m, message, *mlen);
	return 0;
}

