# ============================================================================
# TESSERACT OCR SETUP - AUTOMATED INSTALLATION
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  TESSERACT OCR SETUP - FULL INSTALLATION             " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Step 1: Check if Tesseract is already installed
Write-Host "Step 1: Checking for existing Tesseract installation..." -ForegroundColor Yellow
Write-Host ""

$tesseractPaths = @(
    "C:\Program Files\Tesseract-OCR",
    "C:\Program Files (x86)\Tesseract-OCR",
    "$env:LOCALAPPDATA\Tesseract-OCR"
)

$tesseractInstalled = $false
$tesseractPath = ""

foreach ($path in $tesseractPaths) {
    if (Test-Path "$path\tesseract.exe") {
        $tesseractInstalled = $true
        $tesseractPath = $path
        Write-Host "? Found Tesseract at: $path" -ForegroundColor Green
        break
    }
}

if (-not $tesseractInstalled) {
    Write-Host "? Tesseract not found" -ForegroundColor Red
    Write-Host ""
    Write-Host "MANUAL INSTALLATION REQUIRED:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "1. Download Tesseract installer:" -ForegroundColor White
    Write-Host "   https://github.com/UB-Mannheim/tesseract/wiki" -ForegroundColor Cyan
    Write-Host "   Direct link: https://digi.bib.uni-mannheim.de/tesseract/" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "2. Download: tesseract-ocr-w64-setup-v5.3.3.20231005.exe" -ForegroundColor White
    Write-Host "   (or latest version)" -ForegroundColor Gray
    Write-Host ""
    Write-Host "3. Run installer with these options:" -ForegroundColor White
    Write-Host "   - Install location: C:\Program Files\Tesseract-OCR" -ForegroundColor Gray
    Write-Host "   - Select 'Additional language data (download)'" -ForegroundColor Gray
    Write-Host "   - Check: English, Arabic, French, German, Spanish, Chinese, Japanese" -ForegroundColor Gray
    Write-Host ""
    Write-Host "4. After installation, run this script again" -ForegroundColor White
    Write-Host ""
    
    $download = Read-Host "Would you like to open the download page now? (Y/N)"
    if ($download -eq "Y" -or $download -eq "y") {
        Start-Process "https://github.com/UB-Mannheim/tesseract/wiki"
    }
    
    exit 1
}

# Step 2: Verify tessdata directory
Write-Host ""
Write-Host "Step 2: Verifying tessdata directory..." -ForegroundColor Yellow
Write-Host ""

$tessdataPath = Join-Path $tesseractPath "tessdata"

if (Test-Path $tessdataPath) {
    Write-Host "? tessdata directory found: $tessdataPath" -ForegroundColor Green
} else {
    Write-Host "? tessdata directory not found" -ForegroundColor Red
    exit 1
}

# Step 3: Check for language data files
Write-Host ""
Write-Host "Step 3: Checking language data files..." -ForegroundColor Yellow
Write-Host ""

$requiredLanguages = @(
    @{Code="eng"; Name="English"},
    @{Code="ara"; Name="Arabic"},
    @{Code="fra"; Name="French"},
    @{Code="deu"; Name="German"},
    @{Code="spa"; Name="Spanish"},
    @{Code="chi_sim"; Name="Chinese (Simplified)"},
    @{Code="jpn"; Name="Japanese"}
)

$missingLanguages = @()

foreach ($lang in $requiredLanguages) {
    $trainedDataFile = Join-Path $tessdataPath "$($lang.Code).traineddata"
    if (Test-Path $trainedDataFile) {
        $size = (Get-Item $trainedDataFile).Length / 1MB
        Write-Host "  ? $($lang.Name) ($($lang.Code).traineddata) - $([Math]::Round($size, 2)) MB" -ForegroundColor Green
    } else {
        Write-Host "  ? $($lang.Name) ($($lang.Code).traineddata) - MISSING" -ForegroundColor Red
        $missingLanguages += $lang
    }
}

