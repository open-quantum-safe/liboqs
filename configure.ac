#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Init
AC_INIT([liboqs], [0.1.0], [https://github.com/open-quantum-safe/liboqs/issues], [liboqs-master], [https://openquantumsafe.org/])

AM_INIT_AUTOMAKE([subdir-objects no-dependencies])
AM_PROG_LIBTOOL

# Numeric release version identifier:
# MNNFFPPS: major minor fix patch status
# The status nibble has one of the values 0 for development, 1 to e for release candidates 1 to 4, and f for release
# For example:
# 1.2.3-dev      0x10203000
# 1.2.3-rc1      0x10203001
# 1.2.3-rc2-dev  0x10203002
# 1.2.3-rc2      0x10203002 (same as ...rc2-dev)
# 1.2.3          0x1020300f
# 1.2.3a         0x1020301f
AC_DEFINE([OQS_VERSION_NUMBER],[0x0010000fL],[Numeric liboqs version])
AC_DEFINE([OQS_VERSION_TEXT],["0.1.0"],[Text liboqs version])
AC_DEFINE([OQS_MASTER_BRANCH],[],[liboqs branch])

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
SRCDIR=${SRCDIR}" src/crypto/aes src/crypto/sha3"

# Protocols
SRCDIR=${SRCDIR}" src/kem \
                  src/sig"

#Set the default compilation flags
SET_AM_CFLAGS

AC_SUBST(SRCDIR)
AC_CONFIG_FILES([Makefile
                 src/common/Makefile
                 src/kem/Makefile
                 src/crypto/sha3/Makefile
                 src/crypto/aes/Makefile
                 src/sig/Makefile
                 src/sig/picnic/Makefile
                 src/sig/qtesla/Makefile
                 src/sig/dilithium/Makefile
                 src/kem/bike/Makefile
                 src/kem/frodokem/Makefile
                 src/kem/sike/Makefile
                 src/kem/newhopenist/Makefile
                 src/kem/kyber/Makefile
                 tests/Makefile
                 ])

AC_OUTPUT
