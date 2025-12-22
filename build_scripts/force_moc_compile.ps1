Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Force MOC File Compilation" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Regenerate MOC with all includes
Write-Host "[1/4] Regenerating MOC file..." -ForegroundColor Yellow
$moc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
$qtInclude = "C:\Qt\6.7.3\msvc2019_64\include"

# Delete old MOC
if (Test-Path "src\moc_OCRDialog.cpp") {
    Remove-Item "src\moc_OCRDialog.cpp" -Force
}

# Generate with full include paths
$mocArgs = @(
    "-I", "include",
    "-I", "lib",
    "-I", "$qtInclude",
    "-I", "$qtInclude\QtCore",
    "-I", "$qtInclude\QtWidgets",
    "include\OCRDialog.h",
    "-o", "src\moc_OCRDialog.cpp"
)

& $moc $mocArgs

if (Test-Path "src\moc_OCRDialog.cpp") {
    Write-Host "  [OK] moc_OCRDialog.cpp generated" -ForegroundColor Green
} else {
    Write-Host "  [ERROR] Failed to generate MOC!" -ForegroundColor Red
    exit 1
}

# Step 2: Check .vcxproj has the file
Write-Host ""
Write-Host "[2/4] Checking project file..." -ForegroundColor Yellow
$vcxproj = Get-Content "Naghuma Toolbox.vcxproj" -Raw
if ($vcxproj -notmatch 'moc_OCRDialog\.cpp') {
    Write-Host "  [ERROR] moc_OCRDialog.cpp not in project!" -ForegroundColor Red
    Write-Host "  Run: .\add_ocr_files_to_project.ps1" -ForegroundColor Yellow
    exit 1
}
Write-Host "  [OK] moc_OCRDialog.cpp is in project" -ForegroundColor Green

# Step 3: Touch the file to force recompilation
Write-Host ""
Write-Host "[3/4] Forcing recompilation..." -ForegroundColor Yellow
(Get-Item "src\moc_OCRDialog.cpp").LastWriteTime = Get-Date
Write-Host "  [OK] File timestamp updated" -ForegroundColor Green

# Step 4: Compile ONLY moc_OCRDialog.cpp first
Write-Host ""
Write-Host "[4/4] Compiling moc_OCRDialog.cpp..." -ForegroundColor Yellow

# Try to compile just this one file
$compileCmd = @"
cl.exe /c /I"include" /I"lib" /I"C:\Qt\6.7.3\msvc2019_64\include" /I"C:\Qt\6.7.3\msvc2019_64\include\QtCore" /I"C:\Qt\6.7.3\msvc2019_64\include\QtWidgets" /Fo"x64\Debug\moc_OCRDialog.obj" "src\moc_OCRDialog.cpp"
"@

Write-Host "  Attempting direct compilation..." -ForegroundColor Cyan

# Set up Visual Studio environment
$vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if (Test-Path $vsPath) {
    # Create a temporary batch file to compile
    $tempBat = "temp_compile_moc.bat"
    @"
@echo off
call "$vsPath" > nul 2>&1
cd /d "$PWD"
$compileCmd
"@ | Out-File $tempBat -Encoding ASCII
    
    & cmd /c $tempBat 2>&1
    Remove-Item $tempBat -Force
    
    if (Test-Path "x64\Debug\moc_OCRDialog.obj") {
        Write-Host "  [OK] moc_OCRDialog.obj created!" -ForegroundColor Green
    } else {
        Write-Host "  [WARNING] Direct compilation failed, trying full build..." -ForegroundColor Yellow
    }
}

# Now do full build
Write-Host ""
Write-Host "Building full project..." -ForegroundColor Yellow
Write-Host ""

msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Clean /nologo /v:minimal | Out-Null
$buildResult = msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Build /nologo /v:minimal 2>&1

# Check for linker errors about OCRDialog
$ocrErrors = $buildResult | Select-String "OCRDialog.*LNK2001|LNK1120"
if ($ocrErrors) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  STILL FAILING - MOC FILE NOT LINKING" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    
    Write-Host "The MOC file exists but is not being linked." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "MANUAL FIX REQUIRED:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Option 1: Visual Studio UI" -ForegroundColor Cyan
    Write-Host "  1. Open Naghuma Toolbox.sln in Visual Studio" -ForegroundColor White
    Write-Host "  2. In Solution Explorer, find src\moc_OCRDialog.cpp" -ForegroundColor White
    Write-Host "  3. Right-click -> Exclude From Project" -ForegroundColor White
    Write-Host "  4. Right-click -> Include In Project" -ForegroundColor White
    Write-Host "  5. Right-click -> Compile" -ForegroundColor White
    Write-Host "  6. Build -> Rebuild Solution" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 2: Use filters file" -ForegroundColor Cyan
    Write-Host "  1. Edit Naghuma Toolbox.vcxproj.filters" -ForegroundColor White
    Write-Host "  2. Add moc_OCRDialog.cpp to 'Source Files' filter" -ForegroundColor White
    Write-Host "  3. Save and reload project in Visual Studio" -ForegroundColor White
    Write-Host ""
    
    # Show what's in the project file
    Write-Host "Current project file entry:" -ForegroundColor Yellow
    $vcxproj | Select-String "moc_OCRDialog" | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }
    Write-Host ""
    
    exit 1
}

# Check for success
$errors = $buildResult | Select-String "error"
if ($errors) {
    Write-Host ""
    Write-Host "Build failed with other errors:" -ForegroundColor Red
    $errors | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "  BUILD SUCCESSFUL!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

if (Test-Path "x64\Debug\Naghuma Toolbox.exe") {
    Write-Host "Ready to test OCR!" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Run: .\x64\Debug\`"Naghuma Toolbox.exe`"" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "WARNING: Executable not found!" -ForegroundColor Yellow
}
