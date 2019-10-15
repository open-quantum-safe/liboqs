add_subdirectory(common)
add_subdirectory(crypto)

if(ENABLE_KEM_BIKE)
    add_subdirectory(kem/bike)
    set(KEM_OBJS ${BIKE_OBJS})
endif()
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
if(ENABLE_KEM_SABER)
    add_subdirectory(kem/saber)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:saber>)
endif()
if(ENABLE_KEM_SIKE)
    add_subdirectory(kem/sike)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:sike>)
endif()

add_library(oqs kem/kem.c
                ${KEM_OBJS}
                $<TARGET_OBJECTS:common>
                $<TARGET_OBJECTS:crypto>)
set_target_properties(oqs
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(USE_OPENSSL)
    target_include_directories(oqs PRIVATE "${OPENSSL_INCLUDE_DIR}")
    target_link_libraries(oqs ${OPENSSL_CRYPTO_LIBRARY})
endif()
