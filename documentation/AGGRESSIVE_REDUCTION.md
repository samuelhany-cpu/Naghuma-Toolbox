# AGGRESSIVE MainWindow.cpp Size Reduction Plan
## Target: Reduce from 1800 lines to ~900 lines (50% reduction)

## Analysis of Current Bloat

### 1. Menu Creation Code (200 lines) ? Can reduce to 80 lines
**Problem**: Repetitive connect() calls
```cpp
// Current (8 lines per action)
QAction *loadAction = fileMenu->addAction("Load Image");
connect(loadAction, &QAction::triggered, this, &MainWindow::loadImage);

QAction *saveAction = fileMenu->addAction("Save Image");
connect(saveAction, &QAction::triggered, this, &MainWindow::saveImage);
```

**Solution**: Use helper macro/function (2 lines per action)
```cpp
#define ADD_ACTION(menu, text, slot) connect(menu->addAction(text), &QAction::triggered, this, &MainWindow::slot)

ADD_ACTION(fileMenu, "Load Image", loadImage);
ADD_ACTION(fileMenu, "Save Image", saveImage);
```

**Savings**: 120 lines

### 2. Toolbar Creation (100 lines) ? Can reduce to 40 lines
Same pattern - repetitive button creation

**Savings**: 60 lines

### 3. Dialog Functions (400 lines) ? Can reduce to 150 lines
Functions like `applyTranslation`, `applyRotation`, etc. have repetitive dialog handling

**Solution**: Extract common dialog pattern
```cpp
template<typename DialogType>
void applyDialogTransform(const QString& name, const QString& type) {
    if (!checkImageLoaded()) return;
    
    DialogType dialog(currentImage, this);
    connect(&dialog, &DialogType::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        finalizeProcessing(name, type);
        updateStatus(name + " applied successfully!", "success");
    } else {
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}
```

**Savings**: 250 lines

### 4. Simple Filter Functions (300 lines) ? Already reduced to 150 with helpers
**Current with helpers**: Still verbose lambda syntax

**Further optimization**: Create macro for common pattern
```cpp
#define SIMPLE_FILTER(funcName, processorFunc, layerName, layerType, message) \
void MainWindow::funcName() { \
    applySimpleFilter(processorFunc, \
        [](const cv::Mat& in) { cv::Mat out; processorFunc(in, out); return out; }, \
        layerName, layerType, message); \
}

// Usage
SIMPLE_FILTER(convertToGrayscale, ImageProcessor::convertToGrayscale, 
              "Grayscale", "adjustment", "Converted to grayscale!")
SIMPLE_FILTER(applyGaussianBlur, ImageProcessor::applyGaussianBlur,
              "Gaussian Blur", "filter", "Gaussian blur applied!")
```

**Savings**: Additional 80 lines

### 5. Morphology Operations (150 lines) ? Can reduce to 50 lines
All follow same pattern with parameter

**Solution**: Use template with default parameter
```cpp
template<auto Func>
void applyMorphOp(const QString& name, int kernelSize = 5) {
    applySimpleFilter(
        [kernelSize](const cv::Mat& s, cv::Mat& d) { Func(s, d, kernelSize); },
        [kernelSize](const cv::Mat& in) { cv::Mat out; Func(in, out, kernelSize); return out; },
        name, "morphology", name + " applied successfully!"
    );
}

// Usage
void MainWindow::applyErosion() { applyMorphOp<ImageProcessor::applyErosion>("Erosion"); }
void MainWindow::applyDilation() { applyMorphOp<ImageProcessor::applyDilation>("Dilation"); }
```

**Savings**: 100 lines

### 6. Large Dialog Functions (300 lines) ? Can reduce to 200 lines
Functions like `showImageMetrics`, `showFFTSpectrum`, `showHistogram`

**Solution**: Extract dialog creation pattern
```cpp
template<typename WidgetType>
QDialog* createStandardDialog(const QString& title, int minW, int minH, 
                               const QString& titleText) {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(title);
    dialog->setMinimumSize(minW, minH);
    
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    
    QLabel* titleLabel = new QLabel(titleText);
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    layout->addWidget(titleLabel);
    
    return dialog;
}
```

**Savings**: 100 lines

## Total Potential Savings: 710 lines
- Menu creation: 120 lines
- Toolbar creation: 60 lines
- Dialog transforms: 250 lines
- Simple filters: 80 lines
- Morphology ops: 100 lines
- Large dialogs: 100 lines

**Result**: 1800 - 710 = **1090 lines** (still need 190 more)

## Additional Aggressive Optimizations

### 7. Combine createMenuBar() and createToolBar() logic
Extract action creation into data-driven approach:

```cpp
struct MenuAction {
    QString menuName;
    QString actionName;
    void (MainWindow::*slot)();
    bool addSeparatorAfter = false;
};

const MenuAction menuActions[] = {
    {"File", "Load Image", &MainWindow::loadImage},
    {"File", "Save Image", &MainWindow::saveImage},
    {"File", "Reset", &MainWindow::resetImage, true},
    {"File", "Exit", &MainWindow::close},
    // ... etc
};

void MainWindow::createMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMap<QString, QMenu*> menus;
    
    for (const auto& action : menuActions) {
        if (!menus.contains(action.menuName))
            menus[action.menuName] = menuBar->addMenu(action.menuName);
        
        connect(menus[action.menuName]->addAction(action.actionName), 
                &QAction::triggered, this, action.slot);
        
        if (action.addSeparatorAfter)
            menus[action.menuName]->addSeparator();
    }
}
```

**Savings**: Additional 150 lines

### 8. Simplify Theme Stylesheet
Move to external .qss file or compress

**Savings**: 30 lines

### 9. Combine updateDisplay() and updateMetricsDisplay()
They're always called together

**Savings**: 20 lines

## FINAL TARGET: ~900 lines

**Total reduction**: 1800 ? 900 (50% reduction)
