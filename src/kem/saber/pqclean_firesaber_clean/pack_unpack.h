#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H
#include "SABER_params.h"
#include "poly.h"
#include <stdint.h>
#include <stdio.h>

void PQCLEAN_FIRESABER_CLEAN_POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const poly *data);

void PQCLEAN_FIRESABER_CLEAN_BS2POLT(poly *data, const uint8_t bytes[SABER_SCALEBYTES_KEM]);


void PQCLEAN_FIRESABER_CLEAN_POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], const poly data[SABER_L]);

void PQCLEAN_FIRESABER_CLEAN_POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], const poly data[SABER_L]);


void PQCLEAN_FIRESABER_CLEAN_BS2POLVECq(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECBYTES]);

void PQCLEAN_FIRESABER_CLEAN_BS2POLVECp(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES]);


void PQCLEAN_FIRESABER_CLEAN_BS2POLmsg(poly *data, const uint8_t bytes[SABER_KEYBYTES]);

void PQCLEAN_FIRESABER_CLEAN_POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const poly *data);


#endif
