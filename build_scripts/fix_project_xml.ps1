# ============================================================================
# FIX PROJECT FILE XML ERRORS
# ============================================================================

Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   FIXING PROJECT FILE XML ERRORS      ?" -ForegroundColor Cyan
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Backup
Write-Host "[1/3] Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile "$projectFile.backup_xmlfix" -Force
Write-Host "  ? Backup: $projectFile.backup_xmlfix" -ForegroundColor Green
Write-Host ""

# Read content
Write-Host "[2/3] Analyzing and fixing XML..." -ForegroundColor Yellow
$content = Get-Content $projectFile -Raw

# Remove any stray text nodes between tags
$content = $content -replace '(?s)(</(ClCompile|ClInclude)>)\s*([^\s<][^<]*?)\s*(<(ClCompile|ClInclude|/ItemGroup))', '$1`n    $4'

# Ensure proper ItemGroup closure
$content = $content -replace '(?s)(</ClCompile>)\s+\n\s+\n\s+(</ItemGroup>)', '$1`n  $2'

# Remove duplicate whitespace
$content = $content -replace '(?m)^\s+$', ''

# Ensure consistent indentation for ItemGroup
$content = $content -replace '(?m)^  </ItemGroup>', '  </ItemGroup>'
$content = $content -replace '(?m)^  <ItemGroup>', '  <ItemGroup>'

# Save fixed content
Set-Content $projectFile -Value $content -NoNewline

Write-Host "  ? XML structure cleaned" -ForegroundColor Green
Write-Host ""

# Verify
Write-Host "[3/3] Verifying XML validity..." -ForegroundColor Yellow

try {
    $xml = [xml](Get-Content $projectFile -Raw)
    Write-Host "  ? XML is valid!" -ForegroundColor Green
    
    # Count files
    $clCompile = $xml.Project.ItemGroup.ClCompile.Count
    $clInclude = $xml.Project.ItemGroup.ClInclude.Count
    
    Write-Host "  • ClCompile entries: $clCompile" -ForegroundColor Cyan
    Write-Host "  • ClInclude entries: $clInclude" -ForegroundColor Cyan
    
    # Check for Phase 22 files
    $hasHuffman = $xml.Project.ItemGroup.ClCompile.Include -contains "src\HuffmanDialog.cpp"
    if ($hasHuffman) {
        Write-Host "  • Phase 22 files present: ?" -ForegroundColor Cyan
    } else {
        Write-Host "  • Phase 22 files present: ?" -ForegroundColor Red
    }
    
} catch {
    Write-Host "  ? XML still has errors: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host ""
    Write-Host "Restoring backup..." -ForegroundColor Yellow
    Copy-Item "$projectFile.backup_xmlfix" $projectFile -Force
    Write-Host "  ? Restored" -ForegroundColor Green
    exit 1
}

Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Green
Write-Host "?         ? PROJECT FILE FIXED!        ?" -ForegroundColor Green
Write-Host "??????????????????????????????????????????" -ForegroundColor Green
Write-Host ""
Write-Host "Next: Open Visual Studio and reload the project" -ForegroundColor Yellow
Write-Host ""
