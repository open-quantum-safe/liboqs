/**
 *
 * <rng.c>
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


#include "rng.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h> // void srand(unsigned int seed); int rand(void); RAND_MAX
#include <string.h> // void *memset(void *s, int c, size_t n);
#define __USE_POSIX199309
#include <time.h> // struct timespec; clock_gettime(...); CLOCK_REALTIME

#include "aes256.h"
#include "qc_ldpc_parameters.h"


/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*              start PSEUDO-RAND GENERATOR ROUTINES for rnd.h                */
/*----------------------------------------------------------------------------*/

#if 0
void initialize_pseudo_random_generator_seed(int ac, char *av[])
{

   if (ac == 2)
      srand(atoi(av[1]));
   else {
      struct timespec seedValue;
      clock_gettime(CLOCK_REALTIME, &seedValue);
      srand(seedValue.tv_nsec);
   } // end else-if
   unsigned char pseudo_entropy[48];
   for (int i=0; i< 48; i++) pseudo_entropy[i] = rand() & 0xff;
   randombytes_init(pseudo_entropy,
                    NULL,
                    0 /*unused in NIST function*/);


} // end initilize_pseudo_random_sequence_seed

#endif
/*----------------------------------------------------------------------------*/

/* Initializes a dedicated DRBG context to avoid conflicts with the global one
 * declared by NIST for KATs. Provides the output of the DRBG in output, for
 * the given length */


/*----------------------------------------------------------------------------*/
/*              end PSEUDO-RAND GENERATOR ROUTINES for rnd.h                  */
/*----------------------------------------------------------------------------*/

AES256_CTR_DRBG_struct  DRBG_ctx;

void    AES256_ECB(unsigned char *key, unsigned char *ctr,
                   unsigned char *buffer);

/*
 seedexpander_init()
 ctx            - stores the current state of an instance of the seed expander
 seed           - a 32 byte random value
 diversifier    - an 8 byte diversifier
 maxlen         - maximum number of bytes (less than 2**32) generated under this seed and diversifier
 */
int
seedexpander_init(AES_XOF_struct *ctx,
                  unsigned char *seed,
                  unsigned char *diversifier,
                  unsigned long maxlen)
{
   if ( maxlen >= 0x100000000 )
      return RNG_BAD_MAXLEN;

   ctx->length_remaining = maxlen;

   memset(ctx->key, 0, 32);
   int max_accessible_seed_len = TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH;
   memcpy(ctx->key, seed, max_accessible_seed_len);

   memcpy(ctx->ctr, diversifier, 8);
   ctx->ctr[11] = maxlen % 256;
   maxlen >>= 8;
   ctx->ctr[10] = maxlen % 256;
   maxlen >>= 8;
   ctx->ctr[9] = maxlen % 256;
   maxlen >>= 8;
   ctx->ctr[8] = maxlen % 256;
   memset(ctx->ctr+12, 0x00, 4);

   ctx->buffer_pos = 16;
   memset(ctx->buffer, 0x00, 16);

   return RNG_SUCCESS;
}

/*
 seedexpander()
    ctx  - stores the current state of an instance of the seed expander
    x    - returns the XOF data
    xlen - number of bytes to return
 */
int
seedexpander(AES_XOF_struct *ctx, unsigned char *x, unsigned long xlen)
{
   unsigned long   offset;

   if ( x == NULL )
      return RNG_BAD_OUTBUF;
   if ( xlen >= ctx->length_remaining )
      return RNG_BAD_REQ_LEN;

   ctx->length_remaining -= xlen;

   offset = 0;
   while ( xlen > 0 ) {
      if ( xlen <= (16-ctx->buffer_pos) ) { // buffer has what we need
         memcpy(x+offset, ctx->buffer+ctx->buffer_pos, xlen);
         ctx->buffer_pos += xlen;

         return RNG_SUCCESS;
      }

      // take what's in the buffer
      memcpy(x+offset, ctx->buffer+ctx->buffer_pos, 16-ctx->buffer_pos);
      xlen -= 16-ctx->buffer_pos;
      offset += 16-ctx->buffer_pos;

      AES256_ECB(ctx->key, ctx->ctr, ctx->buffer);
      ctx->buffer_pos = 0;

      //increment the counter
      for (int i=15; i>=12; i--) {
         if ( ctx->ctr[i] == 0xff )
            ctx->ctr[i] = 0x00;
         else {
            ctx->ctr[i]++;
            break;
         }
      }

   }

   return RNG_SUCCESS;
}

// Use whatever AES implementation you have. This uses AES from openSSL library
//    key - 256-bit AES key
//    ptx - a 128-bit plaintext value
//    ctx - a 128-bit ciphertext value

