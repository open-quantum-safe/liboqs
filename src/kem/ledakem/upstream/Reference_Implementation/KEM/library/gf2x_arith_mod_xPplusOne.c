/**
 *
 * <gf2x_arith_mod_xPplusOne.c>
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


#include "gf2x_arith_mod_xPplusOne.h"
#include "rng.h"
#include <string.h>  // memcpy(...), memset(...)
#include <assert.h>

/*----------------------------------------------------------------------------*/

void gf2x_mod(DIGIT out[],
              const int nin, const DIGIT in[])
{

   long int i, j, posTrailingBit, maskOffset;
   DIGIT mask, aux[nin];

   memcpy(aux, in, nin*DIGIT_SIZE_B);
   memset(out, 0x00, NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   if (nin < NUM_DIGITS_GF2X_MODULUS) {
      for (i = 0; i < nin; i++) out[NUM_DIGITS_GF2X_ELEMENT-1-i] = in[nin-1-i];
      return;
   }

   for (i = 0; i < nin-NUM_DIGITS_GF2X_MODULUS; i += 1) {
      for (j = DIGIT_SIZE_b-1; j >= 0; j--) {
         mask = ((DIGIT)0x1) << j;
         if (aux[i] & mask) {
            aux[i] ^= mask;
            posTrailingBit = (DIGIT_SIZE_b-1-j) + i*DIGIT_SIZE_b + P;
            maskOffset = (DIGIT_SIZE_b-1-(posTrailingBit % DIGIT_SIZE_b));
            mask = (DIGIT) 0x1 << maskOffset;
            aux[posTrailingBit/DIGIT_SIZE_b] ^= mask;
         }
      }
   }

   for (j = DIGIT_SIZE_b-1; j >= MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS; j--) {
      mask = ((DIGIT)0x1) << j;
      if (aux[i] & mask) {
         aux[i] ^= mask;
         posTrailingBit = (DIGIT_SIZE_b-1-j) + i*DIGIT_SIZE_b + P;
         maskOffset = (DIGIT_SIZE_b-1-(posTrailingBit % DIGIT_SIZE_b));
         mask = (DIGIT) 0x1 << maskOffset;
         aux[posTrailingBit/DIGIT_SIZE_b] ^= mask;
      }
   }

   int to_copy = (nin > NUM_DIGITS_GF2X_ELEMENT) ? NUM_DIGITS_GF2X_ELEMENT : nin;

   for (i = 0; i < to_copy; i++) {
      out[NUM_DIGITS_GF2X_ELEMENT-1-i] = aux[nin-1-i];
   }

} // end gf2x_mod

/*----------------------------------------------------------------------------*/

static
void left_bit_shift(const int length, DIGIT in[])
{

   int j;
   for (j = 0; j < length-1; j++) {
      in[j] <<= 1;                    /* logical shift does not need clearing */
      in[j] |= in[j+1] >> (DIGIT_SIZE_b-1);
   }
   in[j] <<= 1;
} // end left_bit_shift

/*----------------------------------------------------------------------------*/

static
void right_bit_shift(const int length, DIGIT in[])
{

   int j;
   for (j = length-1; j > 0 ; j--) {
      in[j] >>= 1;
      in[j] |=  (in[j-1] & (DIGIT)0x01) << (DIGIT_SIZE_b-1);
   }
   in[j] >>=1;
} // end right_bit_shift

/*----------------------------------------------------------------------------*/


/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void right_bit_shift_n(const int length, DIGIT in[], const int amount)
{
   assert(amount < DIGIT_SIZE_b);
   if ( amount == 0 ) return;
   int j;
   DIGIT mask;
   mask = ((DIGIT)0x01 << amount) - 1;
   for (j = length-1; j > 0 ; j--) {
      in[j] >>= amount;
      in[j] |=  (in[j-1] & mask) << (DIGIT_SIZE_b - amount);
   }
   in[j] >>= amount;
} // end right_bit_shift_n

/*----------------------------------------------------------------------------*/

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */

void left_bit_shift_n(const int length, DIGIT in[], const int amount)
{
   assert(amount < DIGIT_SIZE_b);
   if ( amount == 0 ) return;
   int j;
   DIGIT mask;
   mask = ~(((DIGIT)0x01 << (DIGIT_SIZE_b - amount)) - 1);
   for (j = 0 ; j < length-1 ; j++) {
      in[j] <<= amount;
      in[j] |=  (in[j+1] & mask) >> (DIGIT_SIZE_b - amount);
   }
   in[j] <<= amount;
} // end right_bit_shift_n

/*----------------------------------------------------------------------------*/
/* shifts by whole digits */

void left_DIGIT_shift_n(const int length, DIGIT in[], int amount)
{
   int j;
   for (j = 0; (j + amount) < length; j++) {
      in[j] = in[j+amount];
   }
   for (; j < length; j++) {
      in[j] = (DIGIT)0;
   }
} // end left_bit_shift_n

/*----------------------------------------------------------------------------*/
/* may shift by an arbitrary amount*/

void left_bit_shift_wide_n(const int length, DIGIT in[], int amount)
{
   left_DIGIT_shift_n(length, in, amount / DIGIT_SIZE_b);
   left_bit_shift_n(length, in, amount % DIGIT_SIZE_b);
} // end left_bit_shift_n

/*----------------------------------------------------------------------------*/

#if (defined(DIGIT_IS_UINT8) || defined(DIGIT_IS_UINT16))
static
uint8_t byte_reverse_with_less32bitDIGIT(uint8_t b)
{
   uint8_t r = b;
   int s = (sizeof(b) << 3) - 1;
   for (b >>= 1; b; b >>= 1) {
      r <<= 1;
      r |= b & 1;
      s--;
   }
   r <<= s;
   return r;
} // end byte_reverse_less32bitDIGIT
#endif

#if defined(DIGIT_IS_UINT32)
static
uint8_t byte_reverse_with_32bitDIGIT(uint8_t b)
{
   b = ( (b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)
       ) * 0x10101LU >> 16;
   return b;
} // end byte_reverse_32bitDIGIT
#endif

#if defined(DIGIT_IS_UINT64)
static
uint8_t byte_reverse_with_64bitDIGIT(uint8_t b)
{
   b = (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;
   return b;
} // end byte_reverse_64bitDIGIT
#endif

/*----------------------------------------------------------------------------*/

static
DIGIT reverse_digit(const DIGIT b)
{
   int i;
   union toReverse_t {
      uint8_t inByte[DIGIT_SIZE_B];
      DIGIT digitValue;
   } toReverse;

   toReverse.digitValue = b;
#if defined(DIGIT_IS_UINT64)
   for (i = 0; i < DIGIT_SIZE_B; i++)
      toReverse.inByte[i] = byte_reverse_with_64bitDIGIT(toReverse.inByte[i]);
   return __builtin_bswap64(toReverse.digitValue);
#elif defined(DIGIT_IS_UINT32)
   for (i = 0; i < DIGIT_SIZE_B; i++)
      toReverse.inByte[i] = byte_reverse_with_32bitDIGIT(toReverse.inByte[i]);
   return __builtin_bswap32(toReverse.digitValue);
#elif defined(DIGIT_IS_UINT16)
   for (i = 0; i < DIGIT_SIZE_B; i++)
      toReverse.inByte[i] = byte_reverse_with_less32bitDIGIT(toReverse.inByte[i]);
   reversed = __builtin_bswap16(toReverse.digitValue);
#elif defined(DIGIT_IS_UINT8)
   return byte_reverse_with_less32bitDIGIT(toReverse.inByte[0]);
#else
#error "Missing implementation for reverse_digit(...) \
with this CPU word bitsize !!! "
#endif    
   return toReverse.digitValue;
} // end reverse_digit


/*----------------------------------------------------------------------------*/

void gf2x_transpose_in_place(DIGIT A[])
{
   /* it keeps the lsb in the same position and
    * inverts the sequence of the remaining bits
    */

   DIGIT mask = (DIGIT)0x1;
   DIGIT rev1, rev2, a00;
   int i, slack_bits_amount = NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b - P;

   if (NUM_DIGITS_GF2X_ELEMENT == 1) {
      a00 = A[0] & mask;
      right_bit_shift(1, A);
      rev1 = reverse_digit(A[0]);
#if (NUM_DIGITS_GF2X_MOD_P_ELEMENT*DIGIT_SIZE_b - P)
      rev1 >>= (DIGIT_SIZE_b-(P%DIGIT_SIZE_b));
#endif
      A[0] = (rev1 & (~mask)) | a00;
      return;
   }

   a00 = A[NUM_DIGITS_GF2X_ELEMENT-1] & mask;
   right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, A);

   for (i = NUM_DIGITS_GF2X_ELEMENT-1; i >= (NUM_DIGITS_GF2X_ELEMENT+1)/2; i--) {
      rev1 = reverse_digit(A[i]);
      rev2 = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT-1-i]);
      A[i] = rev2;
      A[NUM_DIGITS_GF2X_ELEMENT-1-i] = rev1;
   }
   if (NUM_DIGITS_GF2X_ELEMENT % 2 == 1)
      A[NUM_DIGITS_GF2X_ELEMENT/2] = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT/2]);

   if (slack_bits_amount)
      right_bit_shift_n(NUM_DIGITS_GF2X_ELEMENT, A,slack_bits_amount);
   A[NUM_DIGITS_GF2X_ELEMENT-1] = (A[NUM_DIGITS_GF2X_ELEMENT-1] & (~mask)) | a00;
} // end transpose_in_place

