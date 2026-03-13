#ifndef SNTRUP761_H
#define SNTRUP761_H

#define crypto_kem_sntrup761_PUBLICKEYBYTES  1158
#define crypto_kem_sntrup761_SECRETKEYBYTES  1763
#define crypto_kem_sntrup761_CIPHERTEXTBYTES 1039
#define crypto_kem_sntrup761_BYTES           32

int OPENSSH_SNTRUP761_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int OPENSSH_SNTRUP761_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);
int OPENSSH_SNTRUP761_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);

#endif /* SNTRUP761_H */
