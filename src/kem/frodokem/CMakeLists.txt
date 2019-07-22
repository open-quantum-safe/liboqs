SET(SRCS kem_frodokem640aes.c
         external/frodo640aes.c
         kem_frodokem640shake.c
         external/frodo640shake.c
         kem_frodokem976aes.c
         external/frodo976aes.c
         kem_frodokem976shake.c
         external/frodo976shake.c
         kem_frodokem1344aes.c
         external/frodo1344aes.c
         kem_frodokem1344shake.c
         external/frodo1344shake.c)

add_library(frodokem OBJECT ${SRCS})
