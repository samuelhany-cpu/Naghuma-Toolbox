# QUICK REFERENCE: Reduce MainWindow.cpp to ~1000 Lines

## Files You Need
1. ? `include/MainWindow_Macros.h` - Already created
2. ? `include/Theme.h` - Already created
3. ?? `src/MainWindow.cpp` - YOU need to edit this

## 6 Quick Steps (23 minutes total)

### Step 1: Add Includes (30 seconds)
At top of MainWindow.cpp, add:
```cpp
#include "MainWindow_Macros.h"
#include "Theme.h"
```

### Step 2: Replace Constructor Stylesheet (1 minute)
Find the `MainWindow::MainWindow()` constructor.

**Delete this entire block** (lines ~30-190):
```cpp
QString styleSheet = R"(
    QMainWindow {
        // ... 150 lines of CSS ...
    }
)";

QApplication::setStyle("Fusion");
setStyleSheet(styleSheet);
```

**Replace with**:
```cpp
QApplication::setStyle("Fusion");
setStyleSheet(Theme::MAIN_STYLESHEET);
```

**Saved**: 149 lines

### Step 3: Replace createMenuBar() (4 minutes)
**Delete** the entire `createMenuBar()` function (lines ~240-440).

**Replace** with the condensed version from `ULTRA_AGGRESSIVE_GUIDE.md` Step 2.

**Saved**: 130 lines

### Step 4: Replace createToolBar() (2 minutes)
**Delete** the entire `createToolBar()` function (lines ~445-545).

**Replace** with the condensed version from `ULTRA_AGGRESSIVE_GUIDE.md` Step 3.

**Saved**: 70 lines

### Step 5: Replace ALL Simple Functions with Macros (12 minutes)
Find and **completely replace** these sections:

#### A) Basic Image Processing (~line 1110)
**Delete** all 5 function implementations, **replace** with:
```cpp
IMPLEMENT_SIMPLE_FILTER(convertToGrayscale, ImageProcessor::convertToGrayscale, "Grayscale", "adjustment", "Converted to grayscale!")
IMPLEMENT_SIMPLE_FILTER(applyBinaryThreshold, ImageProcessor::applyBinaryThreshold, "Binary Threshold", "adjustment", "Binary threshold applied!")
IMPLEMENT_SIMPLE_FILTER(applyGaussianBlur, ImageProcessor::applyGaussianBlur, "Gaussian Blur", "filter", "Gaussian blur applied!")
IMPLEMENT_SIMPLE_FILTER(applyEdgeDetection, ImageProcessor::detectEdges, "Edge Detection", "filter", "Edge detection applied!")
IMPLEMENT_SIMPLE_FILTER(invertColors, ImageProcessor::invertColors, "Invert Colors", "adjustment", "Colors inverted!")
```
**Saved**: 95 lines

#### B) Advanced Filters (~line 1300)
**Delete** 5 functions, **replace** with:
```cpp
IMPLEMENT_SIMPLE_FILTER(applyLaplacianFilter, ImageFilters::applyLaplacian, "Laplacian Filter", "filter", "Laplacian filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyTraditionalFilter, ImageFilters::applyTraditionalFilter, "Traditional Filter", "filter", "Traditional filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyPyramidalFilter, ImageFilters::applyPyramidalFilter, "Pyramidal Filter", "filter", "Pyramidal filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyCircularFilter, ImageFilters::applyCircularFilter, "Circular Filter", "filter", "Circular filter applied successfully!")
IMPLEMENT_SIMPLE_FILTER(applyConeFilter, ImageFilters::applyConeFilter, "Cone Filter", "filter", "Cone filter applied successfully!")
```
**Saved**: 95 lines

#### C) Simple Transforms (~line 1000)
**Delete** flip functions and histogram ops, **replace** with:
```cpp
IMPLEMENT_SIMPLE_TRANSFORM(applyFlipX, ImageProcessor::flipHorizontal, "Flip Horizontal", "Flipped horizontally!")
IMPLEMENT_SIMPLE_TRANSFORM(applyFlipY, ImageProcessor::flipVertical, "Flip Vertical", "Flipped vertically!")
IMPLEMENT_SIMPLE_TRANSFORM(applyFlipXY, ImageProcessor::flipBoth, "Flip Both", "Flipped both ways!")
IMPLEMENT_SIMPLE_TRANSFORM(applyHistogramEqualization, ImageProcessor::equalizeHistogram, "Histogram Equalization", "Histogram equalization applied!")
IMPLEMENT_SIMPLE_TRANSFORM(applyOtsuThresholding, ImageProcessor::applyOtsuThreshold, "Otsu Thresholding", "Otsu thresholding applied!")
```
**Saved**: 45 lines

