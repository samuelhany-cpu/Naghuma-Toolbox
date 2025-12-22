# 🎨 Custom Clone Prompt: Naghuma Toolbox with New Color Palette & Enhanced UX

**Base Project:** Naghuma Toolbox v2.0.0  
**Objective:** Create a customized clone with modern color scheme and improved user experience  
**Technologies:** Qt 6.7.3, OpenCV 4.3.0, C++17, MSVC 2022  
**Date:** December 22, 2025

---

## 🎯 Project Objectives

### Core Requirements
1. **Maintain Technical Foundation:**
   - Keep all existing image processing algorithms
   - Preserve Qt 6.7.3 + OpenCV 4.3.0 stack
   - Maintain 5-method Selection Tool functionality
   - Keep layer system architecture

2. **Transform Visual Identity:**
   - Implement modern color palette (dark theme with accent colors)
   - Update all UI components with new color scheme
   - Redesign icons and visual elements
   - Apply consistent styling across all dialogs

3. **Enhance User Experience:**
   - Modernize toolbar and menu layouts
   - Add tooltips and contextual help
   - Improve visual feedback and animations
   - Create more intuitive workflows
   - Add keyboard shortcut customization
   - Implement collapsible sidebar panels

---

## 🎨 New Color Palette Specification

### Primary Colors
```css
/* Main Application Background */
--bg-primary: #1E1E2E          /* Deep dark blue-gray */
--bg-secondary: #2A2A3E        /* Lighter panel background */
--bg-tertiary: #363651         /* Hover/active states */

/* Accent Colors */
--accent-primary: #89B4FA      /* Soft blue - primary actions */
--accent-secondary: #F5C2E7    /* Soft pink - secondary actions */
--accent-success: #A6E3A1      /* Mint green - success states */
--accent-warning: #F9E2AF      /* Warm yellow - warnings */
--accent-danger: #F38BA8       /* Coral red - destructive actions */
--accent-info: #94E2D5         /* Teal - informational */

/* Text Colors */
--text-primary: #CDD6F4        /* Main text - high contrast */
--text-secondary: #BAC2DE      /* Secondary text */
--text-tertiary: #A6ADC8       /* Disabled/muted text */
--text-on-accent: #1E1E2E      /* Text on colored backgrounds */

/* Border & Divider Colors */
--border-primary: #45475A      /* Main borders */
--border-secondary: #313244    /* Subtle dividers */
--border-focus: #89B4FA        /* Focused elements */

/* Selection & Overlay Colors */
--selection-fill: rgba(137, 180, 250, 0.3)    /* 30% blue for selection overlay */
--selection-border: #89B4FA                    /* Blue border for selection */
--selection-active: rgba(245, 194, 231, 0.4)  /* 40% pink for active selection */

/* Sidebar & Panel Colors */
--sidebar-bg: #181825          /* Darker sidebar background */
--panel-header: #2A2A3E        /* Panel header background */
--panel-border: #45475A        /* Panel separators */

/* Button Colors */
--btn-primary-bg: #89B4FA
--btn-primary-hover: #A5C4FF
--btn-primary-active: #6E99D9
--btn-secondary-bg: #363651
--btn-secondary-hover: #45475A
--btn-danger-bg: #F38BA8
--btn-danger-hover: #F5A0B5
```

### Color Application Map
| Component | Current Color | New Color | Variable |
|-----------|---------------|-----------|----------|
| Main Window Background | #F0F0F0 (light gray) | #1E1E2E | --bg-primary |
| Toolbar Background | #FFFFFF (white) | #2A2A3E | --bg-secondary |
| Canvas Background | #E0E0E0 (gray) | #181825 | --sidebar-bg |
| Selection Overlay | rgba(0, 255, 0, 0.6) (green) | rgba(137, 180, 250, 0.3) (blue) | --selection-fill |
| Selection Border | #FF0000 (red) | #89B4FA (blue) | --selection-border |
| Primary Button | QPushButton default | #89B4FA | --btn-primary-bg |
| Menu Text | #000000 (black) | #CDD6F4 | --text-primary |
| Disabled Text | #808080 (gray) | #A6ADC8 | --text-tertiary |

---

## 🔧 Technology Stack & Component Paths

### Development Environment
```yaml
Operating System: Windows 10/11 x64
Build Tools: Visual Studio Build Tools 2022
  Path: F:\Build Tools\
  MSBuild: F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe
  MSVC Compiler: F:\Build Tools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe
  Version: 17.x.x (MSVC 14.44.35207)
Language Standard: C++17
```

### Qt Framework
```yaml
Version: 6.7.3
Path: C:\Qt\6.7.3\msvc2019_64\
Binaries: C:\Qt\6.7.3\msvc2019_64\bin\
  - qmake.exe
  - moc.exe (Meta-Object Compiler)
  - uic.exe (UI Compiler)
  - rcc.exe (Resource Compiler)
Libraries: C:\Qt\6.7.3\msvc2019_64\lib\
  - Qt6Core.lib / Qt6Cored.lib
  - Qt6Gui.lib / Qt6Guid.lib
  - Qt6Widgets.lib / Qt6Widgetsd.lib
  - Qt6Svg.lib / Qt6Svgd.lib
Include: C:\Qt\6.7.3\msvc2019_64\include\
  - QtCore\
  - QtGui\
  - QtWidgets\
  - QtSvg\
Plugins: C:\Qt\6.7.3\msvc2019_64\plugins\
  - platforms\qwindowsd.dll (Debug)
  - platforms\qwindows.dll (Release)
```

### OpenCV
```yaml
Version: 4.3.0
Path: F:\OpenCV\opencv\
Build: F:\OpenCV\opencv\build\
Binaries: F:\OpenCV\opencv\build\x64\vc15\bin\
  - opencv_world430d.dll (Debug)
  - opencv_world430.dll (Release)
Libraries: F:\OpenCV\opencv\build\x64\vc15\lib\
  - opencv_world430d.lib (Debug)
  - opencv_world430.lib (Release)
Include: F:\OpenCV\opencv\build\include\
  - opencv2\core.hpp
  - opencv2\imgproc.hpp
  - opencv2\highgui.hpp
  - opencv2\imgcodecs.hpp
```

### Tesseract OCR (Optional)
```yaml
Version: 5.3.0+
Path: C:\Program Files\Tesseract-OCR\
Executable: C:\Program Files\Tesseract-OCR\tesseract.exe
Libraries: C:\Program Files\Tesseract-OCR\
  - libtesseract*.dll
  - leptonica*.dll
Language Data: C:\Program Files\Tesseract-OCR\tessdata\
  - eng.traineddata
  - ara.traineddata
  - osd.traineddata
```

### Font Awesome
```yaml
Version: 6.x Free
Path: F:\Naghuma Toolbox\resources\fonts\
Font File: fontawesome-webfont.ttf
Used For: Toolbar icons, menu icons, UI symbols
Icon Loading: src\MainWindow.cpp (lines 84-135)
```

---

## 📂 Project Structure with Component Paths

```
F:\Naghuma Toolbox\
├── include/                                    # All header files
│   ├── MainWindow.h                           # Main window (Q_OBJECT, 120 lines)
│   │   └── Color refs: Line 15 (selection overlay), Line 89 (sidebar bg)
│   ├── ImageCanvas.h                          # Image display widget (Q_OBJECT)
│   │   └── Color refs: Line 45 (canvas bg), Line 78 (border color)
│   ├── SelectionTool.h                        # Selection tool (Q_OBJECT, 91 lines)
│   │   └── Color refs: Line 22 (overlay color constant)
│   ├── ImageProcessor.h                       # Core algorithms (680 lines)
│   ├── LayerManager.h                         # Layer system (150 lines)
│   ├── RightSidebarWidget.h                   # Sidebar UI (Q_OBJECT)
│   │   └── Color refs: Line 32 (panel headers), Line 55 (dividers)
│   ├── ROISelectionWidget.h                   # ROI tool (Q_OBJECT)
│   ├── ThresholdDialog.h                      # Threshold dialog (Q_OBJECT)
│   ├── HistogramDialog.h                      # Histogram display (Q_OBJECT)
│   ├── HuffmanDialog.h                        # Compression dialog (Q_OBJECT)
│   ├── OCRDialog.h                            # OCR interface (Q_OBJECT)
│   └── CannyDialog.h                          # Canny edge dialog (Q_OBJECT)
│
├── src/                                       # Implementation files
│   ├── main.cpp                               # Entry point (80 lines)
│   │   └── Line 45: Application palette setup
│   ├── MainWindow.cpp                         # Main window logic (3328 lines)
│   │   └── Styling sections:
│   │       - Lines 140-210: Menu bar styling
│   │       - Lines 330-420: Toolbar styling
│   │       - Lines 1650-1750: Sidebar widget styling
│   │       - Lines 2402-2444: Selection mode toggle with overlay colors
│   │       - Lines 2445-2500: Visual feedback for selection
│   ├── SelectionTool.cpp                      # Selection implementation (430+ lines)
│   │   └── Overlay rendering:
│   │       - Lines 250-295: getMaskOverlay() - GREEN overlay generation
│   │       - Lines 298-306: getMaskAsLayer() - Layer visualization
│   │       - Lines 39-58: endRectangle() - Rectangle visual feedback
│   ├── ImageCanvas.cpp                        # Canvas rendering (400+ lines)
│   │   └── Paint events:
│   │       - Lines 85-120: Background rendering
│   │       - Lines 150-180: Image display
│   │       - Lines 200-250: Selection overlay compositing
│   ├── ImageProcessor.cpp                     # Algorithms (2500+ lines)
│   ├── RightSidebarWidget.cpp                 # Sidebar logic (500+ lines)
│   │   └── Widget styling:
│   │       - Lines 50-120: Panel creation with backgrounds
│   │       - Lines 180-220: Layer list styling
│   │       - Lines 280-320: Property widgets
│   ├── ROISelectionWidget.cpp                 # ROI widget (350 lines)
│   ├── ThresholdDialog.cpp                    # Threshold dialog (200 lines)
│   ├── HistogramDialog.cpp                    # Histogram (450 lines)
│   ├── HuffmanDialog.cpp                      # Compression (300 lines)
│   ├── OCRDialog.cpp                          # OCR dialog (400 lines)
│   └── CannyDialog.cpp                        # Canny dialog (250 lines)
│
├── lib/                                       # Custom libraries
│   ├── compression/
│   │   ├── HuffmanCoding.h                    # Huffman header (80 lines)
│   │   └── HuffmanCoding.cpp                  # Huffman implementation (600 lines)
│   └── ocr/
│       ├── TextRecognition.h                  # OCR wrapper header (50 lines)
│       └── TextRecognition.cpp                # OCR implementation (350 lines)
│
├── resources/                                 # Application resources
│   ├── fonts/
│   │   └── fontawesome-webfont.ttf           # Icon font (Font Awesome 6.x)
│   ├── icons/                                 # Custom SVG/PNG icons (if any)
│   └── images/                                # Sample images
│
├── x64/Debug/                                 # Debug build output
│   ├── Naghuma Toolbox.exe                   # Main executable (Debug)
│   ├── Qt6Cored.dll                          # Qt Core (Debug)
│   ├── Qt6Guid.dll                           # Qt GUI (Debug)
│   ├── Qt6Widgetsd.dll                       # Qt Widgets (Debug)
│   ├── opencv_world430d.dll                  # OpenCV (Debug)
│   ├── platforms\qwindowsd.dll               # Qt platform plugin (REQUIRED)
│   └── tessdata\                             # Tesseract language data
│       ├── eng.traineddata
│       └── ara.traineddata
│
└── Naghuma Toolbox.vcxproj                   # Project file with paths
    └── Key sections:
        - Lines 15-25: Qt paths configuration
        - Lines 30-40: OpenCV paths configuration
        - Lines 80-150: Include directories
        - Lines 200-250: Library directories
        - Lines 300-400: Source files list
```

---

## 🎨 UX Enhancement Specifications

### 1. Toolbar Improvements

**Current State:**
- Single row, text + icon buttons
- No grouping or visual separation
- Generic button styling

