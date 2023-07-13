#if !defined(LM_COMMON_H_)
#define LM_COMMON_H_

#include <stddef.h>
#include "common_defs.h"

size_t lm_get_public_key_len(param_set_t lm_type);
size_t lm_get_signature_len(param_set_t lm_type,
                            param_set_t lm_ots_type);

bool lm_look_up_parameter_set(param_set_t parameter_set,
     unsigned *h, unsigned *n, unsigned *height);

/* The format of an LM public key; it consists of: */
#define LM_PUB_PARM_SET    0       /* The parameter set (4 bytes) */
#define LM_PUB_OTS_PARM_SET 4      /* The OTS parameter set (4 bytes) */
#define LM_PUB_I            8      /* Our nonce (I) value (32 or 64 bytes) */
/* The root value comes here */

#endif /* LM_COMMON_H_ */
