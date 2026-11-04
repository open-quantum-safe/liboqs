#ifndef SDITH_LIBOQS_PARAMS_H
#define SDITH_LIBOQS_PARAMS_H

/* SDitH3-L1-gf2-fast-cipherpow, avx2 implementation: the settings api.c is built with. */

#ifndef SDITH3_FOR_LIBOQS
#define SDITH3_FOR_LIBOQS
#endif
#ifndef NDEBUG
#define NDEBUG
#endif

#define APPLY_NAMESPACE oqs_sig_private_sdith_avx2_
#define APPLY_PUBLIC_API_NAMESPACE oqs_sig_pubapi_sdith_l1_gf2_fast_cipherpow_avx2_
#define CRYPTO_SECRETKEYBYTES 147
#define CRYPTO_PUBLICKEYBYTES 70
#define CRYPTO_BYTES 4643
#define SIGNATURE_PARAMS CAT1_FAST_CIPHERPOW_PARAMETERS

#endif  // SDITH_LIBOQS_PARAMS_H