**New Design:**
```cpp
// Location: src/MainWindow.cpp, lines 330-420
// Replace existing toolbar creation with:

QToolBar* toolbar = new QToolBar(this);
toolbar->setObjectName("mainToolbar");
toolbar->setMovable(false);
toolbar->setIconSize(QSize(24, 24));
toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

// Apply modern styling
toolbar->setStyleSheet(R"(
    QToolBar {
        background: #2A2A3E;
        border: none;
        border-bottom: 1px solid #45475A;
        spacing: 8px;
        padding: 8px;
    }
    QToolButton {
        background: transparent;
        color: #CDD6F4;
        border: none;
        border-radius: 6px;
        padding: 8px 12px;
        font-size: 11px;
    }
    QToolButton:hover {
        background: #363651;
    }
    QToolButton:pressed {
        background: #45475A;
    }
    QToolButton:checked {
        background: #89B4FA;
        color: #1E1E2E;
    }
    QToolBar::separator {
        background: #45475A;
        width: 1px;
        margin: 8px 6px;
    }
)");

// Group buttons with separators
// File operations group
toolbar->addAction(openAction);
toolbar->addAction(saveAction);
toolbar->addSeparator();

// Processing group
toolbar->addAction(grayscaleAction);
toolbar->addAction(histogramAction);
toolbar->addSeparator();

// Selection & Analysis group
toolbar->addAction(selectionAction);
toolbar->addAction(edgeDetectionAction);
toolbar->addSeparator();

// Tools group
toolbar->addAction(cropAction);
toolbar->addAction(ocrAction);

addToolBar(Qt::TopToolBarArea, toolbar);
```

---

### 2. Selection Tool Visual Feedback

**Current State:**
- 60% green overlay (`rgba(0, 255, 0, 0.6)`)
- 3px red border (`#FF0000`)

**New Design:**
```cpp
// Location: src/SelectionTool.cpp, lines 250-295
// Replace getMaskOverlay() function:

cv::Mat SelectionTool::getMaskOverlay(const cv::Mat& image) const {
    if (currentMask.empty() || image.empty()) {
        return image.clone();
    }

    cv::Mat overlay = image.clone();
    
    // Ensure 3-channel color image
    if (overlay.channels() == 1) {
        cv::cvtColor(overlay, overlay, cv::COLOR_GRAY2BGR);
    }

    // NEW COLOR: Soft blue overlay (137, 180, 250) at 30% opacity
    cv::Vec3b fillColor(250, 180, 137);  // BGR format: (B, G, R)
    
    for (int y = 0; y < overlay.rows; ++y) {
        for (int x = 0; x < overlay.cols; ++x) {
            if (currentMask.at<uchar>(y, x) > 0) {
                cv::Vec3b& pixel = overlay.at<cv::Vec3b>(y, x);
                // Alpha blending: 30% fill color, 70% original
                pixel[0] = static_cast<uchar>(pixel[0] * 0.7 + fillColor[0] * 0.3);
                pixel[1] = static_cast<uchar>(pixel[1] * 0.7 + fillColor[1] * 0.3);
                pixel[2] = static_cast<uchar>(pixel[2] * 0.7 + fillColor[2] * 0.3);
            }
        }
    }

    // Draw blue border (2px for cleaner look)
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(currentMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Scalar borderColor(250, 180, 137);  // Soft blue in BGR
    cv::drawContours(overlay, contours, -1, borderColor, 2);

    return overlay;
}
```

---

### 3. Main Window Dark Theme

**Current State:**
- Light gray background
- Default Qt widget colors

**New Design:**
```cpp
// Location: src/main.cpp, lines 45-80
// Replace palette setup:

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application-wide dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30, 30, 46));           // #1E1E2E
    darkPalette.setColor(QPalette::WindowText, QColor(205, 214, 244));    // #CDD6F4
    darkPalette.setColor(QPalette::Base, QColor(24, 24, 37));             // #181825
    darkPalette.setColor(QPalette::AlternateBase, QColor(42, 42, 62));    // #2A2A3E
    darkPalette.setColor(QPalette::Text, QColor(205, 214, 244));          // #CDD6F4
    darkPalette.setColor(QPalette::Button, QColor(54, 54, 81));           // #363651
    darkPalette.setColor(QPalette::ButtonText, QColor(205, 214, 244));    // #CDD6F4
    darkPalette.setColor(QPalette::BrightText, Qt::white);
    darkPalette.setColor(QPalette::Link, QColor(137, 180, 250));          // #89B4FA
    darkPalette.setColor(QPalette::Highlight, QColor(137, 180, 250));     // #89B4FA
    darkPalette.setColor(QPalette::HighlightedText, QColor(30, 30, 46));  // #1E1E2E
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(166, 173, 200)); // #A6ADC8
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(166, 173, 200));
    
    app.setPalette(darkPalette);
    
    // Set global stylesheet
    app.setStyleSheet(R"(
        QMainWindow {
            background: #1E1E2E;
        }
        QMenuBar {
            background: #2A2A3E;
            color: #CDD6F4;
            border-bottom: 1px solid #45475A;
            padding: 4px;
        }
        QMenuBar::item {
            background: transparent;
            padding: 6px 12px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background: #363651;
        }
        QMenu {
            background: #2A2A3E;
            color: #CDD6F4;
            border: 1px solid #45475A;
            border-radius: 6px;
            padding: 4px;
        }
        QMenu::item {
            padding: 6px 30px 6px 20px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background: #363651;
        }
        QMenu::separator {
            height: 1px;
            background: #45475A;
            margin: 4px 8px;
        }
        QPushButton {
            background: #363651;
            color: #CDD6F4;
            border: 1px solid #45475A;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
        }
        QPushButton:hover {
            background: #45475A;
            border-color: #89B4FA;
        }
        QPushButton:pressed {
            background: #89B4FA;
            color: #1E1E2E;
        }
        QPushButton:disabled {
            background: #2A2A3E;
            color: #A6ADC8;
            border-color: #313244;
        }
        QDialog {
            background: #1E1E2E;
        }
        QLabel {
            color: #CDD6F4;
        }
        QLineEdit, QTextEdit, QPlainTextEdit {
            background: #181825;
            color: #CDD6F4;
            border: 1px solid #45475A;
            border-radius: 4px;
            padding: 6px;
        }
        QLineEdit:focus, QTextEdit:focus {
            border-color: #89B4FA;
        }
        QScrollBar:vertical {
            background: #2A2A3E;
            width: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background: #45475A;
            border-radius: 6px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background: #89B4FA;
        }
    )");
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
```

---

### 4. Sidebar Panel Redesign

**Current State:**
- Basic vertical layout
- No visual hierarchy
- Plain backgrounds

**New Design:**
```cpp
// Location: src/RightSidebarWidget.cpp, lines 50-120
// Replace panel creation:

void RightSidebarWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);
    
    // Layers Panel with collapsible header
    QWidget* layersPanel = createCollapsiblePanel("Layers", layerListWidget);
    mainLayout->addWidget(layersPanel);
    
    // Properties Panel
    QWidget* propertiesPanel = createCollapsiblePanel("Properties", propertiesWidget);
    mainLayout->addWidget(propertiesPanel);
    
    mainLayout->addStretch();
    
    // Apply sidebar styling
    setStyleSheet(R"(
        RightSidebarWidget {
            background: #181825;
            border-left: 1px solid #45475A;
        }
    )");
}

QWidget* RightSidebarWidget::createCollapsiblePanel(const QString& title, QWidget* content) {
    QWidget* panel = new QWidget();
    QVBoxLayout* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(0);
    
    // Panel header (collapsible)
    QPushButton* headerBtn = new QPushButton(title);
    headerBtn->setCheckable(true);
    headerBtn->setChecked(true);
    headerBtn->setStyleSheet(R"(
        QPushButton {
            background: #2A2A3E;
            color: #CDD6F4;
            border: none;
            border-bottom: 1px solid #45475A;
            text-align: left;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #363651;
        }
        QPushButton:checked::before {
            content: "▼ ";
        }
        QPushButton::before {
            content: "▶ ";
        }
    )");
    
    // Content container
    QWidget* contentContainer = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);
    contentLayout->setContentsMargins(8, 8, 8, 8);
    contentLayout->addWidget(content);
    
    contentContainer->setStyleSheet(R"(
        QWidget {
            background: #1E1E2E;
        }
    )");
    
    // Connect collapse/expand
    connect(headerBtn, &QPushButton::toggled, contentContainer, &QWidget::setVisible);
    
    panelLayout->addWidget(headerBtn);
    panelLayout->addWidget(contentContainer);
    
    return panel;
}
```

---

### 5. Enhanced Button Styles

**Current State:**
- Default Qt buttons
- No visual distinction between button types

**New Design:**
```cpp
// Location: src/MainWindow.cpp, add helper function at line 3300

QString MainWindow::getPrimaryButtonStyle() {
    return R"(
        QPushButton {
            background: #89B4FA;
            color: #1E1E2E;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #A5C4FF;
        }
        QPushButton:pressed {
            background: #6E99D9;
        }
        QPushButton:disabled {
            background: #45475A;
            color: #A6ADC8;
        }
    )";
}

QString MainWindow::getSecondaryButtonStyle() {
    return R"(
        QPushButton {
            background: #363651;
            color: #CDD6F4;
            border: 1px solid #45475A;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 13px;
        }
        QPushButton:hover {
            background: #45475A;
            border-color: #89B4FA;
        }
        QPushButton:pressed {
            background: #2A2A3E;
        }
    )";
}

QString MainWindow::getDangerButtonStyle() {
    return R"(
        QPushButton {
            background: #F38BA8;
            color: #1E1E2E;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #F5A0B5;
        }
        QPushButton:pressed {
            background: #D16E88;
        }
    )";
}

// Apply to dialogs:
// In ThresholdDialog.cpp, HistogramDialog.cpp, etc.
// Replace button creation with:
QPushButton* applyBtn = new QPushButton("Apply");
applyBtn->setStyleSheet(getPrimaryButtonStyle());

QPushButton* cancelBtn = new QPushButton("Cancel");
cancelBtn->setStyleSheet(getSecondaryButtonStyle());
```

---

### 6. Dialog Modernization

**Apply to ALL dialogs: ThresholdDialog, HistogramDialog, HuffmanDialog, OCRDialog, CannyDialog**

```cpp
// Example: src/ThresholdDialog.cpp (apply pattern to all dialogs)

ThresholdDialog::ThresholdDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Threshold Adjustment");
    setModal(true);
    setMinimumSize(450, 300);
    
    // Apply dark theme
    setStyleSheet(R"(
        QDialog {
            background: #1E1E2E;
        }
        QLabel {
            color: #CDD6F4;
            font-size: 13px;
        }
        QSlider::groove:horizontal {
            background: #2A2A3E;
            height: 6px;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #89B4FA;
            width: 16px;
            height: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
        QSlider::handle:horizontal:hover {
            background: #A5C4FF;
        }
        QSpinBox {
            background: #181825;
            color: #CDD6F4;
            border: 1px solid #45475A;
            border-radius: 4px;
            padding: 6px;
        }
        QSpinBox:focus {
            border-color: #89B4FA;
        }
        QGroupBox {
            color: #CDD6F4;
            border: 1px solid #45475A;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: 600;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 8px;
            background: #1E1E2E;
        }
    )");
    
    // Rest of dialog setup...
}
```

---

### 7. Tooltip Enhancements

**Add tooltips to all toolbar buttons and menu items:**

```cpp
// Location: src/MainWindow.cpp, in toolbar creation section

// Example tooltips with keyboard shortcuts
openAction->setToolTip("Open Image File (Ctrl+O)\n\nSupported formats: JPG, PNG, BMP, TIFF");
saveAction->setToolTip("Save Processed Image (Ctrl+S)\n\nSave the current processed result");
grayscaleAction->setToolTip("Convert to Grayscale (Ctrl+G)\n\nConvert image to single-channel grayscale");
selectionAction->setToolTip("Toggle Selection Mode (Ctrl+Shift+S)\n\nActivate selection tools:\n• Rectangle\n• Polygon\n• Magic Wand\n• Threshold\n• Edge-Based");
histogramAction->setToolTip("Show Histogram (Ctrl+H)\n\nDisplay RGB/Grayscale histogram");
edgeDetectionAction->setToolTip("Edge Detection (Ctrl+E)\n\nApply edge detection algorithms:\n• Sobel\n• Canny\n• Prewitt\n• Roberts\n• Laplacian");

// Style tooltips
QApplication::setStyleSheet(QApplication::styleSheet() + R"(
    QToolTip {
        background: #2A2A3E;
        color: #CDD6F4;
        border: 1px solid #45475A;
        border-radius: 4px;
        padding: 8px;
        font-size: 12px;
        opacity: 230;
    }
)");
```

---

### 8. Status Bar Improvements

**Current State:**
- Basic text status messages

