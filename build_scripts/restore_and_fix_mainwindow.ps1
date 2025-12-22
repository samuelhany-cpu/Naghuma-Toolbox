# ============================================================================
# RESTORE MAINWINDOW.CPP AND ADD MOC
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  RESTORING MAINWINDOW.CPP          " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Check if backup exists
if (-not (Test-Path "src\MainWindow.cpp.backup")) {
    Write-Host "ERROR: Backup not found!" -ForegroundColor Red
    exit 1
}

# Restore from backup
Write-Host "Restoring MainWindow.cpp from backup..." -ForegroundColor Yellow
Copy-Item "src\MainWindow.cpp.backup" "src\MainWindow.cpp" -Force

$size = (Get-Item "src\MainWindow.cpp").Length
Write-Host "  Restored: $size bytes" -ForegroundColor Green
Write-Host ""

# Wait a moment for file system
Start-Sleep -Milliseconds 500

# Try to add MOC include
Write-Host "Attempting to add MOC include..." -ForegroundColor Yellow

$maxAttempts = 3
$attempt = 0
$success = $false

while ($attempt -lt $maxAttempts -and -not $success) {
    $attempt++
    Write-Host "  Attempt $attempt..." -ForegroundColor Gray
    
    try {
        $content = Get-Content "src\MainWindow.cpp" -Raw -ErrorAction Stop
        
        # Check if already has MOC include
        if ($content -match 'moc_MainWindow\.cpp') {
            Write-Host "  MOC include already present!" -ForegroundColor Green
            $success = $true
            break
        }
        
        # Add MOC include
        $content = $content.TrimEnd()
        $content += "`r`n`r`n#include `"moc_MainWindow.cpp`"`r`n"
        
        Set-Content "src\MainWindow.cpp" -Value $content -NoNewline -ErrorAction Stop
        
        Write-Host "  MOC include added successfully!" -ForegroundColor Green
        $success = $true
        
    } catch {
        Write-Host "  Failed: $($_.Exception.Message)" -ForegroundColor Red
        if ($attempt -lt $maxAttempts) {
            Write-Host "  Waiting 2 seconds before retry..." -ForegroundColor Yellow
            Start-Sleep -Seconds 2
        }
    }
}

Write-Host ""

if ($success) {
    Write-Host "=====================================" -ForegroundColor Green
    Write-Host "  MAINWINDOW.CPP READY!             " -ForegroundColor Green
    Write-Host "=====================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Reload project in Visual Studio and rebuild." -ForegroundColor Yellow
} else {
    Write-Host "=====================================" -ForegroundColor Red
    Write-Host "  MANUAL ACTION REQUIRED            " -ForegroundColor Red
    Write-Host "=====================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "File is locked by Visual Studio." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "In Visual Studio:" -ForegroundColor Cyan
    Write-Host "  1. Open src\MainWindow.cpp" -ForegroundColor White
    Write-Host "  2. Go to the very end" -ForegroundColor White
    Write-Host '  3. Add: #include "moc_MainWindow.cpp"' -ForegroundColor White
    Write-Host "  4. Save and rebuild" -ForegroundColor White
}

Write-Host ""
