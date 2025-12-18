#include "ThresholdingDialog.h"
#include "ImageCanvas.h"
#include "ImageProcessor.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

ThresholdingDialog::ThresholdingDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent),
      originalImage(image.clone()),
      applied(false),
      thresholdValue(127),
      maxValue(255),
      blockSize(11),
      cConstant(2),
      otsuLevels(2),
      computedThreshold(0.0) {
    
    setWindowTitle("Image Thresholding");
    setMinimumSize(1200, 800);
    setStyleSheet(Theme::MAIN_STYLESHEET);
    
    setupUI();
    updatePreview();
}

ThresholdingDialog::~ThresholdingDialog() {
}

void ThresholdingDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Image Thresholding & Segmentation");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Threshold type selection
    QHBoxLayout *typeLayout = new QHBoxLayout();
    QLabel *typeLabel = new QLabel("Threshold Type:");
    typeLabel->setStyleSheet("color: #c4b5fd; font-size: 11pt;");
    typeLayout->addWidget(typeLabel);
    
    thresholdTypeCombo = new QComboBox();
    thresholdTypeCombo->addItems({
        "Simple Binary Threshold",
        "Adaptive Threshold (Mean)",
        "Adaptive Threshold (Gaussian)",
        "Otsu's Auto Threshold",
        "Multi-Level Otsu (3-4 levels)",
        "Local Threshold"
    });
    thresholdTypeCombo->setMinimumWidth(250);
    connect(thresholdTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThresholdingDialog::onThresholdTypeChanged);
    typeLayout->addWidget(thresholdTypeCombo);
    typeLayout->addStretch();
    mainLayout->addLayout(typeLayout);
    
    // Info label
    infoLabel = new QLabel("Manual threshold value selection");
    infoLabel->setStyleSheet("color: #9ca3af; padding: 5px; font-size: 10pt; font-style: italic;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Threshold info (computed values)
    thresholdInfoLabel = new QLabel("");
    thresholdInfoLabel->setStyleSheet("color: #10b981; padding: 5px; font-size: 10pt; font-weight: bold;");
    thresholdInfoLabel->setAlignment(Qt::AlignCenter);
    thresholdInfoLabel->setVisible(false);
    mainLayout->addWidget(thresholdInfoLabel);
    
    // Parameters panel
    QVBoxLayout *paramsLayout = new QVBoxLayout();
    
    // Simple Threshold Group
    simpleThresholdGroup = new QGroupBox("Simple Threshold Parameters");
    simpleThresholdGroup->setStyleSheet(
        "QGroupBox { "
        "   color: #e879f9; "
        "   border: 2px solid rgba(232, 121, 249, 0.3); "
        "   border-radius: 8px; "
        "   margin-top: 10px; "
        "   padding-top: 15px; "
        "} "
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px; "
        "}"
    );
    QVBoxLayout *simpleLayout = new QVBoxLayout(simpleThresholdGroup);
    
    // Threshold value
    QHBoxLayout *thresholdLayout = new QHBoxLayout();
    QLabel *thresholdLabel = new QLabel("Threshold Value:");
    thresholdLabel->setStyleSheet("color: #c4b5fd;");
    thresholdLayout->addWidget(thresholdLabel);
    
    thresholdValueSlider = new QSlider(Qt::Horizontal);
    thresholdValueSlider->setRange(0, 255);
    thresholdValueSlider->setValue(127);
    connect(thresholdValueSlider, &QSlider::valueChanged, 
            this, &ThresholdingDialog::onThresholdValueChanged);
    thresholdLayout->addWidget(thresholdValueSlider);
    
    thresholdValueSpinBox = new QSpinBox();
    thresholdValueSpinBox->setRange(0, 255);
    thresholdValueSpinBox->setValue(127);
    connect(thresholdValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            thresholdValueSlider, &QSlider::setValue);
    thresholdLayout->addWidget(thresholdValueSpinBox);
    
    simpleLayout->addLayout(thresholdLayout);
    
    // Max value
    QHBoxLayout *maxValueLayout = new QHBoxLayout();
    QLabel *maxLabel = new QLabel("Max Value:");
    maxLabel->setStyleSheet("color: #c4b5fd;");
    maxValueLayout->addWidget(maxLabel);
    
    maxValueSlider = new QSlider(Qt::Horizontal);
    maxValueSlider->setRange(0, 255);
    maxValueSlider->setValue(255);
    connect(maxValueSlider, &QSlider::valueChanged,
            this, &ThresholdingDialog::onMaxValueChanged);
    maxValueLayout->addWidget(maxValueSlider);
    
    maxValueSpinBox = new QSpinBox();
    maxValueSpinBox->setRange(0, 255);
    maxValueSpinBox->setValue(255);
    connect(maxValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            maxValueSlider, &QSlider::setValue);
    maxValueLayout->addWidget(maxValueSpinBox);
    
    simpleLayout->addLayout(maxValueLayout);
    
    paramsLayout->addWidget(simpleThresholdGroup);
    
    // Adaptive Threshold Group
    adaptiveThresholdGroup = new QGroupBox("Adaptive Threshold Parameters");
    adaptiveThresholdGroup->setStyleSheet(simpleThresholdGroup->styleSheet());
    adaptiveThresholdGroup->setVisible(false);
    QVBoxLayout *adaptiveLayout = new QVBoxLayout(adaptiveThresholdGroup);
    
    // Block size
    QHBoxLayout *blockSizeLayout = new QHBoxLayout();
    QLabel *blockSizeLabel = new QLabel("Block Size:");
    blockSizeLabel->setStyleSheet("color: #c4b5fd;");
    blockSizeLayout->addWidget(blockSizeLabel);
    
    blockSizeSlider = new QSlider(Qt::Horizontal);
    blockSizeSlider->setRange(3, 51);
    blockSizeSlider->setValue(11);
    blockSizeSlider->setSingleStep(2);
    connect(blockSizeSlider, &QSlider::valueChanged,
            this, &ThresholdingDialog::onBlockSizeChanged);
    blockSizeLayout->addWidget(blockSizeSlider);
    
    blockSizeSpinBox = new QSpinBox();
    blockSizeSpinBox->setRange(3, 51);
    blockSizeSpinBox->setValue(11);
    blockSizeSpinBox->setSingleStep(2);
    connect(blockSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            blockSizeSlider, &QSlider::setValue);
    blockSizeLayout->addWidget(blockSizeSpinBox);
    
    adaptiveLayout->addLayout(blockSizeLayout);
    
    // C constant
    QHBoxLayout *cConstantLayout = new QHBoxLayout();
    QLabel *cConstantLabel = new QLabel("C Constant:");
    cConstantLabel->setStyleSheet("color: #c4b5fd;");
    cConstantLayout->addWidget(cConstantLabel);
    
    cConstantSlider = new QSlider(Qt::Horizontal);
    cConstantSlider->setRange(-20, 20);
    cConstantSlider->setValue(2);
    connect(cConstantSlider, &QSlider::valueChanged,
            this, &ThresholdingDialog::onCConstantChanged);
    cConstantLayout->addWidget(cConstantSlider);
    
    cConstantSpinBox = new QSpinBox();
    cConstantSpinBox->setRange(-20, 20);
    cConstantSpinBox->setValue(2);
    connect(cConstantSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            cConstantSlider, &QSlider::setValue);
    cConstantLayout->addWidget(cConstantSpinBox);
    
    adaptiveLayout->addLayout(cConstantLayout);
    
    QLabel *adaptiveInfo = new QLabel("Block size must be odd. C is subtracted from mean/weighted mean.");
    adaptiveInfo->setStyleSheet("color: #9ca3af; font-size: 9pt;");
    adaptiveLayout->addWidget(adaptiveInfo);
    
    paramsLayout->addWidget(adaptiveThresholdGroup);
    
    // Otsu Group (auto-computed)
    otsuGroup = new QGroupBox("Otsu's Method (Automatic)");
    otsuGroup->setStyleSheet(simpleThresholdGroup->styleSheet());
    otsuGroup->setVisible(false);
    QVBoxLayout *otsuLayout = new QVBoxLayout(otsuGroup);
    
    QLabel *otsuInfo = new QLabel("Otsu's method automatically computes the optimal threshold.\nNo parameters needed.");
    otsuInfo->setStyleSheet("color: #c4b5fd; font-size: 10pt;");
    otsuInfo->setAlignment(Qt::AlignCenter);
    otsuLayout->addWidget(otsuInfo);
    
    paramsLayout->addWidget(otsuGroup);
    
    // Multi-Level Otsu Group
    multiOtsuGroup = new QGroupBox("Multi-Level Otsu Parameters");
    multiOtsuGroup->setStyleSheet(simpleThresholdGroup->styleSheet());
    multiOtsuGroup->setVisible(false);
    QVBoxLayout *multiOtsuLayout = new QVBoxLayout(multiOtsuGroup);
    
    QHBoxLayout *levelsLayout = new QHBoxLayout();
    QLabel *levelsLabel = new QLabel("Number of Levels:");
    levelsLabel->setStyleSheet("color: #c4b5fd;");
    levelsLayout->addWidget(levelsLabel);
    
    otsuLevelsSpinBox = new QSpinBox();
    otsuLevelsSpinBox->setRange(2, 4);
    otsuLevelsSpinBox->setValue(2);
    connect(otsuLevelsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ThresholdingDialog::onOtsuLevelsChanged);
    levelsLayout->addWidget(otsuLevelsSpinBox);
    levelsLayout->addStretch();
    
    multiOtsuLayout->addLayout(levelsLayout);
    
    QLabel *multiOtsuInfo = new QLabel("Divides the image into multiple intensity levels.");
    multiOtsuInfo->setStyleSheet("color: #9ca3af; font-size: 9pt;");
    multiOtsuLayout->addWidget(multiOtsuInfo);
    
    paramsLayout->addWidget(multiOtsuGroup);
    
    mainLayout->addLayout(paramsLayout);
    
    // Image comparison
    QHBoxLayout *imagesLayout = new QHBoxLayout();
    
    // Original
    QVBoxLayout *originalLayout = new QVBoxLayout();
    QLabel *originalTitle = new QLabel("Original (Grayscale)");
    originalTitle->setStyleSheet("font-weight: bold; color: #e879f9; font-size: 11pt;");
    originalTitle->setAlignment(Qt::AlignCenter);
    originalLayout->addWidget(originalTitle);
    
    originalCanvas = new ImageCanvas(this, "#e879f9");
    originalCanvas->setMinimumSize(450, 350);
    
    // Convert to grayscale for display
    cv::Mat grayDisplay;
    if (originalImage.channels() == 3) {
        cv::cvtColor(originalImage, grayDisplay, cv::COLOR_BGR2GRAY);
    } else {
        grayDisplay = originalImage.clone();
    }
    originalCanvas->setImage(grayDisplay);
    originalLayout->addWidget(originalCanvas);
    
    imagesLayout->addLayout(originalLayout);
    
    // Thresholded
    QVBoxLayout *thresholdedLayout = new QVBoxLayout();
    QLabel *thresholdedTitle = new QLabel("Thresholded (Binary)");
    thresholdedTitle->setStyleSheet("font-weight: bold; color: #10b981; font-size: 11pt;");
    thresholdedTitle->setAlignment(Qt::AlignCenter);
    thresholdedLayout->addWidget(thresholdedTitle);
    
    thresholdedCanvas = new ImageCanvas(this, "#10b981");
    thresholdedCanvas->setMinimumSize(450, 350);
    thresholdedLayout->addWidget(thresholdedCanvas);
    
    imagesLayout->addLayout(thresholdedLayout);
    
    mainLayout->addLayout(imagesLayout);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(120);
    connect(cancelButton, &QPushButton::clicked, this, &ThresholdingDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(120);
    connect(applyButton, &QPushButton::clicked, this, &ThresholdingDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void ThresholdingDialog::showParametersForThresholdType(int typeIndex) {
    // Hide all parameter groups
    simpleThresholdGroup->setVisible(false);
    adaptiveThresholdGroup->setVisible(false);
    otsuGroup->setVisible(false);
    multiOtsuGroup->setVisible(false);
    thresholdInfoLabel->setVisible(false);
    
    // Show relevant parameters
    switch (typeIndex) {
        case 0: // Simple Binary Threshold
            simpleThresholdGroup->setVisible(true);
            infoLabel->setText("Manual threshold value selection");
            break;
        case 1: // Adaptive Mean
            adaptiveThresholdGroup->setVisible(true);
            infoLabel->setText("Adaptive thresholding using mean of neighborhood");
            break;
        case 2: // Adaptive Gaussian
            adaptiveThresholdGroup->setVisible(true);
            infoLabel->setText("Adaptive thresholding using Gaussian-weighted mean");
            break;
        case 3: // Otsu
            otsuGroup->setVisible(true);
            infoLabel->setText("Automatic threshold using Otsu's method");
            thresholdInfoLabel->setVisible(true);
            break;
        case 4: // Multi-Level Otsu
            multiOtsuGroup->setVisible(true);
            infoLabel->setText("Multi-level segmentation using extended Otsu");
            break;
        case 5: // Local Threshold
            adaptiveThresholdGroup->setVisible(true);
            infoLabel->setText("Local thresholding with variable window size");
            break;
    }
}

void ThresholdingDialog::onThresholdTypeChanged(int index) {
    showParametersForThresholdType(index);
    updatePreview();
}

void ThresholdingDialog::onThresholdValueChanged(int value) {
    thresholdValue = value;
    thresholdValueSpinBox->setValue(value);
    updatePreview();
}

void ThresholdingDialog::onMaxValueChanged(int value) {
    maxValue = value;
    maxValueSpinBox->setValue(value);
    updatePreview();
}

void ThresholdingDialog::onAdaptiveMethodChanged(int) {
    updatePreview();
}

void ThresholdingDialog::onAdaptiveTypeChanged(int) {
    updatePreview();
}

void ThresholdingDialog::onBlockSizeChanged(int value) {
    // Ensure block size is odd
    if (value % 2 == 0) {
        value++;
        blockSizeSlider->setValue(value);
        return;
    }
    blockSize = value;
    blockSizeSpinBox->setValue(value);
    updatePreview();
}

void ThresholdingDialog::onCConstantChanged(int value) {
    cConstant = value;
    cConstantSpinBox->setValue(value);
    updatePreview();
}

void ThresholdingDialog::onOtsuLevelsChanged(int value) {
    otsuLevels = value;
    updatePreview();
}

void ThresholdingDialog::updatePreview() {
    int type = thresholdTypeCombo->currentIndex();
    
    switch (type) {
        case 0: applySimpleThreshold(); break;
        case 1: // Adaptive Mean
        case 2: // Adaptive Gaussian
            applyAdaptiveThreshold(); 
            break;
        case 3: applyOtsuThreshold(); break;
        case 4: applyMultiLevelOtsu(); break;
        case 5: applyLocalThreshold(); break;
    }
    
    if (!thresholdedImage.empty()) {
        thresholdedCanvas->setImage(thresholdedImage);
        emit previewUpdated(thresholdedImage);
    }
}

void ThresholdingDialog::applySimpleThreshold() {
    ImageProcessor::applySimpleThreshold(originalImage, thresholdedImage, 
                                         thresholdValue, maxValue);
}

void ThresholdingDialog::applyAdaptiveThreshold() {
    int type = thresholdTypeCombo->currentIndex();
    bool useGaussian = (type == 2);
    
    ImageProcessor::applyAdaptiveThreshold(originalImage, thresholdedImage,
                                           maxValue, blockSize, cConstant, useGaussian);
}

void ThresholdingDialog::applyOtsuThreshold() {
    computedThreshold = ImageProcessor::computeOtsuThreshold(originalImage, thresholdedImage);
    thresholdInfoLabel->setText(QString("Computed Threshold: %1").arg(computedThreshold, 0, 'f', 2));
    thresholdInfoLabel->setVisible(true);
}

void ThresholdingDialog::applyMultiLevelOtsu() {
    ImageProcessor::applyMultiLevelOtsu(originalImage, thresholdedImage, otsuLevels);
}

void ThresholdingDialog::applyLocalThreshold() {
    ImageProcessor::applyLocalThreshold(originalImage, thresholdedImage, blockSize, cConstant);
}

void ThresholdingDialog::onApplyClicked() {
    applied = true;
    accept();
}

void ThresholdingDialog::onCancelClicked() {
    applied = false;
    reject();
}

QString ThresholdingDialog::getThresholdingType() const {
    return thresholdTypeCombo->currentText();
}

#include "moc_ThresholdingDialog.cpp"
