#include "crypto_core_wforcesntrup761.h"
#include "params.h"


#define int8 int8_t
#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

typedef int8 small;


/* return -1 if x!=0; else return 0 */
static int int16_nonzero_mask(int16 x) {
    uint16 u = (uint16) x; /* 0, else 1...65535 */
    uint32 v = u; /* 0, else 1...65535 */
    v = ~v + 1; /* 0, else 2^32-65535...2^32-1 */
    v >>= 31; /* 0, else 1 */
    return -(int) v; /* 0, else -1 */
}

/* 0 if Weightw_is(r), else -1 */
static int Weightw_mask(const small *r) {
    int weight = 0;
    int i;

    for (i = 0; i < p; ++i) {
        weight += r[i] & 1;
    }
    return int16_nonzero_mask((int16) (weight - w));
}

/* out = in if bottom bits of in have weight w */
/* otherwise out = (1,1,...,1,0,0,...,0) */
int PQCLEAN_SNTRUP761_CLEAN_crypto_core_wforcesntrup761(unsigned char *outbytes, const unsigned char *inbytes) {
    small *out = (void *) outbytes;
    const small *in = (const void *) inbytes;
    int i, mask;

    mask = Weightw_mask(in); /* 0 if weight w, else -1 */
    for (i = 0; i < w; ++i) {
        out[i] = (small) (((in[i] ^ 1) & ~mask) ^ 1);
    }
    for (i = w; i < p; ++i) {
        out[i] = (small) (in[i] & ~mask);
    }
    return 0;
}
