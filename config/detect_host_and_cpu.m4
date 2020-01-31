
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
        AM_CPPFLAGS=${AM_CPPFLAGS}" -march=x86-64"
        x86_64=true
        ;;
    aarch64* )
       AM_CPPFLAGS=${AM_CPPFLAGS}" -march=armv8-a+crc"
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
  
  AM_CONDITIONAL([X86], [test "x$x86" = xtrue])
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
  
  AX_CHECK_X86_FEATURES
  
  # adapted from https://stackoverflow.com/a/20337780
  AC_MSG_CHECKING([if build is on Raspberry Pi])
  if test -f "/proc/cpuinfo"; then
    grep --silent Raspberry /proc/cpuinfo
    if test "x$?" == "x0"; then
      on_rpi="yes"
    else
      on_rpi="no"
    fi
  else
    on_rpi="no"
  fi
  AC_MSG_RESULT($on_rpi)
  if test "x$on_rpi" == "xyes"; then
    AC_DEFINE(USE_RASPBERRY_PI, 1, "Define to 1 when on a Raspberry Pi")
  fi

])
