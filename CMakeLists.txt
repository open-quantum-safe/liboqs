cmake_minimum_required (VERSION 3.6)

project(liboqs C ASM)
set(OQS_VERSION_TEXT "0.2.1-dev")
set(OQS_COMPILE_BUILD_TARGET "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM}")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR
   CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")
    set(ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
    # cmake reports AMD64 on Windows, but we might be building for 32-bit.
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86_64")
    else()
        set(ARCH "x86")
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86" OR
       CMAKE_SYSTEM_PROCESSOR STREQUAL "i386" OR
       CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
    set(ARCH "x86")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" OR
       CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(ARCH "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    set(ARCH "arm")
else()
    message(FATAL_ERROR "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR})
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
if(WIN32)
    set(CMAKE_GENERATOR_CC cl)
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Werror -Wall -Wextra -Wpedantic -Wimplicit -Wunused -Wcomment -Wchar-subscripts -Wuninitialized -Wstrict-prototypes -Wshadow -Wformat-security -Wwrite-strings")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "5")
        # GCC 4.9 erroneously reports warnings for certain valid code in BIKE
        # https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-braces -Wno-missing-field-initializers")
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb3")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Generic")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fomit-frame-pointer ")
    else()
        if (NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
           include(.CMake/cpu-extensions.cmake)
           set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fomit-frame-pointer -march=native")
	   set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -march=native")
        endif()
    endif()
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics -Wno-missing-braces -Wno-missing-field-initializers")
endif()
if(MINGW OR MSYS OR CYGWIN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-maybe-uninitialized")
endif()
if(BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()
set(OQS_COMPILE_CFLAGS ${CMAKE_C_FLAGS})

include(.CMake/alg-support.cmake)

include(.CMake/sanitizers.cmake)

if(OQS_USE_OPENSSL)
    if(NOT DEFINED OPENSSL_ROOT_DIR)
        if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
            set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1")
        elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
            set(OPENSSL_ROOT_DIR "/usr")
        endif()
    endif()
    find_package(OpenSSL 1.1.1 REQUIRED)
endif()

set(PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/src/oqs.h
                   ${PROJECT_SOURCE_DIR}/src/common/common.h
                   ${PROJECT_SOURCE_DIR}/src/common/rand.h
                   ${PROJECT_SOURCE_DIR}/src/crypto/aes/aes.h
                   ${PROJECT_SOURCE_DIR}/src/crypto/sha2/sha2.h
                   ${PROJECT_SOURCE_DIR}/src/crypto/sha3/sha3.h
                   ${PROJECT_SOURCE_DIR}/src/kem/kem.h
                   ${PROJECT_SOURCE_DIR}/src/sig/sig.h)

if(OQS_ENABLE_KEM_BIKE)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/bike/kem_bike.h)
endif()
if(OQS_ENABLE_KEM_FRODOKEM)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/frodokem/kem_frodokem.h)
endif()
if(OQS_ENABLE_KEM_SIKE OR OQS_ENABLE_KEM_SIDH)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/sike/kem_sike.h)
endif()
if(OQS_ENABLE_SIG_PICNIC)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/picnic/sig_picnic.h)
endif()
if(OQS_ENABLE_SIG_QTESLA)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/qtesla/sig_qtesla.h)
endif()
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_INCLUDE_HEADERS_START
if(OQS_ENABLE_KEM_KYBER)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/kyber/kem_kyber.h)
endif()
if(OQS_ENABLE_KEM_LEDACRYPT)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ledacrypt/kem_ledacrypt.h)
endif()
if(OQS_ENABLE_KEM_NEWHOPE)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/newhope/kem_newhope.h)
endif()
if(OQS_ENABLE_KEM_NTRU)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/ntru/kem_ntru.h)
endif()
if(OQS_ENABLE_KEM_SABER)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/saber/kem_saber.h)
endif()
if(OQS_ENABLE_KEM_THREEBEARS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/threebears/kem_threebears.h)
endif()
if(OQS_ENABLE_SIG_DILITHIUM)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/dilithium/sig_dilithium.h)
endif()
if(OQS_ENABLE_SIG_MQDSS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/mqdss/sig_mqdss.h)
endif()
if(OQS_ENABLE_SIG_SPHINCS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/sphincs/sig_sphincs.h)
endif()
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_INCLUDE_HEADERS_END
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/oqs)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs)
configure_file(src/oqsconfig.h.cmake ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)
set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_BINARY_DIR}/include/oqs/oqsconfig.h)

include_directories(${PROJECT_BINARY_DIR}/include)
add_subdirectory(src)

add_subdirectory(tests)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYFILE ${PROJECT_SOURCE_DIR}/docs/.Doxyfile)
    add_custom_target(
        gen_docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generate API documentation with Doxygen."
        USES_TERMINAL)
endif()

if(NOT WIN32)
    add_custom_target(
        prettyprint
        COMMAND find src tests -name '*.[ch]' | grep -v '/external/' | grep -v 'kem.*/pqclean_' | grep -v 'sig.*/pqclean_' | xargs astyle --options=.astylerc
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL)
endif()
