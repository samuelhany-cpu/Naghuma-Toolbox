# ============================================================================
# FIX DUPLICATE MOC SYMBOL ERRORS
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  FIXING DUPLICATE MOC SYMBOLS      " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Backup
Write-Host "Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile "$projectFile.before_moc_fix" -Force
Write-Host "  Backup created" -ForegroundColor Green
Write-Host ""

# The problem: MOC files should NOT be compiled as separate ClCompile entries
# They should be #included in their parent .cpp files instead

Write-Host "Analyzing MOC file compilation..." -ForegroundColor Yellow

$content = Get-Content $projectFile -Raw

# Find all moc_ files that are being compiled
$mocPattern = '<ClCompile Include="[^"]*moc_[^"]*\.cpp"'
$matches = [regex]::Matches($content, $mocPattern)

Write-Host "  Found $($matches.Count) MOC files being compiled separately" -ForegroundColor Yellow

if ($matches.Count -eq 0) {
    Write-Host ""
    Write-Host "No MOC files found to exclude. Problem might be elsewhere." -ForegroundColor Yellow
    Write-Host ""
    exit 0
}

Write-Host ""
Write-Host "Removing MOC files from ClCompile list..." -ForegroundColor Cyan

# Remove all moc_ .cpp files from ClCompile
$newContent = $content -replace '\s*<ClCompile Include="[^"]*\\moc_[^"]*\.cpp" />\r?\n', ''

# Save
Set-Content $projectFile -Value $newContent -NoNewline

Write-Host "  Removed $($matches.Count) MOC compilation entries" -ForegroundColor Green
Write-Host ""

# Validate
Write-Host "Validating XML..." -ForegroundColor Yellow
try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  XML: VALID" -ForegroundColor Green
    
    $compileCount = ($xml.Project.ItemGroup.ClCompile | Measure-Object).Count
    Write-Host "  ClCompile entries: $compileCount" -ForegroundColor Cyan
    
} catch {
    Write-Host "  XML INVALID: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "  Restoring backup..." -ForegroundColor Yellow
    Copy-Item "$projectFile.before_moc_fix" $projectFile -Force
    exit 1
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  MOC FILES EXCLUDED FROM BUILD     " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""

Write-Host "Explanation:" -ForegroundColor Yellow
Write-Host "  MOC files contain Qt meta-object code." -ForegroundColor White
Write-Host "  They should be #included at the end of their" -ForegroundColor White
Write-Host "  parent .cpp files, NOT compiled separately." -ForegroundColor White
Write-Host ""
Write-Host "  Example:" -ForegroundColor Cyan
Write-Host "    ColorConversionDialog.cpp should have:" -ForegroundColor White
Write-Host "    #include `"moc_ColorConversionDialog.cpp`"" -ForegroundColor Gray
Write-Host ""

Write-Host "Checking if includes are present..." -ForegroundColor Yellow

# Check a few files to see if they include their MOC
$filesToCheck = @(
    @{cpp = "src\ColorConversionDialog.cpp"; moc = "moc_ColorConversionDialog.cpp"},
    @{cpp = "src\ThresholdingDialog.cpp"; moc = "moc_ThresholdingDialog.cpp"}
)

$needsIncludes = @()

foreach ($file in $filesToCheck) {
    if (Test-Path $file.cpp) {
        $content = Get-Content $file.cpp -Raw
        if ($content -notmatch [regex]::Escape($file.moc)) {
            $needsIncludes += $file
            Write-Host "  MISSING: $($file.cpp) needs to include $($file.moc)" -ForegroundColor Red
        } else {
            Write-Host "  OK: $($file.cpp) includes $($file.moc)" -ForegroundColor Green
        }
    }
}

Write-Host ""

if ($needsIncludes.Count -gt 0) {
    Write-Host "WARNING: Some .cpp files are missing MOC includes!" -ForegroundColor Yellow
    Write-Host "Run add_moc_includes.ps1 to fix this automatically." -ForegroundColor Cyan
} else {
    Write-Host "All checked files have MOC includes." -ForegroundColor Green
}

Write-Host ""
Write-Host "Reload project in Visual Studio and rebuild." -ForegroundColor Yellow
Write-Host ""
