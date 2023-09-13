#ifndef DECRYPT_H
#define DECRYPT_H
/*
  This file is for Nieddereiter decryption
*/

#include "namespace.h"

#define decrypt CRYPTO_NAMESPACE(decrypt)

int decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *c);

#endif
