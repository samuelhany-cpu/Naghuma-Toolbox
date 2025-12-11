# Script to copy required Qt and OpenCV DLLs to output directory

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Setting up DLLs for Naghuma Toolbox" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan

$qtPath = "C:\Qt\6.7.3\msvc2019_64\bin"
$opencvPath = "F:\OpenCV\opencv\build\x64\vc15\bin"
$outputDir = "x64\Debug"

# Create output directory if it doesn't exist
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir -Force | Out-Null
}

Write-Host "`nCopying Qt DLLs..." -ForegroundColor Yellow

# Qt Core DLLs (Debug versions)
$qtDlls = @(
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll"
)

foreach ($dll in $qtDlls) {
    $source = Join-Path $qtPath $dll
    $dest = Join-Path $outputDir $dll
    
    if (Test-Path $source) {
        Copy-Item $source $dest -Force
        Write-Host "  ? $dll" -ForegroundColor Green
    } else {
        Write-Host "  ?? $dll not found at $source" -ForegroundColor Red
    }
}

Write-Host "`nCopying Qt Platform Plugin..." -ForegroundColor Yellow

# Qt Platform Plugin (required for Qt GUI)
$platformsDir = Join-Path $outputDir "platforms"
if (-not (Test-Path $platformsDir)) {
    New-Item -ItemType Directory -Path $platformsDir -Force | Out-Null
}

$qwindowsDll = Join-Path $qtPath "..\plugins\platforms\qwindowsd.dll"
$qwindowsDest = Join-Path $platformsDir "qwindowsd.dll"

if (Test-Path $qwindowsDll) {
    Copy-Item $qwindowsDll $qwindowsDest -Force
    Write-Host "  ? qwindowsd.dll (platform plugin)" -ForegroundColor Green
} else {
    Write-Host "  ?? qwindowsd.dll not found" -ForegroundColor Red
}

Write-Host "`nCopying OpenCV DLLs..." -ForegroundColor Yellow

# OpenCV DLLs
$opencvDlls = @(
    "opencv_world430d.dll"
)

foreach ($dll in $opencvDlls) {
    $source = Join-Path $opencvPath $dll
    $dest = Join-Path $outputDir $dll
    
    if (Test-Path $source) {
        Copy-Item $source $dest -Force
        Write-Host "  ? $dll" -ForegroundColor Green
    } else {
        Write-Host "  ?? $dll not found at $source" -ForegroundColor Red
    }
}

Write-Host "`n================================" -ForegroundColor Cyan
Write-Host "? DLL Setup Complete!" -ForegroundColor Green
Write-Host "================================" -ForegroundColor Cyan

Write-Host "`nYou can now run the application directly from:" -ForegroundColor Yellow
Write-Host "  $outputDir\Naghuma Toolbox.exe" -ForegroundColor Cyan
