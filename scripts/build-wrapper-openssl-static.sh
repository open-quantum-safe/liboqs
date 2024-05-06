#!/bin/bash
# build-wrapper-openssl-static.sh, ABr
#
# Build support for static liboqs library for macOS / iOS / android / linux / windows.
# Specifically created to support building oqs-provider.
#
# Usage:
#   ./scripts/build-wrapper-openssl-static.sh [target]
#     [target] - do_main: Build all, create export
#              - [function]: Any function such as build_apple_macos
#
# On success, the generated [build/export] folder below can be used as
# input to the oqs-provider build.
#
# Output:
# ./build - output folder
# \-> android
#     \-> [archs] - one of arm64-v8a / armeabi-v7a / x86 / x86_64
#         \-> [output] - cmake output and build files
# \-> apple
#     \-> [device] - one of macos / iphoneos / iphonesimulator
#         \-> lib - contains fat lib with all archs
#         \-> [archs] - one of arm64 / x86_64 architecture
#             \-> [output] - cmake output and build files
# \-> export - packaged output
#     \-> android
#         \-> [version] - version automatically determined from build output
#             \-> [archs] - one of arm64-v8a / armeabi-v7a / x86 / x86_64
#                 \-> include - architecture-specific headers
#                 \-> lib - static library output
#     \-> apple
#         \-> [version] - version automatically determined from build output
#             \-> [device] - one of macos / iphoneos / iphonesimulator
#                 \-> lib - contains fat lib with all archs
#                 \-> [archs] - one of arm64 / x86_64 architecture
#                     \-> include - architecture-specific headers
#
# Requires the use of openssl assumed to be available as:
# $the_libs_dir
# \-> openssl-$the_openssl_ver-$the_type-$the_arch - version info such as 3.2.1
#         \-> [archs] - one of arm64-v8a / armeabi-v7a / x86 / x86_64
#             \-> include - headers specific to architecture
#             \-> lib - contains libcrypto.a / libssl.a
# \-> apple
#     \-> $the_openssl_ver - version info such as 3.2.1
#         \-> [device] - one of macos / iphoneos / iphonesimulator
#             \-> include - headers specific to device (note: not by arch)
#             \-> lib - contains fat lib with all archs

# top-level settings - modify in environment from defaults listed here
#set -x
the_openssl_ver="${the_openssl_ver}"
the_libs_dir="${the_libs_dir}"
the_ios_target="${the_ios_target}"
the_macos_target="${the_macos_target}"
the_android_api_level="${the_android_api_level}"
the_oqs_algs_enabled="${the_oqs_algs_enabled}"
set +x

[ x"$the_openssl_ver" = x ] && echo 'Missing the_openssl_ver' && exit 1
[ x"$the_libs_dir" = x ] && echo 'Missing the_libs_dir' && exit 1
[ x"$the_ios_target" = x ] && echo 'Missing the_ios_target' && exit 1
[ x"$the_macos_target" = x ] && echo 'Missing the_macos_target' && exit 1
[ x"$the_android_api_level" = x ] && echo 'Missing the_android_api_level' && exit 1
[ x"$the_oqs_algs_enabled" = x ] && echo 'Missing the_oqs_algs_enabled' && exit 1

# enable debug to get explicit compiler command lines
the_cmake_build_verbose_flag="${the_cmake_build_verbose_flag:-0}"
the_cmake_build_verbose_option=''
[ x"$the_cmake_build_verbose_flag" = x1 ] && the_cmake_build_verbose_option='--verbose'

the_cmake_build_trace_flag="${the_cmake_build_trace_flag:-0}"
the_cmake_build_trace_option=''
[ x"$the_cmake_build_trace_flag" = x1 ] && the_cmake_build_trace_option='--trace'

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
the_top_dir="`realpath "$g_SCRIPT_DIR/.."`"

# what is our OS?
os_is_macos=0 ; os_is_linux=0 ; os_is_windows=0
case "`uname -s | xargs`" in
  Darwin) os_is_macos=1;;
  Linux) os_is_linux=1;;
  CYGWIN*) os_is_windows=1;;
