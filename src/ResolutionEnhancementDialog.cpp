#include "ResolutionEnhancementDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QMessageBox>

ResolutionEnhancementDialog::ResolutionEnhancementDialog(const cv::Mat& inputImage, QWidget* parent)
    : QDialog(parent), sourceImage(inputImage.clone()), currentMethod(Bicubic) {
    
    setWindowTitle("Resolution Enhancement / Upscaling");
    setModal(true);
    setMinimumSize(700, 600);
    
    setupUI();
    updatePreview();
}

void ResolutionEnhancementDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    
    // Title Section
    QLabel* titleLabel = new QLabel("🔍 Resolution Enhancement & Upscaling");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #89B4FA; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Current size info
    QGroupBox* infoGroup = new QGroupBox("📊 Current Image");
    infoGroup->setStyleSheet(
        "QGroupBox { "
        "  font-weight: bold; "
        "  font-size: 11pt; "
        "  padding-top: 20px; "
        "  background: #2A2A3E; "
        "  border: 2px solid #6C7086; "
        "  border-radius: 8px; "
        "  margin-bottom: 5px; "
        "} "
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  left: 12px; "
        "  padding: 0 8px; "
        "  color: #89B4FA; "
        "}"
    );
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
    infoLayout->setContentsMargins(15, 20, 15, 15);
    
    QString currentInfo = QString("📐 %1 x %2 pixels (%3 MP)")
        .arg(sourceImage.cols)
        .arg(sourceImage.rows)
        .arg(QString::number(sourceImage.cols * sourceImage.rows / 1000000.0, 'f', 2));
    
    QLabel* currentSizeLabel = new QLabel(currentInfo);
    currentSizeLabel->setStyleSheet("font-weight: 600; font-size: 11pt; color: #CDD6F4; padding: 8px; background: #1E1E2E; border-radius: 6px;");
    infoLayout->addWidget(currentSizeLabel);
    
    mainLayout->addWidget(infoGroup);
    
    // Scale Factor Control
    QGroupBox* scaleGroup = new QGroupBox("📏 Upscaling Factor");
    scaleGroup->setStyleSheet(
        "QGroupBox { "
        "  font-weight: bold; "
        "  font-size: 11pt; "
        "  padding-top: 20px; "
        "  background: #2A2A3E; "
        "  border: 2px solid #6C7086; "
        "  border-radius: 8px; "
        "} "
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  left: 12px; "
        "  padding: 0 8px; "
        "  color: #89B4FA; "
        "}"
    );
    QVBoxLayout* scaleLayout = new QVBoxLayout(scaleGroup);
    scaleLayout->setSpacing(12);
    scaleLayout->setContentsMargins(15, 20, 15, 15);
    
    QHBoxLayout* scaleSliderLayout = new QHBoxLayout();
    scaleSliderLayout->setSpacing(15);
    
    scaleLabel = new QLabel("Scale Factor:");
    scaleLabel->setStyleSheet("font-size: 10pt; min-width: 100px;");
    
    scaleSlider = new QSlider(Qt::Horizontal);
    scaleSlider->setMinimum(100);   // 1.0x (no change)
    scaleSlider->setMaximum(400);   // 4.0x
    scaleSlider->setValue(200);      // 2.0x default
    scaleSlider->setTickPosition(QSlider::TicksBelow);
    scaleSlider->setTickInterval(50);
    scaleSlider->setMinimumHeight(30);
    
    scaleSpinBox = new QSpinBox();
    scaleSpinBox->setMinimum(100);
    scaleSpinBox->setMaximum(400);
    scaleSpinBox->setValue(200);
    scaleSpinBox->setSuffix("%");
    scaleSpinBox->setSingleStep(10);
    scaleSpinBox->setMinimumWidth(80);
    scaleSpinBox->setMinimumHeight(35);
    scaleSpinBox->setStyleSheet("font-size: 11pt; font-weight: bold; padding: 5px;");
    
    scaleSliderLayout->addWidget(scaleLabel);
    scaleSliderLayout->addWidget(scaleSlider, 1);
    scaleSliderLayout->addWidget(scaleSpinBox);
    
    scaleLayout->addLayout(scaleSliderLayout);
    
    targetSizeLabel = new QLabel();
    targetSizeLabel->setStyleSheet(
        "color: #A6E3A1; "
        "font-size: 11pt; "
        "font-weight: 700; "
        "padding: 10px 15px; "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1E1E2E, stop:1 #2A2A3E); "
        "border: 2px solid #A6E3A1; "
        "border-radius: 6px; "
        "margin-top: 8px;"
    );
    scaleLayout->addWidget(targetSizeLabel);
    
    QLabel* scaleHint = new QLabel("⚠️ 2x = 4x memory, 3x = 9x memory, 4x = 16x memory usage!");
    scaleHint->setStyleSheet("color: #F9E2AF; font-size: 9pt; padding: 8px; margin-top: 5px;");
    scaleLayout->addWidget(scaleHint);
    
    mainLayout->addWidget(scaleGroup);
    
    // Interpolation Method
    QGroupBox* methodGroup = new QGroupBox("🎨 Interpolation Algorithm");
    methodGroup->setStyleSheet(
        "QGroupBox { "
        "  font-weight: bold; "
        "  font-size: 11pt; "
        "  padding-top: 20px; "
        "  background: #2A2A3E; "
        "  border: 2px solid #6C7086; "
        "  border-radius: 8px; "
        "} "
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  left: 12px; "
        "  padding: 0 8px; "
        "  color: #89B4FA; "
        "}"
    );
    QVBoxLayout* methodLayout = new QVBoxLayout(methodGroup);
    methodLayout->setSpacing(12);
    methodLayout->setContentsMargins(15, 20, 15, 15);
    
    QHBoxLayout* methodComboLayout = new QHBoxLayout();
    methodComboLayout->setSpacing(15);
    
    QLabel* methodLabel = new QLabel("Method:");
    methodLabel->setStyleSheet("font-size: 10pt; min-width: 80px;");
    
    methodCombo = new QComboBox();
    methodCombo->addItem("⚡ Nearest Neighbor (Fastest)", Nearest);
    methodCombo->addItem("🐎 Bilinear (Fast & Smooth)", Bilinear);
    methodCombo->addItem("✨ Bicubic (Recommended)", Bicubic);
    methodCombo->addItem("🎯 Lanczos-4 (Best Quality)", Lanczos4);
    methodCombo->addItem("🔪 Edge-Directed (Preserves Details)", EdgeDirected);
    methodCombo->setCurrentIndex(2);  // Bicubic default
    methodCombo->setMinimumHeight(38);
    methodCombo->setStyleSheet("font-size: 11pt; padding: 8px;");
    
    methodComboLayout->addWidget(methodLabel);
    methodComboLayout->addWidget(methodCombo, 1);
    methodLayout->addLayout(methodComboLayout);
    
    // Method descriptions
    QLabel* methodDesc = new QLabel(
        "<div style='line-height: 170%;'>"
        "<p style='margin: 4px 0;'><b style='color: #89B4FA;'>⚡ Nearest:</b> <span style='color: #A8A8A8;'>Fastest, blocky pixels (pixel art)</span></p>"
        "<p style='margin: 4px 0;'><b style='color: #89B4FA;'>🐎 Bilinear:</b> <span style='color: #A8A8A8;'>Fast & smooth, slight blur (web images)</span></p>"
        "<p style='margin: 4px 0;'><b style='color: #89B4FA;'>✨ Bicubic:</b> <span style='color: #A8A8A8;'>Best balance of speed & quality (photos)</span></p>"
        "<p style='margin: 4px 0;'><b style='color: #89B4FA;'>🎯 Lanczos-4:</b> <span style='color: #A8A8A8;'>Sharpest results, slower (print quality)</span></p>"
        "<p style='margin: 4px 0;'><b style='color: #89B4FA;'>🔪 Edge-Directed:</b> <span style='color: #A8A8A8;'>Preserves edges & text (technical drawings)</span></p>"
        "</div>"
    );
    methodDesc->setStyleSheet(
        "color: #A8A8A8; "
        "font-size: 9pt; "
        "padding: 12px; "
        "background: #1E1E2E; "
        "border: 1px solid #45475A; "
        "border-radius: 6px; "
        "margin-top: 8px;"
    );
    methodLayout->addWidget(methodDesc);
    
    mainLayout->addWidget(methodGroup);
    
    // Sharpening Pass
    QGroupBox* sharpenGroup = new QGroupBox("✨ Post-Processing Sharpening");
    sharpenGroup->setStyleSheet(
        "QGroupBox { "
        "  font-weight: bold; "
        "  font-size: 11pt; "
        "  padding-top: 20px; "
        "  background: #2A2A3E; "
        "  border: 2px solid #6C7086; "
        "  border-radius: 8px; "
        "} "
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  left: 12px; "
        "  padding: 0 8px; "
        "  color: #89B4FA; "
        "}"
    );
    QVBoxLayout* sharpenLayout = new QVBoxLayout(sharpenGroup);
    sharpenLayout->setSpacing(10);
    sharpenLayout->setContentsMargins(15, 20, 15, 15);
    
    QHBoxLayout* sharpenSliderLayout = new QHBoxLayout();
    sharpenSliderLayout->setSpacing(15);
    
    sharpenLabel = new QLabel("Sharpen:");
    sharpenLabel->setStyleSheet("font-size: 10pt; min-width: 80px;");
    
    sharpenSlider = new QSlider(Qt::Horizontal);
    sharpenSlider->setMinimum(0);
    sharpenSlider->setMaximum(100);
    sharpenSlider->setValue(30);
    sharpenSlider->setTickPosition(QSlider::TicksBelow);
    sharpenSlider->setTickInterval(10);
    sharpenSlider->setMinimumHeight(30);
    
    sharpenSpinBox = new QSpinBox();
    sharpenSpinBox->setMinimum(0);
    sharpenSpinBox->setMaximum(100);
    sharpenSpinBox->setValue(30);
    sharpenSpinBox->setSuffix("%");
    sharpenSpinBox->setMinimumWidth(80);
    sharpenSpinBox->setMinimumHeight(35);
    sharpenSpinBox->setStyleSheet("font-size: 11pt; padding: 5px;");
    
    sharpenSliderLayout->addWidget(sharpenLabel);
    sharpenSliderLayout->addWidget(sharpenSlider, 1);
    sharpenSliderLayout->addWidget(sharpenSpinBox);
    
    sharpenLayout->addLayout(sharpenSliderLayout);
    
    QLabel* sharpenHint = new QLabel("🔪 Unsharp mask filter to restore fine details. 0% = none, 100% = maximum");
    sharpenHint->setStyleSheet("color: #A8A8A8; font-size: 9pt; padding: 5px;");
    sharpenLayout->addWidget(sharpenHint);
    
    mainLayout->addWidget(sharpenGroup);
    
    // Preview
    QGroupBox* previewGroup = new QGroupBox("🔍 Live Preview (Scaled for Display)");
    previewGroup->setStyleSheet(
        "QGroupBox { "
        "  font-weight: bold; "
        "  font-size: 11pt; "
        "  padding-top: 20px; "
        "  background: #2A2A3E; "
        "  border: 2px solid #89B4FA; "
        "  border-radius: 8px; "
        "} "
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  left: 12px; "
        "  padding: 0 8px; "
        "  color: #89B4FA; "
        "}"
    );
    QVBoxLayout* previewLayout = new QVBoxLayout(previewGroup);
    previewLayout->setContentsMargins(15, 20, 15, 15);
    
    previewLabel = new QLabel("Generating preview...");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setMinimumSize(450, 320);
    previewLabel->setMaximumSize(650, 480);
    previewLabel->setScaledContents(false);
    previewLabel->setStyleSheet("QLabel { background: #1A1A1A; border: 2px solid #89B4FA; border-radius: 8px; padding: 10px; }");
    
    previewLayout->addWidget(previewLabel);
    mainLayout->addWidget(previewGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    buttonLayout->setContentsMargins(0, 15, 0, 0);
    
    applyButton = new QPushButton("✓ Apply Enhancement");
    applyButton->setMinimumSize(160, 42);
    applyButton->setStyleSheet(
        "QPushButton { "
        "  background: #89B4FA; "
        "  color: #1E1E2E; "
        "  border: none; "
        "  border-radius: 6px; "
        "  font-size: 11pt; "
        "  font-weight: bold; "
        "  padding: 12px 24px; "
        "} "
        "QPushButton:hover { background: #A5C4FF; } "
        "QPushButton:pressed { background: #6A9FE8; }"
    );
    
    cancelButton = new QPushButton("✕ Cancel");
    cancelButton->setMinimumSize(120, 42);
    cancelButton->setStyleSheet(
        "QPushButton { "
        "  background: #45475A; "
        "  color: #CDD6F4; "
        "  border: 2px solid #6C7086; "
        "  border-radius: 6px; "
        "  font-size: 11pt; "
        "  padding: 12px 24px; "
        "} "
        "QPushButton:hover { background: #585B70; } "
        "QPushButton:pressed { background: #313244; }"
    );
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Set dialog background
    setStyleSheet("QDialog { background-color: #1E1E2E; }");
    
    // Connect signals
    connect(scaleSlider, &QSlider::valueChanged, this, &ResolutionEnhancementDialog::onScaleChanged);
    connect(scaleSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            scaleSlider, &QSlider::setValue);
    
    connect(methodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ResolutionEnhancementDialog::onMethodChanged);
    
    connect(sharpenSlider, &QSlider::valueChanged, this, &ResolutionEnhancementDialog::onSharpenChanged);
    connect(sharpenSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            sharpenSlider, &QSlider::setValue);
    
    connect(applyButton, &QPushButton::clicked, this, &ResolutionEnhancementDialog::onApply);
    connect(cancelButton, &QPushButton::clicked, this, &ResolutionEnhancementDialog::onCancel);
}

