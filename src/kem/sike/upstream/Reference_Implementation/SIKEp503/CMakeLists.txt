set( LIBRARY_FILES_P503
        api.h api.c )

add_library(sikep503_ref ${LIBRARY_FILES_P503})

add_executable(kat_SIKEp503 PQCtestKAT_kem.c)

target_link_libraries(kat_SIKEp503 sikep503_ref sike_ref_for_test)
