Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  OCR Build Fix - Complete Solution" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Verify Qt MOC tool
Write-Host "[1/6] Verifying Qt MOC tool..." -ForegroundColor Yellow
$mocPath = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
if (!(Test-Path $mocPath)) {
    Write-Host "  [ERROR] MOC not found at: $mocPath" -ForegroundColor Red
    Write-Host "  Please verify Qt installation path" -ForegroundColor Yellow
    exit 1
}
Write-Host "  [OK] MOC found" -ForegroundColor Green

# Step 2: Clean old MOC files
Write-Host ""
Write-Host "[2/6] Cleaning old MOC files..." -ForegroundColor Yellow
$oldMocFiles = @("src\moc_OCRDialog.cpp", "x64\Debug\moc_OCRDialog.obj")
foreach ($file in $oldMocFiles) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  Deleted: $file" -ForegroundColor Gray
    }
}
Write-Host "  [OK] Old files cleaned" -ForegroundColor Green

# Step 3: Regenerate MOC file with proper Qt includes
Write-Host ""
Write-Host "[3/6] Regenerating moc_OCRDialog.cpp..." -ForegroundColor Yellow

# Set up Qt include paths for MOC
$qtIncludePath = "C:\Qt\6.7.3\msvc2019_64\include"
$mocArgs = @(
    "include\OCRDialog.h",
    "-o", "src\moc_OCRDialog.cpp",
    "-I", "include",
    "-I", "lib",
    "-I", "$qtIncludePath",
    "-I", "$qtIncludePath\QtCore",
    "-I", "$qtIncludePath\QtGui",
    "-I", "$qtIncludePath\QtWidgets"
)

& $mocPath $mocArgs

if (!(Test-Path "src\moc_OCRDialog.cpp")) {
    Write-Host "  [ERROR] Failed to generate MOC file!" -ForegroundColor Red
    exit 1
}

$mocSize = (Get-Item "src\moc_OCRDialog.cpp").Length
Write-Host "  [OK] Generated moc_OCRDialog.cpp ($mocSize bytes)" -ForegroundColor Green

# Step 4: Verify moc file content
Write-Host ""
Write-Host "[4/6] Verifying MOC file content..." -ForegroundColor Yellow
$mocContent = Get-Content "src\moc_OCRDialog.cpp" -Raw
if ($mocContent -match "OCRDialog::metaObject" -and $mocContent -match "OCRDialog::qt_metacast") {
    Write-Host "  [OK] MOC file contains required Qt meta functions" -ForegroundColor Green
} else {
    Write-Host "  [WARNING] MOC file may be incomplete" -ForegroundColor Yellow
}

# Step 5: Ensure .vcxproj includes moc file
Write-Host ""
Write-Host "[5/6] Verifying project file..." -ForegroundColor Yellow
$projectFile = "Naghuma Toolbox.vcxproj"
$projectContent = Get-Content $projectFile -Raw

