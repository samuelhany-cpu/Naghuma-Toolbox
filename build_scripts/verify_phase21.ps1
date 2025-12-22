# ============================================================================
# PHASE 21 - VERIFICATION SCRIPT
# ============================================================================
# Verifies all Phase 21 components are in place
# ============================================================================

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  PHASE 21 VERIFICATION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$allGood = $true

# Check header files
Write-Host "[1/6] Checking header files..." -ForegroundColor Yellow
$headers = @(
    "include\IntensityTransformDialog.h",
    "include\SharpeningDialog.h"
)

foreach ($header in $headers) {
    if (Test-Path $header) {
        Write-Host "  ? $header" -ForegroundColor Green
    } else {
        Write-Host "  ? $header MISSING" -ForegroundColor Red
        $allGood = $false
    }
}

# Check implementation files
Write-Host ""
Write-Host "[2/6] Checking implementation files..." -ForegroundColor Yellow
$impls = @(
    "src\IntensityTransformDialog.cpp",
    "src\SharpeningDialog.cpp",
    "src\MainWindow_Phase21.cpp"
)

foreach ($impl in $impls) {
    if (Test-Path $impl) {
        Write-Host "  ? $impl" -ForegroundColor Green
    } else {
        Write-Host "  ? $impl MISSING" -ForegroundColor Red
        $allGood = $false
    }
}

# Check MOC files
Write-Host ""
Write-Host "[3/6] Checking MOC files..." -ForegroundColor Yellow
$mocs = @(
    "src\moc_IntensityTransformDialog.cpp",
    "src\moc_SharpeningDialog.cpp"
)

foreach ($moc in $mocs) {
    if (Test-Path $moc) {
        $size = (Get-Item $moc).Length
        Write-Host "  ? $moc ($size bytes)" -ForegroundColor Green
    } else {
        Write-Host "  ? $moc MISSING" -ForegroundColor Red
        $allGood = $false
    }
}

# Check MainWindow.cpp includes
Write-Host ""
Write-Host "[4/6] Checking MainWindow.cpp includes..." -ForegroundColor Yellow
$mainWindowContent = Get-Content "src\MainWindow.cpp" -Raw

if ($mainWindowContent -match 'IntensityTransformDialog.h') {
    Write-Host "  ? IntensityTransformDialog.h included" -ForegroundColor Green
} else {
    Write-Host "  ? IntensityTransformDialog.h NOT included" -ForegroundColor Red
    $allGood = $false
}

if ($mainWindowContent -match 'SharpeningDialog.h') {
    Write-Host "  ? SharpeningDialog.h included" -ForegroundColor Green
} else {
    Write-Host "  ? SharpeningDialog.h NOT included" -ForegroundColor Red
    $allGood = $false
}

# Check project file
Write-Host ""
Write-Host "[5/6] Checking project file..." -ForegroundColor Yellow
$projectContent = Get-Content "Naghuma Toolbox.vcxproj" -Raw

if ($projectContent -match 'moc_IntensityTransformDialog.cpp') {
    Write-Host "  ? moc_IntensityTransformDialog.cpp in project" -ForegroundColor Green
} else {
    Write-Host "  ? moc_IntensityTransformDialog.cpp NOT in project" -ForegroundColor Red
    $allGood = $false
}

if ($projectContent -match 'moc_SharpeningDialog.cpp') {
    Write-Host "  ? moc_SharpeningDialog.cpp in project" -ForegroundColor Green
} else {
    Write-Host "  ? moc_SharpeningDialog.cpp NOT in project" -ForegroundColor Red
    $allGood = $false
}

# Check MainWindow.h declarations
Write-Host ""
Write-Host "[6/6] Checking MainWindow.h declarations..." -ForegroundColor Yellow
$mainWindowH = Get-Content "include\MainWindow.h" -Raw

if ($mainWindowH -match 'showIntensityTransformDialog') {
    Write-Host "  ? showIntensityTransformDialog() declared" -ForegroundColor Green
} else {
    Write-Host "  ? showIntensityTransformDialog() NOT declared" -ForegroundColor Red
    $allGood = $false
}

if ($mainWindowH -match 'showSharpeningDialog') {
    Write-Host "  ? showSharpeningDialog() declared" -ForegroundColor Green
} else {
    Write-Host "  ? showSharpeningDialog() NOT declared" -ForegroundColor Red
    $allGood = $false
}

# Final result
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan

if ($allGood) {
    Write-Host "  ? ALL CHECKS PASSED" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Phase 21 is ready to build!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "  1. Open solution in Visual Studio" -ForegroundColor Cyan
    Write-Host "  2. Build (Ctrl+Shift+B)" -ForegroundColor Cyan
    Write-Host "  3. Run and test new features" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "New menu items:" -ForegroundColor Yellow
    Write-Host "  • Process ? Intensity Transformations ? Gamma & Log Transform" -ForegroundColor Cyan
    Write-Host "  • Process ? Sharpening Filters ? Laplacian/Unsharp/High-Boost" -ForegroundColor Cyan
} else {
    Write-Host "  ? SOME CHECKS FAILED" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Please fix the issues above before building." -ForegroundColor Red
    Write-Host ""
    Write-Host "To fix automatically, run:" -ForegroundColor Yellow
    Write-Host "  .\apply_phase21_complete.ps1" -ForegroundColor Cyan
}

Write-Host ""
