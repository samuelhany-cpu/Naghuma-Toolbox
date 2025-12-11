# Naghuma Toolbox - Complete Development Roadmap

## ? Phase 1: Image Quality Metrics (COMPLETE)
**Status: DONE**

### Implemented:
- ? RMSE (Root Mean Square Error) calculation
- ? SNR (Signal-to-Noise Ratio) calculation
- ? PSNR (Peak Signal-to-Noise Ratio) calculation
- ? MSE (Mean Square Error) calculation  
- ? Image Metrics dialog in Information menu
- ? Quality interpretation guide

### Files Created:
- `include/ImageMetrics.h`
- `src/ImageMetrics.cpp`
- Updated `MainWindow.h` and `MainWindow.cpp`

### Testing:
Load an image ? Apply any filter ? Information menu ? Image Metrics

---

## Phase 2: Layer Management Fix (READY TO START)
**Priority: HIGH - Bug Fix**

### Issue:
When removing a middle layer, only that specific operation should be removed, not all subsequent operations.

### Implementation:
```cpp
// Store operations with parameters
struct LayerOperation {
    QString name;
    QString type;
    std::function<cv::Mat(const cv::Mat&)> operation;
};

// On layer removal:
1. Remove layer at index from list
2. Start with original image
3. Reapply all remaining operations in order
4. Update displays
```

### Files to Modify:
- `src/LayerManager.cpp` - Add operation replay
- `src/MainWindow.cpp` - Store operation functions
- `include/LayerManager.h` - Add operation storage

---

## Phase 3: Zoom & Skew Improvements
**Priority: HIGH - Fix Broken Features**

### 3A: Fix Zoom Function
Current issue: Zoom function not working

**Fix Steps:**
1. Check `ZoomDialog` implementation
2. Verify transform is being applied
3. Test with different scale values

### 3B: Enhance Skew with Live Preview
**Add:**
- Horizontal skew slider (-45° to +45°)
- Vertical skew slider (-45° to +45°)
- Real-time preview canvas
- Apply/Cancel buttons

**Files:**
- Create `SkewDialog` class (similar to `RotationDialog`)
- Modify `MainWindow::applySkew()` to use dialog

---

## Phase 4: Brightness & Contrast Controls
**Priority: MEDIUM**

### Features:
- Brightness slider: -100 to +100
- Contrast slider: -100 to +100  
- Live preview showing changes
- Reset to default button
- Before/after comparison

### Implementation:
```cpp
class BrightnessContrastDialog : public QDialog {
    // Sliders
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    
    // Preview canvas
    ImageCanvas *previewCanvas;
    
    // Apply formula:
    // output = contrast * input + brightness
};
```

### Menu Location:
Process ? Brightness/Contrast

---

## Phase 5: Drawing Tools & Brushes
**Priority: MEDIUM**

### 5A: Brush Tool
**Types:**
- Circle brush
- Square brush
- Soft edge brush
- Custom shapes

**Controls:**
- Size slider: 1-100 px
- Opacity slider: 0-100%
- Color picker
- Eraser mode

### 5B: FFT Operations
- Fast Fourier Transform filter
- Frequency domain editing
- High-pass/low-pass filters

### 5C: Morphology Operations
- Erosion
- Dilation
- Opening
- Closing
- Morphological gradient

### Files to Create:
- `include/BrushTool.h`
- `src/BrushTool.cpp`
- `include/FFTOperations.h`
- `src/FFTOperations.cpp`
- `include/MorphologyOps.h`
- `src/MorphologyOps.cpp`

---

## Phase 6: Cropping Tool
**Priority: MEDIUM**

### Features:
- Interactive rectangle selection on canvas
- Drag corners to resize
- Move selection
- Show crop dimensions
- Maintain aspect ratio option
- Apply crop

### Implementation:
```cpp
class CropTool : public QObject {
    QRect selection;
    bool isDragging;
    
    void mousePressEvent();
    void mouseMoveEvent();
    void mouseReleaseEvent();
    void paintSelection();
};
```

