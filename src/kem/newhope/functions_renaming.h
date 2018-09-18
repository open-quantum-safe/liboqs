#ifndef __FUNCTIONS_RENAMING_H_INCLUDED__
#define __FUNCTIONS_RENAMING_H_INCLUDED__

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define RENAME_FUNC_NAME(fname) EVALUATOR(FUNC_PREFIX, fname)

#define crypto_kem_keypair RENAME_FUNC_NAME(keypair)
#define crypto_kem_dec RENAME_FUNC_NAME(decaps)
#define crypto_kem_enc RENAME_FUNC_NAME(encaps)

#define cpapke_keypair RENAME_FUNC_NAME(cpapke_keypair)
#define cpapke_enc RENAME_FUNC_NAME(cpapke_enc)
#define cpapke_dec RENAME_FUNC_NAME(cpapke_dec)

#define bitrev_vector RENAME_FUNC_NAME(bitrev_vector)
#define mul_coefficients RENAME_FUNC_NAME(mul_coefficients)
#define ntt RENAME_FUNC_NAME(ntt)

#define poly_uniform RENAME_FUNC_NAME(poly_uniform)
#define poly_sample RENAME_FUNC_NAME(poly_sample)
#define poly_add RENAME_FUNC_NAME(poly_add)
#define poly_ntt RENAME_FUNC_NAME(poly_ntt)
#define poly_invntt RENAME_FUNC_NAME(poly_invntt)
#define poly_mul_pointwise RENAME_FUNC_NAME(poly_mul_pointwise)
#define poly_frombytes RENAME_FUNC_NAME(poly_frombytes)
#define poly_tobytes RENAME_FUNC_NAME(poly_tobytes)
#define poly_compress RENAME_FUNC_NAME(poly_compress)
#define poly_decompress RENAME_FUNC_NAME(poly_decompress)
#define poly_frommsg RENAME_FUNC_NAME(poly_frommsg)
#define poly_tomsg RENAME_FUNC_NAME(poly_tomsg)
#define poly_sub RENAME_FUNC_NAME(poly_sub)

#define montgomery_reduce RENAME_FUNC_NAME(montgomery_reduce)

#define verify RENAME_FUNC_NAME(verify)
#define cmov RENAME_FUNC_NAME(cmov)

#define omegas_bitrev_montgomery RENAME_FUNC_NAME(omegas_bitrev_montgomery)
#define omegas_inv_bitrev_montgomery RENAME_FUNC_NAME(omegas_inv_bitrev_montgomery)
#define psis_bitrev_montgomery RENAME_FUNC_NAME(psis_bitrev_montgomery)
#define psis_inv_montgomery RENAME_FUNC_NAME(psis_inv_montgomery)

#endif
