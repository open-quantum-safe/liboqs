/**
 * @file pack.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 */

#ifndef _PACK_H
#define _PACK_H

#include "type.h"

#define _LOTUS_UNPACK_PUBKEY_BYTES ((_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT) * sizeof(U16))
#define _LOTUS_UNPACK_PRVKEY_BYTES (_LOTUS_LWE_DIM * _LOTUS_LWE_PT * sizeof(U16))
#define _LOTUS_UNPACK_CT_BYTES ((_LOTUS_LWE_DIM + _LOTUS_LWE_PT) * sizeof(U16))

#define _LOTUS_PACK_PUBKEY_BYTES ((_LOTUS_LWE_DIM * _LOTUS_LWE_DIM + _LOTUS_LWE_DIM * _LOTUS_LWE_PT) * _LOTUS_LWE_LOG2_MOD / 8)
#define _LOTUS_PACK_PRVKEY_BYTES (_LOTUS_LWE_DIM * _LOTUS_LWE_PT * 6 / 8)
#define _LOTUS_PACK_CT_BYTES ((_LOTUS_LWE_DIM + _LOTUS_LWE_PT) * _LOTUS_LWE_LOG2_MOD / 8)

void unpack_128elems(U16 *rop, const U8 *op);
void unpack_128dg(U16 *rop, const U8 *op);

void pack_pk(U8 *bin, const U16 *pk);
void unpack_pk(U16 *pk, const U8 *bin);
void pack_sk(U8 *bin, const U16 *sk);
void unpack_sk(U16 *sk, const U8 *bin);
void pack_ct(U8 *bin, const U16 *ct);
void unpack_ct(U16 *ct, const U8 *bin);

#endif
