# 📦 Naghuma Toolbox - Complete Clone & Setup Guide

**Version:** 2.0.0  
**Date:** December 22, 2025  
**Build Status:** ✅ Production Ready

---

## 🎯 Project Overview

**Naghuma Toolbox** is a comprehensive image processing application built with **Qt 6.7.3** and **OpenCV 4.3.0**. It provides professional-grade tools for image analysis, enhancement, filtering, edge detection, OCR, compression, and selective processing with an advanced selection tool system.

### Key Features
- **5-Method Selection Tool**: Rectangle, Polygon, Magic Wand, Threshold, Edge-Based
- **Layer System**: Non-destructive workflow with layer management
- **Image Processing**: 50+ filters, transforms, and enhancement algorithms
- **Edge Detection**: Sobel, Prewitt, Roberts, Canny, Laplacian, LoG
- **OCR Support**: Tesseract integration for text recognition
- **Huffman Compression**: Image compression with encoding/decoding
- **Histogram Analysis**: RGB/Grayscale histograms with equalization
- **ROI Tools**: Crop, resize, rotate with visual feedback
- **Undo/Redo System**: Full operation history management

---

## 🖥️ System Requirements

### Operating System
- **Windows 10/11** (64-bit)
- Windows Server 2019/2022

### Hardware
- **Processor**: Multi-core CPU (Intel Core i5 or AMD Ryzen 5+)
- **RAM**: 8 GB minimum, 16 GB recommended
- **Storage**: 10 GB free space (5 GB for dependencies, 5 GB for build)
- **Display**: 1920x1080 minimum resolution

### Software Prerequisites
- **Visual Studio Build Tools 2022** (MSVC v143, C++17)
- **Git** (for cloning repository)
- **CMake** 3.20+ (optional, for custom builds)
- **PowerShell** 5.1+ (for build scripts)

---

## 📥 Step 1: Clone Repository

### Using Git Command Line
```powershell
# Navigate to your desired directory
cd F:\

# Clone the repository
git clone https://github.com/yourusername/Naghuma-Toolbox.git
cd "Naghuma Toolbox"
```

### Using GitHub Desktop
1. Open GitHub Desktop
2. File → Clone Repository
3. Enter URL: `https://github.com/yourusername/Naghuma-Toolbox.git`
4. Choose local path: `F:\Naghuma Toolbox`
5. Click **Clone**

---

## 🔧 Step 2: Install Dependencies

### 2.1 Visual Studio Build Tools 2022

**Download:** [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/)

**Installation Steps:**
1. Run installer: `vs_BuildTools.exe`
2. Select **Workloads**:
   - ✅ Desktop development with C++
   - ✅ C++ CMake tools for Windows
   - ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)
   - ✅ Windows 10/11 SDK (10.0.22621.0 or later)
3. Individual Components:
   - ✅ C++ ATL for latest v143 build tools (x64/x86)
   - ✅ C++ MFC for latest v143 build tools (x64/x86)
4. Install location: `F:\Build Tools\` (or default `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools`)
5. Click **Install** and wait for completion (~6 GB download)

**Verification:**
```powershell
# Check MSBuild
"F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe" -version
# Expected output: Microsoft (R) Build Engine version 17.x.x

# Check MSVC compiler
cl.exe
# Expected: Microsoft (R) C/C++ Optimizing Compiler Version 19.44.x
```

---

### 2.2 Qt 6.7.3 (MSVC 2019 64-bit)

**Download:** [Qt Online Installer](https://www.qt.io/download-qt-installer)

**Installation Steps:**
1. Run `qt-online-installer-windows-x64.exe`
2. Log in with Qt account (create free account if needed)
3. Select **Custom Installation**
4. Choose Components:
   - ✅ Qt 6.7.3
     - ✅ MSVC 2019 64-bit
     - ✅ Qt 5 Compatibility Module
     - ✅ Additional Libraries:
       - Qt Image Formats
       - Qt SVG
       - Qt Charts
5. Install location: `C:\Qt\` (default) or `F:\Qt\`
6. Click **Install** (~5 GB download)

**Environment Variables:**
```powershell
# Add Qt bin to PATH
$qtPath = "C:\Qt\6.7.3\msvc2019_64\bin"
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$qtPath", [System.EnvironmentVariableTarget]::User)

