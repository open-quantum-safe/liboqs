#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdlib.h>

#include "params.h"
#include "poly.h"

void PQCLEAN_NTRUHRSS701_CLEAN_sample_fg(poly *f, poly *g, const unsigned char uniformbytes[NTRU_SAMPLE_FG_BYTES]);
void PQCLEAN_NTRUHRSS701_CLEAN_sample_rm(poly *r, poly *m, const unsigned char uniformbytes[NTRU_SAMPLE_RM_BYTES]);

void PQCLEAN_NTRUHRSS701_CLEAN_sample_iid(poly *r, const unsigned char uniformbytes[NTRU_SAMPLE_IID_BYTES]);

void PQCLEAN_NTRUHRSS701_CLEAN_sample_iid_plus(poly *r, const unsigned char uniformbytes[NTRU_SAMPLE_IID_BYTES]);

#endif
