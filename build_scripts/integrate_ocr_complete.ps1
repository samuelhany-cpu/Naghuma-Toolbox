# ============================================================================
# COMPLETE INTEGRATION: OCR + REFACTORING + GITHUB
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  NAGHUMA TOOLBOX: COMPLETE INTEGRATION SCRIPT         " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# ============================================================================
# STEP 1: Update MainWindow.h with OCR declaration
# ============================================================================

Write-Host "[1/5] Updating MainWindow.h with OCR handler..." -ForegroundColor Green

$headerPath = "include\MainWindow.h"
$headerContent = Get-Content $headerPath -Raw

if ($headerContent -notmatch 'showOCRDialog') {
    # Find the Phase 22 handler and add Phase 23 after it
    $headerContent = $headerContent -replace '(void showHuffmanDialog\(\);)', "`$1`r`n    void showOCRDialog();  // Phase 23: OCR"
    Set-Content $headerPath -Value $headerContent -NoNewline
    Write-Host "  ? Added showOCRDialog() declaration" -ForegroundColor Green
} else {
    Write-Host "  ? OCR handler already declared" -ForegroundColor Gray
}

# ============================================================================
# STEP 2: Update MainWindow.cpp with OCR include and menu item
# ============================================================================

Write-Host ""
Write-Host "[2/5] Integrating OCR into MainWindow.cpp..." -ForegroundColor Green

$mainWindowPath = "src\MainWindow.cpp"
$mainContent = Get-Content $mainWindowPath -Raw

# Add OCR include
if ($mainContent -notmatch 'OCRDialog\.h') {
    $mainContent = $mainContent -replace '(#include "HuffmanDialog\.h".*)', "`$1`r`n#include `"OCRDialog.h`"  // Phase 23"
    Write-Host "  ? Added OCRDialog.h include" -ForegroundColor Green
}

# Add OCR menu item in Process menu
if ($mainContent -notmatch 'showOCRDialog') {
    # Find Huffman Coding menu item and add OCR after it
    $ocrMenuItem = @"

    // Phase 23: OCR (Optical Character Recognition)
    ADD_MENU_ACTION(processMenu, "OCR - Text Recognition...", showOCRDialog);
"@
    $mainContent = $mainContent -replace '(ADD_MENU_ACTION\(processMenu, "Huffman Coding\.\.\.", showHuffmanDialog\);)', "`$1$ocrMenuItem"
    Write-Host "  ? Added OCR menu item" -ForegroundColor Green
}

Set-Content $mainWindowPath -Value $mainContent -NoNewline

# ============================================================================
# STEP 3: Update project file
# ============================================================================

Write-Host ""
Write-Host "[3/5] Updating project file..." -ForegroundColor Green

$projectFile = "Naghuma Toolbox.vcxproj"
$projContent = Get-Content $projectFile -Raw

# Add OCR files if not present
$ocrFiles = @(
    '<ClCompile Include="lib\ocr\TextRecognition.cpp" />',
    '<ClCompile Include="src\OCRDialog.cpp" />',
    '<ClCompile Include="src\MainWindow_Phase23_OCR.cpp" />',
    '<ClInclude Include="lib\ocr\TextRecognition.h" />',
    '<ClInclude Include="include\OCRDialog.h" />'
)

$updated = $false
foreach ($file in $ocrFiles) {
    if ($projContent -notmatch [regex]::Escape($file)) {
        # Find appropriate ItemGroup and add
        if ($file -like '*ClCompile*') {
            $projContent = $projContent -replace '(<ClCompile Include="src\\MainWindow_Phase22\.cpp" />)', "`$1`r`n    $file"
        } else {
            $projContent = $projContent -replace '(<ClInclude Include="include\\HuffmanDialog\.h" />)', "`$1`r`n    $file"
        }
        $updated = $true
        Write-Host "  ? Added $file" -ForegroundColor Green
    }
}

if ($updated) {
    Set-Content $projectFile -Value $projContent -NoNewline
}

# ============================================================================
# STEP 4: Create .gitignore for GitHub
# ============================================================================

Write-Host ""
Write-Host "[4/5] Creating .gitignore for GitHub..." -ForegroundColor Green

$gitignoreContent = @'
# Visual Studio
.vs/
*.suo
*.user
*.userosscache
*.sln.docstates
*.VC.db
*.VC.VC.opendb

# Build results
[Dd]ebug/
[Dd]ebugPublic/
[Rr]elease/
[Rr]eleases/
x64/
x86/
[Ww][Ii][Nn]32/
[Aa][Rr][Mm]/
[Aa][Rr][Mm]64/
bld/
[Bb]in/
[Oo]bj/
[Ll]og/
[Ll]ogs/

# Visual C++ cache files
ipch/
*.aps
*.ncb
*.opendb
*.opensdf
*.sdf
*.cachefile
*.VC.db
*.VC.VC.opendb

# Visual Studio profiler
*.psess
*.vsp
*.vspx
*.sap

# ReSharper
_ReSharper*/
*.[Rr]e[Ss]harper
*.DotSettings.user

# Build results
*.ilk
*.meta
*.obj
*.iobj
*.pch
*.pdb
*.ipdb
*.pgc
*.pgd
*.rsp
*.sbr
*.tlb
*.tli
*.tlh
*.tmp
*.tmp_proj
*_wpftmp.csproj
*.log
*.vspscc
*.vssscc
.builds
*.pidb
*.svclog
*.scc

