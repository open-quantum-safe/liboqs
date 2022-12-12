#ifndef params_H
#define params_H
#include "crypto_core_multsntrup857.h"
#include "crypto_decode_256x16.h"
#include "crypto_decode_256x2.h"
#include "crypto_decode_857x1723.h"
#include "crypto_decode_857x3.h"
#include "crypto_decode_857xint16.h"
#include "crypto_decode_857xint32.h"
#include "crypto_encode_256x16.h"
#include "crypto_encode_256x2.h"
#include "crypto_encode_857x1723.h"
#include "crypto_encode_857x1723round.h"
#include "crypto_encode_857x3.h"
#include "crypto_encode_857xint16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1312.h"


#define p 857
#define q27 25976 /* closest integer to 2^27/q */
#define q18 51 /* closest integer to 2^18/q */
#define q 5167
#define w 281
#define tau0 2433
#define tau1 101
#define tau2 2265
#define tau3 324
#define I 256

#define ppadsort 857

#define q18 51 /* round(2^18/q) */
#define q27 25976 /* round(2^27/q) */
#define q31 415615 /* floor(2^31/q) */

#define crypto_verify_clen PQCLEAN_NTRULPR857_CLEAN_crypto_verify_1312

#define Rounded_bytes PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857x1723_STRBYTES
#define Rounded_decode PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857x1723

#define Round_and_encode PQCLEAN_NTRULPR857_CLEAN_crypto_encode_857x1723round

#define Small_bytes PQCLEAN_NTRULPR857_CLEAN_crypto_encode_857x3_STRBYTES
#define Small_encode PQCLEAN_NTRULPR857_CLEAN_crypto_encode_857x3
#define Small_decode PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857x3

#define Top_bytes PQCLEAN_NTRULPR857_CLEAN_crypto_encode_256x16_STRBYTES
#define Top_encode PQCLEAN_NTRULPR857_CLEAN_crypto_encode_256x16
#define Top_decode PQCLEAN_NTRULPR857_CLEAN_crypto_decode_256x16

#define Inputs_bytes PQCLEAN_NTRULPR857_CLEAN_crypto_encode_256x2_STRBYTES
#define Inputs_encode PQCLEAN_NTRULPR857_CLEAN_crypto_encode_256x2
#define Inputs_decode PQCLEAN_NTRULPR857_CLEAN_crypto_decode_256x2

#define crypto_decode_pxint32 PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857xint32

#define crypto_decode_pxint16 PQCLEAN_NTRULPR857_CLEAN_crypto_decode_857xint16

#define crypto_encode_pxint16 PQCLEAN_NTRULPR857_CLEAN_crypto_encode_857xint16

#define crypto_core_mult PQCLEAN_NTRULPR857_CLEAN_crypto_core_multsntrup857

#endif
