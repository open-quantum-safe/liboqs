if(OQS_ENABLE_KEM_ntru_hps2048509)
    set(SRCS kem_ntru_hps2048509.c
             pqclean_ntruhps2048509_clean/crypto_sort.c
             pqclean_ntruhps2048509_clean/kem.c
             pqclean_ntruhps2048509_clean/owcpa.c
             pqclean_ntruhps2048509_clean/pack3.c
             pqclean_ntruhps2048509_clean/packq.c
             pqclean_ntruhps2048509_clean/poly.c
             pqclean_ntruhps2048509_clean/sample.c
             pqclean_ntruhps2048509_clean/verify.c)
endif()
if(OQS_ENABLE_KEM_ntru_hps2048677)
    set(SRCS ${SRCS} kem_ntru_hps2048677.c
                     pqclean_ntruhps2048677_clean/crypto_sort.c
                     pqclean_ntruhps2048677_clean/kem.c
                     pqclean_ntruhps2048677_clean/owcpa.c
                     pqclean_ntruhps2048677_clean/pack3.c
                     pqclean_ntruhps2048677_clean/packq.c
                     pqclean_ntruhps2048677_clean/poly.c
                     pqclean_ntruhps2048677_clean/sample.c
                     pqclean_ntruhps2048677_clean/verify.c)
endif()
if(OQS_ENABLE_KEM_ntru_hps4096821)
    set(SRCS ${SRCS} kem_ntru_hps4096821.c
                     pqclean_ntruhps4096821_clean/crypto_sort.c
                     pqclean_ntruhps4096821_clean/kem.c
                     pqclean_ntruhps4096821_clean/owcpa.c
                     pqclean_ntruhps4096821_clean/pack3.c
                     pqclean_ntruhps4096821_clean/packq.c
                     pqclean_ntruhps4096821_clean/poly.c
                     pqclean_ntruhps4096821_clean/sample.c
                     pqclean_ntruhps4096821_clean/verify.c)
endif()
if(OQS_ENABLE_KEM_ntru_hrss701)
    set(SRCS ${SRCS} kem_ntru_hrss701.c
                     pqclean_ntruhrss701_clean/kem.c
                     pqclean_ntruhrss701_clean/owcpa.c
                     pqclean_ntruhrss701_clean/pack3.c
                     pqclean_ntruhrss701_clean/packq.c
                     pqclean_ntruhrss701_clean/poly.c
                     pqclean_ntruhrss701_clean/sample.c
                     pqclean_ntruhrss701_clean/verify.c)
endif()

add_library(ntru OBJECT ${SRCS})
target_include_directories(ntru PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