void ResolutionEnhancementDialog::onScaleChanged(int value) {
    scaleSpinBox->setValue(value);
    
    double scale = value / 100.0;
    int newWidth = static_cast<int>(sourceImage.cols * scale);
    int newHeight = static_cast<int>(sourceImage.rows * scale);
    double newMP = (newWidth * newHeight) / 1000000.0;
    
    QString targetInfo = QString("Target Size: %1 x %2 pixels (%3 MP)")
        .arg(newWidth)
        .arg(newHeight)
        .arg(QString::number(newMP, 'f', 2));
    
    targetSizeLabel->setText(targetInfo);
    
    updatePreview();
}

void ResolutionEnhancementDialog::onMethodChanged(int index) {
    currentMethod = static_cast<InterpolationMethod>(methodCombo->itemData(index).toInt());
    updatePreview();
}

void ResolutionEnhancementDialog::onSharpenChanged(int value) {
    sharpenSpinBox->setValue(value);
    updatePreview();
}

void ResolutionEnhancementDialog::updatePreview() {
    if (sourceImage.empty()) return;
    
    enhanceResolution();
    
    // Convert cv::Mat to QPixmap for display
    cv::Mat displayImage;
    if (resultImage.channels() == 1) {
        cv::cvtColor(resultImage, displayImage, cv::COLOR_GRAY2BGR);
    } else {
        displayImage = resultImage.clone();
    }
    
    // Scale down for preview if too large
    int maxSize = 600;
    double scale = 1.0;
    if (displayImage.cols > maxSize || displayImage.rows > maxSize) {
        scale = std::min((double)maxSize / displayImage.cols, 
                        (double)maxSize / displayImage.rows);
        cv::resize(displayImage, displayImage, cv::Size(), scale, scale, cv::INTER_AREA);
    }
    
    // Convert BGR to RGB
    cv::cvtColor(displayImage, displayImage, cv::COLOR_BGR2RGB);
    
    // Create QImage
    QImage qImage(displayImage.data, displayImage.cols, displayImage.rows,
                 displayImage.step, QImage::Format_RGB888);
    
    // Display
    previewLabel->setPixmap(QPixmap::fromImage(qImage.copy()));
}

