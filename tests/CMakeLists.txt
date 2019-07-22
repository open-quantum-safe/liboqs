set (LIBS oqs m)

# Private API tests

#add_executable(test_aes test_aes.c)
#target_link_libraries(test_aes ${AES})

#add_executable(test_sha3 test_sha3.c)
#target_link_libraries(test_sha3 ${AES})

#add_executable(test_hash test_hash.c)
#target_link_libraries(test_hash ${AES})

# Public API tests

add_executable(example_kem example_kem.c)
target_link_libraries(example_kem ${LIBS})

add_executable(kat_kem kat_kem.c)
target_include_directories(kat_kem PRIVATE "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(kat_kem ${LIBS})

add_executable(speed_kem speed_kem.c)
target_include_directories(speed_kem PRIVATE "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(speed_kem ${LIBS})
