Write-Host ""
Write-Host "??????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?     Naghuma Toolbox - Debug Application Launcher  ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Stop any existing instances
Write-Host "Checking for existing instances..." -ForegroundColor Yellow
$existing = Get-Process "Naghuma Toolbox" -ErrorAction SilentlyContinue
if ($existing) {
    Write-Host "  ? Found running instance(s). Stopping..." -ForegroundColor Yellow
    Stop-Process -Name "Naghuma Toolbox" -Force -ErrorAction SilentlyContinue
    Start-Sleep -Seconds 1
    Write-Host "  ? Stopped" -ForegroundColor Green
} else {
    Write-Host "  ? No existing instances" -ForegroundColor Green
}

# Verify executable exists
$exePath = "x64\Debug\Naghuma Toolbox.exe"
if (!(Test-Path $exePath)) {
    Write-Host ""
    Write-Host "? ERROR: Executable not found!" -ForegroundColor Red
    Write-Host "  Expected location: $exePath" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please build the project first:" -ForegroundColor Yellow
    Write-Host "  1. Open Naghuma Toolbox.sln in Visual Studio" -ForegroundColor Cyan
    Write-Host "  2. Select 'Debug' configuration" -ForegroundColor Cyan
    Write-Host "  3. Press Ctrl+Shift+B to build" -ForegroundColor Cyan
    Write-Host ""
    exit 1
}

# Check DLL count
$dllCount = (Get-ChildItem "x64\Debug\*.dll" -ErrorAction SilentlyContinue | Measure-Object).Count
Write-Host ""
Write-Host "DLL Status:" -ForegroundColor Yellow
Write-Host "  • Total DLLs in Debug folder: $dllCount" -ForegroundColor Cyan

# Check critical DLLs
$criticalDlls = @(
    "opencv_world430d.dll",
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll",
    "tesseract55.dll",
    "leptonica-1.85.0.dll",
    "archive.dll",
    "jpeg62.dll",
    "gif.dll",
    "openjp2.dll"
)

$missingDlls = @()
foreach ($dll in $criticalDlls) {
    if (!(Test-Path "x64\Debug\$dll")) {
        $missingDlls += $dll
    }
}

if ($missingDlls.Count -gt 0) {
    Write-Host ""
    Write-Host "? Missing Critical DLLs:" -ForegroundColor Red
    foreach ($dll in $missingDlls) {
        Write-Host "    - $dll" -ForegroundColor Yellow
    }
    Write-Host ""
    Write-Host "Run this command to copy missing DLLs:" -ForegroundColor Yellow
    Write-Host "  .\copy_missing_dlls.ps1" -ForegroundColor Cyan
    Write-Host ""
    exit 1
} else {
    Write-Host "  • All critical DLLs present ?" -ForegroundColor Green
}

# Launch application
Write-Host ""
Write-Host "Launching application..." -ForegroundColor Yellow
$proc = Start-Process $exePath -PassThru

if ($proc) {
    Write-Host "  ? Process started (ID: $($proc.Id))" -ForegroundColor Green
    
    # Monitor for a few seconds
    Write-Host ""
    Write-Host "Monitoring application..." -ForegroundColor Yellow
    Start-Sleep -Seconds 3
    
    try {
        $running = Get-Process -Id $proc.Id -ErrorAction Stop
        Write-Host ""
        Write-Host "??????????????????????????????????????????????????????" -ForegroundColor Green
        Write-Host "?       ? APPLICATION RUNNING SUCCESSFULLY!         ?" -ForegroundColor Green
        Write-Host "??????????????????????????????????????????????????????" -ForegroundColor Green
        Write-Host ""
        Write-Host "?? Application Info:" -ForegroundColor Cyan
        Write-Host "  • Process ID: $($running.Id)" -ForegroundColor White
        Write-Host "  • Memory Usage: $([math]::Round($running.WorkingSet64/1MB,2)) MB" -ForegroundColor White
        Write-Host "  • Responding: $($running.Responding)" -ForegroundColor $(if($running.Responding){'Green'}else{'Red'})
        Write-Host "  • Executable: $exePath" -ForegroundColor Gray
        Write-Host ""
        Write-Host "? This is the correct Debug version with the proper UI" -ForegroundColor Green
        Write-Host ""
    } catch {
        Write-Host ""
        Write-Host "? Application exited unexpectedly" -ForegroundColor Red
        Write-Host "  Exit code: $($proc.ExitCode)" -ForegroundColor Yellow
        Write-Host ""
    }
} else {
    Write-Host "  ? Failed to start application" -ForegroundColor Red
    Write-Host ""
}
