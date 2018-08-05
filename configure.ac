#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Init
AC_INIT([liboqs], [1.0.0], [])
LT_INIT([disable-shared])

AM_INIT_AUTOMAKE([subdir-objects no-dependencies])

# Configure
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/common/ds_benchmark.h])
AC_CONFIG_MACRO_DIRS([config])

# Set paths to external libraries.
ADD_EXTERNAL_LIB

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_AS

# Perform general system checks 
AC_GENERAL_CHECKS

# DOXYGEN SUPPORT
CONFIG_DOXIGEN

# Configure the list of supported features
CONFIG_FEATURES

AC_CANONICAL_HOST

# Check for which host we are on and setup a few things
# specifically based on the host
case $host_os in
  darwin* )
        darwin=true
        ;;
  linux-android*)
        AM_CFLAGS=${AM_CFLAGS}" -pie "
        linux=true
        ;;
  linux*)
        linux=true
        ;;
    *)
        #Default Case
        AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac

AM_CONDITIONAL([ON_DARWIN], [test "x$darwin" = xtrue])

# Enable assembly optimizations here
# Appearenly asm optimizations do not work well with darwin
if test x"$linux" = x"true"; then
  case $host_cpu in
    x86_64* )
        AM_CPPFLAGS=${AM_CPPFLAGS}" -DSIDH_ASM -march=x86-64"
        x86_64=true
        ;;
    aarch64* )
        AM_CPPFLAGS=${AM_CPPFLAGS}" -DSIDH_ASM -march=armv8-a+crc"
        arm64=true
        ;;
    arm* )
        AM_CPPFLAGS=${AM_CPPFLAGS}" -DARM"
        AM_CONDITIONAL([USE_AES_NI], [false])
        arm=true
        ;;
  esac
fi

AM_CONDITIONAL([X86_64], [test "x$x86_64" = xtrue])
AM_CONDITIONAL([ARM64], [test "x$arm64" = xtrue])
AM_CONDITIONAL([ARM], [test "x$arm" = xtrue])

# Common sources
SRCDIR=${SRCDIR}" src/common"

# Crypto sources
SRCDIR=${SRCDIR}" src/crypto/aes \
                  src/crypto/sha3 \
                  src/crypto/rand_urandom_aesctr \
                  src/crypto/rand_urandom_chacha20"

# Protocols
SRCDIR=${SRCDIR}" src/kem \
                  src/kex \
                  src/sig"

#Set the default compilation flags
SET_CFLAGS

AC_SUBST(ON_DARWIN)
AC_SUBST(X86_64)
AC_SUBST(SRCDIR)

AC_CONFIG_FILES([Makefile
                 src/common/Makefile
                 src/kem/Makefile
                 src/kex/Makefile
                 src/crypto/sha3/Makefile
                 src/crypto/rand_urandom_chacha20/Makefile
                 src/crypto/rand_urandom_aesctr/Makefile
                 src/crypto/aes/Makefile
                 src/kex_rlwe_newhope/Makefile
                 src/kex_sidh_msr/Makefile
                 src/kex_code_mcbits/Makefile
                 src/kex_ntru/Makefile
                 src/sig/Makefile
                 src/sig_picnic/Makefile
                 src/sig_qtesla/Makefile
                 src/kex_rlwe_newhope/avx2/Makefile
                 src/kem/bike/Makefile
                 src/kem/frodo/Makefile
                 src/kem/sike/Makefile                 
                 ])

AC_OUTPUT
