#ifndef params_H
#define params_H
#include "crypto_core_inv3sntrup1277.h"
#include "crypto_core_invsntrup1277.h"
#include "crypto_core_mult3sntrup1277.h"
#include "crypto_core_multsntrup1277.h"
#include "crypto_core_scale3sntrup1277.h"
#include "crypto_core_weightsntrup1277.h"
#include "crypto_core_wforcesntrup1277.h"
#include "crypto_decode_1277x2627.h"
#include "crypto_decode_1277x3.h"
#include "crypto_decode_1277x7879.h"
#include "crypto_decode_1277xint16.h"
#include "crypto_decode_1277xint32.h"
#include "crypto_encode_1277x2627.h"
#include "crypto_encode_1277x2627round.h"
#include "crypto_encode_1277x3.h"
#include "crypto_encode_1277x7879.h"
#include "crypto_encode_1277xfreeze3.h"
#include "crypto_encode_1277xint16.h"
#include "crypto_encode_int16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1847.h"


#define q31 272557 /* floor(2^31/q) */
#define q27 17035 /* closest integer to 2^27/q */
#define q18 33 /* closest integer to 2^18/q */
#define q14 2 /* closest integer to 2^14/q */
#define p 1277
#define q 7879
#define w 492

#define ppadsort 1280

#define crypto_verify_clen PQCLEAN_SNTRUP1277_CLEAN_crypto_verify_1847

#define Rq_bytes PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x7879_STRBYTES
#define Rq_encode PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x7879
#define Rq_decode PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x7879

#define Rounded_bytes PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x2627_STRBYTES
#define Rounded_decode PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x2627

#define Round_and_encode PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x2627round

#define Small_bytes PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x3_STRBYTES
#define Small_encode PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x3
#define Small_decode PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x3

#define crypto_encode_pxfreeze3 PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277xfreeze3

#define crypto_decode_pxint32 PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277xint32

#define crypto_decode_pxint16 PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277xint16

#define crypto_encode_pxint16 PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277xint16

#define crypto_core_wforce PQCLEAN_SNTRUP1277_CLEAN_crypto_core_wforcesntrup1277

#define crypto_core_scale3 PQCLEAN_SNTRUP1277_CLEAN_crypto_core_scale3sntrup1277

#define crypto_core_inv PQCLEAN_SNTRUP1277_CLEAN_crypto_core_invsntrup1277

#define crypto_core_inv3 PQCLEAN_SNTRUP1277_CLEAN_crypto_core_inv3sntrup1277

#define crypto_core_mult3 PQCLEAN_SNTRUP1277_CLEAN_crypto_core_mult3sntrup1277

#define crypto_core_mult PQCLEAN_SNTRUP1277_CLEAN_crypto_core_multsntrup1277

#endif
