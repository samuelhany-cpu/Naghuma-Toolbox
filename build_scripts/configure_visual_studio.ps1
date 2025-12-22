# ============================================================================
# CONFIGURE VISUAL STUDIO FOR TESSERACT
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  CONFIGURING VISUAL STUDIO PROJECT                    " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Load configuration
$configFile = "tesseract_config.json"

if (-not (Test-Path $configFile)) {
    Write-Host "? Configuration file not found!" -ForegroundColor Red
    Write-Host "  Please run: .\install_tesseract_dev.ps1 first" -ForegroundColor Yellow
    exit 1
}

$config = Get-Content $configFile | ConvertFrom-Json
Write-Host "? Loaded Tesseract configuration" -ForegroundColor Green
Write-Host ""

# Project file
$projectFile = "Naghuma Toolbox.vcxproj"

if (-not (Test-Path $projectFile)) {
    Write-Host "? Project file not found: $projectFile" -ForegroundColor Red
    exit 1
}

# Create backup
$backupFile = "$projectFile.before_tesseract_$(Get-Date -Format 'yyyyMMdd_HHmmss')"
Copy-Item $projectFile $backupFile -Force
Write-Host "? Created project backup: $backupFile" -ForegroundColor Gray
Write-Host ""

# Load project XML
[xml]$proj = Get-Content $projectFile

Write-Host "Step 1: Detecting actual library files..." -ForegroundColor Yellow
Write-Host ""

$tesseractLib = $config.LibPath

# Detect actual library files
$tesseractLibFiles = Get-ChildItem $tesseractLib -Filter "tesseract*.lib" -ErrorAction SilentlyContinue
$leptonicaLibFiles = Get-ChildItem $tesseractLib -Filter "leptonica*.lib" -ErrorAction SilentlyContinue

if ($tesseractLibFiles) {
    $tesseractLibName = $tesseractLibFiles[0].Name
    Write-Host "  ? Found Tesseract library: $tesseractLibName" -ForegroundColor Green
} else {
    Write-Host "  ? Tesseract library not found!" -ForegroundColor Red
    exit 1
}

if ($leptonicaLibFiles) {
    $leptonicaLibName = $leptonicaLibFiles[0].Name
    Write-Host "  ? Found Leptonica library: $leptonicaLibName" -ForegroundColor Green
} else {
    Write-Host "  ? Leptonica library not found (may not be needed)" -ForegroundColor Yellow
    $leptonicaLibName = ""
}

# Build library list
if ($leptonicaLibName) {
    $tesseractLibs = "$tesseractLibName;$leptonicaLibName"
} else {
    $tesseractLibs = $tesseractLibName
}

Write-Host ""
Write-Host "  Libraries to link: $tesseractLibs" -ForegroundColor White
Write-Host ""

Write-Host "Step 2: Adding include directories..." -ForegroundColor Yellow

$tesseractInclude = $config.IncludePath

# Find or create AdditionalIncludeDirectories elements
$propertyGroups = $proj.Project.ItemDefinitionGroup

$includeAdded = $false
foreach ($group in $propertyGroups) {
    if ($group.ClCompile) {
        $currentIncludes = $group.ClCompile.AdditionalIncludeDirectories
        
        if (-not $currentIncludes) {
            $currentIncludes = ""
        }
        
        # Remove old Tesseract includes if present
        $currentIncludes = $currentIncludes -replace "C:\\Program Files\\Tesseract-OCR\\include[^;]*;?", ""
        
        # Add new Tesseract includes if not already present
        if ($currentIncludes -notmatch [regex]::Escape($tesseractInclude)) {
            if ($currentIncludes) {
                $group.ClCompile.AdditionalIncludeDirectories = "$tesseractInclude;$currentIncludes"
            } else {
                $group.ClCompile.AdditionalIncludeDirectories = "$tesseractInclude;%(AdditionalIncludeDirectories)"
            }
            $includeAdded = $true
        }
    }
}

if ($includeAdded) {
    Write-Host "  ? Added include directories to all configurations" -ForegroundColor Green
} else {
    Write-Host "  ? Include directories already present" -ForegroundColor Gray
}

Write-Host ""
Write-Host "Step 3: Adding library directories..." -ForegroundColor Yellow

$libAdded = $false
foreach ($group in $propertyGroups) {
    if ($group.Link) {
        $currentLibDirs = $group.Link.AdditionalLibraryDirectories
        
        if (-not $currentLibDirs) {
            $currentLibDirs = ""
        }
        
        # Remove old Tesseract lib paths if present
        $currentLibDirs = $currentLibDirs -replace "C:\\Program Files\\Tesseract-OCR\\lib[^;]*;?", ""
        
        # Add Tesseract library directory if not already present
        if ($currentLibDirs -notmatch [regex]::Escape($tesseractLib)) {
            if ($currentLibDirs) {
                $group.Link.AdditionalLibraryDirectories = "$tesseractLib;$currentLibDirs"
            } else {
                $group.Link.AdditionalLibraryDirectories = "$tesseractLib;%(AdditionalLibraryDirectories)"
            }
            $libAdded = $true
        }
    }
}

