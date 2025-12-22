Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   AGGRESSIVE HEAP SPACE FIX           ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

Write-Host "This will:" -ForegroundColor White
Write-Host "  1. Add /Zm300 flag (300% compiler memory)" -ForegroundColor Gray
Write-Host "  2. Add /bigobj flag (large object files)" -ForegroundColor Gray
Write-Host "  3. Reduce template depth with /constexpr:depth512" -ForegroundColor Gray
Write-Host "  4. Clean all build artifacts" -ForegroundColor Gray
Write-Host "  5. Clear Visual Studio cache" -ForegroundColor Gray
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"
$backupFile = "Naghuma Toolbox.vcxproj.backup_aggressive"

# Backup
Write-Host "[1/5] Creating backup..." -ForegroundColor Yellow
Copy-Item $projectFile $backupFile -Force
Write-Host "  Backup: $backupFile" -ForegroundColor Green

# Read
Write-Host ""
Write-Host "[2/5] Updating compiler flags..." -ForegroundColor Yellow
[xml]$xml = Get-Content $projectFile

$updated = $false

# Find all ItemDefinitionGroup elements
foreach ($itemDefGroup in $xml.Project.ItemDefinitionGroup) {
    $clCompile = $itemDefGroup.ClCompile
    if ($clCompile) {
        Write-Host "  Processing configuration: $($itemDefGroup.Condition)" -ForegroundColor Gray
        
        # Check if AdditionalOptions exists
        if (!$clCompile.AdditionalOptions) {
            # Create AdditionalOptions element
            $additionalOptions = $xml.CreateElement("AdditionalOptions", $xml.DocumentElement.NamespaceURI)
            $additionalOptions.InnerText = "/Zm300 /bigobj /constexpr:depth512 %(AdditionalOptions)"
            $clCompile.AppendChild($additionalOptions) | Out-Null
            Write-Host "    [OK] Added compiler flags" -ForegroundColor Green
            $updated = $true
        } else {
            # Update existing AdditionalOptions
            $options = $clCompile.AdditionalOptions
            
            # Remove old /Zm flag if exists
            $options = $options -replace '/Zm\d+\s*', ''
            # Remove old /bigobj if exists
            $options = $options -replace '/bigobj\s*', ''
            # Remove old /constexpr if exists
            $options = $options -replace '/constexpr:depth\d+\s*', ''
            
            # Add new flags
            $options = "/Zm300 /bigobj /constexpr:depth512 $options"
            $clCompile.AdditionalOptions = $options
            
            Write-Host "    [OK] Updated compiler flags" -ForegroundColor Green
            $updated = $true
        }
    }
}

if ($updated) {
    Write-Host "  [OK] Compiler flags updated" -ForegroundColor Green
} else {
    Write-Host "  [WARNING] No configurations found to update" -ForegroundColor Yellow
}

# Save
Write-Host ""
Write-Host "[3/5] Saving project file..." -ForegroundColor Yellow
$xml.Save((Resolve-Path $projectFile).Path)
Write-Host "  [OK] Saved" -ForegroundColor Green

# Clean build artifacts
Write-Host ""
Write-Host "[4/5] Cleaning build artifacts..." -ForegroundColor Yellow

$cleaned = 0

# Clean Debug
if (Test-Path "x64\Debug") {
    Write-Host "  Cleaning Debug..." -ForegroundColor Gray
    
    Get-ChildItem "x64\Debug\*.obj" -ErrorAction SilentlyContinue | ForEach-Object {
        Remove-Item $_.FullName -Force
        $cleaned++
    }
    
    Get-ChildItem "x64\Debug\*.pch" -ErrorAction SilentlyContinue | ForEach-Object {
        Remove-Item $_.FullName -Force
        $cleaned++
    }
    
    Get-ChildItem "x64\Debug\*.pdb" -ErrorAction SilentlyContinue | ForEach-Object {
        Remove-Item $_.FullName -Force
        $cleaned++
    }
    
    Get-ChildItem "x64\Debug\*.idb" -ErrorAction SilentlyContinue | ForEach-Object {
        Remove-Item $_.FullName -Force
        $cleaned++
    }
}

# Clean Release
if (Test-Path "x64\Release") {
    Write-Host "  Cleaning Release..." -ForegroundColor Gray
    
    Get-ChildItem "x64\Release\*.obj" -ErrorAction SilentlyContinue | ForEach-Object {
        Remove-Item $_.FullName -Force
        $cleaned++
    }
    
    Get-ChildItem "x64\Release\*.pch" -ErrorAction SilentlyContinue | ForEach-Object {
        Remove-Item $_.FullName -Force
        $cleaned++
    }
}

