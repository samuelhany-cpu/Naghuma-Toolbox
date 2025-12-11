@echo off
setlocal enabledelayedexpansion

echo.
echo ========================================
echo   Naghuma Toolbox - Build System
echo ========================================
echo.

cd /d "F:\Naghuma Toolbox"

REM Try to find MSBuild
set "MSBUILD="

REM Check for Visual Studio 2022
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
)

REM Check for Visual Studio 2019
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
)

if not defined MSBUILD (
    echo ERROR: MSBuild not found!
    echo Please install Visual Studio 2019 or 2022
    echo.
    pause
    exit /b 1
)

echo Found MSBuild: %MSBUILD%
echo.

echo Building project...
echo.

"%MSBUILD%" "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build /m /v:minimal

if %errorlevel% neq 0 (
    echo.
    echo ========================================
    echo   BUILD FAILED!
    echo ========================================
    echo.
    echo Check the output above for errors.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   BUILD SUCCESSFUL!
echo ========================================
echo.

if exist "x64\Debug\Naghuma Toolbox.exe" (
    echo Executable: x64\Debug\Naghuma Toolbox.exe
    echo.
    
    choice /C YN /N /M "Run the application now? (Y/N): "
    if !errorlevel! equ 1 (
        echo.
        echo Starting Naghuma Toolbox...
        start "" "x64\Debug\Naghuma Toolbox.exe"
    )
) else (
    echo WARNING: Executable not found!
)

echo.
pause
