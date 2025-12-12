@echo off
:: Naghuma Toolbox Quick Launcher
:: Double-click this file to run the application

echo ========================================
echo   Naghuma Toolbox - Quick Launcher
echo ========================================
echo.

:: Check if PowerShell is available
where powershell >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: PowerShell not found!
    pause
    exit /b 1
)

:: Run the PowerShell launcher script
powershell -ExecutionPolicy Bypass -File "%~dp0scripts\run_app.ps1"

:: Keep window open if there was an error
if %errorlevel% neq 0 (
    echo.
    echo Press any key to exit...
    pause >nul
)
