# ============================================================================
# ADD MOC INCLUDES TO CORE CPP FILES
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  ADDING MOC INCLUDES TO CPP FILES  " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Map of cpp files to their MOC files
$cppToMoc = @{
    "src\CollapsibleToolbar.cpp" = "moc_CollapsibleToolbar.cpp"
    "src\CropTool.cpp" = "moc_CropTool.cpp"
    "src\HistogramWidget.cpp" = "moc_HistogramWidget.cpp"
    "src\ImageCanvas.cpp" = "moc_ImageCanvas.cpp"
    "src\LayerManager.cpp" = "moc_LayerManager.cpp"
    "src\MainWindow.cpp" = "moc_MainWindow.cpp"
    "src\RightSidebarWidget.cpp" = "moc_RightSidebarWidget.cpp"
    "src\ROIManager.cpp" = "moc_ROIManager.cpp"
}

$fixed = 0
$skipped = 0
$notFound = 0

foreach ($cpp in $cppToMoc.Keys) {
    $mocFile = $cppToMoc[$cpp]
    
    if (-not (Test-Path $cpp)) {
        Write-Host "$cpp not found - skipped" -ForegroundColor Gray
        $notFound++
        continue
    }
    
    $content = Get-Content $cpp -Raw
    
    if ($content -match [regex]::Escape($mocFile)) {
        Write-Host "$cpp already includes $mocFile" -ForegroundColor Gray
        $skipped++
    } else {
        Write-Host "Adding $mocFile to $cpp..." -ForegroundColor Yellow
        
        # Add include at the end
        $content = $content.TrimEnd()
        $content += "`r`n`r`n#include `"$mocFile`"`r`n"
        
        Set-Content $cpp -Value $content -NoNewline
        
        Write-Host "  Added: #include `"$mocFile`"" -ForegroundColor Green
        $fixed++
    }
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  MOC INCLUDES ADDED!               " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  Added: $fixed" -ForegroundColor Cyan
Write-Host "  Already present: $skipped" -ForegroundColor Cyan
Write-Host "  Not found: $notFound" -ForegroundColor Cyan
Write-Host ""

if ($fixed -gt 0) {
    Write-Host "Reload project in Visual Studio and rebuild." -ForegroundColor Yellow
    Write-Host "The unresolved external errors should be fixed." -ForegroundColor Green
}

Write-Host ""
