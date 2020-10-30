#ifndef params_H
#define params_H
#include "crypto_core_multsntrup653.h"
#include "crypto_decode_256x16.h"
#include "crypto_decode_256x2.h"
#include "crypto_decode_653x1541.h"
#include "crypto_decode_653x3.h"
#include "crypto_decode_653xint16.h"
#include "crypto_decode_653xint32.h"
#include "crypto_encode_256x16.h"
#include "crypto_encode_256x2.h"
#include "crypto_encode_653x1541.h"
#include "crypto_encode_653x1541round.h"
#include "crypto_encode_653x3.h"
#include "crypto_encode_653xint16.h"
#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include "crypto_verify_1025.h"


#define p 653
#define q 4621
#define w 252
#define tau0 2175
#define tau1 113
#define tau2 2031
#define tau3 290
#define I 256

#define ppadsort 653

#define q18 57 /* round(2^18/q) */
#define q27 29045 /* round(2^27/q) */
#define q31 464722 /* floor(2^31/q) */

#define crypto_verify_clen PQCLEAN_NTRULPR653_AVX2_crypto_verify_1025

#define Rounded_bytes PQCLEAN_NTRULPR653_AVX2_crypto_decode_653x1541_STRBYTES
#define Rounded_decode PQCLEAN_NTRULPR653_AVX2_crypto_decode_653x1541

#define Round_and_encode PQCLEAN_NTRULPR653_AVX2_crypto_encode_653x1541round

#define Small_bytes PQCLEAN_NTRULPR653_AVX2_crypto_encode_653x3_STRBYTES
#define Small_encode PQCLEAN_NTRULPR653_AVX2_crypto_encode_653x3
#define Small_decode PQCLEAN_NTRULPR653_AVX2_crypto_decode_653x3

#define Top_bytes PQCLEAN_NTRULPR653_AVX2_crypto_encode_256x16_STRBYTES
#define Top_encode PQCLEAN_NTRULPR653_AVX2_crypto_encode_256x16
#define Top_decode PQCLEAN_NTRULPR653_AVX2_crypto_decode_256x16

#define Inputs_bytes PQCLEAN_NTRULPR653_AVX2_crypto_encode_256x2_STRBYTES
#define Inputs_encode PQCLEAN_NTRULPR653_AVX2_crypto_encode_256x2
#define Inputs_decode PQCLEAN_NTRULPR653_AVX2_crypto_decode_256x2

#define crypto_decode_pxint32 PQCLEAN_NTRULPR653_AVX2_crypto_decode_653xint32

#define crypto_decode_pxint16 PQCLEAN_NTRULPR653_AVX2_crypto_decode_653xint16

#define crypto_encode_pxint16 PQCLEAN_NTRULPR653_AVX2_crypto_encode_653xint16

#define crypto_core_mult PQCLEAN_NTRULPR653_AVX2_crypto_core_multsntrup653

#endif
