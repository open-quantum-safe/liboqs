# SPDX-License-Identifier: MIT

set(_SLH_DSA_OBJS "")

if (OQS_ENABLE_SIG_SLH_DSA)
	add_library(slh_dsa_c OBJECT
	            slh_dsa_c/sha2_256.c
	            slh_dsa_c/sha2_512.c
	            slh_dsa_c/sha3_api.c
	            slh_dsa_c/sha3_f1600.c
	            slh_dsa_c/slh_dsa.c
	            slh_dsa_c/slh_prehash.c
	            slh_dsa_c/slh_sha2.c
	            slh_dsa_c/slh_shake.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_sha2_128f.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_sha2_128s.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_sha2_192f.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_sha2_192s.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_sha2_256f.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_sha2_256s.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_shake_128f.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_shake_128s.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_shake_192f.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_shake_192s.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_shake_256f.c
	            wrappers/prehash_sha2_224/slh_dsa_sha2_224_prehash_shake_256s.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_sha2_128f.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_sha2_128s.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_sha2_192f.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_sha2_192s.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_sha2_256f.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_sha2_256s.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_shake_128f.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_shake_128s.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_shake_192f.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_shake_192s.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_shake_256f.c
	            wrappers/prehash_sha2_256/slh_dsa_sha2_256_prehash_shake_256s.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_sha2_128f.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_sha2_128s.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_sha2_192f.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_sha2_192s.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_sha2_256f.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_sha2_256s.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_shake_128f.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_shake_128s.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_shake_192f.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_shake_192s.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_shake_256f.c
	            wrappers/prehash_sha2_384/slh_dsa_sha2_384_prehash_shake_256s.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_sha2_128f.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_sha2_128s.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_sha2_192f.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_sha2_192s.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_sha2_256f.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_sha2_256s.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_shake_128f.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_shake_128s.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_shake_192f.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_shake_192s.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_shake_256f.c
	            wrappers/prehash_sha2_512/slh_dsa_sha2_512_prehash_shake_256s.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_sha2_128f.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_sha2_128s.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_sha2_192f.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_sha2_192s.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_sha2_256f.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_sha2_256s.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_shake_128f.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_shake_128s.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_shake_192f.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_shake_192s.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_shake_256f.c
	            wrappers/prehash_sha2_512_224/slh_dsa_sha2_512_224_prehash_shake_256s.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_sha2_128f.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_sha2_128s.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_sha2_192f.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_sha2_192s.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_sha2_256f.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_sha2_256s.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_shake_128f.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_shake_128s.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_shake_192f.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_shake_192s.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_shake_256f.c
	            wrappers/prehash_sha2_512_256/slh_dsa_sha2_512_256_prehash_shake_256s.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_sha2_128f.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_sha2_128s.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_sha2_192f.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_sha2_192s.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_sha2_256f.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_sha2_256s.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_shake_128f.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_shake_128s.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_shake_192f.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_shake_192s.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_shake_256f.c
	            wrappers/prehash_sha3_224/slh_dsa_sha3_224_prehash_shake_256s.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_sha2_128f.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_sha2_128s.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_sha2_192f.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_sha2_192s.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_sha2_256f.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_sha2_256s.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_shake_128f.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_shake_128s.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_shake_192f.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_shake_192s.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_shake_256f.c
	            wrappers/prehash_sha3_256/slh_dsa_sha3_256_prehash_shake_256s.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_sha2_128f.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_sha2_128s.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_sha2_192f.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_sha2_192s.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_sha2_256f.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_sha2_256s.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_shake_128f.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_shake_128s.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_shake_192f.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_shake_192s.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_shake_256f.c
	            wrappers/prehash_sha3_384/slh_dsa_sha3_384_prehash_shake_256s.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_sha2_128f.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_sha2_128s.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_sha2_192f.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_sha2_192s.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_sha2_256f.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_sha2_256s.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_shake_128f.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_shake_128s.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_shake_192f.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_shake_192s.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_shake_256f.c
	            wrappers/prehash_sha3_512/slh_dsa_sha3_512_prehash_shake_256s.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_sha2_128f.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_sha2_128s.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_sha2_192f.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_sha2_192s.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_sha2_256f.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_sha2_256s.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_shake_128f.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_shake_128s.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_shake_192f.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_shake_192s.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_shake_256f.c
	            wrappers/prehash_shake_128/slh_dsa_shake_128_prehash_shake_256s.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_sha2_128f.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_sha2_128s.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_sha2_192f.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_sha2_192s.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_sha2_256f.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_sha2_256s.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_shake_128f.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_shake_128s.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_shake_192f.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_shake_192s.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_shake_256f.c
	            wrappers/prehash_shake_256/slh_dsa_shake_256_prehash_shake_256s.c
	            wrappers/pure/slh_dsa_pure_sha2_128f.c
	            wrappers/pure/slh_dsa_pure_sha2_128s.c
	            wrappers/pure/slh_dsa_pure_sha2_192f.c
	            wrappers/pure/slh_dsa_pure_sha2_192s.c
	            wrappers/pure/slh_dsa_pure_sha2_256f.c
	            wrappers/pure/slh_dsa_pure_sha2_256s.c
	            wrappers/pure/slh_dsa_pure_shake_128f.c
	            wrappers/pure/slh_dsa_pure_shake_128s.c
	            wrappers/pure/slh_dsa_pure_shake_192f.c
	            wrappers/pure/slh_dsa_pure_shake_192s.c
	            wrappers/pure/slh_dsa_pure_shake_256f.c
	            wrappers/pure/slh_dsa_pure_shake_256s.c
	            )
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/slh_dsa_c)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/pure)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha2_224)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha2_256)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha2_384)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha2_512)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha2_512_224)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha2_512_256)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha3_224)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha3_256)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha3_384)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_sha3_512)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_shake_128)
    target_include_directories(slh_dsa_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/wrappers/prehash_shake_256)
    
    set(_SLH_DSA_OBJS ${_SLH_DSA_OBJS} $<TARGET_OBJECTS:slh_dsa_c>)
endif()

set(SLH_DSA_OBJS ${_SLH_DSA_OBJS} PARENT_SCOPE)