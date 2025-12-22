# ============================================================================
# FIX TESSERACT LIBRARIES - DOWNLOAD DEV PACKAGE
# Enhanced: Checks existing installations, uses project folder
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  TESSERACT DEVELOPMENT LIBRARIES SETUP                " -ForegroundColor Cyan
Write-Host "  (Smart Installation - Checks Existing)               " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Use project directory for vcpkg installation
$projectDir = $PSScriptRoot
$vcpkgPath = Join-Path $projectDir "vcpkg"
$vcpkgInstalled = Join-Path $vcpkgPath "installed\x64-windows"

Write-Host "Project Directory: $projectDir" -ForegroundColor White
Write-Host "vcpkg will be installed to: $vcpkgPath" -ForegroundColor White
Write-Host ""

# ============================================================================
# Step 0: Check for existing Tesseract development files
# ============================================================================

Write-Host "Step 0: Checking for existing Tesseract development files..." -ForegroundColor Yellow
Write-Host ""

$existingPaths = @(
    @{Path = "C:\vcpkg"; Name = "Global vcpkg"},
    @{Path = "$vcpkgPath"; Name = "Local vcpkg (project)"},
    @{Path = "C:\Program Files\Tesseract-OCR\include"; Name = "Tesseract headers (system)"},
    @{Path = "$env:LOCALAPPDATA\vcpkg"; Name = "vcpkg (user folder)"}
)

$foundExisting = $false
$useExistingPath = ""

foreach ($location in $existingPaths) {
    if (Test-Path $location.Path) {
        Write-Host "  ? Found: $($location.Name)" -ForegroundColor Green
        Write-Host "    Path: $($location.Path)" -ForegroundColor Gray
        
        # Check if it has Tesseract
        if (Test-Path "$($location.Path)\installed\x64-windows\include\tesseract\baseapi.h") {
            Write-Host "    ? Tesseract development files found!" -ForegroundColor Green
            $foundExisting = $true
            $useExistingPath = $location.Path
        } elseif (Test-Path "$($location.Path)\include\tesseract\baseapi.h") {
            Write-Host "    ? Tesseract headers found!" -ForegroundColor Green
            $foundExisting = $true
            $useExistingPath = $location.Path
        }
    }
}

Write-Host ""

if ($foundExisting) {
    Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Green
    Write-Host "  EXISTING TESSERACT INSTALLATION FOUND!               " -ForegroundColor Green
    Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Green
    Write-Host ""
    Write-Host "Found at: $useExistingPath" -ForegroundColor White
    Write-Host ""
    
    $useExisting = Read-Host "Use existing installation? (Y/N)"
    
    if ($useExisting -eq "Y" -or $useExisting -eq "y") {
        Write-Host ""
        Write-Host "Using existing installation..." -ForegroundColor Green
        
        # Determine correct paths
        if (Test-Path "$useExistingPath\installed\x64-windows") {
            $vcpkgPath = $useExistingPath
            $vcpkgInstalled = Join-Path $vcpkgPath "installed\x64-windows"
        } else {
            $vcpkgInstalled = $useExistingPath
        }
        
        # Update config and skip installation
        $config = @{
            TesseractPath = "C:\Program Files\Tesseract-OCR"
            TessdataPath = "C:\Program Files\Tesseract-OCR\tessdata"
            IncludePath = "$vcpkgInstalled\include"
            LibPath = "$vcpkgInstalled\lib"
            BinPath = "$vcpkgInstalled\bin"
            Version = "5.3.3"
            InstalledLanguages = @("eng", "ara", "fra", "deu", "spa", "chi_sim", "jpn")
            VcpkgPath = $vcpkgPath
            UsingExisting = $true
        }
        
        $config | ConvertTo-Json | Set-Content "tesseract_config.json"
        
        Write-Host "? Configuration updated with existing installation" -ForegroundColor Green
        Write-Host ""
        Write-Host "Skipping download. Proceeding to verification..." -ForegroundColor Yellow
        Write-Host ""
        
        # Jump to verification
        $skipInstallation = $true
    } else {
        Write-Host ""
        Write-Host "Proceeding with fresh installation..." -ForegroundColor Yellow
        Write-Host ""
        $skipInstallation = $false
    }
} else {
    Write-Host "No existing installation found. Will install fresh." -ForegroundColor Yellow
    Write-Host ""
    
    $confirm = Read-Host "Install vcpkg and Tesseract dev package to project folder? (Y/N)"
    if ($confirm -ne "Y" -and $confirm -ne "y") {
        Write-Host "Cancelled." -ForegroundColor Yellow
        exit 0
    }
    
    $skipInstallation = $false
}

