/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: functions for FrodoKEM-976
*           Instantiates "frodo_macrify.c" with the necessary matrix arithmetic functions
*********************************************************************************************/

#include "frodo976_params.h"
#define _AES128_FOR_A_

// Selecting cSHAKE XOF function for the KEM and noise sampling
#define cshake OQS_SHA3_cshake256_simple

#define crypto_kem_keypair OQS_KEM_frodokem_976_aes_keypair
#define crypto_kem_enc OQS_KEM_frodokem_976_aes_encaps
#define crypto_kem_dec OQS_KEM_frodokem_976_aes_decaps

#include "frodo_macrify.h"
#include "kem.c"
#include "frodo_macrify.c"
