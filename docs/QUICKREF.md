# ?? Naghuma Toolbox - Quick Reference Guide

## ?? Color Scheme
- **Primary Pink**: #ff6b9d - Main accent color
- **Lavender**: #a29bfe - Secondary accent  
- **Peach**: #ffeaa7 - Tertiary accent
- **Mint**: #55efc4 - Success color
- **Dark Purple**: #1a1625 - Background

## ?? Features Overview

### ?? **File Operations**
```
Load Image:  File ? ??? Load Image
             Toolbar ? ??? Load Image
             
Save Image:  File ? ?? Save Image  
             Toolbar ? ?? Save

Reset:       File ? ?? Reset
             Toolbar ? ?? Reset
```

### ?? **Information & Analysis**
```
Image Info:    Information ? ?? Image Info
               Shows: Dimensions, channels, data type, min/max/mean values, file path

Pixel Info:    Information ? ?? Pixel Info  
               Query pixel values at specific coordinates

Statistics:    Information ? ?? Statistics
               Min, max, mean, standard deviation, dynamic range
```

### ?? **Geometric Transformations**
```
Translation:   Transform ? ?? Translation
               Move image horizontally (X) and vertically (Y)
               Range: ±half of image dimension

Rotation:      Transform ? ?? Rotation
               Rotate by angle: -180° to +180°

Skew:          Transform ? ?? Skew
               Apply predefined shear transformation

Zoom:          Transform ? ?? Zoom
               Scale factor: 0.5x to 3.0x

Flip Horiz:    Transform ? ?? Flip Horizontal
Flip Vert:     Transform ? ?? Flip Vertical  
Flip Both:     Transform ? ?? Flip Both
```

### ?? **Histogram Operations**
```
Show Histogram:      Histogram ? ?? Show Histogram
                     Beautiful visualization of pixel distribution
                     - Grayscale: Pink gradient
                     - Color: Multi-channel (Pink/Lavender/Yellow)

Equalization:        Histogram ? ?? Equalization
                     Enhance contrast automatically
                     Shows before/after comparison with statistics

Otsu Threshold:      Histogram ? ?? Otsu Thresholding
                     Automatic optimal binary thresholding
```

### ?? **Image Processing**
```
Grayscale:           Process ? ? Grayscale
                     Toolbar ? ? Grayscale
                     Convert color to grayscale

Binary Threshold:    Process ? ? Binary Threshold
                     Threshold value: 128

Gaussian Blur:       Process ? ??? Gaussian Blur
                     Toolbar ? ??? Blur
                     Kernel size: 15x15

Edge Detection:      Process ? ?? Edge Detection
                     Toolbar ? ?? Edges
                     Canny edge detection (100, 200)

Invert Colors:       Process ? ?? Invert Colors
                     Negative image effect
```

## ??? User Interface Elements

### Main Window Layout
```
???????????????????????????????????????????????????????
?  Menu Bar: File | Information | Transform |         ?
?            Histogram | Process                      ?
???????????????????????????????????????????????????????
?  Toolbar: [Load] [Save] [Reset] | [Gray] [Blur] [Edge] ?
???????????????????????????????????????????????????????
?                                                     ?
?  ????????????????????    ????????????????????      ?
?  ?  ?? Original     ?    ?  ? Processed    ?      ?
?  ?      Image       ?    ?      Image       ?      ?
?  ?                  ?    ?                  ?      ?
?  ?                  ?    ?                  ?      ?
?  ?                  ?    ?                  ?      ?
?  ????????????????????    ????????????????????      ?
?   Size info              Size info                 ?
???????????????????????????????????????????????????????
?  Status: Welcome message...        [Progress Bar]  ?
???????????????????????????????????????????????????????
```

### Canvas Features
- Auto-scaling to fit window
- Maintains aspect ratio
- Pink border for original image
- Lavender border for processed image
- Centered display

### Status Bar Colors
```
? Success:  Mint Green (#55efc4)
??  Info:     Lavender (#a29bfe)
??  Warning:  Peach (#ffeaa7)
? Error:    Pink (#ff6b9d)
```

## ?? Dialog Windows

### Translation Dialog
- **X Slider**: Horizontal movement
- **Y Slider**: Vertical movement
- **Spinboxes**: Precise value input
- **Live Preview**: Real-time updates

### Rotation Dialog
- **Angle Slider**: -180° to +180°
- **Angle Spinbox**: Precise angle input
- **Live Preview**: See rotation in real-time

### Zoom Dialog
- **Zoom Slider**: 50% to 300%
- **Zoom Spinbox**: 0.5x to 3.0x
- **Live Preview**: Preview scaled image

### Histogram Window
- **Visual Display**: Beautiful gradient charts
- **Statistics**: Frequency counts
- **Multi-channel**: Separate RGB display for color images

### Histogram Equalization Dialog
- **Side-by-side**: Original vs Equalized
- **Dual Histograms**: Compare distributions
- **Statistics**: Contrast improvement metrics

## ?? Workflow Examples

### Example 1: Basic Enhancement
```
1. Load Image
2. Process ? Grayscale (if needed)
3. Histogram ? Equalization
4. Save Image
```

### Example 2: Edge Analysis
```
1. Load Image
2. Process ? Gaussian Blur
3. Process ? Edge Detection  
4. Histogram ? Show Histogram
5. Save Image
```

### Example 3: Geometric Correction
```
1. Load Image
2. Transform ? Rotation (straighten)
3. Transform ? Zoom (crop to desired size)
4. Save Image
```

### Example 4: Image Analysis
```
1. Load Image
2. Information ? Image Info
3. Information ? Pixel Info (sample points)
4. Histogram ? Show Histogram
5. Information ? Statistics
```

## ?? Tips & Tricks

### Performance
- Large images may take a few seconds to process
- Histogram equalization is more intensive on color images
- Edge detection works best on grayscale images

### Best Practices
1. **Always check original first**: Review image info before processing
2. **Use Reset**: Experiment freely, reset anytime
3. **Save incrementally**: Save at each major step
4. **Histogram analysis**: Check histogram before/after enhancement
5. **Edge prep**: Blur before edge detection for cleaner results

### Common Operations
- **Quick Grayscale**: Toolbar button ?
- **Quick Blur**: Toolbar button ???
- **Quick Edges**: Toolbar button ??
- **Undo**: Use Reset button to restore original

## ?? Supported Formats

### Input Formats
- PNG (.png)
- JPEG (.jpg, .jpeg)
- BMP (.bmp)
- TIFF (.tiff, .tif)

### Output Formats  
- PNG (.png) - Lossless
- JPEG (.jpg) - Lossy compression
- BMP (.bmp) - Uncompressed

## ?? Technical Limits

- **Max Image Size**: Limited by available RAM
- **Channels**: 1 (Grayscale), 3 (RGB), 4 (RGBA)
- **Bit Depth**: 8-bit per channel
- **Color Space**: BGR (OpenCV default)

## ?? UI Customization

Buttons with gradient accent:
- Load Image button
- All Apply buttons in dialogs
- All OK/Close buttons

Regular buttons:
- Save, Reset
- Quick operation buttons
- Cancel buttons

## ?? Keyboard Shortcuts (via Menu)

While there are no hardcoded keyboard shortcuts, you can use Alt+Letter to access menus:
- `Alt+F`: File menu
- `Alt+I`: Information menu
- `Alt+T`: Transform menu
- `Alt+H`: Histogram menu
- `Alt+P`: Process menu

---

**For detailed documentation, see README.md**

**Happy Processing! ??**
