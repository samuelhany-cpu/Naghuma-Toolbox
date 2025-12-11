# Add AdjustmentDialog Files to Project

$vcxproj = "Naghuma Toolbox.vcxproj"
$content = Get-Content $vcxproj -Raw

# Add after TransformDialog.h
$content = $content -replace '(<ClInclude Include="include\\TransformDialog.h" />)', "`$1`r`n    <ClInclude Include=`"include\AdjustmentDialog.h`" />"

# Add after TransformDialog.cpp
$content = $content -replace '(<ClCompile Include="src\\TransformDialog.cpp" />)', "`$1`r`n    <ClCompile Include=`"src\AdjustmentDialog.cpp`" />"

# Add moc file after moc_TransformDialog.cpp
$content = $content -replace '(<ClCompile Include="src\\moc_TransformDialog.cpp" />)', "`$1`r`n    <ClCompile Include=`"src\moc_AdjustmentDialog.cpp`" />"

Set-Content $vcxproj -Value $content

Write-Host "Updated project file with AdjustmentDialog"