**New Design:**
```cpp
// Location: src/MainWindow.cpp, in constructor

QStatusBar* statusBar = new QStatusBar(this);
setStatusBar(statusBar);

// Style status bar
statusBar->setStyleSheet(R"(
    QStatusBar {
        background: #2A2A3E;
        color: #CDD6F4;
        border-top: 1px solid #45475A;
        padding: 4px 8px;
    }
    QStatusBar::item {
        border: none;
    }
)");

// Add permanent widgets
// Image info label
imageInfoLabel = new QLabel("No image loaded");
imageInfoLabel->setStyleSheet("color: #BAC2DE; font-size: 12px;");
statusBar->addPermanentWidget(imageInfoLabel);

// Separator
QFrame* separator = new QFrame();
separator->setFrameShape(QFrame::VLine);
separator->setStyleSheet("background: #45475A; max-width: 1px;");
statusBar->addPermanentWidget(separator);

// Processing status
processingLabel = new QLabel("Ready");
processingLabel->setStyleSheet("color: #A6E3A1; font-size: 12px; font-weight: 600;");
statusBar->addPermanentWidget(processingLabel);

// Update on image load:
void MainWindow::updateStatusBar() {
    if (imageLoaded) {
        QString info = QString("Image: %1x%2 | Channels: %3 | Type: %4")
            .arg(originalImage.cols)
            .arg(originalImage.rows)
            .arg(originalImage.channels())
            .arg(originalImage.type() == CV_8UC1 ? "Grayscale" : "Color");
        imageInfoLabel->setText(info);
    }
}
```

---

### 9. Loading Indicators

**Add visual feedback for long operations:**

```cpp
// Location: src/MainWindow.cpp, add new functions

void MainWindow::showLoadingOverlay(const QString& message) {
    // Create semi-transparent overlay
    loadingOverlay = new QWidget(this);
    loadingOverlay->setStyleSheet("background: rgba(30, 30, 46, 0.85);");
    loadingOverlay->setGeometry(0, 0, width(), height());
    
    QVBoxLayout* layout = new QVBoxLayout(loadingOverlay);
    layout->setAlignment(Qt::AlignCenter);
    
    // Spinner (use QMovie or custom animation)
    QLabel* spinner = new QLabel("⏳");
    spinner->setStyleSheet("font-size: 48px; color: #89B4FA;");
    spinner->setAlignment(Qt::AlignCenter);
    layout->addWidget(spinner);
    
    // Message
    QLabel* msgLabel = new QLabel(message);
    msgLabel->setStyleSheet("color: #CDD6F4; font-size: 16px; font-weight: 600;");
    msgLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(msgLabel);
    
    loadingOverlay->show();
    QApplication::processEvents();
}

void MainWindow::hideLoadingOverlay() {
    if (loadingOverlay) {
        loadingOverlay->deleteLater();
        loadingOverlay = nullptr;
    }
}

// Use in long operations:
void MainWindow::onOCRClicked() {
    showLoadingOverlay("Recognizing text...");
    
    // Perform OCR in QFuture or separate thread
    QFuture<QString> future = QtConcurrent::run([this]() {
        return performOCR();
    });
    
    QFutureWatcher<QString>* watcher = new QFutureWatcher<QString>(this);
    connect(watcher, &QFutureWatcher<QString>::finished, [this, watcher]() {
        hideLoadingOverlay();
        QString result = watcher->result();
        showOCRDialog(result);
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}
```

---

### 10. Animation & Transitions

**Add smooth transitions for UI state changes:**

```cpp
// Location: src/MainWindow.cpp, add animation helpers

void MainWindow::animateSelectionModeToggle(bool enabled) {
    // Animate selection button color change
    QPropertyAnimation* animation = new QPropertyAnimation(selectionButton, "styleSheet");
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    
    if (enabled) {
        animation->setStartValue(getSecondaryButtonStyle());
        animation->setEndValue(R"(
            QPushButton {
                background: #89B4FA;
                color: #1E1E2E;
                border: none;
                border-radius: 6px;
                padding: 8px 12px;
            }
        )");
    } else {
        animation->setStartValue(animation->endValue());
        animation->setEndValue(getSecondaryButtonStyle());
    }
    
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// Apply to all mode toggles and panel expansions
```

---

## 🔨 Implementation Checklist

### Phase 1: Color Palette Application (2-3 hours)
- [ ] Update `main.cpp` with dark palette setup
- [ ] Apply global stylesheet to QApplication
- [ ] Update all dialog backgrounds and text colors
- [ ] Modify button styles (primary, secondary, danger)
- [ ] Update menu bar and menu styling
- [ ] Update toolbar styling
- [ ] Style status bar

### Phase 2: Selection Tool Colors (1 hour)
- [ ] Change selection overlay from green to soft blue
- [ ] Update `SelectionTool.cpp` getMaskOverlay() function
- [ ] Change border color from red to blue
- [ ] Reduce border width from 3px to 2px
- [ ] Update layer visualization colors

### Phase 3: Sidebar Redesign (2 hours)
- [ ] Update `RightSidebarWidget.cpp` styling
- [ ] Implement collapsible panels
- [ ] Add panel headers with expand/collapse
- [ ] Update layer list styling
- [ ] Style property widgets

### Phase 4: Toolbar Enhancement (1-2 hours)
- [ ] Add button grouping with separators
- [ ] Update icon sizes and spacing
- [ ] Apply hover/active/checked states
- [ ] Add text under icons for clarity

### Phase 5: Dialog Modernization (3-4 hours)
- [ ] Update ThresholdDialog styling
- [ ] Update HistogramDialog styling
- [ ] Update HuffmanDialog styling
- [ ] Update OCRDialog styling
- [ ] Update CannyDialog styling
- [ ] Standardize button layouts
- [ ] Apply consistent padding/margins

### Phase 6: UX Enhancements (2-3 hours)
- [ ] Add tooltips to all actions (100+ tooltips)
- [ ] Implement loading overlays
- [ ] Add status bar widgets
- [ ] Create button animation helpers
- [ ] Add transition effects

### Phase 7: Testing & Refinement (2-3 hours)
- [ ] Test all 5 selection methods with new colors
- [ ] Verify contrast ratios (WCAG AA compliance)
- [ ] Test all dialogs for consistency
- [ ] Check tooltip readability
- [ ] Verify dark theme in all states
- [ ] Test keyboard shortcuts
- [ ] Validate loading indicators

### Phase 8: Documentation (1-2 hours)
- [ ] Update USER_GUIDE.md with new screenshots
- [ ] Document color palette in separate guide
- [ ] Create UX changelog
- [ ] Update README with new visuals

**Total Estimated Time:** 14-20 hours

---

## 📝 Implementation Commands

### Step-by-Step Build Process

```powershell
# 1. Clone original repository
cd F:\
git clone https://github.com/yourusername/Naghuma-Toolbox.git "Naghuma Toolbox Custom"
cd "Naghuma Toolbox Custom"

# 2. Create new branch for customization
git checkout -b feature/dark-theme-ux

# 3. Apply color changes to main.cpp
# Edit: src/main.cpp (lines 45-80)
# Replace QPalette setup with new dark palette from this prompt

# 4. Apply selection tool color changes
# Edit: src/SelectionTool.cpp (lines 250-295)
# Replace getMaskOverlay() with new blue overlay implementation

# 5. Update MainWindow styling
# Edit: src/MainWindow.cpp
# - Lines 140-210: Menu bar styling
# - Lines 330-420: Toolbar styling
# - Lines 1650-1750: Sidebar styling

# 6. Update all dialog files
# Edit: src/ThresholdDialog.cpp, src/HistogramDialog.cpp, etc.
# Apply dialog stylesheet from this prompt

# 7. Regenerate MOC files
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
& $qtMoc "include\MainWindow.h" -o "src\moc_MainWindow.cpp"
& $qtMoc "include\SelectionTool.h" -o "src\moc_SelectionTool.cpp"
& $qtMoc "include\RightSidebarWidget.h" -o "src\moc_RightSidebarWidget.cpp"
# ... repeat for all Q_OBJECT classes

# 8. Clean build
$msbuild = "F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "Naghuma Toolbox.vcxproj" /t:Clean /p:Configuration=Debug /p:Platform=x64

# 9. Build with new changes
& $msbuild "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Debug /p:Platform=x64 /m

# 10. Copy required DLLs
$qtBin = "C:\Qt\6.7.3\msvc2019_64\bin"
$debugDir = "x64\Debug"
Copy-Item "$qtBin\Qt6Cored.dll" -Destination $debugDir -Force
Copy-Item "$qtBin\Qt6Guid.dll" -Destination $debugDir -Force
Copy-Item "$qtBin\Qt6Widgetsd.dll" -Destination $debugDir -Force
New-Item -Path "$debugDir\platforms" -ItemType Directory -Force
Copy-Item "$qtBin\..\plugins\platforms\qwindowsd.dll" -Destination "$debugDir\platforms\" -Force
Copy-Item "F:\OpenCV\opencv\build\x64\vc15\bin\opencv_world430d.dll" -Destination $debugDir -Force

# 11. Run and test
.\x64\Debug\Naghuma' 'Toolbox.exe

# 12. Commit changes
git add .
git commit -m "🎨 Implement dark theme with modern UX

- Applied Catppuccin-inspired color palette
- Changed selection overlay from green to soft blue
- Redesigned all dialogs with dark theme
- Enhanced toolbar with grouped buttons
- Added collapsible sidebar panels
- Implemented 100+ tooltips with keyboard shortcuts
- Styled status bar with permanent widgets
- Added loading overlays for long operations
- Applied smooth animations and transitions
- Ensured WCAG AA contrast compliance"

# 13. Create release build
& $msbuild "Naghuma Toolbox.vcxproj" /t:Build /p:Configuration=Release /p:Platform=x64 /m
```

---

## 🎨 Color Palette Quick Reference

### CSS Variables for External Tools

```css
/* Copy these to any CSS/SCSS files if needed */
:root {
  /* Backgrounds */
  --bg-primary: #1E1E2E;
  --bg-secondary: #2A2A3E;
  --bg-tertiary: #363651;
  --bg-sidebar: #181825;
  
  /* Accents */
  --accent-blue: #89B4FA;
  --accent-pink: #F5C2E7;
  --accent-green: #A6E3A1;
  --accent-yellow: #F9E2AF;
  --accent-red: #F38BA8;
  --accent-teal: #94E2D5;
  
  /* Text */
  --text-primary: #CDD6F4;
  --text-secondary: #BAC2DE;
  --text-muted: #A6ADC8;
  
  /* Borders */
  --border-primary: #45475A;
  --border-secondary: #313244;
  --border-focus: #89B4FA;
}
```

### RGB Values for OpenCV

```cpp
// Use in cv::Scalar or cv::Vec3b (BGR format)
const cv::Scalar BG_PRIMARY(46, 30, 30);        // #1E1E2E
const cv::Scalar BG_SECONDARY(62, 42, 42);      // #2A2A3E
const cv::Scalar ACCENT_BLUE(250, 180, 137);    // #89B4FA
const cv::Scalar ACCENT_GREEN(161, 227, 166);   // #A6E3A1
const cv::Scalar ACCENT_RED(168, 139, 243);     // #F38BA8
const cv::Scalar TEXT_PRIMARY(244, 214, 205);   // #CDD6F4
const cv::Scalar BORDER_PRIMARY(90, 71, 69);    // #45475A
```

### QColor Values for Qt

```cpp
// Use in QPalette or setColor()
const QColor BG_PRIMARY(30, 30, 46);         // #1E1E2E
const QColor BG_SECONDARY(42, 42, 62);       // #2A2A3E
const QColor ACCENT_BLUE(137, 180, 250);     // #89B4FA
const QColor ACCENT_PINK(245, 194, 231);     // #F5C2E7
const QColor ACCENT_GREEN(166, 227, 161);    // #A6E3A1
const QColor TEXT_PRIMARY(205, 214, 244);    // #CDD6F4
const QColor BORDER_PRIMARY(69, 71, 90);     // #45475A
```

---

## 🚀 Final Deliverables

### Expected Output Files
1. **Modified Source Files:**
   - `src/main.cpp` - Dark palette setup
   - `src/MainWindow.cpp` - Toolbar, menu, status bar styling
   - `src/SelectionTool.cpp` - Blue overlay implementation
   - `src/RightSidebarWidget.cpp` - Collapsible panels
   - `src/ThresholdDialog.cpp` - Dark theme
   - `src/HistogramDialog.cpp` - Dark theme
   - `src/HuffmanDialog.cpp` - Dark theme
   - `src/OCRDialog.cpp` - Dark theme
   - `src/CannyDialog.cpp` - Dark theme
   - `src/ImageCanvas.cpp` - Canvas styling updates

