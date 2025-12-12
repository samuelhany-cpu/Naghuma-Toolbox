# ULTRA-AGGRESSIVE Refactoring - Get to 900 Lines

## Step 1: Add Macro Header (1 minute)

1. Include the macros at the top of MainWindow.cpp:
```cpp
#include "MainWindow.h"
#include "MainWindow_Macros.h"  // ADD THIS LINE
#include "ImageCanvas.h"
```

2. The macros file (`include/MainWindow_Macros.h`) is already created.

## Step 2: Condense createMenuBar() (5 minutes)

**Replace the entire createMenuBar() function with this:**

```cpp
void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // File Menu
    QMenu *fileMenu = menuBar->addMenu("File");
    ADD_MENU_ACTION(fileMenu, "Load Image", loadImage);
    ADD_MENU_ACTION(fileMenu, "Save Image", saveImage);
    fileMenu->addSeparator();
    ADD_MENU_ACTION(fileMenu, "Reset", resetImage);
    fileMenu->addSeparator();
    ADD_MENU_ACTION(fileMenu, "Exit", close);
    
    // Info Menu
    QMenu *infoMenu = menuBar->addMenu("Information");
    ADD_MENU_ACTION(infoMenu, "Image Info", showImageInfo);
    ADD_MENU_ACTION(infoMenu, "Pixel Info", showPixelInfo);
    ADD_MENU_ACTION(infoMenu, "Statistics", showImageStats);
    infoMenu->addSeparator();
    ADD_MENU_ACTION(infoMenu, "Image Metrics (RMSE/SNR/PSNR)", showImageMetrics);
    
    // Transform Menu
    QMenu *transformMenu = menuBar->addMenu("Transform");
    ADD_MENU_ACTION(transformMenu, "Translation", applyTranslation);
    ADD_MENU_ACTION(transformMenu, "Rotation", applyRotation);
    ADD_MENU_ACTION(transformMenu, "Skew", applySkew);
    ADD_MENU_ACTION(transformMenu, "Zoom", applyZoom);
    transformMenu->addSeparator();
    ADD_MENU_ACTION(transformMenu, "Flip Horizontal", applyFlipX);
    ADD_MENU_ACTION(transformMenu, "Flip Vertical", applyFlipY);
    ADD_MENU_ACTION(transformMenu, "Flip Both", applyFlipXY);
    
    // Histogram Menu
    QMenu *histMenu = menuBar->addMenu("Histogram");
    ADD_MENU_ACTION(histMenu, "Show Histogram", showHistogram);
    ADD_MENU_ACTION(histMenu, "Equalization", applyHistogramEqualization);
    ADD_MENU_ACTION(histMenu, "Otsu Thresholding", applyOtsuThresholding);
    
    // Process Menu
    QMenu *processMenu = menuBar->addMenu("Process");
    ADD_MENU_ACTION(processMenu, "Brightness/Contrast", applyBrightnessContrast);
    processMenu->addSeparator();
    ADD_MENU_ACTION(processMenu, "Grayscale", convertToGrayscale);
    ADD_MENU_ACTION(processMenu, "Binary Threshold", applyBinaryThreshold);
    ADD_MENU_ACTION(processMenu, "Gaussian Blur", applyGaussianBlur);
    ADD_MENU_ACTION(processMenu, "Edge Detection", applyEdgeDetection);
    ADD_MENU_ACTION(processMenu, "Invert Colors", invertColors);
    
    // Filters Menu
    QMenu *filtersMenu = menuBar->addMenu("Filters");
    ADD_MENU_ACTION(filtersMenu, "Laplacian Filter", applyLaplacianFilter);
    ADD_MENU_ACTION(filtersMenu, "Sobel Filter", applySobelCombinedFilter);
    ADD_MENU_ACTION(filtersMenu, "Traditional Filter", applyTraditionalFilter);
    ADD_MENU_ACTION(filtersMenu, "Pyramidal Filter", applyPyramidalFilter);
    ADD_MENU_ACTION(filtersMenu, "Circular Filter", applyCircularFilter);
    ADD_MENU_ACTION(filtersMenu, "Cone Filter", applyConeFilter);
    
    // Morphology Menu
    QMenu *morphMenu = menuBar->addMenu("Morphology");
    ADD_MENU_ACTION(morphMenu, "Erosion", applyErosion);
    ADD_MENU_ACTION(morphMenu, "Dilation", applyDilation);
    morphMenu->addSeparator();
    ADD_MENU_ACTION(morphMenu, "Opening", applyOpening);
    ADD_MENU_ACTION(morphMenu, "Closing", applyClosing);
    morphMenu->addSeparator();
    ADD_MENU_ACTION(morphMenu, "Morphological Gradient", applyMorphGradient);
    
    // FFT Menu
    QMenu *fftMenu = menuBar->addMenu("FFT");
    ADD_MENU_ACTION(fftMenu, "Show FFT Spectrum", showFFTSpectrum);
    fftMenu->addSeparator();
    ADD_MENU_ACTION(fftMenu, "Low-Pass Filter", applyLowPassFilter);
    ADD_MENU_ACTION(fftMenu, "High-Pass Filter", applyHighPassFilter);
    
    // Tools Menu
    QMenu *toolsMenu = menuBar->addMenu("Tools");
    ADD_MENU_ACTION(toolsMenu, "Brush Settings", showBrushSettings);
    toolsMenu->addSeparator();
    QAction *drawModeAction = toolsMenu->addAction("Toggle Drawing Mode");
    drawModeAction->setCheckable(true);
    connect(drawModeAction, &QAction::triggered, this, &MainWindow::toggleDrawingMode);
    ADD_MENU_ACTION(toolsMenu, "Apply Brush Effect (Auto)", applyBrushEffect);
}
```

