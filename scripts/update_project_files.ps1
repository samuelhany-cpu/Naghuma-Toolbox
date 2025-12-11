# Script to add new modular files to Visual Studio project

$vcxprojPath = "Naghuma Toolbox.vcxproj"

Write-Host "Updating Visual Studio project file..." -ForegroundColor Cyan

# Read the project file
$content = Get-Content $vcxprojPath -Raw

# Check if files already exist
if ($content -match "RightSidebarWidget.h") {
    Write-Host "? New files already in project" -ForegroundColor Green
    exit 0
}

# Find the ClInclude section and add new headers
$includeInsert = @"
    <ClInclude Include="include\RightSidebarWidget.h" />
    <ClInclude Include="include\LayerManager.h" />
    <ClInclude Include="include\ImageProcessor.h" />
    <ClInclude Include="include\TransformDialog.h" />
"@

$content = $content -replace '(<ClInclude Include="include\\MainWindow.h" />)', "`$1`n$includeInsert"

# Find the ClCompile section and add new sources
$compileInsert = @"
    <ClCompile Include="src\RightSidebarWidget.cpp" />
    <ClCompile Include="src\LayerManager.cpp" />
    <ClCompile Include="src\ImageProcessor.cpp" />
    <ClCompile Include="src\TransformDialog.cpp" />
"@

$content = $content -replace '(<ClCompile Include="src\\MainWindow.cpp" />)', "`$1`n$compileInsert"

# Save the updated file
Set-Content -Path $vcxprojPath -Value $content

Write-Host "? Project file updated successfully!" -ForegroundColor Green
Write-Host "?? Added 4 header files and 4 source files" -ForegroundColor Yellow
