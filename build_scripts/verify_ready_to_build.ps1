# ============================================================================
# FINAL VERIFICATION - Project is Ready
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  FINAL PROJECT VERIFICATION        " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# 1. Validate XML
Write-Host "[1/5] Validating XML structure..." -ForegroundColor Yellow
try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  XML: VALID" -ForegroundColor Green
} catch {
    Write-Host "  XML: INVALID - $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}
Write-Host ""

# 2. Check Phase 22 files
Write-Host "[2/5] Checking Phase 22 files..." -ForegroundColor Yellow

$phase22Files = @{
    "lib\compression\HuffmanCoding.h" = "Library header"
    "lib\compression\HuffmanCoding.cpp" = "Library implementation"
    "include\HuffmanDialog.h" = "Dialog header"
    "src\HuffmanDialog.cpp" = "Dialog implementation"
    "src\moc_HuffmanDialog.cpp" = "MOC file"
    "src\MainWindow_Phase22.cpp" = "Handler"
}

$allPresent = $true
foreach ($file in $phase22Files.Keys) {
    if (Test-Path $file) {
        $size = (Get-Item $file).Length
        Write-Host "  $file ($size bytes)" -ForegroundColor Green
    } else {
        Write-Host "  MISSING: $file" -ForegroundColor Red
        $allPresent = $false
    }
}

if (-not $allPresent) {
    Write-Host ""
    Write-Host "ERROR: Phase 22 files missing!" -ForegroundColor Red
    exit 1
}
Write-Host ""

# 3. Verify project file entries
Write-Host "[3/5] Verifying .vcxproj entries..." -ForegroundColor Yellow

$projectContent = Get-Content $projectFile -Raw
$requiredEntries = @(
    "HuffmanDialog.cpp",
    "moc_HuffmanDialog.cpp",
    "HuffmanCoding.cpp",
    "MainWindow_Phase22.cpp",
    "HuffmanDialog.h"
)

$allInProject = $true
foreach ($entry in $requiredEntries) {
    if ($projectContent -match [regex]::Escape($entry)) {
        Write-Host "  $entry in project" -ForegroundColor Green
    } else {
        Write-Host "  MISSING from project: $entry" -ForegroundColor Red
        $allInProject = $false
    }
}

if (-not $allInProject) {
    Write-Host ""
    Write-Host "ERROR: Not all Phase 22 files in project!" -ForegroundColor Red
    exit 1
}
Write-Host ""

# 4. Check MainWindow.cpp integration
Write-Host "[4/5] Checking MainWindow integration..." -ForegroundColor Yellow

$mainWindowContent = Get-Content "src\MainWindow.cpp" -Raw

if ($mainWindowContent -match '#include\s+"HuffmanDialog.h"') {
    Write-Host "  HuffmanDialog.h included" -ForegroundColor Green
} else {
    Write-Host "  MISSING: HuffmanDialog.h include" -ForegroundColor Red
    exit 1
}

if ($mainWindowContent -match 'showHuffmanDialog') {
    Write-Host "  Menu handler present" -ForegroundColor Green
} else {
    Write-Host "  MISSING: showHuffmanDialog menu item" -ForegroundColor Red
    exit 1
}
Write-Host ""

# 5. Check MainWindow.h declaration
Write-Host "[5/5] Checking MainWindow.h declaration..." -ForegroundColor Yellow

$mainWindowHeader = Get-Content "include\MainWindow.h" -Raw

if ($mainWindowHeader -match 'void\s+showHuffmanDialog') {
    Write-Host "  showHuffmanDialog declared" -ForegroundColor Green
} else {
    Write-Host "  MISSING: showHuffmanDialog declaration" -ForegroundColor Red
    exit 1
}
Write-Host ""

# Summary
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  ALL CHECKS PASSED!                " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""

Write-Host "Project Status:" -ForegroundColor Yellow
Write-Host "  XML: VALID" -ForegroundColor Green
Write-Host "  Phase 22 files: COMPLETE" -ForegroundColor Green
Write-Host "  Project entries: VERIFIED" -ForegroundColor Green
Write-Host "  Integration: COMPLETE" -ForegroundColor Green
Write-Host ""

Write-Host "READY TO BUILD!" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Open: Naghuma Toolbox.sln" -ForegroundColor White
Write-Host "  2. Reload project (if prompted)" -ForegroundColor White
Write-Host "  3. Build: Ctrl+Shift+B" -ForegroundColor White
Write-Host "  4. Run: Ctrl+F5" -ForegroundColor White
Write-Host "  5. Test: Process -> Huffman Coding..." -ForegroundColor White
Write-Host ""
