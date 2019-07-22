set(sources
        common.c
        rand.c
        rand_nist.c)

add_library(common OBJECT "${sources}")
target_include_directories(common PRIVATE "${OPENSSL_INCLUDE_DIR}")
