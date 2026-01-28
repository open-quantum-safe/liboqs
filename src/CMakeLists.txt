# SPDX-License-Identifier: MIT

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(common)

# initialize KEM|SIG_OBJS for --warn-uninitialized
set(KEM_OBJS "")
set(SIG_OBJS "")
set(SIG_STFL_OBJS "")

if(${OQS_ENABLE_KEM_BIKE})
    add_subdirectory(kem/bike)
    set(KEM_OBJS ${KEM_OBJS} ${BIKE_OBJS})
endif()
if(${OQS_ENABLE_KEM_FRODOKEM})
    add_subdirectory(kem/frodokem)
    set(KEM_OBJS ${KEM_OBJS} ${FRODOKEM_OBJS})
endif()
if(OQS_ENABLE_KEM_NTRUPRIME)
    add_subdirectory(kem/ntruprime)
    set(KEM_OBJS ${KEM_OBJS} ${NTRUPRIME_OBJS})
endif()
if(OQS_ENABLE_KEM_NTRU)
    add_subdirectory(kem/ntru)
    set(KEM_OBJS ${KEM_OBJS} ${NTRU_OBJS})
endif()
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_ADD_ALG_OBJECTS_START
if(OQS_ENABLE_KEM_CLASSIC_MCELIECE)
    add_subdirectory(kem/classic_mceliece)
    set(KEM_OBJS ${KEM_OBJS} ${CLASSIC_MCELIECE_OBJS})
endif()
if(OQS_ENABLE_KEM_HQC)
    add_subdirectory(kem/hqc)
    set(KEM_OBJS ${KEM_OBJS} ${HQC_OBJS})
endif()
if(OQS_ENABLE_KEM_KYBER)
    add_subdirectory(kem/kyber)
    set(KEM_OBJS ${KEM_OBJS} ${KYBER_OBJS})
endif()
if(OQS_ENABLE_KEM_ML_KEM)
    add_subdirectory(kem/ml_kem)
    set(KEM_OBJS ${KEM_OBJS} ${ML_KEM_OBJS})
endif()
if(OQS_ENABLE_SIG_ML_DSA)
    add_subdirectory(sig/ml_dsa)
    set(SIG_OBJS ${SIG_OBJS} ${ML_DSA_OBJS})
endif()
if(OQS_ENABLE_SIG_FALCON)
    add_subdirectory(sig/falcon)
    set(SIG_OBJS ${SIG_OBJS} ${FALCON_OBJS})
endif()
if(OQS_ENABLE_SIG_MAYO)
    add_subdirectory(sig/mayo)
    set(SIG_OBJS ${SIG_OBJS} ${MAYO_OBJS})
endif()
if(OQS_ENABLE_SIG_CROSS)
    add_subdirectory(sig/cross)
    set(SIG_OBJS ${SIG_OBJS} ${CROSS_OBJS})
endif()
if(OQS_ENABLE_SIG_UOV)
    add_subdirectory(sig/uov)
    set(SIG_OBJS ${SIG_OBJS} ${UOV_OBJS})
endif()
if(OQS_ENABLE_SIG_SNOVA)
    add_subdirectory(sig/snova)
    set(SIG_OBJS ${SIG_OBJS} ${SNOVA_OBJS})
endif()
##### OQS_COPY_FROM_UPSTREAM_FRAGMENT_ADD_ALG_OBJECTS_END

if(OQS_ENABLE_SIG_SLH_DSA)
    add_subdirectory(sig/slh_dsa)
    set(SIG_OBJS ${SIG_OBJS} ${SLH_DSA_OBJS})
endif()

if(OQS_ENABLE_SIG_STFL_XMSS)
    add_subdirectory(sig_stfl/xmss)
    set(SIG_STFL_OBJS ${SIG_STFL_OBJS} ${XMSS_OBJS})
endif()

if(OQS_ENABLE_SIG_STFL_LMS)
    add_subdirectory(sig_stfl/lms)
    set(SIG_STFL_OBJS ${SIG_STFL_OBJS} ${LMS_OBJS})
endif()

add_library(oqs kem/kem.c
                ${KEM_OBJS}
                sig/sig.c
                ${SIG_OBJS}
                sig_stfl/sig_stfl.c
                ${SIG_STFL_OBJS}
                ${COMMON_OBJS})

# Internal library to be used only by test programs
add_library(oqs-internal STATIC ${INTERNAL_OBJS})

if(DEFINED SANITIZER_LD_FLAGS)
    target_link_libraries(oqs PUBLIC ${SANITIZER_LD_FLAGS})
    target_link_libraries(oqs-internal PUBLIC ${SANITIZER_LD_FLAGS})
endif()
if(${OQS_USE_OPENSSL})
  if(${OQS_DLOPEN_OPENSSL})
    target_link_libraries(oqs PRIVATE ${CMAKE_DL_LIBS})
    target_link_libraries(oqs-internal PRIVATE ${CMAKE_DL_LIBS})
  else()
    target_link_libraries(oqs PRIVATE ${OPENSSL_CRYPTO_LIBRARY})
    target_link_libraries(oqs-internal PRIVATE ${OPENSSL_CRYPTO_LIBRARY})
  endif()
endif()
if(${OQS_USE_CUPQC})
    set_property(TARGET oqs PROPERTY CUDA_RESOLVE_DEVICE_SYMBOLS ON)
    target_link_libraries(oqs PRIVATE cupqc)
    target_link_options(oqs PRIVATE $<DEVICE_LINK: -dlto>)
endif()
if(${OQS_USE_ICICLE})
    target_link_libraries(oqs PRIVATE icicle::icicle_pqc_package)
endif()

target_include_directories(oqs
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(oqs-internal
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(MINGW)
    target_link_options(oqs-internal PRIVATE -Wl,--export-all-symbols)
endif()

set_target_properties(oqs
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    VERSION ${OQS_VERSION_TEXT}
    SOVERSION 9
    # For Windows DLLs
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set_property(TARGET oqs
    APPEND PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>")

set_target_properties(oqs-internal
    PROPERTIES
    C_VISIBILITY_PRESET default
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    # For Windows DLLs
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/liboqsConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/liboqs
)

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/liboqsConfigVersion.cmake"
  VERSION "${OQS_VERSION_TEXT}"
  COMPATIBILITY AnyNewerVersion
)

if(OQS_USE_OPENSSL)
    set(_oqs_pkgconfig_requires_private openssl)
endif()

# generate pkg-config file
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/liboqs.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/liboqs.pc
  @ONLY
)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/liboqsConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/liboqsConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/liboqs"
)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/liboqs.pc"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

install(TARGETS oqs
        EXPORT liboqsTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/oqs
)

install(EXPORT liboqsTargets
        FILE liboqsTargets.cmake
        NAMESPACE OQS::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/liboqs
)

install(FILES ${PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/oqs)

export(EXPORT liboqsTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/liboqsTargets.cmake"
       NAMESPACE OQS::
)
