#include "TransformDialog.h"
#include "ImageCanvas.h"
#include "ImageProcessor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

// ===== TransformDialog Base Class =====

TransformDialog::TransformDialog(const cv::Mat& sourceImage, QWidget *parent)
    : QDialog(parent), sourceImage(sourceImage.clone()), applied(false) {
    
    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1a0a1f, stop:1 #251e35);
        }
    )");
}

TransformDialog::~TransformDialog() {
}

void TransformDialog::setupBaseUI(const QString& title) {
    setWindowTitle(title);
    setMinimumSize(400, 300);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QLabel *titleLabel = new QLabel(title + " - Live Preview");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #e879f9; padding: 15px;");
    mainLayout->addWidget(titleLabel);
    
    QLabel *infoLabel = new QLabel("Adjust parameters below.\nPreview updates in the main window.");
    infoLabel->setStyleSheet("color: #c4b5fd; padding: 10px; font-size: 10pt;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Buttons at bottom
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    connect(applyButton, &QPushButton::clicked, this, &TransformDialog::onApplyClicked);
    
    cancelButton = new QPushButton("Cancel");
    connect(cancelButton, &QPushButton::clicked, this, &TransformDialog::onCancelClicked);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(cancelButton);
    btnLayout->addWidget(applyButton);
    mainLayout->addLayout(btnLayout);
}

void TransformDialog::emitPreview(const cv::Mat& preview) {
    emit previewUpdated(preview);
}

void TransformDialog::onApplyClicked() {
    applyTransform();
    applied = true;
    accept();
}

void TransformDialog::onCancelClicked() {
    applied = false;
    reject();
}

// ===== TranslationDialog =====

TranslationDialog::TranslationDialog(const cv::Mat& sourceImage, QWidget *parent)
    : TransformDialog(sourceImage, parent), tx(0), ty(0) {
    
    setupBaseUI("Image Translation");
    
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    
    QGroupBox *xGroup = new QGroupBox("Horizontal Translation (X)");
    QVBoxLayout *xLayout = new QVBoxLayout(xGroup);
    sliderX = new QSlider(Qt::Horizontal);
    sliderX->setRange(-1000, 1000);
    sliderX->setValue(0);
    spinBoxX = new QSpinBox();
    spinBoxX->setRange(-1000, 1000);
    spinBoxX->setValue(0);
    xLayout->addWidget(sliderX);
    xLayout->addWidget(spinBoxX);
    mainLayout->insertWidget(1, xGroup);
    
    QGroupBox *yGroup = new QGroupBox("Vertical Translation (Y)");
    QVBoxLayout *yLayout = new QVBoxLayout(yGroup);
    sliderY = new QSlider(Qt::Horizontal);
    sliderY->setRange(-1000, 1000);
    sliderY->setValue(0);
    spinBoxY = new QSpinBox();
    spinBoxY->setRange(-1000, 1000);
    spinBoxY->setValue(0);
    yLayout->addWidget(sliderY);
    yLayout->addWidget(spinBoxY);
    mainLayout->insertWidget(2, yGroup);
    
    // Connect signals
    connect(sliderX, &QSlider::valueChanged, spinBoxX, &QSpinBox::setValue);
    connect(spinBoxX, QOverload<int>::of(&QSpinBox::valueChanged), sliderX, &QSlider::setValue);
    connect(sliderY, &QSlider::valueChanged, spinBoxY, &QSpinBox::setValue);
    connect(spinBoxY, QOverload<int>::of(&QSpinBox::valueChanged), sliderY, &QSlider::setValue);
    
    connect(sliderX, &QSlider::valueChanged, this, &TranslationDialog::updatePreview);
    connect(sliderY, &QSlider::valueChanged, this, &TranslationDialog::updatePreview);
    
    updatePreview();
}

void TranslationDialog::applyTransform() {
    tx = spinBoxX->value();
    ty = spinBoxY->value();
    ImageProcessor::translate(sourceImage, transformedImage, tx, ty);
}

void TranslationDialog::updatePreview() {
    tx = spinBoxX->value();
    ty = spinBoxY->value();
    cv::Mat preview;
    ImageProcessor::translate(sourceImage, preview, tx, ty);
    emitPreview(preview);
}

// ===== RotationDialog =====

RotationDialog::RotationDialog(const cv::Mat& sourceImage, QWidget *parent)
    : TransformDialog(sourceImage, parent), angle(0) {
    
    setupBaseUI("Image Rotation");
    
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    
    QGroupBox *angleGroup = new QGroupBox("Rotation Angle (degrees)");
    QVBoxLayout *angleLayout = new QVBoxLayout(angleGroup);
    angleSlider = new QSlider(Qt::Horizontal);
    angleSlider->setRange(-180, 180);
    angleSlider->setValue(0);
    angleSpinBox = new QDoubleSpinBox();
    angleSpinBox->setRange(-180, 180);
    angleSpinBox->setValue(0);
    angleSpinBox->setSingleStep(1);
    angleLayout->addWidget(angleSlider);
    angleLayout->addWidget(angleSpinBox);
    mainLayout->insertWidget(1, angleGroup);
    
    connect(angleSlider, &QSlider::valueChanged, [this](int value) {
        angleSpinBox->setValue(value);
    });
    connect(angleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        angleSlider->setValue(static_cast<int>(value));
    });
    
    connect(angleSlider, &QSlider::valueChanged, this, &RotationDialog::updatePreview);
    connect(angleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &RotationDialog::updatePreview);
    
    updatePreview();
}