**Lines saved**: 200 ? 70 = **130 lines saved**

## Step 3: Condense createToolBar() (3 minutes)

**Replace with this:**

```cpp
void MainWindow::createToolBar() {
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(24, 24));
    addToolBar(Qt::TopToolBarArea, toolbar);
    
    auto addBtn = [&](const QString& text, auto slot, int minWidth = 100, const char* styleClass = nullptr) {
        QPushButton *btn = new QPushButton(text, this);
        if (styleClass) btn->setProperty("class", styleClass);
        btn->setMinimumWidth(minWidth);
        connect(btn, &QPushButton::clicked, this, slot);
        toolbar->addWidget(btn);
        return btn;
    };
    
    addBtn("Load Image", &MainWindow::loadImage, 140, "accent");
    toolbar->addSeparator();
    addBtn("Save", &MainWindow::saveImage);
    addBtn("Reset", &MainWindow::resetImage);
    addBtn("Use Processed", &MainWindow::useProcessedImage, 120)->setToolTip("Use the processed image for next operations");
    toolbar->addSeparator();
    addBtn("Grayscale", &MainWindow::convertToGrayscale);
    addBtn("Blur", &MainWindow::applyGaussianBlur);
    addBtn("Edges", &MainWindow::applyEdgeDetection);
    toolbar->addSeparator();
    
    QPushButton *drawModeBtn = addBtn("Drawing Mode: OFF", &MainWindow::toggleDrawingMode, 140);
    drawModeBtn->setObjectName("drawModeButton");
    drawModeBtn->setCheckable(true);
}
```

**Lines saved**: 100 ? 30 = **70 lines saved**

## Step 4: Use Macros for All Simple Functions (10 minutes)

**Replace ALL these functions with one-liner macros:**

### Basic Image Processing
```cpp
// ========================================
// BASIC IMAGE PROCESSING
// ========================================

IMPLEMENT_SIMPLE_FILTER(convertToGrayscale, ImageProcessor::convertToGrayscale, "Grayscale", "adjustment", "Converted to grayscale!")
IMPLEMENT_SIMPLE_FILTER(applyBinaryThreshold, ImageProcessor::applyBinaryThreshold, "Binary Threshold", "adjustment", "Binary threshold applied!")
IMPLEMENT_SIMPLE_FILTER(applyGaussianBlur, ImageProcessor::applyGaussianBlur, "Gaussian Blur", "filter", "Gaussian blur applied!")
IMPLEMENT_SIMPLE_FILTER(applyEdgeDetection, ImageProcessor::detectEdges, "Edge Detection", "filter", "Edge detection applied!")
IMPLEMENT_SIMPLE_FILTER(invertColors, ImageProcessor::invertColors, "Invert Colors", "adjustment", "Colors inverted!")
```

**Lines saved**: 100 ? 5 = **95 lines saved**

### Advanced Filters
```cpp
// ========================================
// ADVANCED FILTERS
// ========================================

IMPLEMENT_SIMPLE_FILTER(applyLaplacianFilter, ImageFilters::applyLaplacian, "Laplacian Filter", "filter", "Laplacian filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyTraditionalFilter, ImageFilters::applyTraditionalFilter, "Traditional Filter", "filter", "Traditional filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyPyramidalFilter, ImageFilters::applyPyramidalFilter, "Pyramidal Filter", "filter", "Pyramidal filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyCircularFilter, ImageFilters::applyCircularFilter, "Circular Filter", "filter", "Circular filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyConeFilter, ImageFilters::applyConeFilter, "Cone Filter", "filter", "Cone filter applied successfully!")
```

**Lines saved**: 100 ? 5 = **95 lines saved**

### Simple Transforms
```cpp
// ========================================
// GEOMETRIC TRANSFORMATIONS (Simple)
// ========================================

IMPLEMENT_SIMPLE_TRANSFORM(applyFlipX, ImageProcessor::flipHorizontal, "Flip Horizontal", "Flipped horizontally!")
IMPLEMENT_SIMPLE_TRANSFORM(applyFlipY, ImageProcessor::flipVertical, "Flip Vertical", "Flipped vertically!")
IMPLEMENT_SIMPLE_TRANSFORM(applyFlipXY, ImageProcessor::flipBoth, "Flip Both", "Flipped both ways!")
IMPLEMENT_SIMPLE_TRANSFORM(applyHistogramEqualization, ImageProcessor::equalizeHistogram, "Histogram Equalization", "Histogram equalization applied!")
IMPLEMENT_SIMPLE_TRANSFORM(applyOtsuThresholding, ImageProcessor::applyOtsuThreshold, "Otsu Thresholding", "Otsu thresholding applied!")
```

