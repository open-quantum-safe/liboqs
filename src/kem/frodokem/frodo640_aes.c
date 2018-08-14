/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: functions for FrodoKEM-640
*           Instantiates "frodo_macrify.c" with the necessary matrix arithmetic functions
*********************************************************************************************/

#include "frodo640_params.h"
#define _AES128_FOR_A_

// Selecting cSHAKE XOF function for the KEM and noise sampling
#define cshake OQS_SHA3_cshake128_simple

#define crypto_kem_keypair OQS_KEM_frodokem_640_aes_keypair
#define crypto_kem_enc OQS_KEM_frodokem_640_aes_encaps
#define crypto_kem_dec OQS_KEM_frodokem_640_aes_decaps

#include "frodo_macrify.h"
#include "kem.c"
#include "frodo_macrify.c"
