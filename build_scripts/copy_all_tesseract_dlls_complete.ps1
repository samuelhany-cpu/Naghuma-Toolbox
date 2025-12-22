# Complete DLL Copy Script - All Tesseract Dependencies
# This copies ALL required DLLs from Tesseract installation

Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  Copying ALL Tesseract Dependencies" -ForegroundColor Yellow
Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$tesseractPath = "C:\Program Files\Tesseract-OCR"
$debugPath = "x64\Debug"

if (-not (Test-Path $debugPath)) {
    Write-Host "? Debug folder not found: $debugPath" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path $tesseractPath)) {
    Write-Host "? Tesseract not found at: $tesseractPath" -ForegroundColor Red
    exit 1
}

# All required DLLs organized by category
$dllGroups = @{
    "Image Libraries" = @(
        "libjpeg-8.dll",
        "libtiff-6.dll", 
        "libwebp-7.dll",
        "libopenjp2-7.dll",
        "libgif-7.dll",
        "libpng16-16.dll",
        "libsharpyuv-0.dll",
        "libwebpmux-3.dll"
    )
    
    "OCR Core" = @(
        "libleptonica-6.dll",
        "libtesseract-5.dll"
    )
    
    "Compression" = @(
        "liblzma-5.dll",
        "libbrotlicommon.dll",
        "libbrotlidec.dll",
        "libjbig-0.dll",
        "libdeflate.dll",
        "libLerc.dll",
        "libbz2-1.dll",
        "libzstd.dll",
        "libb2-1.dll",
        "lz4.dll",
        "libarchive-13.dll"
    )
    
    "Text Rendering" = @(
        "libfreetype-6.dll",
        "libharfbuzz-0.dll",
        "libgraphite2.dll",
        "libfontconfig-1.dll",
        "libfribidi-0.dll"
    )
    
    "Unicode/Locale" = @(
        "libiconv-2.dll",
        "libintl-8.dll",
        "libicudt75.dll",
        "libicuin75.dll",
        "libicuuc75.dll",
        "libthai-0.dll",
        "libdatrie-1.dll"
    )
    
    "GLib/Cairo" = @(
        "libglib-2.0-0.dll",
        "libgobject-2.0-0.dll",
        "libgio-2.0-0.dll",
        "libgmodule-2.0-0.dll",
        "libffi-8.dll",
        "libcairo-2.dll",
        "libpixman-1-0.dll"
    )
    
    "Pango Text" = @(
        "libpango-1.0-0.dll",
        "libpangocairo-1.0-0.dll",
        "libpangoft2-1.0-0.dll",
        "libpangowin32-1.0-0.dll"
    )
    
    "Network" = @(
        "libcurl-4.dll",
        "libssh2-1.dll",
        "libidn2-0.dll",
        "libunistring-5.dll",
        "libpsl-5.dll",
        "libssl-3-x64.dll",
        "libcrypto-3-x64.dll"
    )
    
    "Utilities" = @(
        "libexpat-1.dll",
        "libpcre2-8-0.dll"
    )
    
    "Runtime" = @(
        "libgcc_s_seh-1.dll",
        "libstdc++-6.dll",
        "libwinpthread-1.dll"
    )
}

$totalCopied = 0
$totalSkipped = 0
$totalMissing = 0

foreach ($category in $dllGroups.Keys | Sort-Object) {
    Write-Host ""
    Write-Host "?? $category" -ForegroundColor Yellow
    Write-Host ("?" * 50) -ForegroundColor Gray
    
    foreach ($dll in $dllGroups[$category]) {
        $srcPath = Join-Path $tesseractPath $dll
        $dstPath = Join-Path $debugPath $dll
        
        if (-not (Test-Path $srcPath)) {
            Write-Host "  ??  Not found: $dll" -ForegroundColor DarkYellow
            $totalMissing++
            continue
        }
        
        if (Test-Path $dstPath) {
            Write-Host "  ? Already present: $dll" -ForegroundColor Gray
            $totalSkipped++
            continue
        }
        
        try {
            Copy-Item $srcPath $dstPath -Force
            Write-Host "  ? Copied: $dll" -ForegroundColor Green
            $totalCopied++
        } catch {
            Write-Host "  ? Failed: $dll - $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

Write-Host ""
Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  ? Copied: $totalCopied" -ForegroundColor Green
Write-Host "  ? Already present: $totalSkipped" -ForegroundColor Gray
if ($totalMissing -gt 0) {
    Write-Host "  ??  Not found: $totalMissing" -ForegroundColor DarkYellow
}
Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan

# Final verification
$allDlls = Get-ChildItem "$debugPath\*.dll"
$totalCount = ($allDlls | Measure-Object).Count

Write-Host ""
Write-Host "?? Total DLLs in $debugPath : $totalCount" -ForegroundColor Cyan
Write-Host ""

# Check critical DLLs
$criticalDlls = @(
    "opencv_world430d.dll",
    "tesseract55.dll",
    "libleptonica-6.dll",
    "libtesseract-5.dll",
    "libjpeg-8.dll",
    "libtiff-6.dll",
    "libpng16-16.dll",
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll"
)

Write-Host "Critical DLLs Check:" -ForegroundColor Yellow
$allPresent = $true
foreach ($dll in $criticalDlls) {
    $path = Join-Path $debugPath $dll
    if (Test-Path $path) {
        Write-Host "  ? $dll" -ForegroundColor Green
    } else {
        Write-Host "  ? $dll MISSING!" -ForegroundColor Red
        $allPresent = $false
    }
}

Write-Host ""
if ($allPresent) {
    Write-Host "? ALL CRITICAL DLLs PRESENT - READY TO RUN!" -ForegroundColor Green
} else {
    Write-Host "??  Some critical DLLs are missing!" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
