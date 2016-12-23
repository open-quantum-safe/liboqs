/*
 * Copyright (c) 2016, Denis Butin and Stefan-Lukas Gazdag
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>

#include <stdint.h>
#include "hfas_draft.h"
#include "SimpleFIPS202.h"

/* Choose between SHA2 and SHA3 by setting the value to 2 or 3 */
#define SHA 2

void toByte(uint8_t *buf, const uint32_t x, const uint32_t y);
void hash256(unsigned char *out, const unsigned char *in,
    const unsigned int inlen, const int algo);
void hash512(unsigned char *out, const unsigned char *in,
    const unsigned int inlen, const int algo);
int F(unsigned char *out, const unsigned char *in, const unsigned char *key,
    const unsigned int len);
int H(unsigned char *out, const unsigned char *in, const unsigned char *key,
    const unsigned int len);
int H_msg(unsigned char *out, const unsigned char *in,
    const unsigned int msglen, const unsigned char *key,
    const unsigned int len);
void concat_bytes(unsigned char *concat, const unsigned char *left,
    const unsigned int bytes_left, const unsigned char *right,
    const unsigned int bytes_right);

#endif

