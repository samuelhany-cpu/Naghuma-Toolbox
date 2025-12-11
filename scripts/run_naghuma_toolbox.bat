@echo off
REM Naghuma Toolbox Launcher
REM Adds Qt and OpenCV DLL paths to PATH before launching

echo.
echo ====================================================
echo       Naghuma Toolbox - Image Processing Suite
echo ====================================================
echo.

REM Add Qt and OpenCV bin directories to PATH
set PATH=C:\Qt\6.7.3\msvc2019_64\bin;F:\OpenCV\opencv\build\x64\vc15\bin;%PATH%

REM Launch the application
echo Starting Naghuma Toolbox...
echo.

if exist "x64\Debug\Naghuma Toolbox.exe" (
    start "" "x64\Debug\Naghuma Toolbox.exe"
) else if exist "x64\Release\Naghuma Toolbox.exe" (
    start "" "x64\Release\Naghuma Toolbox.exe"
) else (
    echo ERROR: Executable not found!
    echo Please build the project first.
    pause
)
