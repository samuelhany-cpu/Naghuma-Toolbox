#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Naghuma Toolbox Application Launcher
    
.DESCRIPTION
    Master runner script that:
    - Verifies Python environment is set up
    - Checks EasyOCR installation
    - Ensures all dependencies are ready
    - Launches the application
    
.EXAMPLE
    .\run_naghuma_toolbox.ps1
#>

$ErrorActionPreference = "Stop"
$ProjectRoot = $PSScriptRoot
$ExePath = "$ProjectRoot\x64\Debug\Naghuma Toolbox.exe"
$PythonPath = "$ProjectRoot\.venv\Scripts\python.exe"
$EasyOCRRunner = "$ProjectRoot\easyocr_runner.py"

Write-Host "==================================================" -ForegroundColor Cyan
Write-Host "    Naghuma Toolbox Application Launcher" -ForegroundColor Cyan
Write-Host "==================================================" -ForegroundColor Cyan
Write-Host ""

# Check if executable exists
if (-not (Test-Path $ExePath)) {
    Write-Host "[ERROR] Application not found at: $ExePath" -ForegroundColor Red
    Write-Host "Please build the project first:" -ForegroundColor Yellow
    Write-Host '  & "F:\Visual Studio 2022\MSBuild\Current\Bin\MSBuild.exe" "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /m' -ForegroundColor Gray
    exit 1
}
Write-Host "[OK] Application found" -ForegroundColor Green

# Check if curl is available (required for OCR.space API)
$curlCheck = Get-Command curl -ErrorAction SilentlyContinue
if ($curlCheck) {
    Write-Host "[OK] curl found (required for OCR)" -ForegroundColor Green
} else {
    Write-Host "[WARN] curl not found - OCR may not work" -ForegroundColor Yellow
}

# Display system info
Write-Host ""
Write-Host "System Information:" -ForegroundColor Cyan
Write-Host "  - Project Root: $ProjectRoot" -ForegroundColor Gray
Write-Host "  - Python: $PythonPath" -ForegroundColor Gray
Write-Host "  - Executable: $ExePath" -ForegroundColor Gray
Write-Host ""

# Launch application
Write-Host "==================================================" -ForegroundColor Cyan
Write-Host "Launching Naghuma Toolbox..." -ForegroundColor Green
Write-Host "==================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "[INFO] OCR powered by OCR.space Cloud API" -ForegroundColor Cyan
Write-Host "[INFO] Free tier: 25,000 requests/month" -ForegroundColor Cyan
Write-Host ""

# Change to project directory and run
Set-Location $ProjectRoot
& $ExePath

# Capture exit code
$exitCode = $LASTEXITCODE
Write-Host ""
if ($exitCode -eq 0) {
    Write-Host "Application exited successfully" -ForegroundColor Green
} else {
    Write-Host "Application exited with code: $exitCode" -ForegroundColor Yellow
}

exit $exitCode
