# ============================================================================
# DOWNLOAD TESSERACT LANGUAGE FILES (WITH ADMIN PRIVILEGES)
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  DOWNLOADING TESSERACT LANGUAGE FILES                 " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "??  This script needs Administrator privileges to write to Program Files" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Restarting with Administrator privileges..." -ForegroundColor Yellow
    
    Start-Process powershell -Verb RunAs -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`""
    exit
}

Write-Host "? Running as Administrator" -ForegroundColor Green
Write-Host ""

$tessdataPath = "C:\Program Files\Tesseract-OCR\tessdata"

if (-not (Test-Path $tessdataPath)) {
    Write-Host "? Tessdata directory not found: $tessdataPath" -ForegroundColor Red
    Write-Host "  Please install Tesseract first" -ForegroundColor Yellow
    pause
    exit 1
}

Write-Host "Tessdata directory: $tessdataPath" -ForegroundColor White
Write-Host ""

$languages = @(
    @{Code="ara"; Name="Arabic"; Url="https://github.com/tesseract-ocr/tessdata/raw/main/ara.traineddata"},
    @{Code="fra"; Name="French"; Url="https://github.com/tesseract-ocr/tessdata/raw/main/fra.traineddata"},
    @{Code="deu"; Name="German"; Url="https://github.com/tesseract-ocr/tessdata/raw/main/deu.traineddata"},
    @{Code="spa"; Name="Spanish"; Url="https://github.com/tesseract-ocr/tessdata/raw/main/spa.traineddata"},
    @{Code="chi_sim"; Name="Chinese (Simplified)"; Url="https://github.com/tesseract-ocr/tessdata/raw/main/chi_sim.traineddata"},
    @{Code="jpn"; Name="Japanese"; Url="https://github.com/tesseract-ocr/tessdata/raw/main/jpn.traineddata"}
)

Write-Host "Downloading language files..." -ForegroundColor Yellow
Write-Host ""

$downloaded = 0
$skipped = 0
$failed = 0

foreach ($lang in $languages) {
    $outputFile = Join-Path $tessdataPath "$($lang.Code).traineddata"
    
    if (Test-Path $outputFile) {
        $size = (Get-Item $outputFile).Length / 1MB
        Write-Host "  ? $($lang.Name) - Already exists ($([Math]::Round($size, 2)) MB)" -ForegroundColor Gray
        $skipped++
        continue
    }
    
    Write-Host "  Downloading $($lang.Name)..." -ForegroundColor Yellow -NoNewline
    
    try {
        # Use .NET WebClient for better compatibility
        $webClient = New-Object System.Net.WebClient
        $webClient.DownloadFile($lang.Url, $outputFile)
        
        if (Test-Path $outputFile) {
            $size = (Get-Item $outputFile).Length / 1MB
            Write-Host " ? ($([Math]::Round($size, 2)) MB)" -ForegroundColor Green
            $downloaded++
        } else {
            Write-Host " ? Failed" -ForegroundColor Red
            $failed++
        }
    } catch {
        Write-Host " ? Error: $($_.Exception.Message)" -ForegroundColor Red
        $failed++
    }
}

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  DOWNLOAD COMPLETE                                     " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  Downloaded: $downloaded files" -ForegroundColor $(if ($downloaded -gt 0) { "Green" } else { "Gray" })
Write-Host "  Skipped:    $skipped files (already exist)" -ForegroundColor Gray
Write-Host "  Failed:     $failed files" -ForegroundColor $(if ($failed -gt 0) { "Red" } else { "Gray" })
Write-Host ""

if ($failed -gt 0) {
    Write-Host "??  Some files failed to download" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Manual Download:" -ForegroundColor Yellow
    Write-Host "  1. Visit: https://github.com/tesseract-ocr/tessdata" -ForegroundColor White
    Write-Host "  2. Download missing .traineddata files" -ForegroundColor White
    Write-Host "  3. Copy to: $tessdataPath" -ForegroundColor White
    Write-Host ""
}

Write-Host "? Language files ready!" -ForegroundColor Green
Write-Host ""
Write-Host "Press Enter to continue..." -ForegroundColor Yellow
Read-Host
