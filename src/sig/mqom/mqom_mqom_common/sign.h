#ifndef __SIGN_H__
#define __SIGN_H__

#include "common.h"

/* Deal with namespacing */
#define Sign_default MQOM_NAMESPACE(Sign_default)
#define Sign_memopt MQOM_NAMESPACE(Sign_memopt)
#define Verify_default MQOM_NAMESPACE(Verify_default)
#define Verify_memopt MQOM_NAMESPACE(Verify_memopt)

int Sign_default(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t sig[MQOM3_SIG_SIZE]);

int Sign_memopt(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t sig[MQOM3_SIG_SIZE]);

int Verify_default(const uint8_t pk[MQOM3_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM3_SIG_SIZE]);

int Verify_memopt(const uint8_t pk[MQOM3_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM3_SIG_SIZE]);

#ifdef SIGN_MEMOPT
#define Sign Sign_memopt
#else
#define Sign Sign_default
#endif

#ifdef VERIFY_MEMOPT
#define Verify Verify_memopt
#else
#define Verify Verify_default
#endif

#endif /* __SIGN_H__ */
