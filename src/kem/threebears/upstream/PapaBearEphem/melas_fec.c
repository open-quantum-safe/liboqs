/** Melas BCH forward error correction */
#include "melas_fec.h"
typedef uint32_t fec_gf_t;
static const fec_gf_t F = 0x211;

/* Compute syndrome(data), where data has length len */
#define syndrome18(data,len) s18update(0,data,len)
static fec_gf_t s18update(fec_gf_t r, const uint8_t *data, unsigned len) {
    for (unsigned i=0; i<len; i++) {
        r ^= data[i];
        fec_gf_t tmp = (r ^ r<<4 ^ r<<5) & 0xFF;
        tmp ^= tmp<<9;
        r = r>>8 ^ tmp>>4 ^ tmp>>3 ^ tmp<<1;
    }
    return r;
}

/* Append 3 bytes of FEC(data) to data, so that the FEC becomes 0 */
void melas_fec_set (
    uint8_t out[MELAS_FEC_BYTES],
    const uint8_t *data,
    unsigned len
) {
    fec_gf_t fec = syndrome18(data,len);
    for (unsigned i=0; i<3; i++, fec>>=8) {
        out[i] = fec;
    }
}

/* Return a*b^2 mod F */
static fec_gf_t mul2(fec_gf_t a, fec_gf_t b) {
    fec_gf_t r = 0;
    for (unsigned i=0; i<9; i++) {
        r ^= ((b>>8)&1) * a;
        b <<= 1;
        a = a>>2 ^ (a&3)*(F>>2);
    }
    return r;
}

/* Correct data to have the given FEC */
void melas_fec_correct (
    uint8_t *data,
    unsigned len,
    const uint8_t fec[MELAS_FEC_BYTES]
) {
    const uint8_t table[9] = {98,132,77,250,146,45,182,39,157};
    fec_gf_t a=s18update(syndrome18(data,len),fec,MELAS_FEC_BYTES);
    fec_gf_t b=a<<9,c;
    unsigned i,j;
    
    /* Multiply syndrome by reverse(syndrome) */
    for (i=c=0; i<27; i++) {
        c ^= (b&1) * a;
        b >>= 1;
        a = a>>1 ^ (a&1)*(F>>1);
    }
    
    /* b = sqrt(half-trace(1/b)) */
    for (i=0,b=c; i<7; i++) c = mul2(b,c);
    for (i=0,b=0; i<9; i++) b ^= ((c>>i)&1)*table[i];

    /* Circulate */
    for (i=0; i<452-len*8; i+=2) {
        a = a>>2 ^ (a&3)*(F>>2);
    }

    /* Correct the errors */
    b = mul2(a,b);
    for (j=0; j<2; j++, b^=a) {
        for (i=0, c=b; i<len; i++) {
            fec_gf_t tmp = (c^c<<4)&0xFF;
            c = c>>8 ^ tmp>>4 ^ tmp<<1;
            data[i] ^= c & (((c & (c-1))-1)>>9);
        }
    }
}
