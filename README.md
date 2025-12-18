# ?? Naghuma Toolbox - Professional Image Processing Suite

![Version](https://img.shields.io/badge/version-1.7.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Qt](https://img.shields.io/badge/Qt-6.7.3-green.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-red.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)

A powerful, feature-rich image processing application built with Qt and OpenCV, offering professional-grade tools for image manipulation, analysis, and enhancement.

---

## ? Key Features

### ??? Core Functionality
- **Dual Canvas System**: Side-by-side original and processed image display
- **Non-Destructive Editing**: Layer-based processing with full undo/redo
- **Real-Time Preview**: Instant feedback for all operations
- **Zoom & Pan**: Precise image navigation with mouse/keyboard controls
- **Multiple Format Support**: PNG, JPEG, BMP, TIFF

### ?? Image Transformations
- **Geometric Transforms**
  - Translation, Rotation, Skew, Zoom
  - Horizontal/Vertical/Both Flips
  - Interactive crop tool with visual selection
- **Live Preview**: All transforms show real-time preview before applying

### ?? Color Processing (Phases 14-15)
- **Color Space Conversions**
  - RGB ? HSV, LAB, YCbCr, HSI, Grayscale
  - Full bidirectional conversion support
- **Color Enhancement**
  - Per-channel histogram equalization
  - Auto white balance
  - Gamma correction (custom values)
- **Color Effects**
  - Pseudocolor with 19 colormaps (Jet, Viridis, Magma, etc.)
  - Gray level slicing
  - Bit plane slicing (8 levels)

### ?? Histogram Operations
- **Visualization**: Multi-channel histogram display
- **Equalization**: Global and adaptive (CLAHE)
- **Otsu Thresholding**: Automatic optimal thresholding

### ?? Segmentation (Phases 16-17)

#### Phase 16: Thresholding
- **Global Methods**
  - Binary, Binary Inverse, Truncate
  - To Zero, To Zero Inverse
- **Adaptive Methods**
  - Mean Adaptive
  - Gaussian Adaptive
- **Advanced Algorithms**
  - Otsu's Method (automatic optimal threshold)
  - Triangle Method
  - Multi-Otsu (2-level, 3-level)

#### Phase 17: Region-Based Segmentation ? NEW
- **Watershed Segmentation**
  - Topology-based region separation
  - Ideal for separating touching objects
  - Best for: Cell counting, object counting
- **K-Means Clustering**
  - Color-based pixel grouping
  - Adjustable clusters (2-20)
  - Best for: Color quantization, object detection prep
- **Mean Shift Segmentation**
  - Mode-finding algorithm
  - Spatial and color radius control
  - Best for: Natural scenes, texture segmentation
- **GrabCut (Foreground Extraction)**
  - Interactive foreground/background separation
  - Automatic center-region initialization
  - Best for: Background removal, object cutouts
- **SLIC Superpixels**
  - Regular region over-segmentation
  - Adjustable compactness
  - Best for: Computer vision preprocessing

### ?? Image Filters

#### Basic Filters
- Gaussian Blur
- Laplacian Filter
- Sobel Filter (Combined H+V+D)

#### Phase 13: Advanced Edge Detectors
- **Prewitt Operator**
  - Full edge detection
  - Separate X (vertical) and Y (horizontal) components
- **Roberts Cross Operator**
  - Fast 2×2 gradient detection
  - Diagonal edge emphasis
- **Laplacian of Gaussian (LoG)**
  - Combined smoothing and edge detection
  - Adjustable sigma for scale
- **Difference of Gaussians (DoG)**
  - Band-pass filter
  - Scale-space edge detection

#### Specialized Filters
- Traditional Filter
- Pyramidal Filter
- Circular Filter
- Cone Filter

### ?? Morphological Operations
- Erosion & Dilation
- Opening & Closing
- Morphological Gradient
- Customizable structuring elements

### ?? ROI Tools (Phase 12)
- **Interactive ROI Selection**
  - Mouse-based region selection
  - Toggle mode with Ctrl+R
- **Statistical Analysis**
  - Mean, Standard Deviation
  - Min/Max values
  - Histogram per ROI
- **ROI Management**
  - Save/Load ROI sets (JSON format)
  - Multiple ROI support
  - Visual ROI display with statistics

### ?? Frequency Domain
- **FFT Analysis**
  - Fast Fourier Transform visualization
  - Magnitude and phase spectrum
- **Frequency Filters**
  - Low-pass filter (smoothing)
  - High-pass filter (sharpening)
  - Adjustable cutoff frequencies

### ?? Enhancement Tools
- **Brightness/Contrast**: Interactive adjustment
- **Auto Enhancement**
  - CLAHE (Contrast Limited Adaptive Histogram Equalization)
  - Contrast Stretching
- **Noise Removal**
  - Gaussian Filter
  - Median Filter
  - Bilateral Filter (edge-preserving)

### ?? Compression
- **JPEG Compression**: Quality control (1-100)
- **PNG Compression**: Level control (0-9)
- **Metrics**: Real-time compression ratio and PSNR

### ?? Image Analysis
- **Quality Metrics**
  - RMSE (Root Mean Square Error)
  - SNR (Signal-to-Noise Ratio)
  - PSNR (Peak Signal-to-Noise Ratio)
- **Image Information**
  - Dimensions, channels, bit depth
  - Min/Max pixel values
  - Detailed statistics

### ?? User Interface

#### Modern Dark Theme
- Professional purple gradient theme
- Glowing borders and effects
- High contrast for readability

#### Collapsible Toolbars
- **Left Sidebar**: Quick access to common tools
- **Right Sidebar**: Layer management and histogram
- Expandable/collapsible for workspace optimization

#### Layer System
- **Non-Destructive Editing**: All operations stored as layers
- **Layer Management**
  - View all processing steps
  - Remove individual layers
  - Rebuild image from remaining layers
  - Layer thumbnails with operation names
- **Undo/Redo**: Full undo support (Ctrl+Z)

#### Zoom & Pan (Phase 11)
- **Mouse Wheel Zoom**: Ctrl + Wheel
- **Keyboard Shortcuts**
  - Ctrl + Plus/Minus: Zoom in/out
  - Ctrl + 0: Actual size (100%)
  - Ctrl + 9: Fit to window
- **Pan**: Click and drag to pan
- **Zoom Indicator**: Current zoom level display

---

## ?? Getting Started

### Prerequisites
- **Windows 10/11** (64-bit)
- **Qt 6.7.3** (MSVC 2019 64-bit)
- **OpenCV 4.x** (with contrib modules recommended)
- **Visual Studio 2019/2022** (C++17 support)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/samuelhany-cpu/Naghuma-Toolbox.git
   cd Naghuma-Toolbox
   ```

2. **Open in Visual Studio**
   - Open `Naghuma Toolbox.vcxproj`
   - Set configuration to Debug or Release
   - Set platform to x64

3. **Build**
   - Build ? Build Solution (F7)
   - Or use command line:
     ```bash
     msbuild "Naghuma Toolbox.vcxproj" /p:Configuration=Release /p:Platform=x64
     ```

4. **Run**
   - Press F5 or click Run
   - Executable location: `x64/Debug/Naghuma Toolbox.exe`

---

## ?? Usage Guide

### Quick Start (60 seconds)
1. **Load Image**: Click "Load Image" or File ? Load Image
2. **Apply Filter**: Process ? Gaussian Blur
3. **View Result**: Processed image appears on right
4. **Check Metrics**: RMSE/SNR/PSNR shown below image
5. **Undo**: Press Ctrl+Z or click Undo button
6. **Save**: File ? Save Image

### Common Workflows

#### Background Removal
```
1. Load image
2. Segmentation ? Region-Based Segmentation...
3. Select "GrabCut (Foreground)"
4. Adjust iterations (5-10)
5. Click Apply
6. Save result
```

#### Object Counting
```
1. Load image
2. Process ? Grayscale
3. Segmentation ? Region-Based Segmentation...
4. Select "Watershed Segmentation"
5. Adjust threshold (20-40%)
6. Count distinct colored regions
```

#### Color Posterization
```
1. Load colorful image
2. Segmentation ? Region-Based Segmentation...
3. Select "K-Means Clustering"
4. Set clusters to 5-8
5. Apply for artistic effect
```

#### Noise Reduction
```
1. Load noisy image
2. Process ? Noise Removal...
3. Choose filter type:
   - Gaussian: General noise
   - Median: Salt-and-pepper noise
   - Bilateral: Edge-preserving
4. Adjust parameters
5. Apply
```

#### Edge Detection
```
1. Load image
2. Filters ? Edge Detectors ?
   - Prewitt (general)
   - Roberts (fast, diagonal)
   - LoG (smooth edges)
   - DoG (scale-space)
3. View edge map
```

---

## ?? Keyboard Shortcuts

### General
- `Ctrl + Z`: Undo last operation
- `Ctrl + S`: Save image (when processed)
- `Ctrl + O`: Load image

### View Controls
- `Ctrl + Plus`: Zoom in
- `Ctrl + Minus`: Zoom out
- `Ctrl + 0`: Actual size (100%)
- `Ctrl + 9`: Fit to window

### Tools
- `Ctrl + R`: Toggle ROI mode
- `Enter`: Apply crop (when in crop mode)
- `Esc`: Cancel crop

---

## ?? Project Structure

```
Naghuma-Toolbox/
??? include/              # Header files
?   ??? MainWindow.h
?   ??? ImageCanvas.h
?   ??? SegmentationDialog.h    # Phase 17
?   ??? ThresholdingDialog.h    # Phase 16
?   ??? ColorConversionDialog.h # Phase 14
?   ??? ...
??? src/                  # Source files
?   ??? MainWindow.cpp
?   ??? ImageCanvas.cpp
?   ??? SegmentationDialog.cpp  # Phase 17
?   ??? color/                  # Phase 14-15
?   ?   ??? ColorSpace.cpp
?   ?   ??? ColorProcessor.cpp
?   ??? filters/
?       ??? ImageFilters.cpp
??? resources/            # Resources
?   ??? fonts.qrc
?   ??? Font Awesome 6 Free-Solid-900.otf
??? docs/                 # Documentation
?   ??? Phase17_Testing_Guide.md
?   ??? Phase17_Quick_Reference.md
?   ??? ...
??? README.md
```

---

## ?? Development Phases

### Completed Features

? **Phase 1-5**: Core functionality, basic filters, UI setup  
? **Phase 6**: Interactive crop tool  
? **Phase 7**: Image compression (JPEG/PNG)  
? **Phase 8**: Auto enhancement (CLAHE, Contrast Stretching)  
? **Phase 9**: Advanced noise removal  
? **Phase 10**: UI improvements, dark theme  
? **Phase 11**: Zoom & pan controls  
? **Phase 12**: ROI tools and analysis  
? **Phase 13**: Advanced edge detectors (Prewitt, Roberts, LoG, DoG)  
? **Phase 14**: Color space conversions (RGB/HSV/LAB/YCbCr/HSI)  
? **Phase 15**: Color processing operations (white balance, gamma, pseudocolor)  
? **Phase 16**: Image segmentation - Thresholding (Otsu, Adaptive, Multi-level)  
? **Phase 17**: Advanced segmentation (Watershed, K-Means, Mean Shift, GrabCut, SLIC) ? NEW

### Roadmap

?? **Phase 18**: Template matching and feature detection  
?? **Phase 19**: Object tracking  
?? **Phase 20**: Deep learning integration (optional)

---

## ?? Technical Details

### Dependencies
- **Qt 6.7.3**: GUI framework
- **OpenCV 4.x**: Image processing library
- **C++17**: Modern C++ features

### Performance
- **Real-time preview**: < 100ms for most operations
- **Large images**: Optimized for images up to 4K resolution
- **Memory efficient**: Smart pointer management

### Algorithms Implemented
- **Segmentation**: Watershed, K-Means, Mean Shift, GrabCut, SLIC
- **Edge Detection**: Sobel, Prewitt, Roberts, Laplacian, LoG, DoG
- **Noise Removal**: Gaussian, Median, Bilateral
- **Enhancement**: CLAHE, Histogram Equalization, Contrast Stretching
- **Morphology**: Erosion, Dilation, Opening, Closing, Gradient
- **Frequency**: FFT, Low-pass, High-pass filters

---

## ?? Contributing

Contributions are welcome! Please feel free to submit pull requests.

### How to Contribute
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## ?? License

This project is licensed under the MIT License - see the LICENSE file for details.

---

## ????? Author

**Samuel Hany**
- GitHub: [@samuelhany-cpu](https://github.com/samuelhany-cpu)
- Repository: [Naghuma-Toolbox](https://github.com/samuelhany-cpu/Naghuma-Toolbox)

---

## ?? Acknowledgments

- **Qt Framework**: For the excellent GUI toolkit
- **OpenCV**: For comprehensive image processing algorithms
- **Font Awesome**: For beautiful icons
- **Contributors**: Thanks to all who have contributed to this project

---

## ?? Screenshots

### Main Interface
![Main Window](https://via.placeholder.com/800x450?text=Naghuma+Toolbox+Main+Window)

### Segmentation Dialog (Phase 17)
![Segmentation](https://via.placeholder.com/800x450?text=Advanced+Segmentation+Dialog)

### Layer System
![Layers](https://via.placeholder.com/400x600?text=Layer+Management+Sidebar)

---

## ?? Version History

### v1.7.0 (Current) - Phase 17
- ? Added advanced region-based segmentation
- ?? Watershed segmentation
- ?? K-Means clustering
- ?? Mean Shift segmentation
- ?? GrabCut foreground extraction
- ?? SLIC superpixels
- ?? Comprehensive documentation

### v1.6.0 - Phase 16
- Added comprehensive thresholding tools
- Otsu's method, Adaptive thresholding
- Multi-level Otsu segmentation

### v1.5.0 - Phase 14-15
- Color space conversions
- Advanced color processing
- Pseudocolor, bit plane slicing

### v1.4.0 - Phase 13
- Advanced edge detectors
- Prewitt, Roberts, LoG, DoG

### v1.3.0 - Phase 12
- ROI tools and analysis
- Statistical ROI processing

### v1.2.0 - Phase 11
- Zoom and pan controls
- Interactive canvas navigation

### v1.1.0 - Phase 10
- Modern dark theme
- UI improvements

### v1.0.0
- Initial release
- Core functionality

---

## ?? Known Issues

- SLIC Superpixels requires OpenCV contrib modules (falls back to grid-based if unavailable)
- Very large images (>4K) may be slow for real-time preview
- GrabCut uses fixed center-region initialization (interactive mode planned)

---

## ?? Tips & Tricks

1. **Best Results**: Pre-process noisy images with Gaussian blur before segmentation
2. **Parameter Tuning**: Use real-time preview to find optimal parameters
3. **Layer System**: Keep original by applying operations as layers
4. **Undo Freely**: Non-destructive editing allows experimentation
5. **ROI Analysis**: Use ROI mode for statistical analysis of specific regions

---

**? If you find this project useful, please star it on GitHub!**

[Report Bug](https://github.com/samuelhany-cpu/Naghuma-Toolbox/issues) · [Request Feature](https://github.com/samuelhany-cpu/Naghuma-Toolbox/issues) · [Documentation](https://github.com/samuelhany-cpu/Naghuma-Toolbox/wiki)