# Set Qt root
[Environment]::SetEnvironmentVariable("Qt6_DIR", "C:\Qt\6.7.3\msvc2019_64", [System.EnvironmentVariableTarget]::User)
```

**Verification:**
```powershell
# Check Qt version
qmake --version
# Expected: QMake version 3.1, Using Qt version 6.7.3

# Check Qt tools
moc -v
# Expected: moc 6.7.3
```

---

### 2.3 OpenCV 4.3.0 (Pre-built)

**Download:** [OpenCV 4.3.0 Windows](https://github.com/opencv/opencv/releases/tag/4.3.0)

**Installation Steps:**
1. Download: `opencv-4.3.0-vc14_vc15.exe`
2. Extract to: `F:\OpenCV\` (creates `F:\OpenCV\opencv\`)
3. OpenCV structure:
   ```
   F:\OpenCV\opencv\
   ├── build\
   │   ├── x64\vc15\
   │   │   ├── bin\         # DLLs (opencv_world430d.dll, opencv_world430.dll)
   │   │   └── lib\         # Import libraries
   │   └── include\         # Header files
   └── sources\             # Source code (optional)
   ```

**Environment Variables:**
```powershell
# Add OpenCV to PATH
$opencvPath = "F:\OpenCV\opencv\build\x64\vc15\bin"
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$opencvPath", [System.EnvironmentVariableTarget]::User)

# Set OpenCV root
[Environment]::SetEnvironmentVariable("OPENCV_DIR", "F:\OpenCV\opencv\build", [System.EnvironmentVariableTarget]::User)
```

**Verification:**
```powershell
# Check OpenCV DLLs
Test-Path "F:\OpenCV\opencv\build\x64\vc15\bin\opencv_world430d.dll"
# Expected: True
```

---

### 2.4 Tesseract OCR (Optional)

**Download:** [Tesseract Windows Installer](https://github.com/UB-Mannheim/tesseract/wiki)

**Installation Steps:**
1. Download: `tesseract-ocr-w64-setup-v5.3.0.exe` (or latest)
2. Install to: `C:\Program Files\Tesseract-OCR\`
3. During installation, select **Additional language data** for your languages:
   - ✅ English (eng.traineddata)
   - ✅ Arabic (ara.traineddata)
   - ✅ French, Spanish, etc. (optional)
4. Install location contains:
   ```
   C:\Program Files\Tesseract-OCR\
   ├── tesseract.exe
   ├── tessdata\            # Language data files
   │   ├── eng.traineddata
   │   ├── ara.traineddata
   │   └── osd.traineddata
   └── libtesseract*.dll
   ```

**Environment Variables:**
```powershell
# Add Tesseract to PATH
$tesseractPath = "C:\Program Files\Tesseract-OCR"
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$tesseractPath", [System.EnvironmentVariableTarget]::User)

# Set Tesseract data path
[Environment]::SetEnvironmentVariable("TESSDATA_PREFIX", "C:\Program Files\Tesseract-OCR\tessdata", [System.EnvironmentVariableTarget]::User)
```

**Verification:**
```powershell
tesseract --version
# Expected: tesseract 5.3.0
```

**Copy Tesseract DLLs to Project:**
```powershell
cd "F:\Naghuma Toolbox"
Copy-Item "C:\Program Files\Tesseract-OCR\*.dll" -Destination "x64\Debug\" -Force
Copy-Item "C:\Program Files\Tesseract-OCR\tessdata" -Destination "x64\Debug\tessdata" -Recurse -Force
```

---

### 2.5 Font Awesome 6 (Embedded)

**Already Included** in `resources/fonts/fontawesome-webfont.ttf` (no action required)

**Manual Download (if needed):**
1. Visit: [Font Awesome Free](https://fontawesome.com/download)
2. Download: Font Awesome Free 6.x.x
3. Extract and copy: `webfonts/fa-solid-900.ttf` → `F:\Naghuma Toolbox\resources\fonts\fontawesome-webfont.ttf`

---

## 🏗️ Step 3: Build Configuration

### 3.1 Update Project Paths (if different from F:\)

Edit `Naghuma Toolbox.vcxproj` if your paths differ:

```xml
<!-- Update these paths based on your installation -->
<PropertyGroup>
  <!-- Qt paths -->
  <QtDir>C:\Qt\6.7.3\msvc2019_64</QtDir>
  
  <!-- OpenCV paths -->
  <OpenCVDir>F:\OpenCV\opencv\build</OpenCVDir>
  
  <!-- Build Tools paths -->
  <MSBuildDir>F:\Build Tools\MSBuild\Current\Bin</MSBuildDir>
</PropertyGroup>

<!-- Update include directories -->
<ItemDefinitionGroup>
  <ClCompile>
    <AdditionalIncludeDirectories>
      $(ProjectDir)include;
      C:\Qt\6.7.3\msvc2019_64\include;
      F:\OpenCV\opencv\build\include;
      %(AdditionalIncludeDirectories)
    </AdditionalIncludeDirectories>
  </ClCompile>
</ItemDefinitionGroup>

<!-- Update library directories -->
<ItemDefinitionGroup>
  <Link>
    <AdditionalLibraryDirectories>
      C:\Qt\6.7.3\msvc2019_64\lib;
      F:\OpenCV\opencv\build\x64\vc15\lib;
      %(AdditionalLibraryDirectories)
    </AdditionalLibraryDirectories>
  </Link>
</ItemDefinitionGroup>
```

---

### 3.2 Generate Qt MOC Files

Qt requires MOC (Meta-Object Compiler) for Q_OBJECT classes:

```powershell
# Navigate to project directory
cd "F:\Naghuma Toolbox"

# Run MOC generation script (if provided)
.\build_scripts\generate_moc.ps1

# Or manually generate MOC files
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
& $qtMoc "include\MainWindow.h" -o "src\moc_MainWindow.cpp"
& $qtMoc "include\ImageCanvas.h" -o "src\moc_ImageCanvas.cpp"
& $qtMoc "include\SelectionTool.h" -o "src\moc_SelectionTool.cpp"
& $qtMoc "include\ROISelectionWidget.h" -o "src\moc_ROISelectionWidget.cpp"
& $qtMoc "include\ThresholdDialog.h" -o "src\moc_ThresholdDialog.cpp"
& $qtMoc "include\HistogramDialog.h" -o "src\moc_HistogramDialog.cpp"
& $qtMoc "include\HuffmanDialog.h" -o "src\moc_HuffmanDialog.cpp"
& $qtMoc "include\OCRDialog.h" -o "src\moc_OCRDialog.cpp"
& $qtMoc "include\CannyDialog.h" -o "src\moc_CannyDialog.cpp"
```

---

### 3.3 Build with MSBuild

#### Debug Build (Recommended for Development)
```powershell
# Open Developer PowerShell (not required if PATH is set)
# Or use standard PowerShell with full MSBuild path

$msbuild = "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe"

# Clean build
& $msbuild "Naghuma Toolbox.vcxproj" /t:Clean /p:Configuration=Debug /p:Platform=x64

# Build project
& $msbuild "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Debug /p:Platform=x64 /m

# Expected output:
# Build succeeded.
#     0 Warning(s)
#     0 Error(s)
```

#### Release Build (Optimized for Production)
```powershell
$msbuild = "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe"

# Clean build
& $msbuild "Naghuma Toolbox.vcxproj" /t:Clean /p:Configuration=Release /p:Platform=x64

# Build project with optimizations
& $msbuild "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Release /p:Platform=x64 /m /p:Optimization=MaxSpeed

# Expected output:
# Build succeeded.
```

**Build Artifacts:**
- Debug: `F:\Naghuma Toolbox\x64\Debug\Naghuma Toolbox.exe`
- Release: `F:\Naghuma Toolbox\x64\Release\Naghuma Toolbox.exe`

---

### 3.4 Copy Required DLLs

#### Debug Configuration
```powershell
cd "F:\Naghuma Toolbox"