/*----------------------------------------------------------------------------*/

void rotate_bit_left(DIGIT in[])   /*  equivalent to x * in(x) mod x^P+1 */
{

   DIGIT mask,rotated_bit;

   if (NUM_DIGITS_GF2X_MODULUS == NUM_DIGITS_GF2X_ELEMENT) {

      int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS-1;
      mask = ((DIGIT)0x1) << msb_offset_in_digit;
      rotated_bit = !!(in[0] & mask);
      in[0] &= ~mask;                     /* clear shifted bit */
      left_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);
   } else {
      /* NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT and
              * MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0
              */
      mask =  ((DIGIT)0x1) << (DIGIT_SIZE_b-1);
      rotated_bit = !!(in[0] & mask);
      in[0] &= ~mask;                     /* clear shifted bit */
      left_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);

   }
   in[NUM_DIGITS_GF2X_ELEMENT-1] |= rotated_bit;
} // end rotate_bit_left



/*----------------------------------------------------------------------------*/

void rotate_bit_right(DIGIT in[])   /*  x^{-1} * in(x) mod x^P+1 */
{

   DIGIT rotated_bit = in[NUM_DIGITS_GF2X_ELEMENT-1] & ((DIGIT)0x1);
   right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);

   if (NUM_DIGITS_GF2X_MODULUS == NUM_DIGITS_GF2X_ELEMENT) {
      int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS-1;
      rotated_bit = rotated_bit << msb_offset_in_digit;
   } else {
      /* NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT and
              * MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0
              */
      rotated_bit = rotated_bit << (DIGIT_SIZE_b-1);
   }
   in[0] |= rotated_bit;
} // end rotate_bit_right

