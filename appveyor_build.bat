@echo off
IF %COMPILER%==cygwin (
    @echo on
    SET "PATH=C:\cywin64\bin;c:\cygwin64;%PATH%"
    c:\cygwin64\bin\bash.exe -lc "setup-x86_64.exe -qnNdO -R C:/cygwin64 -l C:/cygwin/var/cache/setup -P openssl -P libssl-devel -P ninja -P cmake -P gcc && cd ${APPVEYOR_BUILD_FOLDER} && openssl version && cygcheck -c && pwd && mkdir build && cd build && cmake .. -GNinja -DCMAKE_C_COMPILER=gcc -DOQS_DIST_BUILD=ON -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DOQS_USE_OPENSSL=%OQS_USE_OPENSSL% -DOQS_ALGS_ENABLED=%OQS_ALGS_ENABLED% && ninja "
)
IF %COMPILER%==msys2 (
    @echo on
    SET "PATH=C:\msys64\mingw64\bin;%PATH%"
    bash -lc "cd ${APPVEYOR_BUILD_FOLDER} && mkdir build && cd build && cmake .. -GNinja -DOQS_DIST_BUILD=ON -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DOQS_USE_OPENSSL=%OQS_USE_OPENSSL% -DOQS_ALGS_ENABLED=%OQS_ALGS_ENABLED% && ninja"
)
IF %COMPILER%==msvc2019 (
    @echo on
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    mkdir build
    cd build
    cmake .. -GNinja -DOQS_DIST_BUILD=ON -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DOQS_USE_OPENSSL=%OQS_USE_OPENSSL% -DOQS_ALGS_ENABLED=%OQS_ALGS_ENABLED%
    ninja
)