esac
[ $os_is_macos -eq 0 -a $os_is_linux -eq 0 -a $os_is_windows -eq 0 ] && echo 'CANNOT IDENTIFY OS' && exit 1

# determine our preferred targets
wants_android=0 ; [ $os_is_macos -eq 1 ] && wants_android=1
wants_apple=0 ; [ $os_is_macos -eq 1 ] && wants_apple=1
wants_linux=0 ; [ $os_is_linux -eq 1 ] && wants_linux=1
wants_windows=0 ; [ $os_is_windows -eq 1 ] && wants_windows=1

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
  local l_type='apple'
  local l_openssl_plat_dir="$the_libs_dir/openssl-$the_openssl_ver-$l_type-$i_device"

  echo "BUILD: $l_type ($i_device / $i_arch)..."

  # locate back to script home
  cd "$the_top_dir" || return $?

  # create directory and clear - on errors we are done
  mkdir -p "$the_build_dir_path"/$l_type/$i_device/$i_arch
  cd "$the_build_dir_path/$l_type/$i_device/$i_arch" || return $?
  rm -fR ./*

  # different deployment target for ios vs. macos
  local l_deployment_target=''
  if [ x"$i_device" = xmacos ] ; then
    l_deployment_target="$the_macos_target"
  else
    l_deployment_target="$the_ios_target"
  fi

  # the apple.cmake toolchain is managed by the liboqs team - so use it
  set -x
  cmake \
    $the_cmake_build_trace_option \
    -DCMAKE_TOOLCHAIN_FILE="$the_cmake_dir_path"/apple.cmake  \
    -DPLATFORM=$i_platform \
    -DDEPLOYMENT_TARGET=$l_deployment_target \
    -DOQS_ALGS_ENABLED=$the_oqs_algs_enabled \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_USE_SHA3_OPENSSL=ON \
    -DOPENSSL_USE_STATIC_LIBS=ON \
    -DOPENSSL_ROOT_DIR="$l_openssl_plat_dir" \
    -B . -S "$the_top_dir"
  l_rc=$? ; set +x ; [ $l_rc -ne 0 ] && return $l_rc
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
  local l_type='apple'
  local l_args=''
  local l_device_dir="$the_build_dir_path/$l_type/$i_device"

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

# build macox
function build_apple_macos {
  local l_device='macos'
  build_apple_variant $l_device x86_64 MAC || return $?
  build_apple_variant $l_device arm64 MAC_ARM64 || return $?
  build_apple_fatlibs_std $l_device 'x86_64 arm64' || return $?
  return 0
}

# build apple simulator
function build_apple_iphonesimulator {
  local l_device='iphonesimulator'
  build_apple_variant $l_device x86_64 SIMULATOR64 || return $?
  build_apple_variant $l_device arm64 SIMULATORARM64 || return $?
  build_apple_fatlibs_std $l_device 'x86_64 arm64' || return $?
  return 0
}

# build apple iphone
function build_apple_iphoneos {
  local l_device='iphoneos'
  build_apple_variant $l_device arm64 OS64 || return $?
  build_apple_fatlibs_std $l_device arm64 || return $?
  return 0
}

# build all known apple variants
function build_apple {
  build_apple_macos || return $?
  build_apple_iphonesimulator || return $?
  build_apple_iphoneos || return $?
  return 0
}

##############################################################
# ANDROID build support
function build_android_variant {
  local i_arch="$1" ; shift

  # locals
  local l_rc=0
  local l_type='android'
  local l_openssl_plat_dir="$the_libs_dir/openssl-$the_openssl_ver-$l_type-$i_arch"

  echo "BUILD: $l_type ($i_arch)..."

  # locate back to script home
  cd "$the_top_dir" || return $?

  # create directory and clear - on errors we are done
  mkdir -p "$the_build_dir_path"/$l_type/$i_arch
  cd "$the_build_dir_path"/$l_type/$i_arch || return $?
  rm -fR ./*

  # NOTES:
  # * we *must* use the Android NDK toolchain; it is not provided by liboqs
  # * default cmake toolchain fails with names like 'arm64-v8a' with dashes;
  #   this is why we *must set* OPENSSL_INCLUDE_DIR, OPENSSL_SSL_LIBRARY, OPENSSL_CRYPTO_LIBRARY
  set -x
  cmake \
    $the_cmake_build_trace_option \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME"/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=$i_arch \
    -DANDROID_PLATFORM=android-$the_android_api_level \
    -DOQS_ALGS_ENABLED=$the_oqs_algs_enabled \
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
  # user can pass in the variant desired
  local l_variants='arm64-v8a x86_64 x86 armeabi-v7a'
  [ x"$1" != x ] && l_variants="$@"
  for i in `echo "$l_variants"` ; do
    build_android_variant $i || return $?
  done
  return 0
}

##############################################################
# LINUX build support
function build_linux_variant {
  local i_arch="$1" ; shift

  # locals
  local l_rc=0
  local l_type='linux'
  local l_libc_ver="`ldd --version | grep libc | awk '{print $4}' | xargs`"
  local l_openssl_plat_dir="$the_libs_dir/openssl-$the_openssl_ver-$l_type-$i_arch-glibc-$l_libc_ver"

  echo "BUILD: $l_type ($i_arch, libc $l_libc_ver)..."

  # locate back to script home
  cd "$the_top_dir" || return $?

  # create directory and clear - on errors we are done
  local l_build_dir_path="$the_build_dir_path"/$l_type/$i_arch
  mkdir -p "$l_build_dir_path"
  cd "$l_build_dir_path" || return $?
  #rm -fR ./*

  set -x
  cmake \
    $the_cmake_build_trace_option \
    -DOQS_ALGS_ENABLED=$the_oqs_algs_enabled \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_USE_SHA3_OPENSSL=ON \
    -DOPENSSL_USE_STATIC_LIBS=ON \
    -DOPENSSL_ROOT_DIR="$l_openssl_plat_dir" \
    -DOPENSSL_INCLUDE_DIR="$l_openssl_plat_dir/include" \
    -DOPENSSL_SSL_LIBRARY="$l_openssl_plat_dir/lib64/libssl.a" \
    -DOPENSSL_CRYPTO_LIBRARY="$l_openssl_plat_dir/lib64/libcrypto.a" \
    "$the_top_dir"
  l_rc=$? ; set +x ; [ $l_rc -ne 0 ] && return $l_rc

  # account for -lz which cannot be appended to link flags
  # without modifying FindOpenSSL.cmake or other source files.
  find "$l_build_dir_path"/tests -type f -name link.txt -exec sed -ie 's/libcrypto.a/libcrypto.a -lz/' {} \;
  find "$l_build_dir_path"/tests -type f -name link.txt -exec cat {} \;

  cmake --build . $the_cmake_build_verbose_option || return $?
  echo ''
  return 0
}

# build all known linux variants
function build_linux {
  # user can pass in the variant desired
  local l_variants='x86_64'
  [ x"$1" != x ] && l_variants="$@"
  for i in `echo "$l_variants"` ; do
    build_linux_variant $i || return $?
  done
  return 0
}

##############################################################
# WINDOWS build support
function build_windows_variant {
  local i_arch="$1" ; shift

  # locals
  local l_rc=0
  local l_type='win'
  local l_openssl_plat_dir="$the_libs_dir/openssl-$the_openssl_ver-$l_type-$i_arch"

  echo "BUILD: $l_type ($i_arch)..."

  # locate msbuild
  local l_msbuild_name='MSBuild.exe'
  if ! which $l_msbuild_name >/dev/null 2>&1 ; then
    set -x
    local l_cygpath_VSINSTALLDIR="`cygpath -au "$VSINSTALLDIR" | xargs | dos2unix`"
    if [ x"$l_cygpath_VSINSTALLDIR" != x ] ; then
      export PATH="${l_cygpath_VSINSTALLDIR}MSBuild/Current/Bin:$PATH"
    fi
  fi
  ! which $l_msbuild_name >/dev/null 2>&1 && echo 'UNABLE_TO_LOCATE_MSBUILD' && return 2
  set +x

  # locate back to script home
  cd "$the_top_dir" || return $?

  # create directory and clear - on errors we are done
  local l_build_dir_path="$the_build_dir_path"/$l_type/$i_arch
  mkdir -p "$l_build_dir_path"
  cd "$l_build_dir_path" || return $?
  rm -fR ./*

  # windows path versions
  local l_top_dir_windows="`cygpath -am "$the_top_dir" | xargs`"
  local l_openssl_root_dir_windows="`cygpath -am "$l_openssl_plat_dir" | xargs`"
  local l_openssl_include_dir_windows="`cygpath -am "$l_openssl_plat_dir/include" | xargs`"
  local l_openssl_ssl_library_windows="`cygpath -am "$l_openssl_plat_dir/lib/libssl.lib" | xargs`"
  local l_openssl_crypto_library_windows="`cygpath -am "$l_openssl_plat_dir/lib/libcrypto.lib" | xargs`"

  echo 'CONFIGURE...'
  set -x
  cmake \
    $the_cmake_build_trace_option \
    -DOQS_ALGS_ENABLED=$the_oqs_algs_enabled \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_USE_SHA3_OPENSSL=ON \
    -DOQS_BUILD_ONLY_LIB=ON \
    -DOPENSSL_USE_STATIC_LIBS=ON \
    -DOPENSSL_ROOT_DIR="$l_openssl_root_dir_windows" \
    -DOPENSSL_INCLUDE_DIR="$l_openssl_include_dir_windows" \
    -DOPENSSL_SSL_LIBRARY="$l_openssl_ssl_library_windows" \
    -DOPENSSL_CRYPTO_LIBRARY="$l_openssl_crypto_library_windows" \
    "$l_top_dir_windows"
  l_rc=$? ; set +x ; [ $l_rc -ne 0 ] && return $l_rc
  echo ''

  echo 'BUILD...'
  set -x
  $l_msbuild_name ALL_BUILD.vcxproj /property:Configuration=Release
  l_rc=$? ; set +x ; [ $l_rc -ne 0 ] && return $l_rc
  echo ''

  echo ''
  return 0
}

# build all known windows variants
function build_windows {
  # user can pass in the variant desired
  local l_variants='x64'
  [ x"$1" != x ] && l_variants="$@"
  for i in `echo "$l_variants"` ; do
    build_windows_variant $i || return $?
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

  local l_type=''

  # make sure that all known folders are created
  if [ $wants_apple -eq 1 ] ; then
    l_type='apple'
    verify_folder "$the_build_dir_path"/$l_type
    for l_device in iphoneos iphonesimulator macos ; do
      verify_folder "$the_build_dir_path"/$l_type/$l_device
      verify_folder "$the_build_dir_path"/$l_type/$l_device/lib
      for l_arch in arm64 ; do
        verify_folder "$the_build_dir_path"/$l_type/$l_device/$l_arch
        verify_folder "$the_build_dir_path"/$l_type/$l_device/$l_arch/include
      done
    done
  fi

  if [ $wants_android -eq 1 ] ; then
    l_type='android'
    verify_folder "$the_build_dir_path"/$l_type
    for l_arch in arm64-v8a armeabi-v7a x86 x86_64 ; do
      verify_folder "$the_build_dir_path"/$l_type/$l_arch
      verify_folder "$the_build_dir_path"/$l_type/$l_arch/include
      verify_folder "$the_build_dir_path"/$l_type/$l_arch/lib
    done
  fi

  if [ $wants_linux -eq 1 ] ; then
    l_type='linux'
    verify_folder "$the_build_dir_path"/$l_type
    for l_arch in x86_64 ; do
      verify_folder "$the_build_dir_path"/$l_type/$l_arch
      verify_folder "$the_build_dir_path"/$l_type/$l_arch/include
      verify_folder "$the_build_dir_path"/$l_type/$l_arch/lib
    done
  fi

  if [ $wants_windows -eq 1 ] ; then
    l_type='win'
    verify_folder "$the_build_dir_path"/$l_type
    for l_arch in x64 ; do
      verify_folder "$the_build_dir_path"/$l_type/$l_arch
      verify_folder "$the_build_dir_path"/$l_type/$l_arch/include
      verify_folder "$the_build_dir_path"/$l_type/$l_arch/lib
    done
  fi

  return 0
}

# extract version from the generated compiled library
function get_oqs_version {
  # must have all expected folders
  verify_folders || return $?

  # the version is injected into the generated oqsconfig.h;
  # thus we can read from any folder.
  # we will read from android/arm64-v8a
  local l_oqs_config_h=''
  [ $wants_android -eq 1 ] && l_oqs_config_h="$the_build_dir_path/android/arm64-v8a/include/oqs/oqsconfig.h"
  [ $wants_apple -eq 1 ] && l_oqs_config_h="$the_build_dir_path/apple/iphoneos/arm64/include/oqs/oqsconfig.h"
  [ $wants_linux -eq 1 ] && l_oqs_config_h="$the_build_dir_path/linux/x86_64/include/oqs/oqsconfig.h"
  [ $wants_windows -eq 1 ] && l_oqs_config_h="$the_build_dir_path/win/x64/include/oqs/oqsconfig.h"
  [ ! -s "$l_oqs_config_h" ] && echo "ERROR: Empty or missing '$l_oqs_config_h' (cannot get version)"

  # extract the version
  local l_version="`cat "$l_oqs_config_h" | grep OQS_VERSION_TEXT | awk '{print $3}' | tr -d '"' | xargs | dos2unix`"
  [ x"$l_version" = x ] && echo "ERROR: Unable to read OQS_VERSION_TEXT from '$l_lqs_config'" && return 1
  echo "$l_version"
  return 0
}

# create a single exported folder
function create_export_folder {
  local i_type="$1" ; shift
  local i_version="$1" ; shift
  local i_lib_dir="$1" ; shift
  local i_include_dir="$1" ; shift

  # top folder to use
  local l_top_folder="$the_export_dir_path/$i_type/$i_version"
  mkdir -p "$l_top_folder"
  cd "$l_top_folder" || return $?

  # library first
  mkdir -p "$l_top_folder/$i_lib_dir"
  [ ! -d "$l_top_folder/$i_lib_dir" ] && echo "ERROR: Missing '$l_top_folder/$i_lib_dir' (mkdir failure?)" && return 2
  rm -fR "$l_top_folder/$i_lib_dir"/* 
  cp -R "$the_build_dir_path/$i_type/$i_lib_dir"/* "$l_top_folder/$i_lib_dir/" || return $?

  # now each include folder
  while [ x"$i_include_dir" != x ] ; do
    mkdir -p "$l_top_folder/$i_include_dir"
    [ ! -d "$l_top_folder/$i_include_dir" ] && echo "ERROR: Missing '$l_top_folder/$i_include_dir' (mkdir failure?)" && return 2
    rm -fR "$l_top_folder/$i_include_dir"/* 
    cp -R "$the_build_dir_path/$i_type/$i_include_dir"/* "$l_top_folder/$i_include_dir/" || return $?

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

  # locals
  local l_type=''

  # load in from everything...
  if [ $wants_android -eq 1 ] ; then
    l_type='android'
    create_export_folder $l_type "$l_version" arm64-v8a/lib arm64-v8a/include || return $?
    create_export_folder $l_type "$l_version" armeabi-v7a/lib armeabi-v7a/include || return $?
    create_export_folder $l_type "$l_version" x86/lib x86/include || return $?
    create_export_folder $l_type "$l_version" x86_64/lib x86_64/include || return $?
  fi

  if [ $wants_apple -eq 1 ] ; then
    l_type='apple'
    create_export_folder $l_type "$l_version" iphoneos/lib iphoneos/arm64/include || return $?
    create_export_folder $l_type "$l_version" iphonesimulator/lib iphonesimulator/arm64/include iphonesimulator/x86_64/include || return $?
    create_export_folder $l_type "$l_version" macos/lib macos/arm64/include macos/x86_64/include || return $?
  fi

  if [ $wants_linux -eq 1 ] ; then
    l_type='linux'
    create_export_folder $l_type "$l_version" x86_64/lib x86_64/include || return $?
  fi

  if [ $wants_windows -eq 1 ] ; then
    l_type='win'
    create_export_folder $l_type "$l_version" x64/lib x64/include || return $?
  fi

  # report on what was exported
  echo ''
  echo "VERSION: $l_version"
  find "$the_export_dir_path" -type d -name '*' -exec ls -lad {} \;
  if [ $wants_apple -eq 1 ] ; then
    find "$the_export_dir_path"/apple/"$l_version" -type f -name '*.a' -exec lipo -info {} \;
  fi
  return 0
}

# some projects (oqs-provider for example) assume that we can point directly to
# liboqs for a build. this is not really true because some files are just not
# in the correct location. this function fixes this.
function fix_cmake_provider {
  echo "FIXUP: Begin..."
  cd "$the_build_dir_path" || return $?

  # oqs-provider accepts a liboqs_DIR option which must point to liboqs folder.
  # normally - this will be to the liboqs installed folder.
  # since we are building from source, in our case we must point liboqs_DIR to
  # the [src] folder created as part of cmake build.
  #
  # example: liboqs_DIR="[liboqs_parent]/liboqs/build/apple/macos/x86_64/src"
  # 
  # oqs-provider assumes that liboqsConfig.cmake is in the liboqs_DIR folder and
  # actively uses that file as part of its own build.
  # but there is a big problem with this: liboqsConfig.cmake includes the file
  # liboqsTargets.cmake which is assumed to be in the same folder as liboqsConfig.cmake.
  # and this is not the case - liboqsTargets.cmake is actually under the [cmake] folder
  # in the generated build's [src] folder.
  # we fix this by creating a symlink so that liboqsTargets.cmake can be found in the
  # same [src] folder as liboqsConfig.cmake.
  local l_path=''
  local l_dir=''
  local l_parent_dir=''
	for i in `find . -type f \( -name liboqsTargets.cmake -o -name liboqsConfig.cmake \)` ; do
    l_path="`realpath "$i"`"
    l_dir="`dirname "$l_path"`"
    l_parent_dir="`dirname "$l_dir"`"
    if [ $os_is_windows -eq 0 ] ; then
      echo "ln -fs '$l_path' '$l_parent_dir'/"
      ln -fs "$l_path" "$l_parent_dir"/
    else
      echo "yes | cp '$l_path' '$l_parent_dir'/"
      yes | cp "$l_path" "$l_parent_dir"/
    fi
  done
  echo ''

  return 0
}

##############################################################
# PEP

# build all targets
function do_main {
cat << EOS
OPTIONS (liboqs):
the_openssl_ver="${the_openssl_ver}"
the_libs_dir="${the_libs_dir}"
the_ios_target="${the_ios_target}"
the_macos_target="${the_macos_target}"
the_android_api_level="${the_android_api_level}"
the_oqs_algs_enabled="${the_oqs_algs_enabled}"
the_cmake_build_verbose_flag="${the_cmake_build_verbose_flag}"
the_cmake_build_trace_flag="${the_cmake_build_trace_flag}"

EOS

  if [ $wants_apple -eq 1 ] ; then build_apple || return $? ; fi
  if [ $wants_android -eq 1 ] ; then build_android || return $? ; fi
  if [ $wants_linux -eq 1 ] ; then build_linux || return $? ; fi
  if [ $wants_windows -eq 1 ] ; then build_windows || return $? ; fi
  do_export || return $?
  fix_cmake_provider || return $?
  return 0
}

l_do_run=1
if [ "x$1" != "x" ]; then
  [ "x$1" = "xsource-only" ] && l_do_run=0
fi
if [ $l_do_run -eq 1 ]; then
  if [ x"$1" = x ] ; then
    l_func='do_main'
  else
    l_func="$1"
    shift
  fi
  [ x"$l_func" != x ] && eval "$l_func" "$@" || true
else
  true
fi

