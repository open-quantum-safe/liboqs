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

#include "hfas_draft.h"

/*
 * We use adrs as if it is read from "left to right" as index 0 to 255, so the
 * layer address of 32 bits is to be found "left".
 *
 * Please note that the functions and macros refer to the bits according to
 * their position within the address and therefore start counting the bits of
 * the address from 1, not from 0.
 *
 * Please note that we are aware of the advantages of macros. We simply use
 * these methods to show the basic idea of address operations.
 *
 */

int
setBits(unsigned char *adrs, unsigned int position, const unsigned int n,
    unsigned char *bits) {
	/* returns 1 if something's wrong with the position or size */
	if (position < 1 || n < 1)
		return 1;
	for (unsigned int i = 1; i <= n; i++) {
		if (TSTBIT(bits,i))
			SETBIT(adrs,(i+position-1));
		else
			CLRBIT(adrs,(i+position-1));
	}
	return 0;
}

void
getBits(unsigned char *adrs, unsigned int position, const unsigned int n,
    unsigned char *bits) {
	for (unsigned int i = 1; i <= n; i++) {
		if (TSTBIT(adrs,(i+position-1)))
			SETBIT(bits,i);
		else
			CLRBIT(bits,i);
	}
}

void
setChainAddress(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[3] = (unsigned char)(bits & 0xFF);
	/*
	 * The chain address is used with OTS.
	 * 32 bits
	 */
	/* set 32 bits, from position 161 */
	setBits(adrs, 161, 32, address);
}

void
setHashAddress(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[3] = (unsigned char)(bits & 0xFF);
	/* set 32 bits, from position 193 */
	setBits(adrs, 193, 32, address);
}

void
setTreeHeight(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[3] = (unsigned char)(bits & 0xFF);
	/* set 32 bits, from position 161 */
	setBits(adrs, 161, 32, address);
}

void
getTreeHeight(unsigned char *adrs, uint32_t *bits) {
	unsigned char address[4];
	uint32_t tmp_bits = 0;
	memset(address, 0, sizeof(address));
	getBits(adrs, 161, 32, address);
	tmp_bits = (((uint32_t)address[0]) << 24)|
	    (((uint32_t)address[1]) << 16)|
	    (((uint32_t)address[2]) << 8)|
	    (uint32_t)address[3];
	memcpy(bits, &tmp_bits, sizeof(uint32_t));
}

void
setTreeIndex(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[3] = (unsigned char)(bits & 0xFF);
	/* set 32 bits, from position 193 */	
	setBits(adrs, 193, 32, address);
}

void
setType(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	address[3] = (unsigned char)(bits & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	/* set 32 bits, from position 97 */
	setBits(adrs, 97, 32, address);
	for (unsigned int i = 129; i <= 256; i++)
		CLRBIT(adrs,i);
}

void
setKeyAndMask(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	address[3] = (unsigned char)(bits & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	/* set 32 bits, from position 225 */
	setBits(adrs, 225, 32, address);
}

void
setOTSAddress(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	address[3] = (unsigned char)(bits & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	/* set 32 bits, from position 129 */
	setBits(adrs, 129, 32, address);
}

void
setLTreeAddress(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	address[3] = (unsigned char)(bits & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	/* set 32 bits, from position 129 */
	setBits(adrs, 129, 32, address);
}

void
getTreeIndex(unsigned char *adrs, uint32_t *bits) {
	uint32_t tmp_bits = 0;
	/* The tree index is needed for hash trees and l-trees. */
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	getBits(adrs, 193, 32, address);
	tmp_bits = (((uint32_t)address[0]) << 24)|
	    (((uint32_t)address[1]) << 16)|
	    (((uint32_t)address[2]) << 8)|
	    (uint32_t)address[3];
	/* get 32 bits, from position 193 */
	memcpy(bits, &tmp_bits, sizeof(uint32_t));
}

/* Only needed for XMSS^MT */
void
setLayerAddress(unsigned char *adrs, const uint32_t bits) {
	unsigned char address[4];
	memset(address, 0, sizeof(address));
	address[3] = (unsigned char)(bits & 0xFF);
	address[2] = (unsigned char)((bits >> 8) & 0xFF);
	address[1] = (unsigned char)((bits >> 16) & 0xFF);
	address[0] = (unsigned char)((bits >> 24) & 0xFF);
	/* set 32 bits, from position 1 */
	setBits(adrs, 1, 32, address);
}

/* Only needed for XMSS^MT */
void
setTreeAddress(unsigned char *adrs, const unsigned long long int bits) {
	unsigned char address[8];
	memset(address, 0, sizeof(address));
	address[7] = (unsigned char)(bits & 0xFF);
	address[6] = (unsigned char)((bits >> 8) & 0xFF);
	address[5] = (unsigned char)((bits >> 16) & 0xFF);
	address[4] = (unsigned char)((bits >> 24) & 0xFF);
	address[3] = (unsigned char)((bits >> 32) & 0xFF);
	address[2] = (unsigned char)((bits >> 40) & 0xFF);
	address[1] = (unsigned char)((bits >> 48) & 0xFF);
	address[0] = (unsigned char)((bits >> 56) & 0xFF);
	/* Setting tree address: 64 bits, from position 33 */
	setBits(adrs, 33, 64, address);
}

