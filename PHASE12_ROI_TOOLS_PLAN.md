# Phase 12: ROI Tools Foundation - Implementation Plan

## Overview
Implementing comprehensive Region of Interest (ROI) tools for advanced image analysis, building on the zoom/pan functionality from Phase 11.

## Goals
- ? Rectangle ROI selection tool
- ? ROI statistics calculation (mean, std, min, max, area)
- ? ROI histogram display
- ? ROI manager (save, load, multiple ROIs)
- ? Visual ROI overlay with proper icons
- ? Integration with zoom/pan system

## Features

### 1. ROI Selection Tools

#### Rectangle ROI
- Click and drag to select
- Adjustable size and position
- Visual feedback with border
- Real-time size display

#### Future ROI Types (Phase 12+)
- Ellipse ROI
- Polygon ROI
- Free-hand ROI
- Magic wand (intensity-based)

### 2. ROI Statistics Panel

**Calculated Metrics**:
- **Area**: Number of pixels in ROI
- **Mean**: Average intensity value
- **Std Dev**: Standard deviation
- **Min**: Minimum pixel value
- **Max**: Maximum pixel value
- **Sum**: Total intensity
- **Median**: Median value
- **Mode**: Most frequent value

**Per-Channel Stats** (for color images):
- Red channel statistics
- Green channel statistics
- Blue channel statistics
- Combined statistics

### 3. ROI Manager

**Features**:
- Multiple ROI support
- ROI list display
- Show/hide individual ROIs
- Delete ROI
- Rename ROI
- Save ROI set to file
- Load ROI set from file
- Export statistics to CSV

### 4. ROI Visualization

**Display Options**:
- ROI boundary color (customizable)
- ROI fill opacity
- ROI label display
- Measurement overlays
- Grid overlay on ROI

### 5. Icon System

**Using Qt Standard Icons** (built-in):
```cpp
QStyle::SP_FileDialogNewFolder    // New ROI
QStyle::SP_DialogSaveButton       // Save ROI
QStyle::SP_DialogOpenButton       // Load ROI
QStyle::SP_TrashIcon              // Delete ROI
QStyle::SP_FileIcon               // ROI file
QStyle::SP_ComputerIcon           // Statistics
```

**Future: Font Awesome Integration**:
- Load TTF font file
- Use Unicode characters
- Professional icon set

## Implementation Structure

### Files to Create

#### Headers
```
include/
??? ROITool.h           # Main ROI selection tool
??? ROIManager.h        # ROI management (multiple ROIs)
??? ROIDialog.h         # ROI statistics dialog
??? ROIShape.h          # ROI shape base class
```

#### Implementation
```
src/
??? ROITool.cpp         # ROI tool implementation
??? ROIManager.cpp      # ROI manager implementation
??? ROIDialog.cpp       # Statistics dialog
??? ROIShape.cpp        # Shape classes
??? moc_*.cpp           # Qt MOC files (auto-generated)
```

### Class Hierarchy

```cpp
// Base shape class
class ROIShape {
    QString name;
    QColor color;
    bool visible;
    virtual QRect getBounds() = 0;
    virtual bool contains(QPoint) = 0;
    virtual void draw(QPainter&) = 0;
};

// Rectangle ROI
class RectangleROI : public ROIShape {
    QRect rect;
    // Implementation
};

// ROI Manager
class ROIManager {
    QList<ROIShape*> rois;
    void addROI(ROIShape* roi);
    void removeROI(int index);
    void saveToFile(QString path);
    void loadFromFile(QString path);
};
```

## UI Integration

### Menu Structure
```
Analysis Menu (NEW)
??? ROI Tools
?   ??? Rectangle ROI     [Icon]
?   ??? ROI Manager...    [Icon]
?   ??? ROI Statistics... [Icon]
??? ?????????????
??? Measurements
?   ??? Measure Distance
?   ??? Measure Angle
?   ??? Measure Area
??? ?????????????
```

### Toolbar Additions
```
[ROI] button - Toggle ROI mode
[Stats] button - Show ROI statistics
```

### Status Bar
```
ROI: 250x180 at (120, 85) | Mean: 128.5 | Std: 45.2
```

## Workflow Examples

