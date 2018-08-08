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

  #BIKE depends on OpenSSL
  AM_COND_IF([USE_OPENSSL],
    [ARG_DISBL_SET_WRAP([kem-bike], [kem_bike], [ENABLE_KEM_BIKE], [src/kem/bike] )],
    [AM_CONDITIONAL(ENABLE_KEM_BIKE, [USE_OPENSSL])]
  )

  ARG_DISBL_SET_WRAP([kem-frodo], [kem_frodo], [ENABLE_KEM_FRODO], [src/kem/frodo])
  ARG_DISBL_SET_WRAP([kem-sike],  [kem_sike],  [ENABLE_KEM_SIKE],  [src/kem/sike])

  ARG_ENABL_SET_WRAP([kex-code-mcbits], [kex_code_mcbits], 
                     [ENABLE_MCBITS], [src/kex_code_mcbits])
  ARG_DISBL_SET_WRAP([kex-ntru], [kex_ntru], 
                     [ENABLE_KEX_NTRU], [src/kex_ntru])
  ARG_DISBL_SET_WRAP([kex-rlwe-newhope],[kex_rlwe_newhope], 
                     [ENABLE_KEX_RLWE_NEWHOPE], [src/kex_rlwe_newhope])
  ARG_ENABL_SET_WRAP([kex-rlwe-newhope-avx2], [kex_rlwe_newhope_avx2], 
                     [ENABLE_KEX_RLWE_NEWHOPE_AVX2], [src/kex_rlwe_newhope_avx2])
  ARG_DISBL_SET_WRAP([kex-sidh-msr], [kex_sidh_msr],
                     [ENABLE_KEX_SIDH_MSR], [src/kex_sidh_msr])

  ARG_DISBL_SET_WRAP([sig-picnic], [sig_picnic], 
                     [ENABLE_SIG_PICNIC], [src/sig_picnic])
  ARG_DISBL_SET_WRAP([sig-qtesla], [sig_qtesla], 
                     [ENABLE_SIG_QTESLA], [src/sig_qtesla])
]
)

AC_DEFUN([CONFIG_FEATURES],
[
  CONFIG_FEATURE_FLAGS

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
  ])

  AM_COND_IF([ENABLE_KEM_FRODO], [
    AC_DEFINE(OQS_ENABLE_KEM_frodo_640_aes,    1, "Define to 1 when Frodo-640-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodo_640_cshake, 1, "Define to 1 when Frodo-640-cSHAKE enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodo_976_aes,    1, "Define to 1 when Frodo-976-AES enabled")
    AC_DEFINE(OQS_ENABLE_KEM_frodo_976_cshake, 1, "Define to 1 when Frodo-976-cSHAKE enabled")
  ])

  AM_COND_IF([ENABLE_KEM_SIKE], [
    AC_DEFINE(OQS_ENABLE_KEM_sike_p503, 1, "Define to 1 when Sike-p503 enabled")
    AC_DEFINE(OQS_ENABLE_KEM_sike_p751, 1, "Define to 1 when Sike-p751 enabled")
  ])

]
)
