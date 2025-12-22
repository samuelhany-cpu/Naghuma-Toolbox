#include "BlurDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPixmap>
#include <QImage>

BlurDialog::BlurDialog(const cv::Mat& inputImage, QWidget* parent)
    : QDialog(parent), sourceImage(inputImage.clone()), currentBlurType(Gaussian) {
    
    setWindowTitle("Blur/Smoothing Filters");
    setModal(true);
    setMinimumSize(600, 500);
    
    setupUI();
    updatePreview();
}

void BlurDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    
    // Title Section
    QLabel* titleLabel = new QLabel("Blur & Smoothing Filters");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #89B4FA; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Blur Type Selection
    QGroupBox* typeGroup = new QGroupBox("Filter Type");
    typeGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; padding-top: 15px; }");
    QVBoxLayout* typeLayout = new QVBoxLayout(typeGroup);
    typeLayout->setSpacing(12);
    typeLayout->setContentsMargins(15, 20, 15, 15);
    
    blurTypeGroup = new QButtonGroup(this);
    gaussianRadio = new QRadioButton("🔵 Gaussian Blur");
    medianRadio = new QRadioButton("🔷 Median Filter");
    bilateralRadio = new QRadioButton("✨ Bilateral Filter");
    
    // Style radio buttons
    QString radioStyle = "QRadioButton { font-size: 11pt; padding: 8px; spacing: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }";
    gaussianRadio->setStyleSheet(radioStyle);
    medianRadio->setStyleSheet(radioStyle);
    bilateralRadio->setStyleSheet(radioStyle);
    
    blurTypeGroup->addButton(gaussianRadio, Gaussian);
    blurTypeGroup->addButton(medianRadio, Median);
    blurTypeGroup->addButton(bilateralRadio, Bilateral);
    
    gaussianRadio->setChecked(true);
    
    typeLayout->addWidget(gaussianRadio);
    QLabel* gaussianDesc = new QLabel("   Smooth, general-purpose blur for all images");
    gaussianDesc->setStyleSheet("color: #A8A8A8; font-size: 9pt; margin-left: 30px; margin-bottom: 8px;");
    typeLayout->addWidget(gaussianDesc);
    
    typeLayout->addWidget(medianRadio);
    QLabel* medianDesc = new QLabel("   Removes salt & pepper noise, preserves edges");
    medianDesc->setStyleSheet("color: #A8A8A8; font-size: 9pt; margin-left: 30px; margin-bottom: 8px;");
    typeLayout->addWidget(medianDesc);
    
    typeLayout->addWidget(bilateralRadio);
    QLabel* bilateralDesc = new QLabel("   Edge-preserving smoothing, best for portraits");
    bilateralDesc->setStyleSheet("color: #A8A8A8; font-size: 9pt; margin-left: 30px;");
    typeLayout->addWidget(bilateralDesc);
    
    mainLayout->addWidget(typeGroup);
    
    // Kernel Size Control
    QGroupBox* kernelGroup = new QGroupBox("Blur Intensity");
    kernelGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; padding-top: 15px; }");
    QVBoxLayout* kernelLayout = new QVBoxLayout(kernelGroup);
    kernelLayout->setSpacing(10);
    kernelLayout->setContentsMargins(15, 20, 15, 15);
    
    QHBoxLayout* kernelSliderLayout = new QHBoxLayout();
    kernelSliderLayout->setSpacing(15);
    
    kernelSizeLabel = new QLabel("Kernel Size:");
    kernelSizeLabel->setStyleSheet("font-size: 10pt; min-width: 100px;");
    
    kernelSizeSlider = new QSlider(Qt::Horizontal);
    kernelSizeSlider->setMinimum(1);
    kernelSizeSlider->setMaximum(31);
    kernelSizeSlider->setValue(5);
    kernelSizeSlider->setTickPosition(QSlider::TicksBelow);
    kernelSizeSlider->setTickInterval(2);
    kernelSizeSlider->setMinimumHeight(30);
    
    kernelSizeSpinBox = new QSpinBox();
    kernelSizeSpinBox->setMinimum(1);
    kernelSizeSpinBox->setMaximum(31);
    kernelSizeSpinBox->setValue(5);
    kernelSizeSpinBox->setSingleStep(2);
    kernelSizeSpinBox->setMinimumWidth(70);
    kernelSizeSpinBox->setMinimumHeight(32);
    kernelSizeSpinBox->setStyleSheet("font-size: 11pt; padding: 5px;");
    
    kernelSliderLayout->addWidget(kernelSizeLabel);
    kernelSliderLayout->addWidget(kernelSizeSlider, 1);
    kernelSliderLayout->addWidget(kernelSizeSpinBox);
    
    kernelLayout->addLayout(kernelSliderLayout);
    
    QLabel* kernelHint = new QLabel("💡 Larger values = stronger blur. Must be odd number (1, 3, 5, 7...)");
    kernelHint->setStyleSheet("color: #A8A8A8; font-size: 9pt; padding: 5px; margin-top: 5px;");
    kernelLayout->addWidget(kernelHint);
    
    mainLayout->addWidget(kernelGroup);
    
    // Bilateral-specific controls
    QGroupBox* bilateralGroup = new QGroupBox("Advanced Bilateral Settings");
    bilateralGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; padding-top: 15px; }");
    QVBoxLayout* bilateralLayout = new QVBoxLayout(bilateralGroup);
    bilateralLayout->setSpacing(15);
    bilateralLayout->setContentsMargins(15, 20, 15, 15);
    
    // Sigma Color
    QHBoxLayout* sigmaColorLayout = new QHBoxLayout();
    sigmaColorLayout->setSpacing(15);
    
    sigmaColorLabel = new QLabel("Color Sigma:");
    sigmaColorLabel->setStyleSheet("font-size: 10pt; min-width: 100px;");
    
    sigmaColorSlider = new QSlider(Qt::Horizontal);
    sigmaColorSlider->setMinimum(10);
    sigmaColorSlider->setMaximum(200);
    sigmaColorSlider->setValue(75);
    sigmaColorSlider->setTickPosition(QSlider::TicksBelow);
    sigmaColorSlider->setTickInterval(20);
    sigmaColorSlider->setMinimumHeight(30);
    
    sigmaColorSpinBox = new QSpinBox();
    sigmaColorSpinBox->setMinimum(10);
    sigmaColorSpinBox->setMaximum(200);
    sigmaColorSpinBox->setValue(75);
    sigmaColorSpinBox->setMinimumWidth(70);
    sigmaColorSpinBox->setMinimumHeight(32);
    sigmaColorSpinBox->setStyleSheet("font-size: 11pt; padding: 5px;");
    
    sigmaColorLayout->addWidget(sigmaColorLabel);
    sigmaColorLayout->addWidget(sigmaColorSlider, 1);
    sigmaColorLayout->addWidget(sigmaColorSpinBox);
    bilateralLayout->addLayout(sigmaColorLayout);
    
    QLabel* sigmaColorHint = new QLabel("🎨 Controls color blending strength. Higher = blend more colors together");
    sigmaColorHint->setStyleSheet("color: #A8A8A8; font-size: 9pt; padding: 5px; margin-bottom: 10px;");
    bilateralLayout->addWidget(sigmaColorHint);
    
    // Sigma Space
    QHBoxLayout* sigmaSpaceLayout = new QHBoxLayout();
    sigmaSpaceLayout->setSpacing(15);
    
    sigmaSpaceLabel = new QLabel("Space Sigma:");
    sigmaSpaceLabel->setStyleSheet("font-size: 10pt; min-width: 100px;");
    
    sigmaSpaceSlider = new QSlider(Qt::Horizontal);
    sigmaSpaceSlider->setMinimum(10);
    sigmaSpaceSlider->setMaximum(200);
    sigmaSpaceSlider->setValue(75);
    sigmaSpaceSlider->setTickPosition(QSlider::TicksBelow);
    sigmaSpaceSlider->setTickInterval(20);
    sigmaSpaceSlider->setMinimumHeight(30);
    
    sigmaSpaceSpinBox = new QSpinBox();
    sigmaSpaceSpinBox->setMinimum(10);
    sigmaSpaceSpinBox->setMaximum(200);
    sigmaSpaceSpinBox->setValue(75);
    sigmaSpaceSpinBox->setMinimumWidth(70);
    sigmaSpaceSpinBox->setMinimumHeight(32);
    sigmaSpaceSpinBox->setStyleSheet("font-size: 11pt; padding: 5px;");
    
    sigmaSpaceLayout->addWidget(sigmaSpaceLabel);
    sigmaSpaceLayout->addWidget(sigmaSpaceSlider, 1);
    sigmaSpaceLayout->addWidget(sigmaSpaceSpinBox);
    bilateralLayout->addLayout(sigmaSpaceLayout);
    
    QLabel* sigmaSpaceHint = new QLabel("📏 Controls spatial influence range. Higher = consider distant pixels");
    sigmaSpaceHint->setStyleSheet("color: #A8A8A8; font-size: 9pt; padding: 5px;");
    bilateralLayout->addWidget(sigmaSpaceHint);
    
    mainLayout->addWidget(bilateralGroup);
    bilateralGroup->setVisible(false);  // Hidden by default
    
    // Preview
    QGroupBox* previewGroup = new QGroupBox("Live Preview");
    previewGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 11pt; padding-top: 15px; }");
    QVBoxLayout* previewLayout = new QVBoxLayout(previewGroup);
    previewLayout->setContentsMargins(15, 20, 15, 15);
    
    previewLabel = new QLabel("Processing preview...");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setMinimumSize(450, 320);
    previewLabel->setMaximumSize(600, 450);
    previewLabel->setScaledContents(false);
    previewLabel->setStyleSheet("QLabel { background: #1A1A1A; border: 2px solid #89B4FA; border-radius: 8px; padding: 10px; }");
    
    previewLayout->addWidget(previewLabel);
    mainLayout->addWidget(previewGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    buttonLayout->setContentsMargins(0, 15, 0, 0);
    
    applyButton = new QPushButton("✓ Apply Blur");
    applyButton->setMinimumSize(140, 40);
    applyButton->setStyleSheet(
        "QPushButton { "
        "  background: #89B4FA; "
        "  color: #1E1E2E; "
        "  border: none; "
        "  border-radius: 6px; "
        "  font-size: 11pt; "
        "  font-weight: bold; "
        "  padding: 10px 20px; "
        "} "
        "QPushButton:hover { background: #A5C4FF; } "
        "QPushButton:pressed { background: #6A9FE8; }"
    );
    
    cancelButton = new QPushButton("✕ Cancel");
    cancelButton->setMinimumSize(120, 40);
    cancelButton->setStyleSheet(
        "QPushButton { "
        "  background: #45475A; "
        "  color: #CDD6F4; "
        "  border: 2px solid #6C7086; "
        "  border-radius: 6px; "
        "  font-size: 11pt; "
        "  padding: 10px 20px; "
        "} "
        "QPushButton:hover { background: #585B70; } "
        "QPushButton:pressed { background: #313244; }"
    );
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(blurTypeGroup, &QButtonGroup::idClicked, 
            this, &BlurDialog::onBlurTypeChanged);
    
    connect(kernelSizeSlider, &QSlider::valueChanged, this, &BlurDialog::onKernelSizeChanged);
    connect(kernelSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            kernelSizeSlider, &QSlider::setValue);
    
    connect(sigmaColorSlider, &QSlider::valueChanged, this, &BlurDialog::onSigmaColorChanged);
    connect(sigmaColorSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            sigmaColorSlider, &QSlider::setValue);
    
    connect(sigmaSpaceSlider, &QSlider::valueChanged, this, &BlurDialog::onSigmaSpaceChanged);
    connect(sigmaSpaceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            sigmaSpaceSlider, &QSlider::setValue);
    
    connect(applyButton, &QPushButton::clicked, this, &BlurDialog::onApply);
    connect(cancelButton, &QPushButton::clicked, this, &BlurDialog::onCancel);
    
    // Store bilateral group for showing/hiding
    bilateralGroup->setObjectName("bilateralGroup");
}

