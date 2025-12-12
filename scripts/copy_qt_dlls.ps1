# Copy Qt DLLs to Build Output
# Run this after building to copy required Qt DLLs

param(
    [ValidateSet("Debug", "Release", "Both")]
    [string]$Configuration = "Both"
)

$qtBinDir = "C:\Qt\6.7.3\msvc2019_64\bin"
$projectDir = "F:\Naghuma Toolbox"

Write-Host "`n=== Copying Qt DLLs ===" -ForegroundColor Cyan

# Debug DLLs (with 'd' suffix)
$debugDlls = @(
    "Qt6Cored.dll",
    "Qt6Guid.dll",
    "Qt6Widgetsd.dll"
)

# Release DLLs (no 'd' suffix)
$releaseDlls = @(
    "Qt6Core.dll",
    "Qt6Gui.dll",
    "Qt6Widgets.dll"
)

# Copy Debug DLLs
if ($Configuration -eq "Debug" -or $Configuration -eq "Both") {
    $debugOutDir = Join-Path $projectDir "x64\Debug"
    
    if (Test-Path $debugOutDir) {
        Write-Host "`nCopying Debug DLLs to: $debugOutDir" -ForegroundColor Yellow
        
        foreach ($dll in $debugDlls) {
            $source = Join-Path $qtBinDir $dll
            $dest = Join-Path $debugOutDir $dll
            
            if (Test-Path $source) {
                Copy-Item $source $dest -Force
                Write-Host "  ? $dll" -ForegroundColor Green
            } else {
                Write-Host "  ? $dll (not found at $source)" -ForegroundColor Red
            }
        }
    } else {
        Write-Host "`nDebug output directory not found: $debugOutDir" -ForegroundColor Yellow
        Write-Host "Build the project first." -ForegroundColor Yellow
    }
}

# Copy Release DLLs
if ($Configuration -eq "Release" -or $Configuration -eq "Both") {
    $releaseOutDir = Join-Path $projectDir "x64\Release"
    
    if (Test-Path $releaseOutDir) {
        Write-Host "`nCopying Release DLLs to: $releaseOutDir" -ForegroundColor Yellow
        
        foreach ($dll in $releaseDlls) {
            $source = Join-Path $qtBinDir $dll
            $dest = Join-Path $releaseOutDir $dll
            
            if (Test-Path $source) {
                Copy-Item $source $dest -Force
                Write-Host "  ? $dll" -ForegroundColor Green
            } else {
                Write-Host "  ? $dll (not found at $source)" -ForegroundColor Red
            }
        }
    } else {
        Write-Host "`nRelease output directory not found: $releaseOutDir" -ForegroundColor Yellow
        Write-Host "Build the project in Release mode first." -ForegroundColor Yellow
    }
}

Write-Host "`n=== Done! ===" -ForegroundColor Green
Write-Host "`nYou can now run:" -ForegroundColor Cyan
if ($Configuration -eq "Debug" -or $Configuration -eq "Both") {
    Write-Host "  .\x64\Debug\Naghuma`` Toolbox.exe" -ForegroundColor Yellow
}
if ($Configuration -eq "Release" -or $Configuration -eq "Both") {
    Write-Host "  .\x64\Release\Naghuma`` Toolbox.exe" -ForegroundColor Yellow
}
Write-Host ""
