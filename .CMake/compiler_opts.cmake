# SPDX-License-Identifier: MIT

# First we will determine the optimization target.
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

# Pedantic checks (-Wall, ...) are not enabled by default for Release
# builds such as to avoid future build errors introduced by currently
# unknown compiler warnings

include(CheckCCompilerFlag)
check_c_compiler_flag("-Wa,--noexecstack" CC_SUPPORTS_WA_NOEXECSTACK)

# This sets the equivalent of -Werror for supported compilers
# it can be overriden with --compile-no-warnings-as-errors
# https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
set(CMAKE_COMPILE_WARNING_AS_ERROR ${OQS_STRICT_WARNINGS})

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.18")
    include(CheckLinkerFlag)
    check_linker_flag(C "-Wl,-z,noexecstack" LD_SUPPORTS_WL_Z_NOEXECSTACK)
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    set(TMP_TESTDIR "${CMAKE_BINARY_DIR}/test_noexecstack")
    file(WRITE "${TMP_TESTDIR}/test.c" "int main() { return 0; }\n")
    try_compile(LD_SUPPORTS_WL_Z_NOEXECSTACK "${TMP_TESTDIR}" "${TMP_TESTDIR}/test.c" LINK_OPTIONS "-Wl,-z,noexecstack")
else()
    message(WARNING "Unable to check if '-Wl,-z,noexecstack' is supported.")
    set(LD_SUPPORTS_WL_Z_NOEXECSTACK FALSE)
endif()

