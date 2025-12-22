# ============================================================================
# PHASE 21 - BUILD AND TEST SCRIPT
# ============================================================================
# Complete workflow: Verify ? Build ? Run ? Test
# ============================================================================

Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   PHASE 21 - BUILD & TEST SCRIPT      ?" -ForegroundColor Cyan
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$ErrorActionPreference = "Continue"

# ============================================================================
# STEP 1: PRE-BUILD VERIFICATION
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 1: Pre-Build Verification       ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

$allGood = $true

# Check critical files
$criticalFiles = @(
    "include\IntensityTransformDialog.h",
    "src\IntensityTransformDialog.cpp",
    "src\moc_IntensityTransformDialog.cpp",
    "include\SharpeningDialog.h",
    "src\SharpeningDialog.cpp",
    "src\moc_SharpeningDialog.cpp",
    "src\MainWindow_Phase21.cpp"
)

foreach ($file in $criticalFiles) {
    if (Test-Path $file) {
        Write-Host "  ? $file" -ForegroundColor Green
    } else {
        Write-Host "  ? $file MISSING" -ForegroundColor Red
        $allGood = $false
    }
}

if (-not $allGood) {
    Write-Host ""
    Write-Host "? Critical files missing! Run .\apply_phase21_complete.ps1 first" -ForegroundColor Red
    exit 1
}

# Check MainWindow.cpp includes
$mainWindowContent = Get-Content "src\MainWindow.cpp" -Raw
if ($mainWindowContent -notmatch 'IntensityTransformDialog.h') {
    Write-Host "  ? MainWindow.cpp missing IntensityTransformDialog.h include" -ForegroundColor Red
    $allGood = $false
} else {
    Write-Host "  ? MainWindow.cpp includes Phase 21 headers" -ForegroundColor Green
}

# Check project file
$projectContent = Get-Content "Naghuma Toolbox.vcxproj" -Raw
if ($projectContent -notmatch 'moc_IntensityTransformDialog.cpp') {
    Write-Host "  ? Project file missing MOC files" -ForegroundColor Red
    $allGood = $false
} else {
    Write-Host "  ? Project file includes Phase 21 files" -ForegroundColor Green
}

if (-not $allGood) {
    Write-Host ""
    Write-Host "? Pre-build checks failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "? Pre-build verification passed!" -ForegroundColor Green
Write-Host ""

# ============================================================================
# STEP 2: FIND MSBUILD
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 2: Locating MSBuild             ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

$msbuildPaths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe",
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
)

$msbuild = $null
foreach ($path in $msbuildPaths) {
    if (Test-Path $path) {
        $msbuild = $path
        Write-Host "  ? Found MSBuild: $path" -ForegroundColor Green
        break
    }
}

if (-not $msbuild) {
    Write-Host "  ? MSBuild not found automatically" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Please build manually:" -ForegroundColor Cyan
    Write-Host "  1. Open 'Naghuma Toolbox.sln' in Visual Studio" -ForegroundColor White
    Write-Host "  2. Press Ctrl+Shift+B to build" -ForegroundColor White
    Write-Host "  3. Run the executable from x64\Debug\" -ForegroundColor White
    Write-Host ""
    Write-Host "Press any key to exit..." -ForegroundColor Yellow
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 0
}

Write-Host ""

# ============================================================================
# STEP 3: CLEAN BUILD
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 3: Clean Build                  ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

Write-Host "  ? Cleaning previous build..." -ForegroundColor Cyan
& $msbuild "Naghuma Toolbox.sln" /t:Clean /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ? Clean had warnings (continuing)" -ForegroundColor Yellow
}

Write-Host "  ? Clean complete" -ForegroundColor Green
Write-Host ""

# ============================================================================
# STEP 4: BUILD PROJECT
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 4: Building Project             ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

Write-Host "  ? Compiling Phase 21 features..." -ForegroundColor Cyan
& $msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

Write-Host ""

if ($LASTEXITCODE -eq 0) {
    Write-Host "??????????????????????????????????????????" -ForegroundColor Green
    Write-Host "?        ? BUILD SUCCESSFUL! ?         ?" -ForegroundColor Green
    Write-Host "??????????????????????????????????????????" -ForegroundColor Green
} else {
    Write-Host "??????????????????????????????????????????" -ForegroundColor Red
    Write-Host "?          ? BUILD FAILED! ?           ?" -ForegroundColor Red
    Write-Host "??????????????????????????????????????????" -ForegroundColor Red
    Write-Host ""
    Write-Host "Check the error messages above." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Common fixes:" -ForegroundColor Cyan
    Write-Host "  • Close Visual Studio and rebuild" -ForegroundColor White
    Write-Host "  • Delete x64\Debug folder and rebuild" -ForegroundColor White
    Write-Host "  • Verify all MOC files were generated" -ForegroundColor White
    exit 1
}

Write-Host ""

# ============================================================================
# STEP 5: VERIFY EXECUTABLE
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 5: Verify Executable            ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

$exePath = "x64\Debug\Naghuma Toolbox.exe"
if (Test-Path $exePath) {
    $exeInfo = Get-Item $exePath
    $sizeKB = [math]::Round($exeInfo.Length / 1KB, 2)
    $modified = $exeInfo.LastWriteTime
    
    Write-Host "  ? Executable found" -ForegroundColor Green
    Write-Host "    Path: $exePath" -ForegroundColor Cyan
    Write-Host "    Size: $sizeKB KB" -ForegroundColor Cyan
    Write-Host "    Modified: $modified" -ForegroundColor Cyan
} else {
    Write-Host "  ? Executable not found at $exePath" -ForegroundColor Red
    exit 1
}

Write-Host ""

# ============================================================================
# STEP 6: LAUNCH APPLICATION
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 6: Launch Application           ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

Write-Host "Would you like to launch the application now? (Y/N)" -ForegroundColor Cyan
$response = Read-Host

if ($response -eq 'Y' -or $response -eq 'y') {
    Write-Host ""
    Write-Host "  ? Launching Naghuma Toolbox..." -ForegroundColor Cyan
    Start-Process -FilePath $exePath
    Write-Host "  ? Application started!" -ForegroundColor Green
    Write-Host ""
} else {
    Write-Host ""
    Write-Host "  Skipped launch" -ForegroundColor Yellow
    Write-Host ""
}

# ============================================================================
# STEP 7: TESTING GUIDE
# ============================================================================
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?  STEP 7: Testing Guide                ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?      PHASE 21 TESTING CHECKLIST       ?" -ForegroundColor Cyan
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

Write-Host "?? Test Intensity Transformations:" -ForegroundColor Yellow
Write-Host "   1. Load an image" -ForegroundColor White
Write-Host "   2. Go to: Process ? Intensity Transformations ? Gamma & Log Transform" -ForegroundColor White
Write-Host "   3. Test operations:" -ForegroundColor White
Write-Host "      • Gamma < 1.0 (e.g., 0.4) ? Should brighten" -ForegroundColor Cyan
Write-Host "      • Gamma > 1.0 (e.g., 2.2) ? Should darken" -ForegroundColor Cyan
Write-Host "      • Log Transform ? Compresses dynamic range" -ForegroundColor Cyan
Write-Host ""

Write-Host "?? Test Sharpening Filters:" -ForegroundColor Yellow
Write-Host "   1. Load an image (portrait works best)" -ForegroundColor White
Write-Host "   2. Go to: Process ? Sharpening Filters ? Laplacian/Unsharp/High-Boost" -ForegroundColor White
Write-Host "   3. Test filters:" -ForegroundColor White
Write-Host "      • Laplacian ? Automatic edge enhancement" -ForegroundColor Cyan
Write-Host "      • Unsharp Mask (amount 1.5) ? Natural sharpening" -ForegroundColor Cyan
Write-Host "      • High-Boost (k = 2.0) ? Strong edge emphasis" -ForegroundColor Cyan
Write-Host ""

Write-Host "? Expected Behaviors:" -ForegroundColor Green
Write-Host "   • Preview updates in real-time" -ForegroundColor White
Write-Host "   • Sliders move smoothly" -ForegroundColor White
Write-Host "   • 'Apply' button saves to layer" -ForegroundColor White
Write-Host "   • 'Cancel' discards changes" -ForegroundColor White
Write-Host "   • Undo (Ctrl+Z) removes last layer" -ForegroundColor White
Write-Host ""

Write-Host "??????????????????????????????????????????" -ForegroundColor Green
Write-Host "?    ?? PHASE 21 READY FOR TESTING! ??  ?" -ForegroundColor Green
Write-Host "??????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

# Quick command reference
Write-Host "Quick Commands:" -ForegroundColor Yellow
Write-Host "  • Rerun this script: .\BUILD_AND_TEST_PHASE21.ps1" -ForegroundColor Cyan
Write-Host "  • Open in VS: start 'Naghuma Toolbox.sln'" -ForegroundColor Cyan
Write-Host "  • Run app: .\x64\Debug\Naghuma` Toolbox.exe" -ForegroundColor Cyan
Write-Host ""
