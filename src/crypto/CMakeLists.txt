SET(sources
        aes/aes_c.c
        sha3/sha3_c.c
        sha3/sha3x4.c)

ADD_LIBRARY(crypto OBJECT "${sources}")
