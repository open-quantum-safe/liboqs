#include "crypto_core_weightsntrup761.h"
#include "crypto_encode_int16.h"
#include "params.h"


#define int8 int8_t
#define int16 int16_t


/* out = little-endian weight of bottom bits of in */
int PQCLEAN_SNTRUP761_CLEAN_crypto_core_weightsntrup761(unsigned char *outbytes, const unsigned char *inbytes) {
    int8 *in = (void *) inbytes;
    int16 weight = 0;
    int i;

    for (i = 0; i < p; ++i) {
        weight += in[i] & 1;
    }
    PQCLEAN_SNTRUP761_CLEAN_crypto_encode_int16(outbytes, &weight);
    return 0;
}