#### D) Morphology Operations (~line 1430)
**Delete** all 5 functions, **replace** with:
```cpp
IMPLEMENT_MORPH_OP(applyErosion, ImageProcessor::applyErosion, "Erosion", 5)
IMPLEMENT_MORPH_OP(applyDilation, ImageProcessor::applyDilation, "Dilation", 5)
IMPLEMENT_MORPH_OP(applyOpening, ImageProcessor::applyOpening, "Opening", 5)
IMPLEMENT_MORPH_OP(applyClosing, ImageProcessor::applyClosing, "Closing", 5)
IMPLEMENT_MORPH_OP(applyMorphGradient, ImageProcessor::applyMorphGradient, "Morphological Gradient", 5)
```
**Saved**: 95 lines

#### E) FFT Operations (~line 1560)
**Delete** 2 functions, **replace** with:
```cpp
IMPLEMENT_FFT_FILTER(applyLowPassFilter, ImageProcessor::applyLowPassFilter, "Low-Pass Filter", 30)
IMPLEMENT_FFT_FILTER(applyHighPassFilter, ImageProcessor::applyHighPassFilter, "High-Pass Filter", 30)
```
**Saved**: 38 lines

### Step 6: Merge updateDisplay() Functions (3 minutes)
Find `updateDisplay()` and `updateMetricsDisplay()`.

**Delete both** functions.

**Replace** with the merged version from `ULTRA_AGGRESSIVE_GUIDE.md` Step 5.

**Saved**: 35 lines

## Quick Math

| What | Before | After | Saved |
|------|--------|-------|-------|
| Stylesheet | 150 | 1 | 149 |
| Menu creation | 200 | 70 | 130 |
| Toolbar | 100 | 30 | 70 |
| Basic processing | 100 | 5 | 95 |
| Advanced filters | 100 | 5 | 95 |
| Transforms | 50 | 5 | 45 |
| Morphology | 100 | 5 | 95 |
| FFT | 40 | 2 | 38 |
| Display funcs | 60 | 25 | 35 |
| **TOTAL** | **900** | **148** | **752** |

## Result
**Before**: ~1800 lines
**After**: 1800 - 752 = **~1048 lines**

**42% reduction!**

## Build & Test

```powershell
# Build
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild

# Run
.\x64\Debug\Naghuma` Toolbox.exe
```

## Troubleshooting

**Build Error: "Theme.h not found"**
? Make sure `include/Theme.h` exists

**Build Error: "Macros undefined"**
? Make sure `include/MainWindow_Macros.h` exists

**Runtime Error: Menu actions don't work**
? Check ADD_MENU_ACTION macro syntax

**Runtime Error: Functions crash**
? Make sure you didn't delete the helper methods (checkImageLoaded, applySimpleFilter, applySimpleTransform)

## What NOT to Delete

Keep these unchanged:
- ? Helper methods (checkImageLoaded, applySimpleFilter, applySimpleTransform)
- ? Dialog-based functions (applyTranslation, applyRotation, applyBrightnessContrast, etc.)
- ? Large dialog functions (showImageMetrics, showFFTSpectrum, showHistogram)
- ? Brush functions (toggleDrawingMode, onCanvasMouseMove, etc.)
- ? Layer management (onLayerRemoveRequested, onLayersRemoveRequested)
- ? setupUI, createCentralWidget, createStatusBar
- ? File operations (loadImage, saveImage, resetImage)

## Final Checklist

- [ ] Added Theme.h and MainWindow_Macros.h includes
- [ ] Replaced stylesheet with Theme::MAIN_STYLESHEET
- [ ] Replaced createMenuBar() with condensed version
- [ ] Replaced createToolBar() with condensed version
- [ ] Replaced basic processing functions with macros
- [ ] Replaced advanced filter functions with macros
- [ ] Replaced transform functions with macros
- [ ] Replaced morphology functions with macros
- [ ] Replaced FFT functions with macros
- [ ] Merged updateDisplay() and updateMetricsDisplay()
- [ ] Built successfully
- [ ] Tested all features
- [ ] File is ~1000 lines

?? **Done! You've reduced your MainWindow.cpp by 42%!**
