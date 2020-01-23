#include "bf_decoding.h"
#include "gf2x_arith_mod_xPplusOne.h"

#include <string.h>

int PQCLEAN_LEDAKEMLT52_LEAKTIME_bf_decoding(DIGIT err[],
        const POSITION_T HtrPosOnes[N0][DV],
        const POSITION_T QtrPosOnes[N0][M],
        DIGIT privateSyndrome[],
        uint8_t secondIterThreshold) {

    DIGIT currSyndrome[NUM_DIGITS_GF2X_ELEMENT];
    uint8_t unsatParityChecks[N0 * P];
    POSITION_T currQBlkPos[M], currQBitPos[M];
    POSITION_T syndromePosToFlip, tmp;
    uint32_t correlation, corrt_syndrome_based;
    size_t currQoneIdx, endQblockIdx, currblockoffset;
    int check;
    int iteration = 0;

    do {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_copy(currSyndrome, privateSyndrome);
        memset(unsatParityChecks, 0x00, N0 * P * sizeof(uint8_t));
        for (size_t i = 0; i < N0; i++) {
            for (POSITION_T valueIdx = 0; valueIdx < P; valueIdx++) {
                for (size_t HtrOneIdx = 0; HtrOneIdx < DV; HtrOneIdx++) {
                    tmp = (HtrPosOnes[i][HtrOneIdx] + valueIdx) >= P ?
                          (HtrPosOnes[i][HtrOneIdx] + valueIdx) - P :
                          (HtrPosOnes[i][HtrOneIdx] + valueIdx);
                    if (PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_get_coeff(currSyndrome, tmp)) {
                        unsatParityChecks[i * P + valueIdx]++;
                    }
                }
            }
        }

        /* iteration based threshold determination*/
        corrt_syndrome_based = iteration * secondIterThreshold + (1 - iteration) * B0;

        // Computation of correlation  with a full Q matrix
        for (size_t i = 0; i < N0; i++) {
            for (POSITION_T j = 0; j < P; j++) {
                currQoneIdx =  endQblockIdx = 0;
                correlation = 0;

                for (size_t blockIdx = 0; blockIdx < N0; blockIdx++) {
                    endQblockIdx += qBlockWeights[blockIdx][i];
                    currblockoffset = blockIdx * P;
                    for (; currQoneIdx < endQblockIdx; currQoneIdx++) {
                        tmp = QtrPosOnes[i][currQoneIdx] + j;
                        tmp = tmp >= P ? tmp - P : tmp;
                        currQBitPos[currQoneIdx] = tmp;
                        currQBlkPos[currQoneIdx] = (POSITION_T)blockIdx;
                        correlation += unsatParityChecks[tmp + currblockoffset];
                    }
                }

                /* Correlation based flipping */
                if (correlation >= corrt_syndrome_based) {
                    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_toggle_coeff(err + NUM_DIGITS_GF2X_ELEMENT * i, j);
                    for (size_t v = 0; v < M; v++) {
                        for (size_t HtrOneIdx = 0; HtrOneIdx < DV; HtrOneIdx++) {
                            syndromePosToFlip = (HtrPosOnes[currQBlkPos[v]][HtrOneIdx] + currQBitPos[v]);
                            syndromePosToFlip = syndromePosToFlip >= P ? syndromePosToFlip - P : syndromePosToFlip;
                            PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_toggle_coeff(privateSyndrome, syndromePosToFlip);
                        }
                    } // end for v
                } // end if
            } // end for j
        } // end for i

        iteration = iteration + 1;
        check = 0;
        while (check < NUM_DIGITS_GF2X_ELEMENT && privateSyndrome[check++] == 0) {};

    } while (iteration < ITERATIONS_MAX && check < NUM_DIGITS_GF2X_ELEMENT);

    return (check == NUM_DIGITS_GF2X_ELEMENT);
}
