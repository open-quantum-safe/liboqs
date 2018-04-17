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
#include <stdlib.h>
#include "permutation.h"
#include "rng.h"

void swap(index_t * permutation, index_t a, index_t b) {
  index_t tmp = permutation[a];
  permutation[a] = permutation[b];
  permutation[b] = tmp;
}

/*
	assumes initialisation with randombytes_init()
	return a random index_t
*/
static index_t * buff = NULL;
static int buff_size = 0;

void randindex_init(int rand_size) {
	buff_size = rand_size;
	buff = (index_t *) realloc(buff, buff_size * sizeof(index_t));
	randombytes((unsigned char *) buff, buff_size * sizeof(index_t));
}

index_t randindex() {
	buff_size--;
	if (buff_size < 0) {
		/* generate 16 extra bytes */
		randombytes((unsigned char *) buff, 16);
		buff_size = 16 / sizeof (index_t) - 1;
	}

	return buff[buff_size];
}

index_t uniform(index_t module) {
  index_t res;
  while(1) {
		res = randindex();
		/* assumes type index_t strictly smaller than unsigned int */
    if (res >= ((1UL << (8 * sizeof (index_t))) - ((1UL << (8 * sizeof (index_t))) % module))) {
      continue;
    }
    return res % module;
  }
}

void permute(index_t* permutation, index_t start, index_t n) {
	int i, j;
	// heuristic, 20% extra
	randindex_init(((n - start) * 12) / 10);
  for (i=start; i<n-1; i++) {
    j = uniform(n-i);
    swap(permutation, i, i+j);
  }
}

void change_permutation(index_t* permutation, index_t start, index_t size_permutation) {
	permute(permutation, start, size_permutation);
}


void generate_permutation (index_t * permutation, index_t size_permutation) {
	int i;

	for (i = 0; i < size_permutation; ++i) {
		permutation[i] = i;
	}
	permute(permutation, 0, size_permutation);
}
