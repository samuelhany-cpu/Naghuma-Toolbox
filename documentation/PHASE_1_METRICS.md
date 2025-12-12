# Phase 1 Implementation: Image Metrics (RMSE, SNR, PSNR)

## Files Created:
1. ? include/ImageMetrics.h - Header with metric functions
2. ? src/ImageMetrics.cpp - Implementation with formulas from image

## Next Steps to Complete Phase 1:

### 1. Add to MainWindow.h
Add this to private slots section:
```cpp
void showImageMetrics();
```

### 2. Add to MainWindow.cpp createMenuBar()
After Statistics menu item, add:
```cpp
infoMenu->addSeparator();
QAction *metricsAction = infoMenu->addAction("Image Metrics (RMSE/SNR/PSNR)");
connect(metricsAction, &QAction::triggered, this, &MainWindow::showImageMetrics);
```

### 3. Add to MainWindow.cpp (after showImageStats())
```cpp
void MainWindow::showImageMetrics() {
    if (!imageLoaded) {
        QMessageBox::information(this, "Info", "No image loaded!");
        return;
    }
    
    if (!recentlyProcessed || processedImage.empty()) {
        QMessageBox::warning(this, "Warning", 
            "No processed image to compare!\n\nApply a filter or transformation first.");
        return;
    }
    
    // Calculate metrics
    auto result = ImageMetrics::calculateMetrics(originalImage, processedImage);
    
    if (!result.isValid) {
        QMessageBox::critical(this, "Error", result.errorMessage);
        return;
    }
    
    // Create dialog
    QDialog *metricsDialog = new QDialog(this);
    metricsDialog->setWindowTitle("Image Quality Metrics");
    metricsDialog->setMinimumSize(500, 350);
    
    QVBoxLayout *layout = new QVBoxLayout(metricsDialog);
    
    QLabel *titleLabel = new QLabel("Quality Comparison: Original vs Processed");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    layout->addWidget(titleLabel);
    
    // Metrics display
    QTextEdit *metricsText = new QTextEdit();
    metricsText->setReadOnly(true);
    metricsText->setStyleSheet(
        "background-color: rgba(45, 37, 71, 0.5); "
        "color: #f3e8ff; "
        "border: 2px solid rgba(91, 75, 115, 0.5); "
        "border-radius: 10px; "
        "padding: 20px; "
        "font-family: 'Consolas', monospace; "
        "font-size: 12pt;"
    );
    
    QString metricsInfo = ImageMetrics::formatMetrics(result);
    metricsInfo += "\n\n";
    metricsInfo += "???????????????????????????????????\n";
    metricsInfo += "Interpretation:\n";
    metricsInfo += "???????????????????????????????????\n\n";
    metricsInfo += "• MSE/RMSE: Lower is better (0 = identical)\n";
    metricsInfo += "• SNR: Higher is better (signal vs noise)\n";
    metricsInfo += "• PSNR: Higher is better\n";
    metricsInfo += "  - 30-50 dB: Good quality\n";
    metricsInfo += "  - 20-30 dB: Acceptable\n";
    metricsInfo += "  - <20 dB: Poor quality";
    
    metricsText->setText(metricsInfo);
    layout->addWidget(metricsText);
    
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    closeBtn->setMaximumWidth(150);
    connect(closeBtn, &QPushButton::clicked, metricsDialog, &QDialog::accept);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);
    
    metricsDialog->exec();
}
```

### 4. Add include to MainWindow.cpp
At the top with other includes:
```cpp
#include "ImageMetrics.h"
```

### 5. Update project file
Add to Naghuma Toolbox.vcxproj:
```xml
<ClInclude Include="include\ImageMetrics.h" />
<ClCompile Include="src\ImageMetrics.cpp" />
```

### 6. Build and Test
```powershell
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build
.\scripts\run_app.ps1
```

## Testing Checklist:
- [ ] Load an image
- [ ] Apply a filter (e.g., Gaussian Blur)
- [ ] Go to Information > Image Metrics
- [ ] Verify MSE, RMSE, SNR, PSNR calculations
- [ ] Test with different filters
- [ ] Test error handling (no image, no processed image)

## Formula Reference:
All formulas implemented according to the provided image:
- MSE = (1/MN) ?[f(x,y) - f?(x,y)]²
- RMSE = ?MSE
- SNR = 10*log??(Signal Power / Noise Power)
- PSNR = 10*log??(L²/MSE) where L=255 for 8-bit images
