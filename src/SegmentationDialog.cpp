#include "SegmentationDialog.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStackedWidget>

SegmentationDialog::SegmentationDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), inputImage(image.clone()), applied(false) {
    
    setWindowTitle("Advanced Segmentation - Phase 17");
    setMinimumSize(1000, 700);
    
    setupUI();
    onMethodChanged(0);  // Initialize with first method
}

SegmentationDialog::~SegmentationDialog() {
}

void SegmentationDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Region-Based Segmentation");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Method selection
    QHBoxLayout* methodLayout = new QHBoxLayout();
    QLabel* methodLabel = new QLabel("Segmentation Method:");
    methodLabel->setStyleSheet("color: #c4b5fd; font-weight: bold;");
    methodCombo = new QComboBox();
    methodCombo->addItem("Watershed Segmentation");
    methodCombo->addItem("K-Means Clustering");
    methodCombo->addItem("Mean Shift Segmentation");
    methodCombo->addItem("GrabCut (Foreground)");
    methodCombo->addItem("SLIC Superpixels");
    connect(methodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &SegmentationDialog::onMethodChanged);
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
    
    // === WATERSHED PARAMETERS ===
    watershedParams = new QWidget();
    QVBoxLayout* watershedLayout = new QVBoxLayout(watershedParams);
    
    QHBoxLayout* watershedThreshLayout = new QHBoxLayout();
    QLabel* watershedLabel = new QLabel("Distance Threshold:");
    watershedLabel->setStyleSheet("color: #c4b5fd;");
    watershedThresholdSpin = new QSpinBox();
    watershedThresholdSpin->setRange(1, 100);
    watershedThresholdSpin->setValue(20);
    watershedThresholdSpin->setSuffix(" %");
    connect(watershedThresholdSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    watershedThreshLayout->addWidget(watershedLabel);
    watershedThreshLayout->addWidget(watershedThresholdSpin);
    watershedThreshLayout->addStretch();
    watershedLayout->addLayout(watershedThreshLayout);
    
    QLabel* watershedInfo = new QLabel(
        "Watershed treats image as topographic surface.\n"
        "Lower threshold = more regions (over-segmentation)\n"
        "Higher threshold = fewer regions (under-segmentation)"
    );
    watershedInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    watershedLayout->addWidget(watershedInfo);
    watershedLayout->addStretch();
    
    stackedParams->addWidget(watershedParams);
    
    // === K-MEANS PARAMETERS ===
    kmeansParams = new QWidget();
    QVBoxLayout* kmeansLayout = new QVBoxLayout(kmeansParams);
    
    QHBoxLayout* clustersLayout = new QHBoxLayout();
    QLabel* clustersLabel = new QLabel("Number of Clusters (K):");
    clustersLabel->setStyleSheet("color: #c4b5fd;");
    kmeansClustersSpin = new QSpinBox();
    kmeansClustersSpin->setRange(2, 20);
    kmeansClustersSpin->setValue(3);
    connect(kmeansClustersSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    clustersLayout->addWidget(clustersLabel);
    clustersLayout->addWidget(kmeansClustersSpin);
    clustersLayout->addStretch();
    kmeansLayout->addLayout(clustersLayout);
    
    QHBoxLayout* iterationsLayout = new QHBoxLayout();
    QLabel* iterationsLabel = new QLabel("Max Iterations:");
    iterationsLabel->setStyleSheet("color: #c4b5fd;");
    kmeansIterationsSpin = new QSpinBox();
    kmeansIterationsSpin->setRange(10, 500);
    kmeansIterationsSpin->setValue(100);
    kmeansIterationsSpin->setSingleStep(10);
    connect(kmeansIterationsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    iterationsLayout->addWidget(iterationsLabel);
    iterationsLayout->addWidget(kmeansIterationsSpin);
    iterationsLayout->addStretch();
    kmeansLayout->addLayout(iterationsLayout);
    
    QLabel* kmeansInfo = new QLabel(
        "K-Means clusters pixels based on color similarity.\n"
        "K = number of distinct regions/colors in output.\n"
        "Common values: 3-5 for basic segmentation."
    );
    kmeansInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    kmeansLayout->addWidget(kmeansInfo);
    kmeansLayout->addStretch();
    
    stackedParams->addWidget(kmeansParams);
    
    // === MEAN SHIFT PARAMETERS ===
    meanShiftParams = new QWidget();
    QVBoxLayout* meanShiftLayout = new QVBoxLayout(meanShiftParams);
    
    QHBoxLayout* spatialLayout = new QHBoxLayout();
    QLabel* spatialLabel = new QLabel("Spatial Radius:");
    spatialLabel->setStyleSheet("color: #c4b5fd;");
    spatialRadiusSpin = new QSpinBox();
    spatialRadiusSpin->setRange(5, 50);
    spatialRadiusSpin->setValue(20);
    connect(spatialRadiusSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    spatialLayout->addWidget(spatialLabel);
    spatialLayout->addWidget(spatialRadiusSpin);
    spatialLayout->addStretch();
    meanShiftLayout->addLayout(spatialLayout);
    
    QHBoxLayout* colorLayout = new QHBoxLayout();
    QLabel* colorLabel = new QLabel("Color Radius:");
    colorLabel->setStyleSheet("color: #c4b5fd;");
    colorRadiusSpin = new QSpinBox();
    colorRadiusSpin->setRange(5, 100);
    colorRadiusSpin->setValue(40);
    connect(colorRadiusSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorRadiusSpin);
    colorLayout->addStretch();
    meanShiftLayout->addLayout(colorLayout);
    
    QLabel* meanShiftInfo = new QLabel(
        "Mean Shift finds modes (peaks) in color/spatial distribution.\n"
        "Spatial Radius: nearby pixels in space.\n"
        "Color Radius: nearby pixels in color."
    );
    meanShiftInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    meanShiftLayout->addWidget(meanShiftInfo);
    meanShiftLayout->addStretch();
    
    stackedParams->addWidget(meanShiftParams);
    
    // === GRABCUT PARAMETERS ===
    grabCutParams = new QWidget();
    QVBoxLayout* grabCutLayout = new QVBoxLayout(grabCutParams);
    
    QHBoxLayout* grabCutIterLayout = new QHBoxLayout();
    QLabel* grabCutLabel = new QLabel("Iterations:");
    grabCutLabel->setStyleSheet("color: #c4b5fd;");
    grabCutIterationsSpin = new QSpinBox();
    grabCutIterationsSpin->setRange(1, 10);
    grabCutIterationsSpin->setValue(5);
    connect(grabCutIterationsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    grabCutIterLayout->addWidget(grabCutLabel);
    grabCutIterLayout->addWidget(grabCutIterationsSpin);
    grabCutIterLayout->addStretch();
    grabCutLayout->addLayout(grabCutIterLayout);
    
    QLabel* grabCutInfo = new QLabel(
        "GrabCut extracts foreground from background.\n"
        "Uses center 80% as foreground region automatically.\n"
        "More iterations = better refinement (slower)."
    );
    grabCutInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    grabCutLayout->addWidget(grabCutInfo);
    grabCutLayout->addStretch();
    
    stackedParams->addWidget(grabCutParams);
    
    // === SLIC SUPERPIXEL PARAMETERS ===
    slicParams = new QWidget();
    QVBoxLayout* slicLayout = new QVBoxLayout(slicParams);
    
    QHBoxLayout* regionsLayout = new QHBoxLayout();
    QLabel* regionsLabel = new QLabel("Number of Superpixels:");
    regionsLabel->setStyleSheet("color: #c4b5fd;");
    slicRegionsSpin = new QSpinBox();
    slicRegionsSpin->setRange(10, 1000);
    slicRegionsSpin->setValue(200);
    slicRegionsSpin->setSingleStep(10);
    connect(slicRegionsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    regionsLayout->addWidget(regionsLabel);
    regionsLayout->addWidget(slicRegionsSpin);
    regionsLayout->addStretch();
    slicLayout->addLayout(regionsLayout);
    
    QHBoxLayout* compactnessLayout = new QHBoxLayout();
    QLabel* compactnessLabel = new QLabel("Compactness:");
    compactnessLabel->setStyleSheet("color: #c4b5fd;");
    slicCompactnessSpin = new QSpinBox();
    slicCompactnessSpin->setRange(1, 100);
    slicCompactnessSpin->setValue(10);
    connect(slicCompactnessSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SegmentationDialog::onParameterChanged);
    compactnessLayout->addWidget(compactnessLabel);
    compactnessLayout->addWidget(slicCompactnessSpin);
    compactnessLayout->addStretch();
    slicLayout->addLayout(compactnessLayout);
    
    QLabel* slicInfo = new QLabel(
        "SLIC creates regular superpixel regions.\n"
        "More superpixels = finer segmentation.\n"
        "Higher compactness = more regular shapes."
    );
    slicInfo->setStyleSheet("color: #a78bfa; font-size: 9pt; padding: 10px;");
    slicLayout->addWidget(slicInfo);
    slicLayout->addStretch();
    
    stackedParams->addWidget(slicParams);
    
    paramsLayout->addWidget(stackedParams);
    mainLayout->addWidget(paramsGroup);
    
    // Store reference for method switching
    connect(methodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            stackedParams, &QStackedWidget::setCurrentIndex);
    
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
    connect(resetButton, &QPushButton::clicked, this, &SegmentationDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);
    
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &SegmentationDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void SegmentationDialog::onMethodChanged(int index) {
    updatePreview();
}

void SegmentationDialog::onParameterChanged() {
    updatePreview();
}

void SegmentationDialog::updatePreview() {
    int method = methodCombo->currentIndex();
    
    try {
        switch (method) {
            case 0: applyWatershed(); break;
            case 1: applyKMeans(); break;
            case 2: applyMeanShift(); break;
            case 3: applyGrabCut(); break;
            case 4: applySuperpixelSLIC(); break;
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

void SegmentationDialog::applyWatershed() {
    cv::Mat gray, binary, dist, markers;
    
    // Convert to grayscale
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = inputImage.clone();
    }
    
    // Threshold - use a fixed value or Otsu
    double thresholdPercent = watershedThresholdSpin->value();
    cv::Mat thresh;
    cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    // Noise removal using morphological opening
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(thresh, binary, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);
    
    // Sure background area
    cv::Mat sure_bg;
    cv::dilate(binary, sure_bg, kernel, cv::Point(-1, -1), 3);
    
    // Finding sure foreground area using distance transform
    cv::distanceTransform(binary, dist, cv::DIST_L2, 5);
    
    // Normalize distance transform
    cv::normalize(dist, dist, 0, 1.0, cv::NORM_MINMAX);
    
    // Threshold to get sure foreground
    double distThreshold = 0.01 + (thresholdPercent / 100.0) * 0.6; // Scale 0.01 to 0.61
    cv::Mat sure_fg;
    cv::threshold(dist, sure_fg, distThreshold, 1.0, cv::THRESH_BINARY);
    sure_fg.convertTo(sure_fg, CV_8U);
    
    // Unknown region
    cv::Mat unknown;
    cv::subtract(sure_bg, sure_fg, unknown);
    
    // Marker labelling
    cv::Mat markers32s;
    cv::connectedComponents(sure_fg, markers32s);
    
    // Add one to all labels so that sure background is not 0, but 1
    markers32s = markers32s + 1;
    
    // Mark the unknown region with 0
    markers32s.setTo(0, unknown == 255);
    
    // Convert input to BGR if needed
    cv::Mat inputCopy = inputImage.clone();
    if (inputCopy.channels() == 1) {
        cv::cvtColor(inputCopy, inputCopy, cv::COLOR_GRAY2BGR);
    }
    
    // Ensure markers and input have compatible sizes
    if (markers32s.size() != inputCopy.size()) {
        cv::resize(markers32s, markers32s, inputCopy.size(), 0, 0, cv::INTER_NEAREST);
    }
    
    // Apply watershed
    cv::watershed(inputCopy, markers32s);
    
    // Create colored output
    previewImage = cv::Mat::zeros(markers32s.size(), CV_8UC3);
    
    // Find unique labels
    double minVal, maxVal;
    cv::minMaxLoc(markers32s, &minVal, &maxVal);
    int numLabels = static_cast<int>(maxVal);
    
    // Generate random colors for each label
    std::vector<cv::Vec3b> colors(numLabels + 1);
    colors[0] = cv::Vec3b(0, 0, 0); // Background
    colors[1] = cv::Vec3b(128, 128, 128); // Sure background
    
    for (int i = 2; i <= numLabels; i++) {
        colors[i] = cv::Vec3b(
            (i * 50) % 256,
            (i * 100) % 256,
            (i * 150) % 256
        );
    }
    
    // Color the regions
    for (int i = 0; i < markers32s.rows; i++) {
        for (int j = 0; j < markers32s.cols; j++) {
            int label = markers32s.at<int>(i, j);
            if (label == -1) {
                // Boundary
                previewImage.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 0);
            } else if (label >= 0 && label <= numLabels) {
                previewImage.at<cv::Vec3b>(i, j) = colors[label];
            }
        }
    }
    
    segmentationType = QString("Watershed (%1 regions)").arg(numLabels);
    infoLabel->setText(QString("Watershed segmentation: %1 regions found (Threshold: %2%)")
        .arg(numLabels).arg(watershedThresholdSpin->value()));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SegmentationDialog::applyKMeans() {
    int k = kmeansClustersSpin->value();
    int iterations = kmeansIterationsSpin->value();
    
    cv::Mat data;
    inputImage.reshape(1, inputImage.rows * inputImage.cols).convertTo(data, CV_32F);
    
    cv::Mat labels, centers;
    cv::kmeans(data, k, labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, iterations, 1.0),
               3, cv::KMEANS_PP_CENTERS, centers);
    
    // Reconstruct image
    centers.convertTo(centers, CV_8U);
    previewImage = cv::Mat::zeros(inputImage.size(), inputImage.type());
    
    for (int i = 0; i < inputImage.rows * inputImage.cols; i++) {
        int clusterIdx = labels.at<int>(i);
        int row = i / inputImage.cols;
        int col = i % inputImage.cols;
        
        if (inputImage.channels() == 1) {
            previewImage.at<uchar>(row, col) = centers.at<uchar>(clusterIdx);
        } else {
            previewImage.at<cv::Vec3b>(row, col) = cv::Vec3b(
                centers.at<uchar>(clusterIdx, 0),
                centers.at<uchar>(clusterIdx, 1),
                centers.at<uchar>(clusterIdx, 2)
            );
        }
    }
    
    segmentationType = QString("K-Means (K=%1)").arg(k);
    infoLabel->setText(QString("K-Means clustering: %1 clusters, %2 iterations").arg(k).arg(iterations));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SegmentationDialog::applyMeanShift() {
    int sp = spatialRadiusSpin->value();
    int sr = colorRadiusSpin->value();
    
    cv::Mat inputCopy = inputImage.clone();
    if (inputCopy.channels() == 1) {
        cv::cvtColor(inputCopy, inputCopy, cv::COLOR_GRAY2BGR);
    }
    
    cv::pyrMeanShiftFiltering(inputCopy, previewImage, sp, sr);
    
    segmentationType = QString("Mean Shift (sp=%1,sr=%2)").arg(sp).arg(sr);
    infoLabel->setText(QString("Mean Shift: Spatial=%1, Color=%2").arg(sp).arg(sr));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SegmentationDialog::applyGrabCut() {
    int iterations = grabCutIterationsSpin->value();
    
    cv::Mat inputCopy = inputImage.clone();
    if (inputCopy.channels() == 1) {
        cv::cvtColor(inputCopy, inputCopy, cv::COLOR_GRAY2BGR);
    }
    
    // Define ROI (center 80% of image)
    int margin = static_cast<int>(inputCopy.cols * 0.1);
    cv::Rect rect(margin, margin, 
                  inputCopy.cols - 2 * margin, 
                  inputCopy.rows - 2 * margin);
    
    cv::Mat mask, bgModel, fgModel;
    cv::grabCut(inputCopy, mask, rect, bgModel, fgModel, iterations, cv::GC_INIT_WITH_RECT);
    
    // Create binary mask
    cv::Mat binMask = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);
    binMask.convertTo(binMask, CV_8U, 255);
    
    // Apply mask
    previewImage = cv::Mat::zeros(inputCopy.size(), inputCopy.type());
    inputCopy.copyTo(previewImage, binMask);
    
    segmentationType = QString("GrabCut (iter=%1)").arg(iterations);
    infoLabel->setText(QString("GrabCut foreground extraction: %1 iterations").arg(iterations));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
}

void SegmentationDialog::applySuperpixelSLIC() {
#ifdef HAVE_OPENCV_XIMGPROC
    int regions = slicRegionsSpin->value();
    int compactness = slicCompactnessSpin->value();
    
    cv::Mat inputCopy = inputImage.clone();
    if (inputCopy.channels() == 1) {
        cv::cvtColor(inputCopy, inputCopy, cv::COLOR_GRAY2BGR);
    }
    
    cv::Ptr<cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(
        inputCopy, cv::ximgproc::SLIC, regions, compactness
    );
    slic->iterate(10);
    
    cv::Mat labels;
    slic->getLabels(labels);
    
    // Color each superpixel with average color
    int numLabels = slic->getNumberOfSuperpixels();
    std::vector<cv::Vec3d> sums(numLabels, cv::Vec3d(0, 0, 0));
    std::vector<int> counts(numLabels, 0);
    
    for (int i = 0; i < labels.rows; i++) {
        for (int j = 0; j < labels.cols; j++) {
            int label = labels.at<int>(i, j);
            cv::Vec3b color = inputCopy.at<cv::Vec3b>(i, j);
            sums[label] += cv::Vec3d(color[0], color[1], color[2]);
            counts[label]++;
        }
    }
    
    previewImage = cv::Mat::zeros(inputCopy.size(), CV_8UC3);
    for (int i = 0; i < labels.rows; i++) {
        for (int j = 0; j < labels.cols; j++) {
            int label = labels.at<int>(i, j);
            cv::Vec3d avg = sums[label] / counts[label];
            previewImage.at<cv::Vec3b>(i, j) = cv::Vec3b(
                static_cast<uchar>(avg[0]),
                static_cast<uchar>(avg[1]),
                static_cast<uchar>(avg[2])
            );
        }
    }
    
    // Draw contours
    cv::Mat mask;
    slic->getLabelContourMask(mask, true);
    previewImage.setTo(cv::Scalar(0, 255, 0), mask);
    
    segmentationType = QString("SLIC (regions=%1)").arg(numLabels);
    infoLabel->setText(QString("SLIC Superpixels: %1 regions created").arg(numLabels));
    infoLabel->setStyleSheet("color: #a78bfa; padding: 5px;");
#else
    // Fallback: Use simple grid-based superpixels
    int regions = slicRegionsSpin->value();
    
    cv::Mat inputCopy = inputImage.clone();
    if (inputCopy.channels() == 1) {
        cv::cvtColor(inputCopy, inputCopy, cv::COLOR_GRAY2BGR);
    }
    
    // Calculate grid size
    int gridSize = static_cast<int>(std::sqrt(inputCopy.rows * inputCopy.cols / static_cast<double>(regions)));
    if (gridSize < 1) gridSize = 1;
    
    previewImage = inputCopy.clone();
    
    // Draw grid lines
    for (int y = 0; y < inputCopy.rows; y += gridSize) {
        cv::line(previewImage, cv::Point(0, y), cv::Point(inputCopy.cols, y), cv::Scalar(0, 255, 0), 1);
    }
    for (int x = 0; x < inputCopy.cols; x += gridSize) {
        cv::line(previewImage, cv::Point(x, 0), cv::Point(x, inputCopy.rows), cv::Scalar(0, 255, 0), 1);
    }
    
    // Average colors in each grid cell
    for (int y = 0; y < inputCopy.rows; y += gridSize) {
        for (int x = 0; x < inputCopy.cols; x += gridSize) {
            int endY = std::min(y + gridSize, inputCopy.rows);
            int endX = std::min(x + gridSize, inputCopy.cols);
            
            cv::Rect roi(x, y, endX - x, endY - y);
            cv::Scalar avgColor = cv::mean(inputCopy(roi));
            previewImage(roi).setTo(avgColor);
        }
    }
    
    // Redraw grid
    for (int y = 0; y < inputCopy.rows; y += gridSize) {
        cv::line(previewImage, cv::Point(0, y), cv::Point(inputCopy.cols, y), cv::Scalar(0, 255, 0), 1);
    }
    for (int x = 0; x < inputCopy.cols; x += gridSize) {
        cv::line(previewImage, cv::Point(x, 0), cv::Point(x, inputCopy.rows), cv::Scalar(0, 255, 0), 1);
    }
    
    int actualRegions = (inputCopy.rows / gridSize + 1) * (inputCopy.cols / gridSize + 1);
    segmentationType = QString("Grid Superpixels (regions?%1)").arg(actualRegions);
    infoLabel->setText(QString("Grid-based superpixels (OpenCV ximgproc not available): %1 regions").arg(actualRegions));
    infoLabel->setStyleSheet("color: #fbbf24; padding: 5px;");
#endif
}

void SegmentationDialog::onApplyClicked() {
    if (!previewImage.empty()) {
        segmentedImage = previewImage.clone();
        applied = true;
        accept();
    } else {
        QMessageBox::warning(this, "No Preview", "No segmentation result to apply!");
    }
}

void SegmentationDialog::onResetClicked() {
    // Reset all parameters to default
    watershedThresholdSpin->setValue(20);
    kmeansClustersSpin->setValue(3);
    kmeansIterationsSpin->setValue(100);
    spatialRadiusSpin->setValue(20);
    colorRadiusSpin->setValue(40);
    grabCutIterationsSpin->setValue(5);
    slicRegionsSpin->setValue(200);
    slicCompactnessSpin->setValue(10);
    
    updatePreview();
}
#include "moc_SegmentationDialog.cpp"
