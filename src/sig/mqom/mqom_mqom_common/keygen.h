#ifndef __KEYGEN_H__
#define __KEYGEN_H__

#include "xof.h"
#include "expand_mq.h"

/* Deal with namespacing */
#define KeyGen MQOM_NAMESPACE(KeyGen)

int KeyGen(const uint8_t seed_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], uint8_t sk[MQOM3_SK_SIZE], uint8_t pk[MQOM3_PK_SIZE]);

#endif /* __KEYGEN_H__ */
