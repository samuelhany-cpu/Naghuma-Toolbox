#include "WaveletDialog.h"
#include "ImageCanvas.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStackedWidget>

WaveletDialog::WaveletDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), originalImage(image.clone()), applied(false) {
    
    setWindowTitle("Wavelet Transform - Phase 20");
    setMinimumSize(1200, 800);
    
    setupUI();
    onOperationChanged(0);
}

WaveletDialog::~WaveletDialog() {
}

void WaveletDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Wavelet Transform & Denoising");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Operation selection
    QHBoxLayout* opLayout = new QHBoxLayout();
    QLabel* opLabel = new QLabel("Operation:");
    opLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    operationCombo = new QComboBox();
    operationCombo->addItem("Wavelet Denoising");
    operationCombo->addItem("Decomposition (1 Level)");
    operationCombo->addItem("Decomposition Visualization");
    connect(operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WaveletDialog::onOperationChanged);
    opLayout->addWidget(opLabel);
    opLayout->addWidget(operationCombo, 1);
    mainLayout->addLayout(opLayout);
    
    // Wavelet type selection
    QHBoxLayout* waveletLayout = new QHBoxLayout();
    QLabel* waveletLabel = new QLabel("Wavelet Type:");
    waveletLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    waveletTypeCombo = new QComboBox();
    waveletTypeCombo->addItem("Haar");
    waveletTypeCombo->addItem("Daubechies-2 (DB2)");
    waveletTypeCombo->addItem("Daubechies-4 (DB4)");
    waveletTypeCombo->addItem("Daubechies-6 (DB6)");
    connect(waveletTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WaveletDialog::onWaveletTypeChanged);
    waveletLayout->addWidget(waveletLabel);
    waveletLayout->addWidget(waveletTypeCombo, 1);
    mainLayout->addLayout(waveletLayout);
    
    // Parameters area with stacked widget
    QGroupBox* paramsGroup = new QGroupBox("Parameters");
    paramsGroup->setStyleSheet(
        "QGroupBox { color: #c4b5fd; font-weight: bold; border: 2px solid rgba(91, 75, 115, 0.5); "
        "border-radius: 8px; margin-top: 10px; padding: 15px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* paramsLayout = new QVBoxLayout(paramsGroup);
    
    QStackedWidget* stackedParams = new QStackedWidget();
    
    // === DENOISING PARAMETERS ===
    denoiseParams = new QWidget();
    QVBoxLayout* denoiseLayout = new QVBoxLayout(denoiseParams);
    
    QHBoxLayout* thresholdLayout = new QHBoxLayout();
    QLabel* thresholdLabelText = new QLabel("Threshold:");
    thresholdLabelText->setStyleSheet("color: #c4b5fd;");
    thresholdSlider = new QSlider(Qt::Horizontal);
    thresholdSlider->setRange(1, 100);
    thresholdSlider->setValue(20);
    thresholdLabel = new QLabel("20");
    thresholdLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(thresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        thresholdLabel->setText(QString::number(value));
        onParameterChanged();
    });
    thresholdLayout->addWidget(thresholdLabelText);
    thresholdLayout->addWidget(thresholdSlider);
    thresholdLayout->addWidget(thresholdLabel);
    denoiseLayout->addLayout(thresholdLayout);
    
    QHBoxLayout* methodLayout = new QHBoxLayout();
    QLabel* methodLabel = new QLabel("Thresholding Method:");
    methodLabel->setStyleSheet("color: #c4b5fd;");
    thresholdMethodCombo = new QComboBox();
    thresholdMethodCombo->addItem("Soft Thresholding");
    thresholdMethodCombo->addItem("Hard Thresholding");
    connect(thresholdMethodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WaveletDialog::onThresholdMethodChanged);
    methodLayout->addWidget(methodLabel);
    methodLayout->addWidget(thresholdMethodCombo);
    methodLayout->addStretch();
    denoiseLayout->addLayout(methodLayout);
    
    QHBoxLayout* levelsLayout = new QHBoxLayout();
    QLabel* levelsLabel = new QLabel("Decomposition Levels:");
    levelsLabel->setStyleSheet("color: #c4b5fd;");
    levelsSpinBox = new QSpinBox();
    levelsSpinBox->setRange(1, 5);
    levelsSpinBox->setValue(3);
    connect(levelsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &WaveletDialog::onParameterChanged);
    levelsLayout->addWidget(levelsLabel);
    levelsLayout->addWidget(levelsSpinBox);
    levelsLayout->addStretch();
    denoiseLayout->addLayout(levelsLayout);
    
    QLabel* denoiseInfo = new QLabel(
        "Threshold: Controls noise removal strength (higher = more aggressive).\n"
        "Soft: Shrinks coefficients gradually (smoother results).\n"
        "Hard: Removes coefficients below threshold completely.\n"
        "Levels: Number of wavelet decomposition levels (1-5)."
    );
    denoiseInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    denoiseLayout->addWidget(denoiseInfo);
    denoiseLayout->addStretch();
    
    stackedParams->addWidget(denoiseParams);
    
    // === DECOMPOSITION PARAMETERS ===
    decompositionParams = new QWidget();
    QVBoxLayout* decompositionLayout = new QVBoxLayout(decompositionParams);
    
    QLabel* decompInfo = new QLabel(
        "Wavelet Decomposition splits image into 4 sub-bands:\n"
        "• Approximation (top-left): Low-frequency content\n"
        "• Horizontal Details (top-right): Horizontal edges\n"
        "• Vertical Details (bottom-left): Vertical edges\n"
        "• Diagonal Details (bottom-right): Diagonal features\n\n"
        "Select 'Decomposition Visualization' to see all components."
    );
    decompInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    decompositionLayout->addWidget(decompInfo);
    decompositionLayout->addStretch();
    
    stackedParams->addWidget(decompositionParams);
    
    paramsLayout->addWidget(stackedParams);
    mainLayout->addWidget(paramsGroup);
    
    // Connect stacked widget switching
    connect(operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, stackedParams](int index) {
        if (index == 0) {
            stackedParams->setCurrentWidget(denoiseParams);
        } else {
            stackedParams->setCurrentWidget(decompositionParams);
        }
    });
    
    // Image display area
    QHBoxLayout* imagesLayout = new QHBoxLayout();
    
    QVBoxLayout* originalLayout = new QVBoxLayout();
    QLabel* originalLabel = new QLabel("Original Image");
    originalLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    originalLabel->setAlignment(Qt::AlignCenter);
    originalCanvas = new ImageCanvas(this, "#e879f9");
    originalCanvas->setMinimumSize(400, 350);
    originalCanvas->setImage(originalImage);
    originalLayout->addWidget(originalLabel);
    originalLayout->addWidget(originalCanvas);
    imagesLayout->addLayout(originalLayout);
    
    QVBoxLayout* processedLayout = new QVBoxLayout();
    QLabel* processedLabel = new QLabel("Processed Result");
    processedLabel->setStyleSheet("color: #c026d3; font-weight: bold;");
    processedLabel->setAlignment(Qt::AlignCenter);
    processedCanvas = new ImageCanvas(this, "#c026d3");
    processedCanvas->setMinimumSize(400, 350);
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
    connect(resetButton, &QPushButton::clicked, this, &WaveletDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);
    
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &WaveletDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void WaveletDialog::onOperationChanged(int index) {
    updatePreview();
}

