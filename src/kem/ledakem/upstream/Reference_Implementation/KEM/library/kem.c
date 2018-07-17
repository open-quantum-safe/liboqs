/**
 *
 * <kem.c>
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

#include "niederreiter_keygen.h"
#include "niederreiter_encrypt.h"
#include "niederreiter_decrypt.h"
#include "rng.h"
#include "sha3.h"
#include <string.h>
/* Generates a keypair - pk is the public key and sk is the secret key. */
int crypto_kem_keypair( unsigned char *pk,
                        unsigned char *sk )
{

   AES_XOF_struct niederreiter_keys_expander;
   randombytes( ((privateKeyNiederreiter_t *)sk)->prng_seed,
                TRNG_BYTE_LENGTH);
   seedexpander_from_trng(&niederreiter_keys_expander,
                          ((privateKeyNiederreiter_t *)sk)->prng_seed);
   key_gen_niederreiter((publicKeyNiederreiter_t *) pk,
                        (privateKeyNiederreiter_t *) sk,
                        &niederreiter_keys_expander);
   return 0;
}

/* Encrypt - pk is the public key, ct is a key encapsulation message
  (ciphertext), ss is the shared secret.*/
int crypto_kem_enc( unsigned char *ct,
                    unsigned char *ss,
                    const unsigned char *pk )
{

   AES_XOF_struct niederreiter_encap_key_expander;
   unsigned char encapsulated_key_seed[TRNG_BYTE_LENGTH];
   randombytes(encapsulated_key_seed,TRNG_BYTE_LENGTH);
   seedexpander_from_trng(&niederreiter_encap_key_expander,encapsulated_key_seed);

   DIGIT error_vector[N0*NUM_DIGITS_GF2X_ELEMENT];
   rand_circulant_blocks_sequence(error_vector,
                                  NUM_ERRORS_T,
                                  &niederreiter_encap_key_expander);

   HASH_FUNCTION((const unsigned char *) error_vector,    // input
                 (N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B), // input Length
                 ss);

   encrypt_niederreiter((DIGIT *) ct,(publicKeyNiederreiter_t *) pk, error_vector);
   return 0;
}


/* Decrypt - ct is a key encapsulation message (ciphertext), sk is the private
   key, ss is the shared secret */

int crypto_kem_dec( unsigned char *ss,
                    const unsigned char *ct,
                    const unsigned char *sk )
{


   DIGIT decoded_error_vector[N0*NUM_DIGITS_GF2X_ELEMENT];
   DIGIT mockup_error_vector[N0*NUM_DIGITS_GF2X_ELEMENT];
   memcpy(mockup_error_vector, ct, NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);

   int decode_ok = decrypt_niederreiter(decoded_error_vector,
                                        (privateKeyNiederreiter_t *)sk,
                                        (DIGIT *)ct);
   if (decode_ok == 1) {
      HASH_FUNCTION((const unsigned char *) decoded_error_vector,
                    (N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B),
                    ss);
      return 0;
   } else { /*Here due to constant time execution and IND-CCA requirements */
      HASH_FUNCTION((const unsigned char *) mockup_error_vector,
                    (N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B),
                    ss);
      return 1;
   }
   return 1;
}
