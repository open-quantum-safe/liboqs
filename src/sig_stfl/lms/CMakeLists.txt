# SPDX-License-Identifier: MIT

set(_LMS_OBJS "")

set(SRCS
         external/endian.c
         external/hash.c
         external/hss.c
         external/hss_alloc.c
         external/hss_aux.c
         external/hss_common.c
         external/hss_compute.c
         external/hss_derive.c
         external/hss_generate.c
         external/hss_keygen.c
         external/hss_param.c
         external/hss_reserve.c
         external/hss_sign.c
         external/hss_sign_inc.c
         external/hss_thread_single.c
         external/hss_verify.c
         external/hss_verify_inc.c
         external/hss_zeroize.c
         external/lm_common.c
         external/lm_ots_common.c
         external/lm_ots_sign.c
         external/lm_ots_verify.c
         external/lm_verify.c
         sig_stfl_lms.c
         sig_stfl_lms_functions.c
   )
    
#if (OQS_ENABLE_SIG_STFL_lms)
#    add_compile_definitions(OQS_ENABLE_SIG_STFL_lms)
#    set (SRCS ${SRCS} sig_stfl_lms.c sig_stfl_lms_functions.c)
#endif()


add_library(lms OBJECT ${SRCS})
target_include_directories(lms PRIVATE ${LIBOQS_ROOT_DIR}/include)
set(_LMS_OBJS ${_LMS_OBJS} $<TARGET_OBJECTS:lms>)
set(LMS_OBJS ${_LMS_OBJS} PARENT_SCOPE)