### Example 1: Basic ROI Analysis
1. Load image
2. Click "ROI" button in toolbar
3. Click and drag on image to select region
4. View statistics panel
5. See mean, std, min, max automatically

### Example 2: Multiple ROI Comparison
1. Select first ROI (background)
2. Click "Add to Manager"
3. Select second ROI (foreground)
4. Click "Add to Manager"
5. Open ROI Manager dialog
6. Compare statistics side-by-side
7. Export to CSV

### Example 3: ROI with Zoom
1. Load large image
2. Zoom to 200%
3. Select precise ROI
4. Pan to different area
5. Select another ROI
6. Compare regions at high magnification

## Technical Details

### ROI Statistics Calculation

```cpp
struct ROIStats {
    double mean;
    double stdDev;
    double min;
    double max;
    double median;
    int area;
    long sum;
    
    // Per-channel for color images
    ROIStats red;
    ROIStats green;
    ROIStats blue;
};

ROIStats calculateStats(cv::Mat image, QRect roi) {
    // Extract ROI from image
    cv::Rect cvRoi(roi.x(), roi.y(), roi.width(), roi.height());
    cv::Mat roiMat = image(cvRoi);
    
    // Calculate statistics
    cv::Scalar mean, stddev;
    cv::meanStdDev(roiMat, mean, stddev);
    
    double minVal, maxVal;
    cv::minMaxLoc(roiMat, &minVal, &maxVal);
    
    // Return stats
    // ...
}
```

### ROI File Format (JSON)

```json
{
    "version": "1.0",
    "rois": [
        {
            "name": "Background",
            "type": "rectangle",
            "x": 100,
            "y": 50,
            "width": 200,
            "height": 150,
            "color": "#FF0000",
            "visible": true
        },
        {
            "name": "Signal Region",
            "type": "rectangle",
            "x": 350,
            "y": 200,
            "width": 180,
            "height": 120,
            "color": "#00FF00",
            "visible": true
        }
    ]
}
```

## Testing Checklist

- [ ] Rectangle ROI selection works
- [ ] ROI displays correctly on image
- [ ] Statistics calculated accurately
- [ ] ROI Manager shows all ROIs
- [ ] Multiple ROIs can coexist
- [ ] ROI save/load functionality
- [ ] ROI works with zoom/pan
- [ ] ROI statistics dialog displays
- [ ] Per-channel stats for color images
- [ ] CSV export works
- [ ] ROI delete works
- [ ] ROI visibility toggle works
- [ ] Icons display correctly
- [ ] No memory leaks
- [ ] Smooth user experience

## Performance Considerations

- **Lazy Statistics**: Calculate only when requested
- **Cached Results**: Cache stats until ROI changes
- **Efficient Drawing**: Use QPainter optimization
- **Large Images**: Handle efficiently with tiling

## Expected Results

### Rating Impact
- **Before**: 7.7/10
- **After**: 7.9/10 (+0.2)
- **Coverage**: +3% (basic ROI tools)

### User Benefits
- Precise region analysis
- Quantitative measurements
- Multiple region comparison
- Professional analysis workflow
- Integration with other tools

## Future Enhancements (Phase 13+)

- Elliptical ROI selection
- Polygonal ROI selection
- Free-hand ROI drawing
- Magnetic lasso (edge-snapping)
- ROI arithmetic (union, intersection, difference)
- ROI intensity profiles
- 3D surface plots of ROI
- ROI-based filtering
- ROI tracking in video
- Machine learning on ROI

## Documentation

### User Guide Section
```markdown
# ROI Tools

## Creating an ROI
1. Click the ROI button in toolbar
2. Click and drag on image to select region
3. Release mouse to finalize

## Viewing Statistics
- Statistics automatically displayed in panel
- Click "ROI Statistics" for detailed view

## Managing Multiple ROIs
- Use ROI Manager to handle multiple regions
- Each ROI has unique color and name
- Show/hide ROIs individually
```

---

**Status**: Ready for Implementation  
**Estimated Time**: 4-6 hours  
**Priority**: High  
**Dependencies**: Phase 11 (Zoom/Pan) - ? Complete

**Next Phase**: Phase 13 - Basic Edge Detectors
