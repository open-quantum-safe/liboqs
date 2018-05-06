cmake_minimum_required(VERSION 3.5)
project(sike_weierstrass C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_RELEASE -O3)

find_library(GMP gmp)

include_directories(include)

include_directories(symmetric)

set(
        LIBRARY_FILES
        include/fp.h src/fp.c
        include/sike_params.h src/sike_params.c
        include/fp2.h src/fp2.c
        include/sidh.h src/sidh.c
        include/sike.h src/sike.c
        include/encoding.h src/encoding.c
        include/weierstrass.h src/weierstrass.c
        include/isogeny.h src/isogeny.c
        include/api_generic.h src/api_generic.c
        symmetric/fips202.h symmetric/fips202.c
)

set (
        RANDOM_FILES
        symmetric/random.c symmetric/random.h
)

set(
        RANDOM_FILES_TEST
        symmetric/rng.h symmetric/rng.c
        symmetric/aes.h symmetric/aes_c.c
        symmetric/aes_local.h
)

add_library(sike_weierstrass ${LIBRARY_FILES} ${RANDOM_FILES})
add_library(sike_weierstrass_for_test ${LIBRARY_FILES} ${RANDOM_FILES_TEST})

target_link_libraries(sike_weierstrass "${GMP}")
target_link_libraries(sike_weierstrass_for_test "${GMP}")

add_library(sikep503_weierstrass src/api503.c)
add_library(sikep751_weierstrass src/api751.c)
add_library(sikep964_weierstrass src/api964.c)


enable_testing()

add_subdirectory(test)
