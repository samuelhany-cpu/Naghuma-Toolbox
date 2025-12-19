#include "FeatureDetectionDialog.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStackedWidget>
#include <opencv2/features2d.hpp>

FeatureDetectionDialog::FeatureDetectionDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), inputImage(image.clone()), applied(false), featureCount(0) {
    
    setWindowTitle("Feature Detection - Phase 19");
    setMinimumSize(1000, 700);
    
    setupUI();
    onMethodChanged(0);  // Initialize with first method
}

FeatureDetectionDialog::~FeatureDetectionDialog() {
}

void FeatureDetectionDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Feature & Corner Detection");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Method selection
    QHBoxLayout* methodLayout = new QHBoxLayout();
    QLabel* methodLabel = new QLabel("Detection Method:");
    methodLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    methodCombo = new QComboBox();
    methodCombo->addItem("Harris Corner Detection");
    methodCombo->addItem("Shi-Tomasi (Good Features)");
    methodCombo->addItem("FAST Corner Detection");
    methodCombo->addItem("ORB Features");
    connect(methodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &FeatureDetectionDialog::onMethodChanged);
    methodLayout->addWidget(methodLabel);
    methodLayout->addWidget(methodCombo, 1);
    mainLayout->addLayout(methodLayout);
    
    // Parameters area with stacked widget
    QGroupBox* paramsGroup = new QGroupBox("Parameters");
    paramsGroup->setStyleSheet(
        "QGroupBox { color: #c4b5fd; font-weight: bold; border: 2px solid rgba(91, 75, 115, 0.5); "
        "border-radius: 8px; margin-top: 10px; padding: 15px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* paramsLayout = new QVBoxLayout(paramsGroup);
    
    QStackedWidget* stackedParams = new QStackedWidget();
    
    // === HARRIS PARAMETERS ===
    harrisParams = new QWidget();
    QVBoxLayout* harrisLayout = new QVBoxLayout(harrisParams);
    
    QHBoxLayout* harrisBlockLayout = new QHBoxLayout();
    QLabel* harrisBlockLabel = new QLabel("Block Size:");
    harrisBlockLabel->setStyleSheet("color: #c4b5fd;");
    harrisBlockSizeSpin = new QSpinBox();
    harrisBlockSizeSpin->setRange(2, 31);
    harrisBlockSizeSpin->setValue(2);
    harrisBlockSizeSpin->setSingleStep(1);
    connect(harrisBlockSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    harrisBlockLayout->addWidget(harrisBlockLabel);
    harrisBlockLayout->addWidget(harrisBlockSizeSpin);
    harrisBlockLayout->addStretch();
    harrisLayout->addLayout(harrisBlockLayout);
    
    QHBoxLayout* harrisApertureLayout = new QHBoxLayout();
    QLabel* harrisApertureLabel = new QLabel("Aperture Size:");
    harrisApertureLabel->setStyleSheet("color: #c4b5fd;");
    harrisApertureSpin = new QSpinBox();
    harrisApertureSpin->setRange(3, 31);
    harrisApertureSpin->setValue(3);
    harrisApertureSpin->setSingleStep(2);
    connect(harrisApertureSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    harrisApertureLayout->addWidget(harrisApertureLabel);
    harrisApertureLayout->addWidget(harrisApertureSpin);
    harrisApertureLayout->addStretch();
    harrisLayout->addLayout(harrisApertureLayout);
    
    QHBoxLayout* harrisKLayout = new QHBoxLayout();
    QLabel* harrisKLabelText = new QLabel("Harris K:");
    harrisKLabelText->setStyleSheet("color: #c4b5fd;");
    harrisKSlider = new QSlider(Qt::Horizontal);
    harrisKSlider->setRange(1, 50);
    harrisKSlider->setValue(4);
    harrisKLabel = new QLabel("0.04");
    harrisKLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(harrisKSlider, &QSlider::valueChanged, this, [this](int value) {
        harrisKLabel->setText(QString::number(value / 100.0, 'f', 2));
        onParameterChanged();
    });
    harrisKLayout->addWidget(harrisKLabelText);
    harrisKLayout->addWidget(harrisKSlider);
    harrisKLayout->addWidget(harrisKLabel);
    harrisLayout->addLayout(harrisKLayout);
    
    QHBoxLayout* harrisThreshLayout = new QHBoxLayout();
    QLabel* harrisThreshLabelText = new QLabel("Threshold:");
    harrisThreshLabelText->setStyleSheet("color: #c4b5fd;");
    harrisThresholdSlider = new QSlider(Qt::Horizontal);
    harrisThresholdSlider->setRange(1, 255);
    harrisThresholdSlider->setValue(100);
    harrisThresholdLabel = new QLabel("100");
    harrisThresholdLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(harrisThresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        harrisThresholdLabel->setText(QString::number(value));
        onParameterChanged();
    });
    harrisThreshLayout->addWidget(harrisThreshLabelText);
    harrisThreshLayout->addWidget(harrisThresholdSlider);
    harrisThreshLayout->addWidget(harrisThresholdLabel);
    harrisLayout->addLayout(harrisThreshLayout);
    
    QLabel* harrisInfo = new QLabel(
        "Harris detector finds corners based on intensity gradient.\n"
        "Lower threshold = more corners detected."
    );
    harrisInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    harrisLayout->addWidget(harrisInfo);
    harrisLayout->addStretch();
    
    stackedParams->addWidget(harrisParams);
    
    // === SHI-TOMASI PARAMETERS ===
    shiTomasiParams = new QWidget();
    QVBoxLayout* shiTomasiLayout = new QVBoxLayout(shiTomasiParams);
    
    QHBoxLayout* shiTomasiMaxLayout = new QHBoxLayout();
    QLabel* shiTomasiMaxLabel = new QLabel("Max Corners:");
    shiTomasiMaxLabel->setStyleSheet("color: #c4b5fd;");
    shiTomasiMaxCornersSpin = new QSpinBox();
    shiTomasiMaxCornersSpin->setRange(10, 10000);
    shiTomasiMaxCornersSpin->setValue(100);
    shiTomasiMaxCornersSpin->setSingleStep(10);
    connect(shiTomasiMaxCornersSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    shiTomasiMaxLayout->addWidget(shiTomasiMaxLabel);
    shiTomasiMaxLayout->addWidget(shiTomasiMaxCornersSpin);
    shiTomasiMaxLayout->addStretch();
    shiTomasiLayout->addLayout(shiTomasiMaxLayout);
    
    QHBoxLayout* shiTomasiQualityLayout = new QHBoxLayout();
    QLabel* shiTomasiQualityLabelText = new QLabel("Quality Level:");
    shiTomasiQualityLabelText->setStyleSheet("color: #c4b5fd;");
    shiTomasiQualitySlider = new QSlider(Qt::Horizontal);
    shiTomasiQualitySlider->setRange(1, 100);
    shiTomasiQualitySlider->setValue(1);
    shiTomasiQualityLabel = new QLabel("0.01");
    shiTomasiQualityLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(shiTomasiQualitySlider, &QSlider::valueChanged, this, [this](int value) {
        shiTomasiQualityLabel->setText(QString::number(value / 100.0, 'f', 2));
        onParameterChanged();
    });
    shiTomasiQualityLayout->addWidget(shiTomasiQualityLabelText);
    shiTomasiQualityLayout->addWidget(shiTomasiQualitySlider);
    shiTomasiQualityLayout->addWidget(shiTomasiQualityLabel);
    shiTomasiLayout->addLayout(shiTomasiQualityLayout);
    
    QHBoxLayout* shiTomasiDistLayout = new QHBoxLayout();
    QLabel* shiTomasiDistLabel = new QLabel("Min Distance:");
    shiTomasiDistLabel->setStyleSheet("color: #c4b5fd;");
    shiTomasiMinDistanceSpin = new QSpinBox();
    shiTomasiMinDistanceSpin->setRange(1, 100);
    shiTomasiMinDistanceSpin->setValue(10);
    connect(shiTomasiMinDistanceSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    shiTomasiDistLayout->addWidget(shiTomasiDistLabel);
    shiTomasiDistLayout->addWidget(shiTomasiMinDistanceSpin);
    shiTomasiDistLayout->addStretch();
    shiTomasiLayout->addLayout(shiTomasiDistLayout);
    
    QHBoxLayout* shiTomasiBlockLayout = new QHBoxLayout();
    QLabel* shiTomasiBlockLabel = new QLabel("Block Size:");
    shiTomasiBlockLabel->setStyleSheet("color: #c4b5fd;");
    shiTomasiBlockSizeSpin = new QSpinBox();
    shiTomasiBlockSizeSpin->setRange(3, 31);
    shiTomasiBlockSizeSpin->setValue(3);
    shiTomasiBlockSizeSpin->setSingleStep(2);
    connect(shiTomasiBlockSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    shiTomasiBlockLayout->addWidget(shiTomasiBlockLabel);
    shiTomasiBlockLayout->addWidget(shiTomasiBlockSizeSpin);
    shiTomasiBlockLayout->addStretch();
    shiTomasiLayout->addLayout(shiTomasiBlockLayout);
    
    QLabel* shiTomasiInfo = new QLabel(
        "Shi-Tomasi finds strong corners for tracking.\n"
        "Quality level: minimum acceptable corner quality (0.01 = 1%)."
    );
    shiTomasiInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    shiTomasiLayout->addWidget(shiTomasiInfo);
    shiTomasiLayout->addStretch();
    
    stackedParams->addWidget(shiTomasiParams);
    
    // === FAST PARAMETERS ===
    fastParams = new QWidget();
    QVBoxLayout* fastLayout = new QVBoxLayout(fastParams);
    
    QHBoxLayout* fastThreshLayout = new QHBoxLayout();
    QLabel* fastThreshLabel = new QLabel("Threshold:");
    fastThreshLabel->setStyleSheet("color: #c4b5fd;");
    fastThresholdSpin = new QSpinBox();
    fastThresholdSpin->setRange(1, 255);
    fastThresholdSpin->setValue(10);
    connect(fastThresholdSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    fastThreshLayout->addWidget(fastThreshLabel);
    fastThreshLayout->addWidget(fastThresholdSpin);
    fastThreshLayout->addStretch();
    fastLayout->addLayout(fastThreshLayout);
    
    QHBoxLayout* fastNMSLayout = new QHBoxLayout();
    fastNonMaxSuppressionCheck = new QCheckBox("Non-Maximum Suppression");
    fastNonMaxSuppressionCheck->setChecked(true);
    fastNonMaxSuppressionCheck->setStyleSheet("color: #c4b5fd;");
    connect(fastNonMaxSuppressionCheck, &QCheckBox::toggled,
            this, &FeatureDetectionDialog::onParameterChanged);
    fastNMSLayout->addWidget(fastNonMaxSuppressionCheck);
    fastNMSLayout->addStretch();
    fastLayout->addLayout(fastNMSLayout);
    
    QHBoxLayout* fastTypeLayout = new QHBoxLayout();
    QLabel* fastTypeLabel = new QLabel("FAST Type:");
    fastTypeLabel->setStyleSheet("color: #c4b5fd;");
    fastTypeCombo = new QComboBox();
    fastTypeCombo->addItem("TYPE_9_16");
    fastTypeCombo->addItem("TYPE_7_12");
    fastTypeCombo->addItem("TYPE_5_8");
    connect(fastTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    fastTypeLayout->addWidget(fastTypeLabel);
    fastTypeLayout->addWidget(fastTypeCombo);
    fastTypeLayout->addStretch();
    fastLayout->addLayout(fastTypeLayout);
    
    QLabel* fastInfo = new QLabel(
        "FAST (Features from Accelerated Segment Test).\n"
        "Very fast corner detection, ideal for real-time applications.\n"
        "Lower threshold = more corners detected."
    );
    fastInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    fastLayout->addWidget(fastInfo);
    fastLayout->addStretch();
    
    stackedParams->addWidget(fastParams);
    
    // === ORB PARAMETERS ===
    orbParams = new QWidget();
    QVBoxLayout* orbLayout = new QVBoxLayout(orbParams);
    
    QHBoxLayout* orbMaxLayout = new QHBoxLayout();
    QLabel* orbMaxLabel = new QLabel("Max Features:");
    orbMaxLabel->setStyleSheet("color: #c4b5fd;");
    orbMaxFeaturesSpin = new QSpinBox();
    orbMaxFeaturesSpin->setRange(10, 10000);
    orbMaxFeaturesSpin->setValue(500);
    orbMaxFeaturesSpin->setSingleStep(50);
    connect(orbMaxFeaturesSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    orbMaxLayout->addWidget(orbMaxLabel);
    orbMaxLayout->addWidget(orbMaxFeaturesSpin);
    orbMaxLayout->addStretch();
    orbLayout->addLayout(orbMaxLayout);
    
    QHBoxLayout* orbScaleLayout = new QHBoxLayout();
    QLabel* orbScaleLabelText = new QLabel("Scale Factor:");
    orbScaleLabelText->setStyleSheet("color: #c4b5fd;");
    orbScaleFactorSlider = new QSlider(Qt::Horizontal);
    orbScaleFactorSlider->setRange(110, 200);
    orbScaleFactorSlider->setValue(120);
    orbScaleFactorLabel = new QLabel("1.2");
    orbScaleFactorLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(orbScaleFactorSlider, &QSlider::valueChanged, this, [this](int value) {
        orbScaleFactorLabel->setText(QString::number(value / 100.0, 'f', 2));
        onParameterChanged();
    });
    orbScaleLayout->addWidget(orbScaleLabelText);
    orbScaleLayout->addWidget(orbScaleFactorSlider);
    orbScaleLayout->addWidget(orbScaleFactorLabel);
    orbLayout->addLayout(orbScaleLayout);
    
    QHBoxLayout* orbLevelsLayout = new QHBoxLayout();
    QLabel* orbLevelsLabel = new QLabel("Pyramid Levels:");
    orbLevelsLabel->setStyleSheet("color: #c4b5fd;");
    orbLevelsSpin = new QSpinBox();
    orbLevelsSpin->setRange(1, 16);
    orbLevelsSpin->setValue(8);
    connect(orbLevelsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    orbLevelsLayout->addWidget(orbLevelsLabel);
    orbLevelsLayout->addWidget(orbLevelsSpin);
    orbLevelsLayout->addStretch();
    orbLayout->addLayout(orbLevelsLayout);
    
    QHBoxLayout* orbEdgeLayout = new QHBoxLayout();
    QLabel* orbEdgeLabel = new QLabel("Edge Threshold:");
    orbEdgeLabel->setStyleSheet("color: #c4b5fd;");
    orbEdgeThresholdSpin = new QSpinBox();
    orbEdgeThresholdSpin->setRange(1, 100);
    orbEdgeThresholdSpin->setValue(31);
    connect(orbEdgeThresholdSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FeatureDetectionDialog::onParameterChanged);
    orbEdgeLayout->addWidget(orbEdgeLabel);
    orbEdgeLayout->addWidget(orbEdgeThresholdSpin);
    orbEdgeLayout->addStretch();
    orbLayout->addLayout(orbEdgeLayout);
    
    QLabel* orbInfo = new QLabel(
        "ORB (Oriented FAST and Rotated BRIEF).\n"
        "Rotation-invariant features, good for object recognition."
    );
    orbInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    orbLayout->addWidget(orbInfo);
    orbLayout->addStretch();
    
    stackedParams->addWidget(orbParams);
    
    paramsLayout->addWidget(stackedParams);
    mainLayout->addWidget(paramsGroup);
    
    // Store reference for method switching
    connect(methodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            stackedParams, &QStackedWidget::setCurrentIndex);
    
    // Feature count label
    featureCountLabel = new QLabel("Features: 0");
    featureCountLabel->setStyleSheet("color: #10b981; font-weight: bold; font-size: 11pt; padding: 5px;");
    featureCountLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(featureCountLabel);
    
    // Preview canvas
    QLabel* previewLabel = new QLabel("Preview:");
    previewLabel->setStyleSheet("color: #c4b5fd; font-weight: bold; padding-top: 10px;");
    mainLayout->addWidget(previewLabel);
    
    previewCanvas = new ImageCanvas(this, "#e879f9");
    previewCanvas->setMinimumSize(600, 400);
    mainLayout->addWidget(previewCanvas, 1);
    
    // Info label
    infoLabel = new QLabel("Adjust parameters and preview will update automatically");
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    resetButton = new QPushButton("Reset");
    resetButton->setMinimumWidth(100);
    connect(resetButton, &QPushButton::clicked, this, &FeatureDetectionDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);
    
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &FeatureDetectionDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void FeatureDetectionDialog::onMethodChanged(int index) {
    updatePreview();
}

void FeatureDetectionDialog::onParameterChanged() {
    updatePreview();
}

void FeatureDetectionDialog::updatePreview() {
    int method = methodCombo->currentIndex();
    
    try {
        switch (method) {
            case 0: detectHarrisCorners(); break;
            case 1: detectShiTomasiCorners(); break;
            case 2: detectFASTCorners(); break;
            case 3: detectORBFeatures(); break;
            default: break;
        }
        
        if (!previewImage.empty()) {
            previewCanvas->setImage(previewImage);
            emit previewUpdated(previewImage);
            featureCountLabel->setText(QString("Features Detected: %1").arg(featureCount));
        }
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
    }
}

void FeatureDetectionDialog::detectHarrisCorners() {
    cv::Mat gray, harris, harrisNorm, harrisScaled;
    
    // Convert to grayscale
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = inputImage.clone();
    }
    
    // Harris corner detection
    int blockSize = harrisBlockSizeSpin->value();
    int apertureSize = harrisApertureSpin->value();
    double k = harrisKSlider->value() / 100.0;
    
    cv::cornerHarris(gray, harris, blockSize, apertureSize, k);
    
    // Normalize and convert
    cv::normalize(harris, harrisNorm, 0, 255, cv::NORM_MINMAX);
    cv::convertScaleAbs(harrisNorm, harrisScaled);
    
    // Threshold to get corners
    int threshold = harrisThresholdSlider->value();
    
    // Draw corners on image
    previewImage = inputImage.clone();
    featureCount = 0;
    
    for (int i = 0; i < harrisNorm.rows; i++) {
        for (int j = 0; j < harrisNorm.cols; j++) {
            if (harrisNorm.at<float>(i, j) > threshold) {
                cv::circle(previewImage, cv::Point(j, i), 5, cv::Scalar(0, 255, 0), 2);
                featureCount++;
            }
        }
    }
    
    detectionType = QString("Harris Corners (k=%.2f)").arg(k);
    infoLabel->setText(QString("Harris corner detection: %1 corners found").arg(featureCount));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FeatureDetectionDialog::detectShiTomasiCorners() {
    cv::Mat gray;
    
    // Convert to grayscale
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = inputImage.clone();
    }
    
    // Shi-Tomasi parameters
    int maxCorners = shiTomasiMaxCornersSpin->value();
    double qualityLevel = shiTomasiQualitySlider->value() / 100.0;
    double minDistance = shiTomasiMinDistanceSpin->value();
    int blockSize = shiTomasiBlockSizeSpin->value();
    
    // Detect corners
    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(gray, corners, maxCorners, qualityLevel, minDistance, 
                           cv::Mat(), blockSize, false, 0.04);
    
    // Draw corners
    previewImage = inputImage.clone();
    featureCount = corners.size();
    
    for (const auto& corner : corners) {
        cv::circle(previewImage, corner, 5, cv::Scalar(0, 255, 0), 2);
    }
    
    detectionType = QString("Shi-Tomasi Corners (quality=%.2f)").arg(qualityLevel);
    infoLabel->setText(QString("Shi-Tomasi detection: %1 good features found").arg(featureCount));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FeatureDetectionDialog::detectFASTCorners() {
    cv::Mat gray;
    
    // Convert to grayscale
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = inputImage.clone();
    }
    
    // FAST parameters
    int threshold = fastThresholdSpin->value();
    bool nonMaxSuppression = fastNonMaxSuppressionCheck->isChecked();
    
    int fastType = cv::FastFeatureDetector::TYPE_9_16;
    switch (fastTypeCombo->currentIndex()) {
        case 0: fastType = cv::FastFeatureDetector::TYPE_9_16; break;
        case 1: fastType = cv::FastFeatureDetector::TYPE_7_12; break;
        case 2: fastType = cv::FastFeatureDetector::TYPE_5_8; break;
    }
    
    // Detect FAST corners
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create(
        threshold, nonMaxSuppression, fastType
    );
    detector->detect(gray, keypoints);
    
    // Draw keypoints
    previewImage = inputImage.clone();
    cv::drawKeypoints(previewImage, keypoints, previewImage, cv::Scalar(0, 255, 0),
                     cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    
    featureCount = keypoints.size();
    detectionType = QString("FAST Corners (threshold=%1)").arg(threshold);
    infoLabel->setText(QString("FAST detection: %1 corners found").arg(featureCount));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FeatureDetectionDialog::detectORBFeatures() {
    cv::Mat gray;
    
    // Convert to grayscale
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = inputImage.clone();
    }
    
    // ORB parameters
    int nfeatures = orbMaxFeaturesSpin->value();
    float scaleFactor = orbScaleFactorSlider->value() / 100.0f;
    int nlevels = orbLevelsSpin->value();
    int edgeThreshold = orbEdgeThresholdSpin->value();
    
    // Create ORB detector
    cv::Ptr<cv::ORB> orb = cv::ORB::create(
        nfeatures, scaleFactor, nlevels, edgeThreshold
    );
    
    // Detect and compute
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    orb->detectAndCompute(gray, cv::Mat(), keypoints, descriptors);
    
    // Draw keypoints with rich info
    previewImage = inputImage.clone();
    cv::drawKeypoints(previewImage, keypoints, previewImage, cv::Scalar(0, 255, 0),
                     cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    featureCount = keypoints.size();
    detectionType = QString("ORB Features (max=%1)").arg(nfeatures);
    infoLabel->setText(QString("ORB detection: %1 features with descriptors").arg(featureCount));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FeatureDetectionDialog::onApplyClicked() {
    if (!previewImage.empty()) {
        detectedImage = previewImage.clone();
        applied = true;
        accept();
    } else {
        QMessageBox::warning(this, "No Preview", "No feature detection result to apply!");
    }
}

void FeatureDetectionDialog::onResetClicked() {
    // Reset all parameters to default
    harrisBlockSizeSpin->setValue(2);
    harrisApertureSpin->setValue(3);
    harrisKSlider->setValue(4);
    harrisThresholdSlider->setValue(100);
    
    shiTomasiMaxCornersSpin->setValue(100);
    shiTomasiQualitySlider->setValue(1);
    shiTomasiMinDistanceSpin->setValue(10);
    shiTomasiBlockSizeSpin->setValue(3);
    
    fastThresholdSpin->setValue(10);
    fastNonMaxSuppressionCheck->setChecked(true);
    fastTypeCombo->setCurrentIndex(0);
    
    orbMaxFeaturesSpin->setValue(500);
    orbScaleFactorSlider->setValue(120);
    orbLevelsSpin->setValue(8);
    orbEdgeThresholdSpin->setValue(31);
    
    updatePreview();
}
