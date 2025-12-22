# ============================================================================
# PHASE 21 COMPLETE INTEGRATION SCRIPT
# ============================================================================
# This script:
# 1. Generates MOC files for new dialogs
# 2. Updates .vcxproj with MOC files (when VS is closed)
# 3. Builds the project
# ============================================================================

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "   PHASE 21 INTEGRATION SCRIPT" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ErrorActionPreference = "Stop"
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
$projectFile = "Naghuma Toolbox.vcxproj"

# ============================================================================
# STEP 1: Generate MOC Files
# ============================================================================
Write-Host "[1/3] Generating MOC files..." -ForegroundColor Yellow

if (Test-Path "src\moc_IntensityTransformDialog.cpp") {
    Write-Host "  ? moc_IntensityTransformDialog.cpp exists" -ForegroundColor Green
} else {
    & $qtMoc include\IntensityTransformDialog.h -o src\moc_IntensityTransformDialog.cpp
    Write-Host "  ? Generated moc_IntensityTransformDialog.cpp" -ForegroundColor Green
}

if (Test-Path "src\moc_SharpeningDialog.cpp") {
    Write-Host "  ? moc_SharpeningDialog.cpp exists" -ForegroundColor Green
} else {
    & $qtMoc include\SharpeningDialog.h -o src\moc_SharpeningDialog.cpp
    Write-Host "  ? Generated moc_SharpeningDialog.cpp" -ForegroundColor Green
}

Write-Host ""

# ============================================================================
# STEP 2: Update Project File (if VS is closed)
# ============================================================================
Write-Host "[2/3] Updating project file..." -ForegroundColor Yellow

$content = Get-Content $projectFile -Raw

if ($content -match "moc_IntensityTransformDialog.cpp") {
    Write-Host "  ? Project already contains MOC files" -ForegroundColor Green
} else {
    # Check if file is locked (VS is open)
    try {
        $fileStream = [System.IO.File]::Open($projectFile, 'Open', 'ReadWrite', 'None')
        $fileStream.Close()
        
        # File is not locked, we can modify it
        Write-Host "  ? Updating .vcxproj file..." -ForegroundColor Cyan
        
        # Add MOC files after IntensityTransformDialog.cpp
        $pattern = '(<ClCompile Include="src\\IntensityTransformDialog.cpp" />)'
        $replacement = '$1
    <ClCompile Include="src\\moc_IntensityTransformDialog.cpp" />'
        
        $content = $content -replace $pattern, $replacement
        
        # Add MOC files after SharpeningDialog.cpp
        $pattern = '(<ClCompile Include="src\\SharpeningDialog.cpp" />)'
        $replacement = '$1
    <ClCompile Include="src\\moc_SharpeningDialog.cpp" />'
        
        $content = $content -replace $pattern, $replacement
        
        Set-Content -Path $projectFile -Value $content -NoNewline
        Write-Host "  ? Project file updated" -ForegroundColor Green
        
    } catch {
        Write-Host "  ? Project file is locked (Visual Studio is open)" -ForegroundColor Yellow
        Write-Host "    Please close Visual Studio and run this script again," -ForegroundColor Yellow
        Write-Host "    OR manually add these to the .vcxproj:" -ForegroundColor Yellow
        Write-Host "      - src\moc_IntensityTransformDialog.cpp" -ForegroundColor Cyan
        Write-Host "      - src\moc_SharpeningDialog.cpp" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "  Continuing anyway..." -ForegroundColor Yellow
    }
}

Write-Host ""

# ============================================================================
# STEP 3: Build Project
# ============================================================================
Write-Host "[3/3] Building project..." -ForegroundColor Yellow

$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

if (-not (Test-Path $msbuild)) {
    $msbuild = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
}

if (-not (Test-Path $msbuild)) {
    Write-Host "  ? MSBuild not found. Please build manually." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "   SETUP COMPLETE" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    exit 0
}

Write-Host "  ? Running MSBuild..." -ForegroundColor Cyan
& $msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build /v:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "   ? BUILD SUCCESSFUL" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Phase 21 features are now ready to test!" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "New menu items:" -ForegroundColor Yellow
    Write-Host "  • Process ? Intensity Transformations ? Gamma & Log Transform" -ForegroundColor Cyan
    Write-Host "  • Process ? Sharpening Filters ? Laplacian/Unsharp/High-Boost" -ForegroundColor Cyan
} else {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "   ? BUILD FAILED" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Check the errors above and:" -ForegroundColor Yellow
    Write-Host "  1. Make sure all Phase 21 files exist" -ForegroundColor Cyan
    Write-Host "  2. Verify MOC files were generated" -ForegroundColor Cyan
    Write-Host "  3. Check that MainWindow.cpp includes the new headers" -ForegroundColor Cyan
}

Write-Host ""