void WaveletDialog::onWaveletTypeChanged(int index) {
    updatePreview();
}

void WaveletDialog::onThresholdMethodChanged(int index) {
    updatePreview();
}

void WaveletDialog::onParameterChanged() {
    updatePreview();
}

void WaveletDialog::updatePreview() {
    int opIndex = operationCombo->currentIndex();
    
    try {
        if (opIndex == 0) {
            performDenoising();
        } else if (opIndex == 1) {
            performDecomposition();
        } else {
            performReconstruction();
        }
        
        if (!processedImage.empty()) {
            processedCanvas->setImage(processedImage);
            emit previewUpdated(processedImage);
        }
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
    } catch (const std::exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
    }
}

void WaveletDialog::performDenoising() {
    WaveletTransform::WaveletType wType = static_cast<WaveletTransform::WaveletType>(waveletTypeCombo->currentIndex());
    WaveletTransform::ThresholdMethod tMethod = static_cast<WaveletTransform::ThresholdMethod>(thresholdMethodCombo->currentIndex());
    
    double threshold = thresholdSlider->value();
    int levels = levelsSpinBox->value();
    
    // Convert to grayscale if needed
    cv::Mat grayImage;
    if (originalImage.channels() == 3) {
        cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);
        qDebug() << "Converted to grayscale: " << grayImage.rows << "x" << grayImage.cols << " channels=" << grayImage.channels();
    } else {
        grayImage = originalImage.clone();
        qDebug() << "Already grayscale: " << grayImage.rows << "x" << grayImage.cols << " channels=" << grayImage.channels();
    }
    
    // Verify it's really grayscale
    if (grayImage.channels() != 1) {
        infoLabel->setText(QString("ERROR: Image still has %1 channels after conversion!").arg(grayImage.channels()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        return;
    }
    
    try {
        cv::Mat denoised;
        WaveletTransform::denoise(grayImage, denoised, threshold, tMethod, levels, wType);
        
        // denoise() might return CV_64F, convert to CV_8U first
        if (denoised.depth() != CV_8U) {
            denoised.convertTo(processedImage, CV_8U);
        } else {
            processedImage = denoised;
        }
        
        // Then convert to BGR if original was color
        if (originalImage.channels() == 3 && processedImage.channels() == 1) {
            cv::cvtColor(processedImage, processedImage, cv::COLOR_GRAY2BGR);
        }
        
        operationType = QString("Wavelet Denoising (%1, threshold=%2, levels=%3)")
            .arg(WaveletTransform::getWaveletName(wType).c_str())
            .arg(threshold)
            .arg(levels);
        
        infoLabel->setText(operationType);
        infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("OpenCV Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        qDebug() << "OpenCV Exception:" << e.what();
    } catch (const std::exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        qDebug() << "Exception:" << e.what();
    }
}

void WaveletDialog::performDecomposition() {
    WaveletTransform::WaveletType wType = static_cast<WaveletTransform::WaveletType>(waveletTypeCombo->currentIndex());
    
    // Convert to grayscale if needed
    cv::Mat grayImage;
    if (originalImage.channels() == 3) {
        cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = originalImage.clone();
    }
    
    // Verify it's really grayscale
    if (grayImage.channels() != 1) {
        infoLabel->setText(QString("ERROR: Image still has %1 channels after conversion!").arg(grayImage.channels()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        return;
    }
    
    try {
        cv::Mat approx, horiz, vert, diag;
        WaveletTransform::dwt2D(grayImage, approx, horiz, vert, diag, wType);
        
        // Reconstruct to show it's working
        cv::Mat reconstructed;
        WaveletTransform::idwt2D(approx, horiz, vert, diag, reconstructed, wType);
        
        // Convert from CV_64F to CV_8U first
        reconstructed.convertTo(processedImage, CV_8U);
        
        // Then convert to BGR if original was color
        if (originalImage.channels() == 3 && processedImage.channels() == 1) {
            cv::cvtColor(processedImage, processedImage, cv::COLOR_GRAY2BGR);
        }
        
        operationType = QString("Wavelet Decomposition + Reconstruction (%1)")
            .arg(WaveletTransform::getWaveletName(wType).c_str());
        
        infoLabel->setText(operationType);
        infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("OpenCV Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        qDebug() << "OpenCV Exception:" << e.what();
    } catch (const std::exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        qDebug() << "Exception:" << e.what();
    }
}

void WaveletDialog::performReconstruction() {
    WaveletTransform::WaveletType wType = static_cast<WaveletTransform::WaveletType>(waveletTypeCombo->currentIndex());
    
    // Convert to grayscale if needed
    cv::Mat grayImage;
    if (originalImage.channels() == 3) {
        cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = originalImage.clone();
    }
    
    // Verify it's really grayscale
    if (grayImage.channels() != 1) {
        infoLabel->setText(QString("ERROR: Image still has %1 channels after conversion!").arg(grayImage.channels()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        return;
    }
    
    try {
        cv::Mat approx, horiz, vert, diag;
        WaveletTransform::dwt2D(grayImage, approx, horiz, vert, diag, wType);
        
        // Show visualization
        processedImage = WaveletTransform::visualizeDecomposition(approx, horiz, vert, diag);
        
        // Visualization is already in grayscale (8-bit), convert to BGR if original was color
        if (originalImage.channels() == 3 && processedImage.channels() == 1) {
            cv::cvtColor(processedImage, processedImage, cv::COLOR_GRAY2BGR);
        }
        
        operationType = QString("Wavelet Decomposition Visualization (%1)")
            .arg(WaveletTransform::getWaveletName(wType).c_str());
        
        infoLabel->setText(QString("%1 - Shows: Approx | Horiz | Vert | Diag").arg(operationType));
        infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("OpenCV Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        qDebug() << "OpenCV Exception:" << e.what();
    } catch (const std::exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
        qDebug() << "Exception:" << e.what();
    }
}

void WaveletDialog::onApplyClicked() {
    if (!processedImage.empty()) {
        applied = true;
        accept();
    } else {
        QMessageBox::warning(this, "No Preview", "No processed image to apply!");
    }
}

void WaveletDialog::onResetClicked() {
    thresholdSlider->setValue(20);
    thresholdMethodCombo->setCurrentIndex(0);
    levelsSpinBox->setValue(3);
    waveletTypeCombo->setCurrentIndex(0);
    operationCombo->setCurrentIndex(0);
    
    updatePreview();
}
#include "moc_WaveletDialog.cpp"
