# ============================================================================
#  adapted from 
#  https://www.gnu.org/software/autoconf-archive/ax_gcc_x86_cpu_supports.html
# ============================================================================
#
# SYNOPSIS
#
#   AX_X86_CPU_SUPPORTS(X86-INSTRUCTION-SET,
#     [ACTION-IF-FOUND],[ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   Checks if the host cpu supports X86-INSTRUCTION-SET. The instruction set
#   that can be tested are "mmx, popcnt, sse, sse2, sse3, sse4.1, sse4.2,
#   sse4a, avx, avx2, avx512f, fma, fma4, bmi, bmi2". If the instruction set
#   is supported by the host cpu, the C preprocessor macro
#   HAVE_XXX_INSTRUCTIONS is set to 1. The XXX is up-cased instruction case
#   with dot replaced by underscore. For example, the test for "sse4.2"
#   would export HAVE_SSE4_2_INSTRUCTIONS=1. This macro requires gcc
#   extended builtin function "__builtin_cpu_supports" to detect the cpu features.
#
#   If the test for the instruction set succeeded, the hook ACTION-IF-FOUND
#   would run. Otherwise the hook ACTION-IF-NOT-FOUND would run if
#   specified.
#
#   See also AX_CHECK_X86_FEATURES, which checks all the possible
#   instruction set and export the corresponding CFLAGS.
#
# LICENSE
#
#   Copyright (c) 2016 Felix Chern <idryman@gmail.com>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <https://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 3

AC_DEFUN([AX_X86_CPU_SUPPORTS],
  [AC_REQUIRE([AC_PROG_CC])
   AC_LANG_PUSH([C])
   AS_VAR_PUSHDEF([x86_feature], [AS_TR_SH([ax_cv_x86_cpu_supports_$1])])
   AC_CACHE_CHECK([for x86 $1 instruction support],
     [x86_feature],
     [AC_RUN_IFELSE(
       [AC_LANG_PROGRAM( [#include <stdlib.h> ],
       [ #if defined(__GNUC__) && !defined(__clang__)
           __builtin_cpu_init();
         #endif
         if (__builtin_cpu_supports("$1"))
           return 0;
         return 1;
        ])],
        [x86_feature=yes],
        [x86_feature=no]
     )]
   )
   AC_LANG_POP([C])
   AM_CONDITIONAL(AS_TR_SH(m4_toupper([HAVE_$1_INSTRUCTIONS])), [test x$x86_feature = xyes]) # ADDED BY OQS
   AS_VAR_IF([x86_feature],[yes],
         [AC_DEFINE(
           AS_TR_CPP([HAVE_$1_INSTRUCTIONS]),
           [1],
           [Define if $1 instructions are supported])
          $2],
          [$3]
         )
   AS_VAR_POPDEF([x86_feature])
])