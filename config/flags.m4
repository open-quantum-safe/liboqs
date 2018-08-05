# Set compilation flags
AC_DEFUN([SET_CFLAGS],
[
  AM_CFLAGS=${AM_CFLAGS}" -std=gnu11"

  # Basic set of errors
  AM_CFLAGS=${AM_CFLAGS}" -Wall -Wextra -Werror "
  
  #TODO: undisabled
  #AM_CFLAGS=${AM_CFLAGS}" -Wpedantic -Wcast-qual "

  # Add extra warnings
  AM_CFLAGS=${AM_CFLAGS}" -Wimplicit -Wunused -Wcomment -Wchar-subscripts -Wuninitialized "
  AM_CFLAGS=${AM_CFLAGS}" -Wshadow -Wcast-align -Wwrite-strings "
  AM_CFLAGS=${AM_CFLAGS}" -Wformat-security "

  # Add debug symbols
  AM_CFLAGS=${AM_CFLAGS}" -g "

  # For shared objects
  AM_CFLAGS=${AM_CFLAGS}" -fPIC "

  # Add default include directories
  AM_CFLAGS=${AM_CFLAGS}" -I`pwd`/include -I. "

  AM_COND_IF(USE_OPENSSL, [AM_CFLAGS=${AM_CFLAGS}" -I"${OPENSSL_DIR}"/include "])

  # Extra flags
  AM_CFLAGS=${AM_CFLAGS}" -DCONSTANT_TIME "

  # Optimization level
  AM_CFLAGS=${AM_CFLAGS}" -O3 "

  AM_CFLAGS=${AM_CFLAGS}" -fomit-frame-pointer "

  AM_COND_IF(USE_AES_NI, [AM_CFLAGS=${AM_CFLAGS}" -maes -msse2 "])

  AC_SUBST(AM_CFLAGS)
]
)
