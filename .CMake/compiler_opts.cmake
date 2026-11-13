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

# Compiler warnings are enabled by default for GCC and Clang (see "Compiler
# warnings" below). OQS_STRICT_WARNINGS additionally treats warnings as errors.
# That is meant for development and CI only, so that a newer compiler adding
# warnings cannot break the build of an existing release.

include(CheckCCompilerFlag)
check_c_compiler_flag("-Wa,--noexecstack" CC_SUPPORTS_WA_NOEXECSTACK)

# Treat warnings as errors (-Werror, /WX, ...) via CMake's COMPILE_WARNING_AS_ERROR
# CMake before 3.24 ignores it, which would silently drop -Werror, so refuse to configure.
# https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
if(OQS_STRICT_WARNINGS AND CMAKE_VERSION VERSION_LESS "3.24")
    message(FATAL_ERROR "OQS_STRICT_WARNINGS requires CMake 3.24 or newer (found ${CMAKE_VERSION}). Upgrade CMake or configure with -DOQS_STRICT_WARNINGS=OFF.")
endif()
set(CMAKE_COMPILE_WARNING_AS_ERROR ${OQS_STRICT_WARNINGS})

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.18")
    include(CheckLinkerFlag)
    check_linker_flag(C "-Wl,-z,noexecstack" LD_SUPPORTS_WL_Z_NOEXECSTACK)
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    set(TMP_TESTDIR "${CMAKE_BINARY_DIR}/test_noexecstack")
    file(WRITE "${TMP_TESTDIR}/test.c" "int main() { return 0; }\n")
    try_compile(
        LD_SUPPORTS_WL_Z_NOEXECSTACK
        "${TMP_TESTDIR}"
        "${TMP_TESTDIR}/test.c"
        LINK_OPTIONS "-Wl,-z,noexecstack"
    )
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
            message(
                WARNING
                "Setting OQS_OPT_TARGET=AUTO may not produce optimized code on this system."
            )
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
    add_compile_options(-Wno-unused-command-line-argument)
    if(CC_SUPPORTS_WA_NOEXECSTACK)
        add_compile_options("-Wa,--noexecstack")
    endif()
    if(LD_SUPPORTS_WL_Z_NOEXECSTACK)
        add_link_options("-Wl,-z,noexecstack")
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT AND NOT OQS_EMBEDDED_BUILD)
        set(OQS_USE_PTHREADS ON)
    endif()

    if(${OQS_DEBUG_BUILD})
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
        add_compile_options(-fomit-frame-pointer)
    endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if(
        NOT ${CMAKE_C_COMPILER_VERSION}
            VERSION_GREATER_EQUAL
            ${OQS_MINIMAL_GCC_VERSION}
    )
        message(
            FATAL_ERROR
            "GCC version ${CMAKE_C_COMPILER_VERSION} below minimally required version ${OQS_MINIMAL_GCC_VERSION}."
        )
    endif()
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        if(CC_SUPPORTS_WA_NOEXECSTACK)
            add_compile_options("-Wa,--noexecstack")
        endif()
        if(LD_SUPPORTS_WL_Z_NOEXECSTACK)
            add_link_options("-Wl,-z,noexecstack")
        endif()
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT AND NOT OQS_EMBEDDED_BUILD)
        set(OQS_USE_PTHREADS ON)
    endif()

    if(${OQS_DEBUG_BUILD})
        add_compile_options(-Wstrict-overflow)
        add_compile_options(-ggdb3)
        if(${USE_COVERAGE})
            add_compile_options(-coverage)
            add_link_options(-coverage)
        endif()
    else()
        add_compile_options(-fomit-frame-pointer)
        add_compile_options(-fdata-sections)
        add_compile_options(-ffunction-sections)
        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            add_compile_options(-Wl,-dead_strip)
        else()
            add_compile_options(-Wl,--gc-sections)
        endif()
    endif()

    # workaround for gcc issues on ARM32 as per https://github.com/open-quantum-safe/liboqs/issues/1288
    if(
        ARCH_ARM32v7
        AND (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "11.0.0")
    )
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
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,16777216"
        )
    endif()
endif()

# Hardened compile and link options, following the OpenSSF Compiler Options
# Hardening Guide for C and C++ (as of the 2026-08-20 revision):
# https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++.html
#
# Deliberately not used from the guide:
#   -Wl,-z,nodlopen            (liboqs is loaded with dlopen() by language wrappers.)
#   -D_GLIBCXX_ASSERTIONS      (C++ only)
#   -D_LIBCPP_HARDENING_MODE   (C++ only)
#   -fPIE -pie, -fPIC          (Covered by CMAKE_POSITION_INDEPENDENT_CODE.)
#   -Wl,-z,noexecstack         (Already added above)
#   -O2                        (Set by CMAKE_BUILD_TYPE)

