# ============================================================================
# PHASE 22: HUFFMAN CODING - COMPLETE INTEGRATION
# ============================================================================

Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   PHASE 22: HUFFMAN CODING INTEGRATION?" -ForegroundColor Cyan
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$ErrorActionPreference = "Continue"
$qtPath = "C:\Qt\6.7.3\msvc2019_64\bin"

# ============================================================================
# STEP 1: GENERATE MOC FILE
# ============================================================================
Write-Host "[1/4] Generating MOC file for HuffmanDialog..." -ForegroundColor Yellow

if (Test-Path "$qtPath\moc.exe") {
    & "$qtPath\moc.exe" "include\HuffmanDialog.h" -o "src\moc_HuffmanDialog.cpp"
    
    if (Test-Path "src\moc_HuffmanDialog.cpp") {
        $size = (Get-Item "src\moc_HuffmanDialog.cpp").Length
        Write-Host "  ? MOC file generated: $size bytes" -ForegroundColor Green
    } else {
        Write-Host "  ? MOC generation failed!" -ForegroundColor Red
        exit 1
    }
} else {
    Write-Host "  ? MOC.exe not found at $qtPath" -ForegroundColor Red
    Write-Host "  Please update qtPath variable in script" -ForegroundColor Yellow
    exit 1
}

Write-Host ""

# ============================================================================
# STEP 2: UPDATE PROJECT FILE
# ============================================================================
Write-Host "[2/4] Updating project file..." -ForegroundColor Yellow

$projectFile = "Naghuma Toolbox.vcxproj"
$content = Get-Content $projectFile -Raw

# Check if Phase 22 files already exist
if ($content -match 'HuffmanDialog') {
    Write-Host "  ? Phase 22 files already in project" -ForegroundColor Yellow
} else {
    # Find the last ClInclude entry and add Phase 22 headers after it
    $content = $content -replace '(<ClInclude Include="include\\WaveletDialog.h" />)', ('$1' + "`n" + '    <ClInclude Include="include\HuffmanDialog.h" />')
    
    # Find the last ClCompile entry and add Phase 22 implementation files
    $insertPoint = '<ClCompile Include="src\moc_SharpeningDialog.cpp" />'
    $newEntries = @"
    <ClCompile Include="src\HuffmanDialog.cpp" />
    <ClCompile Include="src\moc_HuffmanDialog.cpp" />
    <ClCompile Include="lib\compression\HuffmanCoding.cpp" />
    <ClCompile Include="src\MainWindow_Phase22.cpp" />
"@
    
    $content = $content -replace "($insertPoint)", ('$1' + "`n" + $newEntries)
    
    # Save updated project file
    Set-Content $projectFile -Value $content -NoNewline
    
    Write-Host "  ? Project file updated with Phase 22 files" -ForegroundColor Green
}

Write-Host ""

# ============================================================================
# STEP 3: ADD MENU ITEM TO MAINWINDOW.CPP
# ============================================================================
Write-Host "[3/4] Adding Huffman menu item..." -ForegroundColor Yellow

$mainWindowFile = "src\MainWindow.cpp"
$mainWindowContent = Get-Content $mainWindowFile -Raw

# Check if menu item already exists
if ($mainWindowContent -match 'Huffman Coding') {
    Write-Host "  ? Huffman menu item already exists" -ForegroundColor Yellow
} else {
    # Find the compression menu section and add Huffman item
    $searchPattern = '(ADD_MENU_ACTION\(processMenu, "Compress Image\.\.\.", applyCompression\);)'
    $replacement = @'
$1

    processMenu->addSeparator();

    // Phase 22: Huffman Coding
    ADD_MENU_ACTION(processMenu, "Huffman Coding...", showHuffmanDialog);
'@
    
    $mainWindowContent = $mainWindowContent -replace $searchPattern, $replacement
    
    # Also need to add the include at the top
    $includePattern = '(#include "SharpeningDialog\.h"  // Phase 21)'
    $includeReplacement = @'
$1
#include "HuffmanDialog.h"  // Phase 22
'@
    
    $mainWindowContent = $mainWindowContent -replace $includePattern, $includeReplacement
    
    # Save updated file
    Set-Content $mainWindowFile -Value $mainWindowContent -NoNewline
    
    Write-Host "  ? Huffman menu item added to Process menu" -ForegroundColor Green
    Write-Host "  ? HuffmanDialog.h include added" -ForegroundColor Green
}

