# SPDX-License-Identifier: MIT

try_run(RUN_RESULT COMPILE_RESULT
        "${CMAKE_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/.CMake/detect_gcc_clang_intrinsics.c"
        COMPILE_DEFINITIONS ${OQS_OPT_FLAG}
        COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT
        RUN_OUTPUT_VARIABLE RUN_OUTPUT)
if(NOT COMPILE_RESULT)
     message(FATAL_ERROR "Could not compile .CMake/detect_gcc_clang_intrinsics.c" ${COMPILE_OUTPUT})
endif()
if(NOT RUN_RESULT EQUAL 0)
	if(CMAKE_CROSSCOMPILING)
		message(STATUS "Detecting language features in cross-compiling mode impossible. Setting all CPU features OFF.")
	else()
     		message(FATAL_ERROR ".CMake/detect_gcc_clang_intrinsics.c returned exit code: " ${RUN_RESULT})
     	endif()
endif()
foreach(CPU_EXTENSION ${RUN_OUTPUT})
     if (NOT DEFINED OQS_USE_${CPU_EXTENSION}_INSTRUCTIONS)
          set(OQS_USE_${CPU_EXTENSION}_INSTRUCTIONS ON)
     endif()
endforeach()
if(OQS_USE_AVX512BW_INSTRUCTIONS AND
   OQS_USE_AVX512DQ_INSTRUCTIONS AND
   OQS_USE_AVX512F_INSTRUCTIONS)
    set(OQS_USE_AVX512_INSTRUCTIONS ON)
endif()
