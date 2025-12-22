# Quick fix to add Phase 21 includes to MainWindow.cpp
# Run this before building

$file = "src\MainWindow.cpp"
$content = Get-Content $file -Raw

# Check if already added
if ($content -match "IntensityTransformDialog.h") {
    Write-Host "? Includes already present" -ForegroundColor Green
    exit 0
}

# Add includes after WaveletDialog.h
$pattern = '(#include "WaveletDialog.h")'
$replacement = '$1
#include "IntensityTransformDialog.h"  // Phase 21
#include "SharpeningDialog.h"  // Phase 21'

$newContent = $content -replace $pattern, $replacement

# Save
Set-Content -Path $file -Value $newContent -NoNewline

Write-Host "? Added Phase 21 includes to MainWindow.cpp" -ForegroundColor Green
