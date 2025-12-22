Write-Host "???????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  DLL Verification Report" -ForegroundColor Yellow
Write-Host "???????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$releaseDir = "x64\Release"

# Check if Release directory exists
if (!(Test-Path $releaseDir)) {
    Write-Host "? Release directory not found: $releaseDir" -ForegroundColor Red
    exit 1
}

# Critical DLL categories
$criticalDLLs = @{
    "Qt Framework" = @(
        "Qt6Core.dll",
        "Qt6Cored.dll",
        "Qt6Gui.dll",
        "Qt6Guid.dll",
        "Qt6Widgets.dll",
        "Qt6Widgetsd.dll"
    )
    "OpenCV" = @(
        "opencv_world430.dll",
        "opencv_world430d.dll"
    )
    "Tesseract OCR" = @(
        "tesseract55.dll",
        "leptonica-1.85.0.dll",
        "libtesseract-5.dll",
        "libleptonica-6.dll"
    )
    "Image Libraries" = @(
        "libpng16.dll",
        "libjpeg-8.dll",
        "libtiff-6.dll",
        "libwebp-7.dll",
        "giflib.dll"
    )
    "Compression" = @(
        "zlib1.dll",
        "libzstd.dll",
        "liblzma.dll",
        "liblz4.dll",
        "bz2.dll"
    )
    "SSL/Network" = @(
        "libssl-3-x64.dll",
        "libcrypto-3-x64.dll",
        "libcurl-4.dll"
    )
}

$totalChecked = 0
$totalFound = 0
$totalMissing = 0

foreach ($category in $criticalDLLs.Keys) {
    Write-Host "[$category]" -ForegroundColor Cyan
    
    foreach ($dll in $criticalDLLs[$category]) {
        $totalChecked++
        $dllPath = Join-Path $releaseDir $dll
        
        if (Test-Path $dllPath) {
            $size = (Get-Item $dllPath).Length
            $sizeMB = [math]::Round($size / 1MB, 2)
            Write-Host "  ? $dll" -ForegroundColor Green -NoNewline
            Write-Host " ($sizeMB MB)" -ForegroundColor Gray
            $totalFound++
        } else {
            Write-Host "  ? $dll" -ForegroundColor Red -NoNewline
            Write-Host " (MISSING)" -ForegroundColor Red
            $totalMissing++
        }
    }
    Write-Host ""
}

# Count all DLLs in Release directory
$allDlls = Get-ChildItem "$releaseDir\*.dll" -ErrorAction SilentlyContinue
$totalDllCount = ($allDlls | Measure-Object).Count
$totalDllSize = ($allDlls | Measure-Object -Property Length -Sum).Sum
$totalDllSizeMB = [math]::Round($totalDllSize / 1MB, 2)

Write-Host "???????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  Total DLLs in Release: $totalDllCount" -ForegroundColor White
Write-Host "  Total DLL Size: $totalDllSizeMB MB" -ForegroundColor White
Write-Host "  Critical DLLs Checked: $totalChecked" -ForegroundColor White
Write-Host "  Critical DLLs Found: $totalFound" -ForegroundColor Green
if ($totalMissing -gt 0) {
    Write-Host "  Critical DLLs Missing: $totalMissing" -ForegroundColor Red
} else {
    Write-Host "  Critical DLLs Missing: 0" -ForegroundColor Green
}
Write-Host "???????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Check if EXE exists
$exePath = Join-Path $releaseDir "Naghuma Toolbox.exe"
if (Test-Path $exePath) {
    $exeSize = (Get-Item $exePath).Length
    $exeSizeMB = [math]::Round($exeSize / 1MB, 2)
    Write-Host "? Naghuma Toolbox.exe found ($exeSizeMB MB)" -ForegroundColor Green
} else {
    Write-Host "? Naghuma Toolbox.exe NOT FOUND" -ForegroundColor Red
}

Write-Host ""

# Check if application is running
$runningProcess = Get-Process "Naghuma Toolbox" -ErrorAction SilentlyContinue
if ($runningProcess) {
    Write-Host "? Application is currently running!" -ForegroundColor Green
    Write-Host "  Process ID: $($runningProcess.Id)" -ForegroundColor Cyan
    $memoryMB = [math]::Round($runningProcess.WorkingSet64 / 1MB, 2)
    Write-Host "  Memory Usage: $memoryMB MB" -ForegroundColor Cyan
    Write-Host "  CPU Time: $($runningProcess.CPU) seconds" -ForegroundColor Cyan
} else {
    Write-Host "? Application is not currently running" -ForegroundColor Yellow
}

Write-Host ""

# Overall status
if ($totalMissing -eq 0 -and (Test-Path $exePath)) {
    Write-Host "???????????????????????????????????????????" -ForegroundColor Green
    Write-Host "  ? ALL SYSTEMS GO! READY TO RUN!" -ForegroundColor Green
    Write-Host "???????????????????????????????????????????" -ForegroundColor Green
    Write-Host ""
    Write-Host "To run the application:" -ForegroundColor Yellow
    Write-Host "  cd x64\Release" -ForegroundColor Cyan
    Write-Host "  .\`"Naghuma Toolbox.exe`"" -ForegroundColor Cyan
    Write-Host ""
} else {
    Write-Host "???????????????????????????????????????????" -ForegroundColor Yellow
    Write-Host "  ? Some issues detected" -ForegroundColor Yellow
    Write-Host "???????????????????????????????????????????" -ForegroundColor Yellow
    
    if ($totalMissing -gt 0) {
        Write-Host ""
        Write-Host "Missing DLLs detected. Copy from Debug folder:" -ForegroundColor Yellow
        Write-Host "  Copy-Item x64\Debug\*.dll x64\Release\ -Force" -ForegroundColor Cyan
    }
}

Write-Host ""
