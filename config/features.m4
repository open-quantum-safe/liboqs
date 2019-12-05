AC_DEFUN([ARG_DISBL_SET_WRAP], [
  AC_MSG_CHECKING([whether to enable $2])
  AC_ARG_ENABLE(
    [$1],
    [AS_HELP_STRING([--disable-$1], [disable $2])],
    [use_$2=$enableval],
    [use_$2=yes]
  )
  AM_CONDITIONAL($3, [test "x${use_$2}" != "xno"])
  AM_COND_IF($3, [
    SRCDIR="${SRCDIR} $4"
    AC_DEFINE([$3], 1, "Defined to 1 when $1 is enabled")
    AC_MSG_RESULT([yes])
  ], [
    AC_MSG_RESULT([no])
  ])
])

AC_DEFUN([CONFIG_FEATURE_FLAGS],
[

  # The flags are organized according to features then cryptosystems.

  # The parameters are
  # 1) Flag-name
  # 2) Param-name
  # 3) Compilation param name
  # 4) Compilation directory

##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ARG_DISBL_SET_WRAP_START
  ARG_DISBL_SET_WRAP([kem-kyber], [kem_kyber], [ENABLE_KEM_KYBER], [src/kem/kyber])
  ARG_DISBL_SET_WRAP([kem-newhope], [kem_newhope], [ENABLE_KEM_NEWHOPE], [src/kem/newhope])
  ARG_DISBL_SET_WRAP([kem-ntru], [kem_ntru], [ENABLE_KEM_NTRU], [src/kem/ntru])
  ARG_DISBL_SET_WRAP([kem-saber], [kem_saber], [ENABLE_KEM_SABER], [src/kem/saber])
  ARG_DISBL_SET_WRAP([sig-dilithium], [sig_dilithium], [ENABLE_SIG_DILITHIUM], [src/sig/dilithium])
  ARG_DISBL_SET_WRAP([sig-mqdss], [sig_mqdss], [ENABLE_SIG_MQDSS], [src/sig/mqdss])
  ARG_DISBL_SET_WRAP([sig-sphincs], [sig_sphincs], [ENABLE_SIG_SPHINCS], [src/sig/sphincs])
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ARG_DISBL_SET_WRAP_END
  ARG_DISBL_SET_WRAP([kem-bike], [kem_bike], [ENABLE_KEM_BIKE], [src/kem/bike])
  ARG_DISBL_SET_WRAP([kem-frodokem], [kem_frodokem], [ENABLE_KEM_FRODOKEM], [src/kem/frodokem])
  ARG_DISBL_SET_WRAP([kem-sike],  [kem_sike],  [ENABLE_KEM_SIKE],  [src/kem/sike])
  ARG_DISBL_SET_WRAP([sig-picnic], [sig_picnic], [ENABLE_SIG_PICNIC], [src/sig/picnic])
  ARG_DISBL_SET_WRAP([sig-qtesla], [sig_qtesla], [ENABLE_SIG_QTESLA], [src/sig/qtesla])
]
)

