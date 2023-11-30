# SPDX-License-Identifier: MIT

# Only add liboqs Zephyr module if enabled in Kconfig
if(CONFIG_LIBOQS)
        # Workarounds for Zephyr
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
                # Workaround as the generic name "arm" is not a supported architecture in liboqs.
                # In Zephyr, however, it is exclusively used for 32-bit ARM architectures.
                set(CMAKE_SYSTEM_PROCESSOR "armv7")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "posix")
                # Workaround to enable the native Zephyr builds on the Linux host system.
                if(BOARD MATCHES "native_posix|native_sim")
                        set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
                else()
                        message(FATAL_ERROR "Unsupported board ${BOARD} with posix architecture")
                endif()
        endif()

        # Configuration for liboqs
        set(OQS_DIST_BUILD  OFF)
        set(OQS_BUILD_ONLY_LIB ON)
        set(OQS_USE_OPENSSL OFF)
        set(OQS_EMBEDDED_BUILD ON)

        set(CMAKE_CROSSCOMPILING ON)
        set(CMAKE_SIZEOF_VOID_P 4) # Not really needed, only for surpressing unrelated warnings

        # Disable features by hand, as CMake won't find them properly with Zephyr
        set(CMAKE_HAVE_GETENTROPY OFF)
        set(CMAKE_HAVE_ALIGNED_ALLOC OFF)
        set(CMAKE_HAVE_POSIX_MEMALIGN OFF)
        set(CMAKE_HAVE_MEMALIGN OFF)
        set(CMAKE_HAVE_EXPLICIT_BZERO OFF)
        set(CMAKE_HAVE_MEMSET_S OFF)
        set(CC_SUPPORTS_WA_NOEXECSTACK OFF)
        set(LD_SUPPORTS_WL_Z_NOEXECSTACK OFF)

        # Add the actual liboqs targets
        add_subdirectory(.. build)

        # Add target specific options to all liboqs targets
        zephyr_get_targets(.. "STATIC_LIBRARY;OBJECT_LIBRARY" ALL_TARGETS)
        foreach(target ${ALL_TARGETS})
                # Zephyr include directories
                target_include_directories(${target} PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_INCLUDE_DIRECTORIES>
                )

                # Zephyr system include directories
                target_include_directories(${target} SYSTEM PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
                )

                # Definitions
                target_compile_definitions(${target} PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_DEFINITIONS>
                )

                # Compile options (includes compiler flags)
                target_compile_options(${target} PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_OPTIONS>
                        $<TARGET_PROPERTY:compiler,no_builtin>
                )

                # liboqs depends on unistd.h, which ultimately needs the generated syscall_list.h file,
                # which is generated as part of ${SYSCALL_LIST_H_TARGET} target. Therefore, we have to
                # make sure that target is built before liboqs.
                add_dependencies(${target} ${SYSCALL_LIST_H_TARGET})

                # We don't want position independent code
                set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE OFF)
        endforeach()

        # Link the liboqs library
        zephyr_link_libraries(oqs)

        # Include the liboqs headers
        zephyr_include_directories(${CMAKE_CURRENT_BINARY_DIR}/build/include)

        if(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7")
                # Undo the workaround from above to not interfere with other modules
                set(CMAKE_SYSTEM_PROCESSOR "arm")
        elseif(CMAKE_SYSTEM_PROCESSOR EQUAL CMAKE_HOST_SYSTEM_PROCESSOR)
                # Undo the workaround from above to not interfere with other modules
                set(CMAKE_SYSTEM_PROCESSOR "posix")
        endif()
endif()