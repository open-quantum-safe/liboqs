#ifndef __KEYGEN_H__
#define __KEYGEN_H__

#include "xof.h"
#include "expand_mq.h"

/* Deal with namespacing */
#define KeyGen MQOM_NAMESPACE(KeyGen)

int KeyGen(const uint8_t seed_key[2 * MQOM2_PARAM_SEED_SIZE], uint8_t sk[MQOM2_SK_SIZE], uint8_t pk[MQOM2_PK_SIZE]);

#endif /* __KEYGEN_H__ */