void ResolutionEnhancementDialog::enhanceResolution() {
    if (sourceImage.empty()) return;
    
    double scale = scaleSlider->value() / 100.0;
    
    if (scale == 1.0) {
        resultImage = sourceImage.clone();
        return;
    }
    
    cv::Size newSize(
        static_cast<int>(sourceImage.cols * scale),
        static_cast<int>(sourceImage.rows * scale)
    );
    
    try {
        switch (currentMethod) {
            case Nearest:
                cv::resize(sourceImage, resultImage, newSize, 0, 0, cv::INTER_NEAREST);
                break;
                
            case Bilinear:
                cv::resize(sourceImage, resultImage, newSize, 0, 0, cv::INTER_LINEAR);
                break;
                
            case Bicubic:
                cv::resize(sourceImage, resultImage, newSize, 0, 0, cv::INTER_CUBIC);
                break;
                
            case Lanczos4:
                cv::resize(sourceImage, resultImage, newSize, 0, 0, cv::INTER_LANCZOS4);
                break;
                
            case EdgeDirected: {
                // Edge-directed interpolation: detect edges and interpolate carefully
                cv::Mat edges;
                if (sourceImage.channels() == 1) {
                    cv::Canny(sourceImage, edges, 50, 150);
                } else {
                    cv::Mat gray;
                    cv::cvtColor(sourceImage, gray, cv::COLOR_BGR2GRAY);
                    cv::Canny(gray, edges, 50, 150);
                }
                
                // Use Lanczos for base, then preserve edges
                cv::resize(sourceImage, resultImage, newSize, 0, 0, cv::INTER_LANCZOS4);
                
                // Apply edge-preserving filter
                if (resultImage.channels() == 3) {
                    cv::edgePreservingFilter(resultImage, resultImage, cv::RECURS_FILTER, 60, 0.4);
                }
                break;
            }
        }
        
        // Apply sharpening pass if enabled
        int sharpenStrength = sharpenSlider->value();
        if (sharpenStrength > 0) {
            applySharpeningPass(resultImage, sharpenStrength);
        }
        
    } catch (const cv::Exception& e) {
        qDebug() << "OpenCV error in resolution enhancement:" << e.what();
        resultImage = sourceImage.clone();
    }
}

