# Generate MOC file for ColorProcessingDialog
Write-Host "Generating MOC file for ColorProcessingDialog..." -ForegroundColor Cyan

$mocPath = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
$headerPath = "include\ColorProcessingDialog.h"
$outputPath = "src\moc_ColorProcessingDialog.cpp"

if (Test-Path $mocPath) {
    & $mocPath -o $outputPath $headerPath
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "MOC file generated successfully!" -ForegroundColor Green
        Write-Host "Output: $outputPath" -ForegroundColor White
    } else {
        Write-Host "MOC generation failed!" -ForegroundColor Red
    }
} else {
    Write-Host "MOC tool not found at: $mocPath" -ForegroundColor Red
    Write-Host "Please check your Qt installation path" -ForegroundColor Yellow
}
