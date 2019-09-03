add_subdirectory(common)
add_subdirectory(crypto)

if(ENABLE_KEM_FRODOKEM)
    add_subdirectory(kem/frodokem)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:frodokem>)
endif()

add_library(oqs kem/kem.c
                ${KEM_OBJS}
                $<TARGET_OBJECTS:common>
                $<TARGET_OBJECTS:crypto>)

if(USE_OPENSSL)
    target_link_libraries(oqs ${OPENSSL_CRYPTO_LIBRARY})
endif()
