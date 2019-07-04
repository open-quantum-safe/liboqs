#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H

#include "SABER_params.h"
#include <stdint.h>
#include <stdio.h>


void PQCLEAN_SABER_CLEAN_pack_3bit(uint8_t *bytes, const uint16_t *data);

void PQCLEAN_SABER_CLEAN_un_pack3bit(const uint8_t *bytes, uint16_t *data);

void PQCLEAN_SABER_CLEAN_pack_4bit(uint8_t *bytes, const uint16_t *data);

void PQCLEAN_SABER_CLEAN_un_pack4bit(const unsigned char *bytes, uint16_t *ar);

void PQCLEAN_SABER_CLEAN_pack_6bit(uint8_t *bytes, const uint16_t *data);

void PQCLEAN_SABER_CLEAN_un_pack6bit(const unsigned char *bytes, uint16_t *data);


void PQCLEAN_SABER_CLEAN_BS2POL(const unsigned char *bytes, uint16_t data[SABER_N]);

void PQCLEAN_SABER_CLEAN_POLVEC2BS(uint8_t *bytes, uint16_t data[SABER_K][SABER_N], uint16_t modulus);

void PQCLEAN_SABER_CLEAN_BS2POLVEC(const unsigned char *bytes, uint16_t data[SABER_K][SABER_N], uint16_t modulus);

#endif
