/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.2 (July 2025)
 *
 * Authors listed in alphabetical order:
 *
 * @author: Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author: Marco Gianvecchio <marco.gianvecchio@mail.polimi.it>
 * @author: Patrick Karl <patrick.karl@tum.de>
 * @author: Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author: Jonas Schupp <jonas.schupp@tum.de>
 *
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#ifndef PQCLEAN_CROSSRSDPG192FAST_CLEAN_API_H
#define PQCLEAN_CROSSRSDPG192FAST_CLEAN_API_H

#pragma once

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_CROSSRSDPG192FAST_CLEAN_CRYPTO_ALGNAME "cross-rsdpg-192-fast"

/*  no. of bytes of the secret key */
#define PQCLEAN_CROSSRSDPG192FAST_CLEAN_CRYPTO_SECRETKEYBYTES 48

/*  no. of bytes of the public key */
#define PQCLEAN_CROSSRSDPG192FAST_CLEAN_CRYPTO_PUBLICKEYBYTES 83

/* no. of bytes of overhead in a signed message */
#define PQCLEAN_CROSSRSDPG192FAST_CLEAN_CRYPTO_BYTES 26772

/* required bytes of input randomness */
#define PQCLEAN_CROSSRSDPG192FAST_CLEAN_CRYPTO_RANDOMBYTES 24


int PQCLEAN_CROSSRSDPG192FAST_CLEAN_crypto_sign_keypair(unsigned char *pk,
        unsigned char *sk
                                                       );

int PQCLEAN_CROSSRSDPG192FAST_CLEAN_crypto_sign(unsigned char *sm,
        size_t *smlen,
        const unsigned char *m,
        size_t mlen,
        const unsigned char *sk
                                               );

int PQCLEAN_CROSSRSDPG192FAST_CLEAN_crypto_sign_open(unsigned char *m,
        size_t *mlen,
        const unsigned char *sm,
        size_t smlen,
        const unsigned char *pk
                                                    );

int PQCLEAN_CROSSRSDPG192FAST_CLEAN_crypto_sign_signature(unsigned char *sig,
        size_t *siglen,
        const unsigned char *m,
        size_t mlen,
        const unsigned char *sk
                                                         );

int PQCLEAN_CROSSRSDPG192FAST_CLEAN_crypto_sign_verify(const unsigned char *sig,
        size_t siglen,
        const unsigned char *m,
        size_t mlen,
        const unsigned char *pk
                                                      );

#endif
