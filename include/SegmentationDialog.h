#ifndef SEGMENTATIONDIALOG_H
#define SEGMENTATIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include "ImageCanvas.h"

/**
 * @brief Dialog for advanced region-based segmentation techniques
 * 
 * Phase 17: Advanced Segmentation
 * - Watershed Segmentation
 * - K-Means Clustering
 * - Mean Shift Segmentation
 * - GrabCut (interactive foreground/background)
 */
class SegmentationDialog : public QDialog {
    Q_OBJECT

public:
    explicit SegmentationDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~SegmentationDialog();

    cv::Mat getSegmentedImage() const { return segmentedImage; }
    QString getSegmentationType() const { return segmentationType; }
    bool wasApplied() const { return applied; }

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
    
    // Segmentation methods
    void applyWatershed();
    void applyKMeans();
    void applyMeanShift();
    void applyGrabCut();
    void applySuperpixelSLIC();

    cv::Mat inputImage;
    cv::Mat segmentedImage;
    cv::Mat previewImage;
    
    QString segmentationType;
    bool applied;

    // UI Components
    QComboBox* methodCombo;
    ImageCanvas* previewCanvas;
    
    // Watershed parameters
    QSpinBox* watershedThresholdSpin;
    
    // K-Means parameters
    QSpinBox* kmeansClustersSpin;
    QSpinBox* kmeansIterationsSpin;
    
    // Mean Shift parameters
    QSpinBox* spatialRadiusSpin;
    QSpinBox* colorRadiusSpin;
    
    // GrabCut parameters
    QSpinBox* grabCutIterationsSpin;
    
    // SLIC Superpixel parameters
    QSpinBox* slicRegionsSpin;
    QSpinBox* slicCompactnessSpin;
    
    // Parameter containers
    QWidget* watershedParams;
    QWidget* kmeansParams;
    QWidget* meanShiftParams;
    QWidget* grabCutParams;
    QWidget* slicParams;
    
    QPushButton* applyButton;
    QPushButton* resetButton;
    
    QLabel* infoLabel;
};

#endif // SEGMENTATIONDIALOG_H
