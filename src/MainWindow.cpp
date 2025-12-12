#include "MainWindow.h"
#include "ImageCanvas.h"
#include "HistogramWidget.h"
#include "RightSidebarWidget.h"
#include "ImageProcessor.h"
#include "TransformDialog.h"
#include "AdjustmentDialog.h"
#include "CropTool.h"
#include "CompressionDialog.h"
#include "filters/ImageFilters.h"
#include "ImageMetrics.h"
#include "Theme.h"
#include "MainWindow_Macros.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QHBoxLayout>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), imageLoaded(false), recentlyProcessed(false), cropMode(false) {
    
    setWindowTitle("Naghuma Toolbox - Image Processing Suite");
    setMinimumSize(1600, 900);
    
    // Initialize crop tool
    cropTool = new CropTool(this);
    
    QApplication::setStyle("Fusion");
    setStyleSheet(Theme::MAIN_STYLESHEET);
    
    setupUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    createMenuBar();
    createToolBar();
    createCentralWidget();
    
    // Create and add right sidebar
    rightSidebar = new RightSidebarWidget(this);
    connect(rightSidebar, &RightSidebarWidget::layerRemoveRequested,
            this, &MainWindow::onLayerRemoveRequested);
    connect(rightSidebar, &RightSidebarWidget::layersRemoveRequested,
            this, &MainWindow::onLayersRemoveRequested);
    
    QWidget *central = centralWidget();
    QHBoxLayout *mainLayout = qobject_cast<QHBoxLayout*>(central->layout());
    if (mainLayout) {
        mainLayout->addWidget(rightSidebar);
    }
    
    createStatusBar();
    
    // Center window on screen
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // File Menu
    QMenu *fileMenu = menuBar->addMenu("File");
    ADD_MENU_ACTION(fileMenu, "Load Image", loadImage);
    ADD_MENU_ACTION(fileMenu, "Save Image", saveImage);
    fileMenu->addSeparator();
    ADD_MENU_ACTION(fileMenu, "Reset", resetImage);
    
    // Add Undo action with Ctrl+Z shortcut
    QAction *undoAction = fileMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence::Undo);  // Ctrl+Z
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoLastOperation);
    
    fileMenu->addSeparator();
    ADD_MENU_ACTION(fileMenu, "Exit", close);
    
    // Info Menu
    QMenu *infoMenu = menuBar->addMenu("Information");
    ADD_MENU_ACTION(infoMenu, "Image Info", showImageInfo);
    ADD_MENU_ACTION(infoMenu, "Pixel Info", showPixelInfo);
    ADD_MENU_ACTION(infoMenu, "Statistics", showImageStats);
    infoMenu->addSeparator();
    ADD_MENU_ACTION(infoMenu, "Image Metrics (RMSE/SNR/PSNR)", showImageMetrics);
    
    // Transform Menu
    QMenu *transformMenu = menuBar->addMenu("Transform");
    ADD_MENU_ACTION(transformMenu, "Translation", applyTranslation);
    ADD_MENU_ACTION(transformMenu, "Rotation", applyRotation);
    ADD_MENU_ACTION(transformMenu, "Skew", applySkew);
    ADD_MENU_ACTION(transformMenu, "Zoom", applyZoom);
    transformMenu->addSeparator();
    ADD_MENU_ACTION(transformMenu, "Flip Horizontal", applyFlipX);
    ADD_MENU_ACTION(transformMenu, "Flip Vertical", applyFlipY);
    ADD_MENU_ACTION(transformMenu, "Flip Both", applyFlipXY);
    transformMenu->addSeparator();
    
    // Crop submenu
    QAction *cropModeAction = transformMenu->addAction("Toggle Crop Mode");
    cropModeAction->setCheckable(true);
    connect(cropModeAction, &QAction::triggered, this, &MainWindow::toggleCropMode);
    ADD_MENU_ACTION(transformMenu, "Apply Crop", applyCrop);
    ADD_MENU_ACTION(transformMenu, "Cancel Crop", cancelCrop);

    // Histogram Menu
    QMenu *histMenu = menuBar->addMenu("Histogram");
    ADD_MENU_ACTION(histMenu, "Show Histogram", showHistogram);
    ADD_MENU_ACTION(histMenu, "Equalization", applyHistogramEqualization);
    ADD_MENU_ACTION(histMenu, "Otsu Thresholding", applyOtsuThresholding);
    
    // Process Menu
    QMenu *processMenu = menuBar->addMenu("Process");
    ADD_MENU_ACTION(processMenu, "Brightness/Contrast", applyBrightnessContrast);
    processMenu->addSeparator();
    ADD_MENU_ACTION(processMenu, "Grayscale", convertToGrayscale);
    ADD_MENU_ACTION(processMenu, "Binary Threshold", applyBinaryThreshold);
    ADD_MENU_ACTION(processMenu, "Gaussian Blur", applyGaussianBlur);
    ADD_MENU_ACTION(processMenu, "Edge Detection", applyEdgeDetection);
    ADD_MENU_ACTION(processMenu, "Invert Colors", invertColors);
    processMenu->addSeparator();
    ADD_MENU_ACTION(processMenu, "Compress Image...", applyCompression);
    
    // Filters Menu
    QMenu *filtersMenu = menuBar->addMenu("Filters");
    ADD_MENU_ACTION(filtersMenu, "Laplacian Filter", applyLaplacianFilter);
    ADD_MENU_ACTION(filtersMenu, "Sobel Filter", applySobelCombinedFilter);
    ADD_MENU_ACTION(filtersMenu, "Traditional Filter", applyTraditionalFilter);
    ADD_MENU_ACTION(filtersMenu, "Pyramidal Filter", applyPyramidalFilter);
    ADD_MENU_ACTION(filtersMenu, "Circular Filter", applyCircularFilter);
    ADD_MENU_ACTION(filtersMenu, "Cone Filter", applyConeFilter);
    
    // Morphology Menu
    QMenu *morphMenu = menuBar->addMenu("Morphology");
    ADD_MENU_ACTION(morphMenu, "Erosion", applyErosion);
    ADD_MENU_ACTION(morphMenu, "Dilation", applyDilation);
    morphMenu->addSeparator();
    ADD_MENU_ACTION(morphMenu, "Opening", applyOpening);
    ADD_MENU_ACTION(morphMenu, "Closing", applyClosing);
    morphMenu->addSeparator();
    ADD_MENU_ACTION(morphMenu, "Morphological Gradient", applyMorphGradient);
    
    // FFT Menu
    QMenu *fftMenu = menuBar->addMenu("FFT");
    ADD_MENU_ACTION(fftMenu, "Show FFT Spectrum", showFFTSpectrum);
    fftMenu->addSeparator();
    ADD_MENU_ACTION(fftMenu, "Low-Pass Filter", applyLowPassFilter);
    ADD_MENU_ACTION(fftMenu, "High-Pass Filter", applyHighPassFilter);
}

