# ============================================================================
# VISUAL STUDIO CACHE CLEAR
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  CLEARING VISUAL STUDIO CACHE      " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Step 1: Close Visual Studio if open" -ForegroundColor Yellow
Write-Host "  Press Enter when VS is closed..." -ForegroundColor White
$null = Read-Host
Write-Host ""

Write-Host "Step 2: Deleting .vs folder..." -ForegroundColor Yellow
if (Test-Path ".vs") {
    Remove-Item ".vs" -Recurse -Force -ErrorAction SilentlyContinue
    Write-Host "  .vs folder deleted" -ForegroundColor Green
} else {
    Write-Host "  .vs folder not found (OK)" -ForegroundColor Gray
}
Write-Host ""

Write-Host "Step 3: Deleting .suo files..." -ForegroundColor Yellow
Get-ChildItem -Recurse -Filter "*.suo" -Force -ErrorAction SilentlyContinue | Remove-Item -Force
Write-Host "  .suo files deleted" -ForegroundColor Green
Write-Host ""

Write-Host "Step 4: Deleting user-specific files..." -ForegroundColor Yellow
if (Test-Path "Naghuma Toolbox.vcxproj.user") {
    Remove-Item "Naghuma Toolbox.vcxproj.user" -Force
    Write-Host "  .vcxproj.user deleted" -ForegroundColor Green
} else {
    Write-Host "  No .vcxproj.user found (OK)" -ForegroundColor Gray
}
Write-Host ""

Write-Host "Step 5: Deleting intermediate files..." -ForegroundColor Yellow
if (Test-Path "x64\Debug") {
    Get-ChildItem "x64\Debug" -Filter "*.obj" | Remove-Item -Force -ErrorAction SilentlyContinue
    Get-ChildItem "x64\Debug" -Filter "*.pdb" | Remove-Item -Force -ErrorAction SilentlyContinue
    Get-ChildItem "x64\Debug" -Filter "*.idb" | Remove-Item -Force -ErrorAction SilentlyContinue
    Write-Host "  Intermediate files deleted" -ForegroundColor Green
} else {
    Write-Host "  No intermediate files (OK)" -ForegroundColor Gray
}
Write-Host ""

Write-Host "=====================================" -ForegroundColor Green
Write-Host "  CACHE CLEARED!                    " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
Write-Host "Now open Visual Studio and load the solution." -ForegroundColor Yellow
Write-Host "The project should load cleanly." -ForegroundColor Yellow
Write-Host ""
