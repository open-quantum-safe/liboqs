#include "bf_decoding.h"
#include "dfr_test.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "qc_ldpc_parameters.h"
#include "sort.h"

#include <string.h>


int PQCLEAN_LEDAKEMLT32_LEAKTIME_DFR_test(POSITION_T LSparse[N0][DV * M], uint8_t *secondIterThreshold) {

    POSITION_T LSparse_loc[N0][DV * M]; /* vector of N_0 sparse blocks */
    uint8_t gamma[N0][N0][P] = {{{0}}};
    uint32_t gammaHist[N0][DV * M + 1] = {{0}};
    size_t maxMut[N0], maxMutMinusOne[N0];
    size_t allBlockMaxSumst, allBlockMaxSumstMinusOne;
    size_t histIdx, toAdd;

    for (size_t i = 0; i < N0; i++) {
        for (size_t j = 0; j < DV * M; j++) {
            if (LSparse[i][j] != 0) {
                LSparse_loc[i][j] = (P - LSparse[i][j]);
            }
        }
        PQCLEAN_LEDAKEMLT32_LEAKTIME_uint32_sort(LSparse_loc[i], DV * M);
    }


    for (size_t i = 0; i < N0; i++ ) {
        for (size_t j = 0; j < N0; j++) {
            for (size_t k = 0; k < (DV * M); k++) {
                for (size_t l = 0; l < (DV * M); l++) {
                    gamma[i][j][(P + LSparse_loc[i][k] - LSparse_loc[j][l]) % P]++;
                }
            }
        }
    }

    for (size_t i = 0; i < N0; i++ ) {
        for (size_t j = 0; j < N0; j++ ) {
            gamma[i][j][0] = 0;
        }
    }

    /* build histogram of values in gamma */
    for (size_t i = 0; i < N0; i++ ) {
        for (size_t j = 0; j < N0; j++ ) {
            for (size_t k = 0; k < P; k++) {
                gammaHist[i][gamma[i][j][k]]++;
            }
        }
    }


    for (size_t gammaBlockRowIdx = 0; gammaBlockRowIdx < N0; gammaBlockRowIdx++) {
        maxMutMinusOne[gammaBlockRowIdx] = 0;
        histIdx = DV * M;
        toAdd = T_BAR - 1;
        while ( (histIdx > 0) && (toAdd > 0)) {
            if (gammaHist[gammaBlockRowIdx][histIdx] > toAdd ) {
                maxMutMinusOne[gammaBlockRowIdx] += histIdx * toAdd;
                toAdd = 0;
            } else {
                maxMutMinusOne[gammaBlockRowIdx] += histIdx * gammaHist[gammaBlockRowIdx][histIdx];
                toAdd -= gammaHist[gammaBlockRowIdx][histIdx];
                histIdx--;
            }
        }
        maxMut[gammaBlockRowIdx] = histIdx + maxMutMinusOne[gammaBlockRowIdx];
    }

    /*seek max values across all gamma blocks */
    allBlockMaxSumst = maxMut[0];
    allBlockMaxSumstMinusOne = maxMutMinusOne[0];
    for (size_t gammaBlockRowIdx = 0; gammaBlockRowIdx < N0; gammaBlockRowIdx++) {
        allBlockMaxSumst = allBlockMaxSumst < maxMut[gammaBlockRowIdx] ?
                           maxMut[gammaBlockRowIdx] :
                           allBlockMaxSumst;
        allBlockMaxSumstMinusOne = allBlockMaxSumstMinusOne < maxMutMinusOne[gammaBlockRowIdx] ?
                                   maxMutMinusOne[gammaBlockRowIdx] :
                                   allBlockMaxSumstMinusOne;
    }
    if (DV * M > (allBlockMaxSumstMinusOne + allBlockMaxSumst)) {
        *secondIterThreshold = (uint8_t) (allBlockMaxSumst + 1);
        return 1;
    }
    return 0;
}
