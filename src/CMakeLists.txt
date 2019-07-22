add_subdirectory(common)
add_subdirectory(crypto)

add_subdirectory(kem/frodokem)
add_subdirectory(kem/kyber)

add_library(oqs SHARED kem/kem.c
                       $<TARGET_OBJECTS:frodokem>
                       $<TARGET_OBJECTS:kyber>
                       $<TARGET_OBJECTS:crypto>
                       $<TARGET_OBJECTS:common>)
add_library(oqs_static STATIC kem/kem.c
                       $<TARGET_OBJECTS:frodokem>
                       $<TARGET_OBJECTS:kyber>
                       $<TARGET_OBJECTS:crypto>
                       $<TARGET_OBJECTS:common>)
target_link_libraries(oqs ${OPENSSL_CRYPTO_LIBRARY})
