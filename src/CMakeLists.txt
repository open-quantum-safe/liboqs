add_subdirectory(common)
add_subdirectory(crypto)

if(OQS_ENABLE_KEM_BIKE)
    add_subdirectory(kem/bike)
    set(KEM_OBJS ${BIKE_OBJS})
endif()
if(OQS_ENABLE_KEM_FRODOKEM)
    add_subdirectory(kem/frodokem)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:frodokem>)
endif()
if(OQS_ENABLE_KEM_SIKE OR OQS_ENABLE_KEM_SIDH)
    add_subdirectory(kem/sike)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:sike>)
endif()
if(OQS_ENABLE_SIG_PICNIC)
    add_subdirectory(sig/picnic)
    set(SIG_OBJS $<TARGET_OBJECTS:picnic>)
endif()
if(OQS_ENABLE_SIG_QTESLA)
    add_subdirectory(sig/qtesla)
    set(SIG_OBJS ${SIG_OBJS} $<TARGET_OBJECTS:qtesla>)
endif()
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ADD_ALG_OBJECTS_START
if(OQS_ENABLE_KEM_KYBER)
    add_subdirectory(kem/kyber)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:kyber>)
endif()
if(OQS_ENABLE_KEM_LEDACRYPT)
    add_subdirectory(kem/ledacrypt)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:ledacrypt>)
endif()
if(OQS_ENABLE_KEM_NEWHOPE)
    add_subdirectory(kem/newhope)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:newhope>)
endif()
if(OQS_ENABLE_KEM_NTRU)
    add_subdirectory(kem/ntru)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:ntru>)
endif()
if(OQS_ENABLE_KEM_SABER)
    add_subdirectory(kem/saber)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:saber>)
endif()
if(OQS_ENABLE_KEM_THREEBEARS)
    add_subdirectory(kem/threebears)
    set(KEM_OBJS ${KEM_OBJS} $<TARGET_OBJECTS:threebears>)
endif()
if(OQS_ENABLE_SIG_DILITHIUM)
    add_subdirectory(sig/dilithium)
    set(SIG_OBJS ${SIG_OBJS} $<TARGET_OBJECTS:dilithium>)
endif()
if(OQS_ENABLE_SIG_MQDSS)
    add_subdirectory(sig/mqdss)
    set(SIG_OBJS ${SIG_OBJS} $<TARGET_OBJECTS:mqdss>)
endif()
if(OQS_ENABLE_SIG_SPHINCS)
    add_subdirectory(sig/sphincs)
    set(SIG_OBJS ${SIG_OBJS} $<TARGET_OBJECTS:sphincs>)
endif()
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ADD_ALG_OBJECTS_END

add_library(oqs kem/kem.c
                ${KEM_OBJS}
                sig/sig.c
                ${SIG_OBJS}
                $<TARGET_OBJECTS:crypto>
                $<TARGET_OBJECTS:common>)
if(OQS_USE_OPENSSL)
    target_link_libraries(oqs ${OPENSSL_CRYPTO_LIBRARY})
endif()

set_target_properties(oqs
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    VERSION 0.0.0
    SOVERSION 0
    # For Windows DLLs
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

install(TARGETS oqs
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES ${PUBLIC_HEADERS}
        DESTINATION include/oqs)