void MainWindow::createToolBar() {
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(24, 24));
    addToolBar(Qt::TopToolBarArea, toolbar);
    
    auto addBtn = [&](const QString& text, auto slot, int minWidth = 100, const char* styleClass = nullptr) {
        QPushButton *btn = new QPushButton(text, this);
        if (styleClass) btn->setProperty("class", styleClass);
        btn->setMinimumWidth(minWidth);
        connect(btn, &QPushButton::clicked, this, slot);
        toolbar->addWidget(btn);
        return btn;
    };
    
    addBtn("Load Image", &MainWindow::loadImage, 140, "accent");
    toolbar->addSeparator();
    addBtn("Save", &MainWindow::saveImage);
    addBtn("Reset", &MainWindow::resetImage);
    
    // Undo button - NEW
    undoButton = addBtn("Undo", &MainWindow::undoLastOperation);
    undoButton->setEnabled(false);  // Initially disabled
    undoButton->setToolTip("Undo last operation (removes last layer)");
    
    addBtn("Use Processed", &MainWindow::useProcessedImage, 120)->setToolTip("Use the processed image for next operations");
    toolbar->addSeparator();
    
    // Crop button
    QPushButton *cropModeBtn = addBtn("Crop Mode: OFF", &MainWindow::toggleCropMode, 120);
    cropModeBtn->setObjectName("cropModeButton");
    cropModeBtn->setCheckable(true);
    cropModeBtn->setToolTip("Toggle crop mode to select and crop image region");
    toolbar->addSeparator();
    
    addBtn("Grayscale", &MainWindow::convertToGrayscale);
    addBtn("Blur", &MainWindow::applyGaussianBlur);
    addBtn("Edges", &MainWindow::applyEdgeDetection);
}

void MainWindow::createCentralWidget() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left and center area for images
    QWidget *imagesArea = new QWidget();
    QHBoxLayout *imagesLayout = new QHBoxLayout(imagesArea);
    imagesLayout->setSpacing(20);
    imagesLayout->setContentsMargins(20, 20, 20, 20);
    
    // Original image section with glow border
    QVBoxLayout *originalLayout = new QVBoxLayout();
    
    QLabel *originalTitle = new QLabel("Original");
    originalTitle->setObjectName("imageTitle");
    originalLayout->addWidget(originalTitle);
    
    // Container for canvas with border glow
    QWidget *originalContainer = new QWidget();
    originalContainer->setObjectName("imageContainer");
    QVBoxLayout *originalContainerLayout = new QVBoxLayout(originalContainer);
    originalContainerLayout->setContentsMargins(0, 0, 0, 0);
    
    originalCanvas = new ImageCanvas(originalContainer, "#e879f9");
    originalContainerLayout->addWidget(originalCanvas);
    
    originalLayout->addWidget(originalContainer, 1);
    
    originalInfoLabel = new QLabel("No image loaded");
    originalInfoLabel->setObjectName("infoLabel");
    originalInfoLabel->setAlignment(Qt::AlignCenter);
    originalLayout->addWidget(originalInfoLabel);
    
    imagesLayout->addLayout(originalLayout, 1);
    
    // Processed image section with glow border
    QVBoxLayout *processedLayout = new QVBoxLayout();
    
    QLabel *processedTitle = new QLabel("Processed");
    processedTitle->setObjectName("processedTitle");
    processedLayout->addWidget(processedTitle);
    
    // Container for canvas with border glow
    QWidget *processedContainer = new QWidget();
    processedContainer->setObjectName("processedContainer");
    QVBoxLayout *processedContainerLayout = new QVBoxLayout(processedContainer);
    processedContainerLayout->setContentsMargins(0, 0, 0, 0);
    
    processedCanvas = new ImageCanvas(processedContainer, "#c026d3");
    processedContainerLayout->addWidget(processedCanvas);
    
    // Connect mouse events for crop tool
    connect(processedCanvas, &ImageCanvas::mousePressed, this, &MainWindow::onCropMousePress);
    connect(processedCanvas, &ImageCanvas::mouseMoved, this, &MainWindow::onCropMouseMove);
    connect(processedCanvas, &ImageCanvas::mouseReleased, this, &MainWindow::onCropMouseRelease);
    
    processedLayout->addWidget(processedContainer, 1);
    
    // Initialize processedInfoLabel (was missing - caused crash)
    processedInfoLabel = new QLabel("No processing yet");
    processedInfoLabel->setObjectName("infoLabel");
    processedInfoLabel->setAlignment(Qt::AlignCenter);
    processedLayout->addWidget(processedInfoLabel);
    
    // Metrics label (below both images)
    metricsLabel = new QLabel("");
    metricsLabel->setObjectName("metricsLabel");
    metricsLabel->setAlignment(Qt::AlignCenter);
    metricsLabel->setVisible(false);
    processedLayout->addWidget(metricsLabel);
    
    imagesLayout->addLayout(processedLayout, 1);
    
    mainLayout->addWidget(imagesArea, 1);
}

