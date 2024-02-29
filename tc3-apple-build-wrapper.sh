#!/bin/bash
# tc3-apple-build-wrapper.sh, ABr
#
# TripleCyber build support for liboqs

# top-level settings
the_openssl_ver="${the_openssl_ver:-3.2.1}"
the_openssl_dir="${the_openssl_dir:-$HOME/proj/git/src/triplecyber.visualstudio.com/abruce-dev/Tc32/External/openssl}"
the_ios_target="${the_ios_target:-17.0}"
the_android_api_level="${the_android_api_level:-34}"

# enable debug to get explicit compiler command lines
the_cmake_build_verbose_flag="${the_cmake_build_verbose_flag:-0}"
the_cmake_build_verbose_option=''
[ x"$the_cmake_build_verbose_flag" = x1 ] && the_cmake_build_verbose_option='--verbose'

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

# the export folder is underneath build to be ignored by git
the_export_dir_name='export'
the_export_dir_path="$the_build_dir_path"/$the_export_dir_name

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

  # create directory and clear - on errors we are done
  mkdir -p "$the_build_dir_path"/$l_platform/$i_device/$i_arch
  cd "$the_build_dir_path/$l_platform/$i_device/$i_arch" || return $?
  rm -fR ./*

  # the apple.cmake toolchain is managed by the liboqs team - so use it
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
  cmake --build . $the_cmake_build_verbose_option || return $?
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

  # create directory and clear - on errors we are done
  mkdir -p "$the_build_dir_path"/$l_platform/$i_arch
  cd "$the_build_dir_path"/$l_platform/$i_arch || return $?
  rm -fR ./*

  # NOTES:
  # * we *must* use the Android NDK toolchain; it is not provided by liboqs
  # * default cmake toolchain fails with names like 'arm64-v8a' with dashes
  set -x
  cmake \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME"/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=$i_arch \
    -DANDROID_PLATFORM=android-$the_android_api_level \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_USE_SHA3_OPENSSL=ON \
    -DOPENSSL_USE_STATIC_LIBS=ON \
    -DOPENSSL_ROOT_DIR="$l_openssl_plat_dir" \
    -DOPENSSL_INCLUDE_DIR="$l_openssl_plat_dir/include" \
    -DOPENSSL_SSL_LIBRARY="$l_openssl_plat_dir/lib/libssl.a" \
    -DOPENSSL_CRYPTO_LIBRARY="$l_openssl_plat_dir/lib/libcrypto.a" \
    "$the_top_dir"
  l_rc=$? ; set +x ; [ $l_rc -ne 0 ] && return $l_rc
  cmake --build . $the_cmake_build_verbose_option || return $?
  echo ''
  return 0
}

# build all known android variants
function build_android {
  for i in arm64-v8a x86_64 x86 armeabi-v7a ; do
    build_android_variant $i || return $?
  done
  return 0
}

##############################################################
# EXPORT functions

# verify expected folder is created
function verify_folder {
  [ -d "$1" ] && return 0
  echo "ERROR: '$1' (missing required folder; rebuild necessary?)"
}

# verify all expected folders are created
function verify_folders {
  # locate back to script home
  cd "$the_top_dir" || return $?

  # make sure that all known folders are created
  l_platform='apple'
  verify_folder "$the_build_dir_path"/$l_platform
  for l_device in iphoneos iphonesimulator macosx ; do
    verify_folder "$the_build_dir_path"/$l_platform/$l_device
    verify_folder "$the_build_dir_path"/$l_platform/$l_device/lib
    for l_arch in arm64 ; do
      verify_folder "$the_build_dir_path"/$l_platform/$l_device/$l_arch
      verify_folder "$the_build_dir_path"/$l_platform/$l_device/$l_arch/include
    done
  done

  l_platform='android'
  verify_folder "$the_build_dir_path"/$l_platform
  for l_arch in arm64-v8a armeabi-v7a x86 x86_64 ; do
    verify_folder "$the_build_dir_path"/$l_platform/$l_arch
    verify_folder "$the_build_dir_path"/$l_platform/$l_arch/include
    verify_folder "$the_build_dir_path"/$l_platform/$l_arch/lib
  done

  return 0
}

# extract version from the generated compiled library
function get_oqs_version {
  # must have all expected folders
  verify_folders || return $?

  # the version is injected into the generated oqsconfig.h;
  # thus we can read from any folder.
  # we will read from android/arm64-v8a
  local l_oqs_config_h="$the_build_dir_path/android/arm64-v8a/include/oqs/oqsconfig.h"
  [ ! -s "$l_oqs_config_h" ] && echo "ERROR: Empty or missing '$l_oqs_config_h' (cannot get version)"

  # extract the version
  local l_version="`cat "$l_oqs_config_h" | grep OQS_VERSION_TEXT | awk '{print $3}' | tr -d '"' | xargs | dos2unix`"
  [ x"$l_version" = x ] && echo "ERROR: Unable to read OQS_VERSION_TEXT from '$l_lqs_config'" && return 1
  echo "$l_version"
  return 0
}

# create a single exported folder
function create_export_folder {
  local i_lib_dir="$1" ; shift
  local i_include_dir="$1" ; shift

  # library first
  mkdir -p "$the_export_dir_path/$i_lib_dir"
  [ ! -d "$the_export_dir_path/$i_lib_dir" ] && echo "ERROR: Missing '$the_export_dir_path/$i_lib_dir' (mkdir failure?)" && return 2
  rm -fR "$the_export_dir_path/$i_lib_dir"/* 
  cp -R "$the_build_dir_path/$i_lib_dir"/* "$the_export_dir_path/$i_lib_dir/" || return $?

  # now each include folder
  while [ x"$i_include_dir" != x ] ; do
    mkdir -p "$the_export_dir_path/$i_include_dir"
    [ ! -d "$the_export_dir_path/$i_include_dir" ] && echo "ERROR: Missing '$the_export_dir_path/$i_include_dir' (mkdir failure?)" && return 2
    rm -fR "$the_export_dir_path/$i_include_dir"/* 
    cp -R "$the_build_dir_path/$i_include_dir"/* "$the_export_dir_path/$i_include_dir/" || return $?

    # next include
    i_include_dir="$1" ; shift
  done

  return 0
}

# create single-level export folder to gather everything
function do_export {
  echo "EXPORT: Begin..."

  # locate back to script home
  cd "$the_top_dir" || return $?

  # get the version - error on failure
  local l_version=`get_oqs_version`
  local l_rc=$?
  if [ $l_rc -ne 0 ] ; then
    # on error, l_version contains the failure message
    echo "$l_version"
    return $l_rc
  fi

  # create the export folder
  mkdir -p "$the_export_dir_path"
  cd "$the_export_dir_path" || return $?

  # load in from everything...
  create_export_folder android/arm64-v8a/lib android/arm64-v8a/include || return $?
  create_export_folder android/armeabi-v7a/lib android/armeabi-v7a/include || return $?
  create_export_folder android/x86/lib android/x86/include || return $?
  create_export_folder android/x86_64/lib android/x86_64/include || return $?
  create_export_folder apple/iphoneos/lib apple/iphoneos/arm64/include || return $?
  create_export_folder apple/iphonesimulator/lib apple/iphonesimulator/arm64/include apple/iphonesimulator/x86_64/include || return $?
  create_export_folder apple/macosx/lib apple/macosx/arm64/include apple/macosx/x86_64/include || return $?

  # report on what was exported
  echo ''
  echo "VERSION: $l_version"
  find "$the_export_dir_path" -type d -name '*' -exec ls -lad {} \;
  find "$the_export_dir_path"/apple -type f -name '*.a' -exec lipo -info {} \;
  return 0
}

##############################################################
# PEP
function do_main {
  build_android || return $?
  build_apple || return $?
  do_export || return $?
  return 0
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