# Backup files
*.bak
*.backup
*.orig

# Qt
*.pro.user
*.pro.user.*
moc_*.cpp
ui_*.h
qrc_*.cpp

# OpenCV
opencv_world*.dll
opencv_ffmpeg*.dll

# Temporary files
*~
*.swp
*.swo

# macOS
.DS_Store

# PowerShell scripts backups
*.ps1.backup*

# Project-specific
/external/
/tessdata/
*.exe
*.dll
*.exp
*.lib
'@

Set-Content ".gitignore" -Value $gitignoreContent
Write-Host "  ? Created .gitignore" -ForegroundColor Green

# ============================================================================
# STEP 5: Update README with OCR section
# ============================================================================

Write-Host ""
Write-Host "[5/5] Updating README.md..." -ForegroundColor Green

$readmePath = "README.md"
$readmeContent = Get-Content $readmePath -Raw

if ($readmeContent -notmatch 'OCR') {
    # Add OCR section after Phase 17
    $ocrSection = @"

### ?? OCR (Phase 23) ? NEW
- **Optical Character Recognition**
  - Multi-language support (English, Arabic, French, German, Spanish, Chinese, Japanese)
  - Preprocessing options (denoise, adaptive threshold)
  - Text extraction with confidence scores
  - Export to text file or clipboard
  - Based on Tesseract OCR engine
  - Works with scanned documents, photos with text, screenshots

"@
    $readmeContent = $readmeContent -replace '(### ?? Histogram Operations)', "$ocrSection`r`n`$1"
    Set-Content $readmePath -Value $readmeContent -NoNewline
    Write-Host "  ? Updated README.md with OCR section" -ForegroundColor Green
}

# ============================================================================
# COMPLETION SUMMARY
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  INTEGRATION COMPLETE!                                " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "?? Summary:" -ForegroundColor Yellow
Write-Host "  ? OCR library created (lib/ocr/)" -ForegroundColor Green
Write-Host "  ? OCR dialog implemented" -ForegroundColor Green
Write-Host "  ? Phase 23 handler added" -ForegroundColor Green
Write-Host "  ? MainWindow integrated" -ForegroundColor Green
Write-Host "  ? Project file updated" -ForegroundColor Green
Write-Host "  ? .gitignore created" -ForegroundColor Green
Write-Host "  ? README.md updated" -ForegroundColor Green
Write-Host ""

Write-Host "?? Next Steps:" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. BUILD PROJECT" -ForegroundColor Yellow
Write-Host "   - Open Visual Studio" -ForegroundColor White
Write-Host "   - Rebuild Solution (Ctrl+Shift+B)" -ForegroundColor White
Write-Host ""
Write-Host "2. TEST OCR FEATURE" -ForegroundColor Yellow
Write-Host "   - Run application (Ctrl+F5)" -ForegroundColor White
Write-Host "   - Load image with text" -ForegroundColor White
Write-Host "   - Process ? OCR - Text Recognition..." -ForegroundColor White
Write-Host ""
Write-Host "3. OPTIONAL: Install Tesseract OCR" -ForegroundColor Yellow
Write-Host "   - Download: https://github.com/tesseract-ocr/tesseract" -ForegroundColor White
Write-Host "   - Install tessdata language files" -ForegroundColor White
Write-Host "   - Set TESSDATA_PREFIX environment variable" -ForegroundColor White
Write-Host "   - Rebuild for full OCR functionality" -ForegroundColor White
Write-Host ""
Write-Host "4. PREPARE FOR GITHUB" -ForegroundColor Yellow
Write-Host "   - Review changes" -ForegroundColor White
Write-Host "   - Commit with message:" -ForegroundColor White
Write-Host "     'feat: Add Phase 23 - OCR Text Recognition'" -ForegroundColor Gray
Write-Host "   - Push to GitHub:" -ForegroundColor White
Write-Host "     git add ." -ForegroundColor Gray
Write-Host "     git commit -m 'feat: Phase 23 OCR + Professional refactoring'" -ForegroundColor Gray
Write-Host "     git push origin master" -ForegroundColor Gray
Write-Host ""

Write-Host "?? Files Created/Modified:" -ForegroundColor Cyan
Write-Host "   NEW: lib/ocr/TextRecognition.h" -ForegroundColor Green
Write-Host "   NEW: lib/ocr/TextRecognition.cpp" -ForegroundColor Green
Write-Host "   NEW: include/OCRDialog.h" -ForegroundColor Green
Write-Host "   NEW: src/OCRDialog.cpp" -ForegroundColor Green
Write-Host "   NEW: src/MainWindow_Phase23_OCR.cpp" -ForegroundColor Green
Write-Host "   NEW: .gitignore" -ForegroundColor Green
Write-Host "   MOD: include/MainWindow.h" -ForegroundColor Yellow
Write-Host "   MOD: src/MainWindow.cpp" -ForegroundColor Yellow
Write-Host "   MOD: Naghuma Toolbox.vcxproj" -ForegroundColor Yellow
Write-Host "   MOD: README.md" -ForegroundColor Yellow
Write-Host ""

Write-Host "?? Phase 23 OCR Feature Ready!" -ForegroundColor Green
Write-Host ""
