# Add MOC files to project and build

Write-Host "Adding MOC files to project..." -ForegroundColor Cyan

$vcxproj = "Naghuma Toolbox.vcxproj"
$content = Get-Content $vcxproj -Raw

# Check if MOC files are already added
if ($content -notmatch "moc_RightSidebarWidget.cpp") {
    Write-Host "Adding new MOC files to project..." -ForegroundColor Yellow
    
    $mocInsert = @"
    <ClCompile Include="src\moc_RightSidebarWidget.cpp" />
    <ClCompile Include="src\moc_LayerManager.cpp" />
    <ClCompile Include="src\moc_TransformDialog.cpp" />
"@
    
    $content = $content -replace '(<ClCompile Include="src\\moc_MainWindow.cpp" />)', "`$1`n$mocInsert"
    Set-Content $vcxproj -Value $content
    Write-Host "? MOC files added to project" -ForegroundColor Green
} else {
    Write-Host "? MOC files already in project" -ForegroundColor Green
}

# Build
Write-Host "`nBuilding project..." -ForegroundColor Cyan
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build /m:1 /nologo

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n? BUILD SUCCESSFUL!" -ForegroundColor Green
    
    # Copy DLLs
    Write-Host "`nCopying required DLLs..." -ForegroundColor Cyan
    & .\scripts\setup_dlls.ps1
    
    Write-Host "`nLaunching application..." -ForegroundColor Cyan
    Start-Process "x64\Debug\Naghuma Toolbox.exe"
} else {
    Write-Host "`n? BUILD FAILED" -ForegroundColor Red
}
