# Add CropTool to project file
$proj = "Naghuma Toolbox.vcxproj"
$lines = [System.Collections.ArrayList](Get-Content $proj)

# Find where to insert
for ($i = 0; $i -lt $lines.Count; $i++) {
    # Add after ImageProcessor.cpp
    if ($lines[$i] -like '*ImageProcessor.cpp*' -and $lines[$i] -like '*ClCompile*') {
        $lines.Insert($i + 1, '    <ClCompile Include="src\CropTool.cpp" />')
        $lines.Insert($i + 2, '    <ClCompile Include="src\moc_CropTool.cpp" />')
        Write-Host "Added CropTool.cpp at line $($i+1)"
        $i += 2
    }
    # Add after ImageProcessor.h
    if ($lines[$i] -like '*ImageProcessor.h*' -and $lines[$i] -like '*ClInclude*') {
        $lines.Insert($i + 1, '    <ClInclude Include="include\CropTool.h" />')
        Write-Host "Added CropTool.h at line $($i+1)"
        $i++
    }
}

$lines | Set-Content $proj
Write-Host "`n? CropTool added to project file" -ForegroundColor Green
