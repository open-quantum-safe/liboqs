SET(frodo_sources
        frodokem/kem_frodokem640aes.c
        frodokem/external/frodo640aes.c
        frodokem/kem_frodokem640shake.c
        frodokem/external/frodo640shake.c
        frodokem/kem_frodokem976aes.c
        frodokem/external/frodo976aes.c
        frodokem/kem_frodokem976shake.c
        frodokem/external/frodo976shake.c
        frodokem/kem_frodokem1344aes.c
        frodokem/external/frodo1344aes.c
        frodokem/kem_frodokem1344shake.c
        frodokem/external/frodo1344shake.c)

add_compile_definitions(OQS_ENABLE_KEM_frodokem_640_aes)
add_compile_definitions(OQS_ENABLE_KEM_frodokem_640_shake)

add_compile_definitions(OQS_ENABLE_KEM_frodokem_976_aes)
add_compile_definitions(OQS_ENABLE_KEM_frodokem_976_shake)

add_compile_definitions(OQS_ENABLE_KEM_frodokem_1344_aes)
add_compile_definitions(OQS_ENABLE_KEM_frodokem_1344_shake)
ADD_LIBRARY(kem_objs OBJECT kem.c ${frodo_sources})
