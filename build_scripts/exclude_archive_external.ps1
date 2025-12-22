# ============================================================================
# EXCLUDE ARCHIVE AND EXTERNAL FOLDERS FROM BUILD
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  EXCLUDING NON-BUILD FOLDERS       " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Backup
Write-Host "Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile "$projectFile.before_exclude" -Force
Write-Host "  Backup created" -ForegroundColor Green
Write-Host ""

# Read project file
Write-Host "Removing archive and external files from project..." -ForegroundColor Yellow
$lines = Get-Content $projectFile

$newLines = @()
$excluded = 0

foreach ($line in $lines) {
    # Skip lines that reference archive or external folders
    if ($line -match 'Include="(archive|external)\\' -or 
        $line -match 'Include=".*\\(archive|external)\\') {
        $excluded++
        Write-Host "  Excluding: $($line.Trim())" -ForegroundColor Gray
    } else {
        $newLines += $line
    }
}

# Write updated project file
$newLines | Set-Content $projectFile

Write-Host ""
Write-Host "Excluded $excluded files from build" -ForegroundColor Green
Write-Host ""

# Validate XML
Write-Host "Validating XML..." -ForegroundColor Yellow
try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  XML: VALID" -ForegroundColor Green
    
    $compileCount = ($xml.Project.ItemGroup.ClCompile | Measure-Object).Count
    $includeCount = ($xml.Project.ItemGroup.ClInclude | Measure-Object).Count
    
    Write-Host "  ClCompile entries: $compileCount" -ForegroundColor Cyan
    Write-Host "  ClInclude entries: $includeCount" -ForegroundColor Cyan
    
} catch {
    Write-Host "  XML INVALID: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "  Restoring backup..." -ForegroundColor Yellow
    Copy-Item "$projectFile.before_exclude" $projectFile -Force
    Write-Host "  Backup restored" -ForegroundColor Green
    exit 1
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  ARCHIVE/EXTERNAL FILES EXCLUDED   " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
Write-Host "Reload the project in Visual Studio and build again." -ForegroundColor Yellow
Write-Host ""
