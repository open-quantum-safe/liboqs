#ifndef H_Q_MATRICES_GENERATION_H
#define H_Q_MATRICES_GENERATION_H

#include "gf2x_arith.h"
#include "qc_ldpc_parameters.h"
#include "rng.h"

void PQCLEAN_LEDAKEMLT32_LEAKTIME_generateHPosOnes(POSITION_T HPosOnes[N0][DV], AES_XOF_struct *keys_expander);
void PQCLEAN_LEDAKEMLT32_LEAKTIME_generateQPosOnes(POSITION_T QPosOnes[N0][M], AES_XOF_struct *keys_expander);
void PQCLEAN_LEDAKEMLT32_LEAKTIME_transposeHPosOnes(POSITION_T HtrPosOnes[N0][DV], POSITION_T HPosOnes[N0][DV]);
void PQCLEAN_LEDAKEMLT32_LEAKTIME_transposeQPosOnes(POSITION_T QtrPosOnes[N0][M], POSITION_T QPosOnes[N0][M]);

#endif