**Lines saved**: 50 ? 5 = **45 lines saved**

### Morphology Operations
```cpp
// ========================================
// MORPHOLOGY OPERATIONS
// ========================================

IMPLEMENT_MORPH_OP(applyErosion, ImageProcessor::applyErosion, "Erosion", 5)
IMPLEMENT_MORPH_OP(applyDilation, ImageProcessor::applyDilation, "Dilation", 5)
IMPLEMENT_MORPH_OP(applyOpening, ImageProcessor::applyOpening, "Opening", 5)
IMPLEMENT_MORPH_OP(applyClosing, ImageProcessor::applyClosing, "Closing", 5)
IMPLEMENT_MORPH_OP(applyMorphGradient, ImageProcessor::applyMorphGradient, "Morphological Gradient", 5)
```

**Lines saved**: 100 ? 5 = **95 lines saved**

### FFT Operations
```cpp
// ========================================
// FFT OPERATIONS
// ========================================

IMPLEMENT_FFT_FILTER(applyLowPassFilter, ImageProcessor::applyLowPassFilter, "Low-Pass Filter", 30)
IMPLEMENT_FFT_FILTER(applyHighPassFilter, ImageProcessor::applyHighPassFilter, "High-Pass Filter", 30)
```

**Lines saved**: 40 ? 2 = **38 lines saved**

## Step 5: Combine updateDisplay() and updateMetricsDisplay() (2 minutes)

**Replace both functions with one:**

```cpp
void MainWindow::updateDisplay() {
    if (imageLoaded && !originalImage.empty()) {
        originalCanvas->setImage(originalImage);
        originalInfoLabel->setText(QString("Size: %1 x %2 | Channels: %3")
            .arg(originalImage.cols).arg(originalImage.rows).arg(originalImage.channels()));
    }
    
    if (recentlyProcessed && !processedImage.empty()) {
        processedCanvas->setImage(processedImage);
        processedInfoLabel->setText(QString("Size: %1 x %2 | Channels: %3")
            .arg(processedImage.cols).arg(processedImage.rows).arg(processedImage.channels()));
        
        // Update metrics inline
        auto result = ImageMetrics::calculateMetrics(originalImage, processedImage);
        if (result.isValid) {
            metricsLabel->setText(QString("RMSE: %1 | SNR: %2 dB | PSNR: %3 dB")
                .arg(result.rmse, 0, 'f', 2)
                .arg(result.snr, 0, 'f', 2)
                .arg(std::isinf(result.psnr) ? QString("?") : QString::number(result.psnr, 'f', 2)));
            metricsLabel->setVisible(true);
        } else {
            metricsLabel->setVisible(false);
        }
    } else {
        metricsLabel->setVisible(false);
    }
}
```

**Delete the separate `updateMetricsDisplay()` function entirely.**

**Lines saved**: 60 ? 25 = **35 lines saved**

## Total Lines Saved

| Section | Before | After | Saved |
|---------|--------|-------|-------|
| createMenuBar() | 200 | 70 | 130 |
| createToolBar() | 100 | 30 | 70 |
| Basic Processing | 100 | 5 | 95 |
| Advanced Filters | 100 | 5 | 95 |
| Simple Transforms | 50 | 5 | 45 |
| Morphology Ops | 100 | 5 | 95 |
| FFT Operations | 40 | 2 | 38 |
| updateDisplay merge | 60 | 25 | 35 |
| **TOTAL** | **750** | **147** | **603** |

## Final Result

**Current**: ~1800 lines
**After refactoring**: 1800 - 603 = **~1197 lines**

**Still need 297 more lines to hit 900...**

## Bonus Step 6: Extract Theme to Constant (saves 50 lines)

Replace the entire stylesheet in the constructor with:

```cpp
#include "Theme.h"  // Create this file with the stylesheet

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), imageLoaded(false), recentlyProcessed(false), drawingMode(false) {
    
    setWindowTitle("Naghuma Toolbox - Image Processing Suite");
    setMinimumSize(1600, 900);
    brushTool = new BrushTool(this);
    
    QApplication::setStyle("Fusion");
    setStyleSheet(Theme::MAIN_STYLESHEET);  // Single line instead of 150!
    
    setupUI();
}
```

**Lines saved**: 150 ? 1 = **149 lines** (by moving stylesheet to Theme.h)

## NEW TOTAL: 1800 - 603 - 149 = **1048 lines**

Still 148 lines over target. Can optimize dialog functions more if needed.

## Summary

Follow steps 1-6 to reduce from 1800 ? 1048 lines (42% reduction)
- Step 1: Add macros (1 min)
- Step 2: Condense menu (5 min)
- Step 3: Condense toolbar (3 min)
- Step 4: Use macros for functions (10 min)
- Step 5: Merge update functions (2 min)
- Step 6: Extract stylesheet (2 min)

**Total time: 23 minutes**
**Total savings: ~750 lines**
