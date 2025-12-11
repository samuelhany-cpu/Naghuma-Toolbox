# Script to update the Visual Studio project file with new library paths
# This script adds the new library files to the .vcxproj file

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "   Visual Studio Project File Updater  " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

$projectRoot = "F:\Naghuma Toolbox"
$vcxprojFile = Join-Path $projectRoot "Naghuma Toolbox.vcxproj"

if (-not (Test-Path $vcxprojFile)) {
    Write-Host "ERROR: Project file not found: $vcxprojFile" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit
}

Write-Host "Found project file: $vcxprojFile" -ForegroundColor Green
Write-Host ""

# Backup the original file
$backupFile = $vcxprojFile + ".backup"
Copy-Item -Path $vcxprojFile -Destination $backupFile -Force
Write-Host "Backup created: $backupFile" -ForegroundColor Yellow
Write-Host ""

# Read the project file
[xml]$xml = Get-Content $vcxprojFile

# Define namespaces
$ns = @{
    ns = "http://schemas.microsoft.com/developer/msbuild/2003"
}

Write-Host "Step 1: Adding header files..." -ForegroundColor Cyan

# Find or create ClInclude ItemGroup
$includeGroup = $xml.Project.ItemGroup | Where-Object { $_.ClInclude -ne $null } | Select-Object -First 1

if ($includeGroup -eq $null) {
    Write-Host "  Creating new ItemGroup for headers..." -ForegroundColor Gray
    $includeGroup = $xml.CreateElement("ItemGroup", $xml.Project.NamespaceURI)
    $xml.Project.AppendChild($includeGroup) | Out-Null
}

# Add new header files if they don't exist
$newHeaders = @(
    "lib\filters\ImageFilters.h",
    "lib\transforms\ImageTransforms.h",
    "lib\histogram\HistogramOperations.h"
)

foreach ($header in $newHeaders) {
    $exists = $includeGroup.ClInclude | Where-Object { $_.Include -eq $header }
    if ($exists -eq $null) {
        $newInclude = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
        $newInclude.SetAttribute("Include", $header)
        $includeGroup.AppendChild($newInclude) | Out-Null
        Write-Host "  [+] Added: $header" -ForegroundColor Green
    } else {
        Write-Host "  [~] Already exists: $header" -ForegroundColor DarkGray
    }
}

Write-Host ""
Write-Host "Step 2: Adding source files..." -ForegroundColor Cyan

# Find or create ClCompile ItemGroup
$compileGroup = $xml.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

if ($compileGroup -eq $null) {
    Write-Host "  Creating new ItemGroup for sources..." -ForegroundColor Gray
    $compileGroup = $xml.CreateElement("ItemGroup", $xml.Project.NamespaceURI)
    $xml.Project.AppendChild($compileGroup) | Out-Null
}

# Add new source files if they don't exist
$newSources = @(
    "lib\filters\ImageFilters.cpp",
    "lib\transforms\ImageTransforms.cpp",
    "lib\histogram\HistogramOperations.cpp"
)

foreach ($source in $newSources) {
    $exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq $source }
    if ($exists -eq $null) {
        $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
        $newCompile.SetAttribute("Include", $source)
        $compileGroup.AppendChild($newCompile) | Out-Null
        Write-Host "  [+] Added: $source" -ForegroundColor Green
    } else {
        Write-Host "  [~] Already exists: $source" -ForegroundColor DarkGray
    }
}

Write-Host ""
Write-Host "Step 3: Saving updated project file..." -ForegroundColor Cyan

# Save the updated XML
$xml.Save($vcxprojFile)

Write-Host "  [OK] Project file updated successfully!" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "             Update Complete!           " -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  - Original file backed up to: $backupFile" -ForegroundColor White
Write-Host "  - Added 3 header files to project" -ForegroundColor White
Write-Host "  - Added 3 source files to project" -ForegroundColor White
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Cyan
Write-Host "  1. Open the solution in Visual Studio" -ForegroundColor White
Write-Host "  2. Reload the project if prompted" -ForegroundColor White
Write-Host "  3. Build the solution (Ctrl+Shift+B)" -ForegroundColor White
Write-Host "  4. Test the new Filters menu" -ForegroundColor White
Write-Host ""
Write-Host "If there are any issues, restore from backup:" -ForegroundColor Yellow
Write-Host "  Copy-Item '$backupFile' '$vcxprojFile' -Force" -ForegroundColor Gray
Write-Host ""

Read-Host "Press Enter to exit"
