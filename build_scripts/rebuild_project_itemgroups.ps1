# ============================================================================
# NUCLEAR OPTION: Rebuild ItemGroup sections from scratch
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Red
Write-Host "  REBUILDING PROJECT ITEMGROUPS     " -ForegroundColor Red
Write-Host "=====================================" -ForegroundColor Red
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Backup
Copy-Item $projectFile "$projectFile.before_rebuild" -Force
Write-Host "Backup created: $projectFile.before_rebuild" -ForegroundColor Yellow
Write-Host ""

# Read existing content
$content = Get-Content $projectFile -Raw

# Extract everything BEFORE first ItemGroup
$beforeItemGroups = $content -replace '(?s)(<ItemGroup>.*)', ''

# Extract everything AFTER last ItemGroup
$afterItemGroups = $content -replace '(?s)(.*</ItemGroup>\s*)', ''

Write-Host "Scanning source files..." -ForegroundColor Cyan

# Scan for all .cpp files
$cppFiles = Get-ChildItem -Recurse -Filter "*.cpp" | 
    Where-Object { $_.FullName -notlike "*\x64\*" -and $_.FullName -notlike "*\.vs\*" } |
    ForEach-Object { $_.FullName.Replace((Get-Location).Path + "\", "").Replace("/", "\") } |
    Sort-Object

Write-Host "  Found $($cppFiles.Count) .cpp files" -ForegroundColor Green

# Scan for all .h files in include/
$hFiles = Get-ChildItem -Path "include" -Filter "*.h" -Recurse |
    ForEach-Object { $_.FullName.Replace((Get-Location).Path + "\", "").Replace("/", "\") } |
    Sort-Object

Write-Host "  Found $($hFiles.Count) .h files" -ForegroundColor Green
Write-Host ""

# Build ClCompile ItemGroup
Write-Host "Building ClCompile ItemGroup..." -ForegroundColor Cyan
$clCompileSection = "  <ItemGroup>`r`n"
foreach ($file in $cppFiles) {
    $clCompileSection += "    <ClCompile Include=`"$file`" />`r`n"
}
$clCompileSection += "  </ItemGroup>`r`n"

# Build ClInclude ItemGroup
Write-Host "Building ClInclude ItemGroup..." -ForegroundColor Cyan
$clIncludeSection = "  <ItemGroup>`r`n"
foreach ($file in $hFiles) {
    $clIncludeSection += "    <ClInclude Include=`"$file`" />`r`n"
}
$clIncludeSection += "  </ItemGroup>`r`n"

# Combine
$newContent = $beforeItemGroups + $clCompileSection + $clIncludeSection + $afterItemGroups

# Save
Set-Content $projectFile -Value $newContent -NoNewline

Write-Host "Project file rebuilt!" -ForegroundColor Green
Write-Host ""

# Validate
Write-Host "Validating XML..." -ForegroundColor Yellow
try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  XML is VALID!" -ForegroundColor Green
    Write-Host ""
    Write-Host "=====================================" -ForegroundColor Green
    Write-Host "  REBUILD SUCCESSFUL!               " -ForegroundColor Green
    Write-Host "=====================================" -ForegroundColor Green
} catch {
    Write-Host "  XML STILL INVALID: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host ""
    Write-Host "Restoring backup..." -ForegroundColor Yellow
    Copy-Item "$projectFile.before_rebuild" $projectFile -Force
    Write-Host "Backup restored. Manual fix required." -ForegroundColor Red
}
