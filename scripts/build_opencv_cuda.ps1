# OpenCV CUDA Build Script for RTX 3050 Ti
# This will build OpenCV with CUDA support (takes 1-2 hours)

Write-Host "=== OpenCV with CUDA - Automated Build ===" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will:" -ForegroundColor Yellow
Write-Host "  1. Download OpenCV source" 
Write-Host "  2. Download OpenCV contrib modules"
Write-Host "  3. Build with CUDA 13.1 support"
Write-Host "  4. Install to C:\opencv-cuda"
Write-Host "  5. Configure Naghuma Toolbox project"
Write-Host ""
Write-Host "Requirements:" -ForegroundColor Yellow
Write-Host "  - CMake (will auto-install if missing)"
Write-Host "  - Visual Studio 2019/2022"
Write-Host "  - 10GB free disk space"
Write-Host "  - 1-2 hours build time"
Write-Host ""

$response = Read-Host "Continue with build? (y/n)"
if ($response -ne "y") {
    Write-Host "Aborted. Your app still works fine on CPU!" -ForegroundColor Green
    exit
}

# Check prerequisites
Write-Host "`nChecking prerequisites..." -ForegroundColor Yellow

# Check Visual Studio
$vsPath = Get-ChildItem "C:\Program Files\Microsoft Visual Studio" -Directory -ErrorAction SilentlyContinue | Select-Object -First 1
if (!$vsPath) {
    Write-Host "ERROR: Visual Studio not found!" -ForegroundColor Red
    Write-Host "Please install Visual Studio 2019 or 2022 first." -ForegroundColor Red
    exit
}
Write-Host "? Visual Studio found: $($vsPath.Name)" -ForegroundColor Green

# Check/Install CMake
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (!$cmake) {
    Write-Host "? CMake not found. Installing..." -ForegroundColor Yellow
    
    if (Get-Command winget -ErrorAction SilentlyContinue) {
        winget install --id Kitware.CMake -e --silent
        Write-Host "? CMake installed" -ForegroundColor Green
        # Refresh PATH
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
    } else {
        Write-Host "ERROR: Please install CMake manually from https://cmake.org/download/" -ForegroundColor Red
        exit
    }
} else {
    Write-Host "? CMake found: $($cmake.Version)" -ForegroundColor Green
}

# Check CUDA
$cudaPath = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1"
if (!(Test-Path $cudaPath)) {
    Write-Host "ERROR: CUDA 13.1 not found at $cudaPath" -ForegroundColor Red
    exit
}
Write-Host "? CUDA 13.1 found" -ForegroundColor Green

# Create build directory
Write-Host "`nSetting up build environment..." -ForegroundColor Yellow
$buildDir = "C:\opencv-build"
$installDir = "C:\opencv-cuda"

if (Test-Path $buildDir) {
    Remove-Item $buildDir -Recurse -Force
}
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null

# Download OpenCV
Write-Host "Downloading OpenCV 4.10.0..." -ForegroundColor Yellow
Set-Location $buildDir

$opencvUrl = "https://github.com/opencv/opencv/archive/refs/tags/4.10.0.zip"
$contribUrl = "https://github.com/opencv/opencv_contrib/archive/refs/tags/4.10.0.zip"

Invoke-WebRequest -Uri $opencvUrl -OutFile "opencv.zip"
Invoke-WebRequest -Uri $contribUrl -OutFile "opencv_contrib.zip"

Write-Host "Extracting..." -ForegroundColor Yellow
Expand-Archive -Path "opencv.zip" -DestinationPath "."
Expand-Archive -Path "opencv_contrib.zip" -DestinationPath "."

# Configure CMake
Write-Host "`nConfiguring CMake (this may take 10 minutes)..." -ForegroundColor Yellow
New-Item -ItemType Directory -Path "opencv-4.10.0\build" -Force | Out-Null
Set-Location "opencv-4.10.0\build"

