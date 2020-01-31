#include "H_Q_matrices_generation.h"
#include "gf2x_arith_mod_xPplusOne.h"

void PQCLEAN_LEDAKEMLT52_LEAKTIME_generateHPosOnes(POSITION_T HPosOnes[N0][DV], AES_XOF_struct *keys_expander) {
    for (size_t i = 0; i < N0; i++) {
        /* Generate a random block of Htr */
        PQCLEAN_LEDAKEMLT52_LEAKTIME_rand_circulant_sparse_block(&HPosOnes[i][0], DV, keys_expander);
    }
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_generateQPosOnes(POSITION_T QPosOnes[N0][M], AES_XOF_struct *keys_expander) {
    size_t placed_ones;

    for (size_t i = 0; i < N0; i++) {
        placed_ones = 0;
        for (size_t j = 0; j < N0; j++) {
            PQCLEAN_LEDAKEMLT52_LEAKTIME_rand_circulant_sparse_block(&QPosOnes[i][placed_ones],
                    qBlockWeights[i][j],
                    keys_expander);
            placed_ones += qBlockWeights[i][j];
        }
    }
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_transposeHPosOnes(POSITION_T HtrPosOnes[N0][DV], POSITION_T HPosOnes[N0][DV]) {
    for (size_t i = 0; i < N0; i++) {
        /* Obtain directly the sparse representation of the block of H */
        for (size_t k = 0; k < DV; k++) {
            HtrPosOnes[i][k] = (P - HPosOnes[i][k])  % P; /* transposes indexes */
        }
    }
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_transposeQPosOnes(POSITION_T QtrPosOnes[N0][M], POSITION_T QPosOnes[N0][M]) {
    POSITION_T transposed_ones_idx[N0] = {0x00};
    size_t currQoneIdx, endQblockIdx;

    for (size_t source_row_idx = 0; source_row_idx < N0 ; source_row_idx++) {
        currQoneIdx = 0; // position in the column of QtrPosOnes[][...]
        endQblockIdx = 0;
        for (int blockIdx = 0; blockIdx < N0; blockIdx++) {
            endQblockIdx += qBlockWeights[source_row_idx][blockIdx];
            for (; currQoneIdx < endQblockIdx; currQoneIdx++) {
                QtrPosOnes[blockIdx][transposed_ones_idx[blockIdx]] =
                    (P - QPosOnes[source_row_idx][currQoneIdx]) % P;
                transposed_ones_idx[blockIdx]++;
            }
        }
    }
}


