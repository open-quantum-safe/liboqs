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
#ifndef SIZES_H
#define SIZES_H

#include "gf.h"

// "#define" related to generic bit/bytes/sizes
// size of a hash (in bytes)
#define HASH_SIZE 32
// number of bytes needed for storing nb_bits bits
#define BITS_TO_BYTES(nb_bits) (((nb_bits) - 1) / 8 + 1)
// number bits in one long = __WORDSIZE
#define BIT_SIZE_OF_LONG (8 * sizeof(long))
// number of long needed for storing nb_bits bits
#define BITS_TO_LONG(nb_bits) (((nb_bits) - 1) / BIT_SIZE_OF_LONG + 1)

// Goppa codes
#define EXT_DEGREE GF_EXT_DEGREE
#define CODIMENSION (NB_ERRORS * EXT_DEGREE)
#define DIMENSION (LENGTH - CODIMENSION)
#define SYNDROME_LENGTH CODIMENSION
#define SYNDROME_BYTES BITS_TO_BYTES(SYNDROME_LENGTH)

// sizes of keys  and ciphertexts
#define SECRETKEY_BYTES ((LENGTH + 1 + 2 * NB_ERRORS) * sizeof (gfelt_t))
#define PUBLICKEY_BYTES (BITS_TO_BYTES(CODIMENSION) * ((DIMENSION) / (ORDER)))
#define CIPHERTEXT_LENGTH (2*(8*HASH_SIZE) + SYNDROME_LENGTH)
#define CIPHERTEXT_BYTES BITS_TO_BYTES(CIPHERTEXT_LENGTH)

// size of the random input for the encapsaluation
#define RANDOM_LENGTH 128
#define RANDOM_BYTES BITS_TO_BYTES(RANDOM_LENGTH)

// access to the i-th bit of a char vector v
#define ind(v, i) ((v)[(i) / 8] >> ((i) % 8) & 1)


#define show_params() {							\
	printf("(n,m,t)=(%d,%d,%d)\n", LENGTH, EXT_DEGREE, NB_ERRORS);	\
	printf("Ciphertext: %d bits\n", CIPHERTEXT_LENGTH);		\
	printf("Public key: %ld bytes\n", PUBLICKEY_BYTES);		\
	printf("Secret key: %ld bytes\n", SECRETKEY_BYTES);		\
    }

#endif
