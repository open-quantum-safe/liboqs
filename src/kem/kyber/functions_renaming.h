#ifndef __FUNCTIONS_RENAMING_H_INCLUDED__
#define __FUNCTIONS_RENAMING_H_INCLUDED__

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define RENAME_FUNC_NAME(fname) EVALUATOR(FUNC_PREFIX, fname)

#define crypto_kem_keypair RENAME_FUNC_NAME(keypair)
#define crypto_kem_enc RENAME_FUNC_NAME(encaps)
#define crypto_kem_dec RENAME_FUNC_NAME(decaps)

#define cbd RENAME_FUNC_NAME(cbd)

#define indcpa_keypair RENAME_FUNC_NAME(indcpa_keypair)
#define indcpa_enc RENAME_FUNC_NAME(indcpa_enc)
#define indcpa_dec RENAME_FUNC_NAME(indcpa_dec)

#define ntt RENAME_FUNC_NAME(ntt)
#define invntt RENAME_FUNC_NAME(invntt)

#define poly_compress RENAME_FUNC_NAME(poly_compress)
#define poly_decompress RENAME_FUNC_NAME(poly_decompress)
#define poly_tobytes RENAME_FUNC_NAME(poly_tobytes)
#define poly_frombytes RENAME_FUNC_NAME(poly_frombytes)
#define poly_frommsg RENAME_FUNC_NAME(poly_frommsg)
#define poly_tomsg RENAME_FUNC_NAME(poly_tomsg)
#define poly_getnoise RENAME_FUNC_NAME(poly_getnoise)
#define poly_ntt RENAME_FUNC_NAME(poly_ntt)
#define poly_invntt RENAME_FUNC_NAME(poly_invntt)
#define poly_add RENAME_FUNC_NAME(poly_add)
#define poly_sub RENAME_FUNC_NAME(poly_sub)

#define polyvec_compress RENAME_FUNC_NAME(polyvec_compress)
#define polyvec_decompress RENAME_FUNC_NAME(polyvec_decompress)
#define polyvec_tobytes RENAME_FUNC_NAME(polyvec_tobytes)
#define polyvec_frombytes RENAME_FUNC_NAME(polyvec_frombytes)
#define polyvec_ntt RENAME_FUNC_NAME(polyvec_ntt)
#define polyvec_invntt RENAME_FUNC_NAME(polyvec_invntt)
#define polyvec_pointwise_acc RENAME_FUNC_NAME(polyvec_pointwise_acc)
#define polyvec_add RENAME_FUNC_NAME(polyvec_add)

#define freeze RENAME_FUNC_NAME(freeze)
#define montgomery_reduce RENAME_FUNC_NAME(montgomery_reduce)
#define barrett_reduce RENAME_FUNC_NAME(barrett_reduce)

#define verify RENAME_FUNC_NAME(verify)
#define cmov RENAME_FUNC_NAME(cmov)

#define omegas_inv_bitrev_montgomery RENAME_FUNC_NAME(omegas_inv_bitrev_montgomery)
#define psis_inv_montgomery RENAME_FUNC_NAME(psis_inv_montgomery)
#define zetas RENAME_FUNC_NAME(zetas)

#endif
