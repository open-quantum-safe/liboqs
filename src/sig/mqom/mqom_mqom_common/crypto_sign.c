#ifdef SUPERCOP
#include "crypto_sign.h"
#include "crypto_declassify.h"
#else
#include "api.h"
#endif

#include "sign.h"
#include <limits.h>
#include <stdint.h>

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
#ifdef SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#else
extern int randombytes(unsigned char* x, unsigned long long xlen);
#endif
#else
#include "randombytes.h"
#endif

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
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
	size_t message_len;

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS) && (SIZE_MAX < ULLONG_MAX)
	if (mlen > (unsigned long long)SIZE_MAX) {
		goto err;
	}
#endif
	message_len = (size_t)mlen;

	// Sample mseed
	uint8_t mseed[MQOM3_PARAM_SEED_SIZE];
#if defined(SUPERCOP) || defined(MQOM3_FOR_LIBOQS)
	randombytes(mseed, MQOM3_PARAM_SEED_SIZE);
#else
	ret = randombytes(mseed, MQOM3_PARAM_SEED_SIZE);
	ERR(ret, err);
#endif
	// Sample salt
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];

#if defined(SUPERCOP) || defined(MQOM3_FOR_LIBOQS)
	randombytes(salt, MQOM3_PARAM_SALT_SIZE);
#else
	ret = randombytes(salt, MQOM3_PARAM_SALT_SIZE);
	ERR(ret, err);
#endif

#ifdef SUPERCOP
        /* Salt declassification (as it is public) for SUPERCOP */
        crypto_declassify(salt, MQOM3_PARAM_SALT_SIZE);
#endif

	// Build the signature
	ret = Sign(sk, m, message_len, salt, mseed, sig);
	ERR(ret, err);
	if (siglen != NULL) {
		*siglen = (unsigned long long) MQOM3_SIG_SIZE;
	}

	ret = 0;
err:
	if ((ret != 0) && (siglen != NULL)) {
		*siglen = 0;
	}
	/* mseed is the master seed: together with the public signature it regenerates
	 * every leaf of the GGM tree, hence the secret witness. Sign() wipes its own
	 * copy, but this one is ours to clear - same discipline as
	 * crypto_sign_prepare(). salt is public, wiped for symmetry only. */
	mqom_cleanse(mseed, sizeof(mseed));
	mqom_cleanse(salt, sizeof(salt));
	return ret;
}

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
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
	size_t message_len;

	if (mlen > ((unsigned long long)SIZE_MAX - MQOM3_SIG_SIZE)) {
		goto err;
	}
	message_len = (size_t)mlen;

	uint8_t *message = sm;
	memmove(message, m, message_len);
	ret = crypto_sign_signature(sm + message_len, smlen, message, message_len, sk);
	ERR(ret, err);
	if (smlen != NULL) {
		*smlen += mlen;
	}

	ret = 0;
err:
	if ((ret != 0) && (smlen != NULL)) {
		*smlen = 0;
	}
	return ret;
}

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
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
#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS) && (SIZE_MAX < ULLONG_MAX)
	if (mlen > (unsigned long long)SIZE_MAX) {
		return -1;
	}
#endif
	if (siglen != (unsigned long long) MQOM3_SIG_SIZE) {
		return -1;
	}

	return Verify(pk, m, mlen, sig);
}

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
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

	if (mlen == NULL) {
		return -1;
	}

	if (
#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS) && (SIZE_MAX < ULLONG_MAX)
	    (smlen > (unsigned long long)SIZE_MAX) ||
#endif
	    (smlen < (unsigned long long) MQOM3_SIG_SIZE)) {
		*mlen = 0;
		return -1;
	}

	const uint8_t *message = sm;
	*mlen = smlen - (unsigned long long) MQOM3_SIG_SIZE;

	int ret = crypto_sign_verify(sm + (*mlen), MQOM3_SIG_SIZE, message, *mlen, pk);
	if (ret) {
		/* Report no message on failure: m has not been written, so a caller that
		 * ignores the return code must not be handed a non-zero length pointing
		 * at uninitialised memory. Matches the Dilithium/Falcon convention. */
		*mlen = 0;
		return ret;
	}

	memmove(m, message, *mlen);
	return 0;
}
