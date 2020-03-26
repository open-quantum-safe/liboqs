@echo off
IF %COMPILER%==msys2 (
    @echo on
    SET "PATH=C:\msys64\mingw64\bin;%PATH%"
    bash -lc "cd ${APPVEYOR_BUILD_FOLDER} && mkdir build && cd build && cmake .. -GNinja -DCMAKE_BUILD_TYPE=Optimized -DBUILD_SHARED_LIBS=%BUILD_SHARED% && ninja"
)
IF %COMPILER%==msvc2019 (
    @echo on
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    mkdir build
    cd build
    REM ToDo: Re-activate Optimized build when fixed 
    cmake .. -GNinja -DBUILD_SHARED_LIBS=%BUILD_SHARED%
    ninja
)
