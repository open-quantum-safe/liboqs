set(OQS_KEM_DEFAULT "OQS_KEM_alg_frodokem_640_aes")

# AES
option(USE_AES_INSTRUCTIONS "Use x86 AES-NI instructions." OFF)
option(USE_AES_OPENSSL "Use OpenSSL's AES implementation." OFF)

# AVX
option(USE_AVX2_INSTRUCTIONS "Use x86 AVX2 instructions." OFF)

# SHA-2/3
option(USE_SHA2_OPENSSL "Use OpenSSL's SHA-2 implementation." OFF)
option(USE_SHA3_OPENSSL "Use OpenSSL's SHA-3 implementation." OFF)

set(USE_OPENSSL OFF)
if (USE_AES_OPENSSL OR
    USE_SHA2_OPENSSL OR
    USE_SHA3_OPENSSL)
    set(USE_OPENSSL ON)
endif ()

# FrodoKEM
option(OQS_ENABLE_KEM_frodokem_640_aes "Enable FrodoKEM 640-AES." ON)
option(OQS_ENABLE_KEM_frodokem_640_shake "Enable FrodoKEM 640-SHAKE." ON)

option(OQS_ENABLE_KEM_frodokem_976_aes "Enable FrodoKEM 976-AES." ON)
option(OQS_ENABLE_KEM_frodokem_976_shake "Enable FrodoKEM 976-SHAKE." ON)

option(OQS_ENABLE_KEM_frodokem_1344_aes "Enable FrodoKEM 1344-AES." ON)
option(OQS_ENABLE_KEM_frodokem_1344_shake "Enable FrodoKEM 1344-SHAKE." ON)
set(ENABLE_KEM_FRODOKEM OFF)
if (OQS_ENABLE_KEM_frodokem_640_aes OR
    OQS_ENABLE_KEM_frodokem_640_shake OR
    OQS_ENABLE_KEM_frodokem_976_aes OR
    OQS_ENABLE_KEM_frodokem_976_shake OR
    OQS_ENABLE_KEM_frodokem_1344_aes OR
    OQS_ENABLE_KEM_frodokem_1344_shake)
    set(ENABLE_KEM_FRODOKEM ON)
endif ()
