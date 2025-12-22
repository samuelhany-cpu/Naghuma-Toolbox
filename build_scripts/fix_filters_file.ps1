Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Fix MOC in Filters File" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$filtersFile = "Naghuma Toolbox.vcxproj.filters"
$backupFile = "Naghuma Toolbox.vcxproj.filters.backup_ocr"

# Backup
Write-Host "Creating backup..." -ForegroundColor Yellow
Copy-Item $filtersFile $backupFile -Force
Write-Host "  Backup: $backupFile" -ForegroundColor Green

# Read filters file
Write-Host ""
Write-Host "Reading filters file..." -ForegroundColor Yellow
$content = Get-Content $filtersFile -Raw

# Check if MOC is already there
if ($content -match 'moc_OCRDialog\.cpp') {
    Write-Host "  moc_OCRDialog.cpp already in filters file" -ForegroundColor Green
} else {
    Write-Host "  Adding moc_OCRDialog.cpp to Source Files filter..." -ForegroundColor Cyan
    
    # Find the Source Files ItemGroup for ClCompile
    $sourcePattern = '(?s)(<ItemGroup>\s*<ClCompile Include="src\\[^"]+\.cpp"[^>]*>\s*<Filter>Source Files</Filter>\s*</ClCompile>)'
    
    if ($content -match $sourcePattern) {
        $firstSourceEntry = $Matches[1]
        $newMocEntry = @"
$firstSourceEntry
    <ClCompile Include="src\moc_OCRDialog.cpp">
      <Filter>Source Files</Filter>
    </ClCompile>
"@
        $content = $content -replace [regex]::Escape($firstSourceEntry), $newMocEntry
        Write-Host "  [OK] Added to Source Files filter" -ForegroundColor Green
    } else {
        Write-Host "  [WARNING] Could not find Source Files pattern" -ForegroundColor Yellow
    }
}

# Add OCR library files to filters
Write-Host ""
Write-Host "Adding OCR library files..." -ForegroundColor Yellow

if ($content -notmatch 'TextRecognition\.cpp') {
    # Add OCR library filter if it doesn't exist
    if ($content -notmatch '<Filter Include="lib\\ocr">') {
        $filterPattern = '(<ItemGroup>\s*<Filter Include="[^"]+">)'
        if ($content -match $filterPattern) {
            $firstFilter = $Matches[1]
            $newFilter = @"
$firstFilter
    <Filter Include="lib\ocr">
      <UniqueIdentifier>{$(New-Guid)}</UniqueIdentifier>
    </Filter>
"@
            $content = $content -replace [regex]::Escape($firstFilter), $newFilter
            Write-Host "  [OK] Created lib\ocr filter" -ForegroundColor Green
        }
    }
    
    # Add TextRecognition files
    $sourcePattern = '(<ItemGroup>\s*<ClCompile[^>]*>)'
    if ($content -match $sourcePattern) {
        $itemGroupStart = $Matches[1]
        $newOcrEntry = @"
$itemGroupStart
    <ClCompile Include="lib\ocr\TextRecognition.cpp">
      <Filter>lib\ocr</Filter>
    </ClCompile>
"@
        $content = $content -replace [regex]::Escape($itemGroupStart), $newOcrEntry
        Write-Host "  [OK] Added TextRecognition.cpp" -ForegroundColor Green
    }
}

# Add OCRDialog.cpp if not there
if ($content -notmatch 'OCRDialog\.cpp' -or !($content -match 'src\\OCRDialog\.cpp')) {
    $sourcePattern = '(<ClCompile Include="src\\[^"]+\.cpp"[^>]*>\s*<Filter>Source Files</Filter>\s*</ClCompile>)'
    if ($content -match $sourcePattern) {
        $firstEntry = $Matches[1]
        $newEntry = @"
$firstEntry
    <ClCompile Include="src\OCRDialog.cpp">
      <Filter>Source Files</Filter>
    </ClCompile>
"@
        $content = $content -replace [regex]::Escape($firstEntry), $newEntry
        Write-Host "  [OK] Added OCRDialog.cpp" -ForegroundColor Green
    }
}

# Add header files
if ($content -notmatch 'TextRecognition\.h') {
    $headerPattern = '(<ItemGroup>\s*<ClInclude[^>]*>)'
    if ($content -match $headerPattern) {
        $headerStart = $Matches[1]
        $newHeaders = @"
$headerStart
    <ClInclude Include="lib\ocr\TextRecognition.h">
      <Filter>lib\ocr</Filter>
    </ClInclude>
    <ClInclude Include="include\OCRDialog.h">
      <Filter>Header Files</Filter>
    </ClInclude>
"@
        $content = $content -replace [regex]::Escape($headerStart), $newHeaders
        Write-Host "  [OK] Added header files" -ForegroundColor Green
    }
}

# Save
Write-Host ""
Write-Host "Saving filters file..." -ForegroundColor Yellow
$content | Set-Content $filtersFile -NoNewline
Write-Host "  [OK] Filters file updated" -ForegroundColor Green

# Verify
Write-Host ""
Write-Host "Verifying changes..." -ForegroundColor Yellow
$verify = Get-Content $filtersFile -Raw

$filesToCheck = @(
    "moc_OCRDialog.cpp",
    "OCRDialog.cpp",
    "TextRecognition.cpp",
    "TextRecognition.h",
    "OCRDialog.h"
)

$allFound = $true
foreach ($file in $filesToCheck) {
    if ($verify -match [regex]::Escape($file)) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
        $allFound = $false
    }
}

Write-Host ""
if ($allFound) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  FILTERS FILE UPDATED SUCCESSFULLY!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "  1. Reload project in Visual Studio (if open)" -ForegroundColor Cyan
    Write-Host "  2. Run: .\fix_ocr_build_complete.ps1" -ForegroundColor Cyan
    Write-Host ""
} else {
    Write-Host "Some files were not added correctly." -ForegroundColor Yellow
    Write-Host "Backup available at: $backupFile" -ForegroundColor Gray
    Write-Host ""
}