if ($missingLanguages.Count -gt 0) {
    Write-Host ""
    Write-Host "Missing language files detected!" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Downloading missing language files..." -ForegroundColor Cyan
    
    $baseUrl = "https://github.com/tesseract-ocr/tessdata/raw/main"
    
    foreach ($lang in $missingLanguages) {
        Write-Host "  Downloading $($lang.Name)..." -ForegroundColor Yellow
        $url = "$baseUrl/$($lang.Code).traineddata"
        $output = Join-Path $tessdataPath "$($lang.Code).traineddata"
        
        try {
            Invoke-WebRequest -Uri $url -OutFile $output -UseBasicParsing
            $size = (Get-Item $output).Length / 1MB
            Write-Host "    ? Downloaded ($([Math]::Round($size, 2)) MB)" -ForegroundColor Green
        } catch {
            Write-Host "    ? Failed to download" -ForegroundColor Red
            Write-Host "    Manual download URL: $url" -ForegroundColor Gray
        }
    }
}

# Step 4: Set environment variable
Write-Host ""
Write-Host "Step 4: Setting TESSDATA_PREFIX environment variable..." -ForegroundColor Yellow
Write-Host ""

try {
    [System.Environment]::SetEnvironmentVariable("TESSDATA_PREFIX", $tessdataPath, "User")
    Write-Host "? TESSDATA_PREFIX set to: $tessdataPath" -ForegroundColor Green
    Write-Host "  (User environment variable)" -ForegroundColor Gray
} catch {
    Write-Host "? Failed to set environment variable" -ForegroundColor Red
    Write-Host "  Please set manually:" -ForegroundColor Yellow
    Write-Host "  Variable: TESSDATA_PREFIX" -ForegroundColor White
    Write-Host "  Value: $tessdataPath" -ForegroundColor White
}

# Step 5: Save configuration
Write-Host ""
Write-Host "Step 5: Saving Tesseract configuration..." -ForegroundColor Yellow
Write-Host ""

$configFile = "tesseract_config.json"
$config = @{
    TesseractPath = $tesseractPath
    TessdataPath = $tessdataPath
    IncludePath = Join-Path $tesseractPath "include"
    LibPath = Join-Path $tesseractPath "lib"
    BinPath = Join-Path $tesseractPath "bin"
    Version = "5.3.3"
    InstalledLanguages = $requiredLanguages | ForEach-Object { $_.Code }
}

$config | ConvertTo-Json | Set-Content $configFile

Write-Host "? Configuration saved to: $configFile" -ForegroundColor Green

# Step 6: Verify Tesseract libraries
Write-Host ""
Write-Host "Step 6: Verifying Tesseract libraries..." -ForegroundColor Yellow
Write-Host ""

$libPath = $config.LibPath
$requiredLibs = @("tesseract53.lib", "leptonica-1.83.1.lib")

if (Test-Path $libPath) {
    Write-Host "? Library directory found: $libPath" -ForegroundColor Green
    
    foreach ($lib in $requiredLibs) {
        $libFile = Join-Path $libPath $lib
        if (Test-Path $libFile) {
            $size = (Get-Item $libFile).Length / 1KB
            Write-Host "  ? $lib ($([Math]::Round($size, 2)) KB)" -ForegroundColor Green
        } else {
            Write-Host "  ? $lib - MISSING" -ForegroundColor Red
        }
    }
} else {
    Write-Host "? Library directory not found" -ForegroundColor Red
    Write-Host "  This might be a different Tesseract version" -ForegroundColor Yellow
}

# Step 7: Summary
Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  TESSERACT SETUP COMPLETE                             " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "?? Configuration Summary:" -ForegroundColor Yellow
Write-Host "  Tesseract Path: $tesseractPath" -ForegroundColor White
Write-Host "  Tessdata Path:  $tessdataPath" -ForegroundColor White
Write-Host "  Include Path:   $($config.IncludePath)" -ForegroundColor White
Write-Host "  Library Path:   $($config.LibPath)" -ForegroundColor White
Write-Host ""

Write-Host "?? Installed Languages:" -ForegroundColor Yellow
foreach ($lang in $requiredLanguages) {
    Write-Host "  ? $($lang.Name)" -ForegroundColor Green
}
Write-Host ""

Write-Host "?? Next Steps:" -ForegroundColor Cyan
Write-Host "  1. Run: .\enable_tesseract_code.ps1" -ForegroundColor White
Write-Host "  2. Run: .\configure_visual_studio.ps1" -ForegroundColor White
Write-Host "  3. Rebuild project in Visual Studio" -ForegroundColor White
Write-Host "  4. Test OCR with real images!" -ForegroundColor White
Write-Host ""

Write-Host "? Tesseract is ready for integration!" -ForegroundColor Green
Write-Host ""
