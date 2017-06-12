#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ([2.69])
AC_INIT([liboqs], [1.0.0], [])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AM_PROG_AS
AM_INIT_AUTOMAKE([subdir-objects no-dependencies])
m4_include(m4/macros/enable-disable.m4)
m4_include(m4/macros/with.m4)
LT_INIT([disable-shared])
AC_CHECK_SIZEOF([size_t])
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h stddef.h stdint.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup])

# Silent rules
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

######################################################################
# DOXYGEN SUPPORT
######################################################################

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg])
######################################################################

ARG_DISBL_SET([aes-ni], [enable AES-NI.])
AM_CONDITIONAL([aes_ni], [test "x$aes_ni" = xtrue])
AM_CONDITIONAL([USE_AES_NI], [test "x$aes_ni" = xtrue])

ARG_ENABL_SET([openssl], [enable OPENSSL.])
AM_CONDITIONAL([openssl], [test "x$openssl" = xtrue])
AM_CONDITIONAL([USE_OPENSSL], [test "x$openssl" = xtrue])

ARG_ENABL_SET([kex-code-mcbits], [enable KEX-CODE-MCBITS.])
AM_CONDITIONAL([kex_code_mcbits], [test "x$kex_code_mcbits" = xtrue])
AM_CONDITIONAL([USE_MCBITS], [test "x$kex_code_mcbits" = xtrue])

ARG_ENABL_SET([kex-ntru], [enable KEX-NTRU.])
AM_CONDITIONAL([ntru], [test "x$kex_ntru" = xtrue])
AM_CONDITIONAL([USE_NTRU], [test "x$kex_ntru" = xtrue])

ARG_ENABL_SET([kex-sidh-iqc-ref], [enable KEX-SIDH-IQC-REF.])
AM_CONDITIONAL([kex_sidh_iqc_ref], [test "x$kex_sidh_iqc_ref" = xtrue])
AM_CONDITIONAL([USE_SIDH_IQC], [test "x$kex_sidh_iqc_ref" = xtrue])

ARG_ENABL_SET([vscrypto], [enable VSCRYPTO.])
AM_CONDITIONAL([vscrypto], [test "x$vscrypto" = xtrue])
AM_CONDITIONAL([USE_VSCRYPTO], [test "x$vscrypto" = xtrue])



AM_CPPFLAGS="-g -std=gnu11 -Wno-unused-function -Werror -Wpedantic -Wall -Wextra -DCONSTANT_TIME"
AC_CANONICAL_HOST
# Check for which host we are on and setup a few things
# specifically based on the host
case $host_os in
  darwin* )
        AM_CONDITIONAL([ON_DARWIN], [test xtrue = xtrue])
        ;;
  linux*)
        if test x"${ac_cv_sizeof_size_t}" = x"8";then
          AM_CPPFLAGS=${AM_CPPFLAGS}" -DSIDH_ASM -march=x86-64"
        fi
        AM_CONDITIONAL([ON_DARWIN], [test xtrue = xfalse])
        ;;
    *)
        #Default Case
        AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac

AC_ARG_WITH(
  [openssl-dir],
  AS_HELP_STRING([--with-openssl-dir=dir],[openssl dir used locally (default /usr).]),
  [AC_DEFINE_UNQUOTED(OPENSSLDIR, [$withval], [OPENSSL DIR used locally])
   AC_SUBST(OPENSSL_DIR, [$withval])],
  [AC_SUBST(OPENSSL_DIR, /usr)]
)
AC_ARG_WITH(
  [gmp-dir],
  AS_HELP_STRING([--with-gmp-dir=dir],[gmp dir used locally (default /usr).]),
  [AC_DEFINE_UNQUOTED(GMPDIR, [$withval], [GMP DIR used locally])
   AC_SUBST(GMP_DIR, [$withval])],
  [AC_SUBST(GMP_DIR, /usr)]
)
AC_ARG_WITH(
  [sodium-dir],
  AS_HELP_STRING([--with-sodium-dir=dir],[sodium dir used locally (default /usr).]),
  [AC_DEFINE_UNQUOTED(SODIUMDIR, [$withval], [SODIUM DIR used locally])
   AC_SUBST(SODIUM_DIR, [$withval])],
  [AC_SUBST(SODIUM_DIR, /usr)]
)

SRCDIR=" src/common src/crypto/aes src/kex src/crypto/rand src/crypto/sha3"
SRCDIR=${SRCDIR}" src/crypto/rand_urandom_aesctr src/crypto/rand_urandom_chacha20"
SRCDIR=${SRCDIR}" src/kex_rlwe_bcns15/"
SRCDIR=${SRCDIR}" src/kex_rlwe_newhope"
SRCDIR=${SRCDIR}" src/kex_lwe_frodo"
SRCDIR=${SRCDIR}" src/kex_rlwe_msrln16"
SRCDIR=${SRCDIR}" src/kex_sidh_cln16"

if test x"$kex_ntru" = x"true"; then
  AM_CPPFLAGS=${AM_CPPFLAGS}" -DENABLE_NTRU"
  SRCDIR=${SRCDIR}" src/kex_ntru"
fi

if test x"$kex_code_mcbits" = x"true"; then
  AM_CPPFLAGS=${AM_CPPFLAGS}" -DENABLE_CODE_MCBITS"
  SRCDIR=${SRCDIR}" src/kex_code_mcbits"
fi

if test x"$aes_ni" = x"true"; then
  AM_CPPFLAGS=${AM_CPPFLAGS}" -DAES_ENABLE_NI"
fi

if test x"$openssl" = x"true"; then
  AM_CPPFLAGS=${AM_CPPFLAGS}" -DUSE_OPENSSL"
fi

if test x"$kex_sidh_iqc_ref" = x"true"; then
  AM_CPPFLAGS=${AM_CPPFLAGS}" -DENABLE_SIDH_IQC_REF"
  SRCDIR=${SRCDIR}" src/kex_sidh_iqc_ref"
fi

if test x"$vscrypto" = x"true"; then
  AM_CPPFLAGS=${AM_CPPFLAGS}" -DENABLE_VSCRYPTO"
  SRCDIR=${SRCDIR}" src/kex_rlwe_vscrypto"
fi


AC_SUBST(AM_CPPFLAGS)
AC_SUBST(SRCDIR)
AC_SUBST(USE_OPENSSL)
AC_SUBST(USE_AES_NI)
AC_SUBST(USE_NTRU)
AC_SUBST(USE_MCBITS)
AC_SUBST(USE_SIDH_IQC)
AC_SUBST(USE_VSCRYPTO)
AC_SUBST(ON_DARWIN)


AC_CONFIG_FILES([Makefile
                 src/common/Makefile
                 src/kex/Makefile
                 src/crypto/sha3/Makefile
                 src/crypto/rand/Makefile
                 src/crypto/rand_urandom_chacha20/Makefile
                 src/crypto/rand_urandom_aesctr/Makefile
                 src/crypto/aes/Makefile
                 src/kex_rlwe_bcns15/Makefile
                 src/kex_rlwe_newhope/Makefile
                 src/kex_rlwe_msrln16/Makefile
                 src/kex_sidh_cln16/Makefile
                 src/kex_code_mcbits/Makefile
                 src/kex_ntru/Makefile
                 src/kex_sidh_iqc_ref/Makefile
                 src/kex_rlwe_vscrypto/Makefile
                 src/kex_lwe_frodo/Makefile])

AC_OUTPUT
