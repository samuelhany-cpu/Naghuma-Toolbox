# ============================================================================
# FIX INCOMPLETE TYPE ERRORS - MainWindow.cpp Include Order Fix
# ============================================================================

Write-Host ""
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "?   FIXING INCOMPLETE TYPE ERRORS       ?" -ForegroundColor Cyan
Write-Host "??????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

$file = "src\MainWindow.cpp"

Write-Host "[1/2] Backing up original file..." -ForegroundColor Yellow
Copy-Item $file "$file.backup" -Force
Write-Host "  ? Backup created: $file.backup" -ForegroundColor Green
Write-Host ""

Write-Host "[2/2] Fixing include order..." -ForegroundColor Yellow

# Read the file
$content = Get-Content $file -Raw

# Define the correct include block
$correctIncludes = @'
#include "MainWindow.h"

// ============================================================================
// UI COMPONENT INCLUDES - Must come FIRST for complete type definitions
// ============================================================================
#include "ImageCanvas.h"
#include "RightSidebarWidget.h"
#include "HistogramWidget.h"
#include "CollapsibleToolbar.h"

// ============================================================================
// DIALOG INCLUDES
// ============================================================================
#include "TransformDialog.h"
#include "AdjustmentDialog.h"
#include "CropTool.h"
#include "CompressionDialog.h"
#include "AutoEnhanceDialog.h"
#include "NoiseRemovalDialog.h"
#include "ColorConversionDialog.h"
#include "ThresholdingDialog.h"        // Phase 16
#include "SegmentationDialog.h"        // Phase 17
#include "FeatureDetectionDialog.h"    // Phase 19
#include "FrequencyFilterDialog.h"     // Phase 19 - Frequency Filters
#include "WaveletDialog.h"             // Phase 20
#include "IntensityTransformDialog.h"  // Phase 21
#include "SharpeningDialog.h"          // Phase 21

// ============================================================================
// ROI INCLUDES
// ============================================================================
#include "ROIManager.h"
#include "ROIShape.h"
#include "ROIDialog.h"

// ============================================================================
// PROCESSING INCLUDES
// ============================================================================
#include "ImageProcessor.h"
#include "filters/ImageFilters.h"
#include "color/ColorSpace.h"
#include "color/ColorProcessor.h"
#include "ImageMetrics.h"

// ============================================================================
// THEME AND HELPERS
// ============================================================================
#include "Theme.h"
#include "MainWindow_Macros.h"

// ============================================================================
// QT INCLUDES
// ============================================================================
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QInputDialog>
#include <algorithm>
'@

# Replace everything from #include "MainWindow.h" up to (but not including) MainWindow::MainWindow
$pattern = '(?s)#include "MainWindow\.h".*?(?=MainWindow::MainWindow)'
$content = $content -replace $pattern, "$correctIncludes`n`n"

# Save the file
Set-Content -Path $file -Value $content -NoNewline

Write-Host "  ? Include order fixed!" -ForegroundColor Green
Write-Host ""

Write-Host "??????????????????????????????????????????" -ForegroundColor Green
Write-Host "?          ? FIX APPLIED!  ?            ?" -ForegroundColor Green
Write-Host "??????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "Changes made:" -ForegroundColor Yellow
Write-Host "  • Reordered includes for complete type definitions" -ForegroundColor Cyan
Write-Host "  • ImageCanvas.h and RightSidebarWidget.h now come FIRST" -ForegroundColor Cyan
Write-Host "  • Organized includes into logical groups" -ForegroundColor Cyan
Write-Host ""

Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Open solution in Visual Studio" -ForegroundColor Cyan
Write-Host "  2. Build (Ctrl+Shift+B)" -ForegroundColor Cyan
Write-Host "  3. Errors should be resolved!" -ForegroundColor Cyan
Write-Host ""

Write-Host "If something goes wrong:" -ForegroundColor Yellow
Write-Host "  • Restore backup: Copy-Item '$file.backup' '$file' -Force" -ForegroundColor Cyan
Write-Host ""
