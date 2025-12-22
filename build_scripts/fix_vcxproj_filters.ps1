# Fix vcxproj.filters file - Remove malformed XML entries
Write-Host "Fixing Naghuma Toolbox.vcxproj.filters..." -ForegroundColor Cyan

$filtersPath = "Naghuma Toolbox.vcxproj.filters"

# Read the entire file
$content = Get-Content $filtersPath -Raw

# Find the problematic section and replace it
$oldPattern = @'
    <ClCompile Include="lib\color\ColorSpace.cpp">
		<ClCompile Include="lib\color\ColorProcessor.cpp" />
      <Filter>Source Files</Filter>
    </ClCompile>
	  <ClCompile Include="lib\color\ColorProcessor.cpp">
		  <Filter>Source Files</Filter>
	  </ClCompile>
'@

$newPattern = @'
    <ClCompile Include="lib\color\ColorSpace.cpp">
      <Filter>Source Files</Filter>
    </ClCompile>
    <ClCompile Include="lib\color\ColorProcessor.cpp">
      <Filter>Source Files</Filter>
    </ClCompile>
'@

# Replace the malformed section
$content = $content.Replace($oldPattern, $newPattern)

# Write back to file
$content | Set-Content $filtersPath -NoNewline -Encoding UTF8

Write-Host "Fixed vcxproj.filters file successfully!" -ForegroundColor Green

# Now fix the main vcxproj file to add ColorProcessor.cpp
Write-Host "`nAdding ColorProcessor files to Naghuma Toolbox.vcxproj..." -ForegroundColor Cyan

$vcxprojPath = "Naghuma Toolbox.vcxproj"
$vcxprojContent = Get-Content $vcxprojPath -Raw

# Add ColorProcessor.cpp if not already present
if ($vcxprojContent -notmatch 'ColorProcessor\.cpp') {
    $vcxprojContent = $vcxprojContent -replace '(<ClCompile Include="lib\\color\\ColorSpace\.cpp" />)', 
        '$1' + "`r`n" + '    <ClCompile Include="lib\color\ColorProcessor.cpp" />'
    
    Write-Host "Added ColorProcessor.cpp to compile list" -ForegroundColor Green
} else {
    Write-Host "ColorProcessor.cpp already in compile list" -ForegroundColor Yellow
}

# Add ColorProcessor.h if not already present
if ($vcxprojContent -notmatch 'ColorProcessor\.h') {
    $vcxprojContent = $vcxprojContent -replace '(<ClInclude Include="lib\\color\\ColorSpace\.h" />)', 
        '$1' + "`r`n" + '    <ClInclude Include="lib\color\ColorProcessor.h" />'
    
    Write-Host "Added ColorProcessor.h to header list" -ForegroundColor Green
} else {
    Write-Host "ColorProcessor.h already in header list" -ForegroundColor Yellow
}

# Write back to vcxproj file
$vcxprojContent | Set-Content $vcxprojPath -NoNewline -Encoding UTF8

Write-Host "`nAll fixes applied successfully!" -ForegroundColor Green
Write-Host "You can now reload the project in Visual Studio and build." -ForegroundColor Cyan
