# Checks for libraries.
AC_DEFUN([ADD_EXTERNAL_LIB],
  [AC_ARG_WITH(
    [openssl-dir],
    AS_HELP_STRING([--with-openssl-dir=dir],[openssl dir used locally (default /usr).]),
    [AC_DEFINE_UNQUOTED(OPENSSLDIR, [$withval], [OPENSSL DIR used locally])
     AC_SUBST(OPENSSL_DIR, [$withval])],
    [AC_SUBST(OPENSSL_DIR, /usr)]
  )]
  [AC_ARG_WITH(
    [sodium-dir],
    AS_HELP_STRING([--with-sodium-dir=dir],[sodium dir used locally (default /usr).]),
    [AC_DEFINE_UNQUOTED(SODIUMDIR, [$withval], [SODIUM DIR used locally])
     AC_SUBST(SODIUM_DIR, [$withval])],
    [AC_SUBST(SODIUM_DIR, /usr)]
  )]
  [AC_ARG_WITH(
    [m4ri-dir],
    AS_HELP_STRING([--with-m4ri-dir=dir],[m4ri dir used locally (default /usr).]),
    [AC_DEFINE_UNQUOTED(M4RIDIR, [$withval], [M4RI DIR used locally])
     AC_SUBST(M4RI_DIR, [$withval])],
    [AC_SUBST(M4RI_DIR, /usr)]
  )]
)
