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

#ifndef HFAS_DRAFT_H
#define HFAS_DRAFT_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Hash Function Address Scheme */

/* macros for the use of a character array */
#define SETBIT(ByteArr,Bit) ((ByteArr)[(Bit-1)>>3] |=  (1<<(7-((Bit-1)&0x07))))
#define CLRBIT(ByteArr,Bit) ((ByteArr)[(Bit-1)>>3] &= ~(1<<(7-((Bit-1)&0x07))))
#define TSTBIT(ByteArr,Bit) ((ByteArr)[(Bit-1)>>3] &   (1<<(7-((Bit-1)&0x07))))

int setBits(unsigned char *adrs, unsigned int position, const unsigned int n,
    unsigned char *bits);
void getBits(unsigned char *adrs, unsigned int position, const unsigned int n,
    unsigned char *bits);
void setChainAddress(unsigned char *adrs, const uint32_t bits);
void setHashAddress(unsigned char *adrs, const uint32_t bits);
void setTreeHeight(unsigned char *adrs, const uint32_t bits);
void getTreeHeight(unsigned char *adrs, uint32_t *bits);
void setTreeIndex(unsigned char *adrs, const uint32_t bits);
void setType(unsigned char *adrs, const uint32_t bits);
void setKeyAndMask(unsigned char *adrs, const uint32_t bits);
void setOTSAddress(unsigned char *adrs, const uint32_t bits);
void setLTreeAddress(unsigned char *adrs, const uint32_t bits);
void getTreeIndex(unsigned char *adrs, uint32_t *bits);
void setLayerAddress(unsigned char *adrs, const uint32_t bits);
void setTreeAddress(unsigned char *adrs, const unsigned long long int bits);

#endif