# Table 1: Compile-time hardening options
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    # Embedded builds (e.g. Zephyr) keep the toolchain's own warning set,
    # since Zephyr's -Werror would turn these additional warnings into errors.
    if(NOT OQS_EMBEDDED_BUILD)
        # From the guide
        add_compile_options(
            -Wall
            -Wextra
            -Wformat
            -Wformat=2
            # TODO: Conversion warnings lead to a huge number of warnings.
            # Enabling these are tracked by: https://github.com/open-quantum-safe/liboqs/issues/2700
            # -Wconversion
            # -Wsign-conversion
            -Wimplicit-fallthrough
        )
        # liboqs additions, not part of the guide
        add_compile_options(
            -Wpedantic
            -Wstrict-prototypes
            -Wshadow
            -Wfloat-equal
            -Wwrite-strings
        )
        # From the guide, GCC only
        if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
            add_compile_options(-Wtrampolines)
            if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "12")
                add_compile_options(-Wbidi-chars=any)
            endif()
        endif()
    endif()

    # Obsolete or dangerous constructs, which are errors in every build.
    # Unlike a blanket -Werror, these checks are stable across compiler releases.
    add_compile_options(
        -Werror=format-security
        -Werror=implicit
        -Werror=incompatible-pointer-types
        -Werror=int-conversion
    )
endif()

# Table 2: Runtime hardening options
# These modify the properties of the generated binary to enhance security
# Because this can affect performance, these are opt-in via OQS_HARDENED_COMPILE_OPTS (default OFF)
if(OQS_HARDENED_COMPILE_OPTS AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    # Supported by every GCC and Clang that liboqs supports.
    add_compile_options(
        -fno-delete-null-pointer-checks
        -fno-strict-overflow
        -fno-strict-aliasing
    )
    if(OQS_USE_PTHREADS)
        add_compile_options(-fexceptions)
    endif()
    # Needs __stack_chk_guard/__stack_chk_fail from the runtime.
    if(NOT OQS_EMBEDDED_BUILD)
        add_compile_options(-fstack-protector-strong)
    endif()

    # Newer flags whose support depends on the compiler version and target.
    # Each is added only if the compiler accepts it.
    set(_oqs_checked_flags
        -fstack-clash-protection
        -fstrict-flex-arrays=3
        -fzero-init-padding-bits=all
    )
    if(ARCH_X86_64)
        list(APPEND _oqs_checked_flags -fcf-protection=full)
    elseif(ARCH_ARM64v8)
        list(APPEND _oqs_checked_flags -mbranch-protection=standard)
    endif()
    # Clang before 16 (AppleClang before 15) miscompiles this flag, so a
    # support check is not enough.
    if(
        (ARCH_X86_64 OR ARCH_ARM64v8)
        AND NOT (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "16")
        AND NOT (CMAKE_C_COMPILER_ID STREQUAL "AppleClang" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "15")
    )
        list(APPEND _oqs_checked_flags -fzero-call-used-regs=used-gpr)
    endif()
    foreach(_flag IN LISTS _oqs_checked_flags)
        string(MAKE_C_IDENTIFIER "CC_SUPPORTS${_flag}" _var)
        check_c_compiler_flag("${_flag}" ${_var})
        if(${_var})
            add_compile_options("${_flag}")
        endif()
    endforeach()

    # Zero-initialize locals in release builds
    # Skip for sanitizer and constant-time builds, since these are meant to catch uninitialized reads
    if(NOT USE_SANITIZER AND NOT OQS_ENABLE_TEST_CONSTANT_TIME)
        check_c_compiler_flag(-ftrivial-auto-var-init=zero CC_SUPPORTS_ftrivial_auto_var_init)
        if(CC_SUPPORTS_ftrivial_auto_var_init)
            if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                # In Debug builds, use `=pattern` to better catch uninitialized reads
                add_compile_options(-ftrivial-auto-var-init=pattern)
            else()
                # As per guide, use `=zero` for release builds
                add_compile_options(-ftrivial-auto-var-init=zero)
            endif()
        endif()
    endif()

    # _FORTIFY_SOURCE needs optimization (-O1 or higher), so skip Debug.
    if(NOT OQS_EMBEDDED_BUILD AND NOT MINGW AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        # As per guide, unset and reset to avoid potential conflicts
        add_compile_options(-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3)
    endif()

    # These linker options are ELF features exclusively, so exclude non-ELF platforms.
    if(NOT APPLE AND NOT WIN32 AND NOT CYGWIN AND NOT MSYS AND NOT OQS_EMBEDDED_BUILD)
        add_link_options(
            -Wl,-z,relro
            -Wl,-z,now
            -Wl,--as-needed
            -Wl,--no-copy-dt-needed-entries
        )
    endif()
endif()

if(CMAKE_C_IMPLICIT_LINK_DIRECTORIES MATCHES "alpine-linux-musl")
    add_link_options(-Wl,-z,stack-size=16777216)
endif()
