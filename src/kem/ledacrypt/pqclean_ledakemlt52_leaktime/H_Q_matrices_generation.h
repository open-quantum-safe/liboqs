#ifndef H_Q_MATRICES_GENERATION_H
#define H_Q_MATRICES_GENERATION_H

#include "gf2x_arith.h"
#include "qc_ldpc_parameters.h"
#include "rng.h"

void PQCLEAN_LEDAKEMLT52_LEAKTIME_generateHPosOnes_HtrPosOnes(POSITION_T HPosOnes[N0][DV], POSITION_T HtrPosOnes[N0][DV], AES_XOF_struct *niederreiter_keys_expander);
void PQCLEAN_LEDAKEMLT52_LEAKTIME_generateQsparse(POSITION_T pos_ones[N0][M], AES_XOF_struct *niederreiter_keys_expander);

#endif