# Qt DLLs
$qtBin = "C:\Qt\6.7.3\msvc2019_64\bin"
$debugDir = "x64\Debug"

Copy-Item "$qtBin\Qt6Cored.dll" -Destination $debugDir -Force
Copy-Item "$qtBin\Qt6Guid.dll" -Destination $debugDir -Force
Copy-Item "$qtBin\Qt6Widgetsd.dll" -Destination $debugDir -Force
Copy-Item "$qtBin\Qt6Svgd.dll" -Destination $debugDir -Force

# Qt platform plugin (REQUIRED)
New-Item -Path "$debugDir\platforms" -ItemType Directory -Force
Copy-Item "$qtBin\..\plugins\platforms\qwindowsd.dll" -Destination "$debugDir\platforms\" -Force

# OpenCV DLLs
$opencvBin = "F:\OpenCV\opencv\build\x64\vc15\bin"
Copy-Item "$opencvBin\opencv_world430d.dll" -Destination $debugDir -Force

# Tesseract DLLs (if OCR enabled)
if (Test-Path "C:\Program Files\Tesseract-OCR") {
    Copy-Item "C:\Program Files\Tesseract-OCR\*.dll" -Destination $debugDir -Force
    Copy-Item "C:\Program Files\Tesseract-OCR\tessdata" -Destination "$debugDir\tessdata" -Recurse -Force
}
```

#### Release Configuration
```powershell
# Qt DLLs (Release versions without 'd' suffix)
$qtBin = "C:\Qt\6.7.3\msvc2019_64\bin"
$releaseDir = "x64\Release"

Copy-Item "$qtBin\Qt6Core.dll" -Destination $releaseDir -Force
Copy-Item "$qtBin\Qt6Gui.dll" -Destination $releaseDir -Force
Copy-Item "$qtBin\Qt6Widgets.dll" -Destination $releaseDir -Force
Copy-Item "$qtBin\Qt6Svg.dll" -Destination $releaseDir -Force

New-Item -Path "$releaseDir\platforms" -ItemType Directory -Force
Copy-Item "$qtBin\..\plugins\platforms\qwindows.dll" -Destination "$releaseDir\platforms\" -Force

# OpenCV DLLs (Release version)
$opencvBin = "F:\OpenCV\opencv\build\x64\vc15\bin"
Copy-Item "$opencvBin\opencv_world430.dll" -Destination $releaseDir -Force

