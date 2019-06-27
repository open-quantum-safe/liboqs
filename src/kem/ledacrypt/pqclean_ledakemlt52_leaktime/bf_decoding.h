#ifndef BF_DECODING_H
#define BF_DECODING_H

#include "gf2x_arith.h"
#include "qc_ldpc_parameters.h"

/*  Definitions for DFR level 2^-SL with SL=128 */
#define ITERATIONS_MAX  (2)
#define B0              (88)
#define T_BAR           (6)

int PQCLEAN_LEDAKEMLT52_LEAKTIME_bf_decoding(DIGIT err[],
        const POSITION_T HtrPosOnes[N0][DV],
        const POSITION_T QtrPosOnes[N0][M],
        DIGIT privateSyndrome[],
        uint8_t threshold);

#endif
