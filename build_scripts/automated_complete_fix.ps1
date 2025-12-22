# ============================================================================
# FINAL FIX - ALL STEPS AUTOMATED
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  AUTOMATED COMPLETE FIX            " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "This will:" -ForegroundColor Yellow
Write-Host "  1. Regenerate project file with proper XML" -ForegroundColor White
Write-Host "  2. Clear Visual Studio cache" -ForegroundColor White
Write-Host "  3. Verify everything is ready" -ForegroundColor White
Write-Host ""
Write-Host "Make sure Visual Studio is CLOSED!" -ForegroundColor Red
Write-Host ""
Write-Host "Press Enter to continue..." -ForegroundColor Yellow
$null = Read-Host

# Step 1: Regenerate
Write-Host ""
Write-Host "[1/3] Regenerating project file..." -ForegroundColor Yellow
& "$PSScriptRoot\regenerate_project.ps1"

# Step 2: Clear cache
Write-Host ""
Write-Host "[2/3] Clearing cache..." -ForegroundColor Yellow

if (Test-Path ".vs") {
    Remove-Item ".vs" -Recurse -Force -ErrorAction SilentlyContinue
    Write-Host "  .vs deleted" -ForegroundColor Green
}

if (Test-Path "Naghuma Toolbox.vcxproj.user") {
    Remove-Item "Naghuma Toolbox.vcxproj.user" -Force
    Write-Host "  .vcxproj.user deleted" -ForegroundColor Green
}

# Step 3: Verify
Write-Host ""
Write-Host "[3/3] Verifying..." -ForegroundColor Yellow
& "$PSScriptRoot\verify_ready_to_build.ps1"

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  READY TO OPEN IN VISUAL STUDIO    " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
