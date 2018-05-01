/********************************************************************************************
* Header defining the APIs for AES in ECB mode
*********************************************************************************************/

#ifndef __AES_H
#define __AES_H

#include <stdint.h>

void AES_ECB_encrypt(const uint8_t* input, const uint8_t* key, uint8_t* output);

#endif
