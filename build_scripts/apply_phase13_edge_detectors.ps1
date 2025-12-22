# Phase 13: Basic Edge Detectors - Complete Implementation Script
# This script adds all Phase 13 edge detectors to the Naghuma Toolbox

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Phase 13: Basic Edge Detectors Implementation" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Add Phase 13 implementations to MainWindow.cpp
Write-Host "[1/4] Adding Phase 13 edge detector implementations..." -ForegroundColor Yellow

$implementations = @'

// ============================================================================
// PHASE 13: BASIC EDGE DETECTORS
// ============================================================================

void MainWindow::applyPrewittEdge() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyPrewittEdge(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyPrewittEdge(input, result);
            return result;
        },
        "Prewitt Edge Detector", "filter", "Prewitt edge detection applied successfully!"
    );
}

void MainWindow::applyPrewittX() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyPrewittX(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyPrewittX(input, result);
            return result;
        },
        "Prewitt X (Vertical Edges)", "filter", "Prewitt X filter applied successfully!"
    );
}

void MainWindow::applyPrewittY() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyPrewittY(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyPrewittY(input, result);
            return result;
        },
        "Prewitt Y (Horizontal Edges)", "filter", "Prewitt Y filter applied successfully!"
    );
}

void MainWindow::applyRobertsCross() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyRobertsCross(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyRobertsCross(input, result);
            return result;
        },
        "Roberts Cross Operator", "filter", "Roberts cross operator applied successfully!"
    );
}

void MainWindow::applyLoG() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyLoG(src, dst, 5, 1.4);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyLoG(input, result, 5, 1.4);
            return result;
        },
        "LoG (Laplacian of Gaussian)", "filter", "LoG filter applied successfully!"
    );
}

void MainWindow::applyDoG() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyDoG(src, dst, 5, 1.0, 9, 2.0);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyDoG(input, result, 5, 1.0, 9, 2.0);
            return result;
        },
        "DoG (Difference of Gaussians)", "filter", "DoG filter applied successfully!"
    );
}
'@

# Read MainWindow.cpp
$mainWindowPath = "src\MainWindow.cpp"
$content = Get-Content $mainWindowPath -Raw

# Check if implementations already exist
if ($content -match "void MainWindow::applyPrewittEdge\(\)") {
    Write-Host "  - Phase 13 implementations already exist, skipping..." -ForegroundColor Gray
} else {
    # Find the last closing brace (end of file)
    $lastBrace = $content.LastIndexOf("}")
    
    # Insert before the last closing brace
    $newContent = $content.Insert($lastBrace, $implementations + "`n")
    Set-Content $mainWindowPath -Value $newContent -NoNewline
    Write-Host "  - Added 6 edge detector implementations" -ForegroundColor Green
}

# Step 2: Update Filters menu in createMenuBar()
Write-Host "[2/4] Updating Filters menu..." -ForegroundColor Yellow

$content = Get-Content $mainWindowPath -Raw

# Find the Cone Filter line and add Edge Detectors submenu after it
$coneFilterLine = 'ADD_MENU_ACTION(filtersMenu, "Cone Filter", applyConeFilter);'

if ($content -match [regex]::Escape($coneFilterLine)) {
    if (-not ($content -match "Edge Detectors")) {
        $edgeMenu = @'

    
    filtersMenu->addSeparator();
    
    // Phase 13: Basic Edge Detectors
    QMenu *edgeMenu = filtersMenu->addMenu("Edge Detectors");
    ADD_MENU_ACTION(edgeMenu, "Prewitt Edge Detector", applyPrewittEdge);
    ADD_MENU_ACTION(edgeMenu, "Prewitt X (Vertical Edges)", applyPrewittX);
    ADD_MENU_ACTION(edgeMenu, "Prewitt Y (Horizontal Edges)", applyPrewittY);
    edgeMenu->addSeparator();
    ADD_MENU_ACTION(edgeMenu, "Roberts Cross Operator", applyRobertsCross);
    edgeMenu->addSeparator();
    ADD_MENU_ACTION(edgeMenu, "LoG (Laplacian of Gaussian)", applyLoG);
    ADD_MENU_ACTION(edgeMenu, "DoG (Difference of Gaussians)", applyDoG);
'@
        
        $content = $content -replace ([regex]::Escape($coneFilterLine)), ($coneFilterLine + $edgeMenu)
        Set-Content $mainWindowPath -Value $content -NoNewline
        Write-Host "  - Added Edge Detectors submenu to Filters menu" -ForegroundColor Green
    } else {
        Write-Host "  - Edge Detectors submenu already exists, skipping..." -ForegroundColor Gray
    }
} else {
    Write-Host "  - Could not find Cone Filter line, menu may need manual update" -ForegroundColor Red
}

# Step 3: Clean build
Write-Host "[3/4] Cleaning and building project..." -ForegroundColor Yellow
Write-Host ""

# Clean
Write-Host "  Cleaning..." -ForegroundColor Gray
$cleanResult = msbuild "Naghuma Toolbox.vcxproj" /t:Clean /p:Configuration=Debug /p:Platform=x64 /v:minimal 2>&1

# Build
Write-Host "  Building..." -ForegroundColor Gray
$buildResult = msbuild "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal /maxcpucount:1 2>&1

# Check for errors
$buildOutput = $buildResult | Out-String
if ($buildOutput -match "error") {
    Write-Host ""
    Write-Host "BUILD FAILED!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Errors:" -ForegroundColor Red
    $buildResult | Select-String "error" | ForEach-Object { Write-Host $_ -ForegroundColor Red }
    exit 1
} else {
    Write-Host ""
    Write-Host "BUILD SUCCESSFUL!" -ForegroundColor Green
}

# Step 4: Run the application
Write-Host "[4/4] Launching application..." -ForegroundColor Yellow
Write-Host ""

$exePath = "x64\Debug\Naghuma Toolbox.exe"

if (Test-Path $exePath) {
    Write-Host "================================================" -ForegroundColor Cyan
    Write-Host "Phase 13 Implementation Complete!" -ForegroundColor Green
    Write-Host "================================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "New Edge Detectors Added:" -ForegroundColor Cyan
    Write-Host "  - Prewitt Edge Detector (Combined)" -ForegroundColor White
    Write-Host "  - Prewitt X (Vertical Edges)" -ForegroundColor White
    Write-Host "  - Prewitt Y (Horizontal Edges)" -ForegroundColor White
    Write-Host "  - Roberts Cross Operator" -ForegroundColor White
    Write-Host "  - LoG (Laplacian of Gaussian)" -ForegroundColor White
    Write-Host "  - DoG (Difference of Gaussians)" -ForegroundColor White
    Write-Host ""
    Write-Host "Menu Location:" -ForegroundColor Cyan
    Write-Host "  Filters -> Edge Detectors -> [Choose detector]" -ForegroundColor White
    Write-Host ""
    Write-Host "Starting application..." -ForegroundColor Yellow
    Write-Host ""
    
    Start-Process $exePath
    
    Write-Host "Application launched successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "To test the new edge detectors:" -ForegroundColor Cyan
    Write-Host "  1. Load an image" -ForegroundColor White
    Write-Host "  2. Go to Filters -> Edge Detectors" -ForegroundColor White
    Write-Host "  3. Choose any edge detector" -ForegroundColor White
    Write-Host "  4. Observe the edge detection results" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "ERROR: Could not find executable at $exePath" -ForegroundColor Red
    exit 1
}
