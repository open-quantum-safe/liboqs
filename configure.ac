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
AM_INIT_AUTOMAKE([subdir-objects])
m4_include(m4/macros/enable-disable.m4)
LT_INIT([disable-shared])

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

ARG_DISBL_SET([aes-ni], [disable AES-NI.])
AM_CONDITIONAL([aes_ni], [test "x$aes_ni" = xtrue])
AM_CONDITIONAL([USE_AES_NI], [test "x$aes_ni" = xtrue])

ARG_ENABL_SET([openssl], [enable OPENSSL.])
AM_CONDITIONAL([openssl], [test "x$openssl" = xtrue])
AM_CONDITIONAL([USE_OPENSSL], [test "x$openssl" = xtrue])

AC_CANONICAL_HOST
# Check for which host we are on and setup a few things
# specifically based on the host
case $host_os in
  darwin* )
        OPENSSL_DIR=/usr/local/opt/openssl
        ;;
  linux*)
        OPENSSL_DIR=/usr
        ;;
    *)
        #Default Case
        AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac


WANTEDLIBS=" src/aes/libaes.la src/kex/libkex.la"
WANTEDLIBS=${WANTEDLIBS}" src/rand/librand.la"
WANTEDLIBS=${WANTEDLIBS}" src/rand_urandom_aesctr/librandaesctr.la"
WANTEDLIBS=${WANTEDLIBS}" src/rand_urandom_chacha20/librandchacha20.la"
WANTEDLIBS=${WANTEDLIBS}" src/kex_rlwe_bcns15/libbcns15.la"
WANTEDLIBS=${WANTEDLIBS}" src/kex_rlwe_newhope/libnewhope.la"
WANTEDLIBS=${WANTEDLIBS}" src/kex_lwe_frodo/libfrodo.la"

SRCDIR=" src/aes src/kex src/rand src/rand_urandom_aesctr src/rand_urandom_chacha20"
SRCDIR=${SRCDIR}" src/kex_rlwe_bcns15/"
SRCDIR=${SRCDIR}" src/kex_rlwe_newhope"
SRCDIR=${SRCDIR}" src/kex_lwe_frodo"

AM_CPPFLAGS="-std=c11"
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(WANTEDLIBS)
AC_SUBST(SRCDIR)
AC_SUBST(OPENSSL_DIR)
AC_SUBST(USE_OPENSSL)

AC_CONFIG_FILES([Makefile
                 src/kex/Makefile
                 src/rand/Makefile
                 src/rand_urandom_chacha20/Makefile
                 src/rand_urandom_aesctr/Makefile
                 src/aes/Makefile
                 src/kex_rlwe_bcns15/Makefile
                 src/kex_rlwe_newhope/Makefile
                 src/kex_lwe_frodo/Makefile])

AC_OUTPUT
