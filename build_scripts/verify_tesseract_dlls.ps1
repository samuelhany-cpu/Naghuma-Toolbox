# ============================================================================
# VERIFY ALL REQUIRED DLLs ARE PRESENT
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  TESSERACT DLL VERIFICATION                            " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$debugFolder = "x64\Debug"
$releaseFolder = "x64\Release"

# Required Tesseract DLLs
$requiredDlls = @(
    "archive.dll",
    "bz2.dll",
    "gif.dll",
    "jpeg62.dll",
    "leptonica-1.85.0.dll",
    "libcrypto-3-x64.dll",
    "libcurl.dll",
    "liblzma.dll",
    "libpng16.dll",
    "libsharpyuv.dll",
    "libssl-3-x64.dll",
    "libwebp.dll",
    "libwebpdecoder.dll",
    "libwebpdemux.dll",
    "libwebpmux.dll",
    "lz4.dll",
    "openjp2.dll",
    "tesseract55.dll",
    "tiff.dll",
    "turbojpeg.dll",
    "zlib1.dll",
    "zstd.dll"
)

# Check Debug folder
Write-Host "Checking Debug folder: $debugFolder" -ForegroundColor Yellow
Write-Host ""

if (Test-Path $debugFolder) {
    $missingDebug = @()
    $foundDebug = 0
    
    foreach ($dll in $requiredDlls) {
        $path = Join-Path $debugFolder $dll
        if (Test-Path $path) {
            Write-Host "  ? $dll" -ForegroundColor Green
            $foundDebug++
        } else {
            Write-Host "  ? $dll - MISSING" -ForegroundColor Red
            $missingDebug += $dll
        }
    }
    
    Write-Host ""
    Write-Host "Debug Summary: $foundDebug / $($requiredDlls.Count) DLLs present" -ForegroundColor $(if ($foundDebug -eq $requiredDlls.Count) { "Green" } else { "Yellow" })
    
    if ($missingDebug.Count -gt 0) {
        Write-Host ""
        Write-Host "Missing DLLs in Debug:" -ForegroundColor Red
        foreach ($dll in $missingDebug) {
            Write-Host "  - $dll" -ForegroundColor Red
        }
    }
} else {
    Write-Host "? Debug folder not found: $debugFolder" -ForegroundColor Red
}

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan

# Check Release folder
Write-Host ""
Write-Host "Checking Release folder: $releaseFolder" -ForegroundColor Yellow
Write-Host ""

if (Test-Path $releaseFolder) {
    $missingRelease = @()
    $foundRelease = 0
    
    foreach ($dll in $requiredDlls) {
        $path = Join-Path $releaseFolder $dll
        if (Test-Path $path) {
            Write-Host "  ? $dll" -ForegroundColor Green
            $foundRelease++
        } else {
            Write-Host "  ? $dll - MISSING" -ForegroundColor Red
            $missingRelease += $dll
        }
    }
    
    Write-Host ""
    Write-Host "Release Summary: $foundRelease / $($requiredDlls.Count) DLLs present" -ForegroundColor $(if ($foundRelease -eq $requiredDlls.Count) { "Green" } else { "Yellow" })
    
    if ($missingRelease.Count -gt 0) {
        Write-Host ""
        Write-Host "Missing DLLs in Release:" -ForegroundColor Red
        foreach ($dll in $missingRelease) {
            Write-Host "  - $dll" -ForegroundColor Red
        }
    }
} else {
    Write-Host "? Release folder not found: $releaseFolder" -ForegroundColor Yellow
    Write-Host "  (This is OK if you haven't built in Release mode)" -ForegroundColor Gray
}

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  VERIFICATION COMPLETE                                  " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

# Overall status
$allPresent = (Test-Path $debugFolder) -and ($missingDebug.Count -eq 0)

if ($allPresent) {
    Write-Host "? All required DLLs are present!" -ForegroundColor Green
    Write-Host ""
    Write-Host "You can now run the application:" -ForegroundColor White
    Write-Host "  • Ctrl+F5 in Visual Studio" -ForegroundColor Cyan
    Write-Host "  • OR: .\run_with_tesseract.bat" -ForegroundColor Cyan
    Write-Host ""
} else {
    Write-Host "? Some DLLs are missing!" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "To fix this, run:" -ForegroundColor White
    Write-Host "  .\copy_all_tesseract_dlls.bat" -ForegroundColor Cyan
    Write-Host ""
}

Write-Host "Note: If you see 'archive.dll' error, run the copy script above." -ForegroundColor Gray
Write-Host ""
