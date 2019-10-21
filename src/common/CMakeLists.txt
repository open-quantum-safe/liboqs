set(SRCS common.c
         rand.c
         rand_nist.c)

add_library(common OBJECT "${SRCS}")

if(USE_OPENSSL)
    target_include_directories(common PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()
