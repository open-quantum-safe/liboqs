#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Init
AC_INIT([liboqs], [1.0.0], [])
LT_INIT([disable-shared])

AM_INIT_AUTOMAKE([subdir-objects no-dependencies])

# Configure
AC_CONFIG_HEADERS([config.h])
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
CONFIG_DOXYGEN

# Detect the host CPU and x86 features that are availble.
DETECT_HOST_AND_CPU

# Configure the list of supported features
CONFIG_FEATURES

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
SET_AM_CFLAGS

AC_SUBST(SRCDIR)
AC_CONFIG_FILES([Makefile
                 src/common/Makefile
                 src/kem/Makefile
                 src/kex/Makefile
                 src/crypto/sha3/Makefile
                 src/crypto/rand_urandom_chacha20/Makefile
                 src/crypto/rand_urandom_aesctr/Makefile
                 src/crypto/aes/Makefile
                 src/kex_sidh_msr/Makefile
                 src/kex_code_mcbits/Makefile
                 src/kex_ntru/Makefile
                 src/sig/Makefile
                 src/sig_picnic/Makefile
                 src/sig_qtesla/Makefile
                 src/kem/bike/Makefile
                 src/kem/frodokem/Makefile
                 src/kem/sike/Makefile
                 src/kem/newhope/Makefile
                 tests/Makefile
                 ])

AC_OUTPUT
