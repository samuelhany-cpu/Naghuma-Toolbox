# Use Qt's Official MOC to Generate File
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"

Write-Host "Generating MOC with Qt's official compiler..." -ForegroundColor Cyan

$args = @(
    "-IC:\Qt\6.7.3\msvc2019_64\include",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtCore",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtGui",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtWidgets",
    "-Iinclude",
    "-DQT_WIDGETS_LIB",
    "-DQT_GUI_LIB",
    "-DQT_CORE_LIB",
    "include\ColorProcessingDialog.h",
    "-o",
    "src\moc_ColorProcessingDialog.cpp"
)

& $qtMoc $args

if (Test-Path "src\moc_ColorProcessingDialog.cpp") {
    Write-Host "? MOC file generated" -ForegroundColor Green
    
    Write-Host "`nBuilding project..." -ForegroundColor Yellow
    msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`n? BUILD SUCCESSFUL - ColorProcessingDialog integrated!" -ForegroundColor Green
    }
} else {
    Write-Host "? MOC generation failed" -ForegroundColor Red
}