# Tesseract DLLs
if (Test-Path "C:\Program Files\Tesseract-OCR") {
    Copy-Item "C:\Program Files\Tesseract-OCR\*.dll" -Destination $releaseDir -Force
    Copy-Item "C:\Program Files\Tesseract-OCR\tessdata" -Destination "$releaseDir\tessdata" -Recurse -Force
}
```

**Automated Script:**
Use provided script: `build_scripts\copy_all_tesseract_dlls.bat`

---

## 🚀 Step 4: Run Application

### From PowerShell
```powershell
cd "F:\Naghuma Toolbox\x64\Debug"
.\Naghuma' 'Toolbox.exe
```

### From File Explorer
1. Navigate to: `F:\Naghuma Toolbox\x64\Debug\`
2. Double-click: `Naghuma Toolbox.exe`

### Expected Output
```
Font Awesome initialized successfully!
All icons loaded successfully!
Naghuma Toolbox v2.0.0 started.
```

---

## 🧪 Step 5: Verify Functionality

### Basic Tests

1. **Load Image**
   - File → Open Image
   - Select test image (JPG/PNG)
   - Image appears in left canvas

2. **Grayscale Conversion**
   - Process → Convert to Grayscale
   - Processed image appears in right canvas

3. **Selection Tool** (NEW in v2.0)
   - Analysis → Selection Tool → Rectangle Selection
   - Press **Ctrl+S** to activate
   - Click and drag on right canvas (grayscale image)
   - Green overlay (60% opacity) appears
   - Release mouse to complete selection

4. **Apply Filter to Selection**
   - Create selection (steps above)
   - Filters → Blur → Gaussian Blur
   - Filter applies only to selected region

5. **Save Selection as Layer**
   - Create selection
   - Analysis → Save Selection as Layer
   - Check right sidebar → Layers panel
   - Layer appears: "Selection: Rectangle ([pixels] pixels, [%]%)"

6. **Edge Detection**
   - Process → Convert to Grayscale
   - Analysis → Edge Detection → Canny
   - Adjust thresholds, click **Apply**

7. **OCR (if Tesseract installed)**
   - Load image with text
   - Analysis → OCR
   - Select language, click **Recognize Text**
   - Extracted text appears in dialog

8. **Histogram**
   - Analysis → Histogram
   - RGB histograms displayed
   - Click **Equalize** to enhance

9. **Compression**
   - Tools → Huffman Compression → Compress
   - Save compressed `.huff` file
   - Tools → Huffman Compression → Decompress
   - Load compressed file, decompress

10. **Undo/Redo**
    - Perform any operation
    - Edit → Undo (Ctrl+Z)
    - Edit → Redo (Ctrl+Y)

---

## 📁 Project Structure

```
F:\Naghuma Toolbox\
├── include/                    # Header files
│   ├── MainWindow.h           # Main window class
│   ├── ImageCanvas.h          # Custom image display widget
│   ├── SelectionTool.h        # NEW: 5-method selection tool
│   ├── ImageProcessor.h       # Core image processing algorithms
│   ├── LayerManager.h         # Layer system management
│   ├── RightSidebarWidget.h   # Layers/properties sidebar
│   ├── ROISelectionWidget.h   # ROI crop tool
│   ├── ThresholdDialog.h      # Threshold dialog
│   ├── HistogramDialog.h      # Histogram display
│   ├── HuffmanDialog.h        # Huffman compression
│   ├── OCRDialog.h            # OCR interface
│   └── CannyDialog.h          # Canny edge detector
├── src/                       # Source files
│   ├── main.cpp               # Application entry point
│   ├── MainWindow.cpp         # Main window implementation (3328 lines)
│   ├── SelectionTool.cpp      # NEW: Selection tool (430+ lines)
│   ├── ImageProcessor.cpp     # Image processing logic (2500+ lines)
│   ├── LayerManager.cpp       # Layer operations
│   ├── RightSidebarWidget.cpp # Sidebar UI
│   ├── ImageCanvas.cpp        # Canvas rendering
│   ├── ROISelectionWidget.cpp # ROI widget
│   ├── HuffmanDialog.cpp      # Compression dialog
│   ├── OCRDialog.cpp          # OCR dialog
│   ├── CannyDialog.cpp        # Canny dialog
│   ├── moc_MainWindow.cpp     # Qt MOC generated (auto)
│   ├── moc_SelectionTool.cpp  # Qt MOC generated (auto)
│   └── [other moc files]      # Qt MOC generated (auto)
├── lib/                       # Custom libraries
│   ├── compression/           
│   │   ├── HuffmanCoding.h    # Huffman algorithm
│   │   └── HuffmanCoding.cpp
│   └── ocr/
│       ├── TextRecognition.h  # OCR wrapper
│       └── TextRecognition.cpp
├── resources/                 # Application resources
│   ├── fonts/
│   │   └── fontawesome-webfont.ttf  # Icon font
│   ├── icons/                 # Custom icons (if any)
│   └── images/                # Sample images
├── documentation/             # Documentation files
│   └── guides/                # User/developer guides
├── build_scripts/             # Build automation scripts
│   ├── generate_moc.ps1       # MOC generation
│   ├── copy_all_tesseract_dlls.bat  # DLL copying
│   └── build_and_run.ps1      # Build + run
├── x64/                       # Build output
│   ├── Debug/                 # Debug binaries
│   │   ├── Naghuma Toolbox.exe
│   │   ├── *.dll              # Qt/OpenCV/Tesseract DLLs
│   │   ├── platforms/         # Qt platform plugin
│   │   └── tessdata/          # Tesseract language data
│   └── Release/               # Release binaries
├── backups/                   # Project file backups
├── Naghuma Toolbox.vcxproj    # Visual Studio project file
├── Naghuma Toolbox.vcxproj.filters  # Project filters
├── Naghuma Toolbox.sln        # Solution file
├── USER_GUIDE.md              # User manual (v2.0.0)
├── SELECTION_TOOL_GUIDE.md    # Selection tool detailed guide
├── SELECTION_LAYERS_GUIDE.md  # Layer integration guide
├── SELECTION_QUICK_REF.md     # Quick reference card
├── LICENSE                    # MIT License
└── README.md                  # Project overview
```

---

## ⚙️ Configuration Files

### CppProperties.json (VS Code IntelliSense)
Located at: `.vscode/CppProperties.json`

```json
{
  "configurations": [
    {
      "name": "x64-Debug",
      "includePath": [
        "${workspaceFolder}/include",
        "C:/Qt/6.7.3/msvc2019_64/include",
        "F:/OpenCV/opencv/build/include",
        "C:/Program Files/Tesseract-OCR/include"
      ],
      "defines": [
        "_DEBUG",
        "UNICODE",
        "_UNICODE",
        "QT_CORE_LIB",
        "QT_GUI_LIB",
        "QT_WIDGETS_LIB"
      ],
      "compilerPath": "F:/Build Tools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "windows-msvc-x64"
    }
  ]
}
```

### settings.json (VS Code Settings)
Located at: `.vscode/settings.json`

```json
{
  "files.associations": {
    "*.h": "cpp",
    "*.cpp": "cpp"
  },
  "C_Cpp.default.cppStandard": "c++17",
  "C_Cpp.default.includePath": [
    "${workspaceFolder}/include",
    "C:/Qt/6.7.3/msvc2019_64/include",
    "F:/OpenCV/opencv/build/include"
  ]
}
```

---

## 🐛 Troubleshooting

### Issue 1: MSBuild Not Found
**Error:** `'MSBuild.exe' is not recognized as an internal or external command`

**Solution:**
```powershell
# Add MSBuild to PATH
$msbuildPath = "F:\Build Tools\MSBuild\Current\Bin"
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$msbuildPath", [System.EnvironmentVariableTarget]::User)