### UI:
- Add "Crop" button to toolbar
- Click and drag on image to select area
- Show crop rectangle with handles

---

## Phase 7: Compression Algorithms
**Priority: LOW**

### Algorithms:
1. **JPEG Compression**
   - Quality slider: 0-100
   - Show file size estimate
   - Calculate compression ratio

2. **PNG Compression**
   - Compression level: 0-9
   - Show file size

### Metrics Display:
- Original size vs Compressed size
- Compression ratio
- RMSE between original and compressed
- PSNR value
- Quality assessment

### Files:
- `include/CompressionDialog.h`
- `src/CompressionDialog.cpp`

---

## Phase 8: Auto Enhancement
**Priority: MEDIUM**

### Algorithm 1: Adaptive Histogram Equalization
- CLAHE (Contrast Limited AHE)
- Tile size parameter
- Clip limit parameter

### Algorithm 2: Automatic Contrast Stretching
- Find min/max values
- Stretch to full 0-255 range
- Preserve color ratios

### Comparison View:
```
[Original] [Algorithm 1] [Algorithm 2]
[Metrics for each]
```

### Menu:
Process ? Auto Enhance ? Show comparison dialog

---

## Phase 9: Noise Removal
**Priority: MEDIUM**

### Filters:
1. **Gaussian Noise Removal**
   - Kernel size selection
   - Sigma parameter

2. **Median Filter**
   - Kernel size: 3x3, 5x5, 7x7
   - Good for salt-and-pepper noise

3. **Bilateral Filter**  
   - Preserves edges
   - Sigma color/space parameters

### Show Improvement:
- SNR before/after
- Side-by-side comparison

---

## Phase 10: UI/UX Improvements
**Priority: HIGH - User Experience**

### 10A: Collapsible Left Toolbar
```
[?] Tools
?? [???] File Operations
?   ?? Load
?   ?? Save
?   ?? Reset
?? [??] Brushes
?? [??] Crop  
?? [?] Filters
?? [??] Adjustments
```

**Behavior:**
- Collapsed by default (icon-only)
- Hover to expand with labels
- Click to pin open
- Smooth slide animation

### 10B: Smooth Transitions
- Fade in/out for dialogs: 200ms
- Slide animations for panels: 300ms
- Button hover effects: 150ms
- Tab switching: 250ms

### 10C: Button Improvements
```css
QPushButton {
    transition: all 0.15s ease-in-out;
}

QPushButton:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 12px rgba(232, 121, 249, 0.4);
}
```

### 10D: Icon System
Create SVG icons for all tools:
- Use Qt Resource System
- 24x24px and 48x48px versions
- Match purple/fuchsia theme

---

## Implementation Schedule

### Week 1:
- ? Phase 1: Metrics (DONE)
- Phase 2: Layer fix
- Phase 3A: Fix zoom

### Week 2:
- Phase 3B: Skew dialog
- Phase 4: Brightness/Contrast
- Phase 10A: Collapsible toolbar

### Week 3:
- Phase 6: Crop tool
- Phase 9: Noise removal
- Phase 10B-D: UI polish

### Week 4:
- Phase 5: Brushes & morphology
- Phase 8: Auto enhance
- Phase 7: Compression

---

## Build & Test Commands

### After Each Phase:
```powershell
# Regenerate MOC if headers changed
C:\Qt\6.7.3\msvc2019_64\bin\moc.exe include\MainWindow.h -o src\moc_MainWindow.cpp

# Build
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build

# Run
.\scripts\run_app.ps1
```

### Testing Checklist Template:
```
Phase X Testing:
? Feature loads without errors
? UI elements render correctly
? Functionality works as expected
? No crashes or memory leaks
? Performance is acceptable
? Keyboard shortcuts work
? Theme colors consistent
```

---

## Next Action:
**Start Phase 2 - Fix Layer Removal**

The layer management system needs to store and replay operations instead of just clearing everything. This is a critical bug that affects usability.

Would you like me to start implementing Phase 2?
