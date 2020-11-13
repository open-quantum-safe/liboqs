#include "SABER_params.h"
#include "pack_unpack.h"
#include "poly.h"
#include <string.h>

void PQCLEAN_SABER_CLEAN_POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const poly *data) {
    size_t j;
    const uint16_t *in = data->coeffs;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 2; j++) {
        out[0] = (uint8_t) ((in[0] & 0x0f) | (in[1] << 4));
        in += 2;
        out += 1;
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLT(poly *data, const uint8_t bytes[SABER_SCALEBYTES_KEM]) {
    /* This function does not reduce its output mod T */
    size_t j;
    const uint8_t *in = bytes;
    uint16_t *out = data->coeffs;
    for (j = 0; j < SABER_N / 2; j++) {
        out[0] = in[0];
        out[1] = in[0] >> 4;
        in += 1;
        out += 2;
    }
}

static void POLq2BS(uint8_t bytes[SABER_POLYBYTES], const poly *data) {
    size_t j;
    const uint16_t *in = data->coeffs;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 8; j++) {
        out[0] = (uint8_t) (in[0]);
        out[1] = (uint8_t) (((in[0] >> 8) & 0x1f) | (in[1] << 5));
        out[2] = (uint8_t) (in[1] >> 3);
        out[3] = (uint8_t) (((in[1] >> 11) & 0x03) | (in[2] << 2));
        out[4] = (uint8_t) (((in[2] >> 6) & 0x7f) | (in[3] << 7));
        out[5] = (uint8_t) (in[3] >> 1);
        out[6] = (uint8_t) (((in[3] >> 9) & 0x0f) | (in[4] << 4));
        out[7] = (uint8_t) (in[4] >> 4);
        out[8] = (uint8_t) (((in[4] >> 12) & 0x01) | (in[5] << 1));
        out[9] = (uint8_t) (((in[5] >> 7) & 0x3f) | (in[6] << 6));
        out[10] = (uint8_t) (in[6] >> 2);
        out[11] = (uint8_t) (((in[6] >> 10) & 0x07) | (in[7] << 3));
        out[12] = (uint8_t) (in[7] >> 5);
        in += 8;
        out += 13;
    }
}

static void BS2POLq(poly *data, const uint8_t bytes[SABER_POLYBYTES]) {
    /* This function does not reduce its output mod Q */
    size_t j;
    const uint8_t *in = bytes;
    uint16_t *out = data->coeffs;
    for (j = 0; j < SABER_N / 8; j++) {
        out[0] = (in[0]) | (in[1] << 8);
        out[1] = (in[1] >> 5) | (in[2] << 3) | (in[3] << 11);
        out[2] = (in[3] >> 2) | (in[4] << 6);
        out[3] = (in[4] >> 7) | (in[5] << 1) | (in[6] << 9);
        out[4] = (in[6] >> 4) | (in[7] << 4) | (in[8] << 12);
        out[5] = (in[8] >> 1) | (in[9] << 7);
        out[6] = (in[9] >> 6) | (in[10] << 2) | (in[11] << 10);
        out[7] = (in[11] >> 3) | (in[12] << 5);
        in += 13;
        out += 8;
    }
}

static void POLp2BS(uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], const poly *data) {
    size_t j;
    const uint16_t *in = data->coeffs;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 4; j++) {
        out[0] = (uint8_t) (in[0]);
        out[1] = (uint8_t) (((in[0] >> 8) & 0x03) | (in[1] << 2));
        out[2] = (uint8_t) (((in[1] >> 6) & 0x0f) | (in[2] << 4));
        out[3] = (uint8_t) (((in[2] >> 4) & 0x3f) | (in[3] << 6));
        out[4] = (uint8_t) (in[3] >> 2);
        in += 4;
        out += 5;
    }
}

static void BS2POLp(poly *data, const uint8_t bytes[SABER_POLYCOMPRESSEDBYTES]) {
    size_t j;
    const uint8_t *in = bytes;
    uint16_t *out = data->coeffs;
    for (j = 0; j < SABER_N / 4; j++) {
        out[0] = in[0] | (in[1] << 8);
        out[1] = (in[1] >> 2) | (in[2] << 6);
        out[2] = (in[2] >> 4) | (in[3] << 4);
        out[3] = (in[3] >> 6) | (in[4] << 2);
        in += 5;
        out += 4;
    }
}

void PQCLEAN_SABER_CLEAN_POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], const poly data[SABER_L]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLq2BS(bytes + i * SABER_POLYBYTES, &data[i]);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLVECq(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECBYTES]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLq(&data[i], bytes + i * SABER_POLYBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], const poly data[SABER_L]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLp2BS(bytes + i * SABER_POLYCOMPRESSEDBYTES, &data[i]);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLVECp(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLp(&data[i], bytes + i * SABER_POLYCOMPRESSEDBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLmsg(poly *data, const uint8_t bytes[SABER_KEYBYTES]) {
    size_t i, j;
    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            data->coeffs[j * 8 + i] = ((bytes[j] >> i) & 0x01);
        }
    }
}

void PQCLEAN_SABER_CLEAN_POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const poly *data) {
    size_t i, j;
    memset(bytes, 0, SABER_KEYBYTES);

    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            bytes[j] = bytes[j] | ((data->coeffs[j * 8 + i] & 0x01) << i);
        }
    }
}