void ResolutionEnhancementDialog::applySharpeningPass(cv::Mat& image, int strength) {
    if (image.empty() || strength == 0) return;
    
    // Unsharp mask technique
    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(0, 0), 1.5);
    
    double amount = strength / 100.0;  // Convert to 0.0-1.0 range
    cv::addWeighted(image, 1.0 + amount, blurred, -amount, 0, image);
}

void ResolutionEnhancementDialog::onApply() {
    enhanceResolution();  // Ensure final result is computed
    
    if (resultImage.empty()) {
        QMessageBox::warning(this, "Error", "Failed to enhance resolution!");
        return;
    }
    
    // Check if result is too large
    long long totalPixels = (long long)resultImage.cols * resultImage.rows;
    if (totalPixels > 100000000) {  // 100 megapixels
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "Large Image Warning",
            QString("The resulting image will be %1 MP (%2 x %3 pixels).\n\n"
                    "This may consume significant memory and take time to process.\n\n"
                    "Continue anyway?")
                .arg(QString::number(totalPixels / 1000000.0, 'f', 1))
                .arg(resultImage.cols)
                .arg(resultImage.rows),
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply == QMessageBox::No) {
            return;
        }
    }
    
    accept();
}

void ResolutionEnhancementDialog::onCancel() {
    reject();
}
