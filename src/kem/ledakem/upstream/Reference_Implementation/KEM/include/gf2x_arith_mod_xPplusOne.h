/**
 *
 * <gf2x_arith_mod_xPplusOne.h>
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

#include "gf2x_limbs.h"
#include "qc_ldpc_parameters.h"

#include "gf2x_arith.h"
#include "rng.h"

/*----------------------------------------------------------------------------*/

#define                NUM_BITS_GF2X_ELEMENT (P)
#define              NUM_DIGITS_GF2X_ELEMENT ((P+DIGIT_SIZE_b-1)/DIGIT_SIZE_b)
#define                NUM_BITS_GF2X_MODULUS (P+1)
#define              NUM_DIGITS_GF2X_MODULUS ((P+1+DIGIT_SIZE_b-1)/DIGIT_SIZE_b)
#define MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS (P-DIGIT_SIZE_b*(NUM_DIGITS_GF2X_MODULUS-1))

#define                    INVALID_POS_VALUE (P)

#define IS_REPRESENTABLE_IN_D_BITS(D, N)                \
  (((unsigned long) N >= (1UL << (D - 1)) && (unsigned long) N < (1UL << D)) ? D : -1)

#define BITS_TO_REPRESENT(N)                            \
  (N == 0 ? 1 : (31                                     \
                 + IS_REPRESENTABLE_IN_D_BITS( 1, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 2, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 3, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 4, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 5, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 6, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 7, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 8, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 9, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(10, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(11, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(12, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(13, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(14, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(15, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(16, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(17, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(18, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(19, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(20, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(21, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(22, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(23, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(24, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(25, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(26, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(27, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(28, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(29, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(30, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(31, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(32, N)    \
                 )                                      \
   )

/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/

static inline void gf2x_copy(DIGIT dest[], const DIGIT in[])
{
   for (int i = NUM_DIGITS_GF2X_ELEMENT-1; i >= 0; i--)
      dest[i] = in[i];
} // end gf2x_copy

/*---------------------------------------------------------------------------*/

void gf2x_mod(DIGIT out[],
              const int nin, const DIGIT in[]); /* out(x) = in(x) mod x^P+1  */

/*---------------------------------------------------------------------------*/

void gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[]);

/*---------------------------------------------------------------------------*/

static inline void gf2x_mod_add(DIGIT Res[], const DIGIT A[], const DIGIT B[])
{
   gf2x_add(NUM_DIGITS_GF2X_ELEMENT, Res,
            NUM_DIGITS_GF2X_ELEMENT, A,
            NUM_DIGITS_GF2X_ELEMENT, B);
} // end gf2x_mod_add

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
int gf2x_mod_inverse(DIGIT out[], const DIGIT in[]);/* ret. 1 if inv. exists */

/*---------------------------------------------------------------------------*/

void gf2x_transpose_in_place(DIGIT
                             A[]); /* in place bit-transp. of a(x) % x^P+1  *
                                      * e.g.: a3 a2 a1 a0 --> a1 a2 a3 a0     */

/*---------------------------------------------------------------------------*/

static inline void gf2x_bitwise_and(DIGIT *const restrict OUT,
                                    const DIGIT *const restrict A,
                                    const DIGIT *const restrict B)
{
   for(int i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0; i--) {
      OUT[i] = A[i] & B[i];
   }
} // end gf2x_bitwise_and

/*---------------------------------------------------------------------------*/

/* population count for a single polynomial */
static inline int population_count(DIGIT upc[])
{
   int ret = 0;
   for(int i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0; i--) {
#if defined(DIGIT_IS_ULLONG)
      ret += __builtin_popcountll((unsigned long long int) (upc[i]));
#elif defined(DIGIT_IS_ULONG)
      ret += __builtin_popcountl((unsigned long int) (upc[i]));
#elif defined(DIGIT_IS_UINT)
      ret += __builtin_popcount((unsigned int) (upc[i]));
#elif defined(DIGIT_IS_UCHAR)
      const unsigned char split_lookup[] = {
         0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
      };
      ret += split_lookup[upc[i]&0xF] + split_lookup[upc[i]>>4];
#else
#error "Missing implementation for population_count(...) \
with this CPU word bitsize !!! "
#endif    
   }
   return ret;
} // end population_count

/*--------------------------------------------------------------------------*/

/* returns the coefficient of the x^exponent term as the LSB of a digit */
static inline
DIGIT gf2x_get_coeff(const DIGIT poly[], const unsigned int exponent)
{
   unsigned int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent;
   unsigned int digitIdx = straightIdx / DIGIT_SIZE_b;
   unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;
   return (poly[digitIdx] >> (DIGIT_SIZE_b-1-inDigitIdx)) & ((DIGIT) 1) ;
}

/*--------------------------------------------------------------------------*/

/* sets the coefficient of the x^exponent term as the LSB of a digit */
static inline
void gf2x_set_coeff(DIGIT poly[], const unsigned int exponent, DIGIT value)
{

   int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent;

   int digitIdx = straightIdx / DIGIT_SIZE_b;

   unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;

   /* clear given coefficient */
   DIGIT mask = ~( ((DIGIT) 1) << (DIGIT_SIZE_b-1-inDigitIdx));
   poly[digitIdx] = poly[digitIdx] & mask;
   poly[digitIdx] = poly[digitIdx] | (( value & ((DIGIT) 1)) <<
                                      (DIGIT_SIZE_b-1-inDigitIdx));
}
/*--------------------------------------------------------------------------*/

/* toggles (flips) the coefficient of the x^exponent term as the LSB of a digit */
static inline
void gf2x_toggle_coeff(DIGIT poly[], const unsigned int exponent)
{

   int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent;
   int digitIdx = straightIdx / DIGIT_SIZE_b;
   unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;

   /* clear given coefficient */
   DIGIT mask = ( ((DIGIT) 1) << (DIGIT_SIZE_b-1-inDigitIdx));
   poly[digitIdx] = poly[digitIdx] ^ mask;
}
/*--------------------------------------------------------------------------*/

void rand_circulant_sparse_block(POSITION_T *pos_ones,
                                 const int countOnes,
                                 AES_XOF_struct *seed_expander_ctx);
/*--------------------------------------------------------------------------*/

void rand_circulant_blocks_sequence(DIGIT sequence[N0*NUM_DIGITS_GF2X_ELEMENT],
                                    const int countOnes,
                                    AES_XOF_struct *seed_expander_ctx
                                   );

/*---------------------------------------------------------------------------*/


void gf2x_mod_add_sparse(int sizeR,
                         POSITION_T Res[],
                         int sizeA,
                         POSITION_T A[],
                         int sizeB,
                         POSITION_T B[]);

/*----------------------------------------------------------------------------*/

void gf2x_transpose_in_place_sparse(int sizeA, POSITION_T A[]);

/*----------------------------------------------------------------------------*/

void gf2x_mod_mul_sparse(int
                         sizeR, /*number of ones in the result, max sizeA*sizeB */
                         POSITION_T Res[],
                         int sizeA, /*number of ones in A*/
                         const POSITION_T A[],
                         int sizeB, /*number of ones in B*/
                         const POSITION_T B[]);
/*----------------------------------------------------------------------------*/
/* PRE: amount is lesser than a digit wide */
void right_bit_shift_n(const int length, DIGIT in[], int amount);
/*----------------------------------------------------------------------------*/
/* PRE: amount is lesser than a digit wide */
void left_bit_shift_n(const int length, DIGIT in[], int amount);
/*----------------------------------------------------------------------------*/
void left_bit_shift_wide_n(const int length, DIGIT in[], int amount);
/*----------------------------------------------------------------------------*/
void gf2x_mod_mul_dense_to_sparse(DIGIT Res[],
                                  const DIGIT dense[],
                                  POSITION_T sparse[],
                                  unsigned int nPos);
/*----------------------------------------------------------------------------*/
static inline
int partition (POSITION_T arr[], int lo, int hi)
{
   POSITION_T x = arr[hi];
   POSITION_T tmp;
   int i = (lo - 1);
   for (int j = lo; j <= hi - 1; j++)  {
      if (arr[j] <= x) {
         i++;
         tmp = arr[i];
         arr[i] = arr[j];
         arr[j] = tmp;
      }
   }
   tmp = arr[i+1];
   arr[i+1] = arr[hi];
   arr[hi] = tmp;

   return i+1;
} // end partition
/*----------------------------------------------------------------------------*/
static inline
void quicksort(POSITION_T Res[], unsigned int sizeR)
{
   /* sort the result */
   int stack[sizeR];
   int hi, lo, pivot, tos = -1;
   stack[++tos] = 0;
   stack[++tos] = sizeR-1;
   while (tos >=0 ) {
      hi = stack[tos--];
      lo = stack[tos--];
      pivot = partition(Res, lo, hi);
      if ( (pivot-1) > lo) {
         stack[++tos] = lo;
         stack[++tos] = pivot-1;
      }
      if ( (pivot + 1) < hi) {
         stack[++tos] = pivot+1;
         stack[++tos] = hi;
      }
   }
}
