#include "OCRDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QPainter>

// ============================================================================
// CONSTRUCTOR & DESTRUCTOR
// ============================================================================

OCRDialog::OCRDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), confidence(0.0f) {
    
    setWindowTitle("OCR - Optical Character Recognition");
    setMinimumSize(1000, 700);
    
    // Validate input image
    if (image.empty()) {
        QMessageBox::critical(this, "OCR Error", 
            "Cannot open OCR dialog: Input image is empty.");
        originalImage = cv::Mat(300, 400, CV_8UC3, cv::Scalar(50, 50, 50));
    } else {
        originalImage = image.clone();
    }
    
    // Setup UI first
    setupUI();
    
    // Initialize OCR engine after UI is ready
    try {
        if (!ocrEngine.initialize()) {
            QMessageBox::warning(this, "OCR Initialization", 
                QString("OCR engine initialization issue:\n%1\n\n"
                       "You can still preprocess the image, but text recognition may not work.")
                .arg(QString::fromStdString(ocrEngine.getLastError())));
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "OCR Error",
            QString("Exception during OCR initialization: %1").arg(e.what()));
    }
    
    updatePreview();
}

OCRDialog::~OCRDialog() {
}

// ============================================================================
// UI SETUP
// ============================================================================

void OCRDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    
    // Title
    QLabel* titleLabel = new QLabel("Optical Character Recognition");
    titleLabel->setStyleSheet(QString(
        "font-size: 18pt; font-weight: bold; color: %1; padding: 10px;")
        .arg(primaryColor));
    mainLayout->addWidget(titleLabel);
    
    // Main content area
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);
    
    // Left side - Preview and controls
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    // Preview
    QLabel* previewTitle = new QLabel("Image Preview");
    previewTitle->setStyleSheet(QString("color: %1; font-weight: bold;").arg(secondaryColor));
    leftLayout->addWidget(previewTitle);
    
    previewLabel = new QLabel();
    previewLabel->setMinimumSize(400, 300);
    previewLabel->setMaximumSize(500, 400);
    previewLabel->setScaledContents(true);
    previewLabel->setStyleSheet(QString(
        "border: 2px solid %1; border-radius: 8px; background-color: %2; padding: 5px;")
        .arg(primaryColor, panelColor));
    leftLayout->addWidget(previewLabel);
    
    // Preprocessing controls
    QGroupBox* preprocessGroup = new QGroupBox("Preprocessing Options");
    preprocessGroup->setStyleSheet(QString(
        "QGroupBox { color: %1; font-weight: bold; border: 2px solid %2; "
        "border-radius: 8px; margin-top: 10px; padding: 15px; "
        "background-color: %3; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }")
        .arg(secondaryColor, primaryColor, panelColor));
    
    QVBoxLayout* preprocessLayout = new QVBoxLayout(preprocessGroup);
    
    grayscaleCheck = new QCheckBox("Convert to Grayscale");
    grayscaleCheck->setChecked(true);
    grayscaleCheck->setStyleSheet(QString("color: %1;").arg(secondaryColor));
    connect(grayscaleCheck, &QCheckBox::toggled, this, &OCRDialog::onPreprocessChanged);
    preprocessLayout->addWidget(grayscaleCheck);
    
    thresholdCheck = new QCheckBox("Apply Adaptive Threshold");
    thresholdCheck->setChecked(true);
    thresholdCheck->setStyleSheet(QString("color: %1;").arg(secondaryColor));
    connect(thresholdCheck, &QCheckBox::toggled, this, &OCRDialog::onPreprocessChanged);
    preprocessLayout->addWidget(thresholdCheck);
    
    denoiseCheck = new QCheckBox("Denoise Image");
    denoiseCheck->setChecked(true);
    denoiseCheck->setStyleSheet(QString("color: %1;").arg(secondaryColor));
    connect(denoiseCheck, &QCheckBox::toggled, this, &OCRDialog::onPreprocessChanged);
    preprocessLayout->addWidget(denoiseCheck);
    
    contrastCheck = new QCheckBox("Enhance Contrast (CLAHE)");
    contrastCheck->setChecked(true);
    contrastCheck->setStyleSheet(QString("color: %1;").arg(secondaryColor));
    connect(contrastCheck, &QCheckBox::toggled, this, &OCRDialog::onPreprocessChanged);
    preprocessLayout->addWidget(contrastCheck);
    
    leftLayout->addWidget(preprocessGroup);
    leftLayout->addStretch();
    
    contentLayout->addLayout(leftLayout);
    
    // Right side - Results
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    // Results title and confidence
    QHBoxLayout* resultsHeaderLayout = new QHBoxLayout();
    QLabel* resultsTitle = new QLabel("Recognition Results");
    resultsTitle->setStyleSheet(QString("color: %1; font-weight: bold;").arg(secondaryColor));
    resultsHeaderLayout->addWidget(resultsTitle);
    resultsHeaderLayout->addStretch();
    
    confidenceLabel = new QLabel("Confidence: 0%");
    confidenceLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(primaryColor));
    resultsHeaderLayout->addWidget(confidenceLabel);
    
    rightLayout->addLayout(resultsHeaderLayout);
    
    // Confidence bar
    confidenceBar = new QProgressBar();
    confidenceBar->setRange(0, 100);
    confidenceBar->setValue(0);
    confidenceBar->setTextVisible(false);
    confidenceBar->setMaximumHeight(8);
    confidenceBar->setStyleSheet(QString(
        "QProgressBar { border: 1px solid %1; border-radius: 4px; background-color: %2; }"
        "QProgressBar::chunk { background-color: %3; }")
        .arg(primaryColor, panelColor, primaryColor));
    rightLayout->addWidget(confidenceBar);
    
    // Results text
    resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("Recognized text will appear here...");
    resultText->setStyleSheet(QString(
        "border: 2px solid %1; border-radius: 8px; background-color: %2; "
        "color: %3; padding: 10px; font-family: 'Courier New', monospace; font-size: 11pt;")
        .arg(primaryColor, panelColor, secondaryColor));
    rightLayout->addWidget(resultText);
    
    // Status label
    statusLabel = new QLabel("Ready to recognize text");
    statusLabel->setStyleSheet(QString("color: %1; font-style: italic; padding: 5px;").arg(secondaryColor));
    rightLayout->addWidget(statusLabel);
    
    contentLayout->addLayout(rightLayout);
    mainLayout->addLayout(contentLayout);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    recognizeButton = new QPushButton("Recognize Text");
    recognizeButton->setMinimumWidth(150);
    recognizeButton->setStyleSheet(QString(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 %1, stop:1 #c026d3); color: white; border: none; "
        "border-radius: 6px; padding: 10px 20px; font-weight: bold; }"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #f0abfc, stop:1 %1); }"
        "QPushButton:pressed { background: #a855f7; }")
        .arg(primaryColor));
    connect(recognizeButton, &QPushButton::clicked, this, &OCRDialog::onRecognizeClicked);
    buttonLayout->addWidget(recognizeButton);
    
    copyButton = new QPushButton("Copy Text");
    copyButton->setMinimumWidth(120);
    copyButton->setEnabled(false);
    copyButton->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: 2px solid %3; "
        "border-radius: 6px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: %3; }"
        "QPushButton:disabled { opacity: 0.5; }")
        .arg(panelColor, secondaryColor, primaryColor));
    connect(copyButton, &QPushButton::clicked, this, &OCRDialog::onCopyTextClicked);
    buttonLayout->addWidget(copyButton);
    
    saveButton = new QPushButton("Save Text");
    saveButton->setMinimumWidth(120);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: 2px solid %3; "
        "border-radius: 6px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: %3; }"
        "QPushButton:disabled { opacity: 0.5; }")
        .arg(panelColor, secondaryColor, primaryColor));
    connect(saveButton, &QPushButton::clicked, this, &OCRDialog::onSaveTextClicked);
    buttonLayout->addWidget(saveButton);
    
    exportButton = new QPushButton("Export with Boxes");
    exportButton->setMinimumWidth(150);
    exportButton->setEnabled(false);
    exportButton->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: 2px solid %3; "
        "border-radius: 6px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: %3; }"
        "QPushButton:disabled { opacity: 0.5; }")
        .arg(panelColor, secondaryColor, primaryColor));
    connect(exportButton, &QPushButton::clicked, this, &OCRDialog::onExportWithBoxesClicked);
    buttonLayout->addWidget(exportButton);
    
    closeButton = new QPushButton("Close");
    closeButton->setMinimumWidth(100);
    closeButton->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: 2px solid %2; "
        "border-radius: 6px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: #3a3448; }")
        .arg(panelColor, secondaryColor));
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Apply dialog stylesheet
    setStyleSheet(QString("QDialog { background-color: %1; }").arg(backgroundColor));
}

