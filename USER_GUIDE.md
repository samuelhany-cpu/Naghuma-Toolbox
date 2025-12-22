# Naghuma Toolbox - Complete User Guide

## Table of Contents
1. [Overview](#overview)
2. [Main Menu Bar](#main-menu-bar)
3. [Toolbar Buttons](#toolbar-buttons)
4. [Processing Tabs](#processing-tabs)
5. [Smoothing/Blur Features](#smoothingblur-features)
6. [Keyboard Shortcuts](#keyboard-shortcuts)

---

## Overview

**Naghuma Toolbox** is a comprehensive image processing application built with C++, Qt 6, and OpenCV. It provides professional-grade tools for image analysis, enhancement, transformation, and optical character recognition (OCR).

**Key Features:**
- Real-time image processing
- Layer-based editing
- Histogram analysis
- OCR (Optical Character Recognition)
- ROI (Region of Interest) tools
- Undo/Redo support
- Export capabilities

---

## Main Menu Bar

### File Menu
| Option | Function | Shortcut |
|--------|----------|----------|
| **Open Image** | Load an image file (JPG, PNG, BMP, TIFF) | `Ctrl+O` |
| **Save** | Save current image to existing file | `Ctrl+S` |
| **Save As** | Save image to new file with options | `Ctrl+Shift+S` |
| **Export** | Export processed image with metadata | - |
| **Recent Files** | Quick access to recently opened images | - |
| **Exit** | Close the application | `Alt+F4` |

### Information Menu
| Option | Function |
|--------|----------|
| **Image Info** | Display image properties (dimensions, channels, bit depth) |
| **Metadata** | View EXIF data and image metadata |
| **Statistics** | Show pixel value statistics (min, max, mean, std dev) |

### Transform Menu
| Option | Function | Description |
|--------|----------|-------------|
| **Rotate 90° CW** | Rotate image clockwise | Quick 90° rotation |
| **Rotate 90° CCW** | Rotate counter-clockwise | Quick 90° rotation |
| **Rotate 180°** | Flip image upside down | Quick 180° rotation |
| **Flip Horizontal** | Mirror image left-right | Horizontal flip |
| **Flip Vertical** | Mirror image top-bottom | Vertical flip |
| **Custom Rotate** | Rotate by any angle | Specify custom angle |
| **Resize** | Change image dimensions | Scale or crop to size |
| **Crop** | Crop to selection | Remove unwanted areas |

### Histogram Menu
| Option | Function |
|--------|----------|
| **Show Histogram** | Display RGB/grayscale histogram |
| **Histogram Equalization** | Enhance contrast using histogram equalization |
| **Adaptive Histogram** | CLAHE (Contrast Limited Adaptive Histogram Equalization) |
| **Histogram Matching** | Match histogram to reference image |

### Process Menu
| Option | Function | Category |
|--------|----------|----------|
| **Grayscale** | Convert to grayscale | Color |
| **RGB to HSV** | Convert to HSV color space | Color |
| **RGB to LAB** | Convert to LAB color space | Color |
| **Invert Colors** | Negative image | Color |
| **Gaussian Blur** | Smooth image using Gaussian filter | **Smoothing** |
| **Median Blur** | Remove noise with median filter | **Smoothing** |
| **Bilateral Filter** | Edge-preserving smoothing | **Smoothing** |
| **Sharpen** | Enhance edges and details | Enhancement |
| **Edge Detection** | Canny edge detector | Feature |
| **Threshold** | Binary/adaptive thresholding | Segmentation |
| **Morphology** | Erosion, dilation, opening, closing | Morphology |

### Filters Menu
| Option | Function | Type |
|--------|----------|------|
| **Sobel** | Gradient-based edge detection | Edge |
| **Laplacian** | Second derivative edge detection | Edge |
| **Prewitt** | Prewitt edge operator | Edge |
| **Roberts Cross** | Roberts edge detection | Edge |
| **Gaussian Blur** | Standard blur (kernel 5x5) | **Smoothing** |
| **Box Filter** | Average blur | **Smoothing** |
| **Motion Blur** | Directional blur | Effect |
| **Emboss** | 3D emboss effect | Effect |
| **Unsharp Mask** | Advanced sharpening | Enhancement |

### Color Menu
| Option | Function |
|--------|----------|
| **Brightness/Contrast** | Adjust brightness and contrast |
| **Hue/Saturation** | Modify color properties |
| **Color Balance** | Adjust RGB channels |
| **Color Curves** | Advanced tone adjustment |
| **Auto White Balance** | Automatic color correction |
| **Gamma Correction** | Adjust gamma value |

### Segmentation Menu
| Option | Function |
|--------|----------|
| **Otsu Threshold** | Automatic threshold selection |
| **Adaptive Threshold** | Local threshold calculation |
| **Watershed** | Marker-based segmentation |
| **GrabCut** | Foreground extraction |
| **K-Means Clustering** | Color-based segmentation |

### Morphology Menu
| Option | Function | Description |
|--------|----------|-------------|
| **Erosion** | Shrink bright regions | Morphological erosion |
| **Dilation** | Expand bright regions | Morphological dilation |
| **Opening** | Erosion followed by dilation | Remove noise |
| **Closing** | Dilation followed by erosion | Fill gaps |
| **Morphological Gradient** | Edge detection | Outline boundaries |
| **Top Hat** | Extract small features | Bright features |
| **Black Hat** | Extract dark features | Dark features |

### FFT (Frequency Domain) Menu
| Option | Function |
|--------|----------|
| **FFT Transform** | Forward Fourier transform |
| **Inverse FFT** | Back to spatial domain |
| **Frequency Filter** | High/low pass filtering |
| **Phase Correlation** | Image alignment |

### Wavelet Menu
| Option | Function |
|--------|----------|
| **Wavelet Transform** | Multi-resolution analysis |
| **Wavelet Denoising** | Noise removal in wavelet domain |
| **Wavelet Compression** | Lossy compression |

### View Menu
| Option | Function | Shortcut |
|--------|----------|----------|
| **Zoom In** | Increase magnification | `Ctrl++` |
| **Zoom Out** | Decrease magnification | `Ctrl+-` |
| **Fit to Window** | Scale to fit display | `Ctrl+0` |
| **Actual Size** | 100% zoom | `Ctrl+1` |
| **Show Grid** | Display overlay grid | - |
| **Show Rulers** | Display rulers | - |
| **Fullscreen** | Toggle fullscreen mode | `F11` |

### Analysis Menu
| Option | Function |
|--------|----------|
| **Measure Distance** | Measure pixels between points |
| **Measure Angle** | Calculate angles |
| **Pixel Probe** | Inspect pixel values |
| **Profile Line** | Intensity profile along line |
| **Color Picker** | Sample colors from image |

### Selection Tool Menu (NEW!)
**Location:** Analysis > Selection Tool

The Selection Tool enables selective processing by allowing you to select specific areas and apply filters only to those regions.

#### Features:
| Option | Function | Shortcut |
|--------|----------|----------|
| **Toggle Selection Mode** | Enable/disable selection mode | `Ctrl+S` |
| **Rectangle Selection** | Click & drag rectangular selection | - |
| **Polygon Selection** | Multi-point lasso tool (right-click to close) | - |
| **Magic Wand** | Click color to select similar pixels | - |
| **Threshold Selection** | Select by intensity range (0-255) | - |
| **Edge-Based Selection** | Intelligent object detection | - |
| **Clear Selection** | Remove current selection | - |
| **Save Selection as Layer** | Save selection mask to layer panel | - |

#### How to Use Selection Tool:
1. **Load image** and convert to **grayscale** (required)
2. Press **Ctrl+S** or click **"Select"** toolbar button
3. Choose selection method from menu
4. Create selection:
   - **Rectangle**: Click and drag
   - **Polygon**: Click multiple points, right-click to close
   - **Magic Wand**: Click on color, set tolerance (0-255)
   - **Threshold**: Enter min/max intensity values
   - **Edge-Based**: Click inside object
5. **Green overlay** shows selected area
6. Apply any filter → affects **selected area only!**

#### Selection Visualization:
- **Green fill (60% opacity)**: Selected pixels
- **Red border (3px)**: Selection boundary
- **Statistics**: Shows pixel count and coverage percentage

#### Save to Layers:
- **Analysis → Selection Tool → Save Selection as Layer**
- Selection appears in Layers panel (right sidebar)
- Layer name: `"Selection: [Method] ([Pixels] pixels, [%]%)"`
- Example: `"Selection: Rectangle (1500 pixels, 4.5%)"`

#### Requirements:
- ⚠️ **Grayscale image required** (single channel)
- ⚠️ **Process image first** (convert original to grayscale)
- ✅ Works on **processed image** (right panel)

---

## Toolbar Buttons

### Top Toolbar (Quick Access)

| Button | Icon | Function | Location |
|--------|------|----------|----------|
| **Load Image** | 📁 | Open image file | Left |
| **Save** | 💾 | Save current image | Left |
| **Reset** | 🔄 | Reset to original | Left |
| **Undo** | ↶ | Undo last action | Left |
| **Select** | ▭ | Toggle selection mode (Ctrl+S) | Left |
| **Grayscale** | ⬛ | Convert to grayscale | Center |
| **Blur** | ≋ | Apply Gaussian blur | Center |
| **Sharpen** | ⚡ | Enhance edges | Center |
| **Edge Detection** | 📐 | Canny edges | Center |
| **Crop** | ✂ | Crop tool | Right |
| **ROI** | ▭ | Region of Interest | Right |
| **Zoom In** | 🔍+ | Increase zoom | Right |
| **Zoom Out** | 🔍- | Decrease zoom | Right |

### Left Sidebar (Tool Palette)

| Tool | Icon | Function | Shortcut |
|------|------|----------|----------|
| **Select** | ➤ | Selection tool | `V` |
| **Move** | ✥ | Move layer | `M` |
| **Crop** | ✂ | Crop image | `C` |
| **Brush** | 🖌 | Paint on image | `B` |
| **Eraser** | ⌨ | Erase pixels | `E` |
| **Color Picker** | 💧 | Sample color | `I` |
| **Zoom** | 🔍 | Zoom tool | `Z` |
| **Hand** | ✋ | Pan image | `H` |

---

## Processing Tabs

### 1. Histogram Tab
**Location:** Right sidebar

**Components:**
- **RGB Histogram Display** - Shows distribution of red, green, blue channels
- **Grayscale Histogram** - Shows intensity distribution
- **Cumulative Histogram** - Cumulative distribution function

**Controls:**
- **Channel Selector** - Toggle R/G/B/Grayscale display
- **Log Scale** - Logarithmic y-axis for better visualization
- **Equalize** - Apply histogram equalization

### 2. Layers Tab
**Location:** Right sidebar

**Features:**
- **Layer List** - All image layers in stack
- **Visibility Toggle** - Show/hide layers
- **Opacity Slider** - Adjust layer transparency
- **Blend Mode** - Normal, Multiply, Screen, Overlay, etc.

**Buttons:**
- **New Layer** - Create blank layer
- **Duplicate** - Copy current layer
- **Merge Down** - Combine with layer below
- **Delete** - Remove layer

### 3. OCR Tab
**Location:** Analysis > OCR or Process menu

**Optical Character Recognition Features:**

**Preprocessing Options:**
- ☑ **Convert to Grayscale** - Convert to single channel
- ☑ **Apply Adaptive Threshold** - Binarize text
- ☑ **Denoise Image** - Remove noise
- ☑ **Enhance Contrast (CLAHE)** - Improve readability

**Buttons:**
- **Recognize Text** - Perform OCR on image
- **Copy Text** - Copy recognized text to clipboard
- **Save Text** - Save text to file
- **Export with Boxes** - Save with bounding boxes

**Status:**
- **Confidence** - OCR confidence percentage
- **Recognition Results** - Extracted text display

**Engine:** OCR.space Cloud API (25,000 requests/month free)

### 4. ROI (Region of Interest) Tab

**Tools:**
- **Rectangle ROI** - Select rectangular region
- **Ellipse ROI** - Select circular/elliptical region
- **Polygon ROI** - Freeform selection
- **Magic Wand** - Color-based selection

**Operations:**
- **Crop to ROI** - Extract selected region
- **Process ROI** - Apply filter only to selection
- **Invert Selection** - Select everything except ROI
- **Clear Selection** - Remove ROI

### 5. Adjustments Tab

**Sliders:**
- **Brightness** - -100 to +100
- **Contrast** - 0 to 200
- **Saturation** - 0 to 200
- **Hue** - -180° to +180°
- **Gamma** - 0.1 to 3.0
- **Temperature** - Cool to Warm

**Auto Adjust:**
- **Auto Levels** - Automatic contrast
- **Auto Color** - Automatic white balance
- **Auto Enhance** - Combined enhancement

---

## Smoothing/Blur Features

### 📍 **Where to Find Smoothing:**

#### **1. Process Menu → Gaussian Blur**
- **Path:** Process → Gaussian Blur
- **Shortcut:** None
- **Function:** Standard Gaussian blur with kernel size 5x5
- **Use Case:** General smoothing, noise reduction

#### **2. Process Menu → Median Blur**
- **Path:** Process → Median Blur
- **Function:** Median filter for noise removal
- **Use Case:** Salt-and-pepper noise, preserves edges

#### **3. Process Menu → Bilateral Filter**
- **Path:** Process → Bilateral Filter
- **Function:** Edge-preserving smoothing
- **Use Case:** Smoothing while keeping edges sharp

#### **4. Filters Menu → Gaussian Blur**
- **Path:** Filters → Gaussian Blur
- **Shortcut:** Accessible via toolbar "Blur" button
- **Function:** Same as Process menu version
- **Implementation Location:** `src/MainWindow.cpp` line 1286

#### **5. Filters Menu → Box Filter**
- **Path:** Filters → Box Filter
- **Function:** Simple averaging filter
- **Use Case:** Quick blur, computationally efficient

#### **6. Filters Menu → Motion Blur**
- **Path:** Filters → Motion Blur
- **Function:** Directional blur effect
- **Use Case:** Creating motion effects

#### **7. Noise Removal Dialog**
- **Path:** Process → Noise Removal → Gaussian Smoothing
- **Function:** Advanced smoothing with parameter control
- **Use Case:** Fine-tuned noise removal

#### **8. OCR Preprocessing**
- **Path:** Analysis → OCR → Denoise Image (checkbox)
- **Function:** Automatic denoising before text recognition
- **Use Case:** Improving OCR accuracy

### **Smoothing Implementation Details:**

**Code Location:** `src/ImageProcessor.cpp` line 22
```cpp
void ImageProcessor::applyGaussianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize)
```

**Available Methods:**
1. **Gaussian Blur** - `cv::GaussianBlur()` - Normal distribution smoothing
2. **Median Blur** - `cv::medianBlur()` - Median-based denoising
3. **Bilateral** - `cv::bilateralFilter()` - Edge-preserving smoothing
4. **Box Filter** - `cv::blur()` - Simple averaging
5. **Motion Blur** - Custom kernel directional blur

---

## Keyboard Shortcuts

### File Operations
| Shortcut | Action |
|----------|--------|
| `Ctrl+O` | Open Image |
| `Ctrl+S` | Save |
| `Ctrl+Shift+S` | Save As |
| `Ctrl+W` | Close Image |
| `Ctrl+Q` | Quit Application |

### Edit Operations
| Shortcut | Action |
|----------|--------|
| `Ctrl+Z` | Undo |
| `Ctrl+Y` or `Ctrl+Shift+Z` | Redo |
| `Ctrl+C` | Copy |
| `Ctrl+X` | Cut |
| `Ctrl+V` | Paste |
| `Delete` | Delete Selection |

### View Operations
| Shortcut | Action |
|----------|--------|
| `Ctrl++` | Zoom In |
| `Ctrl+-` | Zoom Out |
| `Ctrl+0` | Fit to Window |
| `Ctrl+1` | Actual Size (100%) |
| `F11` | Fullscreen |
| `Space` | Pan (hold and drag) |

### Selection Tools
| Shortcut | Tool |
|----------|------|
| `V` | Selection Tool |
| `M` | Move Tool |
| `C` | Crop Tool |
| `R` | Rectangle ROI |
| `E` | Ellipse ROI |
| `L` | Polygon ROI |

### Processing
| Shortcut | Action |
|----------|--------|
| `Ctrl+G` | Convert to Grayscale |
| `Ctrl+I` | Invert Colors |
| `Ctrl+L` | Levels/Histogram |
| `Ctrl+T` | Transform |

---

## Tips & Best Practices

### Smoothing/Blur Best Practices:
1. **For Noise Reduction:**
   - Use **Median Blur** for salt-and-pepper noise
   - Use **Gaussian Blur** for general noise
   - Use **Bilateral Filter** to preserve edges

2. **For Image Enhancement:**
   - Apply smoothing before edge detection
   - Use **CLAHE** after smoothing for better contrast
   - Combine with **Unsharp Mask** for detail preservation

3. **For OCR:**
   - Enable **Denoise Image** in OCR preprocessing
   - Combine with **Adaptive Threshold** for text clarity
   - Use **Bilateral Filter** on scanned documents

### General Workflow:
1. **Open Image** → Load your image file
2. **Analyze** → Check histogram and image info
3. **Preprocess** → Apply smoothing/denoising if needed
4. **Enhance** → Adjust brightness/contrast
5. **Process** → Apply desired filters/effects
6. **Save/Export** → Save your processed image

---

## OCR Usage Guide

### Basic OCR Workflow:
1. **Load Image** with text (documents, signs, labels)
2. **Open OCR Dialog** (Analysis → OCR)
3. **Select Preprocessing Options**:
   - Grayscale: ✓ (Recommended)
   - Threshold: ✓ (For printed text)
   - Denoise: ✓ (For scanned documents)
   - Contrast: ✓ (For low-quality images)
4. **Click "Recognize Text"**
5. **Wait for processing** (cloud API, 2-5 seconds)
6. **View Results** in text area
7. **Copy or Save** extracted text

### OCR Tips:
- Higher resolution = better accuracy
- Black text on white background works best
- Preprocessing improves recognition
- Supports English language
- Internet connection required (cloud-based)

---

## Technical Specifications

**Supported Image Formats:**
- JPEG/JPG
- PNG
- BMP
- TIFF/TIF
- GIF (read-only)
- WebP

**System Requirements:**
- Windows 10/11 (64-bit)
- 4GB RAM minimum (8GB recommended)
- OpenGL 3.3+ compatible graphics
- Internet connection (for OCR features)

**Dependencies:**
- Qt 6.7.3
- OpenCV 4.3.0
- OCR.space API (cloud)
- Windows SDK

---

## Directory Organization

```
F:\Naghuma Toolbox\
├── src/                    # C++ source files
├── include/                # Header files
├── lib/                    # Library implementations
├── resources/              # Images, icons, fonts
├── build_scripts/          # Build automation scripts (*.ps1, *.bat)
├── documentation/          # User guides and docs
│   └── guides/            # Detailed guides
├── backups/               # Project backups
├── x64/Debug/             # Compiled executable
├── README.md              # Quick start guide
├── RUN_APP.bat           # Application launcher
└── USER_GUIDE.md         # This file
```

---

## Support & Resources

**Documentation:**
- This User Guide
- `README.md` - Quick start
- `CONTRIBUTING.md` - Development guide

**Code Locations:**
- Smoothing: `src/ImageProcessor.cpp` (line 22+)
- Main UI: `src/MainWindow.cpp`
- OCR Engine: `lib/ocr/TextRecognition.cpp`
- Dialogs: `src/*Dialog.cpp`

**Launch Application:**
- Double-click `RUN_APP.bat`
- Or run: `.\build_scripts\run_naghuma_toolbox.ps1`

---

**Version:** 2.0.0  
**Last Updated:** December 22, 2025  
**License:** See LICENSE file

**New in v2.0:**
- ✨ **Selection Tool** with 5 methods (Rectangle, Polygon, Magic Wand, Threshold, Edge-Based)
- 🎨 Visual selection overlay (green fill + red border)
- 💾 Save selections as layers
- 📊 Selection statistics and coverage percentage
- 🔧 Selective filter application to specific regions
