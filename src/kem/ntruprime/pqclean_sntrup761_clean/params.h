#ifndef params_H
#define params_H
#include "crypto_core_inv3sntrup761.h"
#include "crypto_core_invsntrup761.h"
#include "crypto_core_mult3sntrup761.h"
#include "crypto_core_multsntrup761.h"
#include "crypto_core_scale3sntrup761.h"
#include "crypto_core_weightsntrup761.h"
#include "crypto_core_wforcesntrup761.h"
#include "crypto_decode_761x1531.h"
#include "crypto_decode_761x3.h"
#include "crypto_decode_761x4591.h"
#include "crypto_decode_761xint16.h"
#include "crypto_decode_761xint32.h"
#include "crypto_encode_761x1531.h"
#include "crypto_encode_761x1531round.h"
#include "crypto_encode_761x3.h"
#include "crypto_encode_761x4591.h"
#include "crypto_encode_761xfreeze3.h"
#include "crypto_encode_761xint16.h"
#include "crypto_encode_int16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1039.h"


#define q31 467759 /* floor(2^31/q) */
#define q27 29235 /* closest integer to 2^27/q */
#define q18 57 /* closest integer to 2^18/q */
#define q14 4 /* closest integer to 2^14/q */
#define p 761
#define q 4591
#define w 286

#define ppadsort 761

#define crypto_verify_clen PQCLEAN_SNTRUP761_CLEAN_crypto_verify_1039

#define Rq_bytes PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x4591_STRBYTES
#define Rq_encode PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x4591
#define Rq_decode PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761x4591

#define Rounded_bytes PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761x1531_STRBYTES
#define Rounded_decode PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761x1531

#define Round_and_encode PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x1531round

#define Small_bytes PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x3_STRBYTES
#define Small_encode PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x3
#define Small_decode PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761x3

#define crypto_encode_pxfreeze3 PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761xfreeze3

#define crypto_decode_pxint32 PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761xint32

#define crypto_decode_pxint16 PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761xint16

#define crypto_encode_pxint16 PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761xint16

#define crypto_core_wforce PQCLEAN_SNTRUP761_CLEAN_crypto_core_wforcesntrup761

#define crypto_core_scale3 PQCLEAN_SNTRUP761_CLEAN_crypto_core_scale3sntrup761

#define crypto_core_inv PQCLEAN_SNTRUP761_CLEAN_crypto_core_invsntrup761

#define crypto_core_inv3 PQCLEAN_SNTRUP761_CLEAN_crypto_core_inv3sntrup761

#define crypto_core_mult3 PQCLEAN_SNTRUP761_CLEAN_crypto_core_mult3sntrup761

#define crypto_core_mult PQCLEAN_SNTRUP761_CLEAN_crypto_core_multsntrup761

#endif
