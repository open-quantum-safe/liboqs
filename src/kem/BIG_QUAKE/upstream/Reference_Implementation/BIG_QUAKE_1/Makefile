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
CC = gcc
CFLAGS = -W -Wall -O4
LDFLAGS = -lssl -lcrypto
CPPFLAGS =

OBJECTS = keccak.o kem.o m2e.o permutation.o gf.o poly.o goppa.o decode.o roots.o

.SECONDARY:

.PHONY: force all

all: nistKAT

#
# main targets
#

nistKAT: $(OBJECTS) rng.o PQCgenKAT_kem.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

#
# cleaning rules
#

clean:
	- /bin/rm -f *.o

veryclean: clean
	- /bin/rm nistKAT PQCkemKAT_*.req PQCkemKAT_*.rsp

# DO NOT DELETE

decode.o: goppa.h gf.h poly.h permutation.h
gf.o: gf.h rng.h
goppa.o: goppa.h gf.h poly.h permutation.h
kem.o: kem.h sizes.h gf.h api.h rng.h goppa.h poly.h permutation.h
kem.o: m2e.h
m2e.o: m2e.h sizes.h gf.h api.h
permutation.o: permutation.h rng.h
poly.o: gf.h poly.h
PQCgenKAT_kem.o: rng.h api.h sizes.h gf.h
rng.o: rng.h
roots.o: gf.h poly.h
