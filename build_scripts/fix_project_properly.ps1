# Fix Project Files Script
Write-Host "==================================" -ForegroundColor Cyan
Write-Host "Fixing Naghuma Toolbox Project Files" -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan

# 1. Fix vcxproj.filters file
Write-Host "`n[1/3] Fixing vcxproj.filters file..." -ForegroundColor Yellow
$filtersPath = "Naghuma Toolbox.vcxproj.filters"
[xml]$filtersXml = Get-Content $filtersPath

# Find the ItemGroup with ClCompile elements
$compileGroup = $filtersXml.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

# Check if ColorProcessor.cpp already exists
$existingColorProcessor = $compileGroup.ClCompile | Where-Object { $_.Include -eq "lib\color\ColorProcessor.cpp" }

if ($null -eq $existingColorProcessor) {
    # Create new ClCompile element for ColorProcessor.cpp
    $newCompile = $filtersXml.CreateElement("ClCompile", $filtersXml.DocumentElement.NamespaceURI)
    $newCompile.SetAttribute("Include", "lib\color\ColorProcessor.cpp")
    
    $filter = $filtersXml.CreateElement("Filter", $filtersXml.DocumentElement.NamespaceURI)
    $filter.InnerText = "Source Files"
    $newCompile.AppendChild($filter) | Out-Null
    
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  - Added ColorProcessor.cpp" -ForegroundColor Green
} else {
    Write-Host "  - ColorProcessor.cpp already exists" -ForegroundColor Yellow
}

$filtersXml.Save((Resolve-Path $filtersPath))
Write-Host "  - Saved vcxproj.filters" -ForegroundColor Green

# 2. Fix vcxproj file - Add ColorProcessor.cpp
Write-Host "`n[2/3] Adding ColorProcessor.cpp to vcxproj..." -ForegroundColor Yellow
$vcxprojPath = "Naghuma Toolbox.vcxproj"
[xml]$vcxprojXml = Get-Content $vcxprojPath

# Find ItemGroup with ClCompile elements
$compileGroup = $vcxprojXml.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

# Check if already exists
$existing = $compileGroup.ClCompile | Where-Object { $_.Include -eq "lib\color\ColorProcessor.cpp" }

if ($null -eq $existing) {
    $newCompile = $vcxprojXml.CreateElement("ClCompile", $vcxprojXml.DocumentElement.NamespaceURI)
    $newCompile.SetAttribute("Include", "lib\color\ColorProcessor.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  - Added ColorProcessor.cpp to compile list" -ForegroundColor Green
} else {
    Write-Host "  - ColorProcessor.cpp already in compile list" -ForegroundColor Yellow
}

# Find ItemGroup with ClInclude elements
$includeGroup = $vcxprojXml.Project.ItemGroup | Where-Object { $_.ClInclude -ne $null } | Select-Object -First 1

# Check if header already exists
$existingHeader = $includeGroup.ClInclude | Where-Object { $_.Include -eq "lib\color\ColorProcessor.h" }

if ($null -eq $existingHeader) {
    $newInclude = $vcxprojXml.CreateElement("ClInclude", $vcxprojXml.DocumentElement.NamespaceURI)
    $newInclude.SetAttribute("Include", "lib\color\ColorProcessor.h")
    $includeGroup.AppendChild($newInclude) | Out-Null
    Write-Host "  - Added ColorProcessor.h to header list" -ForegroundColor Green
} else {
    Write-Host "  - ColorProcessor.h already in header list" -ForegroundColor Yellow
}

$vcxprojXml.Save((Resolve-Path $vcxprojPath))
Write-Host "  - Saved vcxproj" -ForegroundColor Green

# 3. Summary
Write-Host "`n==================================" -ForegroundColor Cyan
Write-Host "Fix Complete!" -ForegroundColor Green
Write-Host "==================================" -ForegroundColor Cyan
Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "1. Reload the project in Visual Studio (if open)" -ForegroundColor White
Write-Host "2. Build the solution (F7)" -ForegroundColor White
Write-Host "3. Run the application (F5)" -ForegroundColor White
