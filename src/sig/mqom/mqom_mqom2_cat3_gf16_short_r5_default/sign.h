#ifndef __SIGN_H__
#define __SIGN_H__

#include "common.h"

/* Deal with namespacing */
#define SampleChallenge MQOM_NAMESPACE(SampleChallenge)
#define Sign MQOM_NAMESPACE(Sign)
#define Verify_default MQOM_NAMESPACE(Verify_default)
#define Verify_memopt MQOM_NAMESPACE(Verify_memopt)

int SampleChallenge(const uint8_t hash[MQOM2_PARAM_DIGEST_SIZE], uint16_t i_star[MQOM2_PARAM_TAU], uint8_t nonce[4]);

int Sign(const uint8_t sk[MQOM2_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t mseed[MQOM2_PARAM_SEED_SIZE], uint8_t sig[MQOM2_SIG_SIZE]);

int Verify_default(const uint8_t pk[MQOM2_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM2_SIG_SIZE]);

int Verify_memopt(const uint8_t pk[MQOM2_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM2_SIG_SIZE]);

#ifdef VERIFY_MEMOPT
#define Verify Verify_memopt
#else
#define Verify Verify_default
#endif

#endif /* __SIGN_H__ */