AC_DEFUN([CONFIG_FEATURES],
[
  CONFIG_FEATURE_FLAGS

  # Default is set to false, will be changed upon demand
  AM_CONDITIONAL([BIKE_ADDITIONAL_IMPL], [false])

  AM_COND_IF([ENABLE_KEM_BIKE], [
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l1_cpa, 1, "Define to 1 when BIKE1-L1-CPA enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l3_cpa, 1, "Define to 1 when BIKE1-L3-CPA enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l1_fo, 1, "Define to 1 when BIKE1-L1-FO enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l3_fo, 1, "Define to 1 when BIKE1-L3-FO enabled")
  ])

##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_AM_COND_IF_START
  AM_COND_IF([ENABLE_KEM_KYBER], [
    AC_DEFINE(OQS_ENABLE_KEM_kyber_512, 1, "Define to 1 when Kyber512 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_768, 1, "Define to 1 when Kyber768 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_1024, 1, "Define to 1 when Kyber1024 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_512_90s, 1, "Define to 1 when Kyber512-90s enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_768_90s, 1, "Define to 1 when Kyber768-90s enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_1024_90s, 1, "Define to 1 when Kyber1024-90s enabled")
  ])
  AM_COND_IF([ENABLE_KEM_NEWHOPE], [
    AC_DEFINE(OQS_ENABLE_KEM_newhope_512cca, 1, "Define to 1 when NewHope-512-CCA enabled")
    AC_DEFINE(OQS_ENABLE_KEM_newhope_1024cca, 1, "Define to 1 when NewHope-1024-CCA enabled")
  ])
  AM_COND_IF([ENABLE_KEM_NTRU], [
    AC_DEFINE(OQS_ENABLE_KEM_ntru_hps2048509, 1, "Define to 1 when NTRU-HPS-2048-509 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_ntru_hps2048677, 1, "Define to 1 when NTRU-HPS-2048-677 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_ntru_hps4096821, 1, "Define to 1 when NTRU-HPS-4096-821 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_ntru_hrss701, 1, "Define to 1 when NTRU-HRSS-701 enabled")
  ])
  AM_COND_IF([ENABLE_KEM_SABER], [
    AC_DEFINE(OQS_ENABLE_KEM_saber_lightsaber, 1, "Define to 1 when LightSaber-KEM enabled")
    AC_DEFINE(OQS_ENABLE_KEM_saber_saber, 1, "Define to 1 when Saber-KEM enabled")
    AC_DEFINE(OQS_ENABLE_KEM_saber_firesaber, 1, "Define to 1 when FireSaber-KEM enabled")
  ])
  AM_COND_IF([ENABLE_SIG_DILITHIUM], [
    AC_DEFINE(OQS_ENABLE_SIG_dilithium_2, 1, "Define to 1 when DILITHIUM_2 enabled")
    AC_DEFINE(OQS_ENABLE_SIG_dilithium_3, 1, "Define to 1 when DILITHIUM_3 enabled")
    AC_DEFINE(OQS_ENABLE_SIG_dilithium_4, 1, "Define to 1 when DILITHIUM_4 enabled")
  ])
  AM_COND_IF([ENABLE_SIG_MQDSS], [
    AC_DEFINE(OQS_ENABLE_SIG_mqdss_31_48, 1, "Define to 1 when MQDSS-31-48 enabled")
    AC_DEFINE(OQS_ENABLE_SIG_mqdss_31_64, 1, "Define to 1 when MQDSS-31-64 enabled")
  ])
  AM_COND_IF([ENABLE_SIG_SPHINCS], [
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_128f_robust, 1, "Define to 1 when SPHINCS+-Haraka-128f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_128f_simple, 1, "Define to 1 when SPHINCS+-Haraka-128f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_128s_robust, 1, "Define to 1 when SPHINCS+-Haraka-128s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_128s_simple, 1, "Define to 1 when SPHINCS+-Haraka-128s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_192f_robust, 1, "Define to 1 when SPHINCS+-Haraka-192f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_192f_simple, 1, "Define to 1 when SPHINCS+-Haraka-192f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_192s_robust, 1, "Define to 1 when SPHINCS+-Haraka-192s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_192s_simple, 1, "Define to 1 when SPHINCS+-Haraka-192s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_256f_robust, 1, "Define to 1 when SPHINCS+-Haraka-256f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_256f_simple, 1, "Define to 1 when SPHINCS+-Haraka-256f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_256s_robust, 1, "Define to 1 when SPHINCS+-Haraka-256s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_haraka_256s_simple, 1, "Define to 1 when SPHINCS+-Haraka-256s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_128f_robust, 1, "Define to 1 when SPHINCS+-SHA256-128f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_128f_simple, 1, "Define to 1 when SPHINCS+-SHA256-128f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_128s_robust, 1, "Define to 1 when SPHINCS+-SHA256-128s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_128s_simple, 1, "Define to 1 when SPHINCS+-SHA256-128s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_192f_robust, 1, "Define to 1 when SPHINCS+-SHA256-192f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_192f_simple, 1, "Define to 1 when SPHINCS+-SHA256-192f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_192s_robust, 1, "Define to 1 when SPHINCS+-SHA256-192s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_192s_simple, 1, "Define to 1 when SPHINCS+-SHA256-192s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_256f_robust, 1, "Define to 1 when SPHINCS+-SHA256-256f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_256f_simple, 1, "Define to 1 when SPHINCS+-SHA256-256f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_256s_robust, 1, "Define to 1 when SPHINCS+-SHA256-256s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_sha256_256s_simple, 1, "Define to 1 when SPHINCS+-SHA256-256s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_128f_robust, 1, "Define to 1 when SPHINCS+-SHAKE256-128f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_128f_simple, 1, "Define to 1 when SPHINCS+-SHAKE256-128f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_128s_robust, 1, "Define to 1 when SPHINCS+-SHAKE256-128s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_128s_simple, 1, "Define to 1 when SPHINCS+-SHAKE256-128s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_192f_robust, 1, "Define to 1 when SPHINCS+-SHAKE256-192f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_192f_simple, 1, "Define to 1 when SPHINCS+-SHAKE256-192f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_192s_robust, 1, "Define to 1 when SPHINCS+-SHAKE256-192s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_192s_simple, 1, "Define to 1 when SPHINCS+-SHAKE256-192s-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_256f_robust, 1, "Define to 1 when SPHINCS+-SHAKE256-256f-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_256f_simple, 1, "Define to 1 when SPHINCS+-SHAKE256-256f-simple enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_256s_robust, 1, "Define to 1 when SPHINCS+-SHAKE256-256s-robust enabled")
    AC_DEFINE(OQS_ENABLE_SIG_sphincs_shake256_256s_simple, 1, "Define to 1 when SPHINCS+-SHAKE256-256s-simple enabled")
  ])
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_AM_COND_IF_END

  AM_COND_IF([ENABLE_KEM_FRODOKEM], [
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_640_aes, 1, "Define to 1 when FrodoKEM-640-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_640_shake, 1, "Define to 1 when FrodoKEM-640-SHAKE enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_976_aes, 1, "Define to 1 when FrodoKEM-976-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_976_shake, 1, "Define to 1 when FrodoKEM-976-SHAKE enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_1344_aes, 1, "Define to 1 when FrodoKEM-1344-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_1344_shake, 1, "Define to 1 when FrodoKEM-1344-SHAKE enabled")
  ])

  AM_COND_IF([ENABLE_KEM_SIKE], [
    AC_DEFINE(OQS_ENABLE_KEM_sike_p434, 1, "Define to 1 when SIKE-p434 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p434_compressed, 1, "Define to 1 when SIKE-p434-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p503, 1, "Define to 1 when SIKE-p503 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p503_compressed, 1, "Define to 1 when SIKE-p503-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p610, 1, "Define to 1 when SIKE-p610 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p610_compressed, 1, "Define to 1 when SIKE-p610-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p751, 1, "Define to 1 when SIKE-p751 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p751_compressed, 1, "Define to 1 when SIKE-p751-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p434, 1, "Define to 1 when SIDH-p434 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p434_compressed, 1, "Define to 1 when SIDH-p434-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p503, 1, "Define to 1 when SIDH-p503 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p503_compressed, 1, "Define to 1 when SIDH-p503-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p610, 1, "Define to 1 when SIDH-p610 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p610_compressed, 1, "Define to 1 when SIDH-p610-compressed enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p751, 1, "Define to 1 when SIDH-p751 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p751_compressed, 1, "Define to 1 when SIDH-p751-compressed enabled")
  ])

  AM_COND_IF([ENABLE_SIG_QTESLA], [
    AC_DEFINE(OQS_ENABLE_SIG_qTesla_p_I,   1, "Define to 1 when qTesla-p-I enabled")
    AC_DEFINE(OQS_ENABLE_SIG_qTesla_p_III, 1, "Define to 1 when qTesla-p-III-size enabled")
  ])

  AM_COND_IF([ENABLE_SIG_PICNIC], [
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L1_FS, 1, "Define to 1 when picnic-L1-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L1_UR, 1, "Define to 1 when picnic-L1-UR enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L3_FS, 1, "Define to 1 when picnic-L3-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L3_UR, 1, "Define to 1 when picnic-L3-UR enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L5_FS, 1, "Define to 1 when picnic-L5-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L5_UR, 1, "Define to 1 when picnic-L5-UR enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic2_L1_FS, 1, "Define to 1 when picnic2-L1-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic2_L3_FS, 1, "Define to 1 when picnic2-L3-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic2_L5_FS, 1, "Define to 1 when picnic2-L5-FS enabled")
  ])

]
)
