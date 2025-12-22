# Add ROI files to project
$projectFile = "Naghuma Toolbox.vcxproj"

# Read the entire file
$content = Get-Content $projectFile -Raw

# Add compile files after CollapsibleToolbar
$roiCompiles = @"
    <ClCompile Include="src\ROIShape.cpp" />
    <ClCompile Include="src\ROIManager.cpp" />
    <ClCompile Include="src\moc_ROIManager.cpp" />
    <ClCompile Include="src\ROIDialog.cpp" />
    <ClCompile Include="src\moc_ROIDialog.cpp" />
"@

# Find the CollapsibleToolbar line and add after it
$content = $content -replace '(    <ClCompile Include="src\\moc_CollapsibleToolbar.cpp" />)', "`$1`r`n$roiCompiles"

# Add include files
$roiIncludes = @"
    <ClInclude Include="include\ROIShape.h" />
    <ClInclude Include="include\ROIManager.h" />
    <ClInclude Include="include\ROIDialog.h" />
"@

$content = $content -replace '(    <ClInclude Include="include\\CollapsibleToolbar.h" />)', "`$1`r`n$roiIncludes"

# Save the file
Set-Content $projectFile -Value $content -NoNewline

Write-Host "? Added ROI source files to project" -ForegroundColor Green
Write-Host "? Added ROI header files to project" -ForegroundColor Green
Write-Host "" 
Write-Host "Project file updated successfully!" -ForegroundColor Cyan
