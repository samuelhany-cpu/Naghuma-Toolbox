Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   FIX COMPILER OUT OF HEAP SPACE      ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"
$backupFile = "Naghuma Toolbox.vcxproj.backup_heap"

Write-Host "This will add /Zm200 flag to increase compiler memory" -ForegroundColor White
Write-Host ""

# Backup
Write-Host "[1/4] Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile $backupFile -Force
Write-Host "  Backup: $backupFile" -ForegroundColor Green

# Read project file
Write-Host ""
Write-Host "[2/4] Reading project file..." -ForegroundColor Yellow
$content = Get-Content $projectFile -Raw

# Check if /Zm flag already exists
if ($content -match '/Zm\d+') {
    Write-Host "  /Zm flag already present, increasing value..." -ForegroundColor Cyan
    # Replace existing /Zm with /Zm300
    $content = $content -replace '/Zm\d+', '/Zm300'
} else {
    Write-Host "  Adding /Zm200 flag to compiler options..." -ForegroundColor Cyan
    
    # Find ClCompile sections for Debug configuration
    $pattern = '(<ClCompile>\s*<PrecompiledHeader[^>]*>\s*</PrecompiledHeader>\s*<WarningLevel[^>]*>\s*</WarningLevel>)'
    
    if ($content -match $pattern) {
        $originalSection = $Matches[1]
        $newSection = $originalSection + "`r`n      <AdditionalOptions>/Zm200 %(AdditionalOptions)</AdditionalOptions>"
        $content = $content -replace [regex]::Escape($originalSection), $newSection
        Write-Host "  [OK] Added /Zm200 flag" -ForegroundColor Green
    } else {
        Write-Host "  [WARNING] Could not find ClCompile section" -ForegroundColor Yellow
        Write-Host "  Trying alternative method..." -ForegroundColor Cyan
        
        # Alternative: Add to all ItemDefinitionGroup sections
        $pattern2 = '(<ItemDefinitionGroup[^>]*>\s*<ClCompile>)'
        if ($content -match $pattern2) {
            $insertPoint = $Matches[1]
            $newContent = $insertPoint + "`r`n      <AdditionalOptions>/Zm200 %(AdditionalOptions)</AdditionalOptions>"
            $content = $content -replace [regex]::Escape($insertPoint), $newContent
            Write-Host "  [OK] Added via alternative method" -ForegroundColor Green
        } else {
            Write-Host "  [ERROR] Could not add /Zm flag automatically" -ForegroundColor Red
            Write-Host ""
            Write-Host "MANUAL FIX REQUIRED:" -ForegroundColor Yellow
            Write-Host "  1. Open Naghuma Toolbox.sln in Visual Studio" -ForegroundColor Cyan
            Write-Host "  2. Right-click project -> Properties" -ForegroundColor Cyan
            Write-Host "  3. C/C++ -> Command Line" -ForegroundColor Cyan
            Write-Host "  4. Add: /Zm200" -ForegroundColor Cyan
            Write-Host "  5. Apply and rebuild" -ForegroundColor Cyan
            Write-Host ""
            exit 1
        }
    }
}

# Also add /bigobj flag for large object files
Write-Host ""
Write-Host "[3/4] Adding /bigobj flag..." -ForegroundColor Yellow
if ($content -notmatch '/bigobj') {
    $pattern = '(<AdditionalOptions>/Zm\d+)'
    if ($content -match $pattern) {
        $zmFlag = $Matches[1]
        $newFlag = $zmFlag + " /bigobj"
        $content = $content -replace [regex]::Escape($zmFlag), $newFlag
        Write-Host "  [OK] Added /bigobj flag" -ForegroundColor Green
    }
}

# Save
Write-Host ""
Write-Host "[4/4] Saving project file..." -ForegroundColor Yellow
$content | Set-Content $projectFile -NoNewline
Write-Host "  [OK] Project file updated" -ForegroundColor Green

# Verify
Write-Host ""
Write-Host "Verifying changes..." -ForegroundColor Yellow
$verify = Get-Content $projectFile -Raw

