# ?? Naghuma Toolbox - Professional Image Processing Suite

A modern, feature-rich desktop image processing application built with C++ using Qt 6 Framework and OpenCV 4, featuring a beautiful feminine-themed dark UI with elegant color palette.

## ? Features

### ?? File Operations
- **Load Image**: Support for PNG, JPG, JPEG, BMP, and TIFF formats
- **Save Image**: Export processed images in multiple formats
- **Reset**: Restore original image instantly

### ?? Image Information & Analysis
- **Image Info**: Detailed metadata including dimensions, channels, data type, and statistics
- **Pixel Info**: Query individual pixel values at any coordinate
- **Statistics**: Min, max, mean values and standard deviation calculation

### ?? Geometric Transformations
- **Translation**: Move image horizontally and vertically with live preview
- **Rotation**: Rotate image by any angle (-180° to +180°)
- **Skew**: Apply affine transformations for perspective effects
- **Zoom**: Scale images from 0.5x to 3.0x
- **Flip Horizontal**: Mirror image left-right
- **Flip Vertical**: Mirror image top-bottom
- **Flip Both**: Rotate 180 degrees

### ?? Histogram Operations
- **Show Histogram**: Beautiful visualization of pixel value distribution
  - Grayscale: Pink gradient display
  - Color: Multi-channel display (Pink, Lavender, Soft Yellow)
- **Histogram Equalization**: Enhance image contrast automatically
  - Before/After comparison view
  - Statistical analysis of improvements
- **Otsu Thresholding**: Automatic optimal threshold selection

### ?? Image Processing
- **Grayscale Conversion**: Convert color images to grayscale
- **Binary Threshold**: Apply binary thresholding (threshold: 128)
- **Gaussian Blur**: Smooth images with 15x15 kernel
- **Edge Detection**: Canny edge detection (100, 200 thresholds)
- **Invert Colors**: Negative image effect

## ?? Design Theme

### Feminine Color Palette
- **Primary Background**: `#0f0a14` (Deep Purple-Black)
- **Secondary Background**: `#1a1625` (Dark Purple)
- **Accent Pink**: `#ff6b9d` (Vibrant Pink)
- **Accent Lavender**: `#a29bfe` (Soft Purple)
- **Accent Peach**: `#ffeaa7` (Soft Yellow)
- **Accent Mint**: `#55efc4` (Mint Green)
- **Text Primary**: `#fef5f8` (Off-White)

### UI Components
- Modern dark theme with elegant gradients
- Rounded corners and smooth borders
- Gradient buttons and sliders
- Responsive canvas with auto-scaling
- Status bar with color-coded messages
- Beautiful histogram visualizations

## ??? Technical Stack

### Technologies
- **Language**: C++17
- **GUI Framework**: Qt 6.7.3 (Widgets module)
- **Image Processing**: OpenCV 4.3.0
- **Build System**: MSBuild (Visual Studio 2022)
- **Platform**: Windows x64

### Project Structure
```
Naghuma Toolbox/
??? Main Application
?   ??? main.cpp                    # Application entry point
?   ??? MainWindow.h/cpp            # Main window with all functionality
?   ??? ImageCanvas.h/cpp           # Custom image display widget
?   ??? HistogramWidget.h/cpp       # Histogram visualization widget
??? MOC Generated Files
?   ??? moc_MainWindow.cpp          # Qt meta-object for MainWindow
?   ??? moc_ImageCanvas.cpp         # Qt meta-object for ImageCanvas
?   ??? moc_HistogramWidget.cpp     # Qt meta-object for HistogramWidget
??? Build Scripts
?   ??? configure_qt.ps1            # Configure Qt paths in project
?   ??? configure_cpp17.ps1         # Set C++17 standard
?   ??? generate_moc.ps1            # Generate MOC files
?   ??? add_moc_to_project.ps1      # Add MOC files to project
??? Launchers
?   ??? run_naghuma_toolbox.bat     # Quick launcher with DLL paths
??? Project Files
    ??? Naghuma Toolbox.sln         # Visual Studio solution
    ??? Naghuma Toolbox.vcxproj     # Visual Studio project
    ??? README.md                   # This file
```

## ?? Building the Project

### Prerequisites
1. **Visual Studio 2022** with C++ desktop development workload
2. **Qt 6.7.3** (MSVC 2019 64-bit) installed at `C:\Qt\6.7.3\msvc2019_64`
3. **OpenCV 4.3.0** installed at `F:\OpenCV\opencv`

### Build Steps

1. **Open the Solution**
   ```
   Open "Naghuma Toolbox.sln" in Visual Studio 2022
   ```

