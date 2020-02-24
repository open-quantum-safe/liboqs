# TODO: Use cpu_features instead of list-cpu-extensions.c
if(ARCH STREQUAL "x86" OR
   ARCH STREQUAL "x86_64")
    try_run(RUN_RESULT COMPILE_RESULT
            "${CMAKE_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/.CMake/list-cpu-extensions.c"
            RUN_OUTPUT_VARIABLE RUN_OUTPUT)
    foreach(CPU_EXTENSION ${RUN_OUTPUT})
        set(OQS_USE_${CPU_EXTENSION}_INSTRUCTIONS TRUE)
    endforeach()
    if(OQS_USE_AVX512BW_INSTRUCTIONS AND
       OQS_USE_AVX512DQ_INSTRUCTIONS AND
       OQS_USE_AVX512F_INSTRUCTIONS)
       set(OQS_USE_AVX512_INSTRUCTIONS ON)
    endif()
endif()
