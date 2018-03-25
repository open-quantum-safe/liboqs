#ifndef PARAMS_H
#define PARAMS_H

#ifndef KYBER_K
#define KYBER_K 4 /* Change this for different security strengths */
#endif

#define KYBER_N 256
#define KYBER_Q 7681


#if   (KYBER_K == 2) /* Kyber512 */
#define KYBER_ETA 5
#elif (KYBER_K == 3) /* Kyber768 */
#define KYBER_ETA 4
#elif (KYBER_K == 4) /*KYBER1024 */
#define KYBER_ETA 3
#else
#error "KYBER_K must be in {2,3,4}"
#endif

#define KYBER_SYMBYTES 32   /* size in bytes of shared key, hashes, and seeds */

#define KYBER_POLYBYTES              416 
#define KYBER_POLYCOMPRESSEDBYTES    96 
#define KYBER_POLYVECBYTES           (KYBER_K * KYBER_POLYBYTES)   
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 352) 

#define KYBER_INDCPA_MSGBYTES       KYBER_SYMBYTES
#define KYBER_INDCPA_PUBLICKEYBYTES (KYBER_POLYVECCOMPRESSEDBYTES + KYBER_SYMBYTES)
#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_POLYVECBYTES)
#define KYBER_INDCPA_BYTES          (KYBER_POLYVECCOMPRESSEDBYTES + KYBER_POLYCOMPRESSEDBYTES)

#define KYBER_PUBLICKEYBYTES  (KYBER_INDCPA_PUBLICKEYBYTES)
#define KYBER_SECRETKEYBYTES  (KYBER_INDCPA_SECRETKEYBYTES +  KYBER_INDCPA_PUBLICKEYBYTES + 2*KYBER_SYMBYTES) /* 32 bytes of additional space to save H(pk) */
#define KYBER_CIPHERTEXTBYTES  KYBER_INDCPA_BYTES

#endif