2. **Generate MOC Files** (if not already done)
   ```powershell
   .\generate_moc.ps1
   ```

3. **Build the Project**
   - Select **Debug** or **Release** configuration
   - Select **x64** platform
   - Build > Rebuild Solution (Ctrl+Alt+F7)

   Or use MSBuild from command line:
   ```powershell
   msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild
   ```

## ?? Running the Application

### Method 1: Using the Batch File (Recommended)
```bat
run_naghuma_toolbox.bat
```

### Method 2: Manually
```powershell
# Add DLL paths to environment
$env:PATH = "C:\Qt\6.7.3\msvc2019_64\bin;F:\OpenCV\opencv\build\x64\vc15\bin;" + $env:PATH

# Run the executable
.\x64\Debug\Naghuma Toolbox.exe
```

### Method 3: From Visual Studio
1. Ensure Qt and OpenCV bin directories are in system PATH
2. Press F5 or click "Start Debugging"

## ?? How to Use

### Loading an Image
1. Click **"??? Load Image"** button in toolbar or use File menu
2. Select an image file (PNG, JPG, BMP, TIFF)
3. Image appears in the left canvas

### Processing an Image
1. Choose any operation from the menu or toolbar
2. For transformations with parameters, adjust sliders/spinboxes in the dialog
3. Processed result appears in the right canvas
4. Save the result using **"?? Save"** button

### Viewing Information
- **Image Info** (?? ? ??): See detailed metadata
- **Pixel Info** (?? ? ??): Query specific pixel values
- **Statistics** (?? ? ??): View statistical analysis
- **Histogram** (?? ? ??): Visualize pixel distribution

### Tips
- Use **Reset** to restore the original image
- Live preview available in transformation dialogs
- Status bar shows operation feedback
- Side-by-side comparison of original and processed images

## ?? Configuration Files

### PowerShell Scripts

#### `configure_qt.ps1`
Configures Qt include and library paths in the Visual Studio project file.

#### `configure_cpp17.ps1`
Sets C++17 standard and required compiler flags for Qt 6.

#### `generate_moc.ps1`
Generates Meta-Object Compiler files required for Qt signals/slots.

#### `add_moc_to_project.ps1`
Adds generated MOC files to the Visual Studio project.

## ?? Dependencies

### Qt 6.7.3 Libraries (Debug)
- Qt6Cored.lib
- Qt6Guid.lib
- Qt6Widgetsd.lib

### Qt 6.7.3 Libraries (Release)
- Qt6Core.lib
- Qt6Gui.lib
- Qt6Widgets.lib

### OpenCV 4.3.0 Libraries
- opencv_world430d.lib (Debug)
- opencv_world430.lib (Release)

### Runtime DLLs Required
From Qt: `C:\Qt\6.7.3\msvc2019_64\bin\`
- Qt6Core.dll / Qt6Cored.dll
- Qt6Gui.dll / Qt6Guid.dll
- Qt6Widgets.dll / Qt6Widgetsd.dll

From OpenCV: `F:\OpenCV\opencv\build\x64\vc15\bin\`
- opencv_world430.dll / opencv_world430d.dll
- opencv_videoio_ffmpeg430_64.dll

## ?? UI Customization

The application uses Qt Stylesheets for theming. To customize colors, edit the `styleSheet` QString in `MainWindow.cpp` constructor.

### Example: Change Accent Color
```cpp
// In MainWindow.cpp, find and modify:
constexpr const char* ACCENT_PINK = "#your_color_here";
```

## ?? Troubleshooting

### Application doesn't start
- **Issue**: Missing DLL error
- **Solution**: Ensure Qt and OpenCV bin directories are in PATH or use `run_naghuma_toolbox.bat`

### Build errors about Qt headers not found
- **Issue**: Qt include paths not configured
- **Solution**: Run `.\configure_qt.ps1` and rebuild

### Linker errors about qt_metacast, metaObject
- **Issue**: MOC files not generated
- **Solution**: Run `.\generate_moc.ps1` then `.\add_moc_to_project.ps1` and rebuild

### C++17 compiler errors
- **Issue**: C++ standard not set correctly
- **Solution**: Run `.\configure_cpp17.ps1` and rebuild

## ?? License

This project is created as an educational image processing application.

## ????? Author

Developed with ?? by Naghuma

## ?? Acknowledgments

- Qt Framework for the excellent GUI toolkit
- OpenCV for powerful image processing capabilities
- The open-source community for continuous support

---

**Enjoy using Naghuma Toolbox! ???**
