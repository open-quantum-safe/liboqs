/*=============================================================================
This file has been adapted from the implementation
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER)
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso.
=============================================================================*/


#include "api.h"
#include "pack_unpack.h"
#include <string.h>

/* This function reduces its input mod T */
void POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const uint16_t data[SABER_N]) {
    size_t j;
    const uint16_t *in = data;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 2; j++) {
        out[0] = (uint8_t) ((in[0] & 0x0f) | (in[1] << 4));
        in += 2;
        out += 1;
    }
}

/* This function does NOT reduce its output mod T */
void BS2POLT(const uint8_t bytes[SABER_SCALEBYTES_KEM], uint16_t data[SABER_N]) {
    PQCLEAN_SABER_AARCH64_asm_4_to_16(&(data[0]), &(bytes[0]));
}

/* This function reduces its input mod q */
void POLq2BS(uint8_t bytes[SABER_POLYBYTES], const uint16_t data[SABER_N]) {
    size_t i;
    const uint16_t *in = data;
    uint8_t *out = bytes;
    for (i = 0; i < SABER_N / 8; i++) {
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

/* This function sign-extends its output from q-bit to 16-bit.
This is needed by 16-bit NTTs */
void BS2POLq(const uint8_t bytes[SABER_POLYBYTES], uint16_t data[SABER_N]) {
    size_t i;
    const uint8_t *in = bytes;
    int16_t *out = (int16_t *)data;

    struct int13_t { // bitfield struct to sign-extend q-bit to 16-bit.
signed int bits:
        SABER_EQ;
    } q0, q1, q2, q3, q4, q5, q6, q7;

    for (i = 0; i < SABER_N / 8; i++) {
        q0.bits = (in[0]) | (in[1] << 8);
        q1.bits = (in[1] >> 5) | (in[2] << 3) | (in[3] << 11);
        q2.bits = (in[3] >> 2) | (in[4] << 6);
        q3.bits = (in[4] >> 7) | (in[5] << 1) | (in[6] << 9);
        q4.bits = (in[6] >> 4) | (in[7] << 4) | (in[8] << 12);
        q5.bits = (in[8] >> 1) | (in[9] << 7);
        q6.bits = (in[9] >> 6) | (in[10] << 2) | (in[11] << 10);
        q7.bits = (in[11] >> 3) | (in[12] << 5);
        out[0] = (int16_t)q0.bits;
        out[1] = (int16_t)q1.bits;
        out[2] = (int16_t)q2.bits;
        out[3] = (int16_t)q3.bits;
        out[4] = (int16_t)q4.bits;
        out[5] = (int16_t)q5.bits;
        out[6] = (int16_t)q6.bits;
        out[7] = (int16_t)q7.bits;
        in += 13;
        out += 8;
    }
}

/* This function reduces its input mod p */
void POLp2BS(uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], const uint16_t data[SABER_N]) {
    size_t i;
    const uint16_t *in = data;
    uint8_t *out = bytes;
    for (i = 0; i < SABER_N / 4; i++) {
        out[0] = (uint8_t) (in[0]);
        out[1] = (uint8_t) (((in[0] >> 8) & 0x03) | (in[1] << 2));
        out[2] = (uint8_t) (((in[1] >> 6) & 0x0f) | (in[2] << 4));
        out[3] = (uint8_t) (((in[2] >> 4) & 0x3f) | (in[3] << 6));
        out[4] = (uint8_t) (in[3] >> 2);
        in += 4;
        out += 5;
    }
}

/* This function sign-extends its output from p-bit to 16-bit.
This is needed by the NTT */
void BS2POLp(const uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], uint16_t data[SABER_N]) {
    size_t j;
    const uint8_t *in = bytes;
    int16_t *out = (int16_t *)data;

    struct int10_t { // bitfield struct to sign-extend p-bit to 16-bit.
signed int bits:
        SABER_EP;
    } p0, p1, p2, p3;

    for (j = 0; j < SABER_N / 4; j++) {
        p0.bits = (in[0]) | (in[1] << 8);
        p1.bits = (in[1] >> 2) | (in[2] << 6);
        p2.bits = (in[2] >> 4) | (in[3] << 4);
        p3.bits = (in[3] >> 6) | (in[4] << 2);
        out[0] = (int16_t)p0.bits;
        out[1] = (int16_t)p1.bits;
        out[2] = (int16_t)p2.bits;
        out[3] = (int16_t)p3.bits;
        in += 5;
        out += 4;
    }
}

void POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], uint16_t data[SABER_L][SABER_N]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLq2BS(bytes + i * SABER_POLYBYTES, data[i]);
    }
}

void BS2POLVECq(const uint8_t bytes[SABER_POLYVECBYTES], uint16_t data[SABER_L][SABER_N]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLq(bytes + i * SABER_POLYBYTES, data[i]);
    }
}

void POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], uint16_t data[SABER_L][SABER_N]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLp2BS(bytes + i * (SABER_EP * SABER_N / 8), data[i]);
    }
}

void BS2POLVECp(const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], uint16_t data[SABER_L][SABER_N]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLp(bytes + i * (SABER_EP * SABER_N / 8), data[i]);
    }
}

void BS2POLmsg(const uint8_t bytes[SABER_KEYBYTES], uint16_t data[SABER_N]) {
    PQCLEAN_SABER_AARCH64_asm_1_to_16(&(data[0]), &(bytes[0]));
}

/* This function reduces its input mod 2 */
void POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const uint16_t data[SABER_N]) {
    size_t i, j;
    uint8_t byte;
    for (j = 0; j < SABER_KEYBYTES; j++) {
        byte = 0;
        for (i = 0; i < 8; i++) {
            byte |= ((data[j * 8 + i] & 0x01) << i);
        }
        bytes[j] = byte;
    }
}