$hasZm = $verify -match '/Zm\d+'
$hasBigobj = $verify -match '/bigobj'

if ($hasZm) {
    $zmValue = $Matches[0]
    Write-Host "  [OK] Compiler memory flag: $zmValue" -ForegroundColor Green
} else {
    Write-Host "  [MISSING] /Zm flag not found" -ForegroundColor Red
}

if ($hasBigobj) {
    Write-Host "  [OK] Large object flag: /bigobj" -ForegroundColor Green
} else {
    Write-Host "  [MISSING] /bigobj flag not found" -ForegroundColor Red
}

Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   COMPILER FLAGS UPDATED               ?" -ForegroundColor Green
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

Write-Host "Additional Steps to Fix Heap Space Issues:" -ForegroundColor Yellow
Write-Host ""

Write-Host "1. CLOSE VISUAL STUDIO completely" -ForegroundColor Cyan
Write-Host ""

Write-Host "2. Clean build artifacts:" -ForegroundColor Cyan
Write-Host "   Remove-Item -Recurse -Force x64\Debug\*.obj" -ForegroundColor White
Write-Host "   Remove-Item -Recurse -Force x64\Debug\*.pch" -ForegroundColor White
Write-Host ""

Write-Host "3. Clear Visual Studio cache:" -ForegroundColor Cyan
Write-Host "   Remove-Item -Recurse -Force .vs" -ForegroundColor White
Write-Host ""

Write-Host "4. Reopen Visual Studio and rebuild" -ForegroundColor Cyan
Write-Host ""

Write-Host "5. If still failing, reduce parallel builds:" -ForegroundColor Cyan
Write-Host "   Tools -> Options -> Projects and Solutions" -ForegroundColor White
Write-Host "   -> Build and Run -> Maximum parallel builds = 1" -ForegroundColor White
Write-Host ""

Write-Host "Would you like to clean build artifacts now? (Y/N): " -NoNewline -ForegroundColor Yellow
$response = Read-Host

if ($response -eq 'Y' -or $response -eq 'y') {
    Write-Host ""
    Write-Host "Cleaning build artifacts..." -ForegroundColor Cyan
    
    # Clean object files
    if (Test-Path "x64\Debug") {
        Write-Host "  Removing .obj files..." -ForegroundColor Gray
        Get-ChildItem "x64\Debug\*.obj" -ErrorAction SilentlyContinue | Remove-Item -Force
        
        Write-Host "  Removing .pch files..." -ForegroundColor Gray
        Get-ChildItem "x64\Debug\*.pch" -ErrorAction SilentlyContinue | Remove-Item -Force
        
        Write-Host "  Removing .pdb files..." -ForegroundColor Gray
        Get-ChildItem "x64\Debug\*.pdb" -ErrorAction SilentlyContinue | Remove-Item -Force
        
        Write-Host "  [OK] Debug artifacts cleaned" -ForegroundColor Green
    }
    
    # Clean VS cache
    if (Test-Path ".vs") {
        Write-Host "  Removing .vs cache..." -ForegroundColor Gray
        Remove-Item -Recurse -Force ".vs" -ErrorAction SilentlyContinue
        Write-Host "  [OK] Visual Studio cache cleaned" -ForegroundColor Green
    }
    
    Write-Host ""
    Write-Host "??????????????????????????????????????????" -ForegroundColor Green
    Write-Host "?   CLEANUP COMPLETE                     ?" -ForegroundColor Green
    Write-Host "??????????????????????????????????????????" -ForegroundColor Green
    Write-Host ""
    Write-Host "Now:" -ForegroundColor Yellow
    Write-Host "  1. Close any open Visual Studio instances" -ForegroundColor Cyan
    Write-Host "  2. Reopen Naghuma Toolbox.sln" -ForegroundColor Cyan
    Write-Host "  3. Build -> Rebuild Solution" -ForegroundColor Cyan
    Write-Host ""
}

Write-Host ""
