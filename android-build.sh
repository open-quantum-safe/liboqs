#!/bin/bash

ANDROID_NDK_DIR=/ram/arm

COMPILE_ARCHITECTURES=("arm64")
OLD_PATH=${PATH}
for ARCH in ${COMPILE_ARCHITECTURES[@]}
do
  COMPILER_PREFIX=""

  export ANDROID_NDK_ROOT="/ram/arm"

  ANDROID_NDK_BIN="${ANDROID_NDK_ROOT}/bin"
  ANDROID_SYSROOT_DIR="${ANDROID_NDK_ROOT}/sysroot"

  export PATH="${ANDROID_NDK_BIN}:${OLD_PATH}"

  export CFLAGS="-O3 -lm"

  case ${ARCH} in
    "arm64" )
      COMPILER_PREFIX=aarch64-linux-android
      ;;
  esac


  #export CC=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-gcc
  #export CPP=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-cpp
  #export CXX=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-g++
  #export LD=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-ld
  #export AR=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-ar
  #export RANLIB=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-ranlib
  #export STRIP=${ANDROID_NDK_BIN}/${COMPILER_PREFIX}-strip

  #echo "---- Compiling for ${ARCH}"

  #make distclean
  ./configure --host="${COMPILER_PREFIX}" --disable-aes-ni --with-sysroot=${ANDROID_SYSROOT_DIR}
  #make

done

export PATH="${OLD_PATH}"

