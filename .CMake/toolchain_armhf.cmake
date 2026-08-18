# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm32v7)
set(CMAKE_CROSSCOMPILING ON)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CROSSCOMPILING_EMULATOR "qemu-arm-static;-L;/usr/arm-linux-gnueabihf/")
