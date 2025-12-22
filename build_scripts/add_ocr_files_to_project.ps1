Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Adding OCR Files to Project" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"
$backupFile = "Naghuma Toolbox.vcxproj.backup_ocr"

# Backup the project file
Write-Host "Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile $backupFile -Force
Write-Host "  Backup created: $backupFile" -ForegroundColor Green

# Read the project file
Write-Host ""
Write-Host "Reading project file..." -ForegroundColor Yellow
$content = Get-Content $projectFile -Raw

# Check if OCR files are already added
if ($content -match "OCRDialog\.cpp" -and $content -match "TextRecognition\.cpp") {
    Write-Host ""
    Write-Host "OCR files are already in the project!" -ForegroundColor Green
    Write-Host ""
    exit 0
}

Write-Host "  Project file loaded" -ForegroundColor Green

# Find the ClCompile ItemGroup (where .cpp files are listed)
$clCompilePattern = '(?s)(<ItemGroup>\s*<ClCompile Include="[^"]*\.cpp"[^>]*/>.*?</ItemGroup>)'
if ($content -match $clCompilePattern) {
    $clCompileGroup = $Matches[1]
    Write-Host ""
    Write-Host "Found ClCompile ItemGroup" -ForegroundColor Green
    
    # Add OCR cpp files if not present
    $newCppFiles = @(
        '    <ClCompile Include="lib\ocr\TextRecognition.cpp" />',
        '    <ClCompile Include="src\OCRDialog.cpp" />',
        '    <ClCompile Include="src\moc_OCRDialog.cpp" />'
    )
    
    # Insert before the closing </ItemGroup>
    $updatedClCompileGroup = $clCompileGroup -replace '(\s*</ItemGroup>)', "`r`n$($newCppFiles -join "`r`n")`r`n`$1"
    $content = $content -replace [regex]::Escape($clCompileGroup), $updatedClCompileGroup
    
    Write-Host "  Added OCR .cpp files" -ForegroundColor Green
} else {
    Write-Host "  ERROR: Could not find ClCompile ItemGroup!" -ForegroundColor Red
    exit 1
}

# Find the ClInclude ItemGroup (where .h files are listed)
$clIncludePattern = '(?s)(<ItemGroup>\s*<ClInclude Include="[^"]*\.h"[^>]*/>.*?</ItemGroup>)'
if ($content -match $clIncludePattern) {
    $clIncludeGroup = $Matches[1]
    Write-Host "Found ClInclude ItemGroup" -ForegroundColor Green
    
    # Add OCR header files if not present
    $newHeaderFiles = @(
        '    <ClInclude Include="lib\ocr\TextRecognition.h" />',
        '    <ClInclude Include="include\OCRDialog.h" />'
    )
    
    # Insert before the closing </ItemGroup>
    $updatedClIncludeGroup = $clIncludeGroup -replace '(\s*</ItemGroup>)', "`r`n$($newHeaderFiles -join "`r`n")`r`n`$1"
    $content = $content -replace [regex]::Escape($clIncludeGroup), $updatedClIncludeGroup
    
    Write-Host "  Added OCR .h files" -ForegroundColor Green
} else {
    Write-Host "  ERROR: Could not find ClInclude ItemGroup!" -ForegroundColor Red
    exit 1
}

# Save the updated project file
Write-Host ""
Write-Host "Saving updated project file..." -ForegroundColor Yellow
$content | Set-Content $projectFile -NoNewline
Write-Host "  Project file updated successfully!" -ForegroundColor Green

# Verify the changes
Write-Host ""
Write-Host "Verifying changes..." -ForegroundColor Yellow
$verifyContent = Get-Content $projectFile -Raw

$filesToCheck = @(
    "lib\ocr\TextRecognition.cpp",
    "lib\ocr\TextRecognition.h",
    "src\OCRDialog.cpp",
    "src\moc_OCRDialog.cpp",
    "include\OCRDialog.h"
)

$allPresent = $true
foreach ($file in $filesToCheck) {
    if ($verifyContent -match [regex]::Escape($file)) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
        $allPresent = $false
    }
}

Write-Host ""
if ($allPresent) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  ALL OCR FILES ADDED SUCCESSFULLY!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "  1. Rebuild the project:" -ForegroundColor Cyan
    Write-Host "     msbuild `"Naghuma Toolbox.vcxproj`" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild" -ForegroundColor White
    Write-Host ""
    Write-Host "  2. Or run the complete OCR setup:" -ForegroundColor Cyan
    Write-Host "     .\build_ocr_complete.ps1" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  WARNING: Some files were not added!" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "You may need to add them manually in Visual Studio:" -ForegroundColor Yellow
    Write-Host "  1. Open Naghuma Toolbox.sln" -ForegroundColor Cyan
    Write-Host "  2. Right-click project -> Add -> Existing Item" -ForegroundColor Cyan
    Write-Host "  3. Add the missing files shown above" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Backup available at: $backupFile" -ForegroundColor Gray
    Write-Host ""
}
