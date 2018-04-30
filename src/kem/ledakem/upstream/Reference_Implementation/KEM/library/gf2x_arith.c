/**
 *
 * <gf2x_arith.c>
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


#include "gf2x_arith.h"
#include <string.h>  // memset(...)

/*----------------------------------------------------------------------------*/

void gf2x_mul_comb(const int nr, DIGIT Res[],
                   const int na, const DIGIT A[],
                   const int nb, const DIGIT B[])
{
   int i, j, k;
   DIGIT u, h;

   memset(Res, 0x00, nr*sizeof(DIGIT));

   for (k = DIGIT_SIZE_b-1; k > 0; k--) {
      for (i = na-1; i >= 0; i--)
         if ( A[i] & (((DIGIT)0x1) << k) )
            for (j = nb-1; j >= 0; j--) Res[i+j+1] ^= B[j];


      u = Res[na+nb-1];
      Res[na+nb-1] = u << 0x1;
      for (j = 1; j < na+nb; ++j) {
         h = u >> (DIGIT_SIZE_b-1);
         u = Res[na+nb-1-j];
         Res[na+nb-1-j] = h^(u << 0x1);
      }
   }
   for (i = na-1; i >= 0; i--)
      if ( A[i] & ((DIGIT)0x1) )
         for (j = nb-1; j >= 0; j--) Res[i+j+1] ^= B[j];
}

/*----------------------------------------------------------------------------*/

void gf2x_add(const int nr, DIGIT Res[],
              const int na, const DIGIT A[],
              const int nb, const DIGIT B[])
{
   for (unsigned i = 0; i < nr; i++)
      Res[i] = A[i] ^ B[i];
} // end gf2x_add

int gf2x_cmp(const unsigned lenA, const DIGIT A[],
             const unsigned lenB, const DIGIT B[])
{

   int i;
   unsigned lA = lenA, lB = lenB;
   for (i = 0; i < lenA && A[i] == 0; i++) lA--;
   for (i = 0; i < lenB && B[i] == 0; i++) lB--;
   if (lA < lB) return -1;
   if (lA > lB) return +1;
   for (i = 0; i < lA; i++) {
      if (A[i] > B[i]) return +1;
      if (A[i] < B[i]) return -1;
   }
   return 0;

} // end gf2x_cmp
