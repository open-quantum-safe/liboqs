if(OQS_ENABLE_SIG_dilithium_2)
    set(SRCS sig_dilithium_2.c
             pqclean_dilithium2_clean/sign.c
             pqclean_dilithium2_clean/polyvec.c
             pqclean_dilithium2_clean/poly.c
             pqclean_dilithium2_clean/packing.c
             pqclean_dilithium2_clean/ntt.c
             pqclean_dilithium2_clean/reduce.c
             pqclean_dilithium2_clean/rounding.c
             pqclean_dilithium2_clean/stream.c)
endif()
if(OQS_ENABLE_SIG_dilithium_3)
    set(SRCS ${SRCS} sig_dilithium_3.c
                     pqclean_dilithium3_clean/sign.c
                     pqclean_dilithium3_clean/polyvec.c
                     pqclean_dilithium3_clean/poly.c
                     pqclean_dilithium3_clean/packing.c
                     pqclean_dilithium3_clean/ntt.c
                     pqclean_dilithium3_clean/reduce.c
                     pqclean_dilithium3_clean/rounding.c
                     pqclean_dilithium3_clean/stream.c)
endif()
if(OQS_ENABLE_SIG_dilithium_4)
    set(SRCS ${SRCS} sig_dilithium_4.c
                     pqclean_dilithium4_clean/sign.c
                     pqclean_dilithium4_clean/polyvec.c
                     pqclean_dilithium4_clean/poly.c
                     pqclean_dilithium4_clean/packing.c
                     pqclean_dilithium4_clean/ntt.c
                     pqclean_dilithium4_clean/reduce.c
                     pqclean_dilithium4_clean/rounding.c
                     pqclean_dilithium4_clean/stream.c)
endif()

add_library(dilithium OBJECT ${SRCS})
target_include_directories(dilithium PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