# ============================================================================
# Step 1: Install vcpkg (if needed)
# ============================================================================

if (-not $skipInstallation) {
    Write-Host ""
    Write-Host "Step 1: Installing vcpkg..." -ForegroundColor Yellow
    Write-Host ""

    if (Test-Path $vcpkgPath) {
        Write-Host "? vcpkg directory already exists at: $vcpkgPath" -ForegroundColor Green
        
        # Check if it's properly installed
        if (Test-Path "$vcpkgPath\vcpkg.exe") {
            Write-Host "? vcpkg.exe found - installation appears complete" -ForegroundColor Green
        } else {
            Write-Host "? vcpkg directory exists but vcpkg.exe not found" -ForegroundColor Yellow
            Write-Host "  Bootstrapping vcpkg..." -ForegroundColor Yellow
            
            if (Test-Path "$vcpkgPath\bootstrap-vcpkg.bat") {
                & "$vcpkgPath\bootstrap-vcpkg.bat"
                
                if ($LASTEXITCODE -ne 0) {
                    Write-Host "? Failed to bootstrap vcpkg" -ForegroundColor Red
                    exit 1
                }
            } else {
                Write-Host "? bootstrap-vcpkg.bat not found. Directory may be corrupted." -ForegroundColor Red
                Write-Host "  Please delete: $vcpkgPath" -ForegroundColor Yellow
                Write-Host "  Then run this script again." -ForegroundColor Yellow
                exit 1
            }
        }
    } else {
        Write-Host "Cloning vcpkg from GitHub to project folder..." -ForegroundColor Yellow
        Write-Host "  Target: $vcpkgPath" -ForegroundColor Gray
        Write-Host ""
        
        # Check if git is available
        $gitAvailable = $null -ne (Get-Command git -ErrorAction SilentlyContinue)
        
        if (-not $gitAvailable) {
            Write-Host "? Git is not installed or not in PATH" -ForegroundColor Red
            Write-Host ""
            Write-Host "MANUAL INSTALLATION:" -ForegroundColor Yellow
            Write-Host "  1. Install Git: https://git-scm.com/download/win" -ForegroundColor White
            Write-Host "  2. OR download vcpkg manually:" -ForegroundColor White
            Write-Host "     https://github.com/Microsoft/vcpkg/archive/refs/heads/master.zip" -ForegroundColor White
            Write-Host "  3. Extract to: $vcpkgPath" -ForegroundColor White
            Write-Host "  4. Run: cd `"$vcpkgPath`" && .\bootstrap-vcpkg.bat" -ForegroundColor White
            exit 1
        }
        
        git clone https://github.com/Microsoft/vcpkg.git $vcpkgPath
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "? Failed to clone vcpkg" -ForegroundColor Red
            Write-Host ""
            Write-Host "ALTERNATIVE: Manual download" -ForegroundColor Yellow
            Write-Host "  1. Download: https://github.com/Microsoft/vcpkg/archive/refs/heads/master.zip" -ForegroundColor White
            Write-Host "  2. Extract to: $vcpkgPath" -ForegroundColor White
            Write-Host "  3. Rename folder if needed (should be 'vcpkg' not 'vcpkg-master')" -ForegroundColor White
            Write-Host "  4. Run this script again" -ForegroundColor White
            exit 1
        }
        
        Write-Host "? vcpkg cloned successfully" -ForegroundColor Green
        Write-Host ""
        Write-Host "Bootstrapping vcpkg..." -ForegroundColor Yellow
        & "$vcpkgPath\bootstrap-vcpkg.bat"
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "? Failed to bootstrap vcpkg" -ForegroundColor Red
            exit 1
        }
        
        Write-Host "? vcpkg installed successfully" -ForegroundColor Green
    }

    # ============================================================================
    # Step 2: Install Tesseract via vcpkg
    # ============================================================================

    Write-Host ""
    Write-Host "Step 2: Installing Tesseract via vcpkg..." -ForegroundColor Yellow
    Write-Host ""
    
    # Check if already installed
    if (Test-Path "$vcpkgInstalled\include\tesseract\baseapi.h") {
        Write-Host "? Tesseract appears to be already installed" -ForegroundColor Green
        
        $reinstall = Read-Host "Reinstall anyway? (Y/N)"
        if ($reinstall -ne "Y" -and $reinstall -ne "y") {
            Write-Host "Skipping Tesseract installation." -ForegroundColor Yellow
            $skipInstallation = $true
        }
    }
    
    if (-not $skipInstallation) {
        Write-Host "Installing Tesseract and dependencies..." -ForegroundColor Yellow
        Write-Host "  (This may take 10-30 minutes on first install)" -ForegroundColor Gray
        Write-Host "  Progress will be shown below:" -ForegroundColor Gray
        Write-Host ""

        & "$vcpkgPath\vcpkg.exe" install tesseract:x64-windows

        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "? Failed to install Tesseract" -ForegroundColor Red
            Write-Host ""
            Write-Host "Common issues:" -ForegroundColor Yellow
            Write-Host "  - Network connection problems" -ForegroundColor White
            Write-Host "  - Insufficient disk space (need ~2GB)" -ForegroundColor White
            Write-Host "  - Antivirus blocking vcpkg" -ForegroundColor White
            Write-Host ""
            Write-Host "Try running vcpkg manually:" -ForegroundColor Yellow
            Write-Host "  cd `"$vcpkgPath`"" -ForegroundColor White
            Write-Host "  .\vcpkg.exe install tesseract:x64-windows" -ForegroundColor White
            exit 1
        }

        Write-Host ""
        Write-Host "? Tesseract installed via vcpkg" -ForegroundColor Green
    }

    # ============================================================================
    # Step 3: Integrate with Visual Studio (optional)
    # ============================================================================

    Write-Host ""
    Write-Host "Step 3: Visual Studio integration..." -ForegroundColor Yellow
    Write-Host ""
    
    Write-Host "Note: vcpkg can integrate with Visual Studio globally," -ForegroundColor Gray
    Write-Host "      but we'll configure project-specific paths instead." -ForegroundColor Gray
    Write-Host ""
    
    $integrate = Read-Host "Run vcpkg integrate (global VS integration)? (Y/N)"
    
    if ($integrate -eq "Y" -or $integrate -eq "y") {
        & "$vcpkgPath\vcpkg.exe" integrate install

        if ($LASTEXITCODE -ne 0) {
            Write-Host "? Integration warning (not critical - will use project paths)" -ForegroundColor Yellow
        } else {
            Write-Host "? vcpkg integrated with Visual Studio" -ForegroundColor Green
        }
    } else {
        Write-Host "Skipped. Project will use specific include/library paths." -ForegroundColor Gray
    }
}