// ============================================================================
// PREVIEW UPDATE
// ============================================================================

void OCRDialog::updatePreview() {
    if (originalImage.empty()) return;
    
    // Safety check - ensure UI is initialized
    if (!grayscaleCheck || !thresholdCheck || !denoiseCheck || !contrastCheck) {
        return;
    }
    
    // Get preprocessing options from checkboxes
    TextRecognition::PreprocessOptions options;
    options.convertToGrayscale = grayscaleCheck->isChecked();
    options.applyThreshold = thresholdCheck->isChecked();
    options.denoiseImage = denoiseCheck->isChecked();
    options.enhanceContrast = contrastCheck->isChecked();
    
    try {
        // Apply preprocessing
        TextRecognition::preprocessImage(originalImage, preprocessedImage, options);
        
        // Convert to QImage for display
        QImage qImage;
        if (preprocessedImage.channels() == 3) {
            qImage = QImage(preprocessedImage.data, preprocessedImage.cols, preprocessedImage.rows,
                           static_cast<int>(preprocessedImage.step), QImage::Format_BGR888);
        } else {
            qImage = QImage(preprocessedImage.data, preprocessedImage.cols, preprocessedImage.rows,
                           static_cast<int>(preprocessedImage.step), QImage::Format_Grayscale8);
        }
        
        // Clone the QImage data to avoid dangling pointer issues
        QImage clonedImage = qImage.copy();
        previewLabel->setPixmap(QPixmap::fromImage(clonedImage));
        
    } catch (const std::exception& e) {
        qWarning() << "Error updating preview:" << e.what();
    }
}

