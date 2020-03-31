set(CPU_FEATURES-ARGS -DCMAKE_POLICY_DEFAULT_CMP0063=NEW
                      -DCMAKE_C_VISIBILITY_PRESET=hidden
                      -DBUILD_PIC=ON)
if(CMAKE_CROSSCOMPILING)
    set(CPU_FEATURES-ARGS ${CPU_FEATURES-ARGS}
                          -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
                          -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR})
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(CPU_FEATURES-ARGS ${CPU_FEATURES-ARGS}
                              -DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER})
    endif()
endif()
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in ${CMAKE_CURRENT_BINARY_DIR}/cpu_features-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
    RESULT_VARIABLE CPUFEATURES_STEP_RESULT
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cpu_features-download)
if(CPUFEATURES_STEP_RESULT)
    message(FATAL_ERROR "CMake step for cpu_features failed: ${CPUFEATURES_STEP_RESULT}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE CPUFEATURES_STEP_RESULT
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cpu_features-download)
if(CPUFEATURES_STEP_RESULT)
    message(FATAL_ERROR "Build step for cpu_features failed: ${CPUFEATURES_STEP_RESULT}")
endif()

set(CPUFEATURES_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpu_features-src/include")
find_library(CPUFEATURES_LIBRARY
    NAMES cpu_features
    PATHS "${CMAKE_CURRENT_BINARY_DIR}/cpu_features-build"
    NO_DEFAULT_PATH)
add_library(CpuFeature::cpu_features UNKNOWN IMPORTED GLOBAL)
set_target_properties(CpuFeature::cpu_features PROPERTIES IMPORTED_LOCATION "${CPUFEATURES_LIBRARY}")
