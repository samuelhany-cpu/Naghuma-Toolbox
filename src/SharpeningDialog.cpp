#include "SharpeningDialog.h"
#include "ImageCanvas.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStackedWidget>

SharpeningDialog::SharpeningDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), originalImage(image.clone()), applied(false),
      filterType(0), amount(1.0), boostFactor(1.5) {
    
    setWindowTitle("Highpass Sharpening Filters - Phase 21");
    setMinimumSize(900, 700);
    
    setupUI();
    updatePreview();
}

SharpeningDialog::~SharpeningDialog() {
}

void SharpeningDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Highpass Sharpening Filters");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Filter type selection
    QHBoxLayout* typeLayout = new QHBoxLayout();
    QLabel* typeLabel = new QLabel("Filter Type:");
    typeLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    filterCombo = new QComboBox();
    filterCombo->addItem("Laplacian Sharpening");
    filterCombo->addItem("Unsharp Masking");
    filterCombo->addItem("High-Boost Filtering");
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SharpeningDialog::onFilterTypeChanged);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(filterCombo, 1);
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
    
    // === LAPLACIAN (No parameters) ===
    QWidget* laplacianWidget = new QWidget();
    QVBoxLayout* laplacianLayout = new QVBoxLayout(laplacianWidget);
    
    QLabel* laplacianInfo = new QLabel(
        "Laplacian Sharpening:\n"
        "• Applies 2nd derivative edge detection\n"
        "• Adds result back to original image\n"
        "• Formula: g(x,y) = f(x,y) + c·?²f(x,y)\n"
        "• c = -1 for standard sharpening\n\n"
        "Good for: General edge enhancement"
    );
    laplacianInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    laplacianLayout->addWidget(laplacianInfo);
    laplacianLayout->addStretch();
    
    stackedParams->addWidget(laplacianWidget);
    
    // === UNSHARP MASKING ===
    unsharpWidget = new QWidget();
    QVBoxLayout* unsharpLayout = new QVBoxLayout(unsharpWidget);
    
    QHBoxLayout* amountSliderLayout = new QHBoxLayout();
    QLabel* amountLabelText = new QLabel("Amount:");
    amountLabelText->setStyleSheet("color: #c4b5fd;");
    amountSlider = new QSlider(Qt::Horizontal);
    amountSlider->setRange(10, 300);  // 0.1 to 3.0
    amountSlider->setValue(100);      // 1.0
    amountSpinBox = new QDoubleSpinBox();
    amountSpinBox->setRange(0.1, 3.0);
    amountSpinBox->setValue(1.0);
    amountSpinBox->setSingleStep(0.1);
    amountSpinBox->setDecimals(2);
    amountSpinBox->setStyleSheet("color: #a78bfa; min-width: 80px;");
    
    connect(amountSlider, &QSlider::valueChanged, this, [this](int value) {
        amountSpinBox->setValue(value / 100.0);
    });
    connect(amountSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        amountSlider->setValue(static_cast<int>(value * 100));
        onAmountChanged(value);
    });
    
    amountSliderLayout->addWidget(amountLabelText);
    amountSliderLayout->addWidget(amountSlider);
    amountSliderLayout->addWidget(amountSpinBox);
    unsharpLayout->addLayout(amountSliderLayout);
    
    QLabel* unsharpInfo = new QLabel(
        "Unsharp Masking:\n"
        "1. Blur the original image\n"
        "2. Subtract blur from original (mask)\n"
        "3. Add weighted mask back to original\n"
        "• Formula: sharp = original + amount × mask\n\n"
        "Amount controls sharpening strength"
    );
    unsharpInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    unsharpLayout->addWidget(unsharpInfo);
    unsharpLayout->addStretch();
    
    stackedParams->addWidget(unsharpWidget);
    
    // === HIGH-BOOST FILTERING ===
    highBoostWidget = new QWidget();
    QVBoxLayout* highBoostLayout = new QVBoxLayout(highBoostWidget);
    
    QHBoxLayout* boostSliderLayout = new QHBoxLayout();
    QLabel* boostLabelText = new QLabel("Boost Factor (k):");
    boostLabelText->setStyleSheet("color: #c4b5fd;");
    boostSlider = new QSlider(Qt::Horizontal);
    boostSlider->setRange(10, 300);   // 0.1 to 3.0
    boostSlider->setValue(150);       // 1.5
    boostSpinBox = new QDoubleSpinBox();
    boostSpinBox->setRange(0.1, 3.0);
    boostSpinBox->setValue(1.5);
    boostSpinBox->setSingleStep(0.1);
    boostSpinBox->setDecimals(2);
    boostSpinBox->setStyleSheet("color: #a78bfa; min-width: 80px;");
    
    connect(boostSlider, &QSlider::valueChanged, this, [this](int value) {
        boostSpinBox->setValue(value / 100.0);
    });
    connect(boostSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        boostSlider->setValue(static_cast<int>(value * 100));
        onBoostFactorChanged(value);
    });
    
    boostSliderLayout->addWidget(boostLabelText);
    boostSliderLayout->addWidget(boostSlider);
    boostSliderLayout->addWidget(boostSpinBox);
    highBoostLayout->addLayout(boostSliderLayout);
    
    QLabel* highBoostInfo = new QLabel(
        "High-Boost Filtering:\n"
        "• Generalized unsharp masking\n"
        "• Formula: sharp = k×original - blur\n"
        "• When k=1: standard highpass\n"
        "• When k>1: emphasizes high frequencies\n\n"
        "Higher k = stronger edge emphasis"
    );
    highBoostInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    highBoostLayout->addWidget(highBoostInfo);
    highBoostLayout->addStretch();
    
    stackedParams->addWidget(highBoostWidget);
    
    paramsLayout->addWidget(stackedParams);
    mainLayout->addWidget(paramsGroup);
    
    // Connect stacked widget switching
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
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
    QLabel* processedLabel = new QLabel("Sharpened Result");
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
    connect(resetButton, &QPushButton::clicked, this, &SharpeningDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);
    
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &SharpeningDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Store processedCanvas for updates
    processedCanvas->setObjectName("processedCanvas");
}

