# ============================================================================
# GENERATE ALL MISSING MOC FILES
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  GENERATING MISSING MOC FILES      " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$qtPath = "C:\Qt\6.7.3\msvc2019_64\bin"
$mocExe = "$qtPath\moc.exe"

if (-not (Test-Path $mocExe)) {
    Write-Host "ERROR: MOC not found at $mocExe" -ForegroundColor Red
    Write-Host "Update qtPath variable in script" -ForegroundColor Yellow
    exit 1
}

# List of header files that need MOC but are missing it
$headersNeedingMoc = @(
    "include\CollapsibleToolbar.h",
    "include\CropTool.h",
    "include\HistogramWidget.h",
    "include\ImageCanvas.h",
    "include\LayerManager.h",
    "include\MainWindow.h",
    "include\RightSidebarWidget.h",
    "include\ROIManager.h"
)

$generated = 0
$skipped = 0

foreach ($header in $headersNeedingMoc) {
    if (-not (Test-Path $header)) {
        Write-Host "  Skipping $header (not found)" -ForegroundColor Gray
        continue
    }
    
    $headerName = Split-Path $header -Leaf
    $mocName = "moc_$headerName" -replace '\.h$', '.cpp'
    $mocPath = "src\$mocName"
    
    if (Test-Path $mocPath) {
        Write-Host "  $mocName already exists" -ForegroundColor Gray
        $skipped++
    } else {
        Write-Host "  Generating $mocName..." -ForegroundColor Yellow
        
        & $mocExe $header -o $mocPath
        
        if ($LASTEXITCODE -eq 0 -and (Test-Path $mocPath)) {
            $size = (Get-Item $mocPath).Length
            Write-Host "    Created ($size bytes)" -ForegroundColor Green
            $generated++
        } else {
            Write-Host "    Failed to generate" -ForegroundColor Red
        }
    }
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  MOC GENERATION COMPLETE           " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  Generated: $generated" -ForegroundColor Cyan
Write-Host "  Skipped (already exist): $skipped" -ForegroundColor Cyan
Write-Host ""

if ($generated -gt 0) {
    Write-Host "Now run: add_moc_includes_to_cpp.ps1" -ForegroundColor Yellow
}

Write-Host ""
