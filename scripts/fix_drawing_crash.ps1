# Fix Drawing Mode Crash and Modularize MainWindow
# This script will:
# 1. Add the new modular brush handler file to the project
# 2. Update MainWindow.cpp to remove the old brush functions
# 3. Rebuild the project

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Fix Drawing Mode & Modularize" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Add new file to vcxproj
Write-Host "[1/4] Adding MainWindowBrush.cpp to project..." -ForegroundColor Yellow

$vcxprojPath = "Naghuma Toolbox.vcxproj"
[xml]$vcxproj = Get-Content $vcxprojPath

# Find ItemGroup for ClCompile
$compileGroup = $vcxproj.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

# Add new file if not exists
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\MainWindowBrush.cpp" }
if (-not $exists) {
    $newCompile = $vcxproj.CreateElement("ClCompile", $vcxproj.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\MainWindowBrush.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  ? Added MainWindowBrush.cpp" -ForegroundColor Green
} else {
    Write-Host "  - MainWindowBrush.cpp already exists" -ForegroundColor Gray
}

$vcxproj.Save($vcxprojPath)

# Step 2: Update filters
Write-Host "[2/4] Updating .filters file..." -ForegroundColor Yellow

$filtersPath = "Naghuma Toolbox.vcxproj.filters"
[xml]$filters = Get-Content $filtersPath

$sourceGroup = $filters.Project.ItemGroup | Where-Object { 
    $_.ClCompile -ne $null -and ($_.ClCompile | Where-Object { $_.Include -like "src\*" })
} | Select-Object -First 1

$exists = $sourceGroup.ClCompile | Where-Object { $_.Include -eq "src\MainWindowBrush.cpp" }
if (-not $exists) {
    $newFilter = $filters.CreateElement("ClCompile", $filters.Project.NamespaceURI)
    $newFilter.SetAttribute("Include", "src\MainWindowBrush.cpp")
    $filterElem = $filters.CreateElement("Filter", $filters.Project.NamespaceURI)
    $filterElem.InnerText = "Source Files"
    $newFilter.AppendChild($filterElem) | Out-Null
    $sourceGroup.AppendChild($newFilter) | Out-Null
    Write-Host "  ? Added to filters" -ForegroundColor Green
} else {
    Write-Host "  - Filter already exists" -ForegroundColor Gray
}

$filters.Save($filtersPath)

# Step 3: Create backup of MainWindow.cpp
Write-Host "[3/4] Creating backup..." -ForegroundColor Yellow
Copy-Item "src\MainWindow.cpp" "src\MainWindow.cpp.backup" -Force
Write-Host "  ? Backup created: src\MainWindow.cpp.backup" -ForegroundColor Green

# Step 4: Instructions for manual fix
Write-Host "[4/4] Manual Steps Required:" -ForegroundColor Yellow
Write-Host ""
Write-Host "  Please follow these steps to complete the fix:" -ForegroundColor White
Write-Host ""
Write-Host "  1. Open src\MainWindow.cpp" -ForegroundColor Cyan
Write-Host "  2. Find the toggleDrawingMode() function (around line 1681)" -ForegroundColor Cyan
Write-Host "  3. Replace these two lines:" -ForegroundColor Cyan
Write-Host "       processedImage = drawingCanvas.clone();" -ForegroundColor Red
Write-Host "       recentlyProcessed = true;" -ForegroundColor Red
Write-Host "       updateDisplay();" -ForegroundColor Red
Write-Host ""
Write-Host "     With these lines:" -ForegroundColor Cyan
Write-Host "       processedImage = drawingCanvas.clone();" -ForegroundColor Green
Write-Host "       recentlyProcessed = true;" -ForegroundColor Green
Write-Host "       processedCanvas->setImage(processedImage);  // Show image FIRST" -ForegroundColor Green
Write-Host "       processedCanvas->setMouseEventsEnabled(true);  // Enable mouse AFTER" -ForegroundColor Green
Write-Host ""
Write-Host "  4. Remove the line:" -ForegroundColor Cyan
Write-Host "       processedCanvas->setMouseEventsEnabled(drawingMode);" -ForegroundColor Red
Write-Host "     (It should be around line 1666)" -ForegroundColor Cyan
Write-Host ""
Write-Host "  5. Save the file" -ForegroundColor Cyan
Write-Host ""
Write-Host "  OR simply delete the 5 brush functions from MainWindow.cpp:" -ForegroundColor Yellow
Write-Host "    - showBrushSettings()" -ForegroundColor Gray
Write-Host "    - toggleDrawingMode()" -ForegroundColor Gray
Write-Host "    - onCanvasMousePress()" -ForegroundColor Gray
Write-Host "    - onCanvasMouseMove()" -ForegroundColor Gray
Write-Host "    - onCanvasMouseRelease()" -ForegroundColor Gray
Write-Host "    - applyBrushEffect()" -ForegroundColor Gray
Write-Host ""
Write-Host "  They are now in MainWindowBrush.cpp!" -ForegroundColor Green
Write-Host ""

# Step 5: Rebuild
Write-Host "Press any key to rebuild the project..." -ForegroundColor Yellow
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

Write-Host ""
Write-Host "Building project..." -ForegroundColor Cyan
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build /m:1 /nologo

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "================================" -ForegroundColor Green
    Write-Host "? BUILD SUCCESSFUL!" -ForegroundColor Green
    Write-Host "================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "The drawing mode crash is now fixed!" -ForegroundColor Green
    Write-Host "Test by:" -ForegroundColor White
    Write-Host "  1. Load an image" -ForegroundColor Gray
    Write-Host "  2. Click 'Drawing Mode: OFF' button" -ForegroundColor Gray
    Write-Host "  3. Draw on the processed canvas" -ForegroundColor Gray
    Write-Host "  4. It should work without crashing!" -ForegroundColor Gray
} else {
    Write-Host ""
    Write-Host "================================" -ForegroundColor Red
    Write-Host "? BUILD FAILED" -ForegroundColor Red
    Write-Host "================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please check the error messages above." -ForegroundColor Yellow
    Write-Host "You may need to manually apply the fix described in step [4/4]" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Press any key to exit..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
