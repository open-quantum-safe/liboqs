/**
 *
 * <api.h>
 *
 * @version 1.0 (September 2017)
 *
 * Reference ISO-C99 Implementation of LEDAkem cipher" using GCC built-ins.
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
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

#pragma once

#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#include "gf2x_arith_mod_xPplusOne.h"

#define CRYPTO_ALGNAME "LEDA"

/* required bytes of input randomness */
#define  CRYPTO_RANDOMBYTES TRNG_BYTE_LENGTH

/* size in bytes of the secret key */
#define  CRYPTO_SECRETKEYBYTES TRNG_BYTE_LENGTH

/* size in bytes of the public key */
#define  CRYPTO_PUBLICKEYBYTES ((N0-1)*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B)

/* size in bytes of the shared secret */
#define  CRYPTO_BYTES HASH_BYTE_LENGTH

/*size in bytes of the ciphertext*/
#define  CRYPTO_CIPHERTEXTBYTES (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B)

/* Your functions must return 0 to indicate success, -1 to indicate an error
 * condition */

/* Generates a keypair - pk is the public key and sk is the secret key. */
int crypto_kem_keypair( unsigned char *pk,
                        unsigned char *sk );

/* Encrypt - pk is the public key, ct is a key encapsulation message
  (ciphertext), ss is the shared secret.*/
int crypto_kem_enc( unsigned char *ct,
                    unsigned char *ss,
                    const unsigned char *pk );


/* Decrypt - ct is a key encapsulation message (ciphertext), sk is the private
   key, ss is the shared secret */

int crypto_kem_dec( unsigned char *ss,
                    const unsigned char *ct,
                    const unsigned char *sk );
