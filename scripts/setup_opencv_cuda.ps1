# Download and Setup OpenCV with CUDA for RTX 3050 Ti
# Run this script as Administrator in PowerShell

Write-Host "=== OpenCV CUDA Setup for Naghuma Toolbox ===" -ForegroundColor Cyan
Write-Host ""

$opencvVersion = "4.10.0"
$installPath = "C:\opencv-cuda"

Write-Host "Target: $installPath" -ForegroundColor Green
Write-Host "OpenCV Version: $opencvVersion" -ForegroundColor Green
Write-Host ""

# Check if already exists
if (Test-Path $installPath) {
    Write-Host "WARNING: $installPath already exists!" -ForegroundColor Yellow
    $response = Read-Host "Delete and reinstall? (y/n)"
    if ($response -eq "y") {
        Remove-Item $installPath -Recurse -Force
    } else {
        Write-Host "Aborted." -ForegroundColor Red
        exit
    }
}

# Download OpenCV (you'll need to manually download from GitHub releases)
Write-Host ""
Write-Host "MANUAL STEP REQUIRED:" -ForegroundColor Yellow
Write-Host "1. Go to: https://github.com/opencv/opencv/releases" -ForegroundColor Cyan
Write-Host "2. Download: opencv-$opencvVersion-windows.exe" -ForegroundColor Cyan
Write-Host "3. Extract to: $installPath" -ForegroundColor Cyan
Write-Host ""
Write-Host "OR search Google for: 'opencv cuda prebuilt windows'" -ForegroundColor Magenta
Write-Host ""

$continue = Read-Host "Have you extracted OpenCV to $installPath? (y/n)"
if ($continue -ne "y") {
    Write-Host "Please extract OpenCV first, then re-run this script." -ForegroundColor Yellow
    exit
}

# Verify CUDA DLLs
Write-Host ""
Write-Host "Verifying CUDA DLLs..." -ForegroundColor Yellow

$cudaDll = Get-ChildItem "$installPath\build\x64" -Recurse -Filter "opencv_cuda*.dll" -ErrorAction SilentlyContinue
if ($cudaDll) {
    Write-Host "? Found CUDA DLLs:" -ForegroundColor Green
    $cudaDll | ForEach-Object { Write-Host "  - $($_.FullName)" -ForegroundColor Green }
} else {
    Write-Host "? ERROR: No CUDA DLLs found!" -ForegroundColor Red
    Write-Host "This OpenCV build doesn't have CUDA support." -ForegroundColor Red
    Write-Host "Please download a CUDA-enabled build." -ForegroundColor Red
    exit
}

# Update System PATH
Write-Host ""
Write-Host "Updating System PATH..." -ForegroundColor Yellow

$currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$newPath = "$installPath\build\x64\vc16\bin"

if ($currentPath -notlike "*$newPath*") {
    $updatedPath = "$currentPath;$newPath"
    [Environment]::SetEnvironmentVariable("Path", $updatedPath, "Machine")
    Write-Host "? Added to PATH: $newPath" -ForegroundColor Green
} else {
    Write-Host "? Already in PATH" -ForegroundColor Green
}

# Copy DLLs to Naghuma Toolbox
Write-Host ""
Write-Host "Copying DLLs to Naghuma Toolbox..." -ForegroundColor Yellow

$projectPath = "F:\Naghuma Toolbox\x64\Debug"
if (Test-Path $projectPath) {
    Copy-Item "$installPath\build\x64\vc16\bin\opencv_world*.dll" $projectPath -Force
    Copy-Item "$installPath\build\x64\vc16\bin\opencv_cuda*.dll" $projectPath -Force -ErrorAction SilentlyContinue
    
    # Copy CUDA runtime DLLs
    $cudaPath = "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1\bin"
    Copy-Item "$cudaPath\cudart64_*.dll" $projectPath -Force -ErrorAction SilentlyContinue
    Copy-Item "$cudaPath\cublas64_*.dll" $projectPath -Force -ErrorAction SilentlyContinue
    Copy-Item "$cudaPath\cufft64_*.dll" $projectPath -Force -ErrorAction SilentlyContinue
    
    Write-Host "? DLLs copied to project" -ForegroundColor Green
} else {
    Write-Host "? Project path not found: $projectPath" -ForegroundColor Yellow
    Write-Host "  You'll need to copy DLLs manually after building." -ForegroundColor Yellow
}

# Create Visual Studio property file
Write-Host ""
Write-Host "Creating Visual Studio property sheet..." -ForegroundColor Yellow

$propsContent = @"
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup>
    <OpenCV_DIR>$installPath\build</OpenCV_DIR>
  </PropertyGroup>
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>$installPath\build\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <PreprocessorDefinitions>HAVE_OPENCV_CUDA;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$installPath\build\x64\vc16\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>opencv_world$($opencvVersion.Replace('.',''))d.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
</Project>
"@

$propsFile = "F:\Naghuma Toolbox\opencv_cuda.props"
$propsContent | Out-File $propsFile -Encoding UTF8
Write-Host "? Created: $propsFile" -ForegroundColor Green

# Summary
Write-Host ""
Write-Host "=== SETUP COMPLETE ===" -ForegroundColor Green
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Cyan
Write-Host "1. Open Visual Studio" -ForegroundColor White
Write-Host "2. Right-click project ? Properties" -ForegroundColor White
Write-Host "3. Add Property Sheet: opencv_cuda.props" -ForegroundColor White
Write-Host "   (View ? Property Manager ? Add Existing Property Sheet)" -ForegroundColor Gray
Write-Host "4. OR manually add:" -ForegroundColor White
Write-Host "   - Include: $installPath\build\include" -ForegroundColor Gray
Write-Host "   - Library: $installPath\build\x64\vc16\lib" -ForegroundColor Gray
Write-Host "   - Preprocessor: HAVE_OPENCV_CUDA" -ForegroundColor Gray
Write-Host "5. Build ? Rebuild Solution" -ForegroundColor White
Write-Host "6. Run and check console for:" -ForegroundColor White
Write-Host "   'GPU Acceleration initialized successfully'" -ForegroundColor Green
Write-Host ""
Write-Host "Restart your computer for PATH changes to take effect!" -ForegroundColor Yellow
Write-Host ""
Write-Host "OpenCV CUDA Location: $installPath" -ForegroundColor Magenta
Write-Host "Property File: $propsFile" -ForegroundColor Magenta
Write-Host ""
