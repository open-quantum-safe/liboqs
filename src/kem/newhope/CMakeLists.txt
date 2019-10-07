if(OQS_ENABLE_KEM_newhope_512cca)
    set(SRCS ${SRCS} kem_newhope_512cca.c
                     pqclean_newhope512cca_clean/cpapke.c
                     pqclean_newhope512cca_clean/kem.c
                     pqclean_newhope512cca_clean/ntt.c
                     pqclean_newhope512cca_clean/poly.c
                     pqclean_newhope512cca_clean/precomp.c
                     pqclean_newhope512cca_clean/reduce.c
                     pqclean_newhope512cca_clean/verify.c)
endif()
if(OQS_ENABLE_KEM_newhope_1024cca)
    set(SRCS ${SRCS} kem_newhope_1024cca.c
                     pqclean_newhope1024cca_clean/cpapke.c
                     pqclean_newhope1024cca_clean/kem.c
                     pqclean_newhope1024cca_clean/ntt.c
                     pqclean_newhope1024cca_clean/poly.c
                     pqclean_newhope1024cca_clean/precomp.c
                     pqclean_newhope1024cca_clean/reduce.c
                     pqclean_newhope1024cca_clean/verify.c)
endif()

add_library(newhope OBJECT ${SRCS})
target_include_directories(newhope PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
