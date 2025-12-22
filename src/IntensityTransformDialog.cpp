#include "IntensityTransformDialog.h"
#include "ImageCanvas.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStackedWidget>
#include <cmath>

IntensityTransformDialog::IntensityTransformDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), originalImage(image.clone()), applied(false), 
      transformType(0), gamma(1.0), logC(1.0) {
    
    setWindowTitle("Intensity Transformations - Phase 21");
    setMinimumSize(900, 700);
    
    setupUI();
    updatePreview();
}

IntensityTransformDialog::~IntensityTransformDialog() {
}

void IntensityTransformDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Intensity Transformations");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Transform type selection
    QHBoxLayout* typeLayout = new QHBoxLayout();
    QLabel* typeLabel = new QLabel("Transform Type:");
    typeLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    transformCombo = new QComboBox();
    transformCombo->addItem("Gamma Correction (Power-Law)");
    transformCombo->addItem("Log Transformation");
    transformCombo->addItem("Power-Law with C");
    connect(transformCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &IntensityTransformDialog::onTransformTypeChanged);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(transformCombo, 1);
    mainLayout->addLayout(typeLayout);
    
    // Parameters area with stacked widget
    QGroupBox* paramsGroup = new QGroupBox("Parameters");
    paramsGroup->setStyleSheet(
        "QGroupBox { color: #c4b5fd; font-weight: bold; border: 2px solid rgba(91, 75, 115, 0.5); "
        "border-radius: 8px; margin-top: 10px; padding: 15px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* paramsLayout = new QVBoxLayout(paramsGroup);
    
    QStackedWidget* stackedParams = new QStackedWidget();
    
    // === GAMMA CORRECTION ===
    gammaWidget = new QWidget();
    QVBoxLayout* gammaLayout = new QVBoxLayout(gammaWidget);
    
    QHBoxLayout* gammaSliderLayout = new QHBoxLayout();
    QLabel* gammaLabelText = new QLabel("Gamma (?):");
    gammaLabelText->setStyleSheet("color: #c4b5fd;");
    gammaSlider = new QSlider(Qt::Horizontal);
    gammaSlider->setRange(10, 500);  // 0.1 to 5.0
    gammaSlider->setValue(100);      // 1.0
    gammaSpinBox = new QDoubleSpinBox();
    gammaSpinBox->setRange(0.1, 5.0);
    gammaSpinBox->setValue(1.0);
    gammaSpinBox->setSingleStep(0.1);
    gammaSpinBox->setDecimals(2);
    gammaSpinBox->setStyleSheet("color: #a78bfa; min-width: 80px;");
    
    connect(gammaSlider, &QSlider::valueChanged, this, [this](int value) {
        gammaSpinBox->setValue(value / 100.0);
    });
    connect(gammaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        gammaSlider->setValue(static_cast<int>(value * 100));
        onGammaChanged(value);
    });
    
    gammaSliderLayout->addWidget(gammaLabelText);
    gammaSliderLayout->addWidget(gammaSlider);
    gammaSliderLayout->addWidget(gammaSpinBox);
    gammaLayout->addLayout(gammaSliderLayout);
    
    QLabel* gammaInfo = new QLabel(
        "Gamma Correction (s = r^?):\n"
        "• ? < 1.0: Brightens dark regions (display calibration)\n"
        "• ? = 1.0: No change (linear)\n"
        "• ? > 1.0: Darkens bright regions (contrast enhancement)\n\n"
        "Common values: 0.4 (bright), 1.0 (none), 2.2 (dark)"
    );
    gammaInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    gammaLayout->addWidget(gammaInfo);
    gammaLayout->addStretch();
    
    stackedParams->addWidget(gammaWidget);
    
    // === LOG TRANSFORMATION ===
    logWidget = new QWidget();
    QVBoxLayout* logLayout = new QVBoxLayout(logWidget);
    
    QHBoxLayout* logCSliderLayout = new QHBoxLayout();
    QLabel* logCLabelText = new QLabel("Constant (c):");
    logCLabelText->setStyleSheet("color: #c4b5fd;");
    logCSlider = new QSlider(Qt::Horizontal);
    logCSlider->setRange(1, 200);    // 0.01 to 2.0
    logCSlider->setValue(100);        // 1.0
    logCSpinBox = new QDoubleSpinBox();
    logCSpinBox->setRange(0.01, 2.0);
    logCSpinBox->setValue(1.0);
    logCSpinBox->setSingleStep(0.01);
    logCSpinBox->setDecimals(2);
    logCSpinBox->setStyleSheet("color: #a78bfa; min-width: 80px;");
    
    connect(logCSlider, &QSlider::valueChanged, this, [this](int value) {
        logCSpinBox->setValue(value / 100.0);
    });
    connect(logCSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        logCSlider->setValue(static_cast<int>(value * 100));
        onLogCChanged(value);
    });
    
    logCSliderLayout->addWidget(logCLabelText);
    logCSliderLayout->addWidget(logCSlider);
    logCSliderLayout->addWidget(logCSpinBox);
    logLayout->addLayout(logCSliderLayout);
    
    QLabel* logInfo = new QLabel(
        "Log Transformation (s = c * log(1 + r)):\n"
        "• Expands dark pixel values\n"
        "• Compresses bright pixel values\n"
        "• Useful for dynamic range compression\n"
        "• Ideal for Fourier spectrum display\n\n"
        "c = scaling constant (typically 1.0)"
    );
    logInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    logLayout->addWidget(logInfo);
    logLayout->addStretch();
    
    stackedParams->addWidget(logWidget);
    
    paramsLayout->addWidget(stackedParams);
    mainLayout->addWidget(paramsGroup);
    
    // Connect stacked widget switching
    connect(transformCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            stackedParams, &QStackedWidget::setCurrentIndex);
    
    // Image display area
    QHBoxLayout* imagesLayout = new QHBoxLayout();
    
    QVBoxLayout* originalLayout = new QVBoxLayout();
    QLabel* originalLabel = new QLabel("Original Image");
    originalLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    originalLabel->setAlignment(Qt::AlignCenter);
    ImageCanvas* originalCanvas = new ImageCanvas(this, "#e879f9");
    originalCanvas->setMinimumSize(350, 300);
    originalCanvas->setImage(originalImage);
    originalLayout->addWidget(originalLabel);
    originalLayout->addWidget(originalCanvas);
    imagesLayout->addLayout(originalLayout);
    
    QVBoxLayout* processedLayout = new QVBoxLayout();
    QLabel* processedLabel = new QLabel("Transformed Result");
    processedLabel->setStyleSheet("color: #c026d3; font-weight: bold;");
    processedLabel->setAlignment(Qt::AlignCenter);
    ImageCanvas* processedCanvas = new ImageCanvas(this, "#c026d3");
    processedCanvas->setMinimumSize(350, 300);
    processedLayout->addWidget(processedLabel);
    processedLayout->addWidget(processedCanvas);
    imagesLayout->addLayout(processedLayout);
    
    mainLayout->addLayout(imagesLayout);
    
    // Info label
    infoLabel = new QLabel("Adjust parameters to see results");
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    resetButton = new QPushButton("Reset");
    resetButton->setMinimumWidth(100);
    connect(resetButton, &QPushButton::clicked, this, &IntensityTransformDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);
    
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &IntensityTransformDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Store processedCanvas for updates
    processedCanvas->setObjectName("processedCanvas");
}

