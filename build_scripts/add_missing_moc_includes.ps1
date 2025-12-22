# ============================================================================
# ADD MISSING MOC INCLUDES TO DIALOG FILES
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  ADDING MISSING MOC INCLUDES       " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$dialogFiles = Get-ChildItem "src" -Filter "*Dialog.cpp" | Where-Object { $_.Name -notlike "moc_*" }

$fixed = 0
$skipped = 0

foreach ($file in $dialogFiles) {
    $mocName = "moc_" + $file.Name
    $mocPath = "src\$mocName"
    
    if (Test-Path $mocPath) {
        $content = Get-Content $file.FullName -Raw
        
        if ($content -notmatch [regex]::Escape($mocName)) {
            Write-Host "Adding MOC include to $($file.Name)..." -ForegroundColor Yellow
            
            # Add include at the end of the file
            $includeStatement = "`r`n#include `"$mocName`"`r`n"
            
            # Remove trailing whitespace and add include
            $content = $content.TrimEnd()
            $content += $includeStatement
            
            Set-Content $file.FullName -Value $content -NoNewline
            
            Write-Host "  Added: #include `"$mocName`"" -ForegroundColor Green
            $fixed++
        } else {
            Write-Host "$($file.Name) already has MOC include - skipped" -ForegroundColor Gray
            $skipped++
        }
    }
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Green
Write-Host "  MOC INCLUDES ADDED!               " -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green
Write-Host ""
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "  Files fixed: $fixed" -ForegroundColor Cyan
Write-Host "  Files skipped (already OK): $skipped" -ForegroundColor Cyan
Write-Host ""
Write-Host "Reload project in Visual Studio and rebuild." -ForegroundColor Yellow
Write-Host "The LNK2005 errors should be resolved." -ForegroundColor Green
Write-Host ""
