# SPDX-License-Identifier: MIT
include(CheckCCompilerFlag)

#! List of supported architecture
#
# Each string is then use for iterating over the list
# `OQS_ARCHITECTURE_PROCS_<string>`.
# When a processor is matched, the variable `OQS_ARCH_<string>` is set to ON
list(APPEND OQS_SUPPORTED_ARCHITECTURES
  "X86_64"
  "X86"
  "ARM64v8"
  "ARM32v7"
  "PPC64LE"
  "PPC64"
  "S390X"
)

#! Lists of processors for each supported architectures.
# Each string is compared against `CMAKE_SYSTEM_PROCESSOR`.
#
# The first string of each list is used to define `OQS_ARCH`
list(APPEND OQS_ARCHITECTURE_PROCS_X86_64   "x86_64" "amd64")
list(APPEND OQS_ARCHITECTURE_PROCS_X86      "i586" "i686")
list(APPEND OQS_ARCHITECTURE_PROCS_ARM64v8  "arm64v8" "arm64v8" "aarch64" "arm64")
list(APPEND OQS_ARCHITECTURE_PROCS_ARM32v7  "arm32v7" "arm32v7" "armel" "armhf" "armv7")
list(APPEND OQS_ARCHITECTURE_PROCS_PPC64LE  "ppc64le" "ppc64le" "powerpc64le")
list(APPEND OQS_ARCHITECTURE_PROCS_PPC64    "ppc64" "powerpc64")
list(APPEND OQS_ARCHITECTURE_PROCS_S390X    "s390x")

#! Define the architecture depending on `CMAKE_SYSTEM_PROCESSOR`
#
# This function defines and export `OQS_ARCH` and `OQS_ARCH_<arch>` where
# `<arch> is an item from `OQS_SUPPORTED_ARCHITECTURES`
function(oqs_define_architecture)
  set(OQS_ARCH "" PARENT_SCOPE)
  string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" selfproc)
  foreach(arch ${OQS_SUPPORTED_ARCHITECTURES})
    set(proclist OQS_ARCHITECTURE_PROCS_${arch})
    list(GET ${proclist} 0 archname)

    list(FIND ${proclist} ${selfproc} found)
    if(found EQUAL -1)
      continue()
    endif()

    set(OQS_ARCH ${archname} PARENT_SCOPE)
    set(OQS_ARCH_${arch} ON PARENT_SCOPE)
    if(OQS_DIST_BUILD)
      set(OQS_DIST_${arch}_BUILD ON)
    endif()
    return()
  endforeach()
endfunction()

#! Format a flag name
#
# String the frist - if exists, and replace any symbols in [-+=,] by _
#
# \arg:fname Flag name
# \arg:outvar Out variable
function(oqs_format_flag fname outvar)
  string(TOUPPER "${fname}" fname_u)
  string(SUBSTRING "${fname_u}" 0 1 firstchar)
  if(${firstchar} STREQUAL "-")
    string(SUBSTRING "${fname_u}" 1 -1 fname_lstripped)
  else()
    set(fname_lstripped "${fname_u}")
  endif()
  string(REGEX REPLACE "[-+=,]" "_" fmtdflag "${fname_lstripped}")
  set("${outvar}" "${fmtdflag}" PARENT_SCOPE)
endfunction()

#! Test if a C flag is supported by the current compiler
#
# \arg:flag Flag to test
# \arg:outvar Out variable, set to YES if supported, otherwise NO
function(oqs_test_c_flag flag outvar)
  oqs_format_flag("${flag}" fmtflag)
  set(supportflag OQS_C_SUPPORTS_${fmtflag})
  if(NOT DEFINED ${supportflag})
    check_c_compiler_flag("${flag}" ${supportflag})
    if(${supportflag})
      message(DEBUG "Compiler flag '${flag}' is supported by ${CMAKE_C_COMPILER}")
    else()
      message(DEBUG "Compiler flag '${flag}' is NOT supported by ${CMAKE_C_COMPILER}")
    endif()
  endif()
  if (${supportflag})
    set("${outvar}" YES PARENT_SCOPE)
  else()
    set("${outvar}" NO PARENT_SCOPE)
  endif()
endfunction()

#! Add one of several compiler flags
#
# \argn: Flags to add
# \arg:target Target to apply the C flags on
# \arg:required Are the flags required (REQUIRED or OPTIONAL)
# \arg:policy Flag policy (see https://cmake.org/cmake/help/latest/command/target_compile_options.html#arguments)
function(oqs_add_c_flags target required policy)
  foreach(flag ${ARGN})
    oqs_test_c_flag("${flag}" supported)
    if(supported)
      target_compile_options(${target} ${policy} "${flag}")
    elseif(${required} STREQUAL REQUIRED)
      message(FATAL_ERROR "Compiler flag '${flag}' is not supported by ${CMAKE_C_COMPILER}")
    endif()
  endforeach()
endfunction()

define_property(
  TARGET
  PROPERTY SANITIZERS
  BRIEF_DOCS "Sanitizers options to apply"
  FULL_DOCS "List of sanitizer options to enable using -fsanitize="
)

