// ============================================================================
// MAINWINDOW - PHASE 23: OCR (OPTICAL CHARACTER RECOGNITION)
// ============================================================================
// Purpose: Extract text from images using OCR technology
// Features: Multi-language support, preprocessing, export to text
// ============================================================================

#include "MainWindow.h"
#include "ImageCanvas.h"
#include "RightSidebarWidget.h"
#include "OCRDialog.h"
#include <QMessageBox>

/**
 * @brief Show OCR Dialog for text recognition
 * 
 * Opens dialog to extract text from current image using OCR.
 * Supports multiple languages and preprocessing options.
 */
void MainWindow::showOCRDialog() {
    if (!checkImageLoaded("perform OCR")) return;
    
    try {
        OCRDialog dialog(currentImage, this);
        
        if (dialog.exec() == QDialog::Accepted) {
            QString recognizedText = dialog.getRecognizedText();
            float confidence = dialog.getConfidence();
            
            if (!recognizedText.isEmpty() && confidence > 0) {
                updateStatus(QString("OCR completed: Confidence %1%")
                    .arg(confidence, 0, 'f', 1), "success");
            }
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "OCR Error",
            QString("Failed to perform OCR:\n%1").arg(e.what()));
        updateStatus("OCR failed", "error");
    }
}
