#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
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


ARG_DISBL_SET([bcns15-1024], [disable bcns15 with 1024 bits (default).])

AM_CONDITIONAL(bcns15_1024, [test "x$bcns15_1024" = xtrue])

AM_COND_IF([bcns15_1024], [SRCDIR=src/kex_rlwe_bcns15/])

AM_COND_IF([bcns15_1024], [WANTEDLIBS=src/kex_rlwe_bcns15/libbcns15.la])

WANTEDLIBS=${WANTEDLIBS}" src/kex/libkex.la"
SRCDIR=${SRCDIR}" src/kex src/rand"


AC_SUBST(WANTEDLIBS)
AC_SUBST(SRCDIR)




AC_CONFIG_FILES([Makefile
                 src/kex/Makefile
                 src/kex_rlwe_bcns15/Makefile
                 src/rand/Makefile])
AC_OUTPUT
