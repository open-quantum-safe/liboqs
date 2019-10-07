add_subdirectory(common)
add_subdirectory(crypto)

if(ENABLE_KEM_FRODOKEM)
    add_subdirectory(kem/frodokem)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:frodokem>)
endif()
if(ENABLE_KEM_KYBER)
    add_subdirectory(kem/kyber)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:kyber>)
endif()
if(ENABLE_KEM_NEWHOPE)
    add_subdirectory(kem/newhope)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:newhope>)
endif()
if(ENABLE_KEM_NTRU)
    add_subdirectory(kem/ntru)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:ntru>)
endif()

add_library(oqs kem/kem.c
                ${KEM_OBJS}
                $<TARGET_OBJECTS:common>
                $<TARGET_OBJECTS:crypto>)

if(USE_OPENSSL)
    target_link_libraries(oqs ${OPENSSL_CRYPTO_LIBRARY})
endif()
