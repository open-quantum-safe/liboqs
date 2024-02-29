#!/bin/bash
# tc3-apple-build-wrapper.sh, ABr
#
# TripleCyber build support for liboqs

# top-level settings
the_openssl_ver="${the_openssl_ver:-3.2.1}"
the_openssl_dir="${the_openssl_dir:-$HOME/proj/git/src/triplecyber.visualstudio.com/abruce-dev/Tc32/External/openssl}"
the_ios_target="${the_ios_target:-17.0}"
the_android_api_level="${the_android_api_level:-34}"

# locate script source directory
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do
  # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
g_SCRIPT_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
the_top_dir="$g_SCRIPT_DIR"

# assume the build directory
the_build_dir_name='build'
the_build_dir_path="$the_top_dir/$the_build_dir_name"

# assume the .CMake folder
the_cmake_dir_name='.CMake'
the_cmake_dir_path="$the_top_dir/$the_cmake_dir_name"

##############################################################
# APPLE build support

# perform a single apple cmake build
function build_apple_variant {
  local i_device="$1" ; shift
  local i_arch="$1" ; shift
  local i_platform="$1" ; shift

  # locals
  local l_rc=0
  local l_platform='apple'
  local l_openssl_plat_dir="$the_openssl_dir/$l_platform/$the_openssl_ver/$i_device"

  echo "BUILD: $l_platform ($i_device / $i_arch)..."

  # locate back to script home
  cd "$the_top_dir" || return $?

  # create directory and change - on errors we are done
  mkdir -p "$the_build_dir_path"/$l_platform/$i_device/$i_arch
  cd "$the_build_dir_path/$l_platform/$i_device/$i_arch" || return $?

  # clear directory and rebuild
  rm -fR ./*
  cmake \
    -DCMAKE_TOOLCHAIN_FILE="$the_cmake_dir_path"/apple.cmake  \
    -DPLATFORM=$i_platform \
    -DDEPLOYMENT_TARGET=$the_ios_target \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_USE_SHA3_OPENSSL=ON \
    -DOPENSSL_USE_STATIC_LIBS=ON \
    -DOPENSSL_ROOT_DIR="$l_openssl_plat_dir" \
    "$the_top_dir"
  l_rc=$? ; [ $l_rc -ne 0 ] && return $l_rc
  cmake --build . || return $?
  echo ''
  return 0
}

# build single fat apple library containing multiple architectures
function build_apple_fatlib {
  local i_device="$1" ; shift
  local i_archs="$1" ; shift
  local i_lib_name="$1" ; shift

  # local args
  local l_platform='apple'
  local l_args=''
  local l_device_dir="$the_build_dir_path/$l_platform/$i_device"

  echo "LIPO: $i_device / $i_lib_name..."

  # change to the device folder and create lib folder
  cd "$l_device_dir" || return $?
  mkdir -p ./lib
  [ ! -d ./lib ] && echo "Unable to create '$l_device_dir/lib'" && return 1

  # build args and execute
  for i_arch in `echo "$i_archs"` ; do
    l_args="${l_args}./$i_arch/lib/$i_lib_name "
  done
  eval xcrun lipo ${l_args}-create -output ./lib/$i_lib_name || return $?
  lipo -info ./lib/$i_lib_name
  ls -la ./lib/$i_lib_name
  echo ''
  return 0
}

# build multiple fat apple libraries containing multiple architectures
function build_apple_fatlibs {
  local i_device="$1" ; shift
  local i_archs="$1" ; shift
  local i_lib_names="$1" ; shift

  # call the helper function once per fatlib name
  for i_lib_name in `echo "$i_lib_names"` ; do
    build_apple_fatlib $i_device "$i_archs" $i_lib_name || return $?
  done
  return 0
}

# build standard set of fat apple libraries containing multiple architectures
function build_apple_fatlibs_std {
  local i_device="$1" ; shift
  local i_archs="$1" ; shift

  build_apple_fatlibs "$i_device" "$i_archs" 'liboqs.a liboqs-internal.a' || return $?
  return 0
}

# build all known apple variants
function build_apple {
  local l_device=''

  l_device='macosx'
  build_apple_variant $l_device x86_64 MAC || return $?
  build_apple_variant $l_device arm64 MAC_ARM64 || return $?
  build_apple_fatlibs_std $l_device 'x86_64 arm64' || return $?

  l_device='iphonesimulator'
  build_apple_variant $l_device x86_64 SIMULATOR64 || return $?
  build_apple_variant $l_device arm64 SIMULATORARM64 || return $?
  build_apple_fatlibs_std $l_device 'x86_64 arm64' || return $?

  l_device='iphoneos'
  build_apple_variant $l_device arm64 OS64 || return $?
  build_apple_fatlibs_std $l_device arm64 || return $?
  return 0
}

##############################################################
# ANDROID build support
function build_android_variant {
  local i_arch="$1" ; shift

  # locals
  local l_rc=0
  local l_platform='android'
  local l_openssl_plat_dir=$the_openssl_dir/$l_platform/$the_openssl_ver/$i_arch

  echo "BUILD: $l_platform ($i_arch)..."

  # locate back to script home
  cd "$the_top_dir" || return $?

  # create directory and change - on errors we are done
  mkdir -p "$the_build_dir_path"/$l_platform/$i_arch
  cd "$the_build_dir_path"/$l_platform/$i_arch || return $?

  # clear directory and rebuild
  rm -fR ./*
  set -x
  #  -DOPENSSL_SSL_LIBRARY="$l_openssl_plat_dir/lib/libssl.a" \
  #  -DOPENSSL_CRYPTO_LIBRARY="$l_openssl_plat_dir/lib/libcrypto.a" \
  cmake --trace-expand \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME"/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=$i_arch \
    -DANDROID_PLATFORM=android-$the_android_api_level \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_USE_SHA3_OPENSSL=ON \
    -DOPENSSL_USE_STATIC_LIBS=ON \
    -DOPENSSL_ROOT_DIR="$l_openssl_plat_dir" \
    -DOPENSSL_INCLUDE_DIR="$l_openssl_plat_dir/openssl/include" \
    "$the_top_dir"
  l_rc=$? ; set +x ; [ $l_rc -ne 0 ] && return $l_rc
  cmake --build . || return $?
  echo ''
  return 0
}

##############################################################
# PEP
function do_main {
  build_apple || return $?
}

l_do_run=1
if [ "x$1" != "x" ]; then
  [ "x$1" = "xsource-only" ] && l_do_run=0
fi
if [ $l_do_run -eq 1 ]; then
  [ x"$1" = x ] && l_func='do_main' || l_func="$1"
  [ x"$l_func" != x ] && eval "$l_func" "$@" || true
else
  true
fi

