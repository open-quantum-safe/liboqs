#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "SABER_params.h"
#include "pack_unpack.h"

void recon_gen(uint16_t b_avx, uint16_t *K1, uint16_t *recon_data);

void recon(uint16_t a, uint16_t recon_data, uint16_t *K2);

void ciphertext_gen(uint16_t b_avx, uint16_t *ciphertext);

void ReconDataGen(uint16_t *vprime, unsigned char *rec_c);

void Recon(uint16_t *recon_data,unsigned char *recon_ar,uint16_t *message_dec_unpacked);

