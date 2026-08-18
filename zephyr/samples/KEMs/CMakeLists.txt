# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(NO_BUILD_TYPE_WARNING ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(liboqs_kems)

target_sources(app PRIVATE
    src/main.c
)
