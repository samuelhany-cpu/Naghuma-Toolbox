Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  OCR Complete Setup & Build" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Verify all OCR files exist
Write-Host "[1/5] Verifying OCR files..." -ForegroundColor Yellow
$ocrFiles = @(
    "lib\ocr\TextRecognition.h",
    "lib\ocr\TextRecognition.cpp",
    "include\OCRDialog.h",
    "src\OCRDialog.cpp",
    "src\moc_OCRDialog.cpp"
)

$allFilesExist = $true
foreach ($file in $ocrFiles) {
    if (Test-Path $file) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
        $allFilesExist = $false
    }
}

if (!$allFilesExist) {
    Write-Host ""
    Write-Host "ERROR: Some OCR files are missing!" -ForegroundColor Red
    Write-Host "Please ensure all files are created first." -ForegroundColor Yellow
    exit 1
}

Write-Host "  All OCR files present!" -ForegroundColor Green

# Step 2: Check tessdata
Write-Host ""
Write-Host "[2/5] Checking tessdata..." -ForegroundColor Yellow
$tessDataPath = "vcpkg\installed\x64-windows\share\tessdata\eng.traineddata"
if (Test-Path $tessDataPath) {
    $size = [math]::Round((Get-Item $tessDataPath).Length/1MB, 2)
    Write-Host "  [OK] eng.traineddata ($size MB)" -ForegroundColor Green
} else {
    Write-Host "  [WARNING] eng.traineddata not found" -ForegroundColor Yellow
    Write-Host "  Downloading..." -ForegroundColor Cyan
    
    $tessDir = "vcpkg\installed\x64-windows\share\tessdata"
    if (!(Test-Path $tessDir)) {
        New-Item -ItemType Directory -Force -Path $tessDir | Out-Null
    }
    
    try {
        $url = "https://github.com/tesseract-ocr/tessdata/raw/main/eng.traineddata"
        Invoke-WebRequest -Uri $url -OutFile $tessDataPath -UseBasicParsing
        Write-Host "  [OK] Downloaded eng.traineddata" -ForegroundColor Green
    } catch {
        Write-Host "  [ERROR] Failed to download: $_" -ForegroundColor Red
        Write-Host "  OCR may not work without language data" -ForegroundColor Yellow
    }
}

# Step 3: Add OCR files to project
Write-Host ""
Write-Host "[3/5] Adding OCR files to project..." -ForegroundColor Yellow
& ".\add_ocr_files_to_project.ps1"
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERROR: Failed to add OCR files to project!" -ForegroundColor Red
    exit 1
}

# Step 4: Clean and rebuild
Write-Host ""
Write-Host "[4/5] Cleaning project..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Clean /nologo /v:minimal | Out-Null
Write-Host "  Project cleaned" -ForegroundColor Green

Write-Host ""
Write-Host "[5/5] Building project with OCR..." -ForegroundColor Yellow
Write-Host "  This may take a few minutes..." -ForegroundColor Cyan
Write-Host ""

$buildOutput = msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Build /nologo /v:minimal 2>&1

# Check for errors
$errors = $buildOutput | Select-String "error"
if ($errors) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  BUILD FAILED!" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Errors:" -ForegroundColor Yellow
    $errors | ForEach-Object { Write-Host $_ -ForegroundColor Red }
    Write-Host ""
    exit 1
}

# Check for warnings
$warnings = $buildOutput | Select-String "warning"
if ($warnings) {
    Write-Host ""
    Write-Host "Build completed with warnings:" -ForegroundColor Yellow
    $warnings | Select-Object -First 5 | ForEach-Object { Write-Host "  $_" -ForegroundColor Yellow }
    if ($warnings.Count -gt 5) {
        Write-Host "  ... and $($warnings.Count - 5) more warnings" -ForegroundColor Gray
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  BUILD SUCCESSFUL!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Step 5: Verify executable and DLLs
Write-Host "Verifying build outputs..." -ForegroundColor Yellow
$exePath = "x64\Debug\Naghuma Toolbox.exe"
if (Test-Path $exePath) {
    Write-Host "  [OK] Naghuma Toolbox.exe" -ForegroundColor Green
    
    # Check critical DLLs
    $criticalDlls = @(
        "tesseract55d.dll",
        "leptonica-1.85.0d.dll",
        "Qt6Cored.dll",
        "opencv_world430d.dll"
    )
    
    $missingDlls = @()
    foreach ($dll in $criticalDlls) {
        $dllPath = "x64\Debug\$dll"
        if (Test-Path $dllPath) {
            Write-Host "  [OK] $dll" -ForegroundColor Green
        } else {
            Write-Host "  [MISSING] $dll" -ForegroundColor Red
            $missingDlls += $dll
        }
    }
    
    if ($missingDlls.Count -gt 0) {
        Write-Host ""
        Write-Host "WARNING: Some DLLs are missing!" -ForegroundColor Yellow
        Write-Host "The application may not run properly." -ForegroundColor Yellow
        Write-Host ""
        Write-Host "Copy missing DLLs with:" -ForegroundColor Cyan
        Write-Host "  .\copy_all_vcpkg_dlls.ps1" -ForegroundColor White
        Write-Host ""
    }
} else {
    Write-Host "  [ERROR] Executable not found!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  OCR SETUP COMPLETE!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "OCR Features:" -ForegroundColor Yellow
Write-Host "  - Text recognition from images" -ForegroundColor White
Write-Host "  - Preprocessing options (grayscale, threshold, denoise)" -ForegroundColor White
Write-Host "  - Confidence scoring" -ForegroundColor White
Write-Host "  - Word-level recognition" -ForegroundColor White
Write-Host "  - Bounding box visualization" -ForegroundColor White
Write-Host "  - Copy/Save/Export functionality" -ForegroundColor White
Write-Host ""
Write-Host "How to use:" -ForegroundColor Yellow
Write-Host "  1. Run the application:" -ForegroundColor Cyan
Write-Host "     .\x64\Debug\`"Naghuma Toolbox.exe`"" -ForegroundColor White
Write-Host ""
Write-Host "  2. Load an image with text" -ForegroundColor Cyan
Write-Host ""
Write-Host "  3. Go to: Process -> OCR - Text Recognition..." -ForegroundColor Cyan
Write-Host ""
Write-Host "  4. Adjust preprocessing options" -ForegroundColor Cyan
Write-Host ""
Write-Host "  5. Click 'Recognize Text'" -ForegroundColor Cyan
Write-Host ""

# Offer to run
Write-Host "Would you like to run the application now? (Y/N): " -NoNewline -ForegroundColor Yellow
$response = Read-Host
if ($response -eq 'Y' -or $response -eq 'y') {
    Write-Host ""
    Write-Host "Launching application..." -ForegroundColor Green
    Start-Process $exePath
}

Write-Host ""
