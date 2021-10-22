#ifndef params_H
#define params_H
#include "crypto_core_multsntrup1277.h"
#include "crypto_decode_1277x2627.h"
#include "crypto_decode_1277x3.h"
#include "crypto_decode_1277xint16.h"
#include "crypto_decode_1277xint32.h"
#include "crypto_decode_256x16.h"
#include "crypto_decode_256x2.h"
#include "crypto_encode_1277x2627.h"
#include "crypto_encode_1277x2627round.h"
#include "crypto_encode_1277x3.h"
#include "crypto_encode_1277xint16.h"
#include "crypto_encode_256x16.h"
#include "crypto_encode_256x2.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1975.h"


#define p 1277
#define q 7879
#define w 429
#define tau0 3724
#define tau1 66
#define tau2 3469
#define tau3 496
#define I 256

#define ppadsort 1280

#define q18 33 /* round(2^18/q) */
#define q27 17035 /* round(2^27/q) */
#define q31 272557 /* floor(2^31/q) */

#define crypto_verify_clen PQCLEAN_NTRULPR1277_AVX2_crypto_verify_1975

#define Rounded_bytes PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277x2627_STRBYTES
#define Rounded_decode PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277x2627

#define Round_and_encode PQCLEAN_NTRULPR1277_AVX2_crypto_encode_1277x2627round

#define Small_bytes PQCLEAN_NTRULPR1277_AVX2_crypto_encode_1277x3_STRBYTES
#define Small_encode PQCLEAN_NTRULPR1277_AVX2_crypto_encode_1277x3
#define Small_decode PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277x3

#define Top_bytes PQCLEAN_NTRULPR1277_AVX2_crypto_encode_256x16_STRBYTES
#define Top_encode PQCLEAN_NTRULPR1277_AVX2_crypto_encode_256x16
#define Top_decode PQCLEAN_NTRULPR1277_AVX2_crypto_decode_256x16

#define Inputs_bytes PQCLEAN_NTRULPR1277_AVX2_crypto_encode_256x2_STRBYTES
#define Inputs_encode PQCLEAN_NTRULPR1277_AVX2_crypto_encode_256x2
#define Inputs_decode PQCLEAN_NTRULPR1277_AVX2_crypto_decode_256x2

#define crypto_decode_pxint32 PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277xint32

#define crypto_decode_pxint16 PQCLEAN_NTRULPR1277_AVX2_crypto_decode_1277xint16

#define crypto_encode_pxint16 PQCLEAN_NTRULPR1277_AVX2_crypto_encode_1277xint16

#define crypto_core_mult PQCLEAN_NTRULPR1277_AVX2_crypto_core_multsntrup1277

#endif
