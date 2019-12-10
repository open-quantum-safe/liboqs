if(OQS_ENABLE_SIG_mqdss_31_48)
    set(SRCS sig_mqdss_31_48.c
             pqclean_mqdss-48_clean/gf31.c
             pqclean_mqdss-48_clean/mq.c
             pqclean_mqdss-48_clean/sign.c)
endif()
if(OQS_ENABLE_SIG_mqdss_31_64)
    set(SRCS ${SRCS} sig_mqdss_31_64.c
                     pqclean_mqdss-64_clean/gf31.c
                     pqclean_mqdss-64_clean/mq.c
                     pqclean_mqdss-64_clean/sign.c)
endif()

add_library(mqdss OBJECT ${SRCS})
target_include_directories(mqdss PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
