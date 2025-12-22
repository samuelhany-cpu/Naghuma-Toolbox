# ============================================================================
# COMPLETE PROJECT FILE REGENERATION
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  REGENERATING PROJECT FILE         " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Backup
Write-Host "Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile "$projectFile.before_regen" -Force
Write-Host "  Backup: $projectFile.before_regen" -ForegroundColor Green
Write-Host ""

# Read current file
$content = Get-Content $projectFile -Raw

# Split into parts
Write-Host "Extracting project sections..." -ForegroundColor Yellow

# Get everything before first ItemGroup
$beforeMatch = $content -match '(?s)(.*?)<ItemGroup>'
$beforeItemGroups = $matches[1]

# Get everything after last ItemGroup
$afterMatch = $content -match '(?s).*</ItemGroup>\s*(.*)$'
$afterItemGroups = $matches[1]

Write-Host "  Before ItemGroups: $($beforeItemGroups.Length) chars" -ForegroundColor Cyan
Write-Host "  After ItemGroups: $($afterItemGroups.Length) chars" -ForegroundColor Cyan
Write-Host ""

# Scan for CPP files
Write-Host "Scanning for source files..." -ForegroundColor Yellow
$cppFiles = @()
Get-ChildItem -Recurse -Filter "*.cpp" | Where-Object { 
    $_.FullName -notlike "*\x64\*" -and 
    $_.FullName -notlike "*\.vs\*" -and
    $_.FullName -notlike "*\Debug\*" -and
    $_.FullName -notlike "*\Release\*"
} | ForEach-Object {
    $relativePath = $_.FullName.Replace((Get-Location).Path + "\", "")
    $cppFiles += $relativePath
}

$cppFiles = $cppFiles | Sort-Object
Write-Host "  Found $($cppFiles.Count) .cpp files" -ForegroundColor Green

# Scan for H files
$hFiles = @()
Get-ChildItem -Path "include" -Recurse -Filter "*.h" | ForEach-Object {
    $relativePath = $_.FullName.Replace((Get-Location).Path + "\", "")
    $hFiles += $relativePath
}

Get-ChildItem -Path "lib" -Recurse -Filter "*.h" | ForEach-Object {
    $relativePath = $_.FullName.Replace((Get-Location).Path + "\", "")
    $hFiles += $relativePath
}

$hFiles = $hFiles | Sort-Object | Select-Object -Unique
Write-Host "  Found $($hFiles.Count) .h files" -ForegroundColor Green
Write-Host ""

# Build new ItemGroup sections with proper formatting
Write-Host "Building new ItemGroup sections..." -ForegroundColor Yellow

$newContent = $beforeItemGroups

# ClCompile ItemGroup
$newContent += "  <ItemGroup>`r`n"
foreach ($file in $cppFiles) {
    $newContent += "    <ClCompile Include=`"$file`" />`r`n"
}
$newContent += "  </ItemGroup>`r`n"

# ClInclude ItemGroup  
$newContent += "  <ItemGroup>`r`n"
foreach ($file in $hFiles) {
    $newContent += "    <ClInclude Include=`"$file`" />`r`n"
}
$newContent += "  </ItemGroup>`r`n"

$newContent += $afterItemGroups

# Write new file
Write-Host "Writing new project file..." -ForegroundColor Yellow
[System.IO.File]::WriteAllText($projectFile, $newContent, [System.Text.Encoding]::UTF8)

Write-Host "  Project file regenerated" -ForegroundColor Green
Write-Host ""

# Validate
Write-Host "Validating new file..." -ForegroundColor Yellow
try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  XML: VALID" -ForegroundColor Green
    
    $compileCount = ($xml.Project.ItemGroup.ClCompile | Measure-Object).Count
    $includeCount = ($xml.Project.ItemGroup.ClInclude | Measure-Object).Count
    
    Write-Host "  ClCompile entries: $compileCount" -ForegroundColor Cyan
    Write-Host "  ClInclude entries: $includeCount" -ForegroundColor Cyan
    
    # Verify Phase 22 files
    Write-Host ""
    Write-Host "Checking Phase 22 files..." -ForegroundColor Yellow
    $phase22Files = @(
        "src\HuffmanDialog.cpp",
        "src\moc_HuffmanDialog.cpp",
        "lib\compression\HuffmanCoding.cpp",
        "src\MainWindow_Phase22.cpp",
        "include\HuffmanDialog.h"
    )
    
    $allPresent = $true
    $projectText = Get-Content $projectFile -Raw
    foreach ($file in $phase22Files) {
        if ($projectText -match [regex]::Escape($file)) {
            Write-Host "  $file - PRESENT" -ForegroundColor Green
        } else {
            Write-Host "  $file - MISSING" -ForegroundColor Red
            $allPresent = $false
        }
    }
    
    if ($allPresent) {
        Write-Host ""
        Write-Host "=====================================" -ForegroundColor Green
        Write-Host "  REGENERATION SUCCESSFUL!          " -ForegroundColor Green
        Write-Host "=====================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Try opening in Visual Studio now." -ForegroundColor Yellow
    } else {
        Write-Host ""
        Write-Host "WARNING: Some Phase 22 files missing!" -ForegroundColor Red
    }
    
} catch {
    Write-Host "  XML VALIDATION FAILED: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host ""
    Write-Host "Restoring backup..." -ForegroundColor Yellow
    Copy-Item "$projectFile.before_regen" $projectFile -Force
    Write-Host "  Backup restored" -ForegroundColor Green
}

Write-Host ""
