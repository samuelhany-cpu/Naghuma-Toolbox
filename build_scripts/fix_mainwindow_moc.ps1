# ============================================================================
# FIX MAINWINDOW.CPP MOC INCLUDE (LOCKED FILE)
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Yellow
Write-Host "  MAINWINDOW.CPP MOC FIX            " -ForegroundColor Yellow
Write-Host "=====================================" -ForegroundColor Yellow
Write-Host ""

Write-Host "MainWindow.cpp is locked by Visual Studio." -ForegroundColor Red
Write-Host ""
Write-Host "Manual steps:" -ForegroundColor Yellow
Write-Host ""
Write-Host "1. In Visual Studio, open src\MainWindow.cpp" -ForegroundColor White
Write-Host ""
Write-Host "2. Scroll to the VERY END of the file" -ForegroundColor White
Write-Host ""
Write-Host "3. Add this line at the end:" -ForegroundColor White
Write-Host ""
Write-Host '   #include "moc_MainWindow.cpp"' -ForegroundColor Cyan
Write-Host ""
Write-Host "4. Save the file (Ctrl+S)" -ForegroundColor White
Write-Host ""
Write-Host "5. Rebuild: Build -> Rebuild Solution" -ForegroundColor White
Write-Host ""
Write-Host "This will fix the remaining 6 unresolved externals." -ForegroundColor Green
Write-Host ""

# Check if already has include
if (Test-Path "src\MainWindow.cpp") {
    $content = Get-Content "src\MainWindow.cpp" -Raw
    
    if ($content -match 'moc_MainWindow\.cpp') {
        Write-Host "NOTE: MainWindow.cpp already has MOC include!" -ForegroundColor Green
        Write-Host "The unresolved externals might be a build cache issue." -ForegroundColor Yellow
        Write-Host ""
        Write-Host "Try: Build -> Clean Solution, then Rebuild" -ForegroundColor Cyan
    }
}

Write-Host ""
