#include "bf_decoding.h"
#include "gf2x_arith_mod_xPplusOne.h"

#include <string.h>

int PQCLEAN_LEDAKEMLT32_LEAKTIME_bf_decoding(DIGIT err[],
        const POSITION_T HtrPosOnes[N0][DV],
        const POSITION_T QtrPosOnes[N0][M],
        DIGIT privateSyndrome[],
        uint8_t threshold) {

    uint8_t unsatParityChecks[N0 * P];
    POSITION_T currQBlkPos[M], currQBitPos[M];
    DIGIT currSyndrome[NUM_DIGITS_GF2X_ELEMENT];
    int check;
    int iteration = 0;
    unsigned int corrt_syndrome_based;

    do {
        PQCLEAN_LEDAKEMLT32_LEAKTIME_gf2x_copy(currSyndrome, privateSyndrome);
        memset(unsatParityChecks, 0x00, N0 * P * sizeof(uint8_t));
        for (int i = 0; i < N0; i++) {
            for (int valueIdx = 0; valueIdx < P; valueIdx++) {
                for (int HtrOneIdx = 0; HtrOneIdx < DV; HtrOneIdx++) {
                    POSITION_T tmp = (HtrPosOnes[i][HtrOneIdx] + valueIdx) >= P ? (HtrPosOnes[i][HtrOneIdx] + valueIdx) - P : (HtrPosOnes[i][HtrOneIdx] + valueIdx);
                    if (PQCLEAN_LEDAKEMLT32_LEAKTIME_gf2x_get_coeff(currSyndrome, tmp)) {
                        unsatParityChecks[i * P + valueIdx]++;
                    }
                }
            }
        }

        /* iteration based threshold determination*/
        corrt_syndrome_based = iteration ? (unsigned int) threshold : B0;

        //Computation of correlation  with a full Q matrix
        for (int i = 0; i < N0; i++) {
            for (int j = 0; j < P; j++) {
                int currQoneIdx = 0; // position in the column of QtrPosOnes[][...]
                int endQblockIdx = 0;
                unsigned int correlation = 0;

                for (int blockIdx = 0; blockIdx < N0; blockIdx++) {
                    endQblockIdx += qBlockWeights[blockIdx][i];
                    int currblockoffset = blockIdx * P;
                    for (; currQoneIdx < endQblockIdx; currQoneIdx++) {
                        POSITION_T tmp = QtrPosOnes[i][currQoneIdx] + j;
                        tmp = tmp >= P ? tmp - P : tmp;
                        currQBitPos[currQoneIdx] = tmp;
                        currQBlkPos[currQoneIdx] = blockIdx;
                        correlation += unsatParityChecks[tmp + currblockoffset];
                    }
                }
                /* Correlation based flipping */
                if (correlation >= corrt_syndrome_based) {
                    PQCLEAN_LEDAKEMLT32_LEAKTIME_gf2x_toggle_coeff(err + NUM_DIGITS_GF2X_ELEMENT * i, j);
                    for (int v = 0; v < M; v++) {
                        POSITION_T syndromePosToFlip;
                        for (int HtrOneIdx = 0; HtrOneIdx < DV; HtrOneIdx++) {
                            syndromePosToFlip = (HtrPosOnes[currQBlkPos[v]][HtrOneIdx] + currQBitPos[v] );
                            syndromePosToFlip = syndromePosToFlip >= P ? syndromePosToFlip - P : syndromePosToFlip;
                            PQCLEAN_LEDAKEMLT32_LEAKTIME_gf2x_toggle_coeff(privateSyndrome, syndromePosToFlip);
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
