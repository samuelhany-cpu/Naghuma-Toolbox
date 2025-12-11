@echo off
echo ====================================================
echo   Naghuma Toolbox - Professional Build Script
echo ====================================================
echo.

REM Find Visual Studio installation
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo ERROR: Visual Studio not found!
    echo Please ensure Visual Studio 2019 or later is installed.
    pause
    exit /b 1
)

REM Get Visual Studio installation path
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -property installationPath`) do set "VSINSTALLDIR=%%i"

if not defined VSINSTALLDIR (
    echo ERROR: Could not find Visual Studio installation directory!
    pause
    exit /b 1
)

echo Using Visual Studio at: %VSINSTALLDIR%
echo.

REM Setup Visual Studio environment
call "%VSINSTALLDIR%\Common7\Tools\VsDevCmd.bat" -arch=x64

echo.
echo [1/3] Cleaning previous build...
if exist "x64\Debug" rmdir /s /q "x64\Debug"
echo   -> Clean complete
echo.

echo [2/3] Building project (Debug x64)...
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m /v:minimal /nologo

if %errorlevel% neq 0 (
    echo.
    echo ====================================================
    echo   BUILD FAILED!
    echo ====================================================
    pause
    exit /b 1
)

echo.
echo [3/3] Build successful!
echo.
echo ====================================================
echo   BUILD COMPLETE!
echo ====================================================
echo.
echo Executable location: x64\Debug\Naghuma Toolbox.exe
echo.

choice /C YN /M "Do you want to run the application now"
if %errorlevel% equ 1 (
    echo.
    echo Starting Naghuma Toolbox...
    start "" "x64\Debug\Naghuma Toolbox.exe"
)
