# Naghuma Toolbox - Professional Project Structure

## Overview
Naghuma Toolbox is a professional image processing application built with Qt and OpenCV, featuring a modular architecture with specialized libraries for different image processing operations.

## Project Structure

```
Naghuma Toolbox/
│
├── src/                          # Application Source Files
│   ├── main.cpp                 # Application entry point
│   ├── MainWindow.cpp           # Main window implementation
│   ├── ImageCanvas.cpp          # Image display canvas
│   ├── HistogramWidget.cpp      # Histogram visualization
│   └── moc_*.cpp                # Qt Meta-Object Compiler files
│
├── include/                      # Application Header Files
│   ├── MainWindow.h             # Main window interface
│   ├── ImageCanvas.h            # Image canvas interface
│   └── HistogramWidget.h        # Histogram widget interface
│
├── lib/                          # External Libraries (Modular Design)
│   │
│   ├── filters/                 # Image Filtering Library
│   │   ├── ImageFilters.h       # Filter declarations
│   │   └── ImageFilters.cpp     # Filter implementations
│   │
│   ├── transforms/              # Geometric Transformations Library
│   │   ├── ImageTransforms.h    # Transform declarations
│   │   └── ImageTransforms.cpp  # Transform implementations
│   │
│   ├── histogram/               # Histogram Operations Library
│   │   ├── HistogramOperations.h    # Histogram declarations
│   │   └── HistogramOperations.cpp  # Histogram implementations
│   │
│   └── utils/                   # Utility Functions
│       └── (future utilities)
│
├── scripts/                      # Build and Utility Scripts
│   ├── remove_all_emojis.ps1   # Remove emojis from source files
│   ├── organize_project.ps1     # Organize project structure
│   ├── build_and_run.bat       # Build and run application
│   ├── configure_qt.ps1         # Qt configuration
│   ├── configure_cpp17.ps1      # C++17 configuration
│   ├── generate_moc.ps1         # Generate MOC files
│   └── (other utility scripts)
│
├── x64/Debug/                    # Build Output Directory
│   └── (compiled binaries)
│
├── Naghuma Toolbox.vcxproj      # Visual Studio Project File
├── Naghuma Toolbox.sln          # Visual Studio Solution File
└── README.md                     # This file

```

## Library Modules

### 1. ImageFilters Library (`lib/filters/`)

Professional image filtering operations with comprehensive edge detection and enhancement capabilities.

**Features:**
- **Edge Detection Filters:**
  - Laplacian filter (standard and custom kernels)
  - Sobel filter (horizontal, vertical, diagonal, combined)
  - Prewitt filter
  - Scharr filter (high-accuracy edge detection)
  - Canny edge detection

- **Smoothing Filters:**
  - Gaussian blur
  - Median blur
  - Bilateral filter (edge-preserving)

- **Enhancement:**
  - Unsharp masking (sharpening)
  - Custom kernel convolution

**Usage Example:**
```cpp
#include "lib/filters/ImageFilters.h"

cv::Mat src, dst;
// Apply Laplacian filter
ImageFilters::applyLaplacian(src, dst);

// Apply combined Sobel filter
cv::Mat dst_H, dst_V, dst_D, dst_S;
ImageFilters::applySobelCombined(src, dst_H, dst_V, dst_D, dst_S);

// Apply Canny edge detection
ImageFilters::applyCanny(src, dst, 100, 200);
```

### 2. ImageTransforms Library (`lib/transforms/`)

Comprehensive geometric transformation operations for image manipulation.

**Features:**
- **Basic Transformations:**
  - Translation
  - Rotation (standard and around custom point)
  - Scaling/Zoom
  - Resize

- **Advanced Transformations:**
  - Skew/Shear
  - Affine transformation (3-point)
  - Perspective transformation (4-point)
  - Custom warp matrices

- **Flip Operations:**
  - Horizontal flip
  - Vertical flip
  - Both axes flip

- **Cropping:**
  - Rectangle crop
  - ROI (Region of Interest) crop

**Usage Example:**
```cpp
#include "lib/transforms/ImageTransforms.h"

cv::Mat src, dst;
// Translate image
ImageTransforms::translate(src, dst, 50, 30);

// Rotate image
ImageTransforms::rotate(src, dst, 45.0);

// Zoom image
ImageTransforms::zoom(src, dst, 1.5);

// Flip horizontally
ImageTransforms::flipHorizontal(src, dst);
```

### 3. HistogramOperations Library (`lib/histogram/`)

Advanced histogram analysis and manipulation for image enhancement.

**Features:**
- **Histogram Analysis:**
  - Grayscale histogram calculation
  - Color (BGR) histogram calculation
  - Statistical analysis (mean, std dev, min, max)

- **Histogram Equalization:**
  - Standard histogram equalization
  - Color-preserving equalization (YCrCb space)
  - Adaptive histogram equalization (CLAHE)

- **Thresholding:**
  - Manual thresholding
  - Otsu's automatic thresholding
  - Adaptive thresholding

