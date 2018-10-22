@echo off

REM compares KAT files for Windows-enabled KEM algs
for /R src\kem\ %%K in (*.kat) DO (
    if exist kat_kem_rsp\%%~nxK (
         FC %%K kat_kem_rsp\%%~nxK >nul 2>nul
    	 if errorlevel 1 (
	    echo %%~nxK not matching
	    exit /b 1
	 )
	 )
)

