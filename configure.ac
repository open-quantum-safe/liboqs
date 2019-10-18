#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Init
AC_INIT([liboqs], [0.2.1-dev], [https://github.com/open-quantum-safe/liboqs/issues], [liboqs-master], [https://openquantumsafe.org/])

AM_INIT_AUTOMAKE([subdir-objects no-dependencies])
AM_PROG_LIBTOOL

# Numeric release version identifier:
# MNNFFPPS: major minor fix patch status
# The status nibble has one of the values 0 for development, 1 to e for release candidates 1 to 14, and f for release
# For example:
# 1.2.3-dev      0x10203000
# 1.2.3-rc1      0x10203001
# 1.2.3-rc2-dev  0x10203002
# 1.2.3-rc2      0x10203002 (same as ...rc2-dev)
# 1.2.3          0x1020300f
# 1.2.3a         0x1020301f
AC_DEFINE([OQS_VERSION_NUMBER],[0x00201000L],[Numeric liboqs version])
AC_DEFINE([OQS_VERSION_TEXT],["0.2.1-dev"],[Text liboqs version])
AC_DEFINE([OQS_MASTER_BRANCH],[],[liboqs branch])

# Configure
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([config])

# Detect the host CPU and x86 features that are available.
DETECT_HOST_AND_CPU

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

# Configure the list of supported features
CONFIG_FEATURES

# Common sources
SRCDIR=${SRCDIR}" src/common"

# Crypto sources
SRCDIR=${SRCDIR}" src/crypto/aes src/crypto/sha2 src/crypto/sha3"

# Protocols
SRCDIR=${SRCDIR}" src/kem \
                  src/sig"

#Set the default compilation flags
SET_AM_CFLAGS

AC_SUBST(SRCDIR)
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_AC_CONFIG_FILES_START
AC_CONFIG_FILES([Makefile
                 src/common/Makefile
                 src/kem/Makefile
                 src/crypto/sha3/Makefile
                 src/crypto/sha2/Makefile
                 src/crypto/aes/Makefile
                 src/sig/Makefile
                 src/sig/picnic/Makefile
                 src/sig/qtesla/Makefile
                 src/sig/dilithium/Makefile
                 src/sig/mqdss/Makefile
                 src/sig/sphincs/Makefile
                 src/kem/bike/Makefile
                 src/kem/kyber/Makefile
                 src/kem/newhope/Makefile
                 src/kem/ntru/Makefile
                 src/kem/saber/Makefile
                 src/kem/frodokem/Makefile
                 src/kem/sike/Makefile
                 tests/Makefile
                 ])
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_AC_CONFIG_FILES_END

AC_ARG_VAR([KEM_DEFAULT], [KEM to set at compile-time as OQS_KEM_DEFAULT, e.g. OQS_KEM_alg_sike_p434])
case "$KEM_DEFAULT" in
#(
"")
  AC_DEFINE([OQS_KEM_DEFAULT], [OQS_KEM_alg_sike_p434], [Set default KEM to SIKEp434])
;; 
#(
*)
  AC_DEFINE_UNQUOTED([OQS_KEM_DEFAULT], [$KEM_DEFAULT], [Set default KEM user-specified value])
;;
esac

AC_ARG_VAR(SIG_DEFAULT, [Signature scheme to set at compile-time as OQS_SIG_DEFAULT, e.g. OQS_SIG_alg_dilithium_2])
case "$SIG_DEFAULT" in
#(
"")
  AC_DEFINE([OQS_SIG_DEFAULT], [OQS_SIG_alg_dilithium_2], [Set default signature scheme to Dilithium2])
;; 
#(
*)
  AC_DEFINE_UNQUOTED([OQS_SIG_DEFAULT], [$SIG_DEFAULT], [Set default signature scheme user-specified value])
;;
esac

AC_DEFINE_UNQUOTED([OQS_COMPILE_CFLAGS], ["$CFLAGS $AM_CFLAGS"], [CFLAGS set during ./configure])
AC_DEFINE_UNQUOTED([OQS_COMPILE_LDFLAGS], ["$LDFLAGS"], [LDFLAGS set during ./configure])
AC_DEFINE_UNQUOTED([OQS_COMPILE_BUILD_TARGET], ["$build"], [build set during ./configure])

AC_OUTPUT
