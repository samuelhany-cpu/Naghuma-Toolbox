#include "FrequencyFilterDialog.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStackedWidget>
#include <cmath>

FrequencyFilterDialog::FrequencyFilterDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), inputImage(image.clone()), applied(false) {
    
    setWindowTitle("Advanced Frequency Filters - Phase 19");
    setMinimumSize(1100, 750);
    
    setupUI();
    onFilterTypeChanged(0);  // Initialize with first filter
}

FrequencyFilterDialog::~FrequencyFilterDialog() {
}

void FrequencyFilterDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Advanced Frequency Domain Filtering");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Filter type selection
    QHBoxLayout* filterLayout = new QHBoxLayout();
    QLabel* filterLabel = new QLabel("Filter Type:");
    filterLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    filterTypeCombo = new QComboBox();
    filterTypeCombo->addItem("Butterworth Lowpass");
    filterTypeCombo->addItem("Butterworth Highpass");
    filterTypeCombo->addItem("Gaussian Lowpass");
    filterTypeCombo->addItem("Gaussian Highpass");
    filterTypeCombo->addItem("Bandpass Filter");
    filterTypeCombo->addItem("Bandreject Filter");
    filterTypeCombo->addItem("Homomorphic Filter");
    connect(filterTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &FrequencyFilterDialog::onFilterTypeChanged);
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterTypeCombo, 1);
    mainLayout->addLayout(filterLayout);
    
    // Parameters area with stacked widget
    QGroupBox* paramsGroup = new QGroupBox("Filter Parameters");
    paramsGroup->setStyleSheet(
        "QGroupBox { color: #c4b5fd; font-weight: bold; border: 2px solid rgba(91, 75, 115, 0.5); "
        "border-radius: 8px; margin-top: 10px; padding: 15px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* paramsLayout = new QVBoxLayout(paramsGroup);
    
    QStackedWidget* stackedParams = new QStackedWidget();
    
    // === STANDARD FILTER PARAMETERS (Butterworth/Gaussian) ===
    standardParams = new QWidget();
    QVBoxLayout* standardLayout = new QVBoxLayout(standardParams);
    
    QHBoxLayout* cutoffLayout = new QHBoxLayout();
    QLabel* cutoffLabelText = new QLabel("Cutoff Frequency:");
    cutoffLabelText->setStyleSheet("color: #c4b5fd;");
    cutoffFreqSlider = new QSlider(Qt::Horizontal);
    cutoffFreqSlider->setRange(1, 200);
    cutoffFreqSlider->setValue(30);
    cutoffFreqLabel = new QLabel("30");
    cutoffFreqLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(cutoffFreqSlider, &QSlider::valueChanged, this, [this](int value) {
        cutoffFreqLabel->setText(QString::number(value));
        onParameterChanged();
    });
    cutoffLayout->addWidget(cutoffLabelText);
    cutoffLayout->addWidget(cutoffFreqSlider);
    cutoffLayout->addWidget(cutoffFreqLabel);
    standardLayout->addLayout(cutoffLayout);
    
    QHBoxLayout* orderLayout = new QHBoxLayout();
    QLabel* orderLabel = new QLabel("Filter Order (Butterworth):");
    orderLabel->setStyleSheet("color: #c4b5fd;");
    filterOrderSpin = new QSpinBox();
    filterOrderSpin->setRange(1, 10);
    filterOrderSpin->setValue(2);
    connect(filterOrderSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FrequencyFilterDialog::onParameterChanged);
    orderLayout->addWidget(orderLabel);
    orderLayout->addWidget(filterOrderSpin);
    orderLayout->addStretch();
    standardLayout->addLayout(orderLayout);
    
    QLabel* standardInfo = new QLabel(
        "Cutoff Frequency: Controls the transition point between pass and stop bands.\n"
        "Order: Higher values create sharper transitions (Butterworth only).\n"
        "Lowpass: Removes high frequencies (blurring, smoothing).\n"
        "Highpass: Removes low frequencies (edge enhancement)."
    );
    standardInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    standardLayout->addWidget(standardInfo);
    standardLayout->addStretch();
    
    stackedParams->addWidget(standardParams);
    
    // === BANDPASS/BANDREJECT PARAMETERS ===
    bandParams = new QWidget();
    QVBoxLayout* bandLayout = new QVBoxLayout(bandParams);
    
    QHBoxLayout* centerLayout = new QHBoxLayout();
    QLabel* centerLabelText = new QLabel("Center Frequency:");
    centerLabelText->setStyleSheet("color: #c4b5fd;");
    centerFreqSlider = new QSlider(Qt::Horizontal);
    centerFreqSlider->setRange(10, 200);
    centerFreqSlider->setValue(50);
    centerFreqLabel = new QLabel("50");
    centerFreqLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(centerFreqSlider, &QSlider::valueChanged, this, [this](int value) {
        centerFreqLabel->setText(QString::number(value));
        onParameterChanged();
    });
    centerLayout->addWidget(centerLabelText);
    centerLayout->addWidget(centerFreqSlider);
    centerLayout->addWidget(centerFreqLabel);
    bandLayout->addLayout(centerLayout);
    
    QHBoxLayout* bwLayout = new QHBoxLayout();
    QLabel* bwLabelText = new QLabel("Bandwidth:");
    bwLabelText->setStyleSheet("color: #c4b5fd;");
    bandwidthSlider = new QSlider(Qt::Horizontal);
    bandwidthSlider->setRange(5, 100);
    bandwidthSlider->setValue(20);
    bandwidthLabel = new QLabel("20");
    bandwidthLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(bandwidthSlider, &QSlider::valueChanged, this, [this](int value) {
        bandwidthLabel->setText(QString::number(value));
        onParameterChanged();
    });
    bwLayout->addWidget(bwLabelText);
    bwLayout->addWidget(bandwidthSlider);
    bwLayout->addWidget(bandwidthLabel);
    bandLayout->addLayout(bwLayout);
    
    QLabel* bandInfo = new QLabel(
        "Center Frequency: Middle of the pass/reject band.\n"
        "Bandwidth: Width of the frequency band to pass/reject.\n"
        "Bandpass: Passes only frequencies within the band.\n"
        "Bandreject: Removes frequencies within the band (notch filter)."
    );
    bandInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    bandLayout->addWidget(bandInfo);
    bandLayout->addStretch();
    
    stackedParams->addWidget(bandParams);
    
    // === HOMOMORPHIC FILTER PARAMETERS ===
    homoParams = new QWidget();
    QVBoxLayout* homoLayout = new QVBoxLayout(homoParams);
    
    QHBoxLayout* gammaLowLayout = new QHBoxLayout();
    QLabel* gammaLowLabelText = new QLabel("Gamma Low (?L):");
    gammaLowLabelText->setStyleSheet("color: #c4b5fd;");
    gammaLowSlider = new QSlider(Qt::Horizontal);
    gammaLowSlider->setRange(1, 100);
    gammaLowSlider->setValue(25);
    gammaLowLabel = new QLabel("0.25");
    gammaLowLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(gammaLowSlider, &QSlider::valueChanged, this, [this](int value) {
        gammaLowLabel->setText(QString::number(value / 100.0, 'f', 2));
        onParameterChanged();
    });
    gammaLowLayout->addWidget(gammaLowLabelText);
    gammaLowLayout->addWidget(gammaLowSlider);
    gammaLowLayout->addWidget(gammaLowLabel);
    homoLayout->addLayout(gammaLowLayout);
    
    QHBoxLayout* gammaHighLayout = new QHBoxLayout();
    QLabel* gammaHighLabelText = new QLabel("Gamma High (?H):");
    gammaHighLabelText->setStyleSheet("color: #c4b5fd;");
    gammaHighSlider = new QSlider(Qt::Horizontal);
    gammaHighSlider->setRange(100, 300);
    gammaHighSlider->setValue(150);
    gammaHighLabel = new QLabel("1.50");
    gammaHighLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(gammaHighSlider, &QSlider::valueChanged, this, [this](int value) {
        gammaHighLabel->setText(QString::number(value / 100.0, 'f', 2));
        onParameterChanged();
    });
    gammaHighLayout->addWidget(gammaHighLabelText);
    gammaHighLayout->addWidget(gammaHighSlider);
    gammaHighLayout->addWidget(gammaHighLabel);
    homoLayout->addLayout(gammaHighLayout);
    
    QHBoxLayout* homoCutoffLayout = new QHBoxLayout();
    QLabel* homoCutoffLabelText = new QLabel("Cutoff Frequency:");
    homoCutoffLabelText->setStyleSheet("color: #c4b5fd;");
    homoCutoffSlider = new QSlider(Qt::Horizontal);
    homoCutoffSlider->setRange(1, 100);
    homoCutoffSlider->setValue(30);
    homoCutoffLabel = new QLabel("30");
    homoCutoffLabel->setStyleSheet("color: #a78bfa; min-width: 60px;");
    connect(homoCutoffSlider, &QSlider::valueChanged, this, [this](int value) {
        homoCutoffLabel->setText(QString::number(value));
        onParameterChanged();
    });
    homoCutoffLayout->addWidget(homoCutoffLabelText);
    homoCutoffLayout->addWidget(homoCutoffSlider);
    homoCutoffLayout->addWidget(homoCutoffLabel);
    homoLayout->addLayout(homoCutoffLayout);
    
    QLabel* homoInfo = new QLabel(
        "Homomorphic filtering for illumination and reflectance separation.\n"
        "?L < 1: Suppresses low frequencies (illumination).\n"
        "?H > 1: Enhances high frequencies (reflectance/details).\n"
        "Useful for images with uneven lighting."
    );
    homoInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    homoLayout->addWidget(homoInfo);
    homoLayout->addStretch();
    
    stackedParams->addWidget(homoParams);
    
    paramsLayout->addWidget(stackedParams);
    mainLayout->addWidget(paramsGroup);
    
    // Connect stacked widget switching
    connect(filterTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, stackedParams](int index) {
        if (index <= 3) {
            stackedParams->setCurrentWidget(standardParams);
        } else if (index <= 5) {
            stackedParams->setCurrentWidget(bandParams);
        } else {
            stackedParams->setCurrentWidget(homoParams);
        }
    });
    
    // Preview canvas
    QLabel* previewLabel = new QLabel("Filtered Result:");
    previewLabel->setStyleSheet("color: #c4b5fd; font-weight: bold; padding-top: 10px;");
    mainLayout->addWidget(previewLabel);
    
    previewCanvas = new ImageCanvas(this, "#e879f9");
    previewCanvas->setMinimumSize(600, 400);
    mainLayout->addWidget(previewCanvas, 1);
    
    // Info label
    infoLabel = new QLabel("Adjust parameters to see filtered result");
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    resetButton = new QPushButton("Reset");
    resetButton->setMinimumWidth(100);
    connect(resetButton, &QPushButton::clicked, this, &FrequencyFilterDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);
    
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &FrequencyFilterDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void FrequencyFilterDialog::onFilterTypeChanged(int index) {
    updatePreview();
}

