#ifndef FEATUREDETECTIONDIALOG_H
#define FEATUREDETECTIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <opencv2/opencv.hpp>
#include "ImageCanvas.h"

/**
 * @brief Dialog for feature detection (corners, keypoints)
 * 
 * Phase 19: Feature Extraction
 * - Harris Corner Detection
 * - Shi-Tomasi (Good Features to Track)
 * - FAST Corner Detection
 * - ORB Feature Detection
 */
class FeatureDetectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit FeatureDetectionDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~FeatureDetectionDialog();

    cv::Mat getDetectedImage() const { return detectedImage; }
    QString getDetectionType() const { return detectionType; }
    bool wasApplied() const { return applied; }
    int getFeatureCount() const { return featureCount; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onMethodChanged(int index);
    void onParameterChanged();
    void onApplyClicked();
    void onResetClicked();

private:
    void setupUI();
    void updatePreview();
    
    // Detection methods
    void detectHarrisCorners();
    void detectShiTomasiCorners();
    void detectFASTCorners();
    void detectORBFeatures();

    cv::Mat inputImage;
    cv::Mat detectedImage;
    cv::Mat previewImage;
    
    QString detectionType;
    bool applied;
    int featureCount;

    // UI Components
    QComboBox* methodCombo;
    ImageCanvas* previewCanvas;
    
    // Harris parameters
    QSpinBox* harrisBlockSizeSpin;
    QSpinBox* harrisApertureSpin;
    QSlider* harrisKSlider;
    QLabel* harrisKLabel;
    QSlider* harrisThresholdSlider;
    QLabel* harrisThresholdLabel;
    
    // Shi-Tomasi parameters
    QSpinBox* shiTomasiMaxCornersSpin;
    QSlider* shiTomasiQualitySlider;
    QLabel* shiTomasiQualityLabel;
    QSpinBox* shiTomasiMinDistanceSpin;
    QSpinBox* shiTomasiBlockSizeSpin;
    
    // FAST parameters
    QSpinBox* fastThresholdSpin;
    QCheckBox* fastNonMaxSuppressionCheck;
    QComboBox* fastTypeCombo;
    
    // ORB parameters
    QSpinBox* orbMaxFeaturesSpin;
    QSlider* orbScaleFactorSlider;
    QLabel* orbScaleFactorLabel;
    QSpinBox* orbLevelsSpin;
    QSpinBox* orbEdgeThresholdSpin;
    
    // Parameter containers
    QWidget* harrisParams;
    QWidget* shiTomasiParams;
    QWidget* fastParams;
    QWidget* orbParams;
    
    QPushButton* applyButton;
    QPushButton* resetButton;
    
    QLabel* infoLabel;
    QLabel* featureCountLabel;
};

#endif // FEATUREDETECTIONDIALOG_H
