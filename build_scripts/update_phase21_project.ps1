# PowerShell script to update project file with Phase 21 MOC files
# Run this after closing Visual Studio

$projectFile = "Naghuma Toolbox.vcxproj"

# Read the project file
$content = Get-Content $projectFile -Raw

# Check if MOC files already exist
if ($content -match "moc_IntensityTransformDialog.cpp") {
    Write-Host "? MOC files already in project" -ForegroundColor Green
    exit 0
}

# Find the line with MainWindow_Phase21.cpp
$pattern = '(<ClCompile Include="src\\MainWindow_Phase21.cpp" />)'
$replacement = '<ClCompile Include="src\\moc_IntensityTransformDialog.cpp" />
    <ClCompile Include="src\\SharpeningDialog.cpp" />
    <ClCompile Include="src\\moc_SharpeningDialog.cpp" />
    <ClCompile Include="src\\MainWindow_Phase21.cpp" />'

# Replace
$newContent = $content -replace $pattern, $replacement

# Save
Set-Content -Path $projectFile -Value $newContent -NoNewline

Write-Host "? Project file updated with Phase 21 MOC files" -ForegroundColor Green
Write-Host "? Added: moc_IntensityTransformDialog.cpp" -ForegroundColor Cyan
Write-Host "? Added: moc_SharpeningDialog.cpp" -ForegroundColor Cyan
Write-Host ""
Write-Host "Now open the solution in Visual Studio and build." -ForegroundColor Yellow
