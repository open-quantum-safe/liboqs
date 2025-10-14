# SPDX-License-Identifier: MIT

if(OQS_ENABLE_KEM_ntruplus_701)
    set(SRCS kem_ntruplus.c ntruplus.c)
    add_library(ntruplus OBJECT ${SRCS})
    target_include_directories(ntruplus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(ntruplus PRIVATE ${PROJECT_SOURCE_DIR}/src/common)
endif()