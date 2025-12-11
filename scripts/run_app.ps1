# Naghuma Toolbox Launcher
# This script sets up the PATH and launches the application

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Naghuma Toolbox Launcher" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$exePath = "x64\Debug\Naghuma Toolbox.exe"

# Check if executable exists
if (-not (Test-Path $exePath)) {
    Write-Host "`n? Application not found!" -ForegroundColor Red
    Write-Host "Please build the project first using:" -ForegroundColor Yellow
    Write-Host "  .\scripts\final_build.ps1" -ForegroundColor Cyan
    exit 1
}

# Setup DLLs if not already present
Write-Host "`nChecking DLLs..." -ForegroundColor Yellow
$qt6CoreDll = "x64\Debug\Qt6Cored.dll"

if (-not (Test-Path $qt6CoreDll)) {
    Write-Host "DLLs not found. Setting up..." -ForegroundColor Yellow
    & .\scripts\setup_dlls.ps1
} else {
    Write-Host "? DLLs already present" -ForegroundColor Green
}

# Add Qt and OpenCV to PATH for this session
$env:PATH = "C:\Qt\6.7.3\msvc2019_64\bin;" + $env:PATH
$env:PATH = "F:\OpenCV\opencv\build\x64\vc15\bin;" + $env:PATH

Write-Host "`n? Environment configured" -ForegroundColor Green
Write-Host "Launching Naghuma Toolbox..." -ForegroundColor Cyan
Write-Host "================================`n" -ForegroundColor Cyan

# Launch the application
Start-Process $exePath
