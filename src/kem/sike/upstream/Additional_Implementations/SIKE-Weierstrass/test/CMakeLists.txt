add_executable(sike_test test.c printing.c printing.h test_arith.c test_arith.h test_sike.c test_sike.h test_extras.c test_extras.h)

target_link_libraries(sike_test sike_weierstrass)

add_test(SIKEp503-arithmetic sike_test SIKEp503 arith)
add_test(SIKEp503-sidh sike_test SIKEp503 sidh_int)
add_test(SIKEp503-pke sike_test SIKEp503 pke)
add_test(SIKEp503-sike sike_test SIKEp503 sike)
add_test(SIKEp503-sike-int sike_test SIKEp503 sike_int)

add_test(SIKEp751-arithmetic sike_test SIKEp751 arith)
add_test(SIKEp751-sidh sike_test SIKEp751 sidh_int)
add_test(SIKEp751-pke sike_test SIKEp751 pke)
add_test(SIKEp751-sike sike_test SIKEp751 sike)
add_test(SIKEp751-sike-int sike_test SIKEp751 sike_int)

add_test(SIKEp964-arithmetic sike_test SIKEp964 arith)
add_test(SIKEp964-sidh sike_test SIKEp964 sidh_int)
add_test(SIKEp964-pke sike_test SIKEp964 pke)
add_test(SIKEp964-sike sike_test SIKEp964 sike)
add_test(SIKEp964-sike-int sike_test SIKEp964 sike_int)

# Note:
# The TARGET defines are only used for speedtest. libsike_ref is portable and doesn't depend on them.

set(OS_UNIX 1)
set(OS_WIN 2)
set(OS_OTHER 3)

if (UNIX)
    add_definitions(-DOS_TARGET=${OS_UNIX})
elseif(WIN32)
    add_definitions(-DOS_TARGET=${OS_WIN})
else()
    add_definitions(-DOS_TARGET=${OS_OTHER})
endif()

set(TARGET_ARM64 1)
set(TARGET_ARM 2)
set(TARGET_AMD64 3)
set(TARGET_x86 4)
set(TARGET_OTHER 5)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
    add_definitions(-DTARGET=${TARGET_ARM64})
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_definitions(-DTARGET=${TARGET_ARM})
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    add_definitions(-DTARGET=${TARGET_AMD64})
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i386" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
    add_definitions(-DTARGET=${TARGET_x86})
else()
    add_definitions(-DTARGET=${TARGET_OTHER})
endif()