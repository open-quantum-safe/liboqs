#ifndef INDCPA_H
#define INDCPA_H

void PQCLEAN_FIRESABER_CLEAN_indcpa_kem_keypair(unsigned char *pk, unsigned char *sk);
void PQCLEAN_FIRESABER_CLEAN_indcpa_kem_enc(const unsigned char *message, unsigned char *noiseseed, const unsigned char *pk, unsigned char *ciphertext);
void PQCLEAN_FIRESABER_CLEAN_indcpa_kem_dec(const unsigned char *sk, const unsigned char *ciphertext, unsigned char *message_dec);

#endif