/*----------------------------------------------------------------------------*/

static
void gf2x_swap(const int length,
               DIGIT f[],
               DIGIT s[])
{
   DIGIT t;
   for (int i = length-1; i >= 0; i--) {
      t = f[i];
      f[i] = s[i];
      s[i] = t;
   }
}  // end gf2x_swap

/*----------------------------------------------------------------------------*/

/*
 * Optimized extended GCD algorithm to compute the multiplicative inverse of
 * a non-zero element in GF(2)[x] mod x^P+1, in polyn. representation.
 *
 * H. Brunner, A. Curiger, and M. Hofstetter. 1993.
 * On Computing Multiplicative Inverses in GF(2^m).
 * IEEE Trans. Comput. 42, 8 (August 1993), 1010-1015.
 * DOI=http://dx.doi.org/10.1109/12.238496
 *
 *
 * Henri Cohen, Gerhard Frey, Roberto Avanzi, Christophe Doche, Tanja Lange,
 * Kim Nguyen, and Frederik Vercauteren. 2012.
 * Handbook of Elliptic and Hyperelliptic Curve Cryptography,
 * Second Edition (2nd ed.). Chapman & Hall/CRC.
 * (Chapter 11 -- Algorithm 11.44 -- pag 223)
 *
 */

int gf2x_mod_inverse(DIGIT out[], const DIGIT in[])     /* in^{-1} mod x^P-1 */
{

   int i;
   long int delta = 0;
   DIGIT u[NUM_DIGITS_GF2X_ELEMENT] = {0},
                                      v[NUM_DIGITS_GF2X_ELEMENT] = {0},
                                            s[NUM_DIGITS_GF2X_MODULUS] = {0},
                                                  f[NUM_DIGITS_GF2X_MODULUS] = {0};

   DIGIT mask;
   u[NUM_DIGITS_GF2X_ELEMENT-1] = 0x1;
   v[NUM_DIGITS_GF2X_ELEMENT-1] = 0x0;

   s[NUM_DIGITS_GF2X_MODULUS-1] = 0x1;
   if (MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0)
      mask = 0x1;
   else
      mask = (((DIGIT)0x1) << MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS);
   s[0] |= mask;

   for (i = NUM_DIGITS_GF2X_ELEMENT-1; i>=0 && in[i] == 0; i--);
   if (i < 0) return 0;

   if (NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT)
      for (i = NUM_DIGITS_GF2X_MODULUS-1; i >= 1 ; i--) f[i] = in[i-1];
   else  /* they are equal */
      for (i = NUM_DIGITS_GF2X_MODULUS-1; i >= 0 ; i--) f[i] = in[i];

   for (i = 1; i <= 2*P; i++) {
      if ( (f[0] & mask) == 0 ) {
         left_bit_shift(NUM_DIGITS_GF2X_MODULUS, f);
         rotate_bit_left(u);
         delta += 1;
      } else {
         if ( (s[0] & mask) != 0) {
            gf2x_add(NUM_DIGITS_GF2X_MODULUS, s,
                     NUM_DIGITS_GF2X_MODULUS, s,
                     NUM_DIGITS_GF2X_MODULUS, f);
            gf2x_mod_add(v, v, u);
         }
         left_bit_shift(NUM_DIGITS_GF2X_MODULUS, s);
         if ( delta == 0 ) {
            gf2x_swap(NUM_DIGITS_GF2X_MODULUS, f, s);
            gf2x_swap(NUM_DIGITS_GF2X_ELEMENT, u, v);
            rotate_bit_left(u);
            delta = 1;
         } else {
            rotate_bit_right(u);
            delta = delta - 1;
         }
      }
   }

   for (i = NUM_DIGITS_GF2X_ELEMENT-1; i >= 0 ; i--) out[i] = u[i];

   return (delta == 0);
} // end gf2x_mod_inverse

