# Comprehensive DLL Diagnostic and Error Detection

Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  NAGHUMA TOOLBOX - DLL DIAGNOSTIC" -ForegroundColor Yellow
Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$debugPath = "x64\Debug"
$exePath = Join-Path $debugPath "Naghuma Toolbox.exe"

# 1. Check EXE exists
Write-Host "1. Checking executable..." -ForegroundColor Yellow
if (Test-Path $exePath) {
    Write-Host "   ? EXE found: $exePath" -ForegroundColor Green
} else {
    Write-Host "   ? EXE not found!" -ForegroundColor Red
    exit 1
}

# 2. Count DLLs
Write-Host ""
Write-Host "2. Checking DLL count..." -ForegroundColor Yellow
$dllCount = (Get-ChildItem "$debugPath\*.dll" | Measure-Object).Count
Write-Host "   ? Total DLLs present: $dllCount" -ForegroundColor Green

# 3. List all DLLs by category
Write-Host ""
Write-Host "3. DLL Inventory:" -ForegroundColor Yellow

$categories = @{
    "OpenCV" = @("opencv_world")
    "Qt6" = @("Qt6Core", "Qt6Gui", "Qt6Widgets")
    "Tesseract" = @("tesseract", "leptonica")
    "Image" = @("jpeg", "tiff", "png", "webp", "gif")
    "Compression" = @("zlib", "lz4", "bz2", "zstd", "lzma")
    "SSL" = @("ssl", "crypto")
}

foreach ($cat in $categories.Keys) {
    $found = @()
    $patterns = $categories[$cat]
    
    foreach ($pattern in $patterns) {
        $matching = Get-ChildItem "$debugPath\*.dll" | 
                    Where-Object { $_.Name -like "*$pattern*" } |
                    Select-Object -ExpandProperty Name
        if ($matching) {
            $found += $matching
        }
    }
    
    if ($found.Count -gt 0) {
        Write-Host "   ? $cat : $($found.Count) DLLs" -ForegroundColor Green
        foreach ($dll in $found) {
            Write-Host "      • $dll" -ForegroundColor Gray
        }
    } else {
        Write-Host "   ??  $cat : No DLLs found" -ForegroundColor Yellow
    }
}

# 4. Check specific critical DLLs
Write-Host ""
Write-Host "4. Critical DLLs check:" -ForegroundColor Yellow

$criticalDlls = @(
    "opencv_world430d.dll",
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll",
    "tesseract55.dll",
    "libleptonica-6.dll",
    "libtesseract-5.dll",
    "libjpeg-8.dll",
    "libpng16-16.dll",
    "libtiff-6.dll",
    "libcrypto-3-x64.dll",
    "libssl-3-x64.dll"
)

$allCriticalPresent = $true
foreach ($dll in $criticalDlls) {
    $path = Join-Path $debugPath $dll
    if (Test-Path $path) {
        Write-Host "   ? $dll" -ForegroundColor Green
    } else {
        Write-Host "   ? $dll MISSING!" -ForegroundColor Red
        $allCriticalPresent = $false
    }
}

# 5. Try to run and capture errors
Write-Host ""
Write-Host "5. Attempting to launch application..." -ForegroundColor Yellow
Write-Host "   (If it fails, Windows will show error dialog)" -ForegroundColor Gray
Write-Host ""

try {
    $process = Start-Process $exePath -PassThru -NoNewWindow
    
    Start-Sleep -Seconds 2
    
    if ($process.HasExited) {
        Write-Host "   ? Process exited immediately (code: $($process.ExitCode))" -ForegroundColor Red
        Write-Host ""
        Write-Host "   This usually means a DLL is missing or incompatible." -ForegroundColor Yellow
        Write-Host "   Check the Windows error dialog for the specific DLL name." -ForegroundColor Yellow
    } else {
        Write-Host "   ? Application is running (PID: $($process.Id))" -ForegroundColor Green
        Write-Host ""
        Write-Host "   If you see a DLL error dialog:" -ForegroundColor Cyan
        Write-Host "   1. Note the EXACT DLL name from the error" -ForegroundColor Cyan
        Write-Host "   2. Close the error dialog" -ForegroundColor Cyan
        Write-Host "   3. Tell me the DLL name" -ForegroundColor Cyan
        Write-Host "   4. I will copy it for you" -ForegroundColor Cyan
    }
} catch {
    Write-Host "   ? Failed to start: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host ""
Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""
Write-Host "DIAGNOSTIC COMPLETE" -ForegroundColor Yellow
Write-Host ""
Write-Host "If you see a missing DLL error:" -ForegroundColor Cyan
Write-Host "  1. Tell me the exact DLL name" -ForegroundColor White
Write-Host "  2. I will find and copy it" -ForegroundColor White
Write-Host ""
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
