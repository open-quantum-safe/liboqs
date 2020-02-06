if(OQS_ENABLE_KEM_threebears_babybear)
    set(SRCS kem_threebears_babybear.c
             pqclean_babybear_clean/kem.c
             pqclean_babybear_clean/melas_fec.c
             pqclean_babybear_clean/ring.c
             pqclean_babybear_clean/threebears.c)
endif()
if(OQS_ENABLE_KEM_threebears_babybear_ephem)
    set(SRCS ${SRCS} kem_threebears_babybear_ephem.c
                     pqclean_babybear-ephem_clean/kem.c
                     pqclean_babybear-ephem_clean/melas_fec.c
                     pqclean_babybear-ephem_clean/ring.c
                     pqclean_babybear-ephem_clean/threebears.c)
endif()
if(OQS_ENABLE_KEM_threebears_mamabear)
    set(SRCS ${SRCS} kem_threebears_mamabear.c
                     pqclean_mamabear_clean/kem.c
                     pqclean_mamabear_clean/melas_fec.c
                     pqclean_mamabear_clean/ring.c
                     pqclean_mamabear_clean/threebears.c)
endif()
if(OQS_ENABLE_KEM_threebears_mamabear_ephem)
    set(SRCS ${SRCS} kem_threebears_mamabear_ephem.c
                     pqclean_mamabear-ephem_clean/kem.c
                     pqclean_mamabear-ephem_clean/melas_fec.c
                     pqclean_mamabear-ephem_clean/ring.c
                     pqclean_mamabear-ephem_clean/threebears.c)
endif()
if(OQS_ENABLE_KEM_threebears_papabear)
    set(SRCS ${SRCS} kem_threebears_papabear.c
                     pqclean_papabear_clean/kem.c
                     pqclean_papabear_clean/melas_fec.c
                     pqclean_papabear_clean/ring.c
                     pqclean_papabear_clean/threebears.c)
endif()
if(OQS_ENABLE_KEM_threebears_papabear_ephem)
    set(SRCS ${SRCS} kem_threebears_papabear_ephem.c
                     pqclean_papabear-ephem_clean/kem.c
                     pqclean_papabear-ephem_clean/melas_fec.c
                     pqclean_papabear-ephem_clean/ring.c
                     pqclean_papabear-ephem_clean/threebears.c)
endif()

add_library(threebears OBJECT ${SRCS})
target_include_directories(threebears PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
