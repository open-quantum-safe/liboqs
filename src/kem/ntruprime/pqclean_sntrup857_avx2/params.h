#ifndef params_H
#define params_H
#include "crypto_core_inv3sntrup857.h"
#include "crypto_core_invsntrup857.h"
#include "crypto_core_mult3sntrup857.h"
#include "crypto_core_multsntrup857.h"
#include "crypto_core_scale3sntrup857.h"
#include "crypto_core_weightsntrup857.h"
#include "crypto_core_wforcesntrup857.h"
#include "crypto_decode_857x1723.h"
#include "crypto_decode_857x3.h"
#include "crypto_decode_857x5167.h"
#include "crypto_decode_857xint16.h"
#include "crypto_decode_857xint32.h"
#include "crypto_encode_857x1723.h"
#include "crypto_encode_857x1723round.h"
#include "crypto_encode_857x3.h"
#include "crypto_encode_857x5167.h"
#include "crypto_encode_857xfreeze3.h"
#include "crypto_encode_857xint16.h"
#include "crypto_encode_int16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1184.h"


#define qinv (-19761) /* reciprocal of q mod 2^16 */
#define q31 415615 /* floor(2^31/q) */
#define q27 25976 /* closest integer to 2^27/q */
#define q18 51 /* closest integer to 2^18/q */
#define q14 3 /* closest integer to 2^14/q */
#define ppad 865
#define endingmask _mm256_set_epi8(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0)
#define crypto_core_weight PQCLEAN_SNTRUP857_AVX2_crypto_core_weightsntrup857
#define p 857
#define q 5167
#define w 322

#define ppadsort 857

#define crypto_verify_clen PQCLEAN_SNTRUP857_AVX2_crypto_verify_1184

#define Rq_bytes PQCLEAN_SNTRUP857_AVX2_crypto_encode_857x5167_STRBYTES
#define Rq_encode PQCLEAN_SNTRUP857_AVX2_crypto_encode_857x5167
#define Rq_decode PQCLEAN_SNTRUP857_AVX2_crypto_decode_857x5167

#define Rounded_bytes PQCLEAN_SNTRUP857_AVX2_crypto_decode_857x1723_STRBYTES
#define Rounded_decode PQCLEAN_SNTRUP857_AVX2_crypto_decode_857x1723

#define Round_and_encode PQCLEAN_SNTRUP857_AVX2_crypto_encode_857x1723round

#define Small_bytes PQCLEAN_SNTRUP857_AVX2_crypto_encode_857x3_STRBYTES
#define Small_encode PQCLEAN_SNTRUP857_AVX2_crypto_encode_857x3
#define Small_decode PQCLEAN_SNTRUP857_AVX2_crypto_decode_857x3

#define crypto_encode_pxfreeze3 PQCLEAN_SNTRUP857_AVX2_crypto_encode_857xfreeze3

#define crypto_decode_pxint32 PQCLEAN_SNTRUP857_AVX2_crypto_decode_857xint32

#define crypto_decode_pxint16 PQCLEAN_SNTRUP857_AVX2_crypto_decode_857xint16

#define crypto_encode_pxint16 PQCLEAN_SNTRUP857_AVX2_crypto_encode_857xint16

#define crypto_core_wforce PQCLEAN_SNTRUP857_AVX2_crypto_core_wforcesntrup857

#define crypto_core_scale3 PQCLEAN_SNTRUP857_AVX2_crypto_core_scale3sntrup857

#define crypto_core_inv PQCLEAN_SNTRUP857_AVX2_crypto_core_invsntrup857

#define crypto_core_inv3 PQCLEAN_SNTRUP857_AVX2_crypto_core_inv3sntrup857

#define crypto_core_mult3 PQCLEAN_SNTRUP857_AVX2_crypto_core_mult3sntrup857

#define crypto_core_mult PQCLEAN_SNTRUP857_AVX2_crypto_core_multsntrup857

#endif