# Or use full path
& "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe" "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Debug /p:Platform=x64
```

---

### Issue 2: Qt Platform Plugin Missing
**Error:** `This application failed to start because no Qt platform plugin could be initialized.`

**Solution:**
```powershell
# Copy platform plugin
$qtPlugins = "C:\Qt\6.7.3\msvc2019_64\plugins"
$debugDir = "x64\Debug"

New-Item -Path "$debugDir\platforms" -ItemType Directory -Force
Copy-Item "$qtPlugins\platforms\qwindowsd.dll" -Destination "$debugDir\platforms\" -Force
```

---

### Issue 3: OpenCV DLL Not Found
**Error:** `opencv_world430d.dll was not found`

**Solution:**
```powershell
# Copy OpenCV DLL
$opencvBin = "F:\OpenCV\opencv\build\x64\vc15\bin"
Copy-Item "$opencvBin\opencv_world430d.dll" -Destination "x64\Debug\" -Force

# Or add to PATH
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$opencvBin", [System.EnvironmentVariableTarget]::User)
```

---

### Issue 4: MOC Files Missing
**Error:** `LNK2019: unresolved external symbol "public: virtual struct QMetaObject const * __cdecl MainWindow::metaObject(void)const "`

**Solution:**
```powershell
# Regenerate MOC files
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
& $qtMoc "include\MainWindow.h" -o "src\moc_MainWindow.cpp"

# Ensure moc_ files are included in project
# Check Naghuma Toolbox.vcxproj for:
# <ClCompile Include="src\moc_MainWindow.cpp" />
```

---

### Issue 5: Selection Tool Not Working
**Error:** No visual feedback, no selection overlay

**Checklist:**
1. **Grayscale Required:**
   - Process → Convert to Grayscale FIRST
   - Selection only works on processed (right) image

2. **Activate Selection Mode:**
   - Press **Ctrl+S** OR click **Select** toolbar button
   - Look for message: "Selection mode enabled"

3. **Mouse Events:**
   - Click and drag on **RIGHT canvas** (processed image)
   - NOT on left canvas (original image)

4. **Debug Output:**
   - Check console for: `"=== toggleSelectionMode called ==="`
   - If no output, selection mode not activating

---

### Issue 6: Tesseract OCR Fails
**Error:** `Failed to initialize Tesseract`

**Solution:**
```powershell
# Set TESSDATA_PREFIX environment variable
[Environment]::SetEnvironmentVariable("TESSDATA_PREFIX", "C:\Program Files\Tesseract-OCR\tessdata", [System.EnvironmentVariableTarget]::User)