if ($libAdded) {
    Write-Host "  ? Added library directories to all configurations" -ForegroundColor Green
} else {
    Write-Host "  ? Library directories already present" -ForegroundColor Gray
}

Write-Host ""
Write-Host "Step 4: Adding Tesseract libraries..." -ForegroundColor Yellow

$depsAdded = $false
foreach ($group in $propertyGroups) {
    if ($group.Link) {
        $currentDeps = $group.Link.AdditionalDependencies
        
        if (-not $currentDeps) {
            $currentDeps = ""
        }
        
        # Remove old Tesseract library references
        $currentDeps = $currentDeps -replace "tesseract\d+\.lib;?", ""
        $currentDeps = $currentDeps -replace "leptonica[^;]*\.lib;?", ""
        
        # Clean up double semicolons
        $currentDeps = $currentDeps -replace ";;+", ";"
        $currentDeps = $currentDeps.TrimStart(';').TrimEnd(';')
        
        # Add new Tesseract libraries
        if ($currentDeps) {
            $group.Link.AdditionalDependencies = "$tesseractLibs;$currentDeps"
        } else {
            $group.Link.AdditionalDependencies = "$tesseractLibs;%(AdditionalDependencies)"
        }
        $depsAdded = $true
    }
}

if ($depsAdded) {
    Write-Host "  ? Added Tesseract libraries: $tesseractLibs" -ForegroundColor Green
} else {
    Write-Host "  ? Libraries already configured" -ForegroundColor Gray
}

# Save project file
$proj.Save($projectFile)

Write-Host ""
Write-Host "Step 5: Creating utility scripts..." -ForegroundColor Yellow
Write-Host ""

# Create a batch file to set up environment before running
$envBatch = @"
@echo off
REM Set Tesseract environment variables
set TESSDATA_PREFIX=$($config.TessdataPath)
set PATH=$($config.BinPath);%PATH%

echo Tesseract environment configured
echo TESSDATA_PREFIX=%TESSDATA_PREFIX%
echo.

REM Run the application
"%~dp0x64\Debug\Naghuma Toolbox.exe"
"@

Set-Content "run_with_tesseract.bat" $envBatch
Write-Host "? Created run_with_tesseract.bat" -ForegroundColor Green

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  VISUAL STUDIO CONFIGURED                              " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "?? Configuration Applied:" -ForegroundColor Yellow
Write-Host "  Include Directory: $tesseractInclude" -ForegroundColor White
Write-Host "  Library Directory: $tesseractLib" -ForegroundColor White
Write-Host "  Libraries:         $tesseractLibs" -ForegroundColor White
Write-Host ""

Write-Host "?? Next Steps:" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. CLOSE Visual Studio if open" -ForegroundColor White
Write-Host ""
Write-Host "2. Copy Runtime DLLs:" -ForegroundColor White
Write-Host "   .\copy_tesseract_dlls.bat" -ForegroundColor Cyan
Write-Host "   (OR manually copy from: $($config.BinPath))" -ForegroundColor Gray
Write-Host ""
Write-Host "3. REOPEN Visual Studio" -ForegroundColor White
Write-Host "   - Open 'Naghuma Toolbox.sln'" -ForegroundColor Gray
Write-Host ""
Write-Host "4. CLEAN and REBUILD Solution" -ForegroundColor White
Write-Host "   - Build ? Clean Solution" -ForegroundColor Gray
Write-Host "   - Build ? Rebuild Solution (Ctrl+Shift+B)" -ForegroundColor Gray
Write-Host ""
Write-Host "5. RUN Application" -ForegroundColor White
Write-Host "   - Option A: Use run_with_tesseract.bat" -ForegroundColor Cyan
Write-Host "   - Option B: Press Ctrl+F5 in Visual Studio" -ForegroundColor Gray
Write-Host ""
Write-Host "6. TEST OCR Feature" -ForegroundColor White
Write-Host "   - Load an image with text" -ForegroundColor Gray
Write-Host "   - Process ? OCR - Text Recognition..." -ForegroundColor Gray
Write-Host "   - Select language and recognize!" -ForegroundColor Gray
Write-Host ""

Write-Host "? Project configured with correct library names!" -ForegroundColor Green
Write-Host ""

Write-Host "Libraries detected and configured:" -ForegroundColor Yellow
Write-Host "  - $tesseractLibName" -ForegroundColor White
if ($leptonicaLibName) {
    Write-Host "  - $leptonicaLibName" -ForegroundColor White
}
Write-Host ""