# Clean intermediate directory
if (Test-Path "x64\Debug\Naghuma Toolbox.tlog") {
    Write-Host "  Cleaning build logs..." -ForegroundColor Gray
    Remove-Item -Recurse -Force "x64\Debug\Naghuma Toolbox.tlog" -ErrorAction SilentlyContinue
}

Write-Host "  [OK] Removed $cleaned build artifacts" -ForegroundColor Green

# Clean VS cache
Write-Host ""
Write-Host "[5/5] Cleaning Visual Studio cache..." -ForegroundColor Yellow

if (Test-Path ".vs") {
    $vsSize = (Get-ChildItem ".vs" -Recurse -File | Measure-Object -Property Length -Sum).Sum
    $vsSizeMB = [math]::Round($vsSize / 1MB, 2)
    
    Write-Host "  Removing .vs cache ($vsSizeMB MB)..." -ForegroundColor Gray
    Remove-Item -Recurse -Force ".vs" -ErrorAction SilentlyContinue
    Write-Host "  [OK] Visual Studio cache cleared" -ForegroundColor Green
} else {
    Write-Host "  [OK] No .vs cache to clean" -ForegroundColor Green
}

# Summary
Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   AGGRESSIVE FIX APPLIED               ?" -ForegroundColor Green
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

Write-Host "Compiler Flags Added:" -ForegroundColor Yellow
Write-Host "  /Zm300           - 300% compiler heap" -ForegroundColor White
Write-Host "  /bigobj          - Support large object files" -ForegroundColor White
Write-Host "  /constexpr:depth512 - Reduce template depth" -ForegroundColor White
Write-Host ""

Write-Host "Build artifacts cleaned: $cleaned files" -ForegroundColor Green
Write-Host ""

Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host "?   NEXT STEPS                           ?" -ForegroundColor Yellow
Write-Host "??????????????????????????????????????????" -ForegroundColor Yellow
Write-Host ""

Write-Host "1. CLOSE ALL Visual Studio instances" -ForegroundColor Cyan
Write-Host ""

Write-Host "2. Reopen project in Visual Studio" -ForegroundColor Cyan
Write-Host ""

Write-Host "3. Reduce parallel builds:" -ForegroundColor Cyan
Write-Host "   Tools -> Options -> Projects and Solutions" -ForegroundColor White
Write-Host "   -> Build and Run" -ForegroundColor White
Write-Host "   -> Maximum number of parallel builds = 1 or 2" -ForegroundColor White
Write-Host ""

Write-Host "4. Build -> Rebuild Solution" -ForegroundColor Cyan
Write-Host ""

Write-Host "5. If STILL failing:" -ForegroundColor Cyan
Write-Host "   - Close Visual Studio" -ForegroundColor White
Write-Host "   - Restart your computer (frees all memory)" -ForegroundColor White
Write-Host "   - Try building in Release mode instead:" -ForegroundColor White
Write-Host "     (Configuration dropdown -> Release)" -ForegroundColor White
Write-Host ""

Write-Host "Would you like to try building now? (Y/N): " -NoNewline -ForegroundColor Yellow
$response = Read-Host

if ($response -eq 'Y' -or $response -eq 'y') {
    Write-Host ""
    Write-Host "Building project (this may take 5-10 minutes)..." -ForegroundColor Cyan
    Write-Host ""
    
    # Try building with MSBuild directly with /m:1 (single-threaded)
    $msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    
    if (Test-Path $msbuild) {
        Write-Host "Using MSBuild with /m:1 (single-threaded)..." -ForegroundColor Yellow
        Write-Host ""
        
        & $msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m:1 /v:minimal
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host ""
            Write-Host "??????????????????????????????????????????" -ForegroundColor Green
            Write-Host "?   BUILD SUCCESSFUL!                    ?" -ForegroundColor Green
            Write-Host "??????????????????????????????????????????" -ForegroundColor Green
            Write-Host ""
        } else {
            Write-Host ""
            Write-Host "??????????????????????????????????????????" -ForegroundColor Red
            Write-Host "?   BUILD FAILED                         ?" -ForegroundColor Red
            Write-Host "??????????????????????????????????????????" -ForegroundColor Red
            Write-Host ""
            Write-Host "Try these solutions:" -ForegroundColor Yellow
            Write-Host "  1. Restart computer to free memory" -ForegroundColor Cyan
            Write-Host "  2. Close all other applications" -ForegroundColor Cyan
            Write-Host "  3. Build in Release mode (uses less memory)" -ForegroundColor Cyan
            Write-Host "  4. Build individual files that fail" -ForegroundColor Cyan
            Write-Host ""
        }
    } else {
        Write-Host "MSBuild not found. Please build manually in Visual Studio." -ForegroundColor Yellow
    }
}

Write-Host ""
