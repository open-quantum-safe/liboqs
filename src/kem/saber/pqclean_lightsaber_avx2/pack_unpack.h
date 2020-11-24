#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H
#include "SABER_params.h"
#include "poly.h"
#include <stdint.h>
#include <stdio.h>

void PQCLEAN_LIGHTSABER_AVX2_POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const poly *data);

void PQCLEAN_LIGHTSABER_AVX2_BS2POLT(poly *data, const uint8_t bytes[SABER_SCALEBYTES_KEM]);


void PQCLEAN_LIGHTSABER_AVX2_POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], const poly data[SABER_L]);

void PQCLEAN_LIGHTSABER_AVX2_POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], const poly data[SABER_L]);


void PQCLEAN_LIGHTSABER_AVX2_BS2POLVECq(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECBYTES]);

void PQCLEAN_LIGHTSABER_AVX2_BS2POLVECp(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES]);


void PQCLEAN_LIGHTSABER_AVX2_BS2POLmsg(poly *data, const uint8_t bytes[SABER_KEYBYTES]);

void PQCLEAN_LIGHTSABER_AVX2_POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const poly *data);


#endif
