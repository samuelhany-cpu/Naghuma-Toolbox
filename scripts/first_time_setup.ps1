# First-Time Setup Script for Naghuma Toolbox
# Run this once after cloning or downloading the project

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Naghuma Toolbox - First-Time Setup" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

Write-Host "`n[1/4] Checking Qt installation..." -ForegroundColor Yellow
$qtPath = "C:\Qt\6.7.3\msvc2019_64\bin"
if (Test-Path $qtPath) {
    Write-Host "? Qt found at $qtPath" -ForegroundColor Green
} else {
    Write-Host "?? Qt not found at expected path" -ForegroundColor Red
    Write-Host "Please install Qt 6.7.3 or update the path in scripts" -ForegroundColor Yellow
}

Write-Host "`n[2/4] Checking OpenCV installation..." -ForegroundColor Yellow
$opencvPath = "F:\OpenCV\opencv\build\x64\vc15\bin"
if (Test-Path $opencvPath) {
    Write-Host "? OpenCV found at $opencvPath" -ForegroundColor Green
} else {
    Write-Host "?? OpenCV not found at expected path" -ForegroundColor Red
    Write-Host "Please install OpenCV 4.3.0 or update the path in scripts" -ForegroundColor Yellow
}

Write-Host "`n[3/4] Generating MOC files..." -ForegroundColor Yellow
if (Test-Path "scripts\generate_moc.ps1") {
    & .\scripts\generate_moc.ps1
    & .\scripts\generate_new_mocs.ps1
    Write-Host "? MOC files generated" -ForegroundColor Green
} else {
    Write-Host "?? MOC generation scripts not found" -ForegroundColor Red
}

Write-Host "`n[4/4] Building project..." -ForegroundColor Yellow
$buildOutput = msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m:1 /nologo 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "? Build successful!" -ForegroundColor Green
    
    Write-Host "`nCopying DLLs..." -ForegroundColor Yellow
    & .\scripts\setup_dlls.ps1
    
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "? SETUP COMPLETE!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    
    Write-Host "`nYou can now run the application using:" -ForegroundColor Yellow
    Write-Host "  • Double-click: Launch Naghuma Toolbox.bat" -ForegroundColor Cyan
    Write-Host "  • PowerShell: .\scripts\run_app.ps1" -ForegroundColor Cyan
    
    $response = Read-Host "`nWould you like to launch the application now? (Y/N)"
    if ($response -eq 'Y' -or $response -eq 'y') {
        Write-Host "`nLaunching..." -ForegroundColor Cyan
        Start-Process "x64\Debug\Naghuma Toolbox.exe"
    }
} else {
    Write-Host "? Build failed" -ForegroundColor Red
    Write-Host "`nShowing last 20 lines of build output:" -ForegroundColor Yellow
    $buildOutput | Select-Object -Last 20
    Write-Host "`nPlease fix the errors and run setup again." -ForegroundColor Yellow
}
