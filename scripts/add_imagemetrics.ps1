# Add ImageMetrics Files to Project

$vcxproj = "Naghuma Toolbox.vcxproj"
$content = Get-Content $vcxproj -Raw

# Add after ImageProcessor.h
$content = $content -replace '(<ClInclude Include="include\\ImageProcessor.h" />)', "`$1`r`n    <ClInclude Include=`"include\ImageMetrics.h`" />"

# Add after ImageProcessor.cpp
$content = $content -replace '(<ClCompile Include="src\\ImageProcessor.cpp" />)', "`$1`r`n    <ClCompile Include=`"src\ImageMetrics.cpp`" />"

Set-Content $vcxproj -Value $content

Write-Host "Updated project file"
