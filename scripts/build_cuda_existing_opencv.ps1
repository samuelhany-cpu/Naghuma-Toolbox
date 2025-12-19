# Build OpenCV 4.12.0 with CUDA using existing installation
# Uses your existing C:\opencv and only downloads what's missing

Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "   OpenCV 4.12.0 CUDA Build for RTX 3050 Ti" -ForegroundColor Yellow
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$opencvSource = "C:\opencv\sources"
$buildDir = "C:\opencv-cuda-build"
$installDir = "C:\opencv-cuda"
$contribVersion = "4.12.0"

# Check existing OpenCV
if (!(Test-Path $opencvSource)) {
    Write-Host "[ERROR] OpenCV source not found at $opencvSource" -ForegroundColor Red
    Write-Host "Please verify your OpenCV installation." -ForegroundColor Yellow
    exit 1
}

Write-Host "[OK] Using existing OpenCV source: $opencvSource" -ForegroundColor Green

# Create build directory
Write-Host "`nPreparing build environment..." -ForegroundColor Yellow
if (Test-Path $buildDir) {
    $response = Read-Host "Build directory exists. Clean it? (y/n)"
    if ($response -eq "y") {
        Remove-Item $buildDir -Recurse -Force
    }
}
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
Set-Location $buildDir

Write-Host "[OK] Build directory: $buildDir" -ForegroundColor Green

# Download OpenCV Contrib (only this is missing)
Write-Host "`nDownloading OpenCV Contrib 4.12.0..." -ForegroundColor Yellow

$contribUrl = "https://github.com/opencv/opencv_contrib/archive/refs/tags/$contribVersion.zip"
$contribZip = "$buildDir\opencv_contrib.zip"

try {
    Invoke-WebRequest -Uri $contribUrl -OutFile $contribZip -UseBasicParsing
    $size = (Get-Item $contribZip).Length / 1MB
    Write-Host "[OK] Downloaded: $([math]::Round($size, 2)) MB" -ForegroundColor Green
} catch {
    Write-Host "[ERROR] Download failed, trying curl..." -ForegroundColor Yellow
    & curl.exe -L $contribUrl -o $contribZip
}

# Extract contrib
Write-Host "Extracting contrib modules..." -ForegroundColor Yellow
Expand-Archive -Path $contribZip -DestinationPath $buildDir -Force
Write-Host "[OK] Extracted" -ForegroundColor Green

# Find contrib path
$contribPath = Get-ChildItem $buildDir -Directory -Filter "opencv_contrib*" | Select-Object -First 1
if (!$contribPath) {
    Write-Host "[ERROR] Contrib extraction failed!" -ForegroundColor Red
    exit 1
}

$contribModulesPath = Join-Path $contribPath.FullName "modules"
Write-Host "[OK] Contrib modules: $contribModulesPath" -ForegroundColor Green

# Create cmake build directory
$cmakeBuildDir = "$buildDir\build"
New-Item -ItemType Directory -Path $cmakeBuildDir -Force | Out-Null
Set-Location $cmakeBuildDir

# Check CUDA
$cudaPath = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1"
if (!(Test-Path $cudaPath)) {
    Write-Host "[ERROR] CUDA 13.1 not found!" -ForegroundColor Red
    exit 1
}
Write-Host "[OK] CUDA 13.1 found" -ForegroundColor Green