void RotationDialog::applyTransform() {
    angle = angleSpinBox->value();
    ImageProcessor::rotate(sourceImage, transformedImage, angle);
}

void RotationDialog::updatePreview() {
    angle = angleSpinBox->value();
    cv::Mat preview;
    ImageProcessor::rotate(sourceImage, preview, angle);
    emitPreview(preview);
}

// ===== ZoomDialog =====

ZoomDialog::ZoomDialog(const cv::Mat& sourceImage, QWidget *parent)
    : TransformDialog(sourceImage, parent), scale(1.0) {
    
    setupBaseUI("Image Zoom");
    
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    
    QGroupBox *zoomGroup = new QGroupBox("Zoom Scale (0.1 - 5.0x)");
    QVBoxLayout *zoomLayout = new QVBoxLayout(zoomGroup);
    zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(10, 500);
    zoomSlider->setValue(100);
    zoomSpinBox = new QDoubleSpinBox();
    zoomSpinBox->setRange(0.1, 5.0);
    zoomSpinBox->setValue(1.0);
    zoomSpinBox->setSingleStep(0.1);
    zoomLayout->addWidget(zoomSlider);
    zoomLayout->addWidget(zoomSpinBox);
    mainLayout->insertWidget(1, zoomGroup);
    
    connect(zoomSlider, &QSlider::valueChanged, [this](int value) {
        zoomSpinBox->setValue(value / 100.0);
    });
    connect(zoomSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        zoomSlider->setValue(static_cast<int>(value * 100));
    });
    
    connect(zoomSlider, &QSlider::valueChanged, this, &ZoomDialog::updatePreview);
    connect(zoomSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &ZoomDialog::updatePreview);
    
    updatePreview();
}

void ZoomDialog::applyTransform() {
    scale = zoomSpinBox->value();
    ImageProcessor::zoom(sourceImage, transformedImage, scale);
}

void ZoomDialog::updatePreview() {
    scale = zoomSpinBox->value();
    cv::Mat preview;
    ImageProcessor::zoom(sourceImage, preview, scale);
    emitPreview(preview);
}

// ===== SkewDialog =====

SkewDialog::SkewDialog(const cv::Mat& sourceImage, QWidget *parent)
    : TransformDialog(sourceImage, parent), skewX(0.0), skewY(0.0) {
    
    setupBaseUI("Image Skew");
    
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    
    QGroupBox *skewXGroup = new QGroupBox("Horizontal Skew (-0.5 to 0.5)");
    QVBoxLayout *skewXLayout = new QVBoxLayout(skewXGroup);
    skewXSlider = new QSlider(Qt::Horizontal);
    skewXSlider->setRange(-50, 50);
    skewXSlider->setValue(0);
    skewXSpinBox = new QDoubleSpinBox();
    skewXSpinBox->setRange(-0.5, 0.5);
    skewXSpinBox->setValue(0.0);
    skewXSpinBox->setSingleStep(0.05);
    skewXLayout->addWidget(skewXSlider);
    skewXLayout->addWidget(skewXSpinBox);
    mainLayout->insertWidget(1, skewXGroup);
    
    QGroupBox *skewYGroup = new QGroupBox("Vertical Skew (-0.5 to 0.5)");
    QVBoxLayout *skewYLayout = new QVBoxLayout(skewYGroup);
    skewYSlider = new QSlider(Qt::Horizontal);
    skewYSlider->setRange(-50, 50);
    skewYSlider->setValue(0);
    skewYSpinBox = new QDoubleSpinBox();
    skewYSpinBox->setRange(-0.5, 0.5);
    skewYSpinBox->setValue(0.0);
    skewYSpinBox->setSingleStep(0.05);
    skewYLayout->addWidget(skewYSlider);
    skewYLayout->addWidget(skewYSpinBox);
    mainLayout->insertWidget(2, skewYGroup);
    
    connect(skewXSlider, &QSlider::valueChanged, [this](int value) {
        skewXSpinBox->setValue(value / 100.0);
    });
    connect(skewXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        skewXSlider->setValue(static_cast<int>(value * 100));
    });
    
    connect(skewYSlider, &QSlider::valueChanged, [this](int value) {
        skewYSpinBox->setValue(value / 100.0);
    });
    connect(skewYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        skewYSlider->setValue(static_cast<int>(value * 100));
    });
    
    connect(skewXSlider, &QSlider::valueChanged, this, &SkewDialog::updatePreview);
    connect(skewXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &SkewDialog::updatePreview);
    connect(skewYSlider, &QSlider::valueChanged, this, &SkewDialog::updatePreview);
    connect(skewYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &SkewDialog::updatePreview);
    
    updatePreview();
}

void SkewDialog::applyTransform() {
    skewX = skewXSpinBox->value();
    skewY = skewYSpinBox->value();
    ImageProcessor::applySkew(sourceImage, transformedImage, skewX, skewY);
}

void SkewDialog::updatePreview() {
    skewX = skewXSpinBox->value();
    skewY = skewYSpinBox->value();
    cv::Mat preview;
    ImageProcessor::applySkew(sourceImage, preview, skewX, skewY);
    emitPreview(preview);
}
