#pragma once

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <opencv2/opencv.hpp>
#include "WaveletTransform.h"

class ImageCanvas;

/**
 * @brief Dialog for wavelet transform and denoising with live preview
 */
class WaveletDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit WaveletDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~WaveletDialog();
    
    // Getters
    cv::Mat getProcessedImage() const { return processedImage; }
    QString getOperationType() const { return operationType; }
    bool wasApplied() const { return applied; }
    
signals:
    void previewUpdated(const cv::Mat& preview);
    
private slots:
    void onOperationChanged(int index);
    void onWaveletTypeChanged(int index);
    void onThresholdMethodChanged(int index);
    void onParameterChanged();
    void onApplyClicked();
    void onResetClicked();
    
private:
    // UI Components
    QComboBox *operationCombo;
    QComboBox *waveletTypeCombo;
    QComboBox *thresholdMethodCombo;
    
    QSlider *thresholdSlider;
    QLabel *thresholdLabel;
    
    QSpinBox *levelsSpinBox;
    
    QPushButton *applyButton;
    QPushButton *resetButton;
    QLabel *infoLabel;
    
    QWidget *denoiseParams;
    QWidget *decompositionParams;
    
    // Image canvases
    ImageCanvas *originalCanvas;
    ImageCanvas *processedCanvas;
    ImageCanvas *decompositionCanvas;
    
    // Image data
    cv::Mat originalImage;
    cv::Mat processedImage;
    cv::Mat decompositionVisualization;
    QString operationType;
    bool applied;
    
    // Helper methods
    void setupUI();
    void updatePreview();
    void performDenoising();
    void performDecomposition();
    void performReconstruction();
};
