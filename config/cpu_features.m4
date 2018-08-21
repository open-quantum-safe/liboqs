
AC_DEFUN([AX_X86_CPU_SUPPORTS],
  [AC_REQUIRE([AC_PROG_CC])
   AM_COND_IF([X86_64],[
   AC_LANG_PUSH([C])
   AS_VAR_PUSHDEF([x86_feature], [AS_TR_SH([ax_cv_x86_cpu_supports_$1])])
   AC_CACHE_CHECK([for x86 $1 instruction support],
     [x86_feature],
     [AC_RUN_IFELSE(
       [AC_LANG_PROGRAM( [#include <stdlib.h> ],
       [ #if defined(__GNUC__) && !defined(__clang__)
           __builtin_cpu_init ();
         #endif
         if (__builtin_cpu_supports("$1"))
         {
           return 0;
         }
         return 1;
        ])],
        [x86_feature=yes],
        [x86_feature=no]
     )]
   )
   AC_LANG_POP([C])
   AM_CONDITIONAL(m4_toupper([USE_$1_INSTRUCTIONS]), [test x$x86_feature = xyes])
   AS_VAR_IF([x86_feature],[yes],
         [AC_DEFINE(
            AS_TR_CPP([USE_$1_INSTRUCTIONS]),
            [1],
            [Define if $1 instructions are supported])
          $2],
          [$3]
         )
   AS_VAR_POPDEF([x86_feature])
   ],
   [AM_CONDITIONAL(m4_toupper([USE_$1_INSTRUCTIONS]), [false])]
   )
])

AC_DEFUN([AX_CHECK_X86_FEATURES],
 [m4_foreach_w(
   [ax_x86_feature],
   [mmx popcnt sse sse2 sse3 sse4a sse5 avx avx2 avx512f fma fma4 bmi bmi2
    avx512bw avx512cd avx512dq avx512er avx512ifma avx512pf avx512vbmi],
   [AX_X86_CPU_SUPPORTS(ax_x86_feature,
     [X86_FEATURE_CFLAGS="$X86_FEATURE_CFLAGS -m[]ax_x86_feature"],
     [])
  ])

  AC_SUBST([X86_FEATURE_CFLAGS])
  m4_ifval([$1],[$1],
    [CFLAGS="$CFLAGS $X86_FEATURE_CFLAGS"])
  $2
])
