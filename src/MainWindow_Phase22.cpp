#include "MainWindow.h"
// UI Component includes - MUST come first for complete type definitions
#include "ImageCanvas.h"
#include "RightSidebarWidget.h"

// Phase 22 Dialog includes
#include "HuffmanDialog.h"

// ============================================================================
// PHASE 22: HUFFMAN CODING
// ============================================================================

void MainWindow::showHuffmanDialog() {
    if (!checkImageLoaded("apply Huffman coding")) return;
    
    HuffmanDialog dialog(currentImage, this);
    
    // Connect preview signal
    connect(&dialog, &HuffmanDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        // Get the decoded image (verifies lossless compression)
        processedImage = dialog.getDecodedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        const auto& result = dialog.getResult();
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            
            QString layerName = QString("Huffman Coding (Ratio: %1:1, Eff: %2%)")
                .arg(result.compressionRatio, 0, 'f', 2)
                .arg(result.efficiency * 100, 0, 'f', 1);
            
            rightSidebar->addLayer(
                layerName,
                "compression",
                processedImage,
                nullptr  // Operation replay not practical for Huffman
            );
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();
        }
        
        QString statusMsg = QString("Huffman coding: %1:1 compression, %2% efficiency, LOSSLESS!")
            .arg(result.compressionRatio, 0, 'f', 2)
            .arg(result.efficiency * 100, 0, 'f', 1);
        
        updateStatus(statusMsg, "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}
