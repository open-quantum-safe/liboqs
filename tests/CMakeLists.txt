set(LIBS oqs)
if(NOT WIN32)
    set (LIBS ${LIBS} m)
endif()

# Common crypto API tests

if(NOT WIN32)
    add_executable(test_aes test_aes.c)
    target_link_libraries(test_aes ${LIBS})

    add_executable(test_hash test_hash.c)
    target_link_libraries(test_hash ${LIBS})

    add_executable(test_sha3 test_sha3.c)
    target_link_libraries(test_sha3 ${LIBS})
endif()

# KEM API tests

add_executable(example_kem example_kem.c)
target_link_libraries(example_kem ${LIBS})

add_executable(kat_kem kat_kem.c)
target_link_libraries(kat_kem ${LIBS})

add_executable(speed_kem speed_kem.c)
target_link_libraries(speed_kem ${LIBS})
