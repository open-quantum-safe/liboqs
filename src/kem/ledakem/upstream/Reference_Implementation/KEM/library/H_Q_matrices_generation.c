/**
 *
 * <H_Q_matrices_generation.c>
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

#include "H_Q_matrices_generation.h"
#include "gf2x_arith_mod_xPplusOne.h"

/*----------------------------------------------------------------------------*/


void generateHPosOnes_HtrPosOnes(POSITION_T HPosOnes[N0][DV],
                                 POSITION_T HtrPosOnes[N0][DV],
                                 AES_XOF_struct *niederreiter_keys_expander)
{

   for (int i = 0; i < N0; i++) {
      /* Generate a random block of H */
      rand_circulant_sparse_block(&HPosOnes[i][0],
                                  DV,
                                  niederreiter_keys_expander);
      /* Obtain directly the sparse representation of the block of Htr */
      for (int k = 0; k < DV; k++) {

         HtrPosOnes[i][k] = (P - HPosOnes[i][k])  % P; /* transposes indexes */
      }// end for k
   }
} // end generateHPosOnes_HtrPosOnes


/*----------------------------------------------------------------------------*/
static
void  generateQsparse(POSITION_T pos_ones[N0][M],
                      AES_XOF_struct *niederreiter_keys_expander)
{
   for (int i = 0; i < N0; i++) {
      int placed_ones = 0;
      for (int j = 0; j < N0; j++) {
         rand_circulant_sparse_block(&pos_ones[i][placed_ones],
                                     qBlockWeights[i][j],
                                     niederreiter_keys_expander);
         placed_ones += qBlockWeights[i][j];
      } // end for j
   } // end for i
} // end generateQsparse

/*----------------------------------------------------------------------------*/


void generateQPosOnes_QtrPosOnes(POSITION_T QPosOnes[N0][M],
                                 POSITION_T QtrPosOnes[N0][M],
                                 AES_XOF_struct *niederreiter_keys_expander)
{
   generateQsparse(QPosOnes, niederreiter_keys_expander);
   unsigned transposed_ones_idx[N0] = {0x00};
   for(unsigned i=0; i < N0 ; i++) {
      int currQoneIdx = 0; // position in the column of QtrPosOnes[][...]
      int endQblockIdx = 0;
      for (int blockIdx = 0; blockIdx < N0; blockIdx++) {
         endQblockIdx += qBlockWeights[i][blockIdx];
         for (; currQoneIdx < endQblockIdx; currQoneIdx++) {
            QtrPosOnes[blockIdx][transposed_ones_idx[blockIdx]] = (P -
                  QPosOnes[i][currQoneIdx]) % P;
            transposed_ones_idx[blockIdx]++;
         }
      }
   }

} // end generateQPosOnes_QtrPosOnes

/*----------------------------------------------------------------------------*/
