#include "CompressionDialog.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <cmath>
#include <vector>

CompressionDialog::CompressionDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), 
      originalImage(image.clone()),
      compressionType("JPEG"),
      quality(95),
      pngLevel(6),
      compressionRatio(1.0),
      rmse(0.0),
      psnr(0.0),
      applied(false),
      originalSize(0),
      compressedSize(0) {
    
    setWindowTitle("Image Compression");
    setMinimumSize(600, 500);
    setStyleSheet(Theme::MAIN_STYLESHEET);
    
    setupUI();
    updateCompression();
}

CompressionDialog::~CompressionDialog() {
}

void CompressionDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Compress Image");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Compression type selection
    QHBoxLayout *typeLayout = new QHBoxLayout();
    QLabel *typeLabel = new QLabel("Compression Type:");
    typeLabel->setStyleSheet("color: #c4b5fd; font-size: 11pt;");
    typeComboBox = new QComboBox();
    typeComboBox->addItem("JPEG Compression");
    typeComboBox->addItem("PNG Compression");
    typeComboBox->setMinimumWidth(200);
    connect(typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CompressionDialog::onCompressionTypeChanged);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeComboBox);
    typeLayout->addStretch();
    mainLayout->addLayout(typeLayout);
    
    mainLayout->addSpacing(15);
    
    // JPEG Settings Group
    jpegGroup = new QGroupBox("JPEG Settings");
    jpegGroup->setStyleSheet(
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
    QVBoxLayout *jpegLayout = new QVBoxLayout(jpegGroup);
    
    QHBoxLayout *jpegQualityLayout = new QHBoxLayout();
    QLabel *jpegLabel = new QLabel("Quality:");
    jpegLabel->setStyleSheet("color: #c4b5fd;");
    jpegQualitySlider = new QSlider(Qt::Horizontal);
    jpegQualitySlider->setRange(1, 100);
    jpegQualitySlider->setValue(95);
    jpegQualityLabel = new QLabel("95");
    jpegQualityLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 40px;");
    jpegQualityLabel->setAlignment(Qt::AlignCenter);
    connect(jpegQualitySlider, &QSlider::valueChanged, 
            this, &CompressionDialog::onJpegQualityChanged);
    jpegQualityLayout->addWidget(jpegLabel);
    jpegQualityLayout->addWidget(jpegQualitySlider);
    jpegQualityLayout->addWidget(jpegQualityLabel);
    jpegLayout->addLayout(jpegQualityLayout);
    
    QLabel *jpegInfoLabel = new QLabel("Higher quality = Larger file size, Better image quality");
    jpegInfoLabel->setStyleSheet("color: #9ca3af; font-size: 9pt; font-style: italic;");
    jpegLayout->addWidget(jpegInfoLabel);
    
    mainLayout->addWidget(jpegGroup);
    
    // PNG Settings Group
    pngGroup = new QGroupBox("PNG Settings");
    pngGroup->setStyleSheet(jpegGroup->styleSheet());
    pngGroup->setVisible(false);
    QVBoxLayout *pngLayout = new QVBoxLayout(pngGroup);
    
    QHBoxLayout *pngLevelLayout = new QHBoxLayout();
    QLabel *pngLabel = new QLabel("Compression Level:");
    pngLabel->setStyleSheet("color: #c4b5fd;");
    pngLevelSlider = new QSlider(Qt::Horizontal);
    pngLevelSlider->setRange(0, 9);
    pngLevelSlider->setValue(6);
    pngLevelLabel = new QLabel("6");
    pngLevelLabel->setStyleSheet("color: #e879f9; font-weight: bold; min-width: 40px;");
    pngLevelLabel->setAlignment(Qt::AlignCenter);
    connect(pngLevelSlider, &QSlider::valueChanged,
            this, &CompressionDialog::onPngLevelChanged);
    pngLevelLayout->addWidget(pngLabel);
    pngLevelLayout->addWidget(pngLevelSlider);
    pngLevelLayout->addWidget(pngLevelLabel);
    pngLayout->addLayout(pngLevelLayout);
    
    QLabel *pngInfoLabel = new QLabel("Higher level = Smaller file size, Slower compression (Lossless)");
    pngInfoLabel->setStyleSheet("color: #9ca3af; font-size: 9pt; font-style: italic;");
    pngLayout->addWidget(pngInfoLabel);
    
    mainLayout->addWidget(pngGroup);
    
    mainLayout->addSpacing(15);
    
    // Metrics Group
    QGroupBox *metricsGroup = new QGroupBox("Compression Metrics");
    metricsGroup->setStyleSheet(jpegGroup->styleSheet());
    QFormLayout *metricsLayout = new QFormLayout(metricsGroup);
    metricsLayout->setLabelAlignment(Qt::AlignRight);
    
    QString labelStyle = "color: #c4b5fd; font-size: 10pt;";
    QString valueStyle = "color: #e879f9; font-weight: bold; font-size: 10pt;";
    
    originalSizeLabel = new QLabel("0 KB");
    originalSizeLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("Original Size:", nullptr), originalSizeLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    compressedSizeLabel = new QLabel("0 KB");
    compressedSizeLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("Compressed Size:", nullptr), compressedSizeLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    compressionRatioLabel = new QLabel("1.00x");
    compressionRatioLabel->setStyleSheet(valueStyle);
    metricsLayout->addRow(new QLabel("Compression Ratio:", nullptr), compressionRatioLabel);
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
    metricsLayout->addRow(new QLabel("Quality Assessment:", nullptr), qualityAssessmentLabel);
    metricsLayout->itemAt(metricsLayout->rowCount() - 1, QFormLayout::LabelRole)->widget()->setStyleSheet(labelStyle);
    
    mainLayout->addWidget(metricsGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &CompressionDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply Compression");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(150);
    connect(applyButton, &QPushButton::clicked, this, &CompressionDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void CompressionDialog::onCompressionTypeChanged(int index) {
    compressionType = (index == 0) ? "JPEG" : "PNG";
    jpegGroup->setVisible(index == 0);
    pngGroup->setVisible(index == 1);
    updateCompression();
}

void CompressionDialog::onJpegQualityChanged(int value) {
    quality = value;
    jpegQualityLabel->setText(QString::number(value));
    updateCompression();
}

void CompressionDialog::onPngLevelChanged(int value) {
    pngLevel = value;
    pngLevelLabel->setText(QString::number(value));
    updateCompression();
}

void CompressionDialog::updateCompression() {
    if (compressionType == "JPEG") {
        compressedImage = compressJPEG(quality);
    } else {
        compressedImage = compressPNG(pngLevel);
    }
    
    updateMetrics();
    emit compressionUpdated(compressedImage);
}

cv::Mat CompressionDialog::compressJPEG(int quality) {
    std::vector<uchar> buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, quality};
    
    cv::imencode(".jpg", originalImage, buffer, params);
    compressedSize = buffer.size();
    
    cv::Mat compressed = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);
    return compressed;
}

cv::Mat CompressionDialog::compressPNG(int level) {
    std::vector<uchar> buffer;
    std::vector<int> params = {cv::IMWRITE_PNG_COMPRESSION, level};
    
    cv::imencode(".png", originalImage, buffer, params);
    compressedSize = buffer.size();
    
    cv::Mat compressed = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);
    return compressed;
}