void IntensityTransformDialog::onTransformTypeChanged(int index) {
    transformType = index;
    updatePreview();
}

void IntensityTransformDialog::onGammaChanged(double value) {
    gamma = value;
    updatePreview();
}

void IntensityTransformDialog::onLogCChanged(double value) {
    logC = value;
    updatePreview();
}

void IntensityTransformDialog::updatePreview() {
    try {
        if (transformType == 0 || transformType == 2) {
            applyGammaCorrection();
        } else {
            applyLogTransform();
        }
        
        if (!transformedImage.empty()) {
            ImageCanvas* canvas = findChild<ImageCanvas*>("processedCanvas");
            if (canvas) {
                canvas->setImage(transformedImage);
            }
            emit previewUpdated(transformedImage);
        }
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
    }
}

void IntensityTransformDialog::applyGammaCorrection() {
    // s = c * r^gamma (Equation 3-5 from Chapter 3)
    // For standard gamma correction, c = 1.0
    
    cv::Mat floatImage;
    originalImage.convertTo(floatImage, CV_32F, 1.0/255.0);  // Normalize to [0, 1]
    
    // Apply gamma correction: s = r^gamma
    cv::pow(floatImage, gamma, floatImage);
    
    // Convert back to 8-bit
    floatImage.convertTo(transformedImage, CV_8U, 255.0);
    
    operationType = QString("Gamma Correction (?=%.2f)").arg(gamma);
    infoLabel->setText(operationType);
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void IntensityTransformDialog::applyLogTransform() {
    // s = c * log(1 + r) (Equation 3-4 from Chapter 3)
    
    cv::Mat floatImage;
    originalImage.convertTo(floatImage, CV_32F);
    
    // Apply log transformation
    cv::log(1.0 + floatImage, floatImage);
    floatImage *= logC;
    
    // Normalize to [0, 255]
    cv::normalize(floatImage, floatImage, 0, 255, cv::NORM_MINMAX);
    floatImage.convertTo(transformedImage, CV_8U);
    
    operationType = QString("Log Transform (c=%.2f)").arg(logC);
    infoLabel->setText(operationType);
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void IntensityTransformDialog::applyPowerLaw() {
    // Same as gamma but keeping separate for clarity
    applyGammaCorrection();
}

void IntensityTransformDialog::onResetClicked() {
    gammaSpinBox->setValue(1.0);
    logCSpinBox->setValue(1.0);
    transformCombo->setCurrentIndex(0);
    updatePreview();
}

void IntensityTransformDialog::onApplyClicked() {
    if (!transformedImage.empty()) {
        applied = true;
        accept();
    }
}
#include "moc_IntensityTransformDialog.cpp"
