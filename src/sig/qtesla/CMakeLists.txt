if(${ARCH} STREQUAL "x86")
    add_compile_definitions(_X86_)
elseif(${ARCH} STREQUAL "x86_64")
    add_compile_definitions(_AMD64_)
elseif(${ARCH} STREQUAL "arm")
    add_compile_definitions(_ARM_)
elseif(${ARCH} STREQUAL "arm64")
    add_compile_definitions(_ARM64_)
endif()

set (SRCS sig_qtesla.c)

if(OQS_ENABLE_SIG_qTesla_p_I)
    set(SRCS ${SRCS} qtesla_p_I.c)
endif()
if(OQS_ENABLE_SIG_qTesla_p_III)
    set(SRCS ${SRCS} qtesla_p_III.c)
endif()

if(NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin"
   AND USE_AVX2_INSTRUCTIONS
   AND USE_AES_INSTRUCTIONS)
        add_compile_definitions(WITH_AVX2)
        include_directories(avx2)
        set(SRCS ${SRCS} avx2/poly_mul1024.S
                         avx2/poly_mul2048.S)
else()
    add_compile_definitions(portable)
endif()

add_library(qTesla OBJECT ${SRCS})
target_include_directories(qTesla PRIVATE external)
