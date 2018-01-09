/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: setting parameters to test FrodoKEM-976
*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ds_benchmark.h"
#include "../src/api_frodo976.h"


#define SYSTEM_NAME    "FrodoKEM-976"

#define crypto_kem_keypair            crypto_kem_keypair_Frodo976
#define crypto_kem_enc                crypto_kem_enc_Frodo976
#define crypto_kem_dec                crypto_kem_dec_Frodo976

#include "test_kem.c"
