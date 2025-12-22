# ============================================================================
# VERIFY ALL DIALOG FILES HAVE MOC INCLUDES
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  VERIFYING MOC INCLUDES            " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Get all dialog .cpp files
$dialogFiles = Get-ChildItem "src" -Filter "*Dialog.cpp" | Where-Object { $_.Name -notlike "moc_*" }

Write-Host "Checking $($dialogFiles.Count) dialog files..." -ForegroundColor Yellow
Write-Host ""

$allGood = $true

foreach ($file in $dialogFiles) {
    $mocName = "moc_" + $file.Name
    $mocPath = "src\$mocName"
    
    if (Test-Path $mocPath) {
        $content = Get-Content $file.FullName -Raw
        
        if ($content -match [regex]::Escape($mocName)) {
            Write-Host "  $($file.Name) - OK (includes $mocName)" -ForegroundColor Green
        } else {
            Write-Host "  $($file.Name) - MISSING INCLUDE for $mocName" -ForegroundColor Red
            $allGood = $false
        }
    } else {
        Write-Host "  $($file.Name) - No MOC file (might not need one)" -ForegroundColor Gray
    }
}

Write-Host ""

if ($allGood) {
    Write-Host "=====================================" -ForegroundColor Green
    Write-Host "  ALL MOC INCLUDES PRESENT!         " -ForegroundColor Green
    Write-Host "=====================================" -ForegroundColor Green
} else {
    Write-Host "=====================================" -ForegroundColor Red
    Write-Host "  SOME MOC INCLUDES MISSING!        " -ForegroundColor Red
    Write-Host "=====================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Run add_missing_moc_includes.ps1 to fix." -ForegroundColor Yellow
}

Write-Host ""
