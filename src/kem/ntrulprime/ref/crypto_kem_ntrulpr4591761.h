#ifndef crypto_kem_ntrulpr4591761_H
#define crypto_kem_ntrulpr4591761_H

#define crypto_kem_ntrulpr4591761_ref_SECRETKEYBYTES 1238
#define crypto_kem_ntrulpr4591761_ref_PUBLICKEYBYTES 1047
#define crypto_kem_ntrulpr4591761_ref_CIPHERTEXTBYTES 1175
#define crypto_kem_ntrulpr4591761_ref_BYTES 32

#ifdef __cplusplus
extern "C" {
#endif
extern int crypto_kem_ntrulpr4591761_ref_keypair(unsigned char *, unsigned char *);
extern int crypto_kem_ntrulpr4591761_ref_enc(unsigned char *, unsigned char *, const unsigned char *);
extern int crypto_kem_ntrulpr4591761_ref_dec(unsigned char *, const unsigned char *, const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_kem_ntrulpr4591761_keypair crypto_kem_ntrulpr4591761_ref_keypair
#define crypto_kem_ntrulpr4591761_enc crypto_kem_ntrulpr4591761_ref_enc
#define crypto_kem_ntrulpr4591761_dec crypto_kem_ntrulpr4591761_ref_dec
#define crypto_kem_ntrulpr4591761_PUBLICKEYBYTES crypto_kem_ntrulpr4591761_ref_PUBLICKEYBYTES
#define crypto_kem_ntrulpr4591761_SECRETKEYBYTES crypto_kem_ntrulpr4591761_ref_SECRETKEYBYTES
#define crypto_kem_ntrulpr4591761_BYTES crypto_kem_ntrulpr4591761_ref_BYTES
#define crypto_kem_ntrulpr4591761_CIPHERTEXTBYTES crypto_kem_ntrulpr4591761_ref_CIPHERTEXTBYTES

#endif
