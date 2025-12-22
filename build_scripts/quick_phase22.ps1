# Quick Phase 22 Integration
Write-Host "Updating project file..." -ForegroundColor Cyan

$projectFile = "Naghuma Toolbox.vcxproj"
$content = Get-Content $projectFile -Raw

# Add Phase 22 files if not present
if ($content -notmatch 'HuffmanDialog') {
    # Add after WaveletDialog includes
    $content = $content -replace '(<ClInclude Include="include\\WaveletDialog.h" />)', '$1`n    <ClInclude Include="include\HuffmanDialog.h" />'
    
    # Add implementation files after moc_SharpeningDialog
    $content = $content -replace '(<ClCompile Include="src\\moc_SharpeningDialog.cpp" />)', @'
$1
    <ClCompile Include="src\HuffmanDialog.cpp" />
    <ClCompile Include="src\moc_HuffmanDialog.cpp" />
    <ClCompile Include="lib\compression\HuffmanCoding.cpp" />
    <ClCompile Include="src\MainWindow_Phase22.cpp" />
'@
    
    Set-Content $projectFile -Value $content -NoNewline
    Write-Host "? Project file updated" -ForegroundColor Green
} else {
    Write-Host "? Already updated" -ForegroundColor Green
}

# Update MainWindow.cpp
Write-Host "Adding menu item..." -ForegroundColor Cyan
$mainFile = "src\MainWindow.cpp"
$mainContent = Get-Content $mainFile -Raw

if ($mainContent -notmatch 'showHuffmanDialog') {
    # Add include
    $mainContent = $mainContent -replace '(#include "SharpeningDialog\.h".*)', '$1`n#include "HuffmanDialog.h"  // Phase 22'
    
    # Add menu item
    $mainContent = $mainContent -replace '(ADD_MENU_ACTION\(processMenu, "Compress Image\.\.\.", applyCompression\);)', @'
$1

    processMenu->addSeparator();

    // Phase 22: Huffman Coding
    ADD_MENU_ACTION(processMenu, "Huffman Coding...", showHuffmanDialog);
'@
    
    Set-Content $mainFile -Value $mainContent -NoNewline
    Write-Host "? Menu added" -ForegroundColor Green
} else {
    Write-Host "? Already added" -ForegroundColor Green
}

Write-Host ""
Write-Host "? DONE! Open Visual Studio and build." -ForegroundColor Green
