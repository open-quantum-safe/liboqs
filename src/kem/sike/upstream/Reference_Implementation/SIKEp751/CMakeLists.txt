set( LIBRARY_FILES_P751
        api.h api.c )

add_library(sikep751_ref ${LIBRARY_FILES_P751})

add_executable(kat_SIKEp751 PQCtestKAT_kem.c)

target_link_libraries(kat_SIKEp751 sikep751_ref sike_ref_for_test)
