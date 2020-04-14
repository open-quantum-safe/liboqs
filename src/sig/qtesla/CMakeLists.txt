if(ARCH STREQUAL "x86")
    set(CPP_DEFS _X86_)
elseif(ARCH STREQUAL "x86_64")
    set(CPP_DEFS _AMD64_)
elseif(ARCH STREQUAL "arm")
    set(CPP_DEFS _ARM_)
elseif(ARCH STREQUAL "arm64")
    set(CPP_DEFS _ARM64_)
endif()

set (SRCS sig_qtesla.c)

if(OQS_ENABLE_SIG_qTesla_p_I)
    set(SRCS ${SRCS} qtesla_p_I.c)
endif()
if(OQS_ENABLE_SIG_qTesla_p_III)
    set(SRCS ${SRCS} qtesla_p_III.c)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" AND
   OQS_USE_AVX2_INSTRUCTIONS AND
   OQS_USE_AES_INSTRUCTIONS)
    set(CPP_DEFS ${CPP_DEFS} WITH_AVX2)
    include_directories(avx2)
    set(SRCS ${SRCS} avx2/poly_mul1024.S
                     avx2/poly_mul2048.S)
    add_compile_options(-mavx2)
    add_compile_options(-maes)
else()
    set(CPP_DEFS ${CPP_DEFS} portable)
endif()

add_library(qtesla OBJECT ${SRCS})
target_include_directories(qtesla PRIVATE external)
target_compile_definitions(qtesla PRIVATE ${CPP_DEFS})

