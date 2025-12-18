# Update vcxproj with ThresholdingDialog files
$vcxprojPath = "Naghuma Toolbox.vcxproj"

Write-Host "Loading project file..." -ForegroundColor Cyan
[xml]$xml = Get-Content $vcxprojPath

# Find ItemGroups
$compileGroup = $xml.Project.ItemGroup | Where-Object { $_.ClCompile } | Select-Object -First 1
$includeGroup = $xml.Project.ItemGroup | Where-Object { $_.ClInclude } | Select-Object -First 1

# Add ThresholdingDialog.cpp
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\ThresholdingDialog.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\ThresholdingDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  + Added src\ThresholdingDialog.cpp" -ForegroundColor Green
}

# Add moc_ThresholdingDialog.cpp
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\moc_ThresholdingDialog.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\moc_ThresholdingDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  + Added src\moc_ThresholdingDialog.cpp" -ForegroundColor Green
}

# Add ThresholdingDialog.h
$exists = $includeGroup.ClInclude | Where-Object { $_.Include -eq "include\ThresholdingDialog.h" }
if (-not $exists) {
    $newInclude = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
    $newInclude.SetAttribute("Include", "include\ThresholdingDialog.h")
    $includeGroup.AppendChild($newInclude) | Out-Null
    Write-Host "  + Added include\ThresholdingDialog.h" -ForegroundColor Green
}

# Save
$xml.Save((Resolve-Path $vcxprojPath).Path)
Write-Host "`n? Project updated successfully!" -ForegroundColor Cyan
