#include "AdjustmentDialog.h"
#include "ImageProcessor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AdjustmentDialog::AdjustmentDialog(const cv::Mat& sourceImage, QWidget *parent)
    : QDialog(parent), sourceImage(sourceImage.clone()), applied(false),
      brightness(0), contrast(0) {
    
    setWindowTitle("Brightness & Contrast");
    setMinimumSize(450, 350);
    
    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1a0a1f, stop:1 #251e35);
        }
    )");
    
    setupUI();
    updatePreview();
}

AdjustmentDialog::~AdjustmentDialog() {
}

void AdjustmentDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Brightness & Contrast Adjustment");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    mainLayout->addWidget(titleLabel);
    
    QLabel *infoLabel = new QLabel("Adjust parameters below.\nPreview updates in the main window.");
    infoLabel->setStyleSheet("color: #c4b5fd; padding: 10px; font-size: 10pt;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Brightness control
    QGroupBox *brightnessGroup = new QGroupBox("Brightness (-100 to +100)");
    QVBoxLayout *brightnessLayout = new QVBoxLayout(brightnessGroup);
    
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(0);
    brightnessSlider->setTickPosition(QSlider::TicksBelow);
    brightnessSlider->setTickInterval(20);
    
    brightnessSpinBox = new QSpinBox();
    brightnessSpinBox->setRange(-100, 100);
    brightnessSpinBox->setValue(0);
    
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessSpinBox);
    mainLayout->addWidget(brightnessGroup);
    
    // Contrast control
    QGroupBox *contrastGroup = new QGroupBox("Contrast (-100 to +100)");
    QVBoxLayout *contrastLayout = new QVBoxLayout(contrastGroup);
    
    contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(-100, 100);
    contrastSlider->setValue(0);
    contrastSlider->setTickPosition(QSlider::TicksBelow);
    contrastSlider->setTickInterval(20);
    
    contrastSpinBox = new QSpinBox();
    contrastSpinBox->setRange(-100, 100);
    contrastSpinBox->setValue(0);
    
    contrastLayout->addWidget(contrastSlider);
    contrastLayout->addWidget(contrastSpinBox);
    mainLayout->addWidget(contrastGroup);
    
    mainLayout->addSpacing(10);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    resetButton = new QPushButton("Reset");
    resetButton->setStyleSheet(
        "background-color: rgba(91, 75, 115, 0.4); "
        "color: #c4b5fd; "
        "border: 1px solid rgba(232, 121, 249, 0.3); "
        "border-radius: 8px; "
        "padding: 10px 20px; "
        "font-weight: 600;"
    );
    
    cancelButton = new QPushButton("Cancel");
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    
    btnLayout->addWidget(resetButton);
    btnLayout->addStretch();
    btnLayout->addWidget(cancelButton);
    btnLayout->addWidget(applyButton);
    
    mainLayout->addLayout(btnLayout);
    
    // Connect signals
    connect(brightnessSlider, &QSlider::valueChanged, brightnessSpinBox, &QSpinBox::setValue);
    connect(brightnessSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            brightnessSlider, &QSlider::setValue);
    
    connect(contrastSlider, &QSlider::valueChanged, contrastSpinBox, &QSpinBox::setValue);
    connect(contrastSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            contrastSlider, &QSlider::setValue);
    
    connect(brightnessSlider, &QSlider::valueChanged, 
            this, &AdjustmentDialog::onBrightnessChanged);
    connect(contrastSlider, &QSlider::valueChanged, 
            this, &AdjustmentDialog::onContrastChanged);
    
    connect(resetButton, &QPushButton::clicked, this, &AdjustmentDialog::onResetClicked);
    connect(applyButton, &QPushButton::clicked, this, &AdjustmentDialog::onApplyClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AdjustmentDialog::onCancelClicked);
}

void AdjustmentDialog::onBrightnessChanged(int value) {
    brightness = value;
    updatePreview();
}

void AdjustmentDialog::onContrastChanged(int value) {
    contrast = value;
    updatePreview();
}

void AdjustmentDialog::onResetClicked() {
    brightnessSlider->setValue(0);
    contrastSlider->setValue(0);
    updatePreview();
}

void AdjustmentDialog::onApplyClicked() {
    applyAdjustments();
    applied = true;
    accept();
}

void AdjustmentDialog::onCancelClicked() {
    applied = false;
    reject();
}

void AdjustmentDialog::updatePreview() {
    cv::Mat preview;
    ImageProcessor::adjustBrightnessContrast(sourceImage, preview, brightness, contrast);
    emit previewUpdated(preview);
}

void AdjustmentDialog::applyAdjustments() {
    ImageProcessor::adjustBrightnessContrast(sourceImage, adjustedImage, brightness, contrast);
}
