# Build and Run Naghuma Toolbox
# Complete build, copy DLLs, and run script

param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    
    [switch]$Clean,
    [switch]$NoBuild,
    [switch]$NoRun
)

$ErrorActionPreference = "Stop"
$solutionPath = "F:\Naghuma Toolbox\Naghuma Toolbox.sln"
$qtBinDir = "C:\Qt\6.7.3\msvc2019_64\bin"
$qtPluginsDir = "C:\Qt\6.7.3\msvc2019_64\plugins"
$outputDir = "F:\Naghuma Toolbox\x64\$Configuration"
$exePath = "$outputDir\Naghuma Toolbox.exe"

Write-Host "`n??????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?       Naghuma Toolbox - Build & Run Script           ?" -ForegroundColor Cyan
Write-Host "??????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "`nConfiguration: $Configuration" -ForegroundColor Yellow
Write-Host "Output: $outputDir`n" -ForegroundColor Yellow

# Step 1: Clean (optional)
if ($Clean -and !$NoBuild) {
    Write-Host "[1/5] Cleaning solution..." -ForegroundColor Cyan
    msbuild $solutionPath /t:Clean /p:Configuration=$Configuration /p:Platform=x64 /nologo /v:minimal
    if ($LASTEXITCODE -ne 0) {
        Write-Host "? Clean failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "? Clean complete`n" -ForegroundColor Green
}

# Step 2: Build
if (!$NoBuild) {
    Write-Host "[2/5] Building solution..." -ForegroundColor Cyan
    $target = if ($Clean) { "Rebuild" } else { "Build" }
    
    msbuild $solutionPath /t:$target /p:Configuration=$Configuration /p:Platform=x64 /nologo /v:minimal
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "`n? Build failed!" -ForegroundColor Red
        exit 1
    }
    Write-Host "`n? Build successful!`n" -ForegroundColor Green
}

# Step 3: Copy Qt DLLs
Write-Host "[3/5] Copying Qt DLLs..." -ForegroundColor Cyan

if ($Configuration -eq "Debug") {
    $dlls = @("Qt6Cored.dll", "Qt6Guid.dll", "Qt6Widgetsd.dll")
    $platformPlugin = "qwindowsd.dll"
} else {
    $dlls = @("Qt6Core.dll", "Qt6Gui.dll", "Qt6Widgets.dll")
    $platformPlugin = "qwindows.dll"
}

foreach ($dll in $dlls) {
    $source = Join-Path $qtBinDir $dll
    $dest = Join-Path $outputDir $dll
    
    if (Test-Path $source) {
        Copy-Item $source $dest -Force -ErrorAction SilentlyContinue
        Write-Host "  ? $dll" -ForegroundColor Green
    } else {
        Write-Host "  ? $dll (not found)" -ForegroundColor Red
    }
}

# Copy Qt platform plugin (IMPORTANT!)
$platformsDir = Join-Path $outputDir "platforms"
if (!(Test-Path $platformsDir)) {
    New-Item -ItemType Directory -Path $platformsDir -Force | Out-Null
}

$platformSource = Join-Path $qtPluginsDir "platforms\$platformPlugin"
$platformDest = Join-Path $platformsDir $platformPlugin

if (Test-Path $platformSource) {
    Copy-Item $platformSource $platformDest -Force -ErrorAction SilentlyContinue
    Write-Host "  ? platforms\$platformPlugin" -ForegroundColor Green
} else {
    Write-Host "  ? platforms\$platformPlugin (not found)" -ForegroundColor Red
}

# Also copy OpenCV DLL
$opencvDll = if ($Configuration -eq "Debug") { "opencv_world430d.dll" } else { "opencv_world430.dll" }
$opencvSource = "F:\OpenCV\opencv\build\x64\vc15\bin\$opencvDll"
$opencvDest = Join-Path $outputDir $opencvDll

if (Test-Path $opencvSource) {
    Copy-Item $opencvSource $opencvDest -Force -ErrorAction SilentlyContinue
    Write-Host "  ? $opencvDll" -ForegroundColor Green
} else {
    Write-Host "  ? $opencvDll (not found - may need manual copy)" -ForegroundColor Yellow
}

Write-Host "`n? DLLs copied`n" -ForegroundColor Green

# Step 4: Create qt.conf (tells Qt where to find plugins)
Write-Host "[4/5] Creating qt.conf..." -ForegroundColor Cyan
$qtConfPath = Join-Path $outputDir "qt.conf"
$qtConfContent = @"
[Paths]
Plugins = .
"@
Set-Content -Path $qtConfPath -Value $qtConfContent -Force
Write-Host "  ? qt.conf created`n" -ForegroundColor Green

# Step 5: Run
if (!$NoRun) {
    if (Test-Path $exePath) {
        Write-Host "[5/5] Launching application..." -ForegroundColor Cyan
        Write-Host "  ? $exePath`n" -ForegroundColor White
        
        Start-Process $exePath
        
        Write-Host "? Application launched!`n" -ForegroundColor Green
    } else {
        Write-Host "? Executable not found: $exePath" -ForegroundColor Red
        exit 1
    }
} else {
    Write-Host "[5/5] Skipping run (use -NoRun to skip)`n" -ForegroundColor Yellow
}

Write-Host "??????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "?                   ALL DONE! ??                        ?" -ForegroundColor Green
Write-Host "??????????????????????????????????????????????????????????`n" -ForegroundColor Green

<#
.SYNOPSIS
    Build and run Naghuma Toolbox

.EXAMPLE
    .\build_and_run.ps1
    # Build Debug and run

.EXAMPLE
    .\build_and_run.ps1 -Configuration Release
    # Build Release and run

.EXAMPLE
    .\build_and_run.ps1 -Clean
    # Clean, rebuild Debug, and run

.EXAMPLE
    .\build_and_run.ps1 -NoBuild
    # Just copy DLLs and run (if already built)

.EXAMPLE
    .\build_and_run.ps1 -NoRun
    # Build and copy DLLs but don't run
#>
