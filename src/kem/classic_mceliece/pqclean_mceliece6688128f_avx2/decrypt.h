#ifndef DECRYPT_H
#define DECRYPT_H
/*
  This file is for Niederreiter decryption
*/

#include "namespace.h"

#define decrypt CRYPTO_NAMESPACE(decrypt)

int decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *s);

#endif
