Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  Switching to Release Build & Copying DLLs" -ForegroundColor Yellow
Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Step 1: Ensure Release folder exists
Write-Host "1. Preparing Release folder..." -ForegroundColor Yellow
if (!(Test-Path "x64\Release")) {
    New-Item "x64\Release" -ItemType Directory -Force | Out-Null
    Write-Host "   ? Created x64\Release folder" -ForegroundColor Green
} else {
    Write-Host "   ? Release folder exists" -ForegroundColor Green
}

# Step 2: Copy ALL DLLs from Debug to Release
Write-Host ""
Write-Host "2. Copying DLLs from Debug to Release..." -ForegroundColor Yellow
$debugDlls = Get-ChildItem "x64\Debug\*.dll"
$copied = 0
$skipped = 0

foreach ($dll in $debugDlls) {
    $dst = "x64\Release\$($dll.Name)"
    if (!(Test-Path $dst)) {
        Copy-Item $dll.FullName $dst -Force
        Write-Host "   ? $($dll.Name)" -ForegroundColor Green
        $copied++
    } else {
        $skipped++
    }
}

Write-Host ""
Write-Host "   Copied: $copied DLLs" -ForegroundColor Green
if ($skipped -gt 0) {
    Write-Host "   Skipped (already present): $skipped DLLs" -ForegroundColor Gray
}

# Step 3: Build Release configuration
Write-Host ""
Write-Host "3. Building Release configuration..." -ForegroundColor Yellow
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

if (Test-Path $msbuild) {
    Write-Host "   Building..." -ForegroundColor Cyan
    & $msbuild "Naghuma Toolbox.sln" /p:Configuration=Release /p:Platform=x64 /t:Build /nologo /v:minimal
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "   ? Build successful!" -ForegroundColor Green
    } else {
        Write-Host "   ??  Build completed with warnings/errors" -ForegroundColor Yellow
        Write-Host "   Check output above for details" -ForegroundColor Yellow
    }
} else {
    Write-Host "   ??  MSBuild not found at expected location" -ForegroundColor Yellow
    Write-Host "   Please build manually in Visual Studio:" -ForegroundColor Cyan
    Write-Host "   1. Open Naghuma Toolbox.sln" -ForegroundColor Cyan
    Write-Host "   2. Change 'Debug' to 'Release' in toolbar" -ForegroundColor Cyan
    Write-Host "   3. Press Ctrl+Shift+B to build" -ForegroundColor Cyan
}

# Step 4: Verify Release build
Write-Host ""
Write-Host "4. Verifying Release build..." -ForegroundColor Yellow

$exePath = "x64\Release\Naghuma Toolbox.exe"
$dllCount = (Get-ChildItem "x64\Release\*.dll" -ErrorAction SilentlyContinue | Measure-Object).Count

if (Test-Path $exePath) {
    Write-Host "   ? EXE exists: Naghuma Toolbox.exe" -ForegroundColor Green
    Write-Host "   ? DLLs present: $dllCount" -ForegroundColor Green
    
    # Check critical DLLs
    $critical = @("opencv_world430d.dll", "Qt6Cored.dll", "tesseract55.dll")
    $allPresent = $true
    foreach ($dll in $critical) {
        if (!(Test-Path "x64\Release\$dll")) {
            Write-Host "   ? Missing: $dll" -ForegroundColor Red
            $allPresent = $false
        }
    }
    
    if ($allPresent) {
        Write-Host ""
        Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
        Write-Host "??? RELEASE BUILD READY TO RUN! ???" -ForegroundColor Green
        Write-Host "???????????????????????????????????????????????????????" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Run the application:" -ForegroundColor Yellow
        Write-Host "  cd x64\Release" -ForegroundColor Cyan
        Write-Host "  .\Naghuma` Toolbox.exe" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Or from current directory:" -ForegroundColor Yellow
        Write-Host "  .\x64\Release\Naghuma` Toolbox.exe" -ForegroundColor Cyan
        Write-Host ""
        
        # Offer to run it
        Write-Host "Would you like to run it now? (Y/N): " -NoNewline -ForegroundColor Yellow
        $response = Read-Host
        if ($response -eq 'Y' -or $response -eq 'y') {
            Write-Host ""
            Write-Host "Launching application..." -ForegroundColor Green
            Start-Process $exePath
        }
    } else {
        Write-Host ""
        Write-Host "??  Some critical DLLs are missing" -ForegroundColor Yellow
        Write-Host "Re-copy DLLs from Debug folder" -ForegroundColor Yellow
    }
} else {
    Write-Host "   ? EXE not found: $exePath" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please build in Visual Studio:" -ForegroundColor Yellow
    Write-Host "1. Open Naghuma Toolbox.sln" -ForegroundColor Cyan
    Write-Host "2. Select 'Release' configuration" -ForegroundColor Cyan
    Write-Host "3. Press Ctrl+Shift+B" -ForegroundColor Cyan
    Write-Host "4. Run this script again" -ForegroundColor Cyan
}

Write-Host ""
