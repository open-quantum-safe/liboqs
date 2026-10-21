#ifndef SDITH_LIBOQS_PARAMS_H
#define SDITH_LIBOQS_PARAMS_H

/* SDitH3-L3-gf2-short-cipherpow, avx2 implementation: the settings api.c is built with. */

#ifndef SDITH3_FOR_LIBOQS
#define SDITH3_FOR_LIBOQS
#endif
#ifndef NDEBUG
#define NDEBUG
#endif

#define APPLY_NAMESPACE oqs_sig_private_sdith_avx2_
#define APPLY_PUBLIC_API_NAMESPACE oqs_sig_pubapi_sdith3_l3_gf2_short_cipherpow_avx2_
#define CRYPTO_SECRETKEYBYTES 208
#define CRYPTO_PUBLICKEYBYTES 98
#define CRYPTO_BYTES 8484
#define SIGNATURE_PARAMS CAT3_SHORT_CIPHERPOW_PARAMETERS

#endif  // SDITH_LIBOQS_PARAMS_H
