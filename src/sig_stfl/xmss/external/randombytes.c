/*=============================================================================
 * Copyright (c) 2022 by SandboxAQ Inc
 * Author: Duc Tri Nguyen (ductri.nguyen@sandboxaq.com)
 * SPDX-License-Identifier: MIT
=============================================================================*/
#include <oqs/common.h>
#include <oqs/rand.h>
#include "randombytes.h"

int randombytes_init(unsigned char buf[48])
{
    /* Using AES as random generator */
    if (OQS_randombytes_switch_algorithm("NIST-KAT") != OQS_SUCCESS)
    {
        return OQS_ERROR;
    }

    /* Initialize NIST KAT seed by value in `buf` */
    OQS_randombytes_nist_kat_init_256bit(buf, NULL);
    return OQS_SUCCESS;
}

void randombytes(unsigned char *x, unsigned long long xlen)
{
    OQS_randombytes(x, xlen);
}
