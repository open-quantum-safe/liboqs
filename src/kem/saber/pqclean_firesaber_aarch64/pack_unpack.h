#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H
/*=============================================================================
This file has been adapted from the implementation
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER)
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso.
=============================================================================*/

#include "SABER_params.h"
#include <stdint.h>
#include <stdio.h>

extern void PQCLEAN_FIRESABER_AARCH64_asm_1_to_16(void *, const void *);
extern void PQCLEAN_FIRESABER_AARCH64_asm_4_to_16(void *, const void *);

extern void PQCLEAN_FIRESABER_AARCH64_asm_10_to_32(void *, const void *);
extern void PQCLEAN_FIRESABER_AARCH64_asm_13_to_32(void *, const void *);
extern void PQCLEAN_FIRESABER_AARCH64_asm_16_to_32(void *, const void *);

#define POLT2BS SABER_NAMESPACE(POLT2BS)
void POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const uint16_t data[SABER_N]);
#define BS2POLT SABER_NAMESPACE(BS2POLT)
void BS2POLT(const uint8_t bytes[SABER_SCALEBYTES_KEM], uint16_t data[SABER_N]);

#define POLq2BS SABER_NAMESPACE(POLq2BS)
void POLq2BS(uint8_t bytes[SABER_POLYBYTES], const uint16_t data[SABER_N]);
#define POLp2BS SABER_NAMESPACE(POLp2BS)
void POLp2BS(uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], const uint16_t data[SABER_N]);

#define BS2POLq SABER_NAMESPACE(BS2POLq)
void BS2POLq(const uint8_t bytes[SABER_POLYBYTES], uint16_t data[SABER_N]);
#define BS2POLp SABER_NAMESPACE(BS2POLp)
void BS2POLp(const uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], uint16_t data[SABER_N]);

#define POLVECq2BS SABER_NAMESPACE(POLVECq2BS)
void POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], uint16_t data[SABER_L][SABER_N]);
#define POLVECp2BS SABER_NAMESPACE(POLVECp2BS)
void POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], uint16_t data[SABER_L][SABER_N]);

#define BS2POLVECq SABER_NAMESPACE(BS2POLVECq)
void BS2POLVECq(const uint8_t bytes[SABER_POLYVECBYTES], uint16_t data[SABER_L][SABER_N]);
#define BS2POLVECp SABER_NAMESPACE(BS2POLVECp)
void BS2POLVECp(const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], uint16_t data[SABER_L][SABER_N]);

#define BS2POLmsg SABER_NAMESPACE(BS2POLmsg)
void BS2POLmsg(const uint8_t bytes[SABER_KEYBYTES], uint16_t data[SABER_N]);
#define POLmsg2BS SABER_NAMESPACE(POLmsg2BS)
void POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const uint16_t data[SABER_N]);

#endif