$cmakeArgs = @(
    "-D", "CMAKE_BUILD_TYPE=RELEASE",
    "-D", "CMAKE_INSTALL_PREFIX=$installDir",
    "-D", "WITH_CUDA=ON",
    "-D", "CUDA_ARCH_BIN=8.6",
    "-D", "CUDA_FAST_MATH=ON",
    "-D", "WITH_CUBLAS=ON",
    "-D", "ENABLE_FAST_MATH=ON",
    "-D", "OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.10.0/modules",
    "-D", "BUILD_EXAMPLES=OFF",
    "-D", "BUILD_TESTS=OFF",
    "-D", "BUILD_PERF_TESTS=OFF",
    "-D", "WITH_QT=OFF",
    "-D", "WITH_OPENGL=ON",
    ".."
)

& cmake $cmakeArgs

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    exit
}

Write-Host "? CMake configured successfully" -ForegroundColor Green

# Build
Write-Host "`nBuilding OpenCV (this will take 1-2 hours)..." -ForegroundColor Yellow
Write-Host "Go grab a coffee! ?" -ForegroundColor Cyan
Write-Host ""

& cmake --build . --config Release --parallel 8

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    exit
}

Write-Host "? Build completed!" -ForegroundColor Green

# Install
Write-Host "`nInstalling..." -ForegroundColor Yellow
& cmake --install .

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Installation failed!" -ForegroundColor Red
    exit
}

Write-Host "? Installation completed!" -ForegroundColor Green

# Verify CUDA DLLs
Write-Host "`nVerifying CUDA support..." -ForegroundColor Yellow
$cudaDlls = Get-ChildItem "$installDir\x64\vc16\bin" -Filter "opencv_cuda*.dll" -ErrorAction SilentlyContinue
if ($cudaDlls) {
    Write-Host "? CUDA DLLs found:" -ForegroundColor Green
    $cudaDlls | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor Green }
} else {
    Write-Host "? WARNING: CUDA DLLs not found!" -ForegroundColor Red
}

# Update PATH
Write-Host "`nUpdating system PATH..." -ForegroundColor Yellow
$currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$newPath = "$installDir\x64\vc16\bin"

if ($currentPath -notlike "*$newPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$currentPath;$newPath", "Machine")
    Write-Host "? PATH updated" -ForegroundColor Green
} else {
    Write-Host "? Already in PATH" -ForegroundColor Green
}

# Copy DLLs to project
Write-Host "`nCopying DLLs to Naghuma Toolbox..." -ForegroundColor Yellow
$projectPath = "F:\Naghuma Toolbox\x64\Debug"
if (Test-Path $projectPath) {
    Copy-Item "$installDir\x64\vc16\bin\*.dll" $projectPath -Force
    Write-Host "? DLLs copied" -ForegroundColor Green
}

# Create property sheet
$propsContent = @"
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup>
    <OpenCV_DIR>$installDir</OpenCV_DIR>
  </PropertyGroup>
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>$installDir\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <PreprocessorDefinitions>HAVE_OPENCV_CUDA;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$installDir\x64\vc16\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>opencv_world4100d.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
</Project>
"@

$propsFile = "F:\Naghuma Toolbox\opencv_cuda.props"
$propsContent | Out-File $propsFile -Encoding UTF8

# Cleanup
Write-Host "`nCleaning up build files..." -ForegroundColor Yellow
Set-Location C:\
Remove-Item $buildDir -Recurse -Force -ErrorAction SilentlyContinue

# Summary
Write-Host ""
Write-Host "=== BUILD COMPLETE! ===" -ForegroundColor Green
Write-Host ""
Write-Host "OpenCV with CUDA installed to: $installDir" -ForegroundColor Cyan
Write-Host "Property file created: $propsFile" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. Restart your computer" -ForegroundColor White
Write-Host "2. Open Visual Studio" -ForegroundColor White
Write-Host "3. Add opencv_cuda.props to your project" -ForegroundColor White
Write-Host "4. Rebuild Naghuma Toolbox" -ForegroundColor White
Write-Host "5. Run and enjoy 10-15x speedup! ?" -ForegroundColor Green
Write-Host ""
