/**
 * @file cpa-pke_opt.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 */

#ifndef _CPA_PKE_OPT_H
#define _CPA_PKE_OPT_H

#include "type.h"

void lotus_cpa_pke_keypair(U16 *pk, U16 *sk);
void lotus_cpa_pke_enc_packed(U8 *ct_packed, const U8 *sigma, const U8 *pk_packed);
void lotus_cpa_pke_dec_packed(U8 *sigma, const U8 *ct_packed, const U8 *sk_packed);

#endif
