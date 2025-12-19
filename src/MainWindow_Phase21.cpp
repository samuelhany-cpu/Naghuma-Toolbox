#include "MainWindow.h"
#include "IntensityTransformDialog.h"
#include "SharpeningDialog.h"
#include "ImageProcessor.h"

// ============================================================================
// PHASE 21: INTENSITY TRANSFORMATIONS & EDGE ENHANCEMENT
// ============================================================================

void MainWindow::showIntensityTransformDialog() {
    if (!checkImageLoaded("apply intensity transformation")) return;
    
    IntensityTransformDialog dialog(currentImage, this);
    
    // Connect preview signal
    connect(&dialog, &IntensityTransformDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        QString operationType = dialog.getOperationType();
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(
                operationType,
                "intensity",
                processedImage,
                nullptr  // Operation replay could be added
            );
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();
        }
        
        updateStatus(QString("%1 applied successfully!").arg(operationType), "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}

void MainWindow::showSharpeningDialog() {
    if (!checkImageLoaded("apply sharpening filter")) return;
    
    SharpeningDialog dialog(currentImage, this);
    
    // Connect preview signal
    connect(&dialog, &SharpeningDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getSharpenedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        QString operationType = dialog.getOperationType();
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(
                operationType,
                "sharpening",
                processedImage,
                nullptr  // Operation replay could be added
            );
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();
        }
        
        updateStatus(QString("%1 applied successfully!").arg(operationType), "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}
