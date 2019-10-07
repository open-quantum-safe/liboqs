set(SRCS common.c
         rand.c
         rand_nist.c)

add_library(common OBJECT "${SRCS}")
