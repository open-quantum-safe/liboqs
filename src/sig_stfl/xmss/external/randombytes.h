/*=============================================================================
 * Copyright (c) 2022 by SandboxAQ Inc
 * Author: Duc Tri Nguyen (ductri.nguyen@sandboxaq.com)
 * SPDX-License-Identifier: MIT
=============================================================================*/
#ifndef XMSS_RANDOMBYTES_H
#define XMSS_RANDOMBYTES_H

#include "namespace.h"
/*
 * Tries to read xlen bytes from a source of randomness, and writes them to x.
 */
#define randombytes XMSS_INNER_NAMESPACE(randombytes)
void randombytes(unsigned char *x, unsigned long long xlen);

#endif
