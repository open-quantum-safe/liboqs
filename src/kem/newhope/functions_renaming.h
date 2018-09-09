#ifndef __FUNCTIONS_RENAMING_H_INCLUDED__
#define __FUNCTIONS_RENAMING_H_INCLUDED__

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define RENAME_FUNC_NAME(fname) EVALUATOR(FUNC_PREFIX, fname)


#define crypto_kem_keypair RENAME_FUNC_NAME(keypair)
#define crypto_kem_dec RENAME_FUNC_NAME(decaps)
#define crypto_kem_enc RENAME_FUNC_NAME(encaps)

#endif
