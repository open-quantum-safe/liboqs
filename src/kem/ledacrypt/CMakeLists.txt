if(OQS_ENABLE_KEM_ledakemlt12)
    set(SRCS kem_ledacrypt_ledakemlt12.c
             pqclean_ledakemlt12_leaktime/bf_decoding.c
             pqclean_ledakemlt12_leaktime/dfr_test.c
             pqclean_ledakemlt12_leaktime/gf2x_arith_mod_xPplusOne.c
             pqclean_ledakemlt12_leaktime/gf2x_arith.c
             pqclean_ledakemlt12_leaktime/H_Q_matrices_generation.c
             pqclean_ledakemlt12_leaktime/kem.c
             pqclean_ledakemlt12_leaktime/niederreiter.c
             pqclean_ledakemlt12_leaktime/rng.c
             pqclean_ledakemlt12_leaktime/sort.c
             pqclean_ledakemlt12_leaktime/utils.c)
endif()
if(OQS_ENABLE_KEM_ledakemlt32)
    set(SRCS ${SRCS} kem_ledacrypt_ledakemlt32.c
                     pqclean_ledakemlt32_leaktime/bf_decoding.c
                     pqclean_ledakemlt32_leaktime/dfr_test.c
                     pqclean_ledakemlt32_leaktime/gf2x_arith_mod_xPplusOne.c
                     pqclean_ledakemlt32_leaktime/gf2x_arith.c
                     pqclean_ledakemlt32_leaktime/H_Q_matrices_generation.c
                     pqclean_ledakemlt32_leaktime/kem.c
                     pqclean_ledakemlt32_leaktime/niederreiter.c
                     pqclean_ledakemlt32_leaktime/rng.c
                     pqclean_ledakemlt32_leaktime/sort.c
                     pqclean_ledakemlt32_leaktime/utils.c)
endif()
if(OQS_ENABLE_KEM_ledakemlt52)
    set(SRCS ${SRCS} kem_ledacrypt_ledakemlt52.c
                     pqclean_ledakemlt52_leaktime/bf_decoding.c
                     pqclean_ledakemlt52_leaktime/dfr_test.c
                     pqclean_ledakemlt52_leaktime/gf2x_arith_mod_xPplusOne.c
                     pqclean_ledakemlt52_leaktime/gf2x_arith.c
                     pqclean_ledakemlt52_leaktime/H_Q_matrices_generation.c
                     pqclean_ledakemlt52_leaktime/kem.c
                     pqclean_ledakemlt52_leaktime/niederreiter.c
                     pqclean_ledakemlt52_leaktime/rng.c
                     pqclean_ledakemlt52_leaktime/sort.c
                     pqclean_ledakemlt52_leaktime/utils.c)
endif()

add_library(ledacrypt OBJECT ${SRCS})
target_include_directories(ledacrypt PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
