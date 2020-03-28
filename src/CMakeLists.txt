add_subdirectory(common)

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
if(OQS_ENABLE_KEM_CLASSIC_MCELIECE)
    add_subdirectory(kem/classic_mceliece)
endif()
if(OQS_ENABLE_KEM_KYBER)
    add_subdirectory(kem/kyber)
endif()
if(OQS_ENABLE_KEM_LEDACRYPT)
    add_subdirectory(kem/ledacrypt)
endif()
if(OQS_ENABLE_KEM_NEWHOPE)
    add_subdirectory(kem/newhope)
endif()
if(OQS_ENABLE_KEM_NTRU)
    add_subdirectory(kem/ntru)
endif()
if(OQS_ENABLE_KEM_SABER)
    add_subdirectory(kem/saber)
endif()
if(OQS_ENABLE_KEM_THREEBEARS)
    add_subdirectory(kem/threebears)
endif()
if(OQS_ENABLE_SIG_DILITHIUM)
    add_subdirectory(sig/dilithium)
endif()
if(OQS_ENABLE_SIG_FALCON)
    add_subdirectory(sig/falcon)
endif()
if(OQS_ENABLE_SIG_MQDSS)
    add_subdirectory(sig/mqdss)
endif()
if(OQS_ENABLE_SIG_RAINBOW)
    add_subdirectory(sig/rainbow)
endif()
if(OQS_ENABLE_SIG_SPHINCS)
    add_subdirectory(sig/sphincs)
endif()
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ADD_ALG_OBJECTS_END

