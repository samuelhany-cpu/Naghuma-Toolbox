# Complete build script for modular Naghuma Toolbox

Write-Host "==================================" -ForegroundColor Cyan
Write-Host "Naghuma Toolbox - Complete Build" -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan

# Step 1: Clean build
Write-Host "`n[1/5] Cleaning previous build..." -ForegroundColor Yellow
Remove-Item -Recurse -Force x64\Debug\*, x64\Release\* -ErrorAction SilentlyContinue
Write-Host "? Clean complete" -ForegroundColor Green

# Step 2: Regenerate ALL MOC files
Write-Host "`n[2/5] Generating MOC files..." -ForegroundColor Yellow
.\scripts\generate_moc.ps1
.\scripts\generate_new_mocs.ps1
Write-Host "? MOC generation complete" -ForegroundColor Green

# Step 3: Fix MainWindow.cpp by removing duplicate/broken code
Write-Host "`n[3/5] Fixing source files..." -ForegroundColor Yellow

# Count current lines
$currentLines = (Get-Content "src\MainWindow.cpp").Count
Write-Host "  Current MainWindow.cpp: $currentLines lines" -ForegroundColor Cyan

# The file should end right after createStatusBar, before the broken section
# Let's rebuild it properly
$goodContent = Get-Content "src\MainWindow.cpp" | Select-Object -First 482
Set-Content "src\MainWindow.cpp" -Value $goodContent

# Add the complete missing functions
& .\scripts\add_missing_functions.ps1

Write-Host "? Source files fixed" -ForegroundColor Green

# Step 4: Build project
Write-Host "`n[4/5] Building project..." -ForegroundColor Yellow
$buildResult = msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m:1 /nologo 2>&1

$errors = $buildResult | Select-String "error" | Measure-Object
$warnings = $buildResult | Select-String "warning" | Measure-Object

if ($LASTEXITCODE -eq 0) {
    Write-Host "? BUILD SUCCESSFUL!" -ForegroundColor Green
    Write-Host "  Warnings: $($warnings.Count)" -ForegroundColor Yellow
} else {
    Write-Host "? BUILD FAILED" -ForegroundColor Red
    Write-Host "  Errors: $($errors.Count)" -ForegroundColor Red
    Write-Host "  Warnings: $($warnings.Count)" -ForegroundColor Yellow
    Write-Host "`nShowing last 20 lines of build output:" -ForegroundColor Yellow
    $buildResult | Select-Object -Last 20
    exit 1
}

# Step 5: Run application
Write-Host "`n[5/5] Launching application..." -ForegroundColor Yellow
if (Test-Path "x64\Debug\Naghuma Toolbox.exe") {
    Write-Host "? Starting Naghuma Toolbox..." -ForegroundColor Green
    Start-Process "x64\Debug\Naghuma Toolbox.exe"
} else {
    Write-Host "? Executable not found!" -ForegroundColor Red
}

Write-Host "`n==================================" -ForegroundColor Cyan
Write-Host "? BUILD AND RUN COMPLETE!" -ForegroundColor Green
Write-Host "==================================" -ForegroundColor Cyan
