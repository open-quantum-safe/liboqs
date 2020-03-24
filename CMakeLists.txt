cmake_minimum_required (VERSION 3.5)
cmake_policy(SET CMP0063 NEW)

project(liboqs C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(OQS_VERSION_TEXT "0.2.1-dev")
set(OQS_COMPILE_BUILD_TARGET "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM}")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
    set(ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i386|i686")
    set(ARCH "x86")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(ARCH "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    set(ARCH "arm")
else()
    message(FATAL_ERROR "Unknown or unsupported processor: " ${CMAKE_SYSTEM_PROCESSOR})
endif()
include(third_party/cpu_features/add_lib.cmake)

if(DEFINED OQS_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${OQS_BUILD_TYPE})
endif()
if(WIN32)
    set(CMAKE_GENERATOR_CC cl)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
include(.CMake/compiler_flags.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Optimized")
    include(.CMake/cpu_extensions.cmake)
endif()

include(.CMake/alg_support.cmake)

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
                   ${PROJECT_SOURCE_DIR}/src/common/rand/rand.h
                   ${PROJECT_SOURCE_DIR}/src/common/aes/aes.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha2/sha2.h
                   ${PROJECT_SOURCE_DIR}/src/common/sha3/sha3.h
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
if(OQS_ENABLE_KEM_CLASSIC_MCELIECE)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/kem/classic_mceliece/kem_classic_mceliece.h)
endif()
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
if(OQS_ENABLE_SIG_FALCON)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/falcon/sig_falcon.h)
endif()
if(OQS_ENABLE_SIG_MQDSS)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/mqdss/sig_mqdss.h)
endif()
if(OQS_ENABLE_SIG_RAINBOW)
    set(PUBLIC_HEADERS ${PUBLIC_HEADERS} ${PROJECT_SOURCE_DIR}/src/sig/rainbow/sig_rainbow.h)
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

if(NOT CMAKE_BUILD_TYPE STREQUAL "Dependency")
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
endif()
