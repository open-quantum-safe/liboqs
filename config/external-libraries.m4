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
    [with_openssl=default]
  )
  # platform-specific default directory
  AS_IF(
    [test "x${with_openssl}" = "xdefault"],
    [AM_COND_IF(ON_DARWIN,
      [with_openssl=/usr/local/opt/openssl],
      [with_openssl=/usr]
    )]
  )
  AS_IF(
    [test "x${with_openssl}" = "xno"],
    [AC_MSG_RESULT([no])],
    [
      AC_MSG_RESULT([${with_openssl}])
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
])