# ============================================================================
# Step 4: Verify installation
# ============================================================================

Write-Host ""
Write-Host "Step 4: Verifying installation..." -ForegroundColor Yellow
Write-Host ""

$verificationPassed = $true

# Check headers
if (Test-Path "$vcpkgInstalled\include\tesseract\baseapi.h") {
    Write-Host "? Tesseract headers found" -ForegroundColor Green
    Write-Host "  Location: $vcpkgInstalled\include\tesseract" -ForegroundColor Gray
} else {
    Write-Host "? Tesseract headers NOT FOUND" -ForegroundColor Red
    $verificationPassed = $false
}

# Check libraries
$tesseractLibs = Get-ChildItem "$vcpkgInstalled\lib" -Filter "tesseract*.lib" -ErrorAction SilentlyContinue
if ($tesseractLibs) {
    Write-Host "? Tesseract libraries found ($($tesseractLibs.Count) files)" -ForegroundColor Green
    foreach ($lib in $tesseractLibs) {
        Write-Host "  - $($lib.Name)" -ForegroundColor White
    }
} else {
    Write-Host "? Tesseract libraries NOT FOUND" -ForegroundColor Red
    $verificationPassed = $false
}

# Check Leptonica libraries
$leptonicaLibs = Get-ChildItem "$vcpkgInstalled\lib" -Filter "leptonica*.lib" -ErrorAction SilentlyContinue
if ($leptonicaLibs) {
    Write-Host "? Leptonica libraries found ($($leptonicaLibs.Count) files)" -ForegroundColor Green
    foreach ($lib in $leptonicaLibs) {
        Write-Host "  - $($lib.Name)" -ForegroundColor White
    }
} else {
    Write-Host "? Leptonica libraries not found (may use different name)" -ForegroundColor Yellow
}