void
AES256_ECB(unsigned char *key, unsigned char *ptx, unsigned char *ctx)
{
   uint32_t round_key[4*(NROUNDS + 1)];
   rijndaelKeySetupEnc(round_key, key, KEYLEN_b);
   rijndaelEncrypt(round_key, NROUNDS, ptx, ctx);
}
#if 0
void
randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength)
{
   unsigned char   seed_material[48];

   memcpy(seed_material, entropy_input, 48);
   if (personalization_string)
      for (int i=0; i<48; i++)
         seed_material[i] ^= personalization_string[i];
   memset(DRBG_ctx.Key, 0x00, 32);
   memset(DRBG_ctx.V, 0x00, 16);
   AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
   DRBG_ctx.reseed_counter = 1;
}

int
randombytes(unsigned char *x, unsigned long long xlen)
{
   unsigned char   block[16];
   int             i = 0;

   while ( xlen > 0 ) {
      //increment V
      for (int j=15; j>=0; j--) {
         if ( DRBG_ctx.V[j] == 0xff )
            DRBG_ctx.V[j] = 0x00;
         else {
            DRBG_ctx.V[j]++;
            break;
         }
      }
      AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
      if ( xlen > 15 ) {
         memcpy(x+i, block, 16);
         i += 16;
         xlen -= 16;
      } else {
         memcpy(x+i, block, xlen);
         xlen = 0;
      }
   }
   AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
   DRBG_ctx.reseed_counter++;

   return RNG_SUCCESS;
}
#endif
void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *V)
{
   unsigned char   temp[48];

   for (int i=0; i<3; i++) {
      //increment V
      for (int j=15; j>=0; j--) {
         if ( V[j] == 0xff )
            V[j] = 0x00;
         else {
            V[j]++;
            break;
         }
      }

      AES256_ECB(Key, V, temp+16*i);
   }
   if ( provided_data != NULL )
      for (int i=0; i<48; i++)
         temp[i] ^= provided_data[i];
   memcpy(Key, temp, 32);
   memcpy(V, temp+32, 16);
}

#if 0

void deterministic_random_byte_generator(unsigned char *const output,
      const unsigned long long output_len,
      const unsigned char *const seed,
      const unsigned long long seed_length
                                        )
{
   /* DRBG context initialization */
   AES256_CTR_DRBG_struct ctx;
   unsigned char   seed_material[48];

   memcpy(seed_material, seed, 48);
   memset(ctx.Key, 0x00, 32);
   memset(ctx.V, 0x00, 16);
   AES256_CTR_DRBG_Update(seed_material, ctx.Key, ctx.V);
   ctx.reseed_counter = 1;

   /* Actual DRBG computation as from the randombytes(unsigned char *x,
    * unsigned long long xlen) from NIST */

   unsigned char   block[16];
   int             i = 0, length_remaining;

   length_remaining=output_len;

   while ( length_remaining > 0 ) {
      //increment V
      for (int j=15; j>=0; j--) {
         if ( ctx.V[j] == 0xff )
            ctx.V[j] = 0x00;
         else {
            ctx.V[j]++;
            break;
         }
      }
      AES256_ECB(ctx.Key, ctx.V, block);
      if ( length_remaining > 15 ) {
         memcpy(output+i, block, 16);
         i += 16;
         length_remaining -= 16;
      } else {
         memcpy(output+i, block, length_remaining);
         length_remaining = 0;
      }
   }
   AES256_CTR_DRBG_Update(NULL, ctx.Key, ctx.V);
   ctx.reseed_counter++;

} // end deterministic_random_byte_generator
#endif

void seedexpander_from_trng(AES_XOF_struct *ctx,
                            const unsigned char *trng_entropy
                            /* TRNG_BYTE_LENGTH wide buffer */)
{

   /*the NIST seedexpander will however access 32B from this buffer */
   unsigned int prng_buffer_size = TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH;
   unsigned char prng_buffer[TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH] = { 0x00 };
   memcpy(prng_buffer,
          trng_entropy,
          TRNG_BYTE_LENGTH < prng_buffer_size ? TRNG_BYTE_LENGTH : prng_buffer_size);
   /* if extra entropy is provided, add it to the diversifier */
#if TRNG_BYTE_LENGTH == 40
   unsigned char *diversifier = ((unsigned char *)trng_entropy)+32;
#else
   unsigned char diversifier[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
#endif
   /* the required seed expansion will be quite small, set the max number of
    * bytes conservatively to 10 MiB*/
   seedexpander_init(ctx,prng_buffer,diversifier,10*1024*1024);
}
