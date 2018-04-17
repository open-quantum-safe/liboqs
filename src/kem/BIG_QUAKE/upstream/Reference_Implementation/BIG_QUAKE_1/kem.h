/******************************************************************************
 * BIG QUAKE -- BInary Goppa QUAsi-cyclic Key Encapsulation
 *
 * Copyright (c) 2017 the BIG QUAKE members
 * https://bigquake.inria.fr/
 *
 * Permission to use this code for BIG QUAKE is granted.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ""AS IS"" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS CORPORATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef KEM_H
#define KEM_H

#include <string.h>
#include "sizes.h"
#include "api.h"


void xor(unsigned char * a, unsigned char * b, int length);
#define addto(a, b) (xor(a, b, BITS_TO_BYTES(CODIMENSION)))

void shift_cols(unsigned char * in, unsigned char * out, int d);


int encrypt_nied(OUT unsigned char *syndrome, IN int * e, unsigned char * pk);
int decrypt_nied(IN unsigned char *syndrome, OUT int * error, IN unsigned char * sk);

#endif
