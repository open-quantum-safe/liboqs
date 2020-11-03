#include "crypto_core_scale3sntrup761.h"
#include "crypto_decode_761xint16.h"
#include "crypto_encode_761xint16.h"


#define p 761
#define q 4591

#define crypto_decode_pxint16 PQCLEAN_SNTRUP761_CLEAN_crypto_decode_761xint16
#define crypto_encode_pxint16 PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761xint16

typedef int16_t Fq;

/* out = 3*in in Rq */
int PQCLEAN_SNTRUP761_CLEAN_crypto_core_scale3sntrup761(unsigned char *outbytes, const unsigned char *inbytes) {
    Fq f[p];
    int i;

    crypto_decode_pxint16(f, inbytes);
    for (i = 0; i < p; ++i) {
        Fq x = f[i];
        x *= 3; /* (-3q+3)/2 ... (3q-3)/2 */
        x -= (q + 1) / 2; /* -2q+1 ... q-2 */
        x += q & (x >> 15); /* -q+1 ... q-1 */
        x += q & (x >> 15); /* 0 ... q-1 */
        x -= (q - 1) / 2; /* -(q-1)/2 ... (q-1)/2 */
        f[i] = x;
    }
    crypto_encode_pxint16(outbytes, f);

    return 0;
}
