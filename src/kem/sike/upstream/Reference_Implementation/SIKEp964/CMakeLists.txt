set( LIBRARY_FILES_P964
        api.h api.c )

add_library(sikep964_ref ${LIBRARY_FILES_P964})

add_executable(kat_SIKEp964 PQCtestKAT_kem.c)

target_link_libraries(kat_SIKEp964 sikep964_ref sike_ref_for_test)