void MainWindow::createStatusBar() {
    QStatusBar *status = statusBar();
    
    statusLabel = new QLabel("Welcome to Naghuma Toolbox! Load an image to get started...");
    status->addWidget(statusLabel, 1);
    
    progressBar = new QProgressBar(this);
    progressBar->setMaximumWidth(200);
    progressBar->setMaximumHeight(20);
    progressBar->setVisible(false);
    status->addPermanentWidget(progressBar);
}

bool MainWindow::checkImageLoaded(const QString& operation) {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", 
            QString("Please load an image first before attempting to %1!").arg(operation));
        return false;
    }
    return true;
}

void MainWindow::applySimpleFilter(
    std::function<void(const cv::Mat&, cv::Mat&)> filterFunc,
    std::function<cv::Mat(const cv::Mat&)> operationFunc,
    const QString& layerName,
    const QString& layerType,
    const QString& successMessage
) {
    if (!checkImageLoaded("apply filter")) return;
    
    filterFunc(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer(layerName, layerType, processedImage, operationFunc);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus(successMessage, "success");
}

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
        
        // Update metrics display
        updateMetricsDisplay();
    } else {
        metricsLabel->setVisible(false);
    }
}

void MainWindow::updateMetricsDisplay() {
    if (!imageLoaded || !recentlyProcessed || processedImage.empty()) {
        metricsLabel->setVisible(false);
        return;
    }
    
    auto result = ImageMetrics::calculateMetrics(originalImage, processedImage);
    
    if (!result.isValid) {
        metricsLabel->setVisible(false);
        return;
    }
    
    QString metricsText = QString("RMSE: %1 | SNR: %2 dB | PSNR: %3 dB")
        .arg(result.rmse, 0, 'f', 2)
        .arg(result.snr, 0, 'f', 2)
        .arg(std::isinf(result.psnr) ? QString("?") : QString::number(result.psnr, 'f', 2));
    
    metricsLabel->setText(metricsText);
    metricsLabel->setVisible(true);
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
        
        // Note: Operations should be provided via applySimpleFilter/applySimpleTransform
        // If we reach here without an operation function, the layer system will use the stored image
        // This is a fallback for operations that don't provide a replay function
        
        rightSidebar->addLayer(layerName, layerType, processedImage, nullptr);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
}

void MainWindow::onLayerRemoveRequested(int layerIndex) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Remove Layer",
        "Remove this processing layer-\n\nImage will be rebuilt from remaining operations.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Remove the layer from the sidebar
        rightSidebar->removeLayer(layerIndex);
        
        // Rebuild image from remaining layers
        int remainingLayers = rightSidebar->getLayerCount();
        
        if (remainingLayers == 0) {
            // No layers left - show original
            currentImage = originalImage.clone();
            processedImage = cv::Mat();
            recentlyProcessed = false;
            processedCanvas->clear();
        } else {
            // Rebuild from all remaining layers
            cv::Mat rebuiltImage = rightSidebar->rebuildImage(originalImage);
            
            if (!rebuiltImage.empty()) {
                currentImage = rebuiltImage.clone();
                processedImage = rebuiltImage.clone();
                recentlyProcessed = true;
            } else {
                // Fallback to previous layer's image
                if (layerIndex > 0) {
                    processedImage = rightSidebar->getLayerImage(layerIndex - 1);
                } else {
                    processedImage = rightSidebar->getLayerImage(0);
                }
                currentImage = processedImage.clone();
                recentlyProcessed = true;
            }
        }
        
        // Update display
        updateDisplay();
        rightSidebar->updateHistogram(currentImage);
        updateUndoButtonState();
        
        updateStatus(QString("Layer removed. %1 layers remaining.")
            .arg(rightSidebar->getLayerCount()), "success");
    }
}

