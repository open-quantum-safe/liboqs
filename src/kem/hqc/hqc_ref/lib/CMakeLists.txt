# lib/CMakeLists.txt
# Build all third-party libraries in lib/

# FIPS202 implementation
file(GLOB FIPS202_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/fips202/fips202.c"
)
add_library(fips202 STATIC ${FIPS202_SRCS})

target_include_directories(fips202 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/fips202
)