Write-Host ""

# ============================================================================
# STEP 4: VERIFICATION
# ============================================================================
Write-Host "[4/4] Verifying integration..." -ForegroundColor Yellow

$allGood = $true

# Check files exist
$criticalFiles = @(
    "lib\compression\HuffmanCoding.h",
    "lib\compression\HuffmanCoding.cpp",
    "include\HuffmanDialog.h",
    "src\HuffmanDialog.cpp",
    "src\moc_HuffmanDialog.cpp",
    "src\MainWindow_Phase22.cpp"
)

foreach ($file in $criticalFiles) {
    if (Test-Path $file) {
        $size = (Get-Item $file).Length
        Write-Host "  ? $file ($size bytes)" -ForegroundColor Green
    } else {
        Write-Host "  ? $file MISSING" -ForegroundColor Red
        $allGood = $false
    }
}

# Check project file
$projectContent = Get-Content $projectFile -Raw
if ($projectContent -match 'moc_HuffmanDialog.cpp') {
    Write-Host "  ? Project file includes Phase 22 MOC" -ForegroundColor Green
} else {
    Write-Host "  ? Project file missing MOC entry" -ForegroundColor Red
    $allGood = $false
}

# Check MainWindow includes
$mainWindowCheck = Get-Content $mainWindowFile -Raw
if ($mainWindowCheck -match 'HuffmanDialog.h') {
    Write-Host "  ? MainWindow.cpp includes HuffmanDialog.h" -ForegroundColor Green
} else {
    Write-Host "  ? MainWindow.cpp missing HuffmanDialog.h" -ForegroundColor Red
    $allGood = $false
}

# Check menu item
if ($mainWindowCheck -match 'showHuffmanDialog') {
    Write-Host "  ? Huffman menu item added" -ForegroundColor Green
} else {
    Write-Host "  ? Menu item not found" -ForegroundColor Red
    $allGood = $false
}

Write-Host ""

if ($allGood) {
    Write-Host "??????????????????????????????????????????" -ForegroundColor Green
    Write-Host "?   ? PHASE 22 INTEGRATION COMPLETE    ?" -ForegroundColor Green
    Write-Host "??????????????????????????????????????????" -ForegroundColor Green
    Write-Host ""
    Write-Host "?? Summary:" -ForegroundColor Yellow
    Write-Host "  • MOC file generated" -ForegroundColor Cyan
    Write-Host "  • Project file updated" -ForegroundColor Cyan
    Write-Host "  • Menu item added: Process ? Huffman Coding..." -ForegroundColor Cyan
    Write-Host "  • All includes resolved" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "?? Next: Open Visual Studio and build!" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Test Path: Process ? Huffman Coding..." -ForegroundColor Yellow
    Write-Host "  1. Load a grayscale or color image" -ForegroundColor White
    Write-Host "  2. Click 'Encode Image'" -ForegroundColor White
    Write-Host "  3. View compression metrics" -ForegroundColor White
    Write-Host "  4. Click 'Decode Image' to verify lossless" -ForegroundColor White
    Write-Host "  5. Click 'Apply & Close' to add layer" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "??????????????????????????????????????????" -ForegroundColor Red
    Write-Host "?      ? INTEGRATION INCOMPLETE        ?" -ForegroundColor Red
    Write-Host "??????????????????????????????????????????" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please check the errors above" -ForegroundColor Yellow
    exit 1
}