2. **New Documentation:**
   - `COLOR_PALETTE_GUIDE.md` - Complete color reference
   - `UX_ENHANCEMENTS.md` - List of UX improvements
   - `DARK_THEME_CHANGELOG.md` - All visual changes

3. **Updated Documentation:**
   - `USER_GUIDE.md` - New screenshots and color references
   - `README.md` - Updated description with "dark theme" mention

4. **Build Artifacts:**
   - `x64/Debug/Naghuma Toolbox.exe` - Debug build with new theme
   - `x64/Release/Naghuma Toolbox.exe` - Release build with new theme

---

## ✅ Validation Criteria

### Visual Verification
- [ ] All backgrounds use dark palette (#1E1E2E, #2A2A3E)
- [ ] Selection overlay shows soft blue (#89B4FA) at 30% opacity
- [ ] Selection border is 2px blue (#89B4FA)
- [ ] All text is readable with sufficient contrast (min 4.5:1)
- [ ] Buttons show distinct hover/pressed/disabled states
- [ ] Toolbar buttons are visually grouped with separators
- [ ] Sidebar panels are collapsible with chevron indicators
- [ ] All dialogs have consistent dark theme
- [ ] Status bar shows persistent info widgets
- [ ] Tooltips display correctly with dark background

### Functional Verification
- [ ] All 5 selection methods work with new colors
- [ ] Layer system saves selections correctly
- [ ] Sidebar panels expand/collapse smoothly
- [ ] Loading overlays appear for long operations
- [ ] Animations are smooth (200-300ms duration)
- [ ] Keyboard shortcuts still work
- [ ] All dialogs are functional
- [ ] Status bar updates correctly

### Accessibility Verification
- [ ] Contrast ratios meet WCAG AA (4.5:1 for text, 3:1 for UI)
- [ ] Focus indicators are visible (#89B4FA borders)
- [ ] Disabled states are clearly distinguished
- [ ] Tooltips provide additional context
- [ ] Keyboard navigation works throughout

---

## 🆕 Recent Feature Additions (v2.1)

### 1. Interactive Blur Dialog

**Feature Overview:**
Replace fixed-parameter blur functions with an interactive dialog providing real-time preview and multiple filter types.

**Location:** `include/BlurDialog.h`, `src/BlurDialog.cpp`

**Key Components:**
```cpp
class BlurDialog : public QDialog {
    Q_OBJECT
public:
    enum BlurType {
        Gaussian,    // Standard Gaussian blur with kernel size
        Median,      // Median filter for salt-and-pepper noise
        Bilateral    // Edge-preserving bilateral filter
    };
    
    BlurDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    BlurType getBlurType() const { return currentBlurType; }
    int getKernelSize() const { return kernelSize; }
};
```

**Features:**
1. **Three Blur Types:**
   - **Gaussian Blur:** Standard smoothing with adjustable kernel size (1-31)
   - **Median Filter:** Removes salt-and-pepper noise, adjustable kernel size (1-31)
   - **Bilateral Filter:** Edge-preserving smoothing with:
     - Kernel size (1-31)
     - Sigma Color (10-200): Color space filtering strength
     - Sigma Space (10-200): Coordinate space filtering strength

2. **Real-Time Preview:**
   - Scaled preview image for quick feedback
   - Updates on parameter changes
   - Efficient processing with cv::resize for display

3. **Interactive Controls:**
   - QSlider for kernel size with odd-number enforcement
   - QSpinBox for precise value entry
   - Bilateral-specific sliders for sigma parameters
   - QComboBox for blur type selection

4. **Selection Tool Integration:**
   - Supports selective blurring with active selection mask
   - Applied via `selectionTool->applyMaskToResult()`
   - Layer system records blur type and parameters

**UI Design Guidelines for Customization:**
```cpp
// Apply dark theme to dialog
dialog.setStyleSheet(R"(
    QDialog {
        background: #1E1E2E;
        color: #CDD6F4;
    }
    QLabel {
        color: #CDD6F4;
        font-size: 13px;
    }
    QSlider::groove:horizontal {
        background: #45475A;
        height: 4px;
        border-radius: 2px;
    }
    QSlider::handle:horizontal {
        background: #89B4FA;
        width: 16px;
        height: 16px;
        border-radius: 8px;
        margin: -6px 0;
    }
    QSlider::handle:horizontal:hover {
        background: #A5C4FF;
    }
    QComboBox {
        background: #363651;
        color: #CDD6F4;
        border: 1px solid #45475A;
        border-radius: 6px;
        padding: 6px 12px;
        font-size: 13px;
    }
    QComboBox:hover {
        border-color: #89B4FA;
        background: #45475A;
    }
    QComboBox::drop-down {
        border: none;
    }
    QComboBox::down-arrow {
        image: url(:/icons/chevron-down.png);
        width: 12px;
        height: 12px;
    }
    QSpinBox {
        background: #363651;
        color: #CDD6F4;
        border: 1px solid #45475A;
        border-radius: 6px;
        padding: 4px 8px;
        font-size: 13px;
    }
    QSpinBox:hover {
        border-color: #89B4FA;
    }
    QSpinBox::up-button, QSpinBox::down-button {
        background: #45475A;
        border: none;
        border-radius: 3px;
    }
    QSpinBox::up-button:hover, QSpinBox::down-button:hover {
        background: #89B4FA;
    }
    QGroupBox {
        color: #CDD6F4;
        border: 1px solid #45475A;
        border-radius: 8px;
        margin-top: 12px;
        padding-top: 12px;
        font-size: 13px;
        font-weight: 600;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top left;
        padding: 0 8px;
        background: #1E1E2E;
    }
)");
```

**Integration Points:**
- **Menu Location:** Filter > Gaussian Blur (replaced with BlurDialog)
- **Keyboard Shortcut:** Could add Ctrl+B for blur
- **Layer Recording:** Creates layer named "Gaussian/Median/Bilateral Blur (kernel: X)"
- **Undo Support:** Full integration with undo system

---

### 2. Resolution Enhancement Dialog

**Feature Overview:**
Upscale images with multiple interpolation methods, including edge-directed enhancement and post-processing sharpening.

**Location:** `include/ResolutionEnhancementDialog.h`, `src/ResolutionEnhancementDialog.cpp`

**Key Components:**
```cpp
class ResolutionEnhancementDialog : public QDialog {
    Q_OBJECT
public:
    enum InterpolationMethod {
        Nearest,        // Fast, blocky results
        Bilinear,       // Smooth, slightly blurry
        Bicubic,        // High quality, slower
        Lanczos4,       // Best quality, slowest
        EdgeDirected    // Custom: preserves edges with cv::edgePreservingFilter
    };
    
    ResolutionEnhancementDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    double getScaleFactor() const { return scaleFactor; }
    InterpolationMethod getMethod() const { return currentMethod; }
};
```

**Features:**
1. **Five Interpolation Methods:**
   - **Nearest Neighbor (cv::INTER_NEAREST):** 
     - Fastest processing
     - Blocky/pixelated results
     - Best for pixel art or when exact pixel values matter
   
   - **Bilinear (cv::INTER_LINEAR):**
     - Fast processing
     - Smooth results, slight blur
     - Good for moderate upscaling (1.5-2x)
   
   - **Bicubic (cv::INTER_CUBIC):**
     - High-quality interpolation
     - Smoother than bilinear
     - Standard for photo upscaling
   
   - **Lanczos4 (cv::INTER_LANCZOS4):**
     - Highest quality OpenCV method
     - Sharp edges, minimal artifacts
     - Best for critical work (2-4x scaling)
   
   - **Edge-Directed (Custom):**
     - Uses cv::Canny edge detection
     - Applies cv::edgePreservingFilter for detail preservation
     - Post-processes with Lanczos4
     - Best for images with fine details/text

2. **Scale Factor Control:**
   - Slider range: 1.0x to 4.0x (100% to 400%)
   - Real-time target size display
   - Megapixel count calculation
   - Warning for very large outputs (>100MP)

3. **Post-Processing Sharpening:**
   - Optional unsharp mask application
   - Slider range: 0-100%
   - Implementation:
     ```cpp
     cv::Mat blurred;
     cv::GaussianBlur(image, blurred, cv::Size(0, 0), 1.5);
     cv::addWeighted(image, 1.0 + sharpenAmount, blurred, -sharpenAmount, 0, result);
     ```

4. **Real-Time Preview:**
   - Shows upscaled result with selected method
   - Performance-optimized display scaling
   - Current vs. target resolution display

5. **Memory Safety:**
   - Calculates output size before processing
   - Warns user if result exceeds 100 megapixels
   - Confirmation dialog for very large operations

**Edge-Directed Enhancement Algorithm:**
```cpp
void enhanceResolution(const cv::Mat& input, cv::Mat& output, 
                      double scale, InterpolationMethod method) {
    if (method == EdgeDirected) {
        // 1. Initial upscale with Lanczos4
        cv::Mat upscaled;
        cv::resize(input, upscaled, cv::Size(), scale, scale, cv::INTER_LANCZOS4);
        
        // 2. Detect edges at original resolution
        cv::Mat gray, edges;
        if (input.channels() == 3)
            cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        else
            gray = input.clone();
        
        cv::Canny(gray, edges, 50, 150);
        
        // 3. Apply edge-preserving filter
        cv::edgePreservingFilter(upscaled, output, cv::RECURS_FILTER, 60, 0.4f);
        
        // 4. Optional sharpening pass
        if (sharpenAmount > 0) {
            applySharpeningPass(output, output, sharpenAmount);
        }
    } else {
        // Standard OpenCV interpolation
        int interpolation = getInterpolationFlag(method);
        cv::resize(input, output, cv::Size(), scale, scale, interpolation);
    }
}
```

**Selection Tool Integration:**
- When selection is active, upscales the mask too
- Uses cv::INTER_NEAREST for mask to preserve binary values
- Blends upscaled region with original resolution base
- Creates seamless composite result

**UI Design Guidelines for Customization:**
```cpp
// Current/Target Size Display
QLabel* sizeLabel = new QLabel();
sizeLabel->setStyleSheet(R"(
    QLabel {
        background: #363651;
        color: #CDD6F4;
        border: 1px solid #45475A;
        border-radius: 8px;
        padding: 16px;
        font-size: 13px;
    }
)");

// Method Description
QString getMethodDescription(InterpolationMethod method) {
    switch (method) {
        case Nearest:
            return "Fastest • Blocky results • Best for pixel art";
        case Bilinear:
            return "Fast • Smooth results • Good for moderate scaling";
        case Bicubic:
            return "High quality • Standard for photos";
        case Lanczos4:
            return "Best quality • Sharpest results • Slowest";
        case EdgeDirected:
            return "Custom • Preserves edges • Best for detailed images";
    }
}

// Warning for large images
if (outputMegapixels > 100.0) {
    QMessageBox::StandardButton reply = QMessageBox::warning(
        this, 
        "Large Output Size",
        QString("The output image will be %.1f megapixels (%.0f MB estimated).\n\n"
                "Processing may take several seconds and use significant memory.\n\n"
                "Continue?").arg(outputMegapixels).arg(outputMegapixels * 3 / 1024.0 * 1024.0),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::No) return;
}
```

**Integration Points:**
- **Menu Location:** Transform > Enhance Resolution (Upscale)...
- **Keyboard Shortcut:** Could add Ctrl+Shift+R
- **Layer Recording:** Creates layer named "Resolution 2.50x (Bicubic)" with scale and method
- **Undo Support:** Full integration with undo system
- **Status Bar:** Shows "Resolution enhanced 2.5x using Bicubic" on completion

**Performance Characteristics:**
| Method | 2x Upscale (1MP→4MP) | 4x Upscale (1MP→16MP) | Quality |
|--------|---------------------|----------------------|---------|
| Nearest | ~5ms | ~20ms | ★☆☆☆☆ |
| Bilinear | ~15ms | ~60ms | ★★☆☆☆ |
| Bicubic | ~30ms | ~120ms | ★★★★☆ |
| Lanczos4 | ~50ms | ~200ms | ★★★★★ |
| Edge-Directed | ~100ms | ~400ms | ★★★★★ |

**Use Cases:**
1. **Print Preparation:** Upscale for higher DPI (Lanczos4 or Edge-Directed)
2. **Web Display:** Moderate scaling with quality (Bicubic)
3. **Icon/Sprite Upscaling:** Pixel-perfect scaling (Nearest)
4. **Photo Enhancement:** Maximize detail preservation (Edge-Directed with sharpening)
5. **Quick Preview:** Fast scaling for display (Bilinear)

---

## 🎯 Advanced Feature Dialogs (Phase 16-23)

### 3. Thresholding Dialog (Phase 16)

**Feature Overview:**
Multi-method thresholding dialog with real-time preview and adaptive threshold algorithms.

**Location:** `include/ThresholdingDialog.h`, `src/ThresholdingDialog.cpp`

**Key Components:**
```cpp
class ThresholdingDialog : public QDialog {
    Q_OBJECT
public:
    enum ThresholdMethod {
        Binary,           // Standard binary threshold
        BinaryInverted,   // Inverted binary
        Truncate,         // Truncate values above threshold
        ToZero,           // Set to zero below threshold
        ToZeroInverted,   // Set to zero above threshold
        Otsu,             // Automatic Otsu threshold
        AdaptiveMean,     // Adaptive threshold using mean
        AdaptiveGaussian, // Adaptive threshold using Gaussian
        MultiOtsu         // Multi-level Otsu (3 classes)
    };
    
    ThresholdingDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    ThresholdMethod getMethod() const { return currentMethod; }
    int getThresholdValue() const { return thresholdValue; }
};
```

**Features:**
1. **Nine Threshold Methods:**
   - **Binary (cv::THRESH_BINARY):** Standard binary thresholding
   - **Binary Inverted (cv::THRESH_BINARY_INV):** Inverted binary
   - **Truncate (cv::THRESH_TRUNC):** Truncate at threshold
   - **To Zero (cv::THRESH_TOZERO):** Zero below threshold
   - **To Zero Inverted (cv::THRESH_TOZERO_INV):** Zero above threshold
   - **Otsu (cv::THRESH_OTSU):** Automatic optimal threshold
   - **Adaptive Mean (cv::ADAPTIVE_THRESH_MEAN_C):** Local mean-based
   - **Adaptive Gaussian (cv::ADAPTIVE_THRESH_GAUSSIAN_C):** Gaussian-weighted
   - **Multi-Otsu:** Multi-level thresholding (3 classes)

2. **Interactive Controls:**
   - Threshold value slider (0-255)
   - Block size for adaptive methods (3-99, odd only)
   - C constant for adaptive methods (-50 to +50)
   - Real-time preview with histogram overlay

3. **Histogram Visualization:**
   - Shows intensity distribution
   - Marks threshold value with vertical line
   - Displays pixel counts for each class

**UI Design:**
```cpp
// Dark theme styling
dialog.setStyleSheet(R"(
    QDialog {
        background: #1E1E2E;
        color: #CDD6F4;
    }
    QGroupBox {
        color: #89B4FA;
        border: 2px solid #45475A;
        border-radius: 8px;
        margin-top: 12px;
        padding-top: 16px;
        font-weight: 600;
        font-size: 14px;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top left;
        padding: 0 8px;
        background: #1E1E2E;
    }
    QComboBox {
        background: #363651;
        color: #CDD6F4;
        border: 1px solid #45475A;
        border-radius: 6px;
        padding: 8px 12px;
        font-size: 13px;
        min-width: 200px;
    }
    QComboBox:hover {
        border-color: #89B4FA;
        background: #45475A;
    }
    QComboBox::drop-down {
        border: none;
        width: 24px;
    }
    QComboBox::down-arrow {
        image: none;
        border-left: 4px solid transparent;
        border-right: 4px solid transparent;
        border-top: 6px solid #CDD6F4;
        margin-right: 8px;
    }
    QSlider::groove:horizontal {
        background: #2A2A3E;
        height: 6px;
        border-radius: 3px;
    }
    QSlider::handle:horizontal {
        background: #89B4FA;
        width: 18px;
        height: 18px;
        margin: -6px 0;
        border-radius: 9px;
    }
    QSlider::handle:horizontal:hover {
        background: #A5C4FF;
    }
    QLabel {
        color: #CDD6F4;
        font-size: 13px;
    }
)");
```

**Integration Points:**
- **Menu Location:** Segmentation > Thresholding...
- **Layer Recording:** Creates layer named "Threshold: Binary (128)" with method and value
- **Selection Tool:** Can create selection mask from threshold result
- **Undo Support:** Full integration

---

### 4. Advanced Segmentation Dialog (Phase 17)

**Feature Overview:**
Region-based segmentation with watershed, K-means, Mean-shift, and GrabCut algorithms.

**Location:** `include/SegmentationDialog.h`, `src/SegmentationDialog.cpp`

**Key Components:**
```cpp
class SegmentationDialog : public QDialog {
    Q_OBJECT
public:
    enum SegmentationMethod {
        Watershed,       // Marker-based watershed segmentation
        KMeans,          // K-means color clustering
        MeanShift,       // Mean-shift color segmentation
        GrabCut,         // Interactive foreground extraction
        Felzenszwalb     // Graph-based segmentation
    };
    
    SegmentationDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    cv::Mat getSegmentationMask() const { return segmentMask; }
    SegmentationMethod getMethod() const { return currentMethod; }
};
```

**Features:**
1. **Five Segmentation Methods:**
   - **Watershed (cv::watershed):**
     - Marker-based segmentation
     - Auto-generates markers using distance transform
     - Manual marker placement mode (coming soon)
     - Displays segment boundaries as colored overlay
   
   - **K-Means (cv::kmeans):**
     - Clusters: 2-16 selectable
     - Iterations: 10-100
     - Epsilon: 0.1-10.0 (convergence threshold)
     - Visualization: Each cluster gets unique color
   
   - **Mean-Shift (cv::pyrMeanShiftFiltering):**
     - Spatial radius: 5-50
     - Color radius: 10-100
     - Max pyramid level: 1-3
     - Preserves edges while merging similar regions
   
   - **GrabCut (cv::grabCut):**
     - Interactive rectangle selection
     - Iterations: 1-10
     - Foreground/background separation
     - Optional manual refinement with brushes
   
   - **Felzenszwalb (Graph-based):**
     - Sigma: 0.1-2.0 (smoothing)
     - K parameter: 100-1000 (segmentation scale)
     - Min size: 10-500 (minimum segment size)

2. **Interactive Controls:**
   - Method-specific parameter sliders
   - Real-time preview updates
   - Segment count display
   - Color-coded overlay modes

3. **Post-Processing:**
   - Morphological cleanup (opening/closing)
   - Small region removal
   - Boundary smoothing
   - Export segments as separate layers

**Algorithm Implementation:**
```cpp
void applyWatershed(const cv::Mat& input, cv::Mat& output, cv::Mat& markers) {
    // 1. Convert to grayscale for processing
    cv::Mat gray;
    if (input.channels() == 3)
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    else
        gray = input.clone();
    
    // 2. Apply noise reduction
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
    
    // 3. Detect edges
    cv::Mat edges;
    cv::Canny(blurred, edges, 50, 150);
    
    // 4. Distance transform
    cv::Mat dist;
    cv::distanceTransform(~edges, dist, cv::DIST_L2, 5);
    
    // 5. Threshold distance to find sure foreground
    cv::Mat sureFg;
    double maxVal;
    cv::minMaxLoc(dist, nullptr, &maxVal);
    cv::threshold(dist, sureFg, 0.5 * maxVal, 255, cv::THRESH_BINARY);
    sureFg.convertTo(sureFg, CV_8U);
    
    // 6. Find sure background (dilated edges)
    cv::Mat sureBg;
    cv::dilate(edges, sureBg, cv::Mat(), cv::Point(-1, -1), 3);
    
    // 7. Find unknown region
    cv::Mat unknown;
    cv::subtract(sureBg, sureFg, unknown);
    
    // 8. Label markers
    cv::connectedComponents(sureFg, markers);
    markers = markers + 1; // Background is 1, unknown is 0
    markers.setTo(0, unknown == 255);
    
    // 9. Apply watershed
    cv::Mat colorImg = input.clone();
    cv::watershed(colorImg, markers);
    
    // 10. Create colored output
    output = input.clone();
    cv::RNG rng(12345);
    std::vector<cv::Vec3b> colors(markers.rows * markers.cols);
    for (auto& color : colors) {
        color = cv::Vec3b(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    }
    
    for (int y = 0; y < markers.rows; y++) {
        for (int x = 0; x < markers.cols; x++) {
            int label = markers.at<int>(y, x);
            if (label == -1) // Boundary
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0); // Red boundaries
            else if (label > 0)
                output.at<cv::Vec3b>(y, x) = colors[label];
        }
    }
}
```

**Integration Points:**
- **Menu Location:** Segmentation > Region-Based Segmentation...
- **Layer Recording:** Creates layer named "Watershed (12 regions)" with method and count
- **Export Options:** Save each segment as separate image
- **Mask Generation:** Can create selection masks from segments

---

### 5. Feature Detection Dialog (Phase 19)

**Feature Overview:**
Detect and visualize key features using SIFT, SURF, ORB, FAST, and Harris corner detection.

**Location:** `include/FeatureDetectionDialog.h`, `src/FeatureDetectionDialog.cpp`

**Key Components:**
```cpp
class FeatureDetectionDialog : public QDialog {
    Q_OBJECT
public:
    enum FeatureMethod {
        SIFT,           // Scale-Invariant Feature Transform
        SURF,           // Speeded Up Robust Features
        ORB,            // Oriented FAST and Rotated BRIEF
        FAST,           // Features from Accelerated Segment Test
        HarrisCorner,   // Harris corner detector
        ShiTomasi,      // Shi-Tomasi good features to track
        BRISK,          // Binary Robust Invariant Scalable Keypoints
        AKAZE           // Accelerated-KAZE
    };
    
    FeatureDetectionDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    std::vector<cv::KeyPoint> getKeypoints() const { return keypoints; }
    cv::Mat getDescriptors() const { return descriptors; }
};
```

**Features:**
1. **Eight Feature Detectors:**
   - **SIFT (cv::SIFT::create()):**
     - Features: 0-10000 (max features)
     - Octaves: 3-8
     - Contrast threshold: 0.01-0.1
     - Edge threshold: 5-20
     - Scale-invariant, rotation-invariant
   
   - **SURF (cv::xfeatures2d::SURF::create()):**
     - Hessian threshold: 100-1000
     - Octaves: 3-6
     - Octave layers: 2-4
     - Extended: 64 or 128 descriptors
   
   - **ORB (cv::ORB::create()):**
     - Features: 100-5000
     - Scale factor: 1.2-2.0
     - Levels: 4-12
     - Edge threshold: 10-50
     - Fast and efficient
   
   - **FAST (cv::FastFeatureDetector::create()):**
     - Threshold: 1-50
     - Non-max suppression: On/Off
     - Detector type: TYPE_5_8, TYPE_7_12, TYPE_9_16
   
   - **Harris Corner (cv::cornerHarris()):**
     - Block size: 2-7
     - Aperture size: 3, 5, 7
     - k parameter: 0.01-0.20
     - Threshold: 0.01-0.1 (relative to max response)
   
   - **Shi-Tomasi (cv::goodFeaturesToTrack()):**
     - Max corners: 50-2000
     - Quality level: 0.001-0.1
     - Min distance: 5-50
     - Block size: 2-7

2. **Visualization Options:**
   - Draw keypoints as circles (size = scale)
   - Color by scale (rainbow gradient)
   - Color by orientation (angle-based hue)
   - Draw orientation arrows
   - Show rich keypoints with size and orientation
   - Adjust marker size multiplier

3. **Feature Analysis:**
   - Total keypoints detected
   - Average response strength
   - Scale distribution histogram
   - Orientation distribution (rose diagram)
   - Spatial distribution heatmap

4. **Export Options:**
   - Save keypoints to XML/JSON
   - Save descriptors to binary file
   - Export annotated image
   - Export feature statistics

**UI Design:**
```cpp
// Feature visualization
void drawKeypoints(const cv::Mat& image, const std::vector<cv::KeyPoint>& kpts,
                   cv::Mat& output, VisualizationMode mode) {
    output = image.clone();
    
    if (mode == ColorByScale) {
        for (const auto& kp : kpts) {
            // Map scale to hue (0-180 for HSV)
            float hue = std::min(180.0f, kp.size * 3.0f);
            cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255, 255));
            cv::Mat bgr;
            cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
            cv::Scalar color(bgr.at<cv::Vec3b>(0, 0));
            
            cv::circle(output, kp.pt, kp.size * sizeMultiplier, color, 2);
            
            if (showOrientation && kp.angle >= 0) {
                cv::Point2f dir(cos(kp.angle * CV_PI / 180.0f),
                               sin(kp.angle * CV_PI / 180.0f));
                cv::Point2f end = kp.pt + dir * kp.size * sizeMultiplier;
                cv::line(output, kp.pt, end, color, 2);
            }
        }
    } else if (mode == ColorByOrientation) {
        for (const auto& kp : kpts) {
            if (kp.angle >= 0) {
                float hue = kp.angle / 2.0f; // Map 0-360 to 0-180
                cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255, 255));
                cv::Mat bgr;
                cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
                cv::Scalar color(bgr.at<cv::Vec3b>(0, 0));
                cv::circle(output, kp.pt, kp.size * sizeMultiplier, color, 2);
            }
        }
    } else {
        cv::drawKeypoints(image, kpts, output, cv::Scalar(0, 255, 0),
                         cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    }
}
```

**Integration Points:**
- **Menu Location:** Segmentation > Feature Detection...
- **Layer Recording:** Creates layer named "SIFT Features (342 keypoints)"
- **Matching:** Can match features between two images
- **Export:** Save features for later matching

---

### 6. Advanced Frequency Filter Dialog (Phase 19)

**Feature Overview:**
Apply frequency-domain filters with multiple filter types and custom parameters.

**Location:** `include/FrequencyFilterDialog.h`, `src/FrequencyFilterDialog.cpp`

**Key Components:**
```cpp
class FrequencyFilterDialog : public QDialog {
    Q_OBJECT
public:
    enum FilterType {
        IdealLowPass,         // Ideal low-pass filter
        IdealHighPass,        // Ideal high-pass filter
        IdealBandPass,        // Ideal band-pass filter
        IdealBandReject,      // Ideal band-reject (notch) filter
        ButterworthLowPass,   // Butterworth low-pass
        ButterworthHighPass,  // Butterworth high-pass
        ButterworthBandPass,  // Butterworth band-pass
        ButterworthBandReject,// Butterworth band-reject
        GaussianLowPass,      // Gaussian low-pass
        GaussianHighPass,     // Gaussian high-pass
        GaussianBandPass,     // Gaussian band-pass
        GaussianBandReject,   // Gaussian band-reject
        HomomorphicFilter     // Homomorphic filtering
    };
    
    FrequencyFilterDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getFilteredImage() const { return filteredImage; }
    QString getFilterType() const;
    bool wasApplied() const { return applied; }
};
```

**Features:**
1. **Thirteen Filter Types:**
   
   **Low-Pass Filters (Smoothing):**
   - **Ideal Low-Pass:** Sharp cutoff, ringing artifacts
   - **Butterworth Low-Pass:** Smooth transition, order 1-10
   - **Gaussian Low-Pass:** Smooth, no ringing, optimal smoothing
   
   **High-Pass Filters (Edge Enhancement):**
   - **Ideal High-Pass:** Sharp cutoff
   - **Butterworth High-Pass:** Smooth rolloff
   - **Gaussian High-Pass:** Optimal edge preservation
   
   **Band-Pass Filters:**
   - **Ideal/Butterworth/Gaussian Band-Pass:** Pass frequencies in range
   - Inner radius: 10-200 pixels
   - Outer radius: 20-300 pixels
   - Width control for frequency selection
   
   **Band-Reject (Notch) Filters:**
   - **Ideal/Butterworth/Gaussian Band-Reject:** Remove specific frequencies
   - Used for removing periodic noise
   - Manual notch placement mode
   
   **Homomorphic Filter:**
   - Simultaneous illumination normalization and contrast enhancement
   - γL (low-frequency gain): 0.1-1.0
   - γH (high-frequency gain): 1.0-3.0
   - Cutoff frequency: 10-100
   - Enhances details in shadows

2. **Interactive Controls:**
   - **Cutoff Frequency (D0):** 10-300 pixels radius
   - **Filter Order (n):** 1-10 (Butterworth only)
   - **Band Width:** 10-200 pixels
   - **Custom Notch Placement:** Click on spectrum to add notch filters
   - **Homomorphic Parameters:** γL, γH, c, D0

3. **Spectrum Visualization:**
   - Shows magnitude spectrum with log scale
   - Displays filter response as overlay
   - Interactive frequency selection
   - Before/after spectrum comparison
   - Phase spectrum display option

4. **Real-Time Preview:**
   - Split view: Original | Filtered
   - Spectrum overlay showing filter shape
   - Histogram comparison
   - Processing time display

**Filter Implementation:**
```cpp
void applyFrequencyFilter(const cv::Mat& input, cv::Mat& output,
                          FilterType type, double D0, int order, 
                          double innerRadius, double outerRadius) {
    // 1. Convert to grayscale if needed
    cv::Mat gray;
    if (input.channels() == 3)
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    else
        gray = input.clone();
    
    // 2. Expand to optimal DFT size
    int M = cv::getOptimalDFTSize(gray.rows);
    int N = cv::getOptimalDFTSize(gray.cols);
    cv::Mat padded;
    cv::copyMakeBorder(gray, padded, 0, M - gray.rows, 0, N - gray.cols,
                       cv::BORDER_CONSTANT, cv::Scalar::all(0));
    
    // 3. Convert to float and normalize
    padded.convertTo(padded, CV_32F);
    
    // 4. Apply DFT
    cv::Mat planes[] = {padded, cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complex;
    cv::merge(planes, 2, complex);
    cv::dft(complex, complex);
    
    // 5. Shift quadrants for centering
    shiftDFT(complex);
    
    // 6. Create filter mask
    cv::Mat filter = createFilterMask(complex.size(), type, D0, order,
                                      innerRadius, outerRadius);
    
    // 7. Apply filter in frequency domain
    cv::Mat filtered;
    cv::multiply(complex, filter, filtered);
    
    // 8. Shift back
    shiftDFT(filtered);
    
    // 9. Inverse DFT
    cv::idft(filtered, filtered);
    
    // 10. Extract real part
    cv::split(filtered, planes);
    cv::normalize(planes[0], output, 0, 255, cv::NORM_MINMAX, CV_8U);
    
    // 11. Crop to original size
    output = output(cv::Rect(0, 0, input.cols, input.rows));
    
    // 12. Convert back to color if needed
    if (input.channels() == 3)
        cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
}

cv::Mat createFilterMask(cv::Size size, FilterType type, double D0, int order,
                         double inner, double outer) {
    cv::Mat filter = cv::Mat::zeros(size, CV_32FC2);
    int cx = size.width / 2;
    int cy = size.height / 2;
    
    for (int y = 0; y < size.height; y++) {
        for (int x = 0; x < size.width; x++) {
            double dist = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            float H = 0.0f;
            
            switch (type) {
                case IdealLowPass:
                    H = (dist <= D0) ? 1.0f : 0.0f;
                    break;
                    
                case IdealHighPass:
                    H = (dist > D0) ? 1.0f : 0.0f;
                    break;
                    
                case ButterworthLowPass:
                    H = 1.0f / (1.0f + pow(dist / D0, 2.0 * order));
                    break;
                    
                case ButterworthHighPass:
                    H = 1.0f / (1.0f + pow(D0 / (dist + 1e-6), 2.0 * order));
                    break;
                    
                case GaussianLowPass:
                    H = exp(-(dist * dist) / (2.0 * D0 * D0));
                    break;
                    
                case GaussianHighPass:
                    H = 1.0f - exp(-(dist * dist) / (2.0 * D0 * D0));
                    break;
                    
                case ButterworthBandPass:
                    if (dist >= inner && dist <= outer)
                        H = 1.0f / (1.0f + pow((dist - (inner + outer) / 2.0) / 
                                    ((outer - inner) / 2.0), 2.0 * order));
                    break;
                    
                case ButterworthBandReject:
                    if (dist < inner || dist > outer)
                        H = 1.0f;
                    else
                        H = 1.0f / (1.0f + pow(((outer - inner) / 2.0) / 
                                    (dist - (inner + outer) / 2.0 + 1e-6), 2.0 * order));
                    break;
            }
            
            filter.at<cv::Vec2f>(y, x) = cv::Vec2f(H, H);
        }
    }
    
    return filter;
}
```

**Integration Points:**
- **Menu Location:** FFT > Advanced Frequency Filters...
- **Layer Recording:** Creates layer named "Butterworth LP (D0=50, n=2)"
- **Spectrum Export:** Save magnitude/phase spectrums
- **Filter Design:** Export filter mask for reuse

---

### 7. Wavelet Transform Dialog (Phase 20)

**Feature Overview:**
Discrete wavelet transform with multiple wavelet families and decomposition levels.

**Location:** `include/WaveletDialog.h`, `src/WaveletDialog.cpp`

**Key Components:**
```cpp
class WaveletDialog : public QDialog {
    Q_OBJECT
public:
    enum WaveletFamily {
        Haar,           // Haar wavelet (simplest)
        Daubechies4,    // Daubechies 4 (db4)
        Daubechies6,    // Daubechies 6 (db6)
        Daubechies8,    // Daubechies 8 (db8)
        Symlet4,        // Symlet 4 (sym4)
        Symlet6,        // Symlet 6 (sym6)
        Coiflet2,       // Coiflet 2 (coif2)
        Biorthogonal,   // Biorthogonal 2.2
        ReverseBior     // Reverse biorthogonal 2.2
    };
    
    enum Operation {
        Decompose,      // Forward DWT
        Reconstruct,    // Inverse DWT
        Denoise,        // Wavelet denoising
        Compress,       // Wavelet compression
        EdgeDetect      // Wavelet-based edge detection
    };
    
    WaveletDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    WaveletFamily getWavelet() const { return currentWavelet; }
    int getDecompositionLevel() const { return level; }
};
```

**Features:**
1. **Nine Wavelet Families:**
   - **Haar:** Simplest, discontinuous, good for edges
   - **Daubechies (db4, db6, db8):** Smooth, more coefficients
   - **Symlets (sym4, sym6):** Nearly symmetric, good for denoising
   - **Coiflets (coif2):** Symmetric, compact support
   - **Biorthogonal:** Perfect reconstruction, linear phase

2. **Five Operations:**
   
   **Decompose (Forward DWT):**
   - Levels: 1-5
   - Displays: LL (approximation), LH (horizontal), HL (vertical), HH (diagonal)
   - Visualization: 4-quadrant layout showing all subbands
   - Coefficient statistics: min, max, mean, std dev
   
   **Reconstruct (Inverse DWT):**
   - Reconstructs image from decomposition
   - Can zero out specific subbands
   - Compare original vs reconstructed
   - Displays PSNR reconstruction quality
   
   **Denoise (Wavelet Shrinkage):**
   - Threshold type: Hard, Soft, Garrote
   - Threshold estimation: Universal, VisuShrink, BayesShrink
   - Sigma estimation: Median Absolute Deviation
   - PSNR improvement display
   
   **Compress (Zero Small Coefficients):**
   - Compression ratio: 10:1 to 100:1
   - Keep largest N coefficients
   - Display: Original size vs compressed size
   - Quality metrics: PSNR, SSIM
   
   **Edge Detect (Gradient from HL + LH):**
   - Combine horizontal and vertical details
   - Multiscale edge detection
   - Adjustable edge strength multiplier

3. **Interactive Visualization:**
   - 4-panel view: LL, LH, HL, HH subbands
   - Logarithmic scale for better visualization
   - Colormap options: Grayscale, Jet, Hot, Cool
   - Histogram of coefficients
   - Coefficient browser (zoom into specific subband)

4. **Advanced Controls:**
   - **Extension Mode:** Symmetric, Periodic, Zero-padding
   - **Coefficient Manipulation:** Multiply/threshold subbands
   - **Multi-level Navigation:** Browse decomposition tree
   - **Export Options:** Save subbands separately

**Implementation:**
```cpp
// Using OpenCV with custom wavelet implementation
void applyDWT(const cv::Mat& input, cv::Mat& LL, cv::Mat& LH,
              cv::Mat& HL, cv::Mat& HH, WaveletFamily wavelet) {
    // Get wavelet filter coefficients
    std::vector<float> loD, hiD, loR, hiR;
    getWaveletFilters(wavelet, loD, hiD, loR, hiR);
    
    // Convert to grayscale float
    cv::Mat gray;
    if (input.channels() == 3)
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    else
        gray = input.clone();
    gray.convertTo(gray, CV_32F);
    
    // Apply row filters
    cv::Mat rowFiltered1, rowFiltered2;
    applyFilter1D(gray, rowFiltered1, loD, true);  // Low-pass rows
    applyFilter1D(gray, rowFiltered2, hiD, true);  // High-pass rows
    
    // Downsample rows
    downsampleRows(rowFiltered1, rowFiltered1);
    downsampleRows(rowFiltered2, rowFiltered2);
    
    // Apply column filters
    applyFilter1D(rowFiltered1, LL, loD, false);  // LL: Low-Low
    applyFilter1D(rowFiltered1, LH, hiD, false);  // LH: Low-High
    applyFilter1D(rowFiltered2, HL, loD, false);  // HL: High-Low
    applyFilter1D(rowFiltered2, HH, hiD, false);  // HH: High-High
    
    // Downsample columns
    downsampleCols(LL, LL);
    downsampleCols(LH, LH);
    downsampleCols(HL, HL);
    downsampleCols(HH, HH);
}

void waveletDenoise(const cv::Mat& input, cv::Mat& output,
                    WaveletFamily wavelet, int levels, 
                    ThresholdType type, float sigma) {
    std::vector<cv::Mat> subbands;
    
    // Multi-level decomposition
    cv::Mat current = input.clone();
    for (int level = 0; level < levels; level++) {
        cv::Mat LL, LH, HL, HH;
        applyDWT(current, LL, LH, HL, HH, wavelet);
        
        // Apply thresholding to detail subbands
        float threshold = sigma * sqrt(2.0f * log(LH.rows * LH.cols));
        applyThreshold(LH, LH, threshold, type);
        applyThreshold(HL, HL, threshold, type);
        applyThreshold(HH, HH, threshold, type);
        
        subbands.push_back(LH);
        subbands.push_back(HL);
        subbands.push_back(HH);
        current = LL;
    }
    
    // Reconstruction
    output = current.clone();
    for (int level = levels - 1; level >= 0; level--) {
        cv::Mat LH = subbands[level * 3];
        cv::Mat HL = subbands[level * 3 + 1];
        cv::Mat HH = subbands[level * 3 + 2];
        applyIDWT(output, LH, HL, HH, output, wavelet);
    }
}
```

**UI Design:**
```cpp
// 4-panel subband viewer
QSplitter* splitter = new QSplitter(Qt::Horizontal);
QSplitter* leftSplit = new QSplitter(Qt::Vertical);
QSplitter* rightSplit = new QSplitter(Qt::Vertical);

QLabel* llLabel = new QLabel();
QLabel* lhLabel = new QLabel();
QLabel* hlLabel = new QLabel();
QLabel* hhLabel = new QLabel();

llLabel->setPixmap(QPixmap::fromImage(convertMatToQImage(LL)));
lhLabel->setPixmap(QPixmap::fromImage(convertMatToQImage(LH)));
hlLabel->setPixmap(QPixmap::fromImage(convertMatToQImage(HL)));
hhLabel->setPixmap(QPixmap::fromImage(convertMatToQImage(HH)));

leftSplit->addWidget(llLabel);
leftSplit->addWidget(lhLabel);
rightSplit->addWidget(hlLabel);
rightSplit->addWidget(hhLabel);

splitter->addWidget(leftSplit);
splitter->addWidget(rightSplit);
```

**Integration Points:**
- **Menu Location:** Wavelet > Wavelet Transform...
- **Layer Recording:** Creates layer named "Wavelet Denoised (Haar, L=3)"
- **Coefficient Export:** Save subband images
- **Compression:** Can save compressed representation

---

### 8. Intensity Transform Dialog (Phase 21)

**Feature Overview:**
Apply gamma correction, logarithmic, and inverse logarithmic transformations with real-time preview.

**Location:** `include/IntensityTransformDialog.h`, `src/IntensityTransformDialog.cpp`

**Key Components:**
```cpp
class IntensityTransformDialog : public QDialog {
    Q_OBJECT
public:
    enum TransformType {
        Gamma,              // Gamma correction: s = c * r^γ
        Log,                // Log transform: s = c * log(1 + r)
        InverseLog,         // Inverse log: s = c * exp(r) - 1
        PowerLaw,           // Power-law: s = c * r^n
        PiecewiseLinear,    // Piecewise linear contrast stretch
        ContrastStretch,    // Min-max normalization
        IntensityLevelSlicing, // Highlight specific intensity range
        BitPlaneRemoval     // Remove specific bit planes
    };
    
    IntensityTransformDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    TransformType getTransformType() const { return currentType; }
};
```

**Features:**
1. **Eight Intensity Transformations:**
   
   **Gamma Correction:**
   - Gamma (γ): 0.1-5.0
   - γ < 1: Brighten dark regions (expand shadows)
   - γ = 1: Linear (no change)
   - γ > 1: Darken bright regions (compress highlights)
   - Formula: `s = 255 * (r/255)^γ`
   - Real-time curve display
   
   **Logarithmic Transform:**
   - Constant (c): 1-100
   - Expands dark values, compresses bright values
   - Formula: `s = c * log(1 + r)`
   - Good for Fourier spectrum visualization
   - Enhances details in shadows
   
   **Inverse Logarithmic:**
   - Constant (c): 0.01-1.0
   - Expands bright values, compresses dark values
   - Formula: `s = exp(r/c) - 1`
   - Opposite of log transform
   
   **Power-Law Transform:**
   - Power (n): -2.0 to 5.0
   - Generalization of gamma
   - Negative powers invert intensity
   
   **Piecewise Linear:**
   - Define up to 5 control points
   - Contrast stretching with custom curve
   - Interactive graph editor
   - Common presets: Linear, S-curve, Inverse-S
   
   **Contrast Stretch:**
   - Input range: [rmin, rmax]
   - Output range: [smin, smax]
   - Auto-detect or manual control
   - Percentile clipping: 0-10%
   
   **Intensity Level Slicing:**
   - Highlight intensities in range [A, B]
   - Preserve background: Yes/No
   - Highlight color: Red, Green, Blue, White
   - With/without background

2. **Interactive Controls:**
   - Slider with live preview
   - Transformation curve display
   - Before/after histogram comparison
   - Split view slider
   - Preset buttons (Brighten, Darken, Enhance Shadows, etc.)

3. **Curve Visualization:**
   - Input intensity (x-axis) vs Output intensity (y-axis)
   - 45° line for reference (linear)
   - Actual transformation curve
   - Histogram overlay showing intensity distribution

**Implementation:**
```cpp
void applyGammaCorrection(const cv::Mat& input, cv::Mat& output, double gamma) {
    // Create lookup table for efficiency
    uchar lut[256];
    for (int i = 0; i < 256; i++) {
        lut[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }
    
    // Apply LUT
    cv::Mat lookupTable(1, 256, CV_8U, lut);
    cv::LUT(input, lookupTable, output);
}

void applyLogTransform(const cv::Mat& input, cv::Mat& output, double c) {
    cv::Mat float_img;
    input.convertTo(float_img, CV_32F);
    
    // Apply log transform
    cv::log(float_img + 1.0, output);
    output = c * output;
    
    // Normalize to [0, 255]
    cv::normalize(output, output, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyPiecewiseLinear(const cv::Mat& input, cv::Mat& output,
                          const std::vector<cv::Point2f>& controlPoints) {
    // Sort control points by x
    std::vector<cv::Point2f> sorted = controlPoints;
    std::sort(sorted.begin(), sorted.end(), 
              [](const cv::Point2f& a, const cv::Point2f& b) { return a.x < b.x; });
    
    // Create LUT using linear interpolation
    uchar lut[256];
    for (int i = 0; i < 256; i++) {
        float x = i / 255.0f;
        float y = interpolateControlPoints(x, sorted);
        lut[i] = cv::saturate_cast<uchar>(y * 255.0f);
    }
    
    // Apply LUT
    cv::Mat lookupTable(1, 256, CV_8U, lut);
    cv::LUT(input, lookupTable, output);
}
```

**UI Design:**
```cpp
// Transformation curve widget
class TransformCurveWidget : public QWidget {
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Draw grid
        painter.setPen(QPen(QColor(69, 71, 90), 1));
        for (int i = 0; i <= 10; i++) {
            int x = width() * i / 10;
            int y = height() * i / 10;
            painter.drawLine(x, 0, x, height());
            painter.drawLine(0, y, width(), y);
        }
        
        // Draw 45° reference line
        painter.setPen(QPen(QColor(186, 194, 222), 1, Qt::DashLine));
        painter.drawLine(0, height(), width(), 0);
        
        // Draw transformation curve
        painter.setPen(QPen(QColor(137, 180, 250), 3));
        for (int x = 0; x < width() - 1; x++) {
            float inVal = x / (float)width();
            float outVal = applyTransform(inVal);
            int y1 = height() - (int)(outVal * height());
            
            float inVal2 = (x + 1) / (float)width();
            float outVal2 = applyTransform(inVal2);
            int y2 = height() - (int)(outVal2 * height());
            
            painter.drawLine(x, y1, x + 1, y2);
        }
        
        // Draw histogram overlay (semi-transparent)
        painter.setOpacity(0.3);
        painter.setBrush(QBrush(QColor(166, 227, 161)));
        // ... draw histogram bars
    }
};
```

**Integration Points:**
- **Menu Location:** Process > Intensity Transformations > Gamma & Log Transform...
- **Layer Recording:** Creates layer named "Gamma Correction (γ=0.5)"
- **Presets:** Save custom curves for reuse
- **Batch Processing:** Apply same transform to multiple images

---

### 9. Sharpening Dialog (Phase 21)

**Feature Overview:**
Advanced sharpening with Laplacian, Unsharp Mask, and High-Boost filtering.

**Location:** `include/SharpeningDialog.h`, `src/SharpeningDialog.cpp`

**Key Components:**
```cpp
class SharpeningDialog : public QDialog {
    Q_OBJECT
public:
    enum SharpeningMethod {
        Laplacian4,         // 4-connected Laplacian
        Laplacian8,         // 8-connected Laplacian
        LaplacianOfGaussian, // LoG (smoothed Laplacian)
        UnsharpMask,        // Gaussian blur + subtraction
        HighBoost,          // Amplified unsharp mask
        AdaptiveSharpening, // Edge-aware sharpening
        LucySharpen,        // Lucy-Richardson sharpening
        WienerSharpening    // Wiener filter-based
    };
    
    SharpeningDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getResultImage() const { return resultImage; }
    SharpeningMethod getMethod() const { return currentMethod; }
};
```

**Features:**
1. **Eight Sharpening Methods:**
   
   **Laplacian (4-connected):**
   - Kernel: `[0 -1 0; -1 4 -1; 0 -1 0]`
   - Strength: 0.1-2.0
   - Formula: `sharpened = original - strength * laplacian`
   - Fast, simple, enhances all edges
   
   **Laplacian (8-connected):**
   - Kernel: `[-1 -1 -1; -1 8 -1; -1 -1 -1]`
   - Stronger diagonal edge response
   - More sensitive to noise
   
   **Laplacian of Gaussian (LoG):**
   - Gaussian blur first (σ = 0.5-5.0)
   - Then apply Laplacian
   - Reduces noise sensitivity
   - Kernel size: 3-15 (odd)
   
   **Unsharp Mask:**
   - Blur radius: 0.5-10.0 pixels
   - Amount: 0.1-3.0
   - Threshold: 0-50 (avoid sharpening flat areas)
   - Formula: `sharpened = original + amount * (original - blurred)`
   - Industry standard for photo sharpening
   
   **High-Boost Filtering:**
   - Amplification factor (A): 1.0-3.0
   - Formula: `sharpened = A * original - blurred`
   - More aggressive than unsharp mask
   - Good for heavily blurred images
   
   **Adaptive Sharpening:**
   - Edge detection first
   - Sharpen only near edges
   - Preserve smooth regions
   - Edge threshold: 10-100
   - Reduces noise amplification
   
   **Lucy-Richardson:**
   - Deconvolution-based sharpening
   - Iterations: 1-20
   - PSF (Point Spread Function) size: 3-15
   - Sigma: 0.5-3.0
   - Best for focus correction
   
   **Wiener Sharpening:**
   - Optimal inverse filtering
   - Noise-to-signal ratio: 0.001-0.1
   - PSF size: 3-15
   - Balances sharpening and noise

2. **Interactive Controls:**
   - Method selection dropdown
   - Strength/amount slider (0-300%)
   - Radius/kernel size slider
   - Threshold slider (for adaptive methods)
   - Edge preservation toggle
   - Real-time split-view preview

3. **Advanced Options:**
   - **Channel Selection:** RGB, Luminance only, Each channel separately
   - **Edge Mask:** Show where sharpening is applied
   - **Before/After Comparison:** Side-by-side or split slider
   - **Oversharpening Warning:** Highlight clipped pixels
   - **Noise Reduction:** Pre-blur before sharpening

**Implementation:**
```cpp
void applyUnsharpMask(const cv::Mat& input, cv::Mat& output,
                      double sigma, double amount, double threshold) {
    // 1. Create blurred version
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(0, 0), sigma);
    
    // 2. Calculate difference (mask)
    cv::Mat mask;
    cv::subtract(input, blurred, mask);
    
    // 3. Apply threshold to mask (avoid sharpening noise)
    if (threshold > 0) {
        for (int y = 0; y < mask.rows; y++) {
            for (int x = 0; x < mask.cols; x++) {
                if (input.channels() == 1) {
                    int diff = abs(mask.at<uchar>(y, x));
                    if (diff < threshold)
                        mask.at<uchar>(y, x) = 0;
                } else {
                    cv::Vec3b diff = mask.at<cv::Vec3b>(y, x);
                    int maxDiff = std::max({abs(diff[0]), abs(diff[1]), abs(diff[2])});
                    if (maxDiff < threshold)
                        mask.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
                }
            }
        }
    }
    
    // 4. Add weighted mask to original
    cv::addWeighted(input, 1.0, mask, amount, 0, output);
}

void applyHighBoostFilter(const cv::Mat& input, cv::Mat& output,
                          double sigma, double A) {
    // 1. Create blurred version
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(0, 0), sigma);
    
    // 2. Apply high-boost formula: A*original - blurred
    cv::addWeighted(input, A, blurred, -1.0, 0, output);
}

void applyAdaptiveSharpening(const cv::Mat& input, cv::Mat& output,
                             double amount, int edgeThreshold) {
    // 1. Detect edges
    cv::Mat gray, edges;
    if (input.channels() == 3)
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    else
        gray = input.clone();
    
    cv::Canny(gray, edges, edgeThreshold / 2, edgeThreshold);
    
    // 2. Dilate edge mask to include nearby pixels
    cv::dilate(edges, edges, cv::Mat(), cv::Point(-1, -1), 2);
    
    // 3. Create sharpened version (unsharp mask)
    cv::Mat sharpened;
    applyUnsharpMask(input, sharpened, 1.0, amount, 0);
    
    // 4. Blend based on edge mask
    output = input.clone();
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            if (edges.at<uchar>(y, x) > 0) {
                if (input.channels() == 1) {
                    output.at<uchar>(y, x) = sharpened.at<uchar>(y, x);
                } else {
                    output.at<cv::Vec3b>(y, x) = sharpened.at<cv::Vec3b>(y, x);
                }
            }
        }
    }
}
```

**UI Design:**
```cpp
// Split-view preview widget
class SplitPreviewWidget : public QWidget {
    Q_OBJECT
public:
    void setSplitPosition(int x) {
        splitX = x;
        update();
    }
    
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        
        // Draw original on left
        QRect leftRect(0, 0, splitX, height());
        painter.drawImage(leftRect, originalImage, 
                         QRect(0, 0, splitX * scale, height() * scale));
        
        // Draw sharpened on right
        QRect rightRect(splitX, 0, width() - splitX, height());
        painter.drawImage(rightRect, sharpenedImage,
                         QRect(splitX * scale, 0, (width() - splitX) * scale, 
                              height() * scale));
        
        // Draw split line
        painter.setPen(QPen(QColor(137, 180, 250), 3));
        painter.drawLine(splitX, 0, splitX, height());
        
        // Draw labels
        painter.setPen(QPen(QColor(205, 214, 244)));
        painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
        painter.drawText(10, 30, "Original");
        painter.drawText(width() - 90, 30, "Sharpened");
    }
    
    void mouseMoveEvent(QMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton) {
            splitX = event->x();
            update();
        }
    }
    
private:
    int splitX;
    QImage originalImage, sharpenedImage;
    double scale;
};
```

**Integration Points:**
- **Menu Location:** Process > Sharpening Filters > Laplacian/Unsharp/High-Boost...
- **Layer Recording:** Creates layer named "Unsharp Mask (r=2.0, amt=1.5)"
- **Presets:** Soft, Medium, Strong, Portrait, Landscape
- **Oversharpening Detection:** Warn if >5% pixels clipped

---

### 10. Huffman Coding Dialog (Phase 22)

**Feature Overview:**
Lossless Huffman compression with visual tree display and encoding statistics.

**Location:** `include/HuffmanDialog.h`, `src/HuffmanDialog.cpp`

**Key Components:**
```cpp
class HuffmanDialog : public QDialog {
    Q_OBJECT
public:
    enum Operation {
        Compress,       // Encode image with Huffman
        Decompress,     // Decode Huffman-encoded data
        Analyze,        // Show frequency analysis only
        CompareMethods  // Compare with other compression
    };
    
    HuffmanDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getDecompressedImage() const { return decompressed; }
    QString getEncodedData() const { return huffmanCode; }
    double getCompressionRatio() const { return compressionRatio; }
};
```

**Features:**
1. **Huffman Compression:**
   - Frequency analysis of pixel values (0-255)
   - Build Huffman tree based on frequencies
   - Generate variable-length codes
   - Encode image data
   - Calculate compression ratio
   - Display code table

2. **Visual Tree Display:**
   - Interactive Huffman tree visualization
   - Color-coded by frequency (hot = common, cold = rare)
   - Show path for specific symbols
   - Zoom and pan controls
   - Export tree as PNG

3. **Statistics Display:**
   - **Original Size:** Width × Height × Channels × 8 bits
   - **Compressed Size:** Total bits in Huffman encoding
   - **Compression Ratio:** Original / Compressed
   - **Space Savings:** (1 - Compressed/Original) × 100%
   - **Entropy:** Theoretical minimum bits per symbol
   - **Efficiency:** Actual bits / Entropy
   - **Most Frequent Symbol:** Value and count
   - **Average Code Length:** Weighted average

4. **Encoding Table:**
   - Scrollable table: Symbol | Frequency | Probability | Code | Length
   - Sortable by any column
   - Search for specific symbol
   - Export table as CSV

5. **Comparison Mode:**
   - Compare Huffman with:
     - Run-Length Encoding (RLE)
     - JPEG compression
     - PNG compression
     - Arithmetic coding (theoretical)
   - Side-by-side compression ratios
   - Quality comparison (for lossy methods)

**Implementation:**
```cpp
struct HuffmanNode {
    int symbol;
    int frequency;
    HuffmanNode *left, *right;
    
    HuffmanNode(int sym, int freq) : symbol(sym), frequency(freq), 
                                     left(nullptr), right(nullptr) {}
};

class HuffmanTree {
public:
    void buildTree(const std::map<int, int>& frequencies) {
        // Priority queue (min-heap) based on frequency
        auto cmp = [](HuffmanNode* a, HuffmanNode* b) { 
            return a->frequency > b->frequency; 
        };
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pq(cmp);
        
        // Create leaf nodes for each symbol
        for (const auto& pair : frequencies) {
            pq.push(new HuffmanNode(pair.first, pair.second));
        }
        
        // Build tree bottom-up
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            
            HuffmanNode* parent = new HuffmanNode(-1, left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }
        
        root = pq.top();
    }
    
    void generateCodes(HuffmanNode* node, std::string code,
                       std::map<int, std::string>& codeTable) {
        if (!node) return;
        
        // Leaf node = symbol
        if (!node->left && !node->right) {
            codeTable[node->symbol] = code.empty() ? "0" : code;
            return;
        }
        
        generateCodes(node->left, code + "0", codeTable);
        generateCodes(node->right, code + "1", codeTable);
    }
    
    std::string encode(const cv::Mat& image, const std::map<int, std::string>& codeTable) {
        std::string encoded;
        encoded.reserve(image.rows * image.cols * 8);
        
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    int symbol = image.at<cv::Vec3b>(y, x)[c];
                    encoded += codeTable.at(symbol);
                }
            }
        }
        
        return encoded;
    }
    
    cv::Mat decode(const std::string& encoded, cv::Size size, int channels) {
        cv::Mat decoded(size, CV_8UC(channels));
        HuffmanNode* current = root;
        int pixelIndex = 0;
        
        for (char bit : encoded) {
            current = (bit == '0') ? current->left : current->right;
            
            if (!current->left && !current->right) {
                // Leaf node reached
                int y = pixelIndex / (size.width * channels);
                int x = (pixelIndex % (size.width * channels)) / channels;
                int c = pixelIndex % channels;
                
                decoded.at<cv::Vec3b>(y, x)[c] = current->symbol;
                
                current = root;
                pixelIndex++;
            }
        }
        
        return decoded;
    }
    
private:
    HuffmanNode* root = nullptr;
};
```

**UI Design:**
```cpp
// Tree visualization widget
class HuffmanTreeWidget : public QWidget {
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        if (root) {
            drawNode(&painter, root, width() / 2, 30, width() / 4, 0);
        }
    }
    
    void drawNode(QPainter* painter, HuffmanNode* node, int x, int y,
                  int xOffset, int level) {
        if (!node) return;
        
        // Draw connections to children
        if (node->left) {
            painter->setPen(QPen(QColor(137, 180, 250), 2));
            painter->drawLine(x, y, x - xOffset, y + 60);
            painter->drawText(x - xOffset + 10, y + 30, "0");
            drawNode(painter, node->left, x - xOffset, y + 60, xOffset / 2, level + 1);
        }
        if (node->right) {
            painter->setPen(QPen(QColor(245, 194, 231), 2));
            painter->drawLine(x, y, x + xOffset, y + 60);
            painter->drawText(x + xOffset - 20, y + 30, "1");
            drawNode(painter, node->right, x + xOffset, y + 60, xOffset / 2, level + 1);
        }
        
        // Draw node circle
        QColor nodeColor = node->left ? QColor(42, 42, 62) : QColor(137, 180, 250);
        painter->setBrush(QBrush(nodeColor));
        painter->setPen(QPen(QColor(69, 71, 90), 2));
        painter->drawEllipse(QPoint(x, y), 20, 20);
        
        // Draw label
        painter->setPen(QColor(205, 214, 244));
        if (node->symbol >= 0) {
            painter->drawText(x - 10, y + 5, QString::number(node->symbol));
        } else {
            painter->drawText(x - 15, y + 5, QString::number(node->frequency));
        }
    }
};
```

**Integration Points:**
- **Menu Location:** Process > Huffman Coding...
- **File Save:** Export compressed data to .huf file
- **File Load:** Load and decompress .huf file
- **Statistics Export:** Save compression stats as CSV

---

### Implementation Checklist for New Features

**BlurDialog Integration:**
- [ ] Dialog styled with dark theme (#1E1E2E background)
- [ ] Sliders have blue accent handles (#89B4FA)
- [ ] Preview shows scaled image with proper aspect ratio
- [ ] Gaussian blur kernel size enforces odd numbers
- [ ] Bilateral sigma sliders update preview in real-time
- [ ] OK/Cancel buttons styled as primary/secondary
- [ ] Dialog remembers last-used settings
- [ ] Selection mask properly applied to blur result
- [ ] Layer system records blur type and parameters

**ResolutionEnhancementDialog Integration:**
- [ ] Dialog styled with dark theme
- [ ] Scale slider shows percentage (100-400%)
- [ ] Current/Target size display updates dynamically
- [ ] Method combo box has descriptions in tooltips
- [ ] Sharpen slider hidden for Nearest method
- [ ] Warning dialog appears for >100MP outputs
- [ ] Edge-Directed method successfully runs cv::edgePreservingFilter
- [ ] Selection mask properly upscaled with INTER_NEAREST
- [ ] Preview shows downscaled version for performance
- [ ] Layer system records scale factor and method name

---

## 📞 Support & Resources

### Component Documentation Paths
- **Qt 6.7 Docs:** `C:\Qt\Docs\Qt-6.7.3\`
- **OpenCV Docs:** `F:\OpenCV\opencv\sources\doc\`
- **Project Docs:** `F:\Naghuma Toolbox\documentation\guides\`

### Build Tools Paths
- **MSBuild:** `F:\Build Tools\MSBuild\Current\Bin\MSBuild.exe`
- **MSVC:** `F:\Build Tools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe`
- **Qt MOC:** `C:\Qt\6.7.3\msvc2019_64\bin\moc.exe`

### Library Paths
- **Qt Libs:** `C:\Qt\6.7.3\msvc2019_64\lib\`
- **OpenCV Libs:** `F:\OpenCV\opencv\build\x64\vc15\lib\`
- **Tesseract Libs:** `C:\Program Files\Tesseract-OCR\`

---

**🎉 Ready to create your customized Naghuma Toolbox with stunning dark theme and modern UX! 🎉**
