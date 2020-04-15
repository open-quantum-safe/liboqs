if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wbad-function-cast)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wcast-qual)
    add_compile_options(-Wnarrowing)
    add_compile_options(-Wconversion)
endif()

set(SHA3_IMPL sha3/sha3_c.c)

if(OQS_USE_AVX2_INSTRUCTIONS AND OQS_USE_AES_INSTRUCTIONS)
    set(SHA3_IMPL ${SHA3_IMPL} sha3/sha3x4.c)
    add_compile_options(-mavx2)
    add_compile_options(-maes)
endif()
add_library(common_avx2 OBJECT ${SHA3_IMPL})