set(OQS_OPT_FLAG "")
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    if(${OQS_DIST_BUILD})
          set(OQS_OPT_TARGET "generic")
    endif()

    if(CMAKE_CROSSCOMPILING AND OQS_OPT_TARGET STREQUAL "auto")
          set(OQS_OPT_TARGET "generic")
    endif()

    if(OQS_OPT_TARGET STREQUAL "generic")
        if(ARCH_S390X)
            # At least z9-109 is needed for 'stckf' in benchmarking code.
            # gcc's default is z900 (older than z9-109), clang's default and minimum is z10.
            # setting to z10 as sensible default.
            set(OQS_OPT_FLAG "-march=z10")
        else()
            # Assume sensible default like -march=x86-64, -march=armv8-a, etc.
	    if(ARCH_ARM64v8)
                set(OQS_OPT_FLAG "-march=armv8-a+crypto")
    	    else()
		set(OQS_OPT_FLAG "")
	    endif()
        endif()
    elseif(OQS_OPT_TARGET STREQUAL "auto")
      if(ARCH_X86_64)
          set(OQS_OPT_FLAG "-march=native")
      elseif(ARCH_ARM64v8 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
          set(OQS_OPT_FLAG "-mcpu=native")
      elseif(ARCH_ARM64v8 AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
          set(OQS_OPT_FLAG "-mcpu=native")
      elseif(ARCH_S390X)
          set(OQS_OPT_FLAG "-march=native")
      else()
          message(WARNING "Setting OQS_OPT_TARGET=AUTO may not produce optimized code on this system.")
      endif()
    else()
      if(ARCH_X86_64)
        set(OQS_OPT_FLAG "-march=${OQS_OPT_TARGET}")
      elseif(ARCH_ARM64v8 OR ARCH_ARM32v7)
        set(OQS_OPT_FLAG "-mcpu=${OQS_OPT_TARGET}")
      elseif(ARCH_S390X)
        set(OQS_OPT_FLAG "-march=${OQS_OPT_TARGET}")
      endif()
    endif()

    add_compile_options(${OQS_OPT_FLAG})

    # If this is not a dist build we also need to set the OQS_USE_[EXTENSION] flags
    if(NOT ${OQS_DIST_BUILD} AND NOT CMAKE_CROSSCOMPILING)
        include(${CMAKE_CURRENT_LIST_DIR}/gcc_clang_intrinsics.cmake)
    endif()
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  if(${OQS_STRICT_WARNINGS})
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wpedantic)
    add_compile_options(-Wno-unused-command-line-argument)
  endif()
    if(CC_SUPPORTS_WA_NOEXECSTACK)
        add_compile_options("-Wa,--noexecstack")
    endif()
    if(LD_SUPPORTS_WL_Z_NOEXECSTACK)
        add_link_options("-Wl,-z,noexecstack")
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if (CMAKE_USE_PTHREADS_INIT AND NOT OQS_EMBEDDED_BUILD)
        set(OQS_USE_PTHREADS ON)
    endif()

    if(${OQS_DEBUG_BUILD})
        if(OQS_ENABLE_TEST_CONSTANT_TIME_OPTIMIZED)
	    add_compile_options(-O3) # run constant-time tests on release code
	endif()
        add_compile_options(-g3)
        add_compile_options(-fno-omit-frame-pointer)
	if(${USE_COVERAGE})
            add_compile_options(-coverage)
            add_link_options(-coverage)
	endif()
        if(USE_SANITIZER STREQUAL "Address")
            add_compile_options(-fno-optimize-sibling-calls)
            add_compile_options(-fsanitize-address-use-after-scope)
            add_compile_options(-fsanitize=address)
            set(SANITIZER_LD_FLAGS "-fsanitize=address")
        elseif(USE_SANITIZER STREQUAL "Memory")
            add_compile_options(-fsanitize=memory)
            set(SANITIZER_LD_FLAGS "-fsanitize=memory")
        elseif(USE_SANITIZER STREQUAL "MemoryWithOrigins")
            add_compile_options(-fsanitize=memory)
            add_compile_options(-fsanitize-memory-track-origins)
            set(SANITIZER_LD_FLAGS "-fsanitize=memory")
        elseif(USE_SANITIZER STREQUAL "Undefined")
            add_compile_options(-fsanitize=undefined)
            if(EXISTS "${BLACKLIST_FILE}")
                add_compile_options(-fsanitize-blacklist=${BLACKLIST_FILE})
            endif()
            set(SANITIZER_LD_FLAGS "-fsanitize=undefined")
        elseif(USE_SANITIZER STREQUAL "Thread")
            add_compile_options(-fsanitize=thread)
            set(SANITIZER_LD_FLAGS "-fsanitize=thread")
        elseif(USE_SANITIZER STREQUAL "Leak")
            add_compile_options(-fsanitize=leak)
            set(SANITIZER_LD_FLAGS "-fsanitize=leak")
        endif()
    else()
        add_compile_options(-O3)
        add_compile_options(-fomit-frame-pointer)
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  if (NOT ${CMAKE_C_COMPILER_VERSION} VERSION_GREATER_EQUAL ${OQS_MINIMAL_GCC_VERSION})
     message(FATAL_ERROR "GCC version ${CMAKE_C_COMPILER_VERSION} below minimally required version ${OQS_MINIMAL_GCC_VERSION}.")
  endif()
  if(${OQS_STRICT_WARNINGS})
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wpedantic)
    add_compile_options(-Wstrict-prototypes)
    add_compile_options(-Wshadow)
    add_compile_options(-Wformat=2)
    add_compile_options(-Wfloat-equal)
    add_compile_options(-Wwrite-strings)
  endif()
    if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        if(CC_SUPPORTS_WA_NOEXECSTACK)
            add_compile_options("-Wa,--noexecstack")
        endif()
        if(LD_SUPPORTS_WL_Z_NOEXECSTACK)
            add_link_options("-Wl,-z,noexecstack")
        endif()
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if (CMAKE_USE_PTHREADS_INIT AND NOT OQS_EMBEDDED_BUILD)
        set(OQS_USE_PTHREADS ON)
    endif()

    if(${OQS_DEBUG_BUILD})
        add_compile_options (-Wstrict-overflow)
        add_compile_options(-ggdb3)
	if(${USE_COVERAGE})
            add_compile_options(-coverage)
            add_link_options(-coverage)
	endif()
    else()
        add_compile_options(-O3)
        add_compile_options(-fomit-frame-pointer)
        add_compile_options(-fdata-sections)
        add_compile_options(-ffunction-sections)
        if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            add_compile_options(-Wl,-dead_strip)
        else ()
            add_compile_options(-Wl,--gc-sections)
        endif ()
    endif()

    # workaround for gcc issues on ARM32 as per https://github.com/open-quantum-safe/liboqs/issues/1288
    if(ARCH_ARM32v7 AND (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "11.0.0"))
        add_compile_options(-fno-ipa-modref)
        add_compile_options(-fno-ipa-pure-const)
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Warning C4146 is raised when a unary minus operator is applied to an
    # unsigned type; this has nonetheless been standard and portable for as
    # long as there has been a C standard, and we need it for constant-time
    # computations. Thus, we disable that spurious warning.
    add_compile_options(/wd4146)
    # Need a larger stack for Classic McEliece
    add_link_options(/STACK:8192000)
    # bring compile options in line with openssl options; link otherwise fails
    add_compile_options(/MT)
endif()

if(MINGW OR MSYS OR CYGWIN)
    set(OQS_USE_PTHREADS OFF)
    # Apply -Wno-maybe-uninitialized only for GCC
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wno-maybe-uninitialized)
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13.0")
        add_link_options(-Wl,--stack,16777216)
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,16777216")
    endif()
endif()

if(CMAKE_C_IMPLICIT_LINK_DIRECTORIES MATCHES "alpine-linux-musl")
    add_link_options(-Wl,-z,stack-size=16777216)
endif()
