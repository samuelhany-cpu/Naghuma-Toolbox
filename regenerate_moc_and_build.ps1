# Regenerate MOC and Build Script
Write-Host "`n=== Regenerating MOC Files and Building ===" -ForegroundColor Cyan

# Remove old moc file if exists
if (Test-Path "src\moc_ColorProcessingDialog.cpp") {
    Remove-Item "src\moc_ColorProcessingDialog.cpp" -Force
    Write-Host "Removed old MOC file" -ForegroundColor Yellow
}

# Step 1: Generate MOC with verbose output
Write-Host "`n[Step 1] Generating MOC file..." -ForegroundColor Yellow
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"

if (-not (Test-Path $qtMoc)) {
    Write-Host "ERROR: Qt MOC not found at $qtMoc" -ForegroundColor Red
    exit 1
}

# Try generating with explicit output
$mocProcess = Start-Process -FilePath $qtMoc -ArgumentList @(
    "-IC:\Qt\6.7.3\msvc2019_64\include",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtCore",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtGui",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtWidgets",
    "-Iinclude",
    "-DQT_WIDGETS_LIB",
    "-DQT_GUI_LIB",
    "-DQT_CORE_LIB",
    "include\ColorProcessingDialog.h",
    "-o",
    "src\moc_ColorProcessingDialog.cpp"
) -Wait -PassThru -NoNewWindow

Write-Host "MOC Exit Code: $($mocProcess.ExitCode)" -ForegroundColor Cyan

# Check if file was actually created and has content
if (Test-Path "src\moc_ColorProcessingDialog.cpp") {
    $fileSize = (Get-Item "src\moc_ColorProcessingDialog.cpp").Length
    if ($fileSize -gt 100) {
        Write-Host "  + MOC file generated successfully ($fileSize bytes)" -ForegroundColor Green
    } else {
        Write-Host "  WARNING: MOC file is too small ($fileSize bytes)" -ForegroundColor Yellow
        Get-Content "src\moc_ColorProcessingDialog.cpp" | Write-Host
        Write-Host "`nAttempting alternative MOC generation..." -ForegroundColor Yellow
        
        # Try without pragma once issues - copy header to temp file
        $tempHeader = "include\ColorProcessingDialog_temp.h"
        Get-Content "include\ColorProcessingDialog.h" | Set-Content $tempHeader
        
        & $qtMoc -IC:\Qt\6.7.3\msvc2019_64\include `
                 -IC:\Qt\6.7.3\msvc2019_64\include\QtCore `
                 -IC:\Qt\6.7.3\msvc2019_64\include\QtGui `
                 -IC:\Qt\6.7.3\msvc2019_64\include\QtWidgets `
                 -Iinclude `
                 -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB `
                 $tempHeader `
                 -o src\moc_ColorProcessingDialog.cpp
        
        Remove-Item $tempHeader -Force
        
        $fileSize = (Get-Item "src\moc_ColorProcessingDialog.cpp").Length
        if ($fileSize -gt 100) {
            Write-Host "  + Alternative MOC generation successful ($fileSize bytes)" -ForegroundColor Green
        } else {
            Write-Host "  ERROR: MOC generation failed" -ForegroundColor Red
            exit 1
        }
    }
} else {
    Write-Host "  ERROR: MOC file not created" -ForegroundColor Red
    exit 1
}

# Verify the MOC file contains the necessary symbols
Write-Host "`n[Step 2] Verifying MOC file..." -ForegroundColor Yellow
$mocContent = Get-Content "src\moc_ColorProcessingDialog.cpp" -Raw
if ($mocContent -match "ColorProcessingDialog::metaObject" -and 
    $mocContent -match "ColorProcessingDialog::qt_metacast" -and
    $mocContent -match "previewUpdated") {
    Write-Host "  + MOC file contains all required symbols" -ForegroundColor Green
} else {
    Write-Host "  WARNING: MOC file may be incomplete" -ForegroundColor Yellow
    Write-Host "  Checking what's in the file..." -ForegroundColor Cyan
    $mocContent | Select-String "ColorProcessingDialog" | ForEach-Object { Write-Host "    $_" }
}

# Step 3: Clean and rebuild
Write-Host "`n[Step 3] Cleaning previous build..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.sln" /t:Clean /p:Configuration=Debug /p:Platform=x64 /nologo /v:quiet

Write-Host "`n[Step 4] Building project..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n=== BUILD SUCCESSFUL ===" -ForegroundColor Green
    Write-Host "`nExecutable: x64\Debug\Naghuma Toolbox.exe" -ForegroundColor Cyan
    
    # Check if executable exists
    if (Test-Path "x64\Debug\Naghuma Toolbox.exe") {
        Write-Host "`nRun the application? (Y/N): " -ForegroundColor Yellow -NoNewline
        $response = Read-Host
        if ($response -eq 'Y' -or $response -eq 'y') {
            Write-Host "Launching application..." -ForegroundColor Green
            Start-Process "x64\Debug\Naghuma Toolbox.exe"
        }
    }
} else {
    Write-Host "`n=== BUILD FAILED ===" -ForegroundColor Red
    Write-Host "Exit code: $LASTEXITCODE" -ForegroundColor Yellow
    
    # Check for specific errors
    Write-Host "`nChecking for common issues..." -ForegroundColor Cyan
    if (-not (Test-Path "src\moc_ColorProcessingDialog.cpp")) {
        Write-Host "  - MOC file is missing" -ForegroundColor Red
    }
    
    exit 1
}