void MainWindow::onLayersRemoveRequested(const QList<int>& layerIndices) {
    if (layerIndices.isEmpty()) return;
    
    QString message = layerIndices.size() == 1 
        ? "Remove 1 layer-"
        : QString("Remove %1 layers?").arg(layerIndices.size());
    message += "\n\nImage will be rebuilt from remaining operations.";
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Remove Layers",
        message,
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Sort indices in descending order to remove from end to start
        QList<int> sortedIndices = layerIndices;
        std::sort(sortedIndices.begin(), sortedIndices.end(), std::greater<int>());
        
        // Remove all selected layers
        for (int index : sortedIndices) {
            rightSidebar->removeLayer(index);
        }
        
        // Rebuild image from remaining layers
        int remainingLayers = rightSidebar->getLayerCount();
        
        if (remainingLayers == 0) {
            // No layers left - show original
            currentImage = originalImage.clone();
            processedImage = cv::Mat();
            recentlyProcessed = false;
            processedCanvas->clear();
        } else {
            // Rebuild from all remaining layers
            cv::Mat rebuiltImage = rightSidebar->rebuildImage(originalImage);
            
            if (!rebuiltImage.empty()) {
                currentImage = rebuiltImage.clone();
                processedImage = rebuiltImage.clone();
                recentlyProcessed = true;
            } else {
                // Fallback to last layer's image
                processedImage = rightSidebar->getLayerImage(remainingLayers - 1);
                currentImage = processedImage.clone();
                recentlyProcessed = true;
            }
        }
        
        // Update display
        updateDisplay();
        rightSidebar->updateHistogram(currentImage);
        updateUndoButtonState();  // Update undo button state
        
        updateStatus(QString("Removed %1 layers. %2 layers remaining.")
            .arg(layerIndices.size())
            .arg(rightSidebar->getLayerCount()), "success");
    }
}

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

void MainWindow::undoLastOperation() {
    if (!imageLoaded) {
        QMessageBox::warning(this, "Warning", "No image loaded!");
        return;
    }
    
    int layerCount = rightSidebar->getLayerCount();
    
    if (layerCount == 0) {
        QMessageBox::information(this, "No Operations", "No operations to undo!");
        return;
    }
    
    // Remove the last layer
    rightSidebar->removeLayer(layerCount - 1);
    
    // Rebuild from remaining layers
    int remainingLayers = rightSidebar->getLayerCount();
    
    if (remainingLayers == 0) {
        // No layers left - revert to original
        currentImage = originalImage.clone();
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateStatus("Undone all operations. Reverted to original image.", "success");
    } else {
        // Rebuild from remaining layers
        cv::Mat rebuiltImage = rightSidebar->rebuildImage(originalImage);
        
        if (!rebuiltImage.empty()) {
            currentImage = rebuiltImage.clone();
            processedImage = rebuiltImage.clone();
            recentlyProcessed = true;
        } else {
            // Fallback to previous layer's image
            processedImage = rightSidebar->getLayerImage(remainingLayers - 1);
            currentImage = processedImage.clone();
            recentlyProcessed = true;
        }
    }
    
    // Update displays
    updateDisplay();
    rightSidebar->updateHistogram(currentImage);
    updateUndoButtonState();
}

void MainWindow::updateUndoButtonState() {
    if (undoButton) {
        bool hasLayers = imageLoaded && rightSidebar->getLayerCount() > 0;
        undoButton->setEnabled(hasLayers);
    }
}

void MainWindow::showImageInfo() {
    if (!checkImageLoaded("show image info")) return;
    QString info = QString("Size: %1x%2\nChannels: %3\nType: %4")
        .arg(currentImage.cols)
        .arg(currentImage.rows)
        .arg(currentImage.channels())
        .arg(currentImage.depth() == CV_8U ? "8-bit" : "Other");
    QMessageBox::information(this, "Image Info", info);
}

void MainWindow::showPixelInfo() {
    if (!checkImageLoaded("show pixel info")) return;
    QMessageBox::information(this, "Pixel Info", "Click on the image canvas to see pixel values");
}

void MainWindow::showImageStats() {
    if (!checkImageLoaded("show statistics")) return;
    double minVal, maxVal;
    cv::minMaxLoc(currentImage, &minVal, &maxVal);
    QString stats = QString("Min: %1\nMax: %2").arg(minVal).arg(maxVal);
    QMessageBox::information(this, "Statistics", stats);
}

