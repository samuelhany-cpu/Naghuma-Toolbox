# Add BrushTool Files to Project

$vcxproj = "Naghuma Toolbox.vcxproj"
$content = Get-Content $vcxproj -Raw

# Add after AdjustmentDialog.h
$content = $content -replace '(<ClInclude Include="include\\AdjustmentDialog.h" />)', "`$1`r`n    <ClInclude Include=`"include\BrushTool.h`" />`r`n    <ClInclude Include=`"include\BrushDialog.h`" />"

# Add after AdjustmentDialog.cpp
$content = $content -replace '(<ClCompile Include="src\\AdjustmentDialog.cpp" />)', "`$1`r`n    <ClCompile Include=`"src\BrushTool.cpp`" />`r`n    <ClCompile Include=`"src\BrushDialog.cpp`" />"

# Add moc files after moc_AdjustmentDialog.cpp
$content = $content -replace '(<ClCompile Include="src\\moc_AdjustmentDialog.cpp" />)', "`$1`r`n    <ClCompile Include=`"src\moc_BrushTool.cpp`" />`r`n    <ClCompile Include=`"src\moc_BrushDialog.cpp`" />"

Set-Content $vcxproj -Value $content

Write-Host "Updated project file with BrushTool files"
