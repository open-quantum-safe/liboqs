#ifndef ENCRYPT_H
#define ENCRYPT_H
/*
  This file is for Niederreiter encryption
*/

#include "namespace.h"

#define encrypt CRYPTO_NAMESPACE(encrypt)
#define encrypt_derand CRYPTO_NAMESPACE(encrypt_derand)

void encrypt(unsigned char *s, const unsigned char *pk, unsigned char *e);
void encrypt_derand(unsigned char *s, const unsigned char *pk, unsigned char *e, const unsigned char *input_seed);

#endif
