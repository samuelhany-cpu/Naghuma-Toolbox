#include "AutoEnhanceDialog.h"
#include "ImageCanvas.h"
#include "ImageProcessor.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <cmath>

AutoEnhanceDialog::AutoEnhanceDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent),
      originalImage(image.clone()),
      algorithmType("Adaptive Histogram"),
      rmse(0.0),
      psnr(0.0),
      applied(false) {
    
    setWindowTitle("Auto Enhancement");
    setMinimumSize(1000, 700);
    setStyleSheet(Theme::MAIN_STYLESHEET);
    
    setupUI();
    updateEnhancement();
}

AutoEnhanceDialog::~AutoEnhanceDialog() {
}

void AutoEnhanceDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Auto Enhance Image");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Algorithm selection group
    QGroupBox *algorithmGroup = new QGroupBox("Enhancement Algorithm");
    algorithmGroup->setStyleSheet(
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
    
    QVBoxLayout *algoLayout = new QVBoxLayout(algorithmGroup);
    
    this->algorithmGroup = new QButtonGroup(this);
    
    adaptiveHistRadio = new QRadioButton("Adaptive Histogram Equalization (CLAHE)");
    adaptiveHistRadio->setStyleSheet("color: #c4b5fd;");
    adaptiveHistRadio->setChecked(true);
    this->algorithmGroup->addButton(adaptiveHistRadio, 0);
    algoLayout->addWidget(adaptiveHistRadio);
    
    QLabel *adaptiveDesc = new QLabel("• Enhances local contrast using tiles\n• Best for images with varying lighting");
    adaptiveDesc->setStyleSheet("color: #9ca3af; font-size: 9pt; margin-left: 20px;");
    algoLayout->addWidget(adaptiveDesc);
    
    algoLayout->addSpacing(10);
    
    contrastStretchRadio = new QRadioButton("Contrast Stretching");
    contrastStretchRadio->setStyleSheet("color: #c4b5fd;");
    this->algorithmGroup->addButton(contrastStretchRadio, 1);
    algoLayout->addWidget(contrastStretchRadio);
    
    QLabel *contrastDesc = new QLabel("• Stretches intensity range to full 0-255\n• Best for low-contrast images");
    contrastDesc->setStyleSheet("color: #9ca3af; font-size: 9pt; margin-left: 20px;");
    algoLayout->addWidget(contrastDesc);
    
    connect(this->algorithmGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &AutoEnhanceDialog::onAlgorithmChanged);
    
    mainLayout->addWidget(algorithmGroup);
    
    mainLayout->addSpacing(10);
    
    // Comparison view (Original vs Enhanced)
    QGroupBox *comparisonGroup = new QGroupBox("Before / After Comparison");
    comparisonGroup->setStyleSheet(algorithmGroup->styleSheet());
    QHBoxLayout *comparisonLayout = new QHBoxLayout(comparisonGroup);
    
    // Original image
    QVBoxLayout *originalLayout = new QVBoxLayout();
    QLabel *originalTitle = new QLabel("Original");
    originalTitle->setStyleSheet("color: #e879f9; font-weight: bold; font-size: 11pt;");
    originalTitle->setAlignment(Qt::AlignCenter);
    originalLayout->addWidget(originalTitle);
    
    originalCanvas = new ImageCanvas(this, "#e879f9");
    originalCanvas->setMinimumSize(400, 300);
    originalCanvas->setImage(originalImage);
    originalLayout->addWidget(originalCanvas);
    
    comparisonLayout->addLayout(originalLayout);
    
    // Enhanced image
    QVBoxLayout *enhancedLayout = new QVBoxLayout();
    QLabel *enhancedTitle = new QLabel("Enhanced");
    enhancedTitle->setStyleSheet("color: #10b981; font-weight: bold; font-size: 11pt;");
    enhancedTitle->setAlignment(Qt::AlignCenter);
    enhancedLayout->addWidget(enhancedTitle);
    
    enhancedCanvas = new ImageCanvas(this, "#10b981");
    enhancedCanvas->setMinimumSize(400, 300);
    enhancedLayout->addWidget(enhancedCanvas);
    
    comparisonLayout->addLayout(enhancedLayout);
    
    mainLayout->addWidget(comparisonGroup);
    
    // Metrics display
    QGroupBox *metricsGroup = new QGroupBox("Quality Metrics");
    metricsGroup->setStyleSheet(algorithmGroup->styleSheet());
    QFormLayout *metricsLayout = new QFormLayout(metricsGroup);
    metricsLayout->setLabelAlignment(Qt::AlignRight);
    
    QString labelStyle = "color: #c4b5fd; font-size: 10pt;";
    QString valueStyle = "color: #e879f9; font-weight: bold; font-size: 10pt;";
    
    algorithmDescLabel = new QLabel("Adaptive Histogram Equalization");
    algorithmDescLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("Algorithm:", nullptr), algorithmDescLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    rmseLabel = new QLabel("0.00");
    rmseLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("RMSE:", nullptr), rmseLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    psnrLabel = new QLabel("? dB");
    psnrLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("PSNR:", nullptr), psnrLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    qualityAssessmentLabel = new QLabel("Excellent Quality");
    qualityAssessmentLabel->setStyleSheet("color: #10b981; font-weight: bold; font-size: 10pt;");
    metricsLayout->addRow(new QLabel("Quality:", nullptr), qualityAssessmentLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    mainLayout->addWidget(metricsGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &AutoEnhanceDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply Enhancement");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(150);
    connect(applyButton, &QPushButton::clicked, this, &AutoEnhanceDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void AutoEnhanceDialog::onAlgorithmChanged() {
    if (adaptiveHistRadio->isChecked()) {
        algorithmType = "Adaptive Histogram";
        algorithmDescLabel->setText("Adaptive Histogram Equalization (CLAHE)");
    } else {
        algorithmType = "Contrast Stretching";
        algorithmDescLabel->setText("Contrast Stretching");
    }
    
    updateEnhancement();
}

void AutoEnhanceDialog::updateEnhancement() {
    if (algorithmType == "Adaptive Histogram") {
        enhancedImage = applyAdaptiveHistogramEqualization();
    } else {
        enhancedImage = applyContrastStretching();
    }
    
    updateComparison();
    updateMetrics();
    emit enhancementUpdated(enhancedImage);
}

cv::Mat AutoEnhanceDialog::applyAdaptiveHistogramEqualization() {
    cv::Mat result;
    ImageProcessor::applyAdaptiveHistogramEqualization(originalImage, result);
    return result;
}

cv::Mat AutoEnhanceDialog::applyContrastStretching() {
    cv::Mat result;
    ImageProcessor::applyContrastStretching(originalImage, result);
    return result;
}

void AutoEnhanceDialog::updateComparison() {
    if (!enhancedImage.empty()) {
        enhancedCanvas->setImage(enhancedImage);
    }
}

void AutoEnhanceDialog::updateMetrics() {
    if (enhancedImage.empty()) {
        return;
    }
    
    // Calculate RMSE and PSNR
    rmse = calculateRMSE(originalImage, enhancedImage);
    psnr = calculatePSNR(rmse);
    
    // Update labels
    rmseLabel->setText(QString("%1").arg(rmse, 0, 'f', 2));
    
    if (std::isinf(psnr)) {
        psnrLabel->setText("? dB");
    } else {
        psnrLabel->setText(QString("%1 dB").arg(psnr, 0, 'f', 2));
    }
    
    // Quality assessment
    QString assessment;
    QString assessmentColor;
    if (psnr >= 40 || std::isinf(psnr)) {
        assessment = "Excellent Enhancement";
        assessmentColor = "#10b981"; // Green
    } else if (psnr >= 30) {
        assessment = "Good Enhancement";
        assessmentColor = "#3b82f6"; // Blue
    } else if (psnr >= 20) {
        assessment = "Fair Enhancement";
        assessmentColor = "#f59e0b"; // Orange
    } else {
        assessment = "Minimal Enhancement";
        assessmentColor = "#ef4444"; // Red
    }
    
    qualityAssessmentLabel->setText(assessment);
    qualityAssessmentLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 10pt;").arg(assessmentColor));
}

double AutoEnhanceDialog::calculateRMSE(const cv::Mat& img1, const cv::Mat& img2) {
    if (img1.empty() || img2.empty() || img1.size() != img2.size()) {
        return 0.0;
    }
    
    cv::Mat img1_converted, img2_converted;
    
    // Convert to same type for comparison
    if (img1.channels() == 3 && img2.channels() == 3) {
        img1.convertTo(img1_converted, CV_64F);
        img2.convertTo(img2_converted, CV_64F);
    } else if (img1.channels() == 1 && img2.channels() == 1) {
        img1.convertTo(img1_converted, CV_64F);
        img2.convertTo(img2_converted, CV_64F);
    } else {
        // Convert both to grayscale if different channel counts
        if (img1.channels() == 3) {
            cv::cvtColor(img1, img1_converted, cv::COLOR_BGR2GRAY);
        } else {
            img1_converted = img1.clone();
        }
        if (img2.channels() == 3) {
            cv::cvtColor(img2, img2_converted, cv::COLOR_BGR2GRAY);
        } else {
            img2_converted = img2.clone();
        }
        img1_converted.convertTo(img1_converted, CV_64F);
        img2_converted.convertTo(img2_converted, CV_64F);
    }
    
    cv::Mat diff;
    cv::subtract(img1_converted, img2_converted, diff);
    diff = diff.mul(diff);
    
    cv::Scalar s = cv::sum(diff);
    double sse = s[0] + s[1] + s[2];
    
    double mse = sse / (double)(img1_converted.total() * img1_converted.channels());
    return std::sqrt(mse);
}

double AutoEnhanceDialog::calculatePSNR(double rmse) {
    if (rmse == 0.0) {
        return std::numeric_limits<double>::infinity();
    }
    
    double maxPixelValue = 255.0;
    double mse = rmse * rmse;
    return 10.0 * std::log10((maxPixelValue * maxPixelValue) / mse);
}

void AutoEnhanceDialog::onApplyClicked() {
    applied = true;
    accept();
}

void AutoEnhanceDialog::onCancelClicked() {
    applied = false;
    reject();
}
