#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>

//#define KYBER_90S /* Uncomment this if you want the 90S variant */
/* Don't change parameters below this line */

#define KYBER_NAMESPACE_512_90s(s) pqcrystals_kyber512_90s_ref_##s
#define KYBER_NAMESPACE_512(s) pqcrystals_kyber512_ref_##s
#define KYBER_NAMESPACE_768_90s(s) pqcrystals_kyber768_90s_ref_##s
#define KYBER_NAMESPACE_768(s) pqcrystals_kyber768_ref_##s
#define KYBER_NAMESPACE_1024_90s(s) pqcrystals_kyber1024_90s_ref_##s
#define KYBER_NAMESPACE_1024(s) pqcrystals_kyber1024_ref_##s


typedef const struct _param
{
    int16_t KYBER_K;

    int16_t KYBER_N;
    int16_t KYBER_Q;

    int16_t KYBER_SYMBYTES;
    int16_t KYBER_SSBYTES;

    int16_t KYBER_POLYBYTES;
    int16_t KYBER_POLYVECBYTES;

    int16_t KYBER_ETA1;
    int16_t KYBER_POLYCOMPRESSEDBYTES;
    int16_t KYBER_POLYVECCOMPRESSEDBYTES;

    int16_t KYBER_ETA2;
    int16_t KYBER_INDCPA_MSGBYTES;
    int16_t KYBER_INDCPA_PUBLICKEYBYTES;
    int16_t KYBER_INDCPA_SECRETKEYBYTES;
    int16_t KYBER_INDCPA_BYTES;

    int16_t KYBER_PUBLICKEYBYTES;
    
    int16_t KYBER_SECRETKEYBYTES;
    int16_t KYBER_CIPHERTEXTBYTES;

} param;


#endif