void MainWindow::applyTranslation() {
    if (!checkImageLoaded("apply translation")) return;
    
    TranslationDialog dialog(currentImage, this);
    
    // Connect preview signal to update processed image
    connect(&dialog, &TranslationDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        // Store the actual operation with captured parameters
        int tx = dialog.getTranslationX();
        int ty = dialog.getTranslationY();
        auto operation = [tx, ty](const cv::Mat& input) -> cv::Mat {
            cv::Mat result;
            cv::Mat transMat = (cv::Mat_<double>(2, 3) << 1, 0, tx, 0, 1, ty);
            cv::warpAffine(input, result, transMat, input.size());
            return result;
        };
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(QString("Translation (%1, %2)").arg(tx).arg(ty), 
                                  "transform", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();  // Update undo button state
        }
        
        updateStatus("Translation applied successfully!", "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}

void MainWindow::applyRotation() {
    if (!checkImageLoaded("apply rotation")) return;
    
    RotationDialog dialog(currentImage, this);
    
    // Connect preview signal to update processed image
    connect(&dialog, &RotationDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        // Store the actual operation with captured parameters
        double angle = dialog.getAngle();
        auto operation = [angle](const cv::Mat& input) -> cv::Mat {
            cv::Mat result;
            cv::Point2f center(input.cols / 2.0f, input.rows / 2.0f);
            cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, 1.0);
            cv::warpAffine(input, result, rotMat, input.size());
            return result;
        };
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(QString("Rotation %1°").arg(angle, 0, 'f', 1), 
                                  "transform", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();  // Update undo button state
        }
        
        updateStatus("Rotation applied successfully!", "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}

void MainWindow::applySkew() {
    if (!checkImageLoaded("apply skew")) return;
    
    SkewDialog dialog(currentImage, this);
    
    // Connect preview signal to update processed image
    connect(&dialog, &SkewDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        // Store the actual operation with captured parameters
        double skewX = dialog.getSkewX();
        double skewY = dialog.getSkewY();
        auto operation = [skewX, skewY](const cv::Mat& input) -> cv::Mat {
            cv::Mat result;
            ImageProcessor::applySkew(input, result, skewX, skewY);
            return result;
        };
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(QString("Skew (%.2f, %.2f)").arg(skewX).arg(skewY), 
                                  "transform", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();  // Update undo button state
        }
        
        updateStatus("Skew applied successfully!", "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}

void MainWindow::applyZoom() {
    if (!checkImageLoaded("apply zoom")) return;
    
    ZoomDialog dialog(currentImage, this);
    
    // Connect preview signal to update processed image
    connect(&dialog, &ZoomDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getTransformedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        // Store the actual operation with captured parameters
        double scale = dialog.getScale();
        auto operation = [scale](const cv::Mat& input) -> cv::Mat {
            cv::Mat result;
            cv::resize(input, result, cv::Size(), scale, scale, cv::INTER_LINEAR);
            return result;
        };
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(QString("Zoom %1x").arg(scale, 0, 'f', 2), 
                                  "transform", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();  // Update undo button state
        }
        
        updateStatus("Zoom applied successfully!", "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}

void MainWindow::applyFlipX() {
    if (!checkImageLoaded("apply flip horizontal")) return;
    
    ImageProcessor::flipHorizontal(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    
    auto operation = [](const cv::Mat& input) -> cv::Mat {
        cv::Mat result;
        ImageProcessor::flipHorizontal(input, result);
        return result;
    };
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer("Flip Horizontal", "transform", processedImage, operation);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus("Flipped horizontally!", "success");
}

void MainWindow::applyFlipY() {
    if (!checkImageLoaded("apply flip vertical")) return;
    
    ImageProcessor::flipVertical(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    
    auto operation = [](const cv::Mat& input) -> cv::Mat {
        cv::Mat result;
        ImageProcessor::flipVertical(input, result);
        return result;
    };
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer("Flip Vertical", "transform", processedImage, operation);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus("Flipped vertically!", "success");
}

void MainWindow::applyFlipXY() {
    if (!checkImageLoaded("apply flip both")) return;
    
    ImageProcessor::flipBoth(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    
    auto operation = [](const cv::Mat& input) -> cv::Mat {
        cv::Mat result;
        ImageProcessor::flipBoth(input, result);
        return result;
    };
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer("Flip Both", "transform", processedImage, operation);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus("Flipped both ways!", "success");
}
void MainWindow::showHistogram() {
    if (!checkImageLoaded("show histogram")) return;
    
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
    if (!checkImageLoaded("apply histogram equalization")) return;
    
    ImageProcessor::equalizeHistogram(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    
    auto operation = [](const cv::Mat& input) -> cv::Mat {
        cv::Mat result;
        ImageProcessor::equalizeHistogram(input, result);
        return result;
    };
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer("Histogram Equalization", "adjustment", processedImage, operation);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus("Histogram equalization applied!", "success");
}

void MainWindow::applyOtsuThresholding() {
    if (!checkImageLoaded("apply Otsu thresholding")) return;
    
    ImageProcessor::applyOtsuThreshold(currentImage, processedImage);
    recentlyProcessed = true;
    updateDisplay();
    
    auto operation = [](const cv::Mat& input) -> cv::Mat {
        cv::Mat result;
        ImageProcessor::applyOtsuThreshold(input, result);
        return result;
    };
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer("Otsu Thresholding", "adjustment", processedImage, operation);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus("Otsu thresholding applied!", "success");
}

void MainWindow::applyBrightnessContrast() {
    if (!checkImageLoaded("adjust brightness/contrast")) return;
    
    AdjustmentDialog dialog(currentImage, this);
    
    // Connect preview signal to update processed image
    connect(&dialog, &AdjustmentDialog::previewUpdated, this, [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        recentlyProcessed = true;
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getAdjustedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        // Store the actual operation with captured parameters
        int brightness = dialog.getBrightness();
        int contrast = dialog.getContrast();
        auto operation = [brightness, contrast](const cv::Mat& input) -> cv::Mat {
            cv::Mat result;
            ImageProcessor::adjustBrightnessContrast(input, result, brightness, contrast);
            return result;
        };
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            rightSidebar->addLayer(QString("Brightness/Contrast (%1, %2)")
                                  .arg(brightness).arg(contrast), 
                                  "adjustment", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();  // Update undo button state
        }
        
        updateStatus("Brightness/Contrast applied successfully!", "success");
    } else {
        // User cancelled - clear preview
        processedImage = cv::Mat();
        recentlyProcessed = false;
        processedCanvas->clear();
        updateDisplay();
    }
}

void MainWindow::convertToGrayscale() {
    applySimpleFilter(
        ImageProcessor::convertToGrayscale,
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::convertToGrayscale(input, result);
            return result;
        },
        "Grayscale", "adjustment", "Converted to grayscale!"
    );
}

void MainWindow::applyBinaryThreshold() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageProcessor::applyBinaryThreshold(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyBinaryThreshold(input, result);
            return result;
        },
        "Binary Threshold", "adjustment", "Binary threshold applied!"
    );
}

void MainWindow::applyGaussianBlur() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageProcessor::applyGaussianBlur(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyGaussianBlur(input, result);
            return result;
        },
        "Gaussian Blur", "filter", "Gaussian blur applied!"
    );
}

void MainWindow::applyEdgeDetection() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageProcessor::detectEdges(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::detectEdges(input, result);
            return result;
        },
        "Edge Detection", "filter", "Edge detection applied!"
    );
}

void MainWindow::invertColors() {
    applySimpleFilter(
        ImageProcessor::invertColors,
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::invertColors(input, result);
            return result;
        },
        "Invert Colors", "adjustment", "Colors inverted!"
    );
}

void MainWindow::applyCompression() {
    if (!checkImageLoaded("apply compression")) return;
    
    CompressionDialog dialog(currentImage, this);
    
    if (dialog.exec() == QDialog::Accepted && dialog.wasApplied()) {
        processedImage = dialog.getCompressedImage();
        recentlyProcessed = true;
        
        // Store compression parameters
        QString compressionType = dialog.getCompressionType();
        int quality = dialog.getQuality();
        int pngLevel = dialog.getPngLevel();
        
        // Create operation function based on compression type
        auto operation = [compressionType, quality, pngLevel](const cv::Mat& input) -> cv::Mat {
            std::vector<uchar> buffer;
            if (compressionType == "JPEG") {
                std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, quality};
                cv::imencode(".jpg", input, buffer, params);
            } else {
                std::vector<int> params = {cv::IMWRITE_PNG_COMPRESSION, pngLevel};
                cv::imencode(".png", input, buffer, params);
            }
            return cv::imdecode(buffer, cv::IMREAD_UNCHANGED);
        };
        
        if (!processedImage.empty()) {
            currentImage = processedImage.clone();
            
            QString layerName;
            if (compressionType == "JPEG") {
                layerName = QString("JPEG Compression (Q:%1, Ratio:%2x)")
                    .arg(quality)
                    .arg(dialog.getCompressionRatio(), 0, 'f', 2);
            } else {
                layerName = QString("PNG Compression (L:%1, Ratio:%2x)")
                    .arg(pngLevel)
                    .arg(dialog.getCompressionRatio(), 0, 'f', 2);
            }
            
            rightSidebar->addLayer(layerName, "compression", processedImage, operation);
            rightSidebar->updateHistogram(processedImage);
            updateUndoButtonState();
        }
        
        QString statusMsg = QString("Compression applied! Ratio: %1x, PSNR: %2 dB")
            .arg(dialog.getCompressionRatio(), 0, 'f', 2)
            .arg(std::isinf(dialog.getPSNR()) ? QString("∞") : QString::number(dialog.getPSNR(), 'f', 2));
        
        updateDisplay();
        updateStatus(statusMsg, "success");
    }
}

void MainWindow::applyLaplacianFilter() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) {
            ImageFilters::applyLaplacian(src, dst);
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyLaplacian(input, result);
            return result;
        },
        "Laplacian Filter", "filter", "Laplacian filter applied successfully!"
    );
}

void MainWindow::applySobelCombinedFilter() {
    if (!checkImageLoaded("apply Sobel filter")) return;
    
    cv::Mat dst_H, dst_V, dst_D;
    ImageFilters::applySobelCombined(currentImage, dst_H, dst_V, dst_D, processedImage, 3);
    recentlyProcessed = true;
    updateDisplay();
    
    auto operation = [](const cv::Mat& input) -> cv::Mat {
        cv::Mat dst_H, dst_V, dst_D, result;
        ImageFilters::applySobelCombined(input, dst_H, dst_V, dst_D, result, 3);
        return result;
    };
    
    if (!processedImage.empty()) {
        currentImage = processedImage.clone();
        rightSidebar->addLayer("Sobel Filter (H+V+D)", "filter", processedImage, operation);
        rightSidebar->updateHistogram(processedImage);
        updateUndoButtonState();  // Update undo button state
    }
    
    updateStatus("Sobel filter applied successfully!", "success");
}

void MainWindow::applyTraditionalFilter() {
    applySimpleFilter(
        ImageFilters::applyTraditionalFilter,
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyTraditionalFilter(input, result);
            return result;
        },
        "Traditional Filter", "filter", "Traditional filter applied successfully!"
    );
}

void MainWindow::applyPyramidalFilter() {
    applySimpleFilter(
        ImageFilters::applyPyramidalFilter,
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyPyramidalFilter(input, result);
            return result;
        },
        "Pyramidal Filter", "filter", "Pyramidal filter applied successfully!"
    );
}

void MainWindow::applyCircularFilter() {
    applySimpleFilter(
        ImageFilters::applyCircularFilter,
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyCircularFilter(input, result);
            return result;
        },
        "Circular Filter", "filter", "Circular filter applied successfully!"
    );
}

void MainWindow::applyConeFilter() {
    applySimpleFilter(
        ImageFilters::applyConeFilter,
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageFilters::applyConeFilter(input, result);
            return result;
        },
        "Cone Filter", "filter", "Cone filter applied successfully!"
    );
}