void BlurDialog::onBlurTypeChanged() {
    int id = blurTypeGroup->checkedId();
    currentBlurType = static_cast<BlurType>(id);
    
    // Show/hide bilateral controls
    QGroupBox* bilateralGroup = findChild<QGroupBox*>("bilateralGroup");
    if (bilateralGroup) {
        bilateralGroup->setVisible(currentBlurType == Bilateral);
    }
    
    updatePreview();
}

void BlurDialog::onKernelSizeChanged(int value) {
    // Ensure odd number
    if (value % 2 == 0) {
        value++;
        kernelSizeSlider->setValue(value);
    }
    kernelSizeSpinBox->setValue(value);
    updatePreview();
}

void BlurDialog::onSigmaColorChanged(int value) {
    sigmaColorSpinBox->setValue(value);
    if (currentBlurType == Bilateral) {
        updatePreview();
    }
}

void BlurDialog::onSigmaSpaceChanged(int value) {
    sigmaSpaceSpinBox->setValue(value);
    if (currentBlurType == Bilateral) {
        updatePreview();
    }
}

void BlurDialog::updatePreview() {
    if (sourceImage.empty()) return;
    
    applyBlur();
    
    // Convert cv::Mat to QPixmap for display
    cv::Mat displayImage;
    if (resultImage.channels() == 1) {
        cv::cvtColor(resultImage, displayImage, cv::COLOR_GRAY2BGR);
    } else {
        displayImage = resultImage.clone();
    }
    
    // Scale down for preview if too large
    int maxSize = 400;
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

void BlurDialog::applyBlur() {
    if (sourceImage.empty()) return;
    
    int kernelSize = kernelSizeSlider->value();
    // Ensure odd number
    if (kernelSize % 2 == 0) kernelSize++;
    
    try {
        switch (currentBlurType) {
            case Gaussian:
                cv::GaussianBlur(sourceImage, resultImage, 
                               cv::Size(kernelSize, kernelSize), 0);
                break;
                
            case Median:
                cv::medianBlur(sourceImage, resultImage, kernelSize);
                break;
                
            case Bilateral: {
                int diameter = kernelSize;
                double sigmaColor = sigmaColorSlider->value();
                double sigmaSpace = sigmaSpaceSlider->value();
                cv::bilateralFilter(sourceImage, resultImage, 
                                   diameter, sigmaColor, sigmaSpace);
                break;
            }
        }
    } catch (const cv::Exception& e) {
        qDebug() << "OpenCV error in blur:" << e.what();
        resultImage = sourceImage.clone();
    }
}

void BlurDialog::onApply() {
    applyBlur();  // Ensure final result is computed
    accept();
}

void BlurDialog::onCancel() {
    reject();
}
