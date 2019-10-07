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

option(OQS_ENABLE_KEM_kyber_512 "Enable Kyber 512" ON)
option(OQS_ENABLE_KEM_kyber_768 "Enable Kyber 768" ON)
option(OQS_ENABLE_KEM_kyber_1024 "Enable Kyber 1024" ON)
set(ENABLE_KEM_KYBER OFF)
if (OQS_ENABLE_KEM_kyber_512 OR
    OQS_ENABLE_KEM_kyber_768 OR
    OQS_ENABLE_KEM_kyber_1024)
    set(ENABLE_KEM_KYBER ON)
endif ()

option(OQS_ENABLE_KEM_newhope_512cca "Enable NewHope 512" ON)
option(OQS_ENABLE_KEM_newhope_1024cca "Enable NewHope 1024" ON)
set(ENABLE_KEM_NEWHOPE OFF)
if (OQS_ENABLE_KEM_newhope_512cca OR
    OQS_ENABLE_KEM_newhope_2014cca)
    set(ENABLE_KEM_NEWHOPE ON)
endif ()

option(OQS_ENABLE_KEM_ntru_hps2048509 "Enable NTRU-HPS2048509" ON)
option(OQS_ENABLE_KEM_ntru_hps2048677 "Enable NTRU-HPS2048677" ON)
option(OQS_ENABLE_KEM_ntru_hps4096821 "Enable NTRU-HPS4096821" ON)
option(OQS_ENABLE_KEM_ntru_hrss701 "Enable NTRU-HRSS701" ON)
set(ENABLE_KEM_NTRU OFF)
if (OQS_ENABLE_KEM_ntru_hps2048509 OR
    OQS_ENABLE_KEM_ntru_hps2048677 OR
    OQS_ENABLE_KEM_ntru_hps4096821 OR
    OQS_ENABLE_KEM_ntru_hrss701)
    set(ENABLE_KEM_NTRU ON)
endif ()

option(OQS_ENABLE_KEM_saber_lightsaber "Enable Saber-Lightsaber" ON)
option(OQS_ENABLE_KEM_saber_saber "Enable Saber-Saber" ON)
option(OQS_ENABLE_KEM_saber_firesaber "Enable Saber-Firesaber" ON)
set(ENABLE_KEM_SABER OFF)
if (OQS_ENABLE_KEM_saber_lightsaber OR
    OQS_ENABLE_KEM_saber_saber OR
    OQS_ENABLE_KEM_saber_firesaber)
    set(ENABLE_KEM_SABER ON)
endif ()
