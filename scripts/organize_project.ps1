# Script to organize project files into the new professional structure
# This will copy files to their new locations

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "   Project File Organization Script    " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

$projectRoot = "F:\Naghuma Toolbox"

# Define source and destination mappings
$fileMappings = @(
    # Main application files
    @{Src="main.cpp"; Dst="src\main.cpp"},
    @{Src="MainWindow.cpp"; Dst="src\MainWindow.cpp"},
    @{Src="MainWindow.h"; Dst="include\MainWindow.h"},
    @{Src="ImageCanvas.cpp"; Dst="src\ImageCanvas.cpp"},
    @{Src="ImageCanvas.h"; Dst="include\ImageCanvas.h"},
    @{Src="HistogramWidget.cpp"; Dst="src\HistogramWidget.cpp"},
    @{Src="HistogramWidget.h"; Dst="include\HistogramWidget.h"},
    
    # MOC files (keep in root or move to build)
    @{Src="moc_MainWindow.cpp"; Dst="src\moc_MainWindow.cpp"},
    @{Src="moc_ImageCanvas.cpp"; Dst="src\moc_ImageCanvas.cpp"},
    @{Src="moc_HistogramWidget.cpp"; Dst="src\moc_HistogramWidget.cpp"}
)

Write-Host "Step 1: Copying files to new structure..." -ForegroundColor Yellow
Write-Host ""

foreach ($mapping in $fileMappings) {
    $srcPath = Join-Path $projectRoot $mapping.Src
    $dstPath = Join-Path $projectRoot $mapping.Dst
    
    if (Test-Path $srcPath) {
        Write-Host "  Copying: $($mapping.Src) -> $($mapping.Dst)" -ForegroundColor Gray
        
        # Ensure destination directory exists
        $dstDir = Split-Path $dstPath -Parent
        if (-not (Test-Path $dstDir)) {
            New-Item -ItemType Directory -Path $dstDir -Force | Out-Null
        }
        
        # Copy file
        Copy-Item -Path $srcPath -Destination $dstPath -Force
        Write-Host "    [OK] Copied successfully" -ForegroundColor Green
    } else {
        Write-Host "  WARNING: Source file not found: $($mapping.Src)" -ForegroundColor Yellow
    }
}

Write-Host ""
Write-Host "Step 2: Moving script files..." -ForegroundColor Yellow
Write-Host ""

$scriptFiles = @(
    "add_moc_to_project.ps1",
    "clean_emojis_simple.ps1",
    "configure_cpp17.ps1",
    "configure_qt.ps1",
    "generate_moc.ps1",
    "remove_emojis.ps1",
    "update_colors.ps1",
    "update_qt_project.ps1",
    "update_vcxproj.ps1"
)

foreach ($script in $scriptFiles) {
    $srcPath = Join-Path $projectRoot $script
    $dstPath = Join-Path $projectRoot "scripts\$script"
    
    if (Test-Path $srcPath) {
        Write-Host "  Moving: $script -> scripts\" -ForegroundColor Gray
        Copy-Item -Path $srcPath -Destination $dstPath -Force
        Write-Host "    [OK] Moved successfully" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "Step 3: Moving batch files..." -ForegroundColor Yellow
Write-Host ""

$batchFiles = @(
    "build_and_run.bat",
    "run_naghuma_toolbox.bat"
)

foreach ($batch in $batchFiles) {
    $srcPath = Join-Path $projectRoot $batch
    $dstPath = Join-Path $projectRoot "scripts\$batch"
    
    if (Test-Path $srcPath) {
        Write-Host "  Moving: $batch -> scripts\" -ForegroundColor Gray
        Copy-Item -Path $srcPath -Destination $dstPath -Force
        Write-Host "    [OK] Moved successfully" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "          Organization Complete!        " -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "New Project Structure:" -ForegroundColor Cyan
Write-Host ""
Write-Host "  Naghuma Toolbox/" -ForegroundColor White
Write-Host "    ├── src/              # Source files (.cpp)" -ForegroundColor Gray
Write-Host "    ├── include/          # Header files (.h)" -ForegroundColor Gray
Write-Host "    ├── lib/              # External libraries" -ForegroundColor Gray
Write-Host "    │   ├── filters/      # Image filters library" -ForegroundColor DarkGray
Write-Host "    │   ├── transforms/   # Transform operations library" -ForegroundColor DarkGray
Write-Host "    │   ├── histogram/    # Histogram operations library" -ForegroundColor DarkGray
Write-Host "    │   └── utils/        # Utility functions" -ForegroundColor DarkGray
Write-Host "    ├── scripts/          # Build and utility scripts" -ForegroundColor Gray
Write-Host "    └── x64/Debug/        # Build output" -ForegroundColor Gray
Write-Host ""
Write-Host "NOTE: Original files remain in place. You can delete them after" -ForegroundColor Yellow
Write-Host "      verifying the project builds successfully with new structure." -ForegroundColor Yellow
Write-Host ""

Read-Host "Press Enter to exit"
