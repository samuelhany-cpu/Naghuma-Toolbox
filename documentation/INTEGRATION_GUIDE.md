# Integration Guide - Modular Naghuma Toolbox

## Files Created

### Header Files (include/)
1. ? **RightSidebarWidget.h** - Sidebar widget with histogram and layers tabs
2. ? **LayerManager.h** - Processing layers management
3. ? **ImageProcessor.h** - Static image processing utilities
4. ? **TransformDialog.h** - Base transform dialog + Translation/Rotation/Zoom

### Implementation Files (src/)
1. ? **RightSidebarWidget.cpp** - Sidebar implementation
2. ? **LayerManager.cpp** - Layer management logic
3. ? **ImageProcessor.cpp** - All image processing functions
4. ? **TransformDialog.cpp** - Transform dialog implementations

### Updated Files
1. ? **MainWindow.h** - Simplified header with new dependencies

### Documentation
1. ? **docs/MODULAR_ARCHITECTURE.md** - Architecture overview

## What Still Needs To Be Done

### 1. Update Visual Studio Project File

Add these to `Naghuma Toolbox.vcxproj`:

```xml
<!-- In ClInclude ItemGroup -->
<ClInclude Include="include\RightSidebarWidget.h" />
<ClInclude Include="include\LayerManager.h" />
<ClInclude Include="include\ImageProcessor.h" />
<ClInclude Include="include\TransformDialog.h" />

<!-- In ClCompile ItemGroup -->
<ClCompile Include="src\RightSidebarWidget.cpp" />
<ClCompile Include="src\LayerManager.cpp" />
<ClCompile Include="src\ImageProcessor.cpp" />
<ClCompile Include="src\TransformDialog.cpp" />
```

### 2. Generate MOC Files

Run Qt's MOC (Meta-Object Compiler) for new Q_OBJECT classes:

```powershell
# From project root
moc include\RightSidebarWidget.h -o src\moc_RightSidebarWidget.cpp
moc include\LayerManager.h -o src\moc_LayerManager.cpp
moc include\TransformDialog.h -o src\moc_TransformDialog.cpp
```

Or add to your .vcxproj for automatic MOC generation.

### 3. Replace MainWindow.cpp

Your current `src\MainWindow.cpp` is incomplete (only 681 lines, missing implementations).

You need a NEW `MainWindow.cpp` that:
- Uses `RightSidebarWidget` instead of creating sidebar manually
- Uses `ImageProcessor` static methods for all processing
- Uses `TransformDialog` classes for transforms
- Delegates layer management to `RightSidebarWidget`

**Key changes in new MainWindow.cpp**:

#### Include new headers:
```cpp
#include "RightSidebarWidget.h"
#include "ImageProcessor.h"
#include "TransformDialog.h"
```

#### In setupUI():
```cpp
void MainWindow::setupUI() {
    createMenuBar();
    createToolBar();
    createCentralWidget();
    
    // Create and add right sidebar
    rightSidebar = new RightSidebarWidget(this);
    connect(rightSidebar, &RightSidebarWidget::layerRemoveRequested,
            this, &MainWindow::onLayerRemoveRequested);
    
    QWidget *central = centralWidget();
    QHBoxLayout *mainLayout = qobject_cast<QHBoxLayout*>(central->layout());
    if (mainLayout) {
        mainLayout->addWidget(rightSidebar);
    }
    
    createStatusBar();
    
    // Center window
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}
```

#### Processing functions now use ImageProcessor:
```cpp
void MainWindow::convertToGrayscale() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::convertToGrayscale(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Grayscale", "adjustment");
    updateStatus("Converted to grayscale!", "success");
}
```

#### Transform dialogs now use TransformDialog classes:
```cpp
void MainWindow::applyTranslation() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    TranslationDialog dialog(currentImage, this);
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        finalizeProcessing(QString("Translation (%1, %2)")
            .arg(dialog.getTranslationX())
            .arg(dialog.getTranslationY()), "transform");
        updateStatus("Translation applied successfully!", "success");
    }
}
```

#### finalizeProcessing now updates sidebar:
```cpp
void MainWindow::finalizeProcessing(const QString& layerName, const QString& layerType) {
    if (!processedImage.empty()) {
        // Update current image
        currentImage = processedImage.clone();
        
        // Add layer to sidebar
        rightSidebar->addLayer(layerName, layerType, processedImage);
        
        // Update histogram
        rightSidebar->updateHistogram(processedImage);
    }
}
```

#### Layer removal:
```cpp
void MainWindow::onLayerRemoveRequested(int layerIndex) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Remove Layer",
        "Remove this processing layer?\n\nThis will rebuild the image from remaining layers.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // For now, just reset - full layer reapplication requires storing parameters
        resetImage();
        updateStatus(QString("Layer removed. Reset to original."), "warning");
    }
}
```

### 4. Build Steps

```powershell
# 1. Clean old build
Remove-Item -Recurse -Force x64\Debug\*, x64\Release\*

# 2. Regenerate MOC files
.\scripts\generate_moc.ps1

# 3. Build project
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild

# 4. Run
.\x64\Debug\Naghuma` Toolbox.exe
```

## Testing Checklist

After integration, test:

- [ ] Load image
- [ ] Apply grayscale ? Check histogram updates
- [ ] Apply blur ? Check layer appears in list
- [ ] Apply rotation with dialog ? Check live preview works
- [ ] Apply translation ? Check real-time preview
- [ ] Click layer in list ? Check "Remove" button enables
- [ ] Try to remove layer
- [ ] Reset image ? Check layers clear
- [ ] Save processed image

## Troubleshooting

### "Unresolved external symbol" errors
- Ensure all new .cpp files are in .vcxproj
- Regenerate MOC files
- Clean and rebuild

### "undefined reference to vtable"
- Missing MOC generation for Q_OBJECT classes
- Run moc on all header files with Q_OBJECT

### Linker errors about ImageProcessor
- ImageProcessor is static class, check all methods are static
- Ensure ImageProcessor.cpp is compiled

### Dialog doesn't show preview
- TransformDialog base class needs ImageCanvas integration
- Check `showPreviewInCanvas()` implementation

## Next Steps

1. Back up your current MainWindow.cpp
2. Create new MainWindow.cpp using the patterns above
3. Update .vcxproj file
4. Generate MOC files
5. Build and test
6. Refine as needed

## Benefits You'll See

? **Cleaner Code**: MainWindow reduced from 2000+ to ~500 lines
? **Better Organization**: Each component in its own file
? **Easier Maintenance**: Fix bugs in isolated components
? **Reusability**: ImageProcessor can be library
? **Professional Structure**: Industry-standard architecture

