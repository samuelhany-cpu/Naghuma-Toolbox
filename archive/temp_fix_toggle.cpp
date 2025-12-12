void MainWindow::toggleDrawingMode() {
    if (!imageLoaded) {
        QMessageBox::warning(this, "Warning", "Please load an image first!");
        return;
    }
    
    if (currentImage.empty()) {
        QMessageBox::warning(this, "Warning", "Current image is not available!");
        return;
    }
    
    drawingMode = !drawingMode;
    
    // Update button text
    QPushButton *btn = findChild<QPushButton*>("drawModeButton");
    if (btn) {
        if (drawingMode) {
            btn->setText("Drawing Mode: ON");
            btn->setProperty("class", "accent");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        } else {
            btn->setText("Drawing Mode: OFF");
            btn->setProperty("class", "");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        }
    }
    
    if (drawingMode) {
        // Start with current image as drawing canvas
        try {
            drawingCanvas = currentImage.clone();
            
            if (drawingCanvas.empty()) {
                QMessageBox::critical(this, "Error", "Failed to create drawing canvas!");
                drawingMode = false;
                processedCanvas->setMouseEventsEnabled(false);
                if (btn) btn->setText("Drawing Mode: OFF");
                return;
            }
            
            // CRITICAL FIX: Set the image in canvas FIRST, then enable mouse events
            processedImage = drawingCanvas.clone();
            recentlyProcessed = true;
            processedCanvas->setImage(processedImage);  // Show image FIRST
            processedCanvas->setMouseEventsEnabled(true);  // Enable mouse events AFTER
            
            updateStatus("Drawing mode ENABLED. Click and drag on the processed image to draw!", "success");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to initialize drawing: %1").arg(e.what()));
            drawingMode = false;
            processedCanvas->setMouseEventsEnabled(false);
            if (btn) btn->setText("Drawing Mode: OFF");
        }
    } else {
        // Disable mouse events first
        processedCanvas->setMouseEventsEnabled(false);
        
        // Finalize drawing
        if (!drawingCanvas.empty()) {
            processedImage = drawingCanvas.clone();
            recentlyProcessed = true;
            
            auto operation = [finalImage = drawingCanvas.clone()](const cv::Mat& input) -> cv::Mat {
                return finalImage;
            };
            
            currentImage = processedImage.clone();
            rightSidebar->addLayer("Hand Drawn", "brush", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
        }
        
        // Clear drawing canvas
        drawingCanvas.release();
        
        updateDisplay();
        updateStatus("Drawing mode DISABLED. Drawing saved as layer!", "info");
    }
}
