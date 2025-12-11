# Naghuma Toolbox - Modular Architecture

## New Project Structure

```
Naghuma Toolbox/
??? include/
?   ??? MainWindow.h              # Main application window (simplified)
?   ??? ImageCanvas.h             # Image display widget
?   ??? HistogramWidget.h         # Histogram visualization
?   ??? RightSidebarWidget.h      # NEW: Sidebar with tabs
?   ??? LayerManager.h            # NEW: Layers tracking
?   ??? ImageProcessor.h          # NEW: Image operations
?   ??? TransformDialog.h         # NEW: Reusable transform dialogs
??? src/
?   ??? main.cpp
?   ??? MainWindow.cpp            # Simplified main window
?   ??? ImageCanvas.cpp
?   ??? HistogramWidget.cpp
?   ??? RightSidebarWidget.cpp    # NEW: Sidebar implementation
?   ??? LayerManager.cpp          # NEW: Layer management
?   ??? ImageProcessor.cpp        # NEW: Processing functions
?   ??? TransformDialog.cpp       # NEW: Dialog implementations
??? lib/
    ??? filters/
    ?   ??? ImageFilters.h
    ?   ??? ImageFilters.cpp
    ??? transforms/
    ?   ??? ImageTransforms.h
    ?   ??? ImageTransforms.cpp
    ??? histogram/
        ??? HistogramOperations.h
        ??? HistogramOperations.cpp
```

## Modular Components

### 1. RightSidebarWidget
**Purpose**: Encapsulates the right sidebar with histogram and layers tabs.

**Responsibilities**:
- Display histogram of current/processed image
- Show list of processing layers
- Handle layer selection and removal
- Emit signals when layers are modified

**Key Methods**:
```cpp
void updateHistogram(const cv::Mat& image);
void addLayer(const QString& name, const QString& type, const cv::Mat& image);
void clearLayers();
```

### 2. LayerManager
**Purpose**: Manages the stack of processing layers.

**Responsibilities**:
- Store processing history
- Add/remove layers
- Track layer metadata (name, type, image)
- Emit signals on changes

**Key Methods**:
```cpp
void addLayer(const QString& name, const QString& type, const cv::Mat& image);
void removeLayer(int index);
const QVector<ProcessingLayer>& getLayers() const;
```

### 3. ImageProcessor
**Purpose**: Static utility class for all image processing operations.

**Responsibilities**:
- Centralize image processing algorithms
- Provide consistent interface
- Handle grayscale/color conversions
- Process transforms and filters

**Key Methods**:
```cpp
static void convertToGrayscale(const cv::Mat& src, cv::Mat& dst);
static void applyGaussianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize);
static void rotate(const cv::Mat& src, cv::Mat& dst, double angle);
static void translate(const cv::Mat& src, cv::Mat& dst, int tx, int ty);
```

### 4. TransformDialog (Base + Derived)
**Purpose**: Reusable dialog framework for transformations with live preview.

**Classes**:
- `TransformDialog` - Base class
- `TranslationDialog` - Translation UI
- `RotationDialog` - Rotation UI  
- `ZoomDialog` - Zoom UI

**Benefits**:
- DRY principle - no code duplication
- Consistent UX across all transforms
- Easy to add new transform dialogs

## MainWindow Simplification

The new `MainWindow` is much simpler:

**Before**: ~2000+ lines of mixed concerns
**After**: ~500 lines of coordination logic

### What MainWindow Now Does:
1. Creates UI layout
2. Connects menu/toolbar actions
3. Coordinates between components
4. Manages image state (original, current, processed)
5. Delegates processing to ImageProcessor
6. Updates RightSidebarWidget

### What MainWindow No Longer Does:
- ? Direct image processing (moved to ImageProcessor)
- ? Histogram management (moved to RightSidebarWidget)
- ? Layer tracking (moved to LayerManager)
- ? Transform dialog UI (moved to TransformDialog classes)

## Benefits of Modular Architecture

### 1. **Separation of Concerns**
Each class has a single, well-defined responsibility.

### 2. **Testability**
Components can be unit tested independently:
```cpp
// Test ImageProcessor without GUI
cv::Mat src, dst;
ImageProcessor::convertToGrayscale(src, dst);
ASSERT_EQ(dst.channels(), 1);
```

### 3. **Reusability**
Components can be used in other projects:
- `ImageProcessor` - standalone library
- `RightSidebarWidget` - reusable in other image apps
- `TransformDialog` - template for custom transforms

### 4. **Maintainability**
- Easy to find and fix bugs
- Clear file organization
- Reduced cognitive load

### 5. **Extensibility**
Adding new features is straightforward:

**Add new transform**:
1. Create new dialog class (inherits TransformDialog)
2. Implement transform in ImageProcessor
3. Add menu action in MainWindow

**Add new layer type**:
1. Extend LayerManager with new type
2. Update RightSidebarWidget icons
3. No changes to MainWindow needed

## How Components Interact

```
MainWindow
    ??> ImageProcessor (static calls for processing)
    ??> RightSidebarWidget
    ?       ??> HistogramWidget (display histogram)
    ?       ??> LayerManager (track layers)
    ??> TransformDialog (show dialogs)
            ??> ImageProcessor (apply transforms)
```

## Integration Steps

1. ? Create RightSidebarWidget class
2. ? Create LayerManager class
3. ? Create ImageProcessor class
4. ? Create TransformDialog classes
5. ? Update MainWindow to use new components
6. ? Update project files
7. ? Build and test

## Next Steps

To complete the integration:

1. Replace MainWindow.cpp with simplified version
2. Update .vcxproj to include new files
3. Run qmake/moc for new Qt classes
4. Build and test modular version
5. Remove old commented code