- **Enhancement:**
  - Histogram stretching
  - Histogram matching
  - Gamma correction
  - Brightness/Contrast adjustment

**Usage Example:**
```cpp
#include "lib/histogram/HistogramOperations.h"

cv::Mat src, dst;
// Apply histogram equalization
HistogramOperations::equalizeHistogram(src, dst);

// Apply Otsu thresholding
double threshold = HistogramOperations::otsuThreshold(src, dst);

// Apply CLAHE (adaptive equalization)
HistogramOperations::adaptiveHistogramEqualization(src, dst, 2.0, 8);

// Calculate statistics
double mean, stddev, minVal, maxVal;
HistogramOperations::calculateStatistics(src, mean, stddev, minVal, maxVal);
```

## Building the Project

### Prerequisites
- Visual Studio 2019 or later
- Qt 5.x or 6.x
- OpenCV 4.x
- C++17 support

### Build Steps

1. **Configure Qt paths:**
   ```powershell
   .\scripts\configure_qt.ps1
   ```

2. **Generate MOC files:**
   ```powershell
   .\scripts\generate_moc.ps1
   ```

3. **Build and run:**
   ```powershell
   .\scripts\build_and_run.bat
   ```

   Or open `Naghuma Toolbox.sln` in Visual Studio and build.

### Updating Project Files

After reorganizing the project structure, update the `.vcxproj` file to include new paths:

```xml
<!-- In ItemGroup for Include Files -->
<ClInclude Include="include\MainWindow.h" />
<ClInclude Include="include\ImageCanvas.h" />
<ClInclude Include="include\HistogramWidget.h" />
<ClInclude Include="lib\filters\ImageFilters.h" />
<ClInclude Include="lib\transforms\ImageTransforms.h" />
<ClInclude Include="lib\histogram\HistogramOperations.h" />

<!-- In ItemGroup for Source Files -->
<ClCompile Include="src\main.cpp" />
<ClCompile Include="src\MainWindow.cpp" />
<ClCompile Include="src\ImageCanvas.cpp" />
<ClCompile Include="src\HistogramWidget.cpp" />
<ClCompile Include="lib\filters\ImageFilters.cpp" />
<ClCompile Include="lib\transforms\ImageTransforms.cpp" />
<ClCompile Include="lib\histogram\HistogramOperations.cpp" />
```

## Utility Scripts

### Remove Emojis Script
Removes all emoji and special Unicode characters from source files:
```powershell
.\scripts\remove_all_emojis.ps1
```

### Organize Project Script
Reorganizes files into the professional structure:
```powershell
.\scripts\organize_project.ps1
```

## Integration into MainWindow

To use the new libraries in your MainWindow class:

```cpp
// In MainWindow.h
#include "../lib/filters/ImageFilters.h"
#include "../lib/transforms/ImageTransforms.h"
#include "../lib/histogram/HistogramOperations.h"

// In MainWindow.cpp
void MainWindow::applyEdgeDetection() {
    if (!imageLoaded) return;
    
    // Use the ImageFilters library
    ImageFilters::applySobel(currentImage, processedImage, 'b');
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Sobel filter applied!", "success");
}

void MainWindow::applyRotation() {
    if (!imageLoaded) return;
    
    // Use the ImageTransforms library
    ImageTransforms::rotate(currentImage, processedImage, 45.0);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Rotation applied!", "success");
}

void MainWindow::applyHistogramEqualization() {
    if (!imageLoaded) return;
    
    // Use the HistogramOperations library
    HistogramOperations::equalizeHistogram(currentImage, processedImage);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Histogram equalization applied!", "success");
}
```

## Features

### Current Features
- Image loading and saving
- Real-time image display with dual canvas
- Comprehensive histogram visualization
- Edge detection (Sobel, Laplacian, Canny, Prewitt, Scharr)
- Geometric transformations (rotate, translate, scale, flip, skew)
- Histogram operations (equalization, CLAHE, Otsu thresholding)
- Image filtering (blur, sharpen, bilateral)
- Beautiful feminine-themed UI (pink, navy, baby blue)

### Professional Enhancements
- **Modular Architecture:** Separated concerns into specialized libraries
- **Clean Code:** Well-documented, namespace-organized functions
- **Reusability:** Libraries can be used in other projects
- **Maintainability:** Easy to locate and update specific functionality
- **Scalability:** Simple to add new features to appropriate libraries

## Development Guidelines

### Adding New Features

1. **For new filters:** Add to `lib/filters/ImageFilters.h` and `.cpp`
2. **For new transforms:** Add to `lib/transforms/ImageTransforms.h` and `.cpp`
3. **For histogram operations:** Add to `lib/histogram/HistogramOperations.h` and `.cpp`
4. **For utilities:** Create new files in `lib/utils/`

### Code Style
- Use namespace organization
- Document all public functions
- Follow existing naming conventions
- Keep functions focused and single-purpose

## License
(Add your license information here)

## Contributors
- Naghuma Development Team

## Version History
- **v2.0** - Professional modular architecture with separated libraries
- **v1.0** - Initial monolithic implementation

---

**Last Updated:** November 2025
