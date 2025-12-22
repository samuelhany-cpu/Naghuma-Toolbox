# Quick DLL Fix Script
# Run this if DLLs are missing after rebuild

Write-Host "????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  Copying Missing Tesseract DLLs" -ForegroundColor Yellow
Write-Host "????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$tesseractPath = "C:\Program Files\Tesseract-OCR"
$debugPath = "x64\Debug"

if (-not (Test-Path $debugPath)) {
    Write-Host "? Debug folder not found: $debugPath" -ForegroundColor Red
    exit 1
}

$requiredDlls = @(
    "libcrypto-3-x64.dll",
    "libarchive-13.dll",
    "libgcc_s_seh-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "libzstd.dll"
)

$copied = 0
$skipped = 0
$missing = 0

foreach ($dll in $requiredDlls) {
    $srcPath = Join-Path $tesseractPath $dll
    $dstPath = Join-Path $debugPath $dll
    
    if (-not (Test-Path $srcPath)) {
        Write-Host "??  Source not found: $dll" -ForegroundColor Yellow
        $missing++
        continue
    }
    
    if (Test-Path $dstPath) {
        Write-Host "? Already present: $dll" -ForegroundColor Gray
        $skipped++
        continue
    }
    
    try {
        Copy-Item $srcPath $dstPath -Force
        Write-Host "? Copied: $dll" -ForegroundColor Green
        $copied++
    } catch {
        Write-Host "? Failed to copy: $dll - $($_.Exception.Message)" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  Copied: $copied" -ForegroundColor Green
Write-Host "  Skipped: $skipped" -ForegroundColor Gray
if ($missing -gt 0) {
    Write-Host "  Missing: $missing" -ForegroundColor Yellow
}
Write-Host "????????????????????????????????????????" -ForegroundColor Cyan

# Verify total DLL count
$totalDlls = (Get-ChildItem "$debugPath\*.dll" | Measure-Object).Count
Write-Host ""
Write-Host "Total DLLs in $debugPath : $totalDlls" -ForegroundColor Cyan
Write-Host ""

if ($totalDlls -ge 23) {
    Write-Host "? All DLLs present! Ready to run." -ForegroundColor Green
} else {
    Write-Host "??  Some DLLs may be missing (expected: 23, found: $totalDlls)" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
