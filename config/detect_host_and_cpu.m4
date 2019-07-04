
AC_DEFUN([DETECT_HOST_AND_CPU], [
  AC_CANONICAL_HOST

  # Check for which host we are on and setup a few things
  # specifically based on the host
  case $host_os in
    darwin* )
          darwin=true
          AM_LIBTOOLFLAGS=${AM_LIBTOOLFLAGS}" -no_warning_for_no_symbols "
          ;;
    linux-android*)
          AM_CFLAGS=${AM_CFLAGS}" -pie "
          linux=true
          ;;
    linux*)
          linux=true
          ;;
    openbsd*)
          openbsd=true
          ;;
      *)
          #Default Case
          AC_MSG_ERROR([Your platform is not currently supported])
          ;;
  esac

  AM_CONDITIONAL([ON_DARWIN], [test "x$darwin" = xtrue])
  AM_CONDITIONAL([ON_LINUX], [test "x$linux" = xtrue])
  AM_CONDITIONAL([ON_OPENBSD], [test "x$openbsd" = xtrue])

  # Enable assembly optimizations here
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
        arm=true
        ;;
    *)
        #Default Case
        AC_MSG_ERROR([Your CPU is not currently supported])
        ;;
  esac
  
  AM_CONDITIONAL([X86_64], [test "x$x86_64" = xtrue])
  AM_CONDITIONAL([ARM64], [test "x$arm64" = xtrue])
  AM_CONDITIONAL([ARM], [test "x$arm" = xtrue])

  AC_CACHE_CHECK([for GNUC compiler],
    [gcc_cv_compiler],
    [AC_RUN_IFELSE(
    [AC_LANG_PROGRAM( [],
    [ #if defined(__GNUC__) && !defined(__clang__)
        return 0;
      #else
        return 1;
      #endif
    ])],
    [gcc_cv_compiler=true],
    [gcc_cv_compiler=false]
    )]
  )

])