/*----------------------------------------------------------------------------*/

void gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[])
{

   DIGIT aux[2*NUM_DIGITS_GF2X_ELEMENT];
   gf2x_mul_comb(2*NUM_DIGITS_GF2X_ELEMENT, aux,
                 NUM_DIGITS_GF2X_ELEMENT, A,
                 NUM_DIGITS_GF2X_ELEMENT, B);
   gf2x_mod(Res, 2*NUM_DIGITS_GF2X_ELEMENT, aux);

} // end gf2x_mod_mul

/*----------------------------------------------------------------------------*/

/*PRE: the representation of the sparse coefficients is sorted in increasing
 order of the coefficients themselves */
void gf2x_mod_mul_dense_to_sparse(DIGIT Res[],
                                  const DIGIT dense[],
                                  POSITION_T sparse[],
                                  unsigned int nPos)
{
   DIGIT aux[2*NUM_DIGITS_GF2X_ELEMENT] = {0x00};
   DIGIT resDouble[2*NUM_DIGITS_GF2X_ELEMENT] = {0x00};
   memcpy(aux+NUM_DIGITS_GF2X_ELEMENT,dense, NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   memcpy(resDouble+NUM_DIGITS_GF2X_ELEMENT,dense,
          NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);

   if(sparse[0] != INVALID_POS_VALUE) {
      left_bit_shift_wide_n(2*NUM_DIGITS_GF2X_ELEMENT,resDouble,sparse[0]);
      left_bit_shift_wide_n(2*NUM_DIGITS_GF2X_ELEMENT,aux,sparse[0]);

      for (unsigned int i = 1; i < nPos; i++) {
         if (sparse[i] != INVALID_POS_VALUE) {
            left_bit_shift_wide_n(2*NUM_DIGITS_GF2X_ELEMENT,aux, (sparse[i]-sparse[i-1]) );
            gf2x_add(2*NUM_DIGITS_GF2X_ELEMENT,resDouble,
                     2*NUM_DIGITS_GF2X_ELEMENT,aux,
                     2*NUM_DIGITS_GF2X_ELEMENT,resDouble);
         }
      }
   }

   gf2x_mod(Res, 2*NUM_DIGITS_GF2X_ELEMENT, resDouble);

} // end gf2x_mod_mul

/*----------------------------------------------------------------------------*/


void gf2x_transpose_in_place_sparse(int sizeA, POSITION_T A[])
{

   POSITION_T t;
   int i = 0, j;

   if (A[i] == 0) {
      i = 1;
   }
   j = i;

   for (; i < sizeA && A[i] != INVALID_POS_VALUE; i++) {
      A[i] = P-A[i];
   }

   for (i -= 1; j < i; j++, i--) {
      t = A[j];
      A[j] = A[i];
      A[i] = t;
   }

} // end gf2x_transpose_in_place_sparse

/*----------------------------------------------------------------------------*/

void gf2x_mod_mul_sparse(int
                         sizeR, /*number of ones in the result, max sizeA*sizeB */
                         POSITION_T Res[],
                         int sizeA, /*number of ones in A*/
                         const POSITION_T A[],
                         int sizeB, /*number of ones in B*/
                         const POSITION_T B[])
{
   for(int i = 0; i< sizeR; i++) {
      Res[i]= INVALID_POS_VALUE;
   }
   /* compute all the coefficients, filling invalid positions with P*/
   unsigned i = 0;
   for(; i < sizeA  && A[i] != INVALID_POS_VALUE; i++) {
      unsigned j = 0;
      for (; j < sizeB && B[j] != INVALID_POS_VALUE; j++) {
         uint32_t prod = ((uint32_t) A[i]) + ((uint32_t) B[j]);
         Res[i*sizeB+j] = prod >= P ? prod - P : prod;
      }
      for (; j < sizeB ; j++) {
         Res[i*sizeB+j] = INVALID_POS_VALUE;
      }
   }

   for(; i < sizeA; i++) {
      for (unsigned j = 0; j < sizeB; j++) {
         Res[i*sizeB+j] = INVALID_POS_VALUE;
      }
   }
   quicksort(Res, sizeR);
   /* eliminate duplicates */
   int write_idx = 0;
   for(unsigned i = 0; i < sizeR-1  && Res[i] != INVALID_POS_VALUE; i++) {
      if (Res[i] == Res[i+1]) {
         i++;
      } else {
         Res[write_idx] = Res[i];
         write_idx++;
      }
   }
   /* fill remaining cells with INVALID_POS_VALUE */
   for(; write_idx < sizeR; write_idx++) {
      Res[write_idx] = INVALID_POS_VALUE;
   }

} // end gf2x_mod_mul_sparse


/*----------------------------------------------------------------------------*/
/* the implementation is safe even in case A or B alias with the result */
void gf2x_mod_add_sparse(int sizeR,
                         POSITION_T Res[],
                         int sizeA,
                         POSITION_T A[],
                         int sizeB,
                         POSITION_T B[])
{

   POSITION_T tmpRes[sizeR];
   int idxA = 0, idxB = 0, idxR = 0;

   while ( idxA < sizeA &&  idxB < sizeB &&  A[idxA] != INVALID_POS_VALUE &&
           B[idxB] != INVALID_POS_VALUE ) {

      if (A[idxA] == B[idxB]) {
         idxA++;
         idxB++;
      } else {
         if (A[idxA] < B[idxB]) {
            tmpRes[idxR] = A[idxA];
            idxA++;
         } else {
            tmpRes[idxR] = B[idxB];
            idxB++;
         }
         idxR++;
      }
   }

   while (idxA < sizeA && A[idxA] != INVALID_POS_VALUE) {
      tmpRes[idxR] = A[idxA];
      idxA++;
      idxR++;
   }

   while (idxB < sizeB && B[idxB] != INVALID_POS_VALUE) {
      tmpRes[idxR] = B[idxB];
      idxB++;
      idxR++;
   }

   while (idxR < sizeR) {
      tmpRes[idxR] = INVALID_POS_VALUE;
      idxR++;
   }
   memcpy(Res,tmpRes,sizeof(POSITION_T)*sizeR);

} // end gf2x_mod_add_sparse

/*----------------------------------------------------------------------------*/

/* Return a uniform random value in the range 0..n-1 inclusive,
 * applying a rejection sampling strategy and exploiting as a random source
 * the NIST seedexpander seeded with the proper key.
 * Assumes that the maximum value for the range n is 2^32-1
 */
static
int rand_range(const int n, const int logn, AES_XOF_struct *seed_expander_ctx)
{

   unsigned long required_rnd_bytes = (logn+7)/8;
   unsigned char rnd_char_buffer[4];
   uint32_t rnd_value;
   uint32_t mask = ( (uint32_t)1 << logn) - 1;

   do {
      seedexpander(seed_expander_ctx, rnd_char_buffer, required_rnd_bytes);
      /* obtain an endianness independent representation of the generated random
       bytes into an unsigned integer */
      rnd_value =  ((uint32_t)rnd_char_buffer[3] << 24) +
                   ((uint32_t)rnd_char_buffer[2] << 16) +
                   ((uint32_t)rnd_char_buffer[1] <<  8) +
                   ((uint32_t)rnd_char_buffer[0] <<  0) ;
      rnd_value = mask & rnd_value;
   } while (rnd_value >= n);

   return rnd_value;
} // end rand_range



/*----------------------------------------------------------------------------*/
/* Obtains fresh randomness and seed-expands it until all the required positions
 * for the '1's in the circulant block are obtained */

void rand_circulant_sparse_block(POSITION_T *pos_ones,
                                 const int countOnes,
                                 AES_XOF_struct *seed_expander_ctx)
{

   int duplicated, placedOnes = 0;

   while (placedOnes < countOnes) {
      int p = rand_range(NUM_BITS_GF2X_ELEMENT,
                         BITS_TO_REPRESENT(P),
                         seed_expander_ctx);
      duplicated = 0;
      for (int j = 0; j < placedOnes; j++) if (pos_ones[j] == p) duplicated = 1;
      if (duplicated == 0) {
         pos_ones[placedOnes] = p;
         placedOnes++;
      }
   }
} // rand_circulant_sparse_block

/*----------------------------------------------------------------------------*/


void rand_circulant_blocks_sequence(DIGIT sequence[N0*NUM_DIGITS_GF2X_ELEMENT],
                                    const int countOnes,
                                    AES_XOF_struct *seed_expander_ctx)
{

   int rndPos[countOnes],  duplicated, counter = 0;
   memset(sequence, 0x00, N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);


   while (counter < countOnes) {
      int p = rand_range(N0*NUM_BITS_GF2X_ELEMENT,BITS_TO_REPRESENT(P),
                         seed_expander_ctx);
      duplicated = 0;
      for (int j = 0; j < counter; j++) if (rndPos[j] == p) duplicated = 1;
      if (duplicated == 0) {
         rndPos[counter] = p;
         counter++;
      }
   }
   for (int j = 0; j < counter; j++) {
      int polyIndex = rndPos[j] / P;
      int exponent = rndPos[j] % P;
      gf2x_set_coeff( sequence + NUM_DIGITS_GF2X_ELEMENT*polyIndex, exponent,
                      ( (DIGIT) 1));
   }

} // end rand_circulant_blocks_sequence

/*----------------------------------------------------------------------------*/
