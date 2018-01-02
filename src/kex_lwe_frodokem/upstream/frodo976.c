/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: functions for FrodoKEM-976
*           Instantiates "frodo_macrify.c" with the necessary matrix arithmetic functions
*********************************************************************************************/

#include "api_frodo976.h"
#include "frodo_macrify.h"


// Parameters for "FrodoKEM-976"
#define PARAMS_N 976
#define PARAMS_NBAR 8
#define PARAMS_LOGQ 16
#define PARAMS_Q (1 << PARAMS_LOGQ)
#define PARAMS_EXTRACTED_BITS 3
#define PARAMS_STRIPE_STEP 8
#define PARAMS_PARALLEL 4
#define BYTES_SEED_A 16
#define BYTES_MU (PARAMS_EXTRACTED_BITS*PARAMS_NBAR*PARAMS_NBAR)/8

// Selecting cSHAKE XOF function for the KEM and noise sampling
#define cshake    cshake256_simple

// CDF table
uint16_t CDF_TABLE[11] = {5638, 15915, 23689, 28571, 31116, 32217, 32613, 32731, 32760, 32766, 32767};
uint16_t CDF_TABLE_LEN = 11;

#define crypto_kem_keypair            crypto_kem_keypair_Frodo976
#define crypto_kem_enc                crypto_kem_enc_Frodo976
#define crypto_kem_dec                crypto_kem_dec_Frodo976

#include "kem.c"
#include "noise.c"
#if defined(USE_REFERENCE)
#include "frodo_macrify_reference.c"
#else
#include "frodo_macrify.c"
#endif
