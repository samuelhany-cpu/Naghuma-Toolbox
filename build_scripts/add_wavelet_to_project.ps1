# Add Wavelet files to Naghuma Toolbox project
Write-Host "`n=== Adding Wavelet Transform Files to Project ===" -ForegroundColor Cyan

$vcxprojPath = "Naghuma Toolbox.vcxproj"

# Backup
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
Copy-Item $vcxprojPath "$vcxprojPath.backup_$timestamp"
Write-Host "Backup created: $vcxprojPath.backup_$timestamp" -ForegroundColor Green

# Read XML
[xml]$xml = Get-Content $vcxprojPath

# Find ItemGroups
$compileGroup = $xml.Project.ItemGroup | Where-Object { $_.ClCompile } | Select-Object -First 1
$includeGroup = $xml.Project.ItemGroup | Where-Object { $_.ClInclude } | Select-Object -First 1

# Add WaveletTransform.cpp
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\WaveletTransform.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\WaveletTransform.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "Added: src\WaveletTransform.cpp" -ForegroundColor Green
}

# Add WaveletDialog.cpp
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\WaveletDialog.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\WaveletDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "Added: src\WaveletDialog.cpp" -ForegroundColor Green
}

# Add moc_WaveletDialog.cpp
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\moc_WaveletDialog.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\moc_WaveletDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "Added: src\moc_WaveletDialog.cpp" -ForegroundColor Green
}

# Add WaveletTransform.h
$exists = $includeGroup.ClInclude | Where-Object { $_.Include -eq "include\WaveletTransform.h" }
if (-not $exists) {
    $newInclude = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
    $newInclude.SetAttribute("Include", "include\WaveletTransform.h")
    $includeGroup.AppendChild($newInclude) | Out-Null
    Write-Host "Added: include\WaveletTransform.h" -ForegroundColor Green
}

# Add WaveletDialog.h
$exists = $includeGroup.ClInclude | Where-Object { $_.Include -eq "include\WaveletDialog.h" }
if (-not $exists) {
    $newInclude = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
    $newInclude.SetAttribute("Include", "include\WaveletDialog.h")
    $includeGroup.AppendChild($newInclude) | Out-Null
    Write-Host "Added: include\WaveletDialog.h" -ForegroundColor Green
}

# Save
$xml.Save((Resolve-Path $vcxprojPath).Path)
Write-Host "`nProject file updated successfully!" -ForegroundColor Green
