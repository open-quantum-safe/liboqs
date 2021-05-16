@echo off
IF %COMPILER%==cygwin (
    @echo on
    echo "OSSL: %OPENSSL_ROOT_DIR%"
    set "OPENSSL_ROOT_DIR="
    echo "OSSL: %OPENSSL_ROOT_DIR%"
    SET "PATH=C:\cywin64\bin;c:\cygwin64;%PATH%"
    c:\cygwin64\bin\bash.exe -lc "echo '1 OSSLRD: $OPENSSL_ROOT_DIR' && unset OPENSSL_ROOT_DIR && echo $PATH && setup-x86_64.exe -qnNdO -R C:/cygwin64 -l C:/cygwin/var/cache/setup -P openssl-devel -P openssl -P ninja -P cmake -P gcc && cd ${APPVEYOR_BUILD_FOLDER} && openssl version && cygcheck -c && cygcheck -l openssl && cygcheck -l openssl-devel && echo 'OSSLDIR: $OPENSSL_ROOT_DIR' && find /usr -name opensslv.h -print && pwd && export OPENSSL_ROOT_DIR=/usr/share && echo 'OSSLR: $OPENSSL_ROOT_DIR' && mkdir build && cd build && whoami && touch CMakeDoxyfile.tpl && ls -la && cmake .. -GNinja -DCMAKE_C_COMPILER=gcc -DOQS_MINIMAL_BUILD=ON -DOQS_DIST_BUILD=ON -DOQS_ENABLE_SIG_SPHINCS=OFF -DOQS_ENABLE_SIG_RAINBOW=OFF -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DOQS_USE_OPENSSL=%OQS_USE_OPENSSL% && ninja "
)
IF %COMPILER%==msys2 (
    @echo on
    SET "PATH=C:\msys64\mingw64\bin;%PATH%"
    bash -lc "cd ${APPVEYOR_BUILD_FOLDER} && mkdir build && cd build && cmake .. -GNinja -DOQS_DIST_BUILD=ON -DOQS_ENABLE_SIG_SPHINCS=OFF -DOQS_ENABLE_SIG_RAINBOW=OFF -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DOQS_USE_OPENSSL=%OQS_USE_OPENSSL% && ninja"
)
IF %COMPILER%==msvc2019 (
    @echo on
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    mkdir build
    cd build
    REM SPHINCS and Rainbow cause a big slowdown in the tests
    cmake .. -GNinja -DOQS_DIST_BUILD=ON -DOQS_ENABLE_SIG_SPHINCS=OFF -DOQS_MINIMAL_BUILD=ON -DOQS_ENABLE_SIG_RAINBOW=OFF -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DOQS_USE_OPENSSL=%OQS_USE_OPENSSL%
    ninja
)