void FrequencyFilterDialog::onParameterChanged() {
    updatePreview();
}

void FrequencyFilterDialog::updatePreview() {
    int filterIndex = filterTypeCombo->currentIndex();
    
    try {
        switch (filterIndex) {
            case 0: applyButterworthLowpass(); break;
            case 1: applyButterworthHighpass(); break;
            case 2: applyGaussianLowpass(); break;
            case 3: applyGaussianHighpass(); break;
            case 4: applyBandpassFilter(); break;
            case 5: applyBandrejectFilter(); break;
            case 6: applyHomomorphicFilter(); break;
            default: break;
        }
        
        if (!previewImage.empty()) {
            previewCanvas->setImage(previewImage);
            emit previewUpdated(previewImage);
        }
    } catch (const cv::Exception& e) {
        infoLabel->setText(QString("Error: %1").arg(e.what()));
        infoLabel->setStyleSheet("color: #ff6b6b; padding: 5px;");
    }
}

void FrequencyFilterDialog::shiftDFT(cv::Mat& fImage) {
    int cx = fImage.cols / 2;
    int cy = fImage.rows / 2;

    cv::Mat tmp;
    cv::Mat q0(fImage, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(fImage, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(fImage, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(fImage, cv::Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void FrequencyFilterDialog::createFrequencyFilter(cv::Mat& filter, int type, double cutoff, int order) {
    int rows = filter.rows;
    int cols = filter.cols;
    int cx = cols / 2;
    int cy = rows / 2;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double distance = std::sqrt((i - cy) * (i - cy) + (j - cx) * (j - cx));
            
            if (type == 0) { // Butterworth Lowpass
                double value = 1.0 / (1.0 + std::pow(distance / cutoff, 2.0 * order));
                filter.at<float>(i, j) = static_cast<float>(value);
            } else if (type == 1) { // Butterworth Highpass
                if (distance == 0) distance = 0.01;
                double value = 1.0 / (1.0 + std::pow(cutoff / distance, 2.0 * order));
                filter.at<float>(i, j) = static_cast<float>(value);
            } else if (type == 2) { // Gaussian Lowpass
                double value = std::exp(-(distance * distance) / (2.0 * cutoff * cutoff));
                filter.at<float>(i, j) = static_cast<float>(value);
            } else if (type == 3) { // Gaussian Highpass
                double value = 1.0 - std::exp(-(distance * distance) / (2.0 * cutoff * cutoff));
                filter.at<float>(i, j) = static_cast<float>(value);
            }
        }
    }
}

void FrequencyFilterDialog::createBandFilter(cv::Mat& filter, int type, double centerFreq, double bandwidth) {
    int rows = filter.rows;
    int cols = filter.cols;
    int cx = cols / 2;
    int cy = rows / 2;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double distance = std::sqrt((i - cy) * (i - cy) + (j - cx) * (j - cx));
            double diff = std::abs(distance - centerFreq);
            
            if (type == 0) { // Bandpass
                double value = std::exp(-(diff * diff) / (2.0 * bandwidth * bandwidth));
                filter.at<float>(i, j) = static_cast<float>(value);
            } else { // Bandreject
                double value = 1.0 - std::exp(-(diff * diff) / (2.0 * bandwidth * bandwidth));
                filter.at<float>(i, j) = static_cast<float>(value);
            }
        }
    }
}

void FrequencyFilterDialog::applyFrequencyFilter(const cv::Mat& input, cv::Mat& output, const cv::Mat& filter) {
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Convert to float
    cv::Mat floatImg;
    gray.convertTo(floatImg, CV_32F);
    
    // DFT
    cv::Mat dftInput;
    cv::dft(floatImg, dftInput, cv::DFT_COMPLEX_OUTPUT);
    shiftDFT(dftInput);
    
    // Split into real and imaginary
    std::vector<cv::Mat> planes;
    cv::split(dftInput, planes);
    
    // Apply filter to both planes
    planes[0] = planes[0].mul(filter);
    planes[1] = planes[1].mul(filter);
    
    // Merge
    cv::Mat filteredDFT;
    cv::merge(planes, filteredDFT);
    
    // Inverse shift and DFT
    shiftDFT(filteredDFT);
    cv::Mat inverse;
    cv::idft(filteredDFT, inverse, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
    
    // Normalize
    cv::normalize(inverse, inverse, 0, 255, cv::NORM_MINMAX);
    inverse.convertTo(output, CV_8U);
    
    // Convert back to color if needed
    if (input.channels() == 3) {
        cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    }
}

void FrequencyFilterDialog::applyButterworthLowpass() {
    double cutoff = cutoffFreqSlider->value();
    int order = filterOrderSpin->value();
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    
    createFrequencyFilter(filter, 0, cutoff, order);
    applyFrequencyFilter(inputImage, previewImage, filter);
    
    filterType = QString("Butterworth Lowpass (D0=%1, n=%2)").arg(cutoff).arg(order);
    infoLabel->setText(QString("Butterworth lowpass applied (cutoff=%1, order=%2)").arg(cutoff).arg(order));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::applyButterworthHighpass() {
    double cutoff = cutoffFreqSlider->value();
    int order = filterOrderSpin->value();
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    
    createFrequencyFilter(filter, 1, cutoff, order);
    applyFrequencyFilter(inputImage, previewImage, filter);
    
    filterType = QString("Butterworth Highpass (D0=%1, n=%2)").arg(cutoff).arg(order);
    infoLabel->setText(QString("Butterworth highpass applied (cutoff=%1, order=%2)").arg(cutoff).arg(order));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::applyGaussianLowpass() {
    double cutoff = cutoffFreqSlider->value();
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    
    createFrequencyFilter(filter, 2, cutoff, 0);
    applyFrequencyFilter(inputImage, previewImage, filter);
    
    filterType = QString("Gaussian Lowpass (?=%1)").arg(cutoff);
    infoLabel->setText(QString("Gaussian lowpass applied (sigma=%1)").arg(cutoff));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::applyGaussianHighpass() {
    double cutoff = cutoffFreqSlider->value();
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    
    createFrequencyFilter(filter, 3, cutoff, 0);
    applyFrequencyFilter(inputImage, previewImage, filter);
    
    filterType = QString("Gaussian Highpass (?=%1)").arg(cutoff);
    infoLabel->setText(QString("Gaussian highpass applied (sigma=%1)").arg(cutoff));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::applyBandpassFilter() {
    double centerFreq = centerFreqSlider->value();
    double bandwidth = bandwidthSlider->value();
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    
    createBandFilter(filter, 0, centerFreq, bandwidth);
    applyFrequencyFilter(inputImage, previewImage, filter);
    
    filterType = QString("Bandpass (Center=%1, BW=%2)").arg(centerFreq).arg(bandwidth);
    infoLabel->setText(QString("Bandpass filter applied (center=%1, bandwidth=%2)").arg(centerFreq).arg(bandwidth));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::applyBandrejectFilter() {
    double centerFreq = centerFreqSlider->value();
    double bandwidth = bandwidthSlider->value();
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    
    createBandFilter(filter, 1, centerFreq, bandwidth);
    applyFrequencyFilter(inputImage, previewImage, filter);
    
    filterType = QString("Bandreject (Center=%1, BW=%2)").arg(centerFreq).arg(bandwidth);
    infoLabel->setText(QString("Bandreject filter applied (center=%1, bandwidth=%2)").arg(centerFreq).arg(bandwidth));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::applyHomomorphicFilter() {
    double gammaLow = gammaLowSlider->value() / 100.0;
    double gammaHigh = gammaHighSlider->value() / 100.0;
    double cutoff = homoCutoffSlider->value();
    
    cv::Mat gray;
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = inputImage.clone();
    }
    
    // Add small constant to avoid log(0)
    cv::Mat floatImg;
    gray.convertTo(floatImg, CV_32F);
    floatImg += 1.0;
    
    // Take logarithm
    cv::log(floatImg, floatImg);
    
    // DFT
    cv::Mat dftInput;
    cv::dft(floatImg, dftInput, cv::DFT_COMPLEX_OUTPUT);
    shiftDFT(dftInput);
    
    // Create homomorphic filter
    int rows = gray.rows;
    int cols = gray.cols;
    int cx = cols / 2;
    int cy = rows / 2;
    
    cv::Mat filter = cv::Mat::zeros(rows, cols, CV_32F);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double distance = std::sqrt((i - cy) * (i - cy) + (j - cx) * (j - cx));
            double h = (gammaHigh - gammaLow) * (1.0 - std::exp(-(distance * distance) / (2.0 * cutoff * cutoff))) + gammaLow;
            filter.at<float>(i, j) = static_cast<float>(h);
        }
    }
    
    // Apply filter
    std::vector<cv::Mat> planes;
    cv::split(dftInput, planes);
    planes[0] = planes[0].mul(filter);
    planes[1] = planes[1].mul(filter);
    
    cv::Mat filteredDFT;
    cv::merge(planes, filteredDFT);
    
    // Inverse DFT
    shiftDFT(filteredDFT);
    cv::Mat inverse;
    cv::idft(filteredDFT, inverse, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
    
    // Exponentiate
    cv::exp(inverse, inverse);
    
    // Normalize
    cv::normalize(inverse, inverse, 0, 255, cv::NORM_MINMAX);
    inverse.convertTo(previewImage, CV_8U);
    
    // Convert back to color if needed
    if (inputImage.channels() == 3) {
        cv::cvtColor(previewImage, previewImage, cv::COLOR_GRAY2BGR);
    }
    
    filterType = QString("Homomorphic (?L=%.2f, ?H=%.2f)").arg(gammaLow).arg(gammaHigh);
    infoLabel->setText(QString("Homomorphic filter applied (?L=%.2f, ?H=%.2f, cutoff=%1)")
        .arg(cutoff).arg(gammaLow).arg(gammaHigh));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void FrequencyFilterDialog::onApplyClicked() {
    if (!previewImage.empty()) {
        filteredImage = previewImage.clone();
        applied = true;
        accept();
    } else {
        QMessageBox::warning(this, "No Preview", "No filtered image to apply!");
    }
}

void FrequencyFilterDialog::onResetClicked() {
    // Reset all parameters to default
    cutoffFreqSlider->setValue(30);
    filterOrderSpin->setValue(2);
    
    centerFreqSlider->setValue(50);
    bandwidthSlider->setValue(20);
    
    gammaLowSlider->setValue(25);
    gammaHighSlider->setValue(150);
    homoCutoffSlider->setValue(30);
    
    updatePreview();
}
#include "moc_FrequencyFilterDialog.cpp"
