/* ****************************** *
 * Titanium_CCA_super             *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * CCA KEM                        *
 * ****************************** */
 
#ifndef KEM_H
#define KEM_H

int crypto_kem_keypair(
unsigned char *pk,
unsigned char *sk
);

int crypto_kem_enc(
unsigned char *ct,
unsigned char *ss,
const unsigned char *pk
);

int crypto_kem_dec(
unsigned char *ss,
const unsigned char *ct,
const unsigned char *sk
);

#endif
