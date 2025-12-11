$vcxproj = "Naghuma Toolbox.vcxproj"
$content = Get-Content $vcxproj -Raw

if ($content -notmatch "moc_TransformDialog") {
    Write-Host "Adding missing MOC files..." -ForegroundColor Yellow
    
    $mocInsert = @"
    <ClCompile Include="src\moc_RightSidebarWidget.cpp" />
    <ClCompile Include="src\moc_LayerManager.cpp" />
    <ClCompile Include="src\moc_TransformDialog.cpp" />
"@
    
    $content = $content -replace '(<ClCompile Include="src\\moc_MainWindow.cpp" />)', "`$1`n$mocInsert"
    Set-Content $vcxproj -Value $content
    Write-Host "? Added 3 MOC files to project" -ForegroundColor Green
} else {
    Write-Host "? MOC files already in project" -ForegroundColor Green
}

# Verify
$content = Get-Content $vcxproj -Raw
if ($content -match "moc_Transform Dialog") {
    Write-Host "? Verified: moc_TransformDialog.cpp is in project" -ForegroundColor Green
}
