@echo off
echo ====================================================
echo   Naghuma Toolbox - Complete Build Script
echo ====================================================
echo.

echo [1/4] Generating MOC files...
powershell -ExecutionPolicy Bypass -File generate_moc.ps1
if %errorlevel% neq 0 (
    echo ERROR: MOC generation failed!
    pause
    exit /b 1
)

echo.
echo [2/4] Adding MOC files to project...
powershell -ExecutionPolicy Bypass -File add_moc_to_project.ps1
if %errorlevel% neq 0 (
    echo ERROR: Failed to add MOC files to project!
    pause
    exit /b 1
)

echo.
echo [3/4] Building project...
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m /nologo
if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo [4/4] Build complete!
echo.
echo ====================================================
echo   Build successful! Use run_naghuma_toolbox.bat to launch
echo ====================================================
echo.

choice /C YN /M "Do you want to run the application now?"
if %errorlevel% equ 1 (
    call run_naghuma_toolbox.bat
)
