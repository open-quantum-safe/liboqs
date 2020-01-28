# Checks for libraries.
AC_DEFUN([ADD_EXTERNAL_LIB],[

  AC_MSG_CHECKING([whether to use OpenSSL])
  # define the --with-openssl option
  AC_ARG_WITH([openssl],
    [AS_HELP_STRING(
      [--without-openssl],
      [disable OpenSSL; use --with-openssl=dir to specify OpenSSL dir (default /usr or /usr/local/opt/openssl on macOS)]
    )],
    [with_openssl=$withval],
    [with_openssl=yes]
  )
  # platform-specific default directory
  AS_IF(
    [test "x${with_openssl}" = "xyes"],
    [AM_COND_IF(ON_DARWIN,
      [
        AS_IF(
          [test -d "/usr/local/opt/openssl@1.1"],
          [with_openssl=/usr/local/opt/openssl@1.1],
          [with_openssl=/usr/local/opt/openssl]
        )
      ],
      [with_openssl=/usr]
    )]
  )
  AS_IF(
    [test "x${with_openssl}" = "xno"],
    [AC_MSG_RESULT([no])],
    [
      AC_MSG_RESULT([yes (${with_openssl})])
      # set OPENSSL_DIR based on value provided
      AC_SUBST([OPENSSL_DIR], [${with_openssl}])
    ]
  )
  # define automake conditional USE_OPENSSL
  AM_CONDITIONAL(USE_OPENSSL, [test "x${with_openssl}" != "xno"])
  # define C macro USE_OPENSSL
  AM_COND_IF(USE_OPENSSL,
    [AC_DEFINE(USE_OPENSSL, 1, [Defined to 1 if using OpenSSL in liboqs])]
  )

  AS_IF(
    [test "x${with_openssl}" != "xno"],
    [
      AC_MSG_CHECKING([OpenSSL version])
      OPENSSL_VERSION=`grep OPENSSL_VERSION_TEXT ${with_openssl}/include/openssl/opensslv.h | head -n 1 | sed -e 's/.*OpenSSL //' | sed -e 's/ .*//'`
      OPENSSL_MAJOR_VERSION=`echo ${OPENSSL_VERSION} | cut -c 1-3`
      AS_IF(
        [test "x${OPENSSL_MAJOR_VERSION}" == "x1.1"],
        [AC_MSG_RESULT([ok (${OPENSSL_VERSION})])],
        [AC_MSG_FAILURE([too old (found ${OPENSSL_VERSION}, need >= 1.1.0)])]
      )
    ]
  )

  AC_ARG_WITH([m4ri-dir],
    [AS_HELP_STRING(
      [--with-m4ri-dir=dir],
      [m4ri dir used locally (default /usr)]
    )],
    [
      AC_DEFINE_UNQUOTED(M4RIDIR, [$withval], [M4RI DIR used locally])
      AC_SUBST(M4RI_DIR, [$withval])
    ],
    [AC_SUBST(M4RI_DIR, /usr)]
  )

  # define the --with-sha3 option
  AC_ARG_WITH([sha3],
    [AS_HELP_STRING(
      [--with-sha3=arg],
      [SHA3 implementation to use: openssl (default if available), c]
    )],
    [],
    [with_sha3=default]
  )
  # if supposed to use the default option
  AS_IF(
    [test "x${with_sha3}" = "xdefault"],
    # if OpenSSL is available
    [AM_COND_IF(USE_OPENSSL,
      [
        # check whether we have EVP_sha3_256
        # note we need to save then restore CFLAGS/LDFLAGS
        old_CFLAGS=${CFLAGS}
        old_LDFLAGS=${LDFLAGS}
        CFLAGS="${CFLAGS} -I${with_openssl}/include"
        LDFLAGS="${LDFLAGS} -L${with_openssl}/lib"
        AC_LANG_PUSH([C])
        AC_CHECK_LIB([crypto], [EVP_sha3_256],
          [with_sha3=openssl],
          [with_sha3=c]
        )
        AC_LANG_POP([C])
        CFLAGS=${CFLAGS}
        LDFLAGS=${LDFLAGS}
      ],
      # use C
      [with_sha3=c]
    )]
  )
  # report result
  AC_MSG_CHECKING([which SHA-3 implementation to use])
  # check for invalid arguments
  AS_IF(
    [test "x${with_sha3}" != "xopenssl" -a "x${with_sha3}" != "xc"],
    AC_MSG_FAILURE([invalid --with-sha3 option])
  )
  AC_MSG_RESULT([${with_sha3}])
  # set automake variables and C defines
  AM_CONDITIONAL(USE_SHA3_OPENSSL, [test "x${with_sha3}" = "xopenssl"])
  AM_CONDITIONAL(USE_SHA3_C, [test "x${with_sha3}" = "xc"])
  AM_COND_IF(USE_SHA3_OPENSSL,[
    AC_DEFINE(USE_SHA3_OPENSSL, [1], [Defined to 1 if liboqs should use OpenSSL for SHA-3 where possible])
  ])
  AM_COND_IF(USE_SHA3_C,[
    AC_DEFINE(USE_SHA3_C, [1], [Define to 1 if liboqs should use built-in C code for SHA-3])
  ])

  # define the --with-aes option
  AC_ARG_WITH([aes],
    [AS_HELP_STRING(
      [--with-aes=arg],
      [AES implementation to use: openssl (default if available), c]
    )],
    [],
    [with_aes=default]
  )
  # if supposed to use the default option
  AS_IF(
    [test "x${with_aes}" = "xdefault"],
    # if OpenSSL is available
    [AM_COND_IF(USE_OPENSSL,
      [with_aes=openssl],
      [with_aes=c]
    )]
  )
  # report result
  AC_MSG_CHECKING([which AES implementation to use])
  # check for invalid arguments
  AS_IF(
    [test "x${with_aes}" != "xopenssl" -a "x${with_aes}" != "xc"],
    AC_MSG_FAILURE([invalid --with-aes option])
  )
  AC_MSG_RESULT([${with_aes}])
  # set automake variables and C defines
  AM_CONDITIONAL(USE_AES_OPENSSL, [test "x${with_aes}" = "xopenssl"])
  AM_CONDITIONAL(USE_AES_C, [test "x${with_aes}" = "xc"])
  AM_COND_IF(USE_AES_OPENSSL,[
    AC_DEFINE(USE_AES_OPENSSL, [1], [Defined to 1 if liboqs should use OpenSSL for AES where possible])
  ])
  AM_COND_IF(USE_AES_C,[
    AC_DEFINE(USE_AES_C, [1], [Define to 1 if liboqs should use built-in C code for AES])
  ])

  # define the --with-sha2 option
  AC_ARG_WITH([sha2],
    [AS_HELP_STRING(
      [--with-sha2=arg],
      [SHA-2 implementation to use: openssl (default if available), c]
    )],
    [],
    [with_sha2=default]
  )
  # if supposed to use the default option
  AS_IF(
    [test "x${with_sha2}" = "xdefault"],
    # if OpenSSL is available
    [AM_COND_IF(USE_OPENSSL,
      [with_sha2=openssl],
      [with_sha2=c]
    )]
  )
  # report result
  AC_MSG_CHECKING([which SHA-2 implementation to use])
  # check for invalid arguments
  AS_IF(
    [test "x${with_sha2}" != "xopenssl" -a "x${with_sha2}" != "xc"],
    AC_MSG_FAILURE([invalid --with-sha2 option])
  )
  AC_MSG_RESULT([${with_sha2}])
  # set automake variables and C defines
  AM_CONDITIONAL(USE_SHA2_OPENSSL, [test "x${with_sha2}" = "xopenssl"])
  AM_CONDITIONAL(USE_SHA2_C, [test "x${with_sha2}" = "xc"])
  AM_COND_IF(USE_SHA2_OPENSSL,[
    AC_DEFINE(USE_SHA2_OPENSSL, [1], [Defined to 1 if liboqs should use OpenSSL for SHA-2 where possible])
  ])
  AM_COND_IF(USE_SHA2_C,[
    AC_DEFINE(USE_SHA2_C, [1], [Define to 1 if liboqs should use built-in C code for SHA-2])
  ])

])