if (!($projectContent -match 'src\\moc_OCRDialog\.cpp')) {
    Write-Host "  Adding moc_OCRDialog.cpp to project..." -ForegroundColor Cyan
    
    # Find the ClCompile ItemGroup and add moc file
    $clCompilePattern = '(<ItemGroup>\s*<ClCompile[^>]*Include="src\\[^"]*\.cpp"[^>]*/>)'
    if ($projectContent -match $clCompilePattern) {
        $insertPoint = $Matches[0]
        $newEntry = $insertPoint + "`r`n    <ClCompile Include=`"src\moc_OCRDialog.cpp`" />"
        $projectContent = $projectContent -replace [regex]::Escape($insertPoint), $newEntry
        $projectContent | Set-Content $projectFile -NoNewline
        Write-Host "  [OK] Added to project file" -ForegroundColor Green
    }
} else {
    Write-Host "  [OK] Already in project file" -ForegroundColor Green
}

# Step 6: Clean and rebuild with verbose output
Write-Host ""
Write-Host "[6/6] Building project..." -ForegroundColor Yellow
Write-Host "  Cleaning..." -ForegroundColor Cyan

# Clean first
$cleanOutput = msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Clean /nologo /v:minimal 2>&1
Write-Host "  [OK] Project cleaned" -ForegroundColor Green

Write-Host ""
Write-Host "  Building (this may take 2-3 minutes)..." -ForegroundColor Cyan
Write-Host ""

# Build with detailed output for moc_OCRDialog
$buildOutput = msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Build /nologo /v:normal 2>&1

# Check if moc_OCRDialog.cpp was compiled
$mocCompiled = $false
$buildOutput | ForEach-Object {
    if ($_ -match "moc_OCRDialog\.cpp") {
        Write-Host "  [COMPILE] $_" -ForegroundColor Cyan
        $mocCompiled = $true
    }
}

if (!$mocCompiled) {
    Write-Host ""
    Write-Host "  [WARNING] moc_OCRDialog.cpp was not compiled!" -ForegroundColor Yellow
    Write-Host "  Checking build output for errors..." -ForegroundColor Cyan
}

# Check for linker errors
$linkerErrors = $buildOutput | Select-String "LNK2001|LNK1120"
if ($linkerErrors) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  BUILD FAILED - Linker Errors" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    $linkerErrors | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    Write-Host ""
    
    # Diagnostic information
    Write-Host "Diagnostic Information:" -ForegroundColor Yellow
    Write-Host "  1. MOC file exists: $(Test-Path 'src\moc_OCRDialog.cpp')" -ForegroundColor White
    Write-Host "  2. MOC file size: $((Get-Item 'src\moc_OCRDialog.cpp').Length) bytes" -ForegroundColor White
    Write-Host "  3. In project file: $(if ($projectContent -match 'moc_OCRDialog') {'YES'} else {'NO'})" -ForegroundColor White
    Write-Host ""
    
    Write-Host "SOLUTION: Try manual build in Visual Studio:" -ForegroundColor Yellow
    Write-Host "  1. Open Naghuma Toolbox.sln" -ForegroundColor Cyan
    Write-Host "  2. Right-click 'src\moc_OCRDialog.cpp' in Solution Explorer" -ForegroundColor Cyan
    Write-Host "  3. Select 'Compile'" -ForegroundColor Cyan
    Write-Host "  4. Then rebuild entire project" -ForegroundColor Cyan
    Write-Host ""
    exit 1
}

# Check for general errors
$errors = $buildOutput | Select-String "error"
if ($errors) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  BUILD FAILED" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    $errors | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    Write-Host ""
    exit 1
}

# Success!
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  BUILD SUCCESSFUL!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Verify executable
$exePath = "x64\Debug\Naghuma Toolbox.exe"
if (Test-Path $exePath) {
    $exeSize = [math]::Round((Get-Item $exePath).Length/1MB, 2)
    Write-Host "  [OK] Naghuma Toolbox.exe ($exeSize MB)" -ForegroundColor Green
    
    # Check for moc_OCRDialog in object files
    $objPath = "x64\Debug\moc_OCRDialog.obj"
    if (Test-Path $objPath) {
        Write-Host "  [OK] moc_OCRDialog.obj compiled successfully" -ForegroundColor Green
    } else {
        Write-Host "  [WARNING] moc_OCRDialog.obj not found" -ForegroundColor Yellow
    }
} else {
    Write-Host "  [ERROR] Executable not found!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "OCR is now ready to use!" -ForegroundColor Green
Write-Host ""
Write-Host "Test it:" -ForegroundColor Yellow
Write-Host "  1. Run: .\x64\Debug\`"Naghuma Toolbox.exe`"" -ForegroundColor Cyan
Write-Host "  2. Load an image with text" -ForegroundColor Cyan
Write-Host "  3. Go to: Process -> OCR - Text Recognition..." -ForegroundColor Cyan
Write-Host ""

# Offer to run
Write-Host "Run the application now? (Y/N): " -NoNewline -ForegroundColor Yellow
$response = Read-Host
if ($response -eq 'Y' -or $response -eq 'y') {
    Write-Host ""
    Write-Host "Launching..." -ForegroundColor Green
    Start-Process $exePath
}

Write-Host ""