#! Enable sanitizer options
#
# \argn: Sanitizer options to add
# \arg:target Target to apply the sanitizer options on
# \arg:required Are the sanitizers options required (REQUIRED or OPTIONAL)
# \arg:policy Policy (see https://cmake.org/cmake/help/latest/command/target_compile_options.html#arguments)
function(oqs_add_sanitizers target required policy)
  get_target_property(sanitizers ${target} SANITIZERS)
  foreach(san ${ARGN})
    oqs_test_c_flag("-fsanitize=${san}" supported)
    if(supported)
      list(APPEND sanitizers "${san}")
    elseif(${required} STREQUAL REQUIRED)
      message(FATAL_ERROR "Sanitizer option '${san}' is not supported by ${CMAKE_C_COMPILER}")
    endif()
  endforeach()
  set_target_properties(${target} PROPERTIES SANITIZERS "${sanitizers}")
endfunction()

#! Apply the sanitizers options
#
# After building the list of sanitizers options using `oqs_add_sanitizers`,
# apply the `-fsanitize=<list>` to the given target
# \arg:policy Policy (see https://cmake.org/cmake/help/latest/command/target_compile_options.html#arguments)
function(oqs_apply_sanitizers target policy)
  get_target_property(sanitizerslist ${target} SANITIZERS)
  if(sanitizerslist)
    list(JOIN sanitizerslist "," sanitizers)
    target_compile_options(${target} ${policy} "-fsanitize=${sanitizers}")
    target_link_options(${target} PRIVATE "-fsanitize=${sanitizers}")
  endif()
endfunction()

#! Add one or several linker flags
#
# \argn: Linker flags to set
# \arg:target Target
# \arg:policy Policy (see https://cmake.org/cmake/help/latest/command/target_compile_options.html#arguments)
function(oqs_add_ld_flags target policy)
  if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13.0")
    target_link_options(${target} ${policy} ${ARGN})
  else()
    list(JOIN ${ARGN} " " flags)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flags}")
  endif()
endfunction()

