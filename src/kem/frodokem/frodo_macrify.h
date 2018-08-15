/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: header for internal functions
*********************************************************************************************/

#ifndef _FRODOKEM_MACRIFY_H_
#define _FRODOKEM_MACRIFY_H_

#include <stddef.h>
#include <stdint.h>
#include "config.h"

void oqs_kem_frodokem_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb);
void oqs_kem_frodokem_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb);
static void frodo_sample_n(uint16_t *s, const size_t n);

static int frodo_mul_add_as_plus_e(uint16_t *b, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
static int frodo_mul_add_sa_plus_e(uint16_t *b, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
static void frodo_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e);
static void frodo_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s);

static void frodo_add(uint16_t *out, const uint16_t *a, const uint16_t *b);
static void frodo_sub(uint16_t *out, const uint16_t *a, const uint16_t *b);
static void frodo_key_encode(uint16_t *out, const uint16_t *in);
static void frodo_key_decode(uint16_t *out, const uint16_t *in);

#endif
