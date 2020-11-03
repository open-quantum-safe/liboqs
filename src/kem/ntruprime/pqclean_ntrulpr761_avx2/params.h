#ifndef params_H
#define params_H
#include "crypto_core_multsntrup761.h"
#include "crypto_decode_256x16.h"
#include "crypto_decode_256x2.h"
#include "crypto_decode_761x1531.h"
#include "crypto_decode_761x3.h"
#include "crypto_decode_761xint16.h"
#include "crypto_decode_761xint32.h"
#include "crypto_encode_256x16.h"
#include "crypto_encode_256x2.h"
#include "crypto_encode_761x1531.h"
#include "crypto_encode_761x1531round.h"
#include "crypto_encode_761x3.h"
#include "crypto_encode_761xint16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1167.h"


#define p 761
#define q 4591
#define w 250
#define tau0 2156
#define tau1 114
#define tau2 2007
#define tau3 287
#define I 256

#define ppadsort 768

#define q18 57 /* round(2^18/q) */
#define q27 29235 /* round(2^27/q) */
#define q31 467759 /* floor(2^31/q) */

#define crypto_verify_clen PQCLEAN_NTRULPR761_AVX2_crypto_verify_1167

#define Rounded_bytes PQCLEAN_NTRULPR761_AVX2_crypto_decode_761x1531_STRBYTES
#define Rounded_decode PQCLEAN_NTRULPR761_AVX2_crypto_decode_761x1531

#define Round_and_encode PQCLEAN_NTRULPR761_AVX2_crypto_encode_761x1531round

#define Small_bytes PQCLEAN_NTRULPR761_AVX2_crypto_encode_761x3_STRBYTES
#define Small_encode PQCLEAN_NTRULPR761_AVX2_crypto_encode_761x3
#define Small_decode PQCLEAN_NTRULPR761_AVX2_crypto_decode_761x3

#define Top_bytes PQCLEAN_NTRULPR761_AVX2_crypto_encode_256x16_STRBYTES
#define Top_encode PQCLEAN_NTRULPR761_AVX2_crypto_encode_256x16
#define Top_decode PQCLEAN_NTRULPR761_AVX2_crypto_decode_256x16

#define Inputs_bytes PQCLEAN_NTRULPR761_AVX2_crypto_encode_256x2_STRBYTES
#define Inputs_encode PQCLEAN_NTRULPR761_AVX2_crypto_encode_256x2
#define Inputs_decode PQCLEAN_NTRULPR761_AVX2_crypto_decode_256x2

#define crypto_decode_pxint32 PQCLEAN_NTRULPR761_AVX2_crypto_decode_761xint32

#define crypto_decode_pxint16 PQCLEAN_NTRULPR761_AVX2_crypto_decode_761xint16

#define crypto_encode_pxint16 PQCLEAN_NTRULPR761_AVX2_crypto_encode_761xint16

#define crypto_core_mult PQCLEAN_NTRULPR761_AVX2_crypto_core_multsntrup761

#endif