oqs_define_architecture()
if(NOT OQS_ARCH)
  if(OQS_PERMIT_UNSUPPORTED_ARCHITECTURE)
    message(WARNING "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR})
    message(WARNING "Compilation on an unsupported processor should only be used for testing, as it may result an insecure configuration, for example due to variable-time instructions leaking secret information.")
    # disable SIKE whose build fails on unsupported platforms
    set(OQS_ENABLE_KEM_SIKE OFF)
    set(OQS_ENABLE_KEM_SIDH OFF)
  else()
    message(FATAL_ERROR "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR} ". Override by setting OQS_PERMIT_UNSUPPORTED_ARCHITECTURE=ON")
  endif()
else()
  message(STATUS "Building liboqs for ${OQS_ARCH}")
endif()

# Now we will determine the optimization target.
#
# If OQS_DIST_BUILD=ON we need to target a generic CPU for any code
# that is not protected by runtime CPU feature detection.
#
# If OQS_DIST_BUILD=OFF then we will optimize all code for the CPU
# specified by OQS_OPT_TARGET.
#
# If OQS_OPT_TARGET=auto we target the current CPU.
# If OQS_OPT_TARGET=generic we target a generic CPU.
# Otherwise we target the specified CPU.
set(OQS_OPT_FLAG "")
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    if(${OQS_DIST_BUILD})
          set(OQS_OPT_TARGET "generic")
    endif()

    if(CMAKE_CROSSCOMPILING AND OQS_OPT_TARGET STREQUAL "auto")
          set(OQS_OPT_TARGET "generic")
    endif()

    if(OQS_OPT_TARGET STREQUAL "generic")
        if(OQS_ARCH_S390X)
            # At least z9-109 is needed for 'stckf' in benchmarking code.
            # gcc's default is z900 (older than z9-109), clang's default and minimum is z10.
            # setting to z10 as sensible default.
            set(OQS_OPT_FLAG "-march=z10")
        else()
            # Assume sensible default like -march=x86-64, -march=armv8-a, etc.
            set(OQS_OPT_FLAG "")
        endif()
    elseif(OQS_OPT_TARGET STREQUAL "auto")
      if(OQS_ARCH_X86_64)
          set(OQS_OPT_FLAG "-march=native")
      elseif(OQS_ARCH_ARM64v8 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
          set(OQS_OPT_FLAG "-mcpu=native")
      elseif(OQS_ARCH_ARM64v8 AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
          set(OQS_OPT_FLAG "-mcpu=native")
      elseif(OQS_ARCH_S390X)
          set(OQS_OPT_FLAG "-march=native")
      else()
          message(WARNING "Setting OQS_OPT_TARGET=AUTO may not produce optimized code on this system.")
      endif()
    else()
      if(OQS_ARCH_X86_64)
        set(OQS_OPT_FLAG "-march=${OQS_OPT_TARGET}")
      elseif(OQS_ARCH_ARM64v8 OR OQS_ARCH_ARM32v7)
        set(OQS_OPT_FLAG "-mcpu=${OQS_OPT_TARGET}")
      elseif(OQS_ARCH_S390X)
        set(OQS_OPT_FLAG "-march=${OQS_OPT_TARGET}")
      endif()
    endif()

    oqs_add_c_flags(oqs REQUIRED PRIVATE ${OQS_OPT_FLAG})

    # If this is not a dist build we also need to set the OQS_USE_[EXTENSION] flags
    if(NOT OQS_DIST_BUILD)
        include(${CMAKE_CURRENT_LIST_DIR}/gcc_clang_intrinsics.cmake)
    endif()
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    oqs_add_c_flags(oqs REQUIRED PRIVATE
      "-Werror"
      "-Wall"
      "-Wextra"
      "-Wpedantic"
      "-Wno-unused-command-line-argument"
    )
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack")

    if(NOT ${OQS_BUILD_ONLY_LIB})
        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)
        set(OQS_USE_PTHREADS_IN_TESTS 1)
    endif()

    if(${OQS_DEBUG_BUILD})
        oqs_add_c_flags(oqs REQUIRED PRIVATE "-g3" "-fno-omit-frame-pointer")
        oqs_add_ld_flags(oqs PRIVATE "-fno-omit-frame-pointer")
        if(USE_SANITIZER STREQUAL "Address")
            oqs_add_c_flags(oqs REQUIRED PRIVATE
              "-fno-optimize-sibling-calls"
              "-fsanitize-address-use-after-scope"
            )
            oqs_add_sanitizers(oqs REQUIRED "address")
        elseif(USE_SANITIZER STREQUAL "Memory")
            oqs_add_sanitizers(oqs REQUIRED "address" "memory")
        elseif(USE_SANITIZER STREQUAL "MemoryWithOrigins")
            oqs_add_sanitizers(oqs REQUIRED "address" "memory")
            oqs_add_c_flags(oqs REQUIRED PRIVATE "-fsanitize-memory-track-origins")
        elseif(USE_SANITIZER STREQUAL "Undefined")
            oqs_add_sanitizers(oqs REQUIRED "undefined")
            if(EXISTS "${BLACKLIST_FILE}")
                oqs_add_c_flags(oqs REQUIRED PRIVATE "-fsanitize-blacklist=${BLACKLIST_FILE}")
            endif()
        elseif(USE_SANITIZER STREQUAL "Thread")
            oqs_add_sanitizers(oqs REQUIRED "thread")
        elseif(USE_SANITIZER STREQUAL "Leak")
            oqs_add_sanitizers(oqs REQUIRED "leak")
        endif()
        oqs_apply_sanitizers(oqs PRIVATE)
    else()
        oqs_add_c_flags(oqs REQUIRED PRIVATE "-O3" "-fomit-frame-pointer")
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    oqs_add_c_flags(oqs REQUIRED PRIVATE
      "-Werror"
      "-Wall"
      "-Wextra"
      "-Wpedantic"
      "-Wno-unused-command-line-argument"
      "-Wstrict-prototypes"
      "-Wshadow"
      "-Wformat=2"
      "-Wfloat-equal"
      "-Wwrite-strings"
    )
    if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack")
    endif()

    if(NOT ${OQS_BUILD_ONLY_LIB})
        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)
        set(OQS_USE_PTHREADS_IN_TESTS 1)
    endif()

    if(${OQS_DEBUG_BUILD})
        oqs_add_c_flags(oqs REQUIRED PRIVATE
          "-Wstrict-overflow"
          "-ggdb3"
        )
    else()
        oqs_add_c_flags(oqs REQUIRED PRIVATE
          "-O"
          "-fomit-frame-pointer"
          "-fdata-sections"
          "-ffunction-sections"
        )
        if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            oqs_add_c_flags(oqs REQUIRED PRIVATE "-Wl,-dead_strip")
        else()
            oqs_add_c_flags(oqs REQUIRED PRIVATE "-Wl,--gc-sections")
        endif()
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Warning C4146 is raised when a unary minus operator is applied to an
    # unsigned type; this has nonetheless been standard and portable for as
    # long as there has been a C standard, and we need it for constant-time
    # computations. Thus, we disable that spurious warning.
    oqs_add_c_flags(oqs REQUIRED PRIVATE "/wd4146")
    # Need a larger stack for Classic McEliece
    oqs_add_ld_flags(oqs PRIVATE "/STACK:8192000")
    # bring compile options in line with openssl options; link otherwise fails
    oqs_add_c_flags(oqs REQUIRED PRIVATE "/MT")
endif()

if(MINGW OR MSYS OR CYGWIN)
    oqs_add_c_flags(oqs REQUIRED PRIVATE "-Wno-maybe-uninitialized")
    oqs_add_ld_flags(oqs PRIVATE "--stack,16777216")
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13.0")
        target_link_options(oqs PUBLIC "--stack,16777216")
    endif()
endif()

if(CMAKE_C_IMPLICIT_LINK_DIRECTORIES MATCHES "alpine-linux-musl")
    oqs_add_ld_flags(oqs PUBLIC "-z stack-size=16777216")
endif()
