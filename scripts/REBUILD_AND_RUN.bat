@echo off
echo ====================================================
echo   Naghuma Toolbox - Rebuild and Run
echo ====================================================
echo.

cd /d "F:\Naghuma Toolbox"

REM Find MSBuild
set "MSBUILD="

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
)

if not defined MSBUILD (
    echo ERROR: MSBuild not found!
    echo Trying with Visual Studio Developer Command Prompt...
    echo.
    goto :TRY_DEVENV
)

echo Found MSBuild at: %MSBUILD%
echo.
echo [1/3] Cleaning previous build...
"%MSBUILD%" "Naghuma Toolbox.sln" /t:Clean /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

echo.
echo [2/3] Rebuilding project...
"%MSBUILD%" "Naghuma Toolbox.sln" /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

if %errorlevel% neq 0 (
    echo.
    echo ====================================================
    echo   BUILD FAILED!
    echo ====================================================
    echo.
    echo Errors occurred during build. Check output above.
    pause
    exit /b 1
)

goto :COPY_DLLS

:TRY_DEVENV
REM Try to use devenv
devenv "Naghuma Toolbox.sln" /Rebuild "Debug|x64"
if %errorlevel% neq 0 (
    echo BUILD FAILED!
    pause
    exit /b 1
)

:COPY_DLLS
echo.
echo [3/3] Copying required DLLs...
powershell -ExecutionPolicy Bypass -File "scripts\copy_dlls.ps1"

echo.
echo ====================================================
echo   BUILD SUCCESSFUL!
echo ====================================================
echo.

if exist "x64\Debug\Naghuma Toolbox.exe" (
    echo Running application...
    echo.
    start "" "x64\Debug\Naghuma Toolbox.exe"
    echo.
    echo Application started!
) else (
    echo WARNING: Executable not found!
)

echo.
pause
