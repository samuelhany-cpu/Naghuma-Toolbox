@echo off
echo Starting Naghuma Toolbox...
echo.

cd /d "F:\Naghuma Toolbox\x64\Debug"

if not exist "Naghuma Toolbox.exe" (
    echo ERROR: Naghuma Toolbox.exe not found!
    echo Please build the project first.
    pause
    exit /b 1
)

REM Check if DLLs exist
if not exist "Qt6Cored.dll" (
    echo Missing Qt DLLs! Running copy script...
    powershell -ExecutionPolicy Bypass -File "..\..\scripts\copy_dlls.ps1"
    echo.
)

echo Launching Naghuma Toolbox...
start "" "Naghuma Toolbox.exe"

echo Application started!
echo.
