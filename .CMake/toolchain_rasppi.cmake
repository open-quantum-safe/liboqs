# SPDX-License-Identifier: MIT

# How to use:
# apt install gcc-8-arm-linux-gnueabihf
# cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../.CMake/toolchain_rasppi.cmake -DOQS_USE_OPENSSL=OFF ..

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armhf)
set(CMAKE_CROSSCOMPILING ON)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc-8)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Unconditionally set for this platform
add_definitions( -DOQS_USE_RASPBERRY_PI )

