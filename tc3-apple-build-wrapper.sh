#!/bin/bash
# tc3-apple-build-wrapper.sh, ABr
#
# TripleCyber build support for liboqs

# top-level settings
the_openssl_ver=3.2.1
the_openssl_dir=$HOME/proj/git/src/triplecyber.visualstudio.com/abruce-dev/Tc32/External/openssl
the_ios_target=17.0

# apple build
the_platform=apple

# build macosx
the_device=macosx
the_openssl_plat_dir=$the_openssl_dir/apple/$the_openssl_ver/$the_device
mkdir -p ./build/apple/$the_device/{x86_64,arm64}
cd ./build/$the_platform/$the_device/x86_64 || exit $?
rm -fR ./*
cmake -DCMAKE_TOOLCHAIN_FILE=../../../../.CMake/apple.cmake -DPLATFORM=MAC -DDEPLOYMENT_TARGET=$the_ios_target -DOQS_USE_OPENSSL=ON -DOQS_USE_SHA3_OPENSSL=ON -DOPENSSL_USE_STATIC_LIBS=ON -DOPENSSL_ROOT_DIR=$the_openssl_plat_dir ../../../.. || exit $?
cmake --build . || exit $?
cd ../arm64 || exit $?
rm -fR ./*
cmake -DCMAKE_TOOLCHAIN_FILE=../../../../.CMake/apple.cmake -DPLATFORM=MAC_ARM64 -DDEPLOYMENT_TARGET=$the_ios_target -DOQS_USE_OPENSSL=ON -DOQS_USE_SHA3_OPENSSL=ON -DOPENSSL_USE_STATIC_LIBS=ON -DOPENSSL_ROOT_DIR=$the_openssl_plat_dir ../../../.. || exit $?
cmake --build . || exit $?
cd .. || exit $?
mkdir -p ./lib
xcrun lipo ./arm64/lib/liboqs.a ./x86_64/lib/liboqs.a -create -output ./lib/liboqs.a || exit $?
xcrun lipo ./arm64/lib/liboqs-internal.a ./x86_64/lib/liboqs-internal.a -create -output ./lib/liboqs-internal.a || exit $?
cd ../../.. || exit $?

# build iphonesimulator
the_device=iphonesimulator
the_openssl_plat_dir=$the_openssl_dir/apple/$the_openssl_ver/$the_device
mkdir -p ./build/apple/$the_device/{x86_64,arm64}
cd ./build/$the_platform/$the_device/x86_64 || exit $?
rm -fR ./*
cmake -DCMAKE_TOOLCHAIN_FILE=../../../../.CMake/apple.cmake -DPLATFORM=SIMULATOR64 -DDEPLOYMENT_TARGET=$the_ios_target -DOQS_USE_OPENSSL=ON -DOQS_USE_SHA3_OPENSSL=ON -DOPENSSL_USE_STATIC_LIBS=ON -DOPENSSL_ROOT_DIR=$the_openssl_plat_dir ../../../.. || exit $?
cmake --build . || exit $?
cd ../arm64 || exit $?
rm -fR ./*
cmake -DCMAKE_TOOLCHAIN_FILE=../../../../.CMake/apple.cmake -DPLATFORM=SIMULATORARM64 -DDEPLOYMENT_TARGET=$the_ios_target -DOQS_USE_OPENSSL=ON -DOQS_USE_SHA3_OPENSSL=ON -DOPENSSL_USE_STATIC_LIBS=ON -DOPENSSL_ROOT_DIR=$the_openssl_plat_dir ../../../.. || exit $?
cmake --build . || exit $?
cd .. || exit $?
mkdir -p ./lib
xcrun lipo ./arm64/lib/liboqs.a ./x86_64/lib/liboqs.a -create -output ./lib/liboqs.a || exit $?
xcrun lipo ./arm64/lib/liboqs-internal.a ./x86_64/lib/liboqs-internal.a -create -output ./lib/liboqs-internal.a || exit $?
cd ../../.. || exit $?

# build iphoneos
the_device=iphoneos
the_openssl_plat_dir=$the_openssl_dir/apple/$the_openssl_ver/$the_device
mkdir -p ./build/apple/$the_device/arm64
cd ./build/$the_platform/$the_device/arm64 || exit $?
rm -fR ./*
cmake -DCMAKE_TOOLCHAIN_FILE=../../../../.CMake/apple.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET=$the_ios_target -DOQS_USE_OPENSSL=ON -DOQS_USE_SHA3_OPENSSL=ON -DOPENSSL_USE_STATIC_LIBS=ON -DOPENSSL_ROOT_DIR=$the_openssl_plat_dir ../../../.. || exit $?
cmake --build . || exit $?
cd .. || exit $?
mkdir -p ./lib
xcrun lipo ./arm64/lib/liboqs.a -create -output ./lib/liboqs.a || exit $?
xcrun lipo ./arm64/lib/liboqs-internal.a -create -output ./lib/liboqs-internal.a || exit $?
cd ../../.. || exit $?

