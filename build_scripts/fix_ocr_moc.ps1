# ============================================================================
# FIX OCR MOC FILE - GENERATE AND INCLUDE
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  FIXING OCR MOC FILE                   " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Find Qt MOC executable
$qtDir = $env:QTDIR
if (-not $qtDir) {
    $qtDir = "C:\Qt\6.7.3\msvc2019_64"
}

$mocExe = Join-Path $qtDir "bin\moc.exe"

if (-not (Test-Path $mocExe)) {
    Write-Host "? MOC executable not found at: $mocExe" -ForegroundColor Red
    Write-Host ""
    Write-Host "Manual fix required:" -ForegroundColor Yellow
    Write-Host "  1. Close Visual Studio" -ForegroundColor White
    Write-Host "  2. Add to src\OCRDialog.cpp at the end:" -ForegroundColor White
    Write-Host '     #include "moc_OCRDialog.cpp"' -ForegroundColor Cyan
    Write-Host "  3. Reopen Visual Studio and rebuild" -ForegroundColor White
    exit 1
}

Write-Host "? Found MOC: $mocExe" -ForegroundColor Green
Write-Host ""

# Generate MOC file
Write-Host "Generating moc_OCRDialog.cpp..." -ForegroundColor Yellow

$headerPath = "include\OCRDialog.h"
$mocOutputPath = "src\moc_OCRDialog.cpp"

if (-not (Test-Path $headerPath)) {
    Write-Host "? Header file not found: $headerPath" -ForegroundColor Red
    exit 1
}

# Run MOC
& $mocExe $headerPath -o $mocOutputPath

if (Test-Path $mocOutputPath) {
    $size = (Get-Item $mocOutputPath).Length
    Write-Host "? Generated moc_OCRDialog.cpp ($size bytes)" -ForegroundColor Green
} else {
    Write-Host "? Failed to generate MOC file" -ForegroundColor Red
    exit 1
}

Write-Host ""

# Check if OCRDialog.cpp already includes the MOC file
Write-Host "Checking OCRDialog.cpp..." -ForegroundColor Yellow

$cppPath = "src\OCRDialog.cpp"
$content = Get-Content $cppPath -Raw

if ($content -match 'moc_OCRDialog\.cpp') {
    Write-Host "? MOC include already present" -ForegroundColor Green
} else {
    Write-Host "  Adding MOC include..." -ForegroundColor Yellow
    
    # Add MOC include at the end
    $content = $content.TrimEnd()
    $content += "`r`n`r`n#include `"moc_OCRDialog.cpp`"`r`n"
    
    Set-Content $cppPath -Value $content -NoNewline
    Write-Host "? Added MOC include to OCRDialog.cpp" -ForegroundColor Green
}

Write-Host ""
Write-Host "????????????????????????????????????????" -ForegroundColor Green
Write-Host "  MOC FILE FIXED!                       " -ForegroundColor Green
Write-Host "????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "Now in Visual Studio:" -ForegroundColor Yellow
Write-Host "  1. Build ? Clean Solution" -ForegroundColor White
Write-Host "  2. Build ? Rebuild Solution" -ForegroundColor White
Write-Host ""
