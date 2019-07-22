set(KYBER512_SRCS kem_kyber_512.c
                  pqclean_kyber512_clean/cbd.c
                  pqclean_kyber512_clean/indcpa.c
                  pqclean_kyber512_clean/kem.c
                  pqclean_kyber512_clean/ntt.c
                  pqclean_kyber512_clean/poly.c
                  pqclean_kyber512_clean/polyvec.c
                  pqclean_kyber512_clean/reduce.c
                  pqclean_kyber512_clean/verify.c
                  pqclean_kyber512_clean/symmetric-fips202.c)

set(KYBER768_SRCS kem_kyber_768.c
                  pqclean_kyber768_clean/cbd.c
                  pqclean_kyber768_clean/indcpa.c
                  pqclean_kyber768_clean/kem.c
                  pqclean_kyber768_clean/ntt.c
                  pqclean_kyber768_clean/poly.c
                  pqclean_kyber768_clean/polyvec.c
                  pqclean_kyber768_clean/reduce.c
                  pqclean_kyber768_clean/verify.c
                  pqclean_kyber768_clean/symmetric-fips202.c)

set(KYBER1024_SRCS kem_kyber_1024.c
                  pqclean_kyber1024_clean/cbd.c
                  pqclean_kyber1024_clean/indcpa.c
                  pqclean_kyber1024_clean/kem.c
                  pqclean_kyber1024_clean/ntt.c
                  pqclean_kyber1024_clean/poly.c
                  pqclean_kyber1024_clean/polyvec.c
                  pqclean_kyber1024_clean/reduce.c
                  pqclean_kyber1024_clean/verify.c
                  pqclean_kyber1024_clean/symmetric-fips202.c)

add_library(kyber OBJECT ${KYBER512_SRCS} ${KYBER768_SRCS} ${KYBER1024_SRCS})
target_include_directories(kyber PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