# Check DLLs
$dlls = Get-ChildItem "$vcpkgInstalled\bin" -Filter "*.dll" -ErrorAction SilentlyContinue
if ($dlls) {
    Write-Host "? Runtime DLLs found ($($dlls.Count) files)" -ForegroundColor Green
} else {
    Write-Host "? Runtime DLLs not found" -ForegroundColor Yellow
}

if (-not $verificationPassed) {
    Write-Host ""
    Write-Host "? VERIFICATION FAILED" -ForegroundColor Red
    Write-Host "  Installation may be incomplete" -ForegroundColor Yellow
    Write-Host "  Try reinstalling by running this script again" -ForegroundColor Yellow
    exit 1
}

# ============================================================================
# Step 5: Update configuration
# ============================================================================

Write-Host ""
Write-Host "Step 5: Updating configuration..." -ForegroundColor Yellow
Write-Host ""

# Update config file
$config = @{
    TesseractPath = "C:\Program Files\Tesseract-OCR"
    TessdataPath = "C:\Program Files\Tesseract-OCR\tessdata"
    IncludePath = "$vcpkgInstalled\include"
    LibPath = "$vcpkgInstalled\lib"
    BinPath = "$vcpkgInstalled\bin"
    Version = "5.3.3"
    InstalledLanguages = @("eng", "ara", "fra", "deu", "spa", "chi_sim", "jpn")
    VcpkgPath = $vcpkgPath
    ProjectLocal = $true
}

$config | ConvertTo-Json | Set-Content "tesseract_config.json"

Write-Host "? Configuration updated" -ForegroundColor Green

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  TESSERACT DEV LIBRARIES READY                         " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "?? Installation Summary:" -ForegroundColor Yellow
Write-Host "  vcpkg Location:  $vcpkgPath" -ForegroundColor White
Write-Host "  Installation:    $vcpkgInstalled" -ForegroundColor White
Write-Host "  Headers:         $vcpkgInstalled\include" -ForegroundColor White
Write-Host "  Libraries:       $vcpkgInstalled\lib" -ForegroundColor White
Write-Host "  Runtime DLLs:    $vcpkgInstalled\bin" -ForegroundColor White
Write-Host ""

Write-Host "?? Next Steps:" -ForegroundColor Cyan
Write-Host ""
Write-Host "  1. Configure Visual Studio project:" -ForegroundColor White
Write-Host "     .\configure_visual_studio.ps1" -ForegroundColor Cyan
Write-Host "     (Will use paths from tesseract_config.json)" -ForegroundColor Gray
Write-Host ""
Write-Host "  2. Copy runtime DLLs to output directory:" -ForegroundColor White
Write-Host "     xcopy /Y `"$vcpkgInstalled\bin\*.dll`" `"x64\Debug\`"" -ForegroundColor Cyan
Write-Host ""
Write-Host "  3. Rebuild project in Visual Studio:" -ForegroundColor White
Write-Host "     - Close Visual Studio" -ForegroundColor Gray
Write-Host "     - Reopen solution" -ForegroundColor Gray
Write-Host "     - Build ? Rebuild Solution" -ForegroundColor Gray
Write-Host ""
Write-Host "  4. Test OCR feature!" -ForegroundColor White
Write-Host ""

Write-Host "? Development environment ready!" -ForegroundColor Green
Write-Host ""

# Create a quick-copy DLL script
$copyDllScript = @"
@echo off
echo Copying Tesseract runtime DLLs...
xcopy /Y "$vcpkgInstalled\bin\*.dll" "x64\Debug\" >nul 2>&1
if %errorlevel% equ 0 (
    echo Success: DLLs copied to x64\Debug\
) else (
    xcopy /Y "$vcpkgInstalled\bin\*.dll" "x64\Release\" >nul 2>&1
    if %errorlevel% equ 0 (
        echo Success: DLLs copied to x64\Release\
    ) else (
        echo Warning: Could not copy DLLs. Please copy manually.
    )
)
pause
"@

Set-Content "copy_tesseract_dlls.bat" $copyDllScript

Write-Host "Created utility script: copy_tesseract_dlls.bat" -ForegroundColor Gray
Write-Host "  (Run this after building to copy DLLs)" -ForegroundColor Gray
Write-Host ""
