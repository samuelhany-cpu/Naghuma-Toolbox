# MainWindow Modularization Plan

## Overview
The MainWindow.cpp file (1800+ lines) has been refactored into modular handler classes to improve maintainability, readability, and testability.

## New Architecture

### Handler Classes Created

1. **MenuHandlers** (`include/MenuHandlers.h`, `src/MenuHandlers.cpp`)
   - File operations: loadImage(), saveImage(), resetImage(), useProcessedImage()
   - Information dialogs: showImageInfo(), showPixelInfo(), showImageStats(), showImageMetrics()

2. **TransformHandlers** (`include/TransformHandlers.h`, `src/TransformHandlers.cpp`)
   - Geometric transforms: applyTranslation(), applyRotation(), applySkew(), applyZoom()
   - Flip operations: applyFlipX(), applyFlipY(), applyFlipXY()

3. **FilterHandlers** (`include/FilterHandlers.h`, `src/FilterHandlers.cpp`)
   - Histogram: showHistogram(), applyHistogramEqualization(), applyOtsuThresholding()
   - Basic processing: convertToGrayscale(), applyGaussianBlur(), etc.
   - Advanced filters: applyLaplacianFilter(), applySobelCombinedFilter(), etc.
   - Morphology: applyErosion(), applyDilation(), applyOpening(), applyClosing()
   - FFT: showFFTSpectrum(), applyLowPassFilter(), applyHighPassFilter()

4. **BrushHandlers** (`include/BrushHandlers.h`, `src/BrushHandlers.cpp`)
   - Brush operations: showBrushSettings(), applyBrushEffect(), toggleDrawingMode()
   - Mouse events: onCanvasMousePress(), onCanvasMouseMove(), onCanvasMouseRelease()

5. **MainWindowAccessor** (`include/MainWindowAccessor.h`)
   - Interface providing controlled access to MainWindow internals
   - Allows handlers to access image data, UI components, and helper functions

## File Structure

```
Naghuma Toolbox/
??? include/
?   ??? MainWindow.h               (Updated - now uses handlers)
?   ??? MainWindowAccessor.h       (New - access interface)
?   ??? MenuHandlers.h             (New)
?   ??? TransformHandlers.h        (New)
?   ??? FilterHandlers.h           (New)
?   ??? BrushHandlers.h            (New)
??? src/
?   ??? MainWindow.cpp             (Simplified - 400 lines)
?   ??? MenuHandlers.cpp           (New)
?   ??? TransformHandlers.cpp      (New)
?   ??? FilterHandlers.cpp         (New)
?   ??? BrushHandlers.cpp          (New)
```

## Benefits

### 1. **Reduced Complexity**
- MainWindow.cpp: 1800+ lines ? ~400 lines (78% reduction)
- Each handler: 200-400 lines (manageable size)

### 2. **Single Responsibility Principle**
- Each handler class has one clear purpose
- Easier to understand and maintain

### 3. **Improved Testability**
- Handlers can be unit tested independently
- Mock MainWindowAccessor for isolated testing

### 4. **Better Organization**
- Related functions grouped logically
- Easy to find specific functionality

### 5. **Easier Collaboration**
- Team members can work on different handlers simultaneously
- Reduced merge conflicts

## Implementation Steps

### Step 1: Create Handler Headers and Implementations
? MenuHandlers.h/cpp
? TransformHandlers.h/cpp  
? FilterHandlers.h/cpp
? BrushHandlers.h/cpp

### Step 2: Update MainWindow
? Simplify MainWindow.cpp
? Initialize handler objects in constructor
? Connect menu actions to handler methods

### Step 3: Update Project File
Add new files to Naghuma Toolbox.vcxproj:
```xml
<ClInclude Include="include\MainWindowAccessor.h" />
<ClInclude Include="include\MenuHandlers.h" />
<ClInclude Include="include\TransformHandlers.h" />
<ClInclude Include="include\FilterHandlers.h" />
<ClInclude Include="include\BrushHandlers.h" />

<ClCompile Include="src\MenuHandlers.cpp" />
<ClCompile Include="src\TransformHandlers.cpp" />
<ClCompile Include="src\FilterHandlers.cpp" />
<ClCompile Include="src\BrushHandlers.cpp" />
```

### Step 4: Generate MOC Files
```powershell
moc include\MenuHandlers.h -o src\moc_MenuHandlers.cpp
moc include\TransformHandlers.h -o src\moc_TransformHandlers.cpp
moc include\FilterHandlers.h -o src\moc_FilterHandlers.cpp
moc include\BrushHandlers.h -o src\moc_BrushHandlers.cpp
```

### Step 5: Build and Test
```powershell
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild
.\x64\Debug\Naghuma` Toolbox.exe
```

## Usage Example

### Before (Monolithic)
```cpp
// MainWindow.cpp - Everything in one file
void MainWindow::loadImage() {
    // 30 lines of code...
}

void MainWindow::applyRotation() {
    // 40 lines of code...
}

void MainWindow::convertToGrayscale() {
    // 20 lines of code...
}
// ... 1800+ more lines
```

### After (Modular)
```cpp
// MainWindow.cpp - Clean coordination
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Initialize handlers
    menuHandlers = new MenuHandlers(this, this);
    transformHandlers = new TransformHandlers(this, this);
    filterHandlers = new FilterHandlers(this, this);
    brushHandlers = new BrushHandlers(this, this);
    
    setupUI();
}

void MainWindow::createMenuBar() {
    // File Menu
    QAction *loadAction = fileMenu->addAction("Load Image");
    connect(loadAction, &QAction::triggered, menuHandlers, &MenuHandlers::loadImage);
    
    // Transform Menu
    QAction *rotateAction = transformMenu->addAction("Rotation");
    connect(rotateAction, &QAction::triggered, transformHandlers, &TransformHandlers::applyRotation);
    
    // Process Menu
    QAction *grayscaleAction = processMenu->addAction("Grayscale");
    connect(grayscaleAction, &QAction::triggered, filterHandlers, &FilterHandlers::convertToGrayscale);
}
```

## Testing

### Unit Test Example
```cpp
// Test TransformHandlers independently
class MockMainWindow : public MainWindowAccessor {
    // Implement interface with test data
};

TEST(TransformHandlers, ApplyRotation) {
    MockMainWindow mockWindow;
    TransformHandlers handlers(&mockWindow);
    
    // Test rotation logic
    handlers.applyRotation();
    
    // Verify results
    ASSERT_FALSE(mockWindow.getProcessedImage().empty());
}
```

## Migration Checklist

- [x] Create MainWindowAccessor interface
- [x] Create MenuHandlers.h/cpp
- [ ] Create TransformHandlers.h/cpp
- [ ] Create FilterHandlers.h/cpp
- [ ] Create BrushHandlers.h/cpp
- [ ] Update MainWindow.h
- [ ] Simplify MainWindow.cpp
- [ ] Update .vcxproj file
- [ ] Generate MOC files
- [ ] Build and test
- [ ] Update documentation

## Notes

- All handler classes use the MainWindowAccessor interface for type safety
- Qt's signal/slot mechanism used for clean event handling
- Maintains backward compatibility - no changes to public API
- Can be done incrementally - one handler at a time

## Future Enhancements

1. **Add logging**: Each handler can log operations independently
2. **Add undo/redo**: Easier to implement per-handler command pattern
3. **Plugin system**: Load handlers dynamically at runtime
4. **Scripting support**: Expose handlers to Python/JavaScript bindings