add_library(oqs kem/kem.c
                ${KEM_OBJS}
                sig/sig.c
                ${SIG_OBJS}
##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ADD_TEMPLATE_OBJECTS_START
$<TARGET_OBJECTS:classic_mceliece_348864_vec>
       $<TARGET_OBJECTS:classic_mceliece_348864f_vec>
       $<TARGET_OBJECTS:classic_mceliece_460896_vec>
       $<TARGET_OBJECTS:classic_mceliece_460896f_vec>
       $<TARGET_OBJECTS:classic_mceliece_6688128_vec>
       $<TARGET_OBJECTS:classic_mceliece_6688128f_vec>
       $<TARGET_OBJECTS:classic_mceliece_6960119_vec>
       $<TARGET_OBJECTS:classic_mceliece_6960119f_vec>
       $<TARGET_OBJECTS:classic_mceliece_8192128_vec>
       $<TARGET_OBJECTS:classic_mceliece_8192128f_vec>
       $<TARGET_OBJECTS:kyber_512_clean>
       $<TARGET_OBJECTS:kyber_512_avx2>
       $<TARGET_OBJECTS:kyber_768_clean>
       $<TARGET_OBJECTS:kyber_768_avx2>
       $<TARGET_OBJECTS:kyber_1024_clean>
       $<TARGET_OBJECTS:kyber_1024_avx2>
       $<TARGET_OBJECTS:kyber_512_90s_clean>
       $<TARGET_OBJECTS:kyber_512_90s_avx2>
       $<TARGET_OBJECTS:kyber_768_90s_clean>
       $<TARGET_OBJECTS:kyber_768_90s_avx2>
       $<TARGET_OBJECTS:kyber_1024_90s_clean>
       $<TARGET_OBJECTS:kyber_1024_90s_avx2>
       $<TARGET_OBJECTS:ledacrypt_ledakemlt12_leaktime>
       $<TARGET_OBJECTS:ledacrypt_ledakemlt32_leaktime>
       $<TARGET_OBJECTS:ledacrypt_ledakemlt52_leaktime>
       $<TARGET_OBJECTS:newhope_512cca_clean>
       $<TARGET_OBJECTS:newhope_1024cca_clean>
       $<TARGET_OBJECTS:ntru_hps2048509_clean>
       $<TARGET_OBJECTS:ntru_hps2048677_clean>
       $<TARGET_OBJECTS:ntru_hps4096821_clean>
       $<TARGET_OBJECTS:ntru_hrss701_clean>
       $<TARGET_OBJECTS:saber_lightsaber_clean>
       $<TARGET_OBJECTS:saber_saber_clean>
       $<TARGET_OBJECTS:saber_firesaber_clean>
       $<TARGET_OBJECTS:threebears_babybear_clean>
       $<TARGET_OBJECTS:threebears_babybear_ephem_clean>
       $<TARGET_OBJECTS:threebears_mamabear_clean>
       $<TARGET_OBJECTS:threebears_mamabear_ephem_clean>
       $<TARGET_OBJECTS:threebears_papabear_clean>
       $<TARGET_OBJECTS:threebears_papabear_ephem_clean>
       $<TARGET_OBJECTS:dilithium_2_clean>
       $<TARGET_OBJECTS:dilithium_2_avx2>
       $<TARGET_OBJECTS:dilithium_3_clean>
       $<TARGET_OBJECTS:dilithium_3_avx2>
       $<TARGET_OBJECTS:dilithium_4_clean>
       $<TARGET_OBJECTS:dilithium_4_avx2>
       $<TARGET_OBJECTS:falcon_512_clean>
       $<TARGET_OBJECTS:falcon_1024_clean>
       $<TARGET_OBJECTS:mqdss_31_48_clean>
       $<TARGET_OBJECTS:mqdss_31_64_clean>
       $<TARGET_OBJECTS:rainbow_Ia_classic_clean>
       $<TARGET_OBJECTS:rainbow_Ia_cyclic_clean>
       $<TARGET_OBJECTS:rainbow_Ia_cyclic_compressed_clean>
       $<TARGET_OBJECTS:rainbow_IIIc_classic_clean>
       $<TARGET_OBJECTS:rainbow_IIIc_cyclic_clean>
       $<TARGET_OBJECTS:rainbow_IIIc_cyclic_compressed_clean>
       $<TARGET_OBJECTS:rainbow_Vc_classic_clean>
       $<TARGET_OBJECTS:rainbow_Vc_cyclic_clean>
       $<TARGET_OBJECTS:rainbow_Vc_cyclic_compressed_clean>
       $<TARGET_OBJECTS:sphincs_haraka_128f_robust_clean>
       $<TARGET_OBJECTS:sphincs_haraka_128f_simple_clean>
       $<TARGET_OBJECTS:sphincs_haraka_128s_robust_clean>
       $<TARGET_OBJECTS:sphincs_haraka_128s_simple_clean>
       $<TARGET_OBJECTS:sphincs_haraka_192f_robust_clean>
       $<TARGET_OBJECTS:sphincs_haraka_192f_simple_clean>
       $<TARGET_OBJECTS:sphincs_haraka_192s_robust_clean>
       $<TARGET_OBJECTS:sphincs_haraka_192s_simple_clean>
       $<TARGET_OBJECTS:sphincs_haraka_256f_robust_clean>
       $<TARGET_OBJECTS:sphincs_haraka_256f_simple_clean>
       $<TARGET_OBJECTS:sphincs_haraka_256s_robust_clean>
       $<TARGET_OBJECTS:sphincs_haraka_256s_simple_clean>
       $<TARGET_OBJECTS:sphincs_sha256_128f_robust_clean>
       $<TARGET_OBJECTS:sphincs_sha256_128f_simple_clean>
       $<TARGET_OBJECTS:sphincs_sha256_128s_robust_clean>
       $<TARGET_OBJECTS:sphincs_sha256_128s_simple_clean>
       $<TARGET_OBJECTS:sphincs_sha256_192f_robust_clean>
       $<TARGET_OBJECTS:sphincs_sha256_192f_simple_clean>
       $<TARGET_OBJECTS:sphincs_sha256_192s_robust_clean>
       $<TARGET_OBJECTS:sphincs_sha256_192s_simple_clean>
       $<TARGET_OBJECTS:sphincs_sha256_256f_robust_clean>
       $<TARGET_OBJECTS:sphincs_sha256_256f_simple_clean>
       $<TARGET_OBJECTS:sphincs_sha256_256s_robust_clean>
       $<TARGET_OBJECTS:sphincs_sha256_256s_simple_clean>
       $<TARGET_OBJECTS:sphincs_shake256_128f_robust_clean>
       $<TARGET_OBJECTS:sphincs_shake256_128f_simple_clean>
       $<TARGET_OBJECTS:sphincs_shake256_128s_robust_clean>
       $<TARGET_OBJECTS:sphincs_shake256_128s_simple_clean>
       $<TARGET_OBJECTS:sphincs_shake256_192f_robust_clean>
       $<TARGET_OBJECTS:sphincs_shake256_192f_simple_clean>
       $<TARGET_OBJECTS:sphincs_shake256_192s_robust_clean>
       $<TARGET_OBJECTS:sphincs_shake256_192s_simple_clean>
       $<TARGET_OBJECTS:sphincs_shake256_256f_robust_clean>
       $<TARGET_OBJECTS:sphincs_shake256_256f_simple_clean>
       $<TARGET_OBJECTS:sphincs_shake256_256s_robust_clean>
       $<TARGET_OBJECTS:sphincs_shake256_256s_simple_clean>
       ##### OQS_COPY_FROM_PQCLEAN_FRAGMENT_ADD_TEMPLATE_OBJECTS_END
                $<TARGET_OBJECTS:common>)
if(DEFINED SANITIZER_LD_FLAGS)
    target_link_libraries(oqs PUBLIC ${SANITIZER_LD_FLAGS})
endif()
if(OQS_USE_OPENSSL)
    target_link_libraries(oqs PUBLIC ${OPENSSL_CRYPTO_LIBRARY})
    target_include_directories(oqs PUBLIC ${OPENSSL_INCLUDE_DIR})
endif()

set_target_properties(oqs
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    VERSION 0.0.0
    SOVERSION 0
    # For Windows DLLs
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_library(OQS::oqs ALIAS oqs)

install(TARGETS oqs
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES ${PUBLIC_HEADERS}
        DESTINATION include/oqs)
