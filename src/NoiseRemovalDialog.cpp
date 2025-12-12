#include "NoiseRemovalDialog.h"
#include "ImageCanvas.h"
#include "ImageProcessor.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <cmath>

NoiseRemovalDialog::NoiseRemovalDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent),
      originalImage(image.clone()),
      filterType("Gaussian"),
      kernelSize(5),
      sigmaColor(75.0),
      sigmaSpace(75.0),
      originalSNR(0.0),
      filteredSNR(0.0),
      snrImprovement(0.0),
      psnr(0.0),
      applied(false) {
    
    setWindowTitle("Noise Removal");
    setMinimumSize(1000, 750);
    setStyleSheet(Theme::MAIN_STYLESHEET);
    
    // Calculate original SNR
    originalSNR = calculateSNR(originalImage);
    
    setupUI();
    updateFilter();
}

NoiseRemovalDialog::~NoiseRemovalDialog() {
}

void NoiseRemovalDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Noise Removal");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Filter selection group
    QGroupBox *filterSelectionGroup = new QGroupBox("Filter Type");
    filterSelectionGroup->setStyleSheet(
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
    
    QHBoxLayout *filterSelectionLayout = new QHBoxLayout(filterSelectionGroup);
    
    filterGroup = new QButtonGroup(this);
    
    gaussianRadio = new QRadioButton("Gaussian Blur");
    gaussianRadio->setStyleSheet("color: #c4b5fd;");
    gaussianRadio->setChecked(true);
    filterGroup->addButton(gaussianRadio, 0);
    filterSelectionLayout->addWidget(gaussianRadio);
    
    medianRadio = new QRadioButton("Median Filter");
    medianRadio->setStyleSheet("color: #c4b5fd;");
    filterGroup->addButton(medianRadio, 1);
    filterSelectionLayout->addWidget(medianRadio);
    
    bilateralRadio = new QRadioButton("Bilateral Filter");
    bilateralRadio->setStyleSheet("color: #c4b5fd;");
    filterGroup->addButton(bilateralRadio, 2);
    filterSelectionLayout->addWidget(bilateralRadio);
    
    connect(filterGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &NoiseRemovalDialog::onFilterTypeChanged);
    
    mainLayout->addWidget(filterSelectionGroup);
    
    mainLayout->addSpacing(10);
    
    // Filter parameters
    QHBoxLayout *paramsLayout = new QHBoxLayout();
    
    // Gaussian parameters
    gaussianGroup = new QGroupBox("Gaussian Parameters");
    gaussianGroup->setStyleSheet(filterSelectionGroup->styleSheet());
    QVBoxLayout *gaussianLayout = new QVBoxLayout(gaussianGroup);
    
    QHBoxLayout *gaussianKernelLayout = new QHBoxLayout();
    QLabel *gaussianKernelTextLabel = new QLabel("Kernel Size:");
    gaussianKernelTextLabel->setStyleSheet("color: #c4b5fd;");
    gaussianKernelSlider = new QSlider(Qt::Horizontal);
    gaussianKernelSlider->setRange(3, 15);
    gaussianKernelSlider->setValue(5);
    gaussianKernelSlider->setSingleStep(2);
    gaussianKernelLabel = new QLabel("5");
    gaussianKernelLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 30px;");
    gaussianKernelLabel->setAlignment(Qt::AlignCenter);
    connect(gaussianKernelSlider, &QSlider::valueChanged, this, &NoiseRemovalDialog::onKernelSizeChanged);
    gaussianKernelLayout->addWidget(gaussianKernelTextLabel);
    gaussianKernelLayout->addWidget(gaussianKernelSlider);
    gaussianKernelLayout->addWidget(gaussianKernelLabel);
    gaussianLayout->addLayout(gaussianKernelLayout);
    
    QLabel *gaussianInfo = new QLabel("Smooths image, good for general noise");
    gaussianInfo->setStyleSheet("color: #9ca3af; font-size: 9pt;");
    gaussianLayout->addWidget(gaussianInfo);
    
    paramsLayout->addWidget(gaussianGroup);
    
    // Median parameters
    medianGroup = new QGroupBox("Median Parameters");
    medianGroup->setStyleSheet(filterSelectionGroup->styleSheet());
    medianGroup->setVisible(false);
    QVBoxLayout *medianLayout = new QVBoxLayout(medianGroup);
    
    QHBoxLayout *medianKernelLayout = new QHBoxLayout();
    QLabel *medianKernelTextLabel = new QLabel("Kernel Size:");
    medianKernelTextLabel->setStyleSheet("color: #c4b5fd;");
    medianKernelSlider = new QSlider(Qt::Horizontal);
    medianKernelSlider->setRange(3, 15);
    medianKernelSlider->setValue(5);
    medianKernelSlider->setSingleStep(2);
    medianKernelLabel = new QLabel("5");
    medianKernelLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 30px;");
    medianKernelLabel->setAlignment(Qt::AlignCenter);
    connect(medianKernelSlider, &QSlider::valueChanged, this, &NoiseRemovalDialog::onKernelSizeChanged);
    medianKernelLayout->addWidget(medianKernelTextLabel);
    medianKernelLayout->addWidget(medianKernelSlider);
    medianKernelLayout->addWidget(medianKernelLabel);
    medianLayout->addLayout(medianKernelLayout);
    
    QLabel *medianInfo = new QLabel("Best for salt & pepper noise");
    medianInfo->setStyleSheet("color: #9ca3af; font-size: 9pt;");
    medianLayout->addWidget(medianInfo);
    
    paramsLayout->addWidget(medianGroup);
    
    // Bilateral parameters
    bilateralGroup = new QGroupBox("Bilateral Parameters");
    bilateralGroup->setStyleSheet(filterSelectionGroup->styleSheet());
    bilateralGroup->setVisible(false);
    QVBoxLayout *bilateralLayout = new QVBoxLayout(bilateralGroup);
    
    QHBoxLayout *bilateralKernelLayout = new QHBoxLayout();
    QLabel *bilateralKernelTextLabel = new QLabel("Diameter:");
    bilateralKernelTextLabel->setStyleSheet("color: #c4b5fd;");
    bilateralKernelSlider = new QSlider(Qt::Horizontal);
    bilateralKernelSlider->setRange(5, 15);
    bilateralKernelSlider->setValue(9);
    bilateralKernelLabel = new QLabel("9");
    bilateralKernelLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 30px;");
    bilateralKernelLabel->setAlignment(Qt::AlignCenter);
    connect(bilateralKernelSlider, &QSlider::valueChanged, this, &NoiseRemovalDialog::onKernelSizeChanged);
    bilateralKernelLayout->addWidget(bilateralKernelTextLabel);
    bilateralKernelLayout->addWidget(bilateralKernelSlider);
    bilateralKernelLayout->addWidget(bilateralKernelLabel);
    bilateralLayout->addLayout(bilateralKernelLayout);
    
    QHBoxLayout *sigmaColorLayout = new QHBoxLayout();
    QLabel *sigmaColorTextLabel = new QLabel("Sigma Color:");
    sigmaColorTextLabel->setStyleSheet("color: #c4b5fd;");
    sigmaColorSlider = new QSlider(Qt::Horizontal);
    sigmaColorSlider->setRange(10, 150);
    sigmaColorSlider->setValue(75);
    sigmaColorLabel = new QLabel("75");
    sigmaColorLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 30px;");
    sigmaColorLabel->setAlignment(Qt::AlignCenter);
    connect(sigmaColorSlider, &QSlider::valueChanged, this, &NoiseRemovalDialog::onSigmaColorChanged);
    sigmaColorLayout->addWidget(sigmaColorTextLabel);
    sigmaColorLayout->addWidget(sigmaColorSlider);
    sigmaColorLayout->addWidget(sigmaColorLabel);
    bilateralLayout->addLayout(sigmaColorLayout);
    
    QHBoxLayout *sigmaSpaceLayout = new QHBoxLayout();
    QLabel *sigmaSpaceTextLabel = new QLabel("Sigma Space:");
    sigmaSpaceTextLabel->setStyleSheet("color: #c4b5fd;");
    sigmaSpaceSlider = new QSlider(Qt::Horizontal);
    sigmaSpaceSlider->setRange(10, 150);
    sigmaSpaceSlider->setValue(75);
    sigmaSpaceLabel = new QLabel("75");
    sigmaSpaceLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 30px;");
    sigmaSpaceLabel->setAlignment(Qt::AlignCenter);
    connect(sigmaSpaceSlider, &QSlider::valueChanged, this, &NoiseRemovalDialog::onSigmaSpaceChanged);
    sigmaSpaceLayout->addWidget(sigmaSpaceTextLabel);
    sigmaSpaceLayout->addWidget(sigmaSpaceSlider);
    sigmaSpaceLayout->addWidget(sigmaSpaceLabel);
    bilateralLayout->addLayout(sigmaSpaceLayout);
    
    QLabel *bilateralInfo = new QLabel("Preserves edges while removing noise");
    bilateralInfo->setStyleSheet("color: #9ca3af; font-size: 9pt;");
    bilateralLayout->addWidget(bilateralInfo);
    
    paramsLayout->addWidget(bilateralGroup);
    
    mainLayout->addLayout(paramsLayout);
    
    mainLayout->addSpacing(10);
    
    // Comparison view
    QGroupBox *comparisonGroup = new QGroupBox("Before / After Comparison");
    comparisonGroup->setStyleSheet(filterSelectionGroup->styleSheet());
    QHBoxLayout *comparisonLayout = new QHBoxLayout(comparisonGroup);
    
    // Original
    QVBoxLayout *originalLayout = new QVBoxLayout();
    QLabel *originalTitle = new QLabel("Noisy Image");
    originalTitle->setStyleSheet("color: #ef4444; font-weight: bold; font-size: 11pt;");
    originalTitle->setAlignment(Qt::AlignCenter);
    originalLayout->addWidget(originalTitle);
    
    originalCanvas = new ImageCanvas(this, "#ef4444");
    originalCanvas->setMinimumSize(400, 300);
    originalCanvas->setImage(originalImage);
    originalLayout->addWidget(originalCanvas);
    
    comparisonLayout->addLayout(originalLayout);
    
    // Denoised
    QVBoxLayout *denoisedLayout = new QVBoxLayout();
    QLabel *denoisedTitle = new QLabel("Denoised");
    denoisedTitle->setStyleSheet("color: #10b981; font-weight: bold; font-size: 11pt;");
    denoisedTitle->setAlignment(Qt::AlignCenter);
    denoisedLayout->addWidget(denoisedTitle);
    
    denoisedCanvas = new ImageCanvas(this, "#10b981");
    denoisedCanvas->setMinimumSize(400, 300);
    denoisedLayout->addWidget(denoisedCanvas);
    
    comparisonLayout->addLayout(denoisedLayout);
    
    mainLayout->addWidget(comparisonGroup);
    
    // Metrics display
    QGroupBox *metricsGroup = new QGroupBox("Noise Reduction Metrics");
    metricsGroup->setStyleSheet(filterSelectionGroup->styleSheet());
    QFormLayout *metricsLayout = new QFormLayout(metricsGroup);
    metricsLayout->setLabelAlignment(Qt::AlignRight);
    
    QString labelStyle = "color: #c4b5fd; font-size: 10pt;";
    QString valueStyle = "color: #e879f9; font-weight: bold; font-size: 10pt;";
    
    originalSNRLabel = new QLabel("0.00 dB");
    originalSNRLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("Original SNR:", nullptr), originalSNRLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    filteredSNRLabel = new QLabel("0.00 dB");
    filteredSNRLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("Filtered SNR:", nullptr), filteredSNRLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    snrImprovementLabel = new QLabel("+0.00 dB");
    snrImprovementLabel->setStyleSheet("color: #10b981; font-weight: bold; font-size: 10pt;");
    metricsLayout->addRow(new QLabel("SNR Improvement:", nullptr), snrImprovementLabel);
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
    connect(cancelButton, &QPushButton::clicked, this, &NoiseRemovalDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply Filter");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(150);
    connect(applyButton, &QPushButton::clicked, this, &NoiseRemovalDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void NoiseRemovalDialog::onFilterTypeChanged() {
    if (gaussianRadio->isChecked()) {
        filterType = "Gaussian";
        gaussianGroup->setVisible(true);
        medianGroup->setVisible(false);
        bilateralGroup->setVisible(false);
        kernelSize = gaussianKernelSlider->value();
    } else if (medianRadio->isChecked()) {
        filterType = "Median";
        gaussianGroup->setVisible(false);
        medianGroup->setVisible(true);
        bilateralGroup->setVisible(false);
        kernelSize = medianKernelSlider->value();
    } else {
        filterType = "Bilateral";
        gaussianGroup->setVisible(false);
        medianGroup->setVisible(false);
        bilateralGroup->setVisible(true);
        kernelSize = bilateralKernelSlider->value();
    }
    
    updateFilter();
}

void NoiseRemovalDialog::onKernelSizeChanged(int value) {
    // Ensure odd values for Gaussian and Median
    if (filterType == "Gaussian" || filterType == "Median") {
        if (value % 2 == 0) {
            value++;
            if (filterType == "Gaussian") {
                gaussianKernelSlider->setValue(value);
            } else {
                medianKernelSlider->setValue(value);
            }
            return;
        }
    }
    
    kernelSize = value;
    
    if (filterType == "Gaussian") {
        gaussianKernelLabel->setText(QString::number(value));
    } else if (filterType == "Median") {
        medianKernelLabel->setText(QString::number(value));
    } else {
        bilateralKernelLabel->setText(QString::number(value));
    }
    
    updateFilter();
}

void NoiseRemovalDialog::onSigmaColorChanged(int value) {
    sigmaColor = value;
    sigmaColorLabel->setText(QString::number(value));
    updateFilter();
}

void NoiseRemovalDialog::onSigmaSpaceChanged(int value) {
    sigmaSpace = value;
    sigmaSpaceLabel->setText(QString::number(value));
    updateFilter();
}

void NoiseRemovalDialog::updateFilter() {
    if (filterType == "Gaussian") {
        denoisedImage = applyGaussianFilter();
    } else if (filterType == "Median") {
        denoisedImage = applyMedianFilter();
    } else {
        denoisedImage = applyBilateralFilter();
    }
    
    updateComparison();
    updateMetrics();
    emit filterUpdated(denoisedImage);
}

cv::Mat NoiseRemovalDialog::applyGaussianFilter() {
    cv::Mat result;
    ImageProcessor::applyGaussianNoiseRemoval(originalImage, result, kernelSize);
    return result;
}

cv::Mat NoiseRemovalDialog::applyMedianFilter() {
    cv::Mat result;
    ImageProcessor::applyMedianFilter(originalImage, result, kernelSize);
    return result;
}

cv::Mat NoiseRemovalDialog::applyBilateralFilter() {
    cv::Mat result;
    ImageProcessor::applyBilateralFilter(originalImage, result, kernelSize, sigmaColor, sigmaSpace);
    return result;
}

void NoiseRemovalDialog::updateComparison() {
    if (!denoisedImage.empty()) {
        denoisedCanvas->setImage(denoisedImage);
    }
}

void NoiseRemovalDialog::updateMetrics() {
    if (denoisedImage.empty()) {
        return;
    }
    
    // Calculate filtered SNR
    filteredSNR = calculateSNR(denoisedImage);
    
    // Calculate SNR improvement
    snrImprovement = filteredSNR - originalSNR;
    
    // Calculate PSNR
    psnr = calculatePSNR(originalImage, denoisedImage);
    
    // Update labels
    originalSNRLabel->setText(QString("%1 dB").arg(originalSNR, 0, 'f', 2));
    filteredSNRLabel->setText(QString("%1 dB").arg(filteredSNR, 0, 'f', 2));
    
    if (snrImprovement >= 0) {
        snrImprovementLabel->setText(QString("+%1 dB").arg(snrImprovement, 0, 'f', 2));
        snrImprovementLabel->setStyleSheet("color: #10b981; font-weight: bold; font-size: 10pt;");
    } else {
        snrImprovementLabel->setText(QString("%1 dB").arg(snrImprovement, 0, 'f', 2));
        snrImprovementLabel->setStyleSheet("color: #ef4444; font-weight: bold; font-size: 10pt;");
    }
    
    if (std::isinf(psnr)) {
        psnrLabel->setText("? dB");
    } else {
        psnrLabel->setText(QString("%1 dB").arg(psnr, 0, 'f', 2));
    }
    
    // Quality assessment
    QString assessment;
    QString assessmentColor;
    if (snrImprovement >= 5.0) {
        assessment = "Excellent Noise Reduction";
        assessmentColor = "#10b981";
    } else if (snrImprovement >= 2.0) {
        assessment = "Good Noise Reduction";
        assessmentColor = "#3b82f6";
    } else if (snrImprovement >= 0.5) {
        assessment = "Fair Noise Reduction";
        assessmentColor = "#f59e0b";
    } else {
        assessment = "Minimal Improvement";
        assessmentColor = "#ef4444";
    }
    
    qualityAssessmentLabel->setText(assessment);
    qualityAssessmentLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 10pt;").arg(assessmentColor));
}

double NoiseRemovalDialog::calculateSNR(const cv::Mat& img) {
    if (img.empty()) {
        return 0.0;
    }
    
    cv::Mat img_converted;
    if (img.channels() == 3) {
        cv::cvtColor(img, img_converted, cv::COLOR_BGR2GRAY);
    } else {
        img_converted = img.clone();
    }
    
    img_converted.convertTo(img_converted, CV_64F);
    
    // Calculate mean
    cv::Scalar meanScalar = cv::mean(img_converted);
    double mean = meanScalar[0];
    
    // Calculate standard deviation
    cv::Mat meanMat = cv::Mat::ones(img_converted.size(), CV_64F) * mean;
    cv::Mat diff;
    cv::subtract(img_converted, meanMat, diff);
    diff = diff.mul(diff);
    
    cv::Scalar stdScalar = cv::mean(diff);
    double variance = stdScalar[0];
    double stdDev = std::sqrt(variance);
    
    // SNR = mean / stdDev (in dB)
    if (stdDev == 0.0) {
        return std::numeric_limits<double>::infinity();
    }
    
    double snr = 20.0 * std::log10(mean / stdDev);
    return snr;
}

double NoiseRemovalDialog::calculatePSNR(const cv::Mat& img1, const cv::Mat& img2) {
    if (img1.empty() || img2.empty() || img1.size() != img2.size()) {
        return 0.0;
    }
    
    cv::Mat img1_converted, img2_converted;
    img1.convertTo(img1_converted, CV_64F);
    img2.convertTo(img2_converted, CV_64F);
    
    cv::Mat diff;
    cv::subtract(img1_converted, img2_converted, diff);
    diff = diff.mul(diff);
    
    cv::Scalar s = cv::sum(diff);
    double sse = s[0] + s[1] + s[2];
    
    double mse = sse / (double)(img1_converted.total() * img1_converted.channels());
    
    if (mse == 0.0) {
        return std::numeric_limits<double>::infinity();
    }
    
    double maxPixelValue = 255.0;
    return 10.0 * std::log10((maxPixelValue * maxPixelValue) / mse);
}

void NoiseRemovalDialog::onApplyClicked() {
    applied = true;
    accept();
}

void NoiseRemovalDialog::onCancelClicked() {
    applied = false;
    reject();
}
