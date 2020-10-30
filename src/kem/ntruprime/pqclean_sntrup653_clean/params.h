#ifndef params_H
#define params_H
#include "crypto_core_inv3sntrup653.h"
#include "crypto_core_invsntrup653.h"
#include "crypto_core_mult3sntrup653.h"
#include "crypto_core_multsntrup653.h"
#include "crypto_core_scale3sntrup653.h"
#include "crypto_core_weightsntrup653.h"
#include "crypto_core_wforcesntrup653.h"
#include "crypto_decode_653x1541.h"
#include "crypto_decode_653x3.h"
#include "crypto_decode_653x4621.h"
#include "crypto_decode_653xint16.h"
#include "crypto_decode_653xint32.h"
#include "crypto_encode_653x1541.h"
#include "crypto_encode_653x1541round.h"
#include "crypto_encode_653x3.h"
#include "crypto_encode_653x4621.h"
#include "crypto_encode_653xfreeze3.h"
#include "crypto_encode_653xint16.h"
#include "crypto_encode_int16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_897.h"


#define q27 29045 /* closest integer to 2^27/q */
#define q18 57 /* closest integer to 2^18/q */
#define p 653
#define q 4621
#define w 288

#define ppadsort 653

#define crypto_verify_clen PQCLEAN_SNTRUP653_CLEAN_crypto_verify_897

#define Rq_bytes PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x4621_STRBYTES
#define Rq_encode PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x4621
#define Rq_decode PQCLEAN_SNTRUP653_CLEAN_crypto_decode_653x4621

#define Rounded_bytes PQCLEAN_SNTRUP653_CLEAN_crypto_decode_653x1541_STRBYTES
#define Rounded_decode PQCLEAN_SNTRUP653_CLEAN_crypto_decode_653x1541

#define Round_and_encode PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x1541round

#define Small_bytes PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x3_STRBYTES
#define Small_encode PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x3
#define Small_decode PQCLEAN_SNTRUP653_CLEAN_crypto_decode_653x3

#define crypto_encode_pxfreeze3 PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653xfreeze3

#define crypto_decode_pxint32 PQCLEAN_SNTRUP653_CLEAN_crypto_decode_653xint32

#define crypto_decode_pxint16 PQCLEAN_SNTRUP653_CLEAN_crypto_decode_653xint16

#define crypto_encode_pxint16 PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653xint16

#define crypto_core_wforce PQCLEAN_SNTRUP653_CLEAN_crypto_core_wforcesntrup653

#define crypto_core_scale3 PQCLEAN_SNTRUP653_CLEAN_crypto_core_scale3sntrup653

#define crypto_core_inv PQCLEAN_SNTRUP653_CLEAN_crypto_core_invsntrup653

#define crypto_core_inv3 PQCLEAN_SNTRUP653_CLEAN_crypto_core_inv3sntrup653

#define crypto_core_mult PQCLEAN_SNTRUP653_CLEAN_crypto_core_multsntrup653

#define crypto_core_mult3 PQCLEAN_SNTRUP653_CLEAN_crypto_core_mult3sntrup653

#endif