void MainWindow::applyErosion() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyErosion(src, dst, 5); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyErosion(input, result, 5);
            return result;
        },
        "Erosion", "morphology", "Erosion applied successfully!"
    );
}

void MainWindow::applyDilation() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyDilation(src, dst, 5); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyDilation(input, result, 5);
            return result;
        },
        "Dilation", "morphology", "Dilation applied successfully!"
    );
}

void MainWindow::applyOpening() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyOpening(src, dst, 5); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyOpening(input, result, 5);
            return result;
        },
        "Opening", "morphology", "Opening applied successfully!"
    );
}

void MainWindow::applyClosing() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyClosing(src, dst, 5); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyClosing(input, result, 5);
            return result;
        },
        "Closing", "morphology", "Closing applied successfully!"
    );
}

void MainWindow::applyMorphGradient() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyMorphGradient(src, dst, 5); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyMorphGradient(input, result, 5);
            return result;
        },
        "Morphological Gradient", "morphology", "Morphological gradient applied successfully!"
    );
}

void MainWindow::showFFTSpectrum() {
    if (!checkImageLoaded("show FFT spectrum")) return;
    
    cv::Mat magnitude, phase;
    ImageProcessor::applyFFT(currentImage, magnitude, phase);
    
    QDialog *fftDialog = new QDialog(this);
    fftDialog->setWindowTitle("FFT Spectrum");
    fftDialog->setMinimumSize(600, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(fftDialog);
    
    QLabel *titleLabel = new QLabel("Frequency Domain Representation");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    layout->addWidget(titleLabel);
    
    // Display magnitude spectrum
    ImageCanvas *spectrumCanvas = new ImageCanvas(fftDialog, "#e879f9");
    spectrumCanvas->setMinimumSize(400, 400);
    spectrumCanvas->setImage(magnitude);
    layout->addWidget(spectrumCanvas);
    
    QLabel *infoLabel = new QLabel("Brighter regions = Higher frequency components");
    infoLabel->setStyleSheet("color: #c4b5fd; padding: 10px; font-size: 10pt;");
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);
    
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    closeBtn->setMaximumWidth(150);
    connect(closeBtn, &QPushButton::clicked, fftDialog, &QDialog::accept);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);
    
    fftDialog->exec();
}

