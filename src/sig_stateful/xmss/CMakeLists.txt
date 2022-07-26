
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
CMAKE_C_COMPILER_ID MATCHES "Clang")
 add_compile_options(-Wno-missing-braces)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
 if(CMAKE_C_COMPILER_VERSION VERSION_GREATER "3.9")
     add_compile_options(-Wno-unguarded-availability-new)
 else() #Wno-unguarded-availability-new won't be recognized
     add_compile_options(-Wno-error)
 endif()
endif()
if(WIN32 AND NOT (MINGW OR MSYS OR CYGWIN))
 # ignore some warnings. picnic team confirmed these are safe to ignore; will be fixed eventually
 add_compile_options(/wd4244 /wd4267 /wd4334)
endif()

set(SRCS sig_stfl_xmss_sha256.c
         sig_stfl_xmss_sha512.c
         sig_stfl_xmss_shake256.c
         sig_stfl_xmss_shake512.c
         sig_stfl_functions.c
         external/hash_address.c
         external/hash.c
         external/params.c
         external/utils.c
         external/wots.c
         external/xmss_commons.c
         external/xmss_core_fast.c
         external/xmss.c)

add_library(xmss OBJECT ${SRCS})
target_include_directories(xmss PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_compile_definitions(xmss PRIVATE OQS
                                          XMSS_EXPORT=
                                          WITH_SHA256_H10
                                          WITH_SHA256_H16
                                          WITH_SHA256_H20
                                          WITH_SHA512_H10
                                          WITH_SHA512_H16
                                          WITH_SHA512_H20
                                          WITH_SHAKE256_H10
                                          WITH_SHAKE256_H16
                                          WITH_SHAKE256_H20
                                          WITH_SHAKE512_H10
                                          WITH_SHAKE512_H16
                                          WITH_SHAKE512_H20
                                          )