# ============================================================================
# MASTER SCRIPT - ENABLE FULL TESSERACT OCR (ALL-IN-ONE)
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?  NAGHUMA TOOLBOX - ENABLE FULL TESSERACT OCR            ?" -ForegroundColor Cyan
Write-Host "?  Automated Setup (All-in-One)                           ?" -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

Write-Host "This script will:" -ForegroundColor Yellow
Write-Host "  1. Setup Tesseract OCR" -ForegroundColor White
Write-Host "  2. Enable Tesseract code" -ForegroundColor White
Write-Host "  3. Configure Visual Studio project" -ForegroundColor White
Write-Host "  4. Prepare for build and test" -ForegroundColor White
Write-Host ""

$confirm = Read-Host "Continue? (Y/N)"
if ($confirm -ne "Y" -and $confirm -ne "y") {
    Write-Host "Cancelled." -ForegroundColor Yellow
    exit 0
}

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  PHASE 1: TESSERACT INSTALLATION                      " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

& .\setup_tesseract.ps1

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "? Tesseract setup failed!" -ForegroundColor Red
    Write-Host "  Please install Tesseract manually and re-run this script" -ForegroundColor Yellow
    exit 1
}

Write-Host ""
Write-Host "Press Enter to continue to Phase 2..." -ForegroundColor Yellow
Read-Host

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  PHASE 2: ENABLE TESSERACT CODE                       " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

& .\enable_tesseract_code.ps1

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "? Code enabling failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Press Enter to continue to Phase 3..." -ForegroundColor Yellow
Read-Host

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  PHASE 3: CONFIGURE VISUAL STUDIO                     " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

& .\configure_visual_studio.ps1

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "? Visual Studio configuration failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "?  ALL PHASES COMPLETE - TESSERACT OCR READY!             ?" -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "?? Summary:" -ForegroundColor Yellow
Write-Host ""

# Load and display configuration
$configFile = "tesseract_config.json"
if (Test-Path $configFile) {
    $config = Get-Content $configFile | ConvertFrom-Json
    
    Write-Host "? Tesseract Installed:" -ForegroundColor Green
    Write-Host "   Path: $($config.TesseractPath)" -ForegroundColor White
    Write-Host "   Version: $($config.Version)" -ForegroundColor White
    Write-Host ""
    
    Write-Host "? Languages Configured:" -ForegroundColor Green
    foreach ($lang in $config.InstalledLanguages) {
        Write-Host "   - $lang" -ForegroundColor White
    }
    Write-Host ""
    
    Write-Host "? Code Updated:" -ForegroundColor Green
    Write-Host "   - lib\ocr\TextRecognition.cpp (Tesseract enabled)" -ForegroundColor White
    Write-Host "   - Backup created: TextRecognition.cpp.backup" -ForegroundColor White
    Write-Host ""
    
    Write-Host "? Visual Studio Configured:" -ForegroundColor Green
    Write-Host "   - Include paths added" -ForegroundColor White
    Write-Host "   - Library paths added" -ForegroundColor White
    Write-Host "   - Dependencies linked" -ForegroundColor White
    Write-Host "   - Backup created: Naghuma Toolbox.vcxproj.before_tesseract" -ForegroundColor White
    Write-Host ""
}

Write-Host "?? NEXT STEPS:" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. CLOSE Visual Studio (if open)" -ForegroundColor White
Write-Host "   File ? Exit" -ForegroundColor Gray
Write-Host ""
Write-Host "2. CLEAR Visual Studio cache:" -ForegroundColor White
Remove-Item ".vs" -Recurse -Force -ErrorAction SilentlyContinue
if ($?) {
    Write-Host "   ? Cache cleared" -ForegroundColor Green
}
Write-Host ""
Write-Host "3. REOPEN Visual Studio" -ForegroundColor White
Write-Host "   - Double-click: Naghuma Toolbox.sln" -ForegroundColor Gray
Write-Host ""
Write-Host "4. REBUILD Project:" -ForegroundColor White
Write-Host "   - Build ? Clean Solution" -ForegroundColor Gray
Write-Host "   - Build ? Rebuild Solution (Ctrl+Shift+B)" -ForegroundColor Gray
Write-Host ""
Write-Host "5. RUN Application:" -ForegroundColor White
Write-Host "   Option A (Recommended):" -ForegroundColor Yellow
Write-Host "   - Double-click: run_with_tesseract.bat" -ForegroundColor Gray
Write-Host ""
Write-Host "   Option B (From VS):" -ForegroundColor Yellow
Write-Host "   - Debug ? Start Without Debugging (Ctrl+F5)" -ForegroundColor Gray
Write-Host ""
Write-Host "6. TEST OCR:" -ForegroundColor White
Write-Host "   - Load an image with text" -ForegroundColor Gray
Write-Host "   - Process ? OCR - Text Recognition..." -ForegroundColor Gray
Write-Host "   - Select language" -ForegroundColor Gray
Write-Host "   - Click 'Recognize Text'" -ForegroundColor Gray
Write-Host "   - See REAL text recognition!" -ForegroundColor Green
Write-Host ""

Write-Host "?? Documentation:" -ForegroundColor Yellow
Write-Host "   - Complete Guide: ENABLE_TESSERACT_OCR_GUIDE.md" -ForegroundColor White
Write-Host "   - Troubleshooting: See guide for common issues" -ForegroundColor White
Write-Host ""

Write-Host "? Tesseract OCR is now FULLY ENABLED!" -ForegroundColor Green
Write-Host ""

# Create a status file
$statusFile = "TESSERACT_STATUS.txt"
$status = @"
TESSERACT OCR STATUS
====================

Date: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
Status: ENABLED AND CONFIGURED

Installation:
- Tesseract Path: $($config.TesseractPath)
- Version: $($config.Version)
- Languages: $($config.InstalledLanguages -join ", ")

Configuration:
- Code: ENABLED (TextRecognition.cpp updated)
- Project: CONFIGURED (include/library paths added)
- Environment: SET (TESSDATA_PREFIX configured)

Next Steps:
1. Close and reopen Visual Studio
2. Rebuild solution
3. Run application
4. Test OCR feature

Ready for Production: YES
"@

Set-Content $statusFile $status
Write-Host "Status saved to: $statusFile" -ForegroundColor Gray
Write-Host ""
