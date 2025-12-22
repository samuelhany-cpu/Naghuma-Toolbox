# ============================================================================
# DEEP FIX FOR PROJECT FILE XML - Line 106 Error
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  FIX PROJECT XML - Line 106 Error  " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Backup
Write-Host "[1/4] Creating safety backup..." -ForegroundColor Yellow
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
Copy-Item $projectFile "$projectFile.backup_$timestamp" -Force
Write-Host "  Backup: $projectFile.backup_$timestamp" -ForegroundColor Green
Write-Host ""

# Read and diagnose
Write-Host "[2/4] Diagnosing exact problem at line 106..." -ForegroundColor Yellow
$lines = Get-Content $projectFile
Write-Host "  Lines around 106:" -ForegroundColor Cyan

for ($i = 103; $i -lt 109; $i++) {
    $lineNum = $i + 1
    $line = $lines[$i]
    $display = $line -replace '\s', '·'  # Show spaces as dots
    
    if ($lineNum -eq 106) {
        Write-Host "  >> Line $lineNum : [$display]" -ForegroundColor Red
        
        # Check what's wrong
        if ($line -match '^\s+$') {
            Write-Host "     ISSUE: Line contains only whitespace" -ForegroundColor Yellow
        }
        elseif ($line -notmatch '</?[A-Za-z]') {
            Write-Host "     ISSUE: Line contains text but no XML tags" -ForegroundColor Yellow
        }
    } else {
        Write-Host "     Line $lineNum : [$display]" -ForegroundColor Gray
    }
}
Write-Host ""

# Fix the content
Write-Host "[3/4] Applying comprehensive XML fix..." -ForegroundColor Yellow

$content = Get-Content $projectFile -Raw

# Remove ALL lines that are just whitespace between XML tags
$content = $content -replace '(?m)^[ \t]+$\r?\n', ''

# Remove text nodes between closing and opening tags
$content = $content -replace '>\s*\r?\n\s*\r?\n\s*<', ">`r`n    <"

# Clean up ItemGroup sections specifically
$content = $content -replace '(</ClCompile>)\s*\r?\n\s*\r?\n\s*(</ItemGroup>)', "`$1`r`n  `$2"
$content = $content -replace '(</ClInclude>)\s*\r?\n\s*\r?\n\s*(</ItemGroup>)', "`$1`r`n  `$2"

# Ensure ItemGroup tags are properly formatted
$content = $content -replace '(?m)^(\s*)<ItemGroup>\s*$', '  <ItemGroup>'
$content = $content -replace '(?m)^(\s*)</ItemGroup>\s*$', '  </ItemGroup>'

# Remove any remaining empty lines between ItemGroup close and open
$content = $content -replace '(</ItemGroup>)\s*\r?\n\s*\r?\n\s*(<ItemGroup>)', "`$1`r`n  `$2"

# Save
Set-Content $projectFile -Value $content -NoNewline

Write-Host "  XML structure cleaned" -ForegroundColor Green
Write-Host ""

# Verify
Write-Host "[4/4] Validating XML..." -ForegroundColor Yellow

try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  XML is now VALID!" -ForegroundColor Green
    Write-Host ""
    
    # Show what's at line 106 now
    $fixedLines = Get-Content $projectFile
    Write-Host "  Fixed line 106 content:" -ForegroundColor Cyan
    Write-Host "  >> $($fixedLines[105])" -ForegroundColor Green
    Write-Host ""
    
    # Count entries
    $compileCount = ($xml.Project.ItemGroup.ClCompile | Measure-Object).Count
    $includeCount = ($xml.Project.ItemGroup.ClInclude | Measure-Object).Count
    
    Write-Host "  Project statistics:" -ForegroundColor Cyan
    Write-Host "    ClCompile entries: $compileCount" -ForegroundColor White
    Write-Host "    ClInclude entries: $includeCount" -ForegroundColor White
    
    Write-Host ""
    Write-Host "=====================================" -ForegroundColor Green
    Write-Host "     PROJECT FILE FIXED!            " -ForegroundColor Green
    Write-Host "=====================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next: Open Visual Studio and reload project" -ForegroundColor Yellow
    Write-Host ""
    
} catch {
    Write-Host "  STILL HAS XML ERRORS!" -ForegroundColor Red
    Write-Host "  Error: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host ""
    Write-Host "Attempting nuclear option..." -ForegroundColor Yellow
    Write-Host ""
    
    # Nuclear option: rebuild ItemGroup sections from scratch
    & "$PSScriptRoot\rebuild_project_itemgroups.ps1"
}