void OCRDialog::onPreprocessChanged() {
    updatePreview();
}

// ============================================================================
// OCR RECOGNITION
// ============================================================================

void OCRDialog::onRecognizeClicked() {
    if (!ocrEngine.isInitialized()) {
        QMessageBox::warning(this, "OCR Not Ready", 
            "OCR engine is not initialized. Please check tessdata files.");
        return;
    }
    
    statusLabel->setText("Recognizing text...");
    statusLabel->setStyleSheet(QString("color: %1; font-style: italic;").arg(primaryColor));
    QApplication::processEvents();
    
    // Get preprocessing options
    TextRecognition::PreprocessOptions options;
    options.convertToGrayscale = grayscaleCheck->isChecked();
    options.applyThreshold = thresholdCheck->isChecked();
    options.denoiseImage = denoiseCheck->isChecked();
    options.enhanceContrast = contrastCheck->isChecked();
    
    // Perform OCR
    lastResult = ocrEngine.recognizeText(originalImage, options);
    
    if (lastResult.success) {
        displayResults(lastResult);
        statusLabel->setText(QString("Recognition complete! Found %1 words.")
            .arg(lastResult.words.size()));
        statusLabel->setStyleSheet(QString("color: #10b981; font-style: italic;"));
    } else {
        statusLabel->setText(QString("Recognition failed: %1")
            .arg(QString::fromStdString(lastResult.errorMessage)));
        statusLabel->setStyleSheet(QString("color: #ef4444; font-style: italic;"));
        QMessageBox::warning(this, "OCR Failed", 
            QString("Failed to recognize text:\n%1")
            .arg(QString::fromStdString(lastResult.errorMessage)));
    }
}

void OCRDialog::displayResults(const TextRecognition::OCRResult& result) {
    // Display text
    recognizedText = QString::fromStdString(result.text);
    resultText->setPlainText(recognizedText);
    
    // Update confidence
    confidence = result.confidence;
    confidenceLabel->setText(QString("Confidence: %1%").arg(static_cast<int>(confidence)));
    confidenceBar->setValue(static_cast<int>(confidence));
    
    // Enable action buttons
    copyButton->setEnabled(true);
    saveButton->setEnabled(true);
    exportButton->setEnabled(!result.boundingBoxes.empty());
}

// ============================================================================
// ACTIONS
// ============================================================================

void OCRDialog::onCopyTextClicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(recognizedText);
    
    statusLabel->setText("Text copied to clipboard!");
    statusLabel->setStyleSheet(QString("color: #10b981; font-style: italic;"));
}

void OCRDialog::onSaveTextClicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Text", "",
        "Text Files (*.txt);;All Files (*)");
    
    if (filename.isEmpty()) return;
    
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << recognizedText;
        file.close();
        
        statusLabel->setText(QString("Text saved to %1").arg(filename));
        statusLabel->setStyleSheet(QString("color: #10b981; font-style: italic;"));
    } else {
        QMessageBox::warning(this, "Save Failed", "Could not save file.");
    }
}

void OCRDialog::onExportWithBoxesClicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Export Image with Bounding Boxes", "",
        "PNG Images (*.png);;JPEG Images (*.jpg);;All Files (*)");
    
    if (filename.isEmpty()) return;
    
    // Create image with bounding boxes
    cv::Mat outputImage = originalImage.clone();
    drawBoundingBoxes(outputImage, lastResult.boundingBoxes);
    
    // Save image
    if (cv::imwrite(filename.toStdString(), outputImage)) {
        statusLabel->setText(QString("Image exported to %1").arg(filename));
        statusLabel->setStyleSheet(QString("color: #10b981; font-style: italic;"));
    } else {
        QMessageBox::warning(this, "Export Failed", "Could not save image.");
    }
}

void OCRDialog::drawBoundingBoxes(cv::Mat& image, const std::vector<cv::Rect>& boxes) {
    for (const auto& box : boxes) {
        cv::rectangle(image, box, cv::Scalar(232, 121, 249), 2);  // Fuchsia color
    }
}

// ============================================================================
// GETTERS
// ============================================================================

QString OCRDialog::getRecognizedText() const {
    return recognizedText;
}

float OCRDialog::getConfidence() const {
    return confidence;
}