void MainWindow::applyLowPassFilter() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyLowPassFilter(src, dst, 30); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyLowPassFilter(input, result, 30);
            return result;
        },
        "Low-Pass Filter", "fft", "Low-pass filter applied successfully!"
    );
}

void MainWindow::applyHighPassFilter() {
    applySimpleFilter(
        [](const cv::Mat& src, cv::Mat& dst) { 
            ImageProcessor::applyHighPassFilter(src, dst, 30); 
        },
        [](const cv::Mat& input) {
            cv::Mat result;
            ImageProcessor::applyHighPassFilter(input, result, 30);
            return result;
        },
        "High-Pass Filter", "fft", "High-pass filter applied successfully!"
    );
}

void MainWindow::showImageMetrics() {
    if (!checkImageLoaded("show image metrics")) return;
    
    if (!recentlyProcessed || processedImage.empty()) {
        QMessageBox::warning(this, "Warning", 
            "No processed image to compare!\n\nApply a filter or transformation first.");
        return;
    }
    
    auto result = ImageMetrics::calculateMetrics(originalImage, processedImage);
    
    if (!result.isValid) {
        QMessageBox::critical(this, "Error", result.errorMessage);
        return;
    }
    
    QDialog *metricsDialog = new QDialog(this);
    metricsDialog->setWindowTitle("Image Quality Metrics");
    metricsDialog->setMinimumSize(500, 350);
    
    QVBoxLayout *layout = new QVBoxLayout(metricsDialog);
    
    QLabel *titleLabel = new QLabel("Quality Comparison: Original vs Processed");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    layout->addWidget(titleLabel);
    
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
    metricsInfo += "\n\n===================================\n";
    metricsInfo += "Interpretation:\n";
    metricsInfo += "===================================\n\n";
    metricsInfo += "� MSE/RMSE: Lower is better (0 = identical)\n";
    metricsInfo += "� SNR: Higher is better (signal vs noise)\n";
    metricsInfo += "� PSNR: Higher is better\n";
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

// ============================================================================
// CROP TOOL IMPLEMENTATION
// ============================================================================

void MainWindow::toggleCropMode() {
    if (!imageLoaded) {
        QMessageBox::warning(this, "Warning", "Please load an image first!");
        QPushButton *btn = findChild<QPushButton*>("cropModeButton");
        if (btn) btn->setChecked(false);
        return;
    }
    
    if (currentImage.empty()) {
        QMessageBox::warning(this, "Warning", "Current image is not available!");
        QPushButton *btn = findChild<QPushButton*>("cropModeButton");
        if (btn) btn->setChecked(false);
        return;
    }
    
    cropMode = !cropMode;
    
    // Update button text and style
    QPushButton *btn = findChild<QPushButton*>("cropModeButton");
    if (btn) {
        if (cropMode) {
            btn->setText("Crop Mode: ON");
            btn->setProperty("class", "accent");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        } else {
            btn->setText("Crop Mode: OFF");
            btn->setProperty("class", "");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        }
    }
    
    // Enable/disable mouse events on processed canvas
    processedCanvas->setMouseEventsEnabled(cropMode);
    
    if (cropMode) {
        // Start crop mode - show current image on processed canvas
        cropPreviewImage = currentImage.clone();
        processedImage = cropPreviewImage.clone();
        recentlyProcessed = true;
        updateDisplay();
        updateStatus("Crop mode ENABLED. Click and drag on the processed image to select area to crop!", "success");
    } else {
        // Exit crop mode
        if (cropTool->hasSelection()) {
            // Cancel any pending selection
            cropTool->cancelSelection();
        }
        processedCanvas->clear();
        processedImage = cv::Mat();
        recentlyProcessed = false;
        updateDisplay();
        updateStatus("Crop mode DISABLED", "info");
    }
}

void MainWindow::onCropMousePress(const QPoint& pos) {
    if (!cropMode || cropPreviewImage.empty() || pos.x() < 0 || pos.y() < 0) return;
    
    // Ensure position is within image bounds
    if (pos.x() >= cropPreviewImage.cols || pos.y() >= cropPreviewImage.rows) return;
    
    cropTool->startSelection(pos);
}

void MainWindow::onCropMouseMove(const QPoint& pos) {
    if (!cropMode || cropPreviewImage.empty() || pos.x() < 0 || pos.y() < 0) return;
    
    // Ensure position is within image bounds
    if (pos.x() >= cropPreviewImage.cols || pos.y() >= cropPreviewImage.rows) return;
    
    if (cropTool->isSelectingNow()) {
        cropTool->updateSelection(pos);
        
        // Update preview with selection overlay
        cv::Mat preview = cropTool->getPreview(cropPreviewImage);
        processedImage = preview;
        processedCanvas->setImage(processedImage);
    }
}

void MainWindow::onCropMouseRelease(const QPoint& pos) {
    if (!cropMode || cropPreviewImage.empty()) return;
    
    cropTool->finishSelection();
    
    // Update preview with final selection
    if (cropTool->hasSelection() && cropTool->isValidCrop()) {
        cv::Mat preview = cropTool->getPreview(cropPreviewImage);
        processedImage = preview;
        processedCanvas->setImage(processedImage);
        
        QRect cropRect = cropTool->getCropRect();
        updateStatus(QString("Crop area selected: %1x%2 at (%3, %4). Click 'Apply Crop' to crop the image.")
            .arg(cropRect.width())
            .arg(cropRect.height())
            .arg(cropRect.x())
            .arg(cropRect.y()), "success");
    }
}

void MainWindow::applyCrop() {
    if (!cropMode) {
        QMessageBox::warning(this, "Warning", "Please enable Crop Mode first!");
        return;
    }
    
    if (!cropTool->hasSelection() || !cropTool->isValidCrop()) {
        QMessageBox::warning(this, "Warning", 
            "No valid crop area selected!\n\nClick and drag on the image to select an area to crop.");
        return;
    }
    
    // Apply crop
    cv::Mat croppedImage = cropTool->applyCrop(cropPreviewImage);
    
    if (croppedImage.empty()) {
        QMessageBox::critical(this, "Error", "Failed to crop image!");
        return;
    }
    
    // Store crop rectangle for layer description
    QRect cropRect = cropTool->getValidatedRect(cv::Size(cropPreviewImage.cols, cropPreviewImage.rows));
    
    // Create operation that captures crop rectangle
    auto operation = [cropRect](const cv::Mat& input) -> cv::Mat {
        // Validate rectangle is within bounds
        if (cropRect.x() < 0 || cropRect.y() < 0 ||
            cropRect.x() + cropRect.width() > input.cols ||
            cropRect.y() + cropRect.height() > input.rows) {
            return input;
        }
        
        cv::Rect cvRect(cropRect.x(), cropRect.y(), 
                       cropRect.width(), cropRect.height());
        return input(cvRect).clone();
    };
    
    // Store the pre-crop image as original for metrics comparison
    originalImage = cropPreviewImage.clone();
    
    // Update current and processed images
    processedImage = croppedImage.clone();
    currentImage = croppedImage.clone();
    recentlyProcessed = true;
    
    // Add to layers
    rightSidebar->addLayer(
        QString("Crop (%1x%2)").arg(cropRect.width()).arg(cropRect.height()),
        "transform",
        processedImage,
        operation
    );
    rightSidebar->updateHistogram(processedImage);
    updateUndoButtonState();  // Update undo button state
    
    // Exit crop mode
    cropMode = false;
    processedCanvas->setMouseEventsEnabled(false);
    QPushButton *btn = findChild<QPushButton*>("cropModeButton");
    if (btn) {
        btn->setChecked(false);
        btn->setText("Crop Mode: OFF");
        btn->setProperty("class", "");
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
    
    // Reset crop tool
    cropTool->cancelSelection();
    
    // Update display - this will show the original (pre-crop) on left and cropped on right
    updateDisplay();
    updateStatus(QString("Image cropped to %1x%2 pixels!")
        .arg(croppedImage.cols)
        .arg(croppedImage.rows), "success");
}

void MainWindow::cancelCrop() {
    if (!cropMode) {
        return;
    }
    
    // Cancel selection
    cropTool->cancelSelection();
    
    // Exit crop mode
    cropMode = false;
    processedCanvas->setMouseEventsEnabled(false);
    QPushButton *btn = findChild<QPushButton*>("cropModeButton");
    if (btn) {
        btn->setChecked(false);
        btn->setText("Crop Mode: OFF");
        btn->setProperty("class", "");
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
    
    // Clear preview
    processedCanvas->clear();
    processedImage = cv::Mat();
    recentlyProcessed = false;
    updateDisplay();
    
    updateStatus("Crop cancelled", "info");
}

// ============================================================================
// KEYBOARD EVENT HANDLING
// ============================================================================

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Handle Enter/Return key to apply crop
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (cropMode && cropTool->hasSelection() && cropTool->isValidCrop()) {
            applyCrop();
            event->accept();
            return;
        }
    }
    
    // Handle Escape key to cancel crop
    if (event->key() == Qt::Key_Escape) {
        if (cropMode) {
            cancelCrop();
            event->accept();
            return;
        }
    }
    
    // Pass other events to base class
    QMainWindow::keyPressEvent(event);
}