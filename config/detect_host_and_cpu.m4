
AC_DEFUN([DETECT_HOST_AND_CPU], [
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
  AM_CONDITIONAL([ON_LINUX], [test "x$linux" = xtrue])
  AC_SUBST(ON_LINUX)
  AC_SUBST(ON_DARWIN)

  # Enable assembly optimizations here
  # Appearenly asm optimizations do not work well with darwin
  AM_COND_IF([ON_LINUX], [
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
      *)
          #Default Case
          AC_MSG_ERROR([Your CPU is not currently supported])
          ;;
      esac
  

    AM_CONDITIONAL([X86_64], [test "x$x86_64" = xtrue])
    AM_CONDITIONAL([ARM64], [test "x$arm64" = xtrue])
    AM_CONDITIONAL([ARM], [test "x$arm" = xtrue])
    AC_SUBST(ARM)
    AC_SUBST(ARM64)
    AC_SUBST(X86_64)

    #Check if further x86 optimizations are available to use (e.g., avx/avx2/bmi).
    AM_COND_IF([X86_64], [AX_CHECK_X86_FEATURES])
  ])
])
