# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm64v8)
set(CMAKE_CROSSCOMPILING ON)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CROSSCOMPILING_EMULATOR "qemu-aarch64-static;-L;/usr/aarch64-linux-gnu/")
