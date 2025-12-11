$missingFunctions = @'

// ===== Helper Functions =====

void MainWindow::updateDisplay() {
    if (imageLoaded && !originalImage.empty()) {
        originalCanvas->setImage(originalImage);
        QString info = QString("Size: %1 x %2 | Channels: %3")
                      .arg(originalImage.cols)
                      .arg(originalImage.rows)
                      .arg(originalImage.channels());
        originalInfoLabel->setText(info);
    }
    
    if (recentlyProcessed && !processedImage.empty()) {
        processedCanvas->setImage(processedImage);
        QString info = QString("Size: %1 x %2 | Channels: %3")
                      .arg(processedImage.cols)
                      .arg(processedImage.rows)
                      .arg(processedImage.channels());
        processedInfoLabel->setText(info);
    }
}

void MainWindow::updateStatus(const QString& message, const QString& type, int progress) {
    QString prefix;
    QString color;
    
    if (type == "success") {
        prefix = "[OK] ";
        color = Theme::SUCCESS_COLOR;
    } else if (type == "error") {
        prefix = "[ERROR] ";
        color = Theme::ERROR_COLOR;
    } else if (type == "warning") {
        prefix = "[WARNING] ";
        color = Theme::WARNING_COLOR;
    } else {
        prefix = "[INFO] ";
        color = Theme::INFO_COLOR;
    }
    
    statusLabel->setText(prefix + message);
    statusLabel->setStyleSheet(QString("color: %1; padding: 5px; font-weight: 500;").arg(color));
    
    if (progress >= 0) {
        progressBar->setValue(progress);
        progressBar->setVisible(true);
    } else {
        progressBar->setVisible(false);
    }
}

void MainWindow::finalizeProcessing(const QString& layerName, const QString& layerType) {
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer(layerName, layerType, processedImage);
        rightSidebar->updateHistogram(processedImage);
    }
}

void MainWindow::onLayerRemoveRequested(int layerIndex) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Remove Layer",
        "Remove this processing layer?\n\nThis will reset to original image.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        resetImage();
        updateStatus("Layer removed. Image reset to original.", "warning");
    }
}

// ===== File Operations =====

void MainWindow::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Image File",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp *.tiff *.tif)");
    
    if (fileName.isEmpty()) return;
    
    updateStatus("Loading image...", "info", 50);
    
    originalImage = cv::imread(fileName.toStdString());
    
    if (originalImage.empty()) {
        QMessageBox::critical(this, "Error", "Failed to load image!");
        updateStatus("Failed to load image", "error");
        return;
    }
    
    currentImage = originalImage.clone();
    imagePath = fileName;
    imageLoaded = true;
    recentlyProcessed = false;
    processedCanvas->clear();
    
    rightSidebar->clearLayers();
    rightSidebar->updateHistogram(originalImage);
    
    updateDisplay();
    updateStatus("Image loaded successfully!", "success");
}

void MainWindow::saveImage() {
    if (!recentlyProcessed || processedImage.empty()) {
        QMessageBox::warning(this, "Warning", "No processed image to save!");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Processed Image",
        "",
        "PNG (*.png);;JPEG (*.jpg);;BMP (*.bmp);;TIFF (*.tif *.tiff)");
    
    if (fileName.isEmpty()) return;
    
    updateStatus("Saving image...", "info", 75);
    
    bool success = cv::imwrite(fileName.toStdString(), processedImage);
    
    if (success) {
        updateStatus("Image saved successfully!", "success");
    } else {
        QMessageBox::critical(this, "Error", "Failed to save image!");
        updateStatus("Failed to save image", "error");
    }
}

void MainWindow::resetImage() {
    if (!imageLoaded) {
        QMessageBox::warning(this, "Warning", "No image loaded!");
        return;
    }
    
    currentImage = originalImage.clone();
    processedImage = cv::Mat();
    recentlyProcessed = false;
    
    rightSidebar->clearLayers();
    rightSidebar->updateHistogram(originalImage);
    
    processedCanvas->clear();
    updateDisplay();
    updateStatus("Image reset to original", "info");
}

void MainWindow::useProcessedImage() {
    if (!recentlyProcessed || processedImage.empty()) {
        QMessageBox::warning(this, "Warning", "No processed image available!");
        return;
    }
    
    currentImage = processedImage.clone();
    originalCanvas->setImage(currentImage);
    processedCanvas->clear();
    processedImage = cv::Mat();
    recentlyProcessed = false;
    
    updateDisplay();
    updateStatus("Using processed image as current. Ready for new operations.", "success");
}

// ===== Information Functions =====

void MainWindow::showImageInfo() {
    if (!imageLoaded) {
        QMessageBox::information(this, "Info", "No image loaded!");
        return;
    }
    QString info = QString("Size: %1x%2\nChannels: %3\nType: %4")
        .arg(currentImage.cols)
        .arg(currentImage.rows)
        .arg(currentImage.channels())
        .arg(currentImage.depth() == CV_8U ? "8-bit" : "Other");
    QMessageBox::information(this, "Image Info", info);
}

void MainWindow::showPixelInfo() {
    if (!imageLoaded) {
        QMessageBox::information(this, "Info", "No image loaded!");
        return;
    }
    QMessageBox::information(this, "Pixel Info", "Click on the image canvas to see pixel values");
}

void MainWindow::showImageStats() {
    if (!imageLoaded) {
        QMessageBox::information(this, "Info", "No image loaded!");
        return;
    }
    double minVal, maxVal;
    cv::minMaxLoc(currentImage, &minVal, &maxVal);
    QString stats = QString("Min: %1\nMax: %2").arg(minVal).arg(maxVal);
    QMessageBox::information(this, "Statistics", stats);
}

// ===== Transform Operations =====

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

void MainWindow::applyRotation() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    RotationDialog dialog(currentImage, this);
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        finalizeProcessing(QString("Rotation %1°")
            .arg(dialog.getAngle(), 0, 'f', 1), "transform");
        updateStatus("Rotation applied successfully!", "success");
    }
}

void MainWindow::applySkew() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::applySkew(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Skew", "transform");
    updateStatus("Skew applied successfully!", "success");
}

void MainWindow::applyZoom() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ZoomDialog dialog(currentImage, this);
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        finalizeProcessing(QString("Zoom %1x")
            .arg(dialog.getScale(), 0, 'f', 2), "transform");
        updateStatus("Zoom applied successfully!", "success");
    }
}

void MainWindow::applyFlipX() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::flipHorizontal(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Flip Horizontal", "transform");
    updateStatus("Flipped horizontally!", "success");
}

void MainWindow::applyFlipY() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::flipVertical(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Flip Vertical", "transform");
    updateStatus("Flipped vertically!", "success");
}

void MainWindow::applyFlipXY() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::flipBoth(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Flip Both", "transform");
    updateStatus("Flipped both ways!", "success");
}

// ===== Histogram Operations =====

void MainWindow::showHistogram() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    QDialog *histDialog = new QDialog(this);
    histDialog->setWindowTitle("Image Histogram");
    histDialog->setMinimumSize(750, 550);
    
    QVBoxLayout *layout = new QVBoxLayout(histDialog);
    
    QLabel *titleLabel = new QLabel("Pixel Value Distribution");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    layout->addWidget(titleLabel);
    
    HistogramWidget *histWidget = new HistogramWidget(histDialog);
    histWidget->setImage(currentImage);
    layout->addWidget(histWidget);
    
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    closeBtn->setMaximumWidth(150);
    connect(closeBtn, &QPushButton::clicked, histDialog, &QDialog::accept);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);
    
    histDialog->exec();
}

void MainWindow::applyHistogramEqualization() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::equalizeHistogram(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Histogram Equalization", "adjustment");
    updateStatus("Histogram equalization applied!", "success");
}

void MainWindow::applyOtsuThresholding() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::applyOtsuThreshold(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Otsu Thresholding", "adjustment");
    updateStatus("Otsu thresholding applied!", "success");
}

// ===== Image Processing =====

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

void MainWindow::applyBinaryThreshold() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::applyBinaryThreshold(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Binary Threshold", "adjustment");
    updateStatus("Binary threshold applied!", "success");
}

void MainWindow::applyGaussianBlur() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::applyGaussianBlur(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Gaussian Blur", "filter");
    updateStatus("Gaussian blur applied!", "success");
}

void MainWindow::applyEdgeDetection() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::detectEdges(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Edge Detection", "filter");
    updateStatus("Edge detection applied!", "success");
}

void MainWindow::invertColors() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageProcessor::invertColors(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Invert Colors", "adjustment");
    updateStatus("Colors inverted!", "success");
}

// ===== Filter Operations =====

void MainWindow::applyLaplacianFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageFilters::applyLaplacian(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Laplacian Filter", "filter");
    updateStatus("Laplacian filter applied successfully!", "success");
}

void MainWindow::applySobelCombinedFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::Mat dst_H, dst_V, dst_D;
    ImageFilters::applySobelCombined(currentImage, dst_H, dst_V, dst_D, processedImage, 3);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Sobel Filter (H+V+D)", "filter");
    updateStatus("Sobel filter applied successfully!", "success");
}

void MainWindow::applyTraditionalFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageFilters::applyTraditionalFilter(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Traditional Filter", "filter");
    updateStatus("Traditional filter applied successfully!", "success");
}

void MainWindow::applyPyramidalFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageFilters::applyPyramidalFilter(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Pyramidal Filter", "filter");
    updateStatus("Pyramidal filter applied successfully!", "success");
}

void MainWindow::applyCircularFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageFilters::applyCircularFilter(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Circular Filter", "filter");
    updateStatus("Circular filter applied successfully!", "success");
}

void MainWindow::applyConeFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    ImageFilters::applyConeFilter(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    finalizeProcessing("Cone Filter", "filter");
    updateStatus("Cone filter applied successfully!", "success");
}
'@

# Append to MainWindow.cpp
Add-Content -Path "src\MainWindow.cpp" -Value $missingFunctions

Write-Host "? Missing functions added to MainWindow.cpp" -ForegroundColor Green
Write-Host "?? Total lines: $((Get-Content 'src\MainWindow.cpp').Count)" -ForegroundColor Cyan
