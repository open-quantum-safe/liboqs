#ifndef ENCRYPT_H
#define ENCRYPT_H
/*
  This file is for Niederreiter encryption
*/

#include "namespace.h"

#define encrypt CRYPTO_NAMESPACE(encrypt)

void encrypt(unsigned char *s, const unsigned char *pk, unsigned char *e);

#endif
