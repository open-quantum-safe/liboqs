#include "H_Q_matrices_generation.h"
#include "gf2x_arith_mod_xPplusOne.h"

void PQCLEAN_LEDAKEMLT52_LEAKTIME_generateHPosOnes_HtrPosOnes(
    POSITION_T HPosOnes[N0][DV],
    POSITION_T HtrPosOnes[N0][DV],
    AES_XOF_struct *keys_expander) {
    for (int i = 0; i < N0; i++) {
        /* Generate a random block of Htr */
        PQCLEAN_LEDAKEMLT52_LEAKTIME_rand_circulant_sparse_block(&HtrPosOnes[i][0], DV, keys_expander);
    }
    for (int i = 0; i < N0; i++) {
        /* Obtain directly the sparse representation of the block of H */
        for (int k = 0; k < DV; k++) {
            HPosOnes[i][k] = (P - HtrPosOnes[i][k])  % P; /* transposes indexes */
        }
    }
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_generateQsparse(
    POSITION_T pos_ones[N0][M],
    AES_XOF_struct *keys_expander) {
    for (int i = 0; i < N0; i++) {
        int placed_ones = 0;
        for (int j = 0; j < N0; j++) {
            PQCLEAN_LEDAKEMLT52_LEAKTIME_rand_circulant_sparse_block(&pos_ones[i][placed_ones],
                    qBlockWeights[i][j],
                    keys_expander);
            placed_ones += qBlockWeights[i][j];
        }
    }
}