# Copy tessdata to executable directory
Copy-Item "C:\Program Files\Tesseract-OCR\tessdata" -Destination "x64\Debug\tessdata" -Recurse -Force

# Verify language files exist
Test-Path "x64\Debug\tessdata\eng.traineddata"  # Should be True
```

---

### Issue 7: Build Fails with Heap Space Error
**Error:** `fatal error C1060: compiler is out of heap space`

**Solution:**
```powershell
# Increase compiler heap size
# Edit Naghuma Toolbox.vcxproj:
<ClCompile>
  <AdditionalOptions>/Zm200 %(AdditionalOptions)</AdditionalOptions>
</ClCompile>

# Or split large files into smaller modules
```

---

## 📚 Additional Resources

### Official Documentation
- [Qt 6.7 Documentation](https://doc.qt.io/qt-6.7/)
- [OpenCV 4.3.0 Documentation](https://docs.opencv.org/4.3.0/)
- [Tesseract OCR Wiki](https://github.com/tesseract-ocr/tesseract/wiki)

### Project Documentation
- [USER_GUIDE.md](USER_GUIDE.md) - Complete user manual
- [SELECTION_TOOL_GUIDE.md](SELECTION_TOOL_GUIDE.md) - Selection tool details
- [SELECTION_LAYERS_GUIDE.md](SELECTION_LAYERS_GUIDE.md) - Layer system guide
- [SELECTION_QUICK_REF.md](SELECTION_QUICK_REF.md) - Quick reference

### Community & Support
- GitHub Issues: [Report bugs/requests](https://github.com/yourusername/Naghuma-Toolbox/issues)
- Discussions: [Ask questions](https://github.com/yourusername/Naghuma-Toolbox/discussions)

---

## 🔄 Update Workflow

### Pulling Latest Changes
```powershell
cd "F:\Naghuma Toolbox"

# Fetch latest changes
git fetch origin

# View changes
git log HEAD..origin/master --oneline

# Pull and merge
git pull origin master

# Regenerate MOC files (if headers changed)
.\build_scripts\generate_moc.ps1