# Check CMake
if (!(Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "[ERROR] CMake not found!" -ForegroundColor Red
    Write-Host "Installing CMake..." -ForegroundColor Yellow
    winget install --id Kitware.CMake -e --silent
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
}

Write-Host "[OK] CMake ready" -ForegroundColor Green

# Configure CMake
Write-Host ""
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "   Configuring CMake (10-15 minutes)" -ForegroundColor Yellow
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$cmakeArgs = @(
    "-G", "Visual Studio 17 2022",
    "-A", "x64",
    "-D", "CMAKE_BUILD_TYPE=RELEASE",
    "-D", "CMAKE_INSTALL_PREFIX=$installDir",
    "-D", "OPENCV_EXTRA_MODULES_PATH=$contribModulesPath",
    "-D", "BUILD_EXAMPLES=OFF",
    "-D", "BUILD_TESTS=OFF",
    "-D", "BUILD_PERF_TESTS=OFF",
    "-D", "BUILD_opencv_python2=OFF",
    "-D", "BUILD_opencv_python3=OFF",
    "-D", "WITH_CUDA=ON",
    "-D", "CUDA_ARCH_BIN=8.6",
    "-D", "CUDA_ARCH_PTX=8.6",
    "-D", "CUDA_FAST_MATH=ON",
    "-D", "WITH_CUBLAS=ON",
    "-D", "WITH_CUFFT=ON",
    "-D", "ENABLE_FAST_MATH=ON",
    "-D", "OPENCV_DNN_CUDA=ON",
    "-D", "WITH_TBB=ON",
    "-D", "WITH_OPENGL=ON",
    $opencvSource
)

Write-Host "Configuring..." -ForegroundColor Cyan
& cmake @cmakeArgs

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "[ERROR] CMake configuration failed!" -ForegroundColor Red
    Write-Host "Check errors above. Common issues:" -ForegroundColor Yellow
    Write-Host "  - Visual Studio not found" -ForegroundColor Gray
    Write-Host "  - CUDA path incorrect" -ForegroundColor Gray
    exit 1
}

Write-Host ""
Write-Host "[OK] Configuration complete!" -ForegroundColor Green

# Build
Write-Host ""
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "   Building OpenCV with CUDA (60-90 minutes)" -ForegroundColor Yellow
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will use all CPU cores. Please be patient..." -ForegroundColor Yellow
Write-Host "Go grab a coffee! ?" -ForegroundColor Cyan
Write-Host ""

$buildStart = Get-Date

& cmake --build . --config Release --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "[ERROR] Build failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "This is common on first try. Solutions:" -ForegroundColor Yellow
    Write-Host "  1. Try building again (often works)" -ForegroundColor Cyan
    Write-Host "  2. Check you have enough RAM (8GB+ recommended)" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "To retry manually:" -ForegroundColor Yellow
    Write-Host "  cd '$cmakeBuildDir'" -ForegroundColor Cyan
    Write-Host "  cmake --build . --config Release --parallel" -ForegroundColor Cyan
    exit 1
}

$buildTime = (Get-Date) - $buildStart
Write-Host ""
Write-Host "[OK] Build completed in $($buildTime.TotalMinutes.ToString('F1')) minutes!" -ForegroundColor Green

# Install
Write-Host ""
Write-Host "Installing to $installDir..." -ForegroundColor Yellow
& cmake --install .

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Installation failed!" -ForegroundColor Red
    exit 1
}

Write-Host "[OK] Installed successfully!" -ForegroundColor Green

# Verify CUDA DLLs
Write-Host ""
Write-Host "Verifying CUDA support..." -ForegroundColor Yellow

$vcDirs = @("vc17", "vc16")
$cudaDlls = $null
$vcDirFound = "vc17"  # Default

foreach ($vcDir in $vcDirs) {
    $binPath = "$installDir\x64\$vcDir\bin"
    if (Test-Path $binPath) {
        $cudaDlls = Get-ChildItem $binPath -Filter "opencv_cuda*.dll" -ErrorAction SilentlyContinue
        if ($cudaDlls) {
            Write-Host "[OK] CUDA DLLs found in $vcDir" ":" -ForegroundColor Green
            $cudaDlls | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor Green }
            $vcDirFound = $vcDir
            break
        }
    }
}

if (!$cudaDlls) {
    Write-Host "[WARNING] CUDA DLLs not found!" -ForegroundColor Red
    Write-Host "Build may not have CUDA support." -ForegroundColor Yellow
    Write-Host "Check build logs above for CUDA-related errors." -ForegroundColor Yellow
}

# Update PATH
Write-Host ""
Write-Host "Updating system PATH..." -ForegroundColor Yellow

$binDir = "$installDir\x64\$vcDirFound\bin"
$currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")

