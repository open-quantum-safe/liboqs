
#if LIMASZ==2048
  // Lima_2p_2048
  #define CRYPTO_SECRETKEYBYTES 18433
  #define CRYPTO_PUBLICKEYBYTES 12289
  #define CRYPTO_BYTES 32
  #define CRYPTO_CIPHERTEXTBYTES 6915
#elif LIMASZ==1018
  // Lima_sp__1018
  #define CRYPTO_SECRETKEYBYTES 9163
  #define CRYPTO_PUBLICKEYBYTES 6109
  #define CRYPTO_BYTES 32
  #define CRYPTO_CIPHERTEXTBYTES 3825
#elif LIMASZ==1306
  // Lima_sp_1306
  #define CRYPTO_SECRETKEYBYTES 15673
  #define CRYPTO_PUBLICKEYBYTES 10449
  #define CRYPTO_BYTES 32
  #define CRYPTO_CIPHERTEXTBYTES 6251
#elif LIMASZ==1822 
  // Lima_sp_1822
  #define CRYPTO_SECRETKEYBYTES 21865
  #define CRYPTO_PUBLICKEYBYTES 14577
  #define CRYPTO_BYTES 32
  #define CRYPTO_CIPHERTEXTBYTES 8315
#elif LIMASZ==2062
  // Lima_sp_2062
  #define CRYPTO_SECRETKEYBYTES 24745
  #define CRYPTO_PUBLICKEYBYTES 16497
  #define CRYPTO_BYTES 32
  #define CRYPTO_CIPHERTEXTBYTES 9275
#else
  // Lima_2p_1024
  #define CRYPTO_SECRETKEYBYTES 9217
  #define CRYPTO_PUBLICKEYBYTES 6145
  #define CRYPTO_BYTES 32
  #define CRYPTO_CIPHERTEXTBYTES 3843
#endif

#define CRYPTO_ALGNAME "LIMA-EncapCPA"

int crypto_kem_keypair( unsigned char *pk, unsigned char *sk);

int crypto_kem_enc( unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int crypto_kem_dec( unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
