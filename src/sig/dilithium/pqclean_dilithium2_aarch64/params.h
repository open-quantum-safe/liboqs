
/*
 * This file is dual licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * or public domain at https://github.com/pq-crystals/dilithium
 */

#ifndef PARAMS_H
#define PARAMS_H

#define DILITHIUM_MODE 2
//#define DILITHIUM_MODE 3
//#define DILITHIUM_MODE 5

#define CRYPTO_NAMESPACETOP PQCLEAN_DILITHIUM2_AARCH64_crypto_sign
#define CRYPTO_NAMESPACE(s) PQCLEAN_DILITHIUM2_AARCH64_##s
#define DILITHIUM_NAMESPACETOP CRYPTO_NAMESPACETOP
#define DILITHIUM_NAMESPACE(s) CRYPTO_NAMESPACE(s)


#define SEEDBYTES 32
#define CRHBYTES 64
#define N 256
#define DILITHIUM_Q 8380417
#define D 13
#define ROOT_OF_UNITY 1753


#define K 4
#define L 4
#define ETA 2
#define TAU 39
#define BETA 78
#define GAMMA1 (1 << 17)
#define GAMMA2 ((DILITHIUM_Q-1)/88)
#define OMEGA 80
#define CRYPTO_ALGNAME "Dilithium2"


#define POLYT1_PACKEDBYTES  320
#define POLYT0_PACKEDBYTES  416
#define POLYVECH_PACKEDBYTES (OMEGA + K)


#if GAMMA1 == (1 << 17)
#define POLYZ_PACKEDBYTES   576
#elif GAMMA1 == (1 << 19)
#define POLYZ_PACKEDBYTES   640
#endif

#if GAMMA2 == (DILITHIUM_Q-1)/88
#define POLYW1_PACKEDBYTES  192
#elif GAMMA2 == (DILITHIUM_Q-1)/32
#define POLYW1_PACKEDBYTES  128
#endif

#define POLYETA_PACKEDBYTES  96

#define CRYPTO_PUBLICKEYBYTES (SEEDBYTES + K*POLYT1_PACKEDBYTES)
#define CRYPTO_SECRETKEYBYTES (3*SEEDBYTES \
                               + L*POLYETA_PACKEDBYTES \
                               + K*POLYETA_PACKEDBYTES \
                               + K*POLYT0_PACKEDBYTES)
#define CRYPTO_BYTES (SEEDBYTES + L*POLYZ_PACKEDBYTES + POLYVECH_PACKEDBYTES)

#endif
