# Set compilation flags
AC_DEFUN([SET_AM_CFLAGS],
[
  AM_CFLAGS=${AM_CFLAGS}" -std=gnu11"

  # Basic set of errors
  AM_CFLAGS=${AM_CFLAGS}" -Wall -Wextra -Werror "

  #TODO: undisabled
  #AM_CFLAGS=${AM_CFLAGS}" -Wpedantic -Wcast-qual "

  # Add extra warnings
  AM_CFLAGS=${AM_CFLAGS}" -Wimplicit -Wunused -Wcomment -Wchar-subscripts -Wuninitialized -Wstrict-prototypes"
  AM_CFLAGS=${AM_CFLAGS}" -Wshadow -Wformat-security -Wwrite-strings "

  # Disable, due to bugs in GCC 4.8/4.9
  AM_CFLAGS=${AM_CFLAGS}" -Wno-missing-braces -Wno-missing-field-initializers"
  # Add debug symbols
  AM_CFLAGS=${AM_CFLAGS}" -g "

  # For shared objects
  AM_CFLAGS=${AM_CFLAGS}" -fPIC "

  # Hidding all unneeded symbols.
  AM_CFLAGS=${AM_CFLAGS}" -fvisibility=hidden "

  # Add default include directories
  AM_CFLAGS=${AM_CFLAGS}" -I`pwd`/include -I. "

  AM_COND_IF(USE_OPENSSL, [AM_CFLAGS=${AM_CFLAGS}" -I"${OPENSSL_DIR}"/include "])

  # Optimization level
  AM_CFLAGS=${AM_CFLAGS}" -O3 "

  # Sanitizer build
  AC_ARG_WITH(sanitizer,
    [  --with-sanitizer        enable the ASAN and UBSAN],
    with_sanitizer=$withval,
    with_sanitizer=yes)
  AS_IF([test "x$with_sanitizer" = "xyes"], [
    AM_CFLAGS=${AM_CFLAGS}" -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer"
  ], [
    AM_CFLAGS=${AM_CFLAGS}" -fomit-frame-pointer "
  ])

  # Extra flags
  AM_CFLAGS=${AM_CFLAGS}" -DCONSTANT_TIME "

  AM_CONDITIONAL(CONSTANT_TIME,[true])
  AC_SUBST(AM_CFLAGS)
]
)
