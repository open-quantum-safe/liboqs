/**
 *
 * <niederreiter_decrypt.c>
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

#include "niederreiter_decrypt.h"

#include "qc_ldpc_parameters.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "H_Q_matrices_generation.h"

#include "bf_decoding.h"

#include <string.h>

/*----------------------------------------------------------------------------*/

int decrypt_niederreiter(DIGIT err[],            // N0 circ poly
                         const privateKeyNiederreiter_t *const sk,
                         const DIGIT syndrome[]  // 1 circ poly
                        )
{
   AES_XOF_struct niederreiter_decrypt_expander;
   seedexpander_from_trng(&niederreiter_decrypt_expander,
                          sk->prng_seed);

   /**************************************************************************/
   // sequence of N0 circ block matrices (p x p):
   DIGIT Ln0tr[NUM_DIGITS_GF2X_ELEMENT] = {0x00};
   POSITION_T HPosOnes[N0][DV];
   POSITION_T HtrPosOnes[N0][DV];
   POSITION_T QtrPosOnes[N0][M];
   POSITION_T QPosOnes[N0][M];
   generateHPosOnes_HtrPosOnes(HPosOnes, HtrPosOnes,
                               &niederreiter_decrypt_expander);
   generateQPosOnes_QtrPosOnes(QPosOnes, QtrPosOnes,
                               &niederreiter_decrypt_expander);

   POSITION_T auxSparse[DV*M];
   POSITION_T Ln0trSparse[DV*M];
   for(int i = 0; i< DV*M; i++) {
      Ln0trSparse[i] = INVALID_POS_VALUE;
      auxSparse[i]= INVALID_POS_VALUE;
   }

   for (int i = 0; i < N0; i++) {
      gf2x_mod_mul_sparse(DV*M, auxSparse,
                          DV,   HPosOnes[i],
                          qBlockWeights[i][N0-1], &QPosOnes[i][M-qBlockWeights[i][N0-1]]
                         );
      gf2x_mod_add_sparse(DV*M, Ln0trSparse,
                          DV*M, Ln0trSparse,
                          DV*M, auxSparse
                         );
   } // end for i
   gf2x_transpose_in_place_sparse(DV*M, Ln0trSparse);

   for (int i = 0; i < DV*M && Ln0trSparse[i] != INVALID_POS_VALUE; i++) {
      gf2x_set_coeff(Ln0tr, Ln0trSparse[i], (DIGIT) 0x1);
   }

   int decryptOk;
   DIGIT privateSyndrome[NUM_DIGITS_GF2X_ELEMENT];
   gf2x_mod_mul(privateSyndrome,
                Ln0tr,
                syndrome
               );

   memset(err, 0x00, N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   decryptOk = bf_decoding(err, (const POSITION_T (*)[DV]) HtrPosOnes,
                           (const POSITION_T (*)[M]) QtrPosOnes, privateSyndrome);
   return decryptOk;
} // end decrypt_niederreiter

/*----------------------------------------------------------------------------*/


