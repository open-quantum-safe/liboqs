#ifndef PACK_H
#define PACK_H

#include "poly.h"
#include <stdint.h>

static void hash_H(unsigned char *c_bin, poly v, const unsigned char *hm);
static void pack_sk(unsigned char *sk, poly s, poly_k e, unsigned char *seeds);
static void encode_pk(unsigned char *pk, const poly_k t, const unsigned char *seedA);
static void decode_pk(int32_t *pk, unsigned char *seedA, const unsigned char *pk_in);
static void encode_sig(unsigned char *sm, unsigned char *c, poly z);
static void decode_sig(unsigned char *c, poly z, const unsigned char *sm);

#endif