void SharpeningDialog::onFilterTypeChanged(int index) {
    filterType = index;
    updatePreview();
}

void SharpeningDialog::onAmountChanged(double value) {
    amount = value;
    updatePreview();
}

void SharpeningDialog::onBoostFactorChanged(double value) {
    boostFactor = value;
    updatePreview();
}

void SharpeningDialog::updatePreview() {
    try {
        if (filterType == 0) {
            applyLaplacianSharpening();
        } else if (filterType == 1) {
            applyUnsharpMasking();
        } else {
            applyHighBoostFiltering();
        }
        
        if (!sharpenedImage.empty()) {
            ImageCanvas* canvas = findChild<ImageCanvas*>("processedCanvas");
            if (canvas) {
                canvas->setImage(sharpenedImage);
            }
            emit previewUpdated(sharpenedImage);
        }
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
    }
}

void SharpeningDialog::applyLaplacianSharpening() {
    // Apply Laplacian operator
    cv::Mat gray, laplacian;
    if (originalImage.channels() == 3) {
        cv::cvtColor(originalImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = originalImage.clone();
    }
    
    cv::Laplacian(gray, laplacian, CV_16S, 3);
    cv::convertScaleAbs(laplacian, laplacian);
    
    // Convert back to same type as original
    cv::Mat laplacian3Channel;
    if (originalImage.channels() == 3) {
        cv::cvtColor(laplacian, laplacian3Channel, cv::COLOR_GRAY2BGR);
    } else {
        laplacian3Channel = laplacian;
    }
    
    // Add Laplacian to original: g(x,y) = f(x,y) - ?²f(x,y)
    cv::subtract(originalImage, laplacian3Channel, sharpenedImage);
    
    operationType = "Laplacian Sharpening";
    infoLabel->setText(operationType);
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SharpeningDialog::applyUnsharpMasking() {
    // 1. Blur the original
    cv::Mat blurred;
    cv::GaussianBlur(originalImage, blurred, cv::Size(5, 5), 1.0);
    
    // 2. Create mask: original - blurred
    cv::Mat mask;
    cv::subtract(originalImage, blurred, mask);
    
    // 3. Add weighted mask: sharp = original + amount × mask
    cv::Mat weightedMask;
    mask.convertTo(weightedMask, -1, amount);
    cv::add(originalImage, weightedMask, sharpenedImage);
    
    operationType = QString("Unsharp Masking (amount=%.2f)").arg(amount);
    infoLabel->setText(operationType);
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SharpeningDialog::applyHighBoostFiltering() {
    // High-boost: sharp = k×original - blurred
    // = (k-1)×original + (original - blurred)
    // = (k-1)×original + mask
    
    // 1. Blur the original
    cv::Mat blurred;
    cv::GaussianBlur(originalImage, blurred, cv::Size(5, 5), 1.0);
    
    // 2. Apply high-boost formula
    cv::Mat scaledOriginal, result;
    originalImage.convertTo(scaledOriginal, -1, boostFactor);
    cv::subtract(scaledOriginal, blurred, result);
    
    // Clamp to valid range
    result.convertTo(sharpenedImage, originalImage.type());
    
    operationType = QString("High-Boost Filtering (k=%.2f)").arg(boostFactor);
    infoLabel->setText(operationType);
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SharpeningDialog::onResetClicked() {
    amountSpinBox->setValue(1.0);
    boostSpinBox->setValue(1.5);
    filterCombo->setCurrentIndex(0);
    updatePreview();
}

void SharpeningDialog::onApplyClicked() {
    if (!sharpenedImage.empty()) {
        applied = true;
        accept();
    }
}
#include "moc_SharpeningDialog.cpp"
