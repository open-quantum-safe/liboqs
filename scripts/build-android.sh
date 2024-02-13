#!/bin/bash

# SPDX-License-Identifier: MIT

set -e

show_help() {
    echo ""
    echo " Usage: ./build-android <ndk-dir> -a [abi] -b [build-directory] -s [sdk-version] -f [extra-cmake-flags]"

    echo "   ndk-dir: the directory of the Android NDK (required)"
    echo "   abi: the Android ABI to target for the build"
    echo "   build-directory: the directory in which to build the project"
    echo "   sdk-version: the minimum Android SDK version to target"
    echo "   extra-cmake-flags: extra flags to use for CMake configuration"
    echo ""
    exit 0
}

# If no arguments provided, show help
if [ $# -eq 0 ]
then
    show_help
fi

# If help requested, show help
for arg in "$@"
do
    if [ "$arg" == "--help" ] || [ "$arg" == "-h" ]
    then
        show_help
    fi
done

# Make sure script will work the same if called from
# root directory or scripts directory
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path/.."

NDK=$1
# Verify NDK is valid directory
if [ -d "$NDK" ]
then
    echo "Valid directory for NDK at $NDK"
else
    echo "Directory for NDK doesn't exist at $NDK"
    exit 1
fi

# Parse optional parameters
ABI="armeabi-v7a"
MINSDKVERSION=21
BUILDDIR="build"

OPTIND=2
while getopts "a:s:b:f:" flag
do
    case $flag in
        a) ABI=$OPTARG;;
        s) MINSDKVERSION=$OPTARG;;
        b) BUILDDIR=$OPTARG;;
        f) EXTRAFLAGS="$OPTARG";;
        *) exit 1
    esac
done

# Check ABI is supported
valid_abis=("armeabi-v7a" "arm64-v8a" "x86" "x86_64")
abi_match=false
for i in "${valid_abis[@]}"
do
   :
   if [ "$ABI" == "$i" ]
   then abi_match=true
   fi
done
if [ "$abi_match" = true ]
then
    echo "Compiling for ABI $ABI"
else
    echo "Invalid Android ABI of $ABI"
    echo "Valid ABIs are:"
    printf "%s\\n" "${valid_abis[@]}"
    exit 1
fi

# Check SDK version is supported
highestSdkVersion=29
if (( 1 <= MINSDKVERSION && MINSDKVERSION <= highestSdkVersion ))
then
    echo "Compiling for SDK $MINSDKVERSION"
else
    echo "Invalid SDK level of $MINSDKVERSION"
    exit 1
fi

# Remove build directory if it exists
if [ -d "$BUILDDIR" ]
then
    echo "Cleaning up previous build"
    rm -r "$BUILDDIR"
fi

echo "Building in directory $BUILDDIR"

# Build
mkdir "$BUILDDIR" && cd "$BUILDDIR"
cmake .. -DOQS_USE_OPENSSL=OFF \
         -DBUILD_SHARED_LIBS=ON  \
         -DCMAKE_TOOLCHAIN_FILE="$NDK"/build/cmake/android.toolchain.cmake \
         -DANDROID_ABI="$ABI" \
         -DANDROID_NATIVE_API_LEVEL="$MINSDKVERSION" \
         $EXTRAFLAGS
cmake --build ./

# Provide rudimentary information following build
echo "Completed build run for ABI $ABI, SDK Version $MINSDKVERSION"
