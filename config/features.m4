AC_DEFUN([ARG_DISBL_SET_WRAP],
[
  ARG_DISBL_SET([$1], [disable m4_toupper($1).])
  AM_CONDITIONAL($3, [test x$$2 = xtrue])
  AM_COND_IF($3, [
    SRCDIR="${SRCDIR} $4"
    AC_DEFINE([$3], 1, "Define to 1 when $1 is enabled")
  ], [])
]
)

AC_DEFUN([ARG_ENABL_SET_WRAP],
[
  ARG_ENABL_SET([$1], [enable m4_toupper($1).])
  AM_CONDITIONAL($3, [test x$$2 = xtrue])
  AM_COND_IF($3, [
    SRCDIR="${SRCDIR} $4"
    AC_DEFINE([$3], 1, "Define to 1 when $1 is enabled")
  ], [])
]
)

AC_DEFUN([CONFIG_FEATURE_FLAGS],
[

  #The flags are organizes according to features then cryptosystems.

  # The parameters are
  # 1) Flag-name
  # 2) Param-name
  # 3) Compilation param name
  # 4) Compilation directory
  ARG_DISBL_SET_WRAP([aes-ni],  [aes_ni],  [USE_AES_NI])
  ARG_ENABL_SET_WRAP([openssl], [openssl], [USE_OPENSSL])

  AS_IF([test "x${enable_shared}" = "xyes" ], AC_MSG_RESULT([yes]), AC_MSG_RESULT([no]))
  AM_CONDITIONAL([ENABLE_SHARED],[test "x${enable_shared}" = "xyes"])
  AC_SUBST(ENABLE_SHARED)

  #BIKE depends on OpenSSL
  AM_COND_IF([USE_OPENSSL],
    [ARG_DISBL_SET_WRAP([kem-bike], [kem_bike], [ENABLE_KEM_BIKE], [src/kem/bike] )],
    [AM_CONDITIONAL(ENABLE_KEM_BIKE, [false])]
  )

##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ARG_DISBL_SET_WRAP_START
  ARG_DISBL_SET_WRAP([kem-frodokem], [kem_frodokem], [ENABLE_KEM_FRODOKEM], [src/kem/frodokem])
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ARG_DISBL_SET_WRAP_END
  ARG_DISBL_SET_WRAP([kem-sike],  [kem_sike],  [ENABLE_KEM_SIKE],  [src/kem/sike])
  ARG_DISBL_SET_WRAP([kem-newhope],  [kem_newhope],  [ENABLE_KEM_NEWHOPE],  [src/kem/newhopenist])
  ARG_DISBL_SET_WRAP([kem-kyber],  [kem_kyber],  [ENABLE_KEM_KYBER],  [src/kem/kyber])

  ARG_DISBL_SET_WRAP([sig-picnic], [sig_picnic],
                     [ENABLE_SIG_PICNIC], [src/sig/picnic])
  ARG_DISBL_SET_WRAP([sig-qtesla], [sig_qtesla],
                     [ENABLE_SIG_QTESLA], [src/sig/qtesla])
]
)

AC_DEFUN([CONFIG_FEATURES],
[
  CONFIG_FEATURE_FLAGS

  # Default is set to false, will be changed upon demand
  AM_CONDITIONAL([BIKE_ADDITIONAL_IMPL], [false])

  AM_COND_IF([ENABLE_KEM_BIKE], [
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l1, 1, "Define to 1 when BIKE1-L1 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l3, 1, "Define to 1 when BIKE1-L3 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike1_l5, 1, "Define to 1 when BIKE1-L5 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike2_l1, 1, "Define to 1 when BIKE2-L1 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike2_l3, 1, "Define to 1 when BIKE2-L3 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike2_l5, 1, "Define to 1 when BIKE2-L5 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike3_l1, 1, "Define to 1 when BIKE3-L1 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike3_l3, 1, "Define to 1 when BIKE3-L3 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_bike3_l5, 1, "Define to 1 when BIKE3-L5 enabled")

    AM_COND_IF([USE_AVX2_INSTRUCTIONS],
               [AM_CONDITIONAL([BIKE_ADDITIONAL_IMPL], [test x$gcc_cv_compiler = xtrue])
                AM_COND_IF([BIKE_ADDITIONAL_IMPL], [AC_DEFINE(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION, 1, "Define to 1 when BIKE uses the additional implementation")])
               ])
  ])

##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_AM_COND_IF_START
  AM_COND_IF([ENABLE_KEM_FRODOKEM], [
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_640_aes, 1, "Define to 1 when FrodoKEM-640-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_640_shake, 1, "Define to 1 when FrodoKEM-640-SHAKE enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_976_aes, 1, "Define to 1 when FrodoKEM-976-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_976_shake, 1, "Define to 1 when FrodoKEM-976-SHAKE enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_1344_aes, 1, "Define to 1 when FrodoKEM-1344-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodokem_1344_shake, 1, "Define to 1 when FrodoKEM-1344-SHAKE enabled")
  ])
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_AM_COND_IF_END

  AM_COND_IF([ENABLE_KEM_SIKE], [
    AC_DEFINE(OQS_ENABLE_KEM_sike_p503, 1, "Define to 1 when Sike-p503 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p751, 1, "Define to 1 when Sike-p751 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p503, 1, "Define to 1 when Sidh-p503 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sidh_p751, 1, "Define to 1 when Sidh-p751 enabled")
  ])

  AM_COND_IF([ENABLE_SIG_QTESLA], [
    AC_DEFINE(OQS_ENABLE_SIG_qTESLA_I,         1, "Define to 1 when qTESLA-I enabled")
    AC_DEFINE(OQS_ENABLE_SIG_qTESLA_III_size,  1, "Define to 1 when qTESLA-III-size enabled")
    AC_DEFINE(OQS_ENABLE_SIG_qTESLA_III_speed, 1, "Define to 1 when qTESLA-III-speed enabled")
  ])

  AM_COND_IF([ENABLE_SIG_PICNIC], [
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L1_FS, 1, "Define to 1 when picnic-L1-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L1_UR, 1, "Define to 1 when picnic-L1-UR enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L3_FS, 1, "Define to 1 when picnic-L3-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L3_UR, 1, "Define to 1 when picnic-L3-UR enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L5_FS, 1, "Define to 1 when picnic-L5-FS enabled")
    AC_DEFINE(OQS_ENABLE_SIG_picnic_L5_UR, 1, "Define to 1 when picnic-L5-UR enabled")
  ])

  AM_COND_IF([ENABLE_KEM_NEWHOPE], [
    AC_DEFINE(OQS_ENABLE_KEM_newhope_512_cca_kem, 1, "Define to 1 when NewHope-512-CCA-KEM enabled")
    AC_DEFINE(OQS_ENABLE_KEM_newhope_1024_cca_kem, 1, "Define to 1 when NewHope-1024-CCA-KEM enabled")
  ])

  AM_COND_IF([ENABLE_KEM_KYBER], [
    AC_DEFINE(OQS_ENABLE_KEM_kyber_512_cca_kem, 1, "Define to 1 when Kyber-512-CCA-KEM enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_768_cca_kem, 1, "Define to 1 when Kyber-512-CCA-KEM enabled")
    AC_DEFINE(OQS_ENABLE_KEM_kyber_1024_cca_kem, 1, "Define to 1 when Kyber-1024-CCA-KEM enabled")
  ])
]
)