# Rebuild project
& "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe" "Naghuma Toolbox.vcxproj" /t:Rebuild /p:Configuration=Debug /p:Platform=x64
```

---

## 🎯 Development Quick Start

### For Contributors

1. **Clone & Build** (10 minutes)
   ```powershell
   git clone https://github.com/yourusername/Naghuma-Toolbox.git
   cd "Naghuma Toolbox"
   .\build_scripts\generate_moc.ps1
   & "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe" "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Debug /p:Platform=x64
   ```

2. **Copy DLLs** (2 minutes)
   ```powershell
   .\build_scripts\copy_all_tesseract_dlls.bat
   ```

3. **Run & Test** (1 minute)
   ```powershell
   .\x64\Debug\Naghuma' 'Toolbox.exe
   ```

Total setup time: **~15 minutes** (excluding dependency downloads)

---

## 📦 Distribution Package

### Creating Standalone Installer

1. **Build Release:**
   ```powershell
   & "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe" "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Release /p:Platform=x64
   ```

2. **Collect Dependencies:**
   ```powershell
   $releaseDir = "x64\Release"
   $distDir = "Distribution\Naghuma Toolbox v2.0"
   
   New-Item -Path $distDir -ItemType Directory -Force
   
   # Copy executable
   Copy-Item "$releaseDir\Naghuma Toolbox.exe" -Destination $distDir
   
   # Copy Qt DLLs
   Copy-Item "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Core.dll" -Destination $distDir
   Copy-Item "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Gui.dll" -Destination $distDir
   Copy-Item "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Widgets.dll" -Destination $distDir
   
   # Copy platform plugin
   New-Item -Path "$distDir\platforms" -ItemType Directory -Force
   Copy-Item "C:\Qt\6.7.3\msvc2019_64\plugins\platforms\qwindows.dll" -Destination "$distDir\platforms\"
   
   # Copy OpenCV DLL
   Copy-Item "F:\OpenCV\opencv\build\x64\vc15\bin\opencv_world430.dll" -Destination $distDir
   
   # Copy documentation
   Copy-Item "USER_GUIDE.md" -Destination $distDir
   Copy-Item "LICENSE" -Destination $distDir
   ```

3. **Create Installer (Optional):**
   - Use [Inno Setup](https://jrsoftware.org/isinfo.php)
   - Or [WiX Toolset](https://wixtoolset.org/)
   - Or create ZIP archive:
     ```powershell
     Compress-Archive -Path $distDir -DestinationPath "Naghuma_Toolbox_v2.0_Windows_x64.zip"
     ```

---

## ✅ Final Checklist

Before deploying or sharing:

- [ ] All dependencies installed (Qt, OpenCV, Build Tools)
- [ ] Environment variables configured (PATH, Qt6_DIR, OPENCV_DIR)
- [ ] MOC files generated for all Q_OBJECT classes
- [ ] Project builds without errors (Debug + Release)
- [ ] All required DLLs copied to output directory
- [ ] Platform plugin (`qwindows.dll`) in `platforms/` subdirectory
- [ ] Tesseract DLLs + `tessdata/` folder copied (if OCR enabled)
- [ ] Application runs and displays Font Awesome icons
- [ ] Selection Tool works (Ctrl+S, grayscale image, green overlay)
- [ ] All 5 selection methods tested (Rectangle, Polygon, Magic Wand, Threshold, Edge-Based)
- [ ] Layer system functional (save/load selections)
- [ ] Documentation reviewed (USER_GUIDE.md, SELECTION_TOOL_GUIDE.md)

---

## 📝 Version History

### v2.0.0 (December 22, 2025) - Selection Tool Release
- ✨ Added 5-method Selection Tool (Rectangle, Polygon, Magic Wand, Threshold, Edge-Based)
- 💾 Integrated selections with Layer System
- 🎨 Visual feedback: 60% green overlay + 3px red border
- 📊 Selection statistics (pixel count, coverage %)
- 🔧 Grayscale requirement validation
- 🧹 Removed deprecated Brush Tool
- 📚 Comprehensive documentation (4 new guides)
- 🛠️ Toolbar cleanup (removed 5 redundant buttons)

### v1.x.x (Prior Releases)
- Image processing filters (50+ algorithms)
- Edge detection (6 methods)
- Histogram analysis
- OCR integration (Tesseract)
- Huffman compression
- ROI tools
- Undo/Redo system
- Layer management

---

## 🤝 Contributing

Contributions welcome! See [CONTRIBUTING.md](documentation/guides/CONTRIBUTING.md) for guidelines.

**Quick Contribution Steps:**
1. Fork repository
2. Create feature branch: `git checkout -b feature/AmazingFeature`
3. Commit changes: `git commit -m "Add AmazingFeature"`
4. Push to branch: `git push origin feature/AmazingFeature`
5. Open Pull Request

---

## 📄 License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) file for details.

---

## 👥 Authors

- **Original Author** - Initial work and v1.x development
- **Contributors** - See [GitHub Contributors](https://github.com/yourusername/Naghuma-Toolbox/contributors)

---

## 🙏 Acknowledgments

- **Qt Framework** - Cross-platform GUI toolkit
- **OpenCV** - Computer vision library
- **Tesseract OCR** - Text recognition engine
- **Font Awesome** - Icon toolkit
- **Community** - Bug reports, feature requests, and contributions

---

## 📞 Support

- **Issues:** [GitHub Issues](https://github.com/yourusername/Naghuma-Toolbox/issues)
- **Discussions:** [GitHub Discussions](https://github.com/yourusername/Naghuma-Toolbox/discussions)
- **Email:** your.email@example.com (if applicable)

---

**🎉 You're all set! Enjoy using Naghuma Toolbox v2.0 🎉**