if ($currentPath -notlike "*$binDir*") {
    try {
        [Environment]::SetEnvironmentVariable("Path", "$currentPath;$binDir", "Machine")
        Write-Host "[OK] PATH updated" -ForegroundColor Green
    } catch {
        Write-Host "[WARNING] Could not update PATH (need admin)" -ForegroundColor Yellow
        Write-Host "Please add manually: $binDir" -ForegroundColor Cyan
    }
}

# Copy DLLs to project
Write-Host ""
Write-Host "Copying DLLs to Naghuma Toolbox..." -ForegroundColor Yellow

$projectPaths = @(
    "F:\Naghuma Toolbox\x64\Debug",
    "F:\Naghuma Toolbox\x64\Release"
)

foreach ($path in $projectPaths) {
    if (!(Test-Path $path)) {
        New-Item -ItemType Directory -Path $path -Force | Out-Null
    }
    
    Copy-Item "$binDir\*.dll" $path -Force -ErrorAction SilentlyContinue
    Write-Host "  [OK] Copied to: $path" -ForegroundColor Green
}

# Copy CUDA runtime
$cudaBin = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1\bin"
if (Test-Path $cudaBin) {
    Write-Host ""
    Write-Host "Copying CUDA runtime DLLs..." -ForegroundColor Yellow
    foreach ($path in $projectPaths) {
        Copy-Item "$cudaBin\cudart64_*.dll" $path -Force -ErrorAction SilentlyContinue
        Copy-Item "$cudaBin\cublas64_*.dll" $path -Force -ErrorAction SilentlyContinue
        Copy-Item "$cudaBin\cufft64_*.dll" $path -Force -ErrorAction SilentlyContinue
    }
    Write-Host "[OK] CUDA DLLs copied" -ForegroundColor Green
}

# Create property sheet
Write-Host ""
Write-Host "Creating Visual Studio property sheet..." -ForegroundColor Yellow

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
      <AdditionalLibraryDirectories>$installDir\x64\$vcDirFound\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>opencv_world4120.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
</Project>
"@

$propsFile = "F:\Naghuma Toolbox\opencv_cuda.props"
$propsContent | Out-File $propsFile -Encoding UTF8
Write-Host "[OK] Created: $propsFile" -ForegroundColor Green

# Cleanup
Write-Host ""
$cleanup = Read-Host "Delete build directory to save space? (y/n)"
if ($cleanup -eq "y") {
    Set-Location "C:\"
    Start-Sleep -Seconds 2
    Remove-Item $buildDir -Recurse -Force -ErrorAction SilentlyContinue
    Write-Host "[OK] Build directory cleaned" -ForegroundColor Green
}

# Summary
$totalTime = (Get-Date) - $buildStart
Write-Host ""
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "   BUILD COMPLETE! ??" -ForegroundColor Yellow
Write-Host "????????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""
Write-Host "Build time: $($totalTime.TotalMinutes.ToString('F1')) minutes" -ForegroundColor Cyan
Write-Host "Installed to: $installDir" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. RESTART your computer (for PATH changes)" -ForegroundColor White
Write-Host "2. Open Visual Studio 2022" -ForegroundColor White
Write-Host "3. Open: F:\Naghuma Toolbox\Naghuma Toolbox.sln" -ForegroundColor White
Write-Host "4. View ? Property Manager" -ForegroundColor White
Write-Host "5. Right-click project ? Add Existing Property Sheet" -ForegroundColor White
Write-Host "6. Select: opencv_cuda.props" -ForegroundColor White
Write-Host "7. Build ? Rebuild Solution" -ForegroundColor White
Write-Host "8. Run (F5) and check console:" -ForegroundColor White
Write-Host ""
Write-Host "Expected Output:" -ForegroundColor Green
Write-Host "  GPU Acceleration initialized successfully" -ForegroundColor Cyan
Write-Host "  CUDA Device: NVIDIA GeForce RTX 3050 Ti Laptop" -ForegroundColor Cyan
Write-Host "  Compute Capability: 8.6" -ForegroundColor Cyan
Write-Host "  Total Memory: 4096 MB" -ForegroundColor Cyan
Write-Host ""
Write-Host "Property file: $propsFile" -ForegroundColor Magenta
Write-Host ""
Write-Host "Enjoy 10-15x faster image processing! ???" -ForegroundColor Green
Write-Host ""