void CompressionDialog::updateMetrics() {
    // Calculate original size (estimate)
    originalSize = originalImage.total() * originalImage.elemSize();
    
    // Calculate compression ratio
    compressionRatio = calculateCompressionRatio(originalImage, compressedImage);
    
    // Calculate RMSE and PSNR
    rmse = calculateRMSE(originalImage, compressedImage);
    psnr = calculatePSNR(rmse);
    
    // Update labels
    originalSizeLabel->setText(QString("%1 KB").arg(originalSize / 1024.0, 0, 'f', 2));
    compressedSizeLabel->setText(QString("%1 KB").arg(compressedSize / 1024.0, 0, 'f', 2));
    compressionRatioLabel->setText(QString("%1x").arg(compressionRatio, 0, 'f', 2));
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
        assessment = "Excellent Quality";
        assessmentColor = "#10b981"; // Green
    } else if (psnr >= 30) {
        assessment = "Good Quality";
        assessmentColor = "#3b82f6"; // Blue
    } else if (psnr >= 20) {
        assessment = "Fair Quality";
        assessmentColor = "#f59e0b"; // Orange
    } else {
        assessment = "Poor Quality";
        assessmentColor = "#ef4444"; // Red
    }
    
    qualityAssessmentLabel->setText(assessment);
    qualityAssessmentLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 10pt;").arg(assessmentColor));
}

double CompressionDialog::calculateRMSE(const cv::Mat& img1, const cv::Mat& img2) {
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

double CompressionDialog::calculatePSNR(double rmse) {
    if (rmse == 0.0) {
        return std::numeric_limits<double>::infinity();
    }
    
    double maxPixelValue = 255.0;
    double mse = rmse * rmse;
    return 10.0 * std::log10((maxPixelValue * maxPixelValue) / mse);
}

double CompressionDialog::calculateCompressionRatio(const cv::Mat& original, const cv::Mat& compressed) {
    if (compressedSize == 0) {
        return 1.0;
    }
    
    return (double)originalSize / (double)compressedSize;
}

void CompressionDialog::onApplyClicked() {
    applied = true;
    accept();
}

void CompressionDialog::onCancelClicked() {
    applied = false;
    reject();
}
