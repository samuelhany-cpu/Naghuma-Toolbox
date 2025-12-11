# Copy Required DLLs to Debug Folder
# This script copies Qt and OpenCV DLLs needed to run the application

Write-Host "Copying required DLLs to Debug folder..." -ForegroundColor Cyan
Write-Host ""

$DebugFolder = "F:\Naghuma Toolbox\x64\Debug"
$QtPath = "C:\Qt\6.7.3\msvc2019_64\bin"
$OpenCVPath = "F:\OpenCV\opencv\build\x64\vc15\bin"

# Check if paths exist
if (-not (Test-Path $QtPath)) {
    Write-Host "ERROR: Qt path not found: $QtPath" -ForegroundColor Red
    Write-Host "Please verify Qt installation" -ForegroundColor Yellow
    Read-Host "Press Enter to continue anyway"
}

if (-not (Test-Path $OpenCVPath)) {
    Write-Host "ERROR: OpenCV path not found: $OpenCVPath" -ForegroundColor Red
    Write-Host "Please verify OpenCV installation" -ForegroundColor Yellow
    Read-Host "Press Enter to continue anyway"
}

# Create Debug folder if it doesn't exist
if (-not (Test-Path $DebugFolder)) {
    New-Item -ItemType Directory -Path $DebugFolder -Force | Out-Null
}

$copiedCount = 0
$failedCount = 0

# Qt Debug DLLs
$qtDlls = @(
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll"
)

Write-Host "Copying Qt Debug DLLs..." -ForegroundColor Yellow
foreach ($dll in $qtDlls) {
    $sourcePath = Join-Path $QtPath $dll
    $destPath = Join-Path $DebugFolder $dll
    
    if (Test-Path $sourcePath) {
        try {
            Copy-Item -Path $sourcePath -Destination $destPath -Force
            Write-Host "  [OK] $dll" -ForegroundColor Green
            $copiedCount++
        }
        catch {
            Write-Host "  [FAILED] $dll - $($_.Exception.Message)" -ForegroundColor Red
            $failedCount++
        }
    }
    else {
        Write-Host "  [NOT FOUND] $dll" -ForegroundColor Yellow
        $failedCount++
    }
}

# OpenCV Debug DLL
Write-Host ""
Write-Host "Copying OpenCV Debug DLL..." -ForegroundColor Yellow
$opencvDll = "opencv_world430d.dll"
$sourcePath = Join-Path $OpenCVPath $opencvDll
$destPath = Join-Path $DebugFolder $opencvDll

if (Test-Path $sourcePath) {
    try {
        Copy-Item -Path $sourcePath -Destination $destPath -Force
        Write-Host "  [OK] $opencvDll" -ForegroundColor Green
        $copiedCount++
    }
    catch {
        Write-Host "  [FAILED] $opencvDll - $($_.Exception.Message)" -ForegroundColor Red
        $failedCount++
    }
}
else {
    Write-Host "  [NOT FOUND] $opencvDll" -ForegroundColor Yellow
    $failedCount++
}

# Copy Qt platform plugin
Write-Host ""
Write-Host "Copying Qt platform plugin..." -ForegroundColor Yellow
$platformsFolder = Join-Path $DebugFolder "platforms"
if (-not (Test-Path $platformsFolder)) {
    New-Item -ItemType Directory -Path $platformsFolder -Force | Out-Null
}

$qwindowsDll = "qwindowsd.dll"
$sourcePath = Join-Path $QtPath "..\plugins\platforms\$qwindowsDll"
$destPath = Join-Path $platformsFolder $qwindowsDll

if (Test-Path $sourcePath) {
    try {
        Copy-Item -Path $sourcePath -Destination $destPath -Force
        Write-Host "  [OK] $qwindowsDll (platform plugin)" -ForegroundColor Green
        $copiedCount++
    }
    catch {
        Write-Host "  [FAILED] $qwindowsDll - $($_.Exception.Message)" -ForegroundColor Red
        $failedCount++
    }
}
else {
    Write-Host "  [NOT FOUND] $qwindowsDll" -ForegroundColor Yellow
    $failedCount++
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "Summary:" -ForegroundColor Cyan
Write-Host "  Copied: $copiedCount files" -ForegroundColor Green
Write-Host "  Failed: $failedCount files" -ForegroundColor $(if ($failedCount -gt 0) { "Red" } else { "Gray" })
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

if ($copiedCount -gt 0) {
    Write-Host "DLLs copied successfully!" -ForegroundColor Green
    Write-Host "You can now run the application." -ForegroundColor White
}
else {
    Write-Host "WARNING: No DLLs were copied!" -ForegroundColor Red
    Write-Host "Please check Qt and OpenCV installation paths." -ForegroundColor Yellow
}

Write-Host ""
