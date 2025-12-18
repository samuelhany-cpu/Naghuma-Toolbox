#ifndef THRESHOLDINGDIALOG_H
#define THRESHOLDINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <opencv2/opencv.hpp>

class ImageCanvas;

/**
 * @brief Dialog for interactive thresholding operations with live preview
 * 
 * Supports multiple thresholding methods:
 * - Simple Binary Thresholding
 * - Adaptive Thresholding (Mean & Gaussian)
 * - Otsu's Method
 * - Multi-level Otsu
 * - Local Thresholding
 */
class ThresholdingDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ThresholdingDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~ThresholdingDialog();
    
    // Getters
    cv::Mat getThresholdedImage() const { return thresholdedImage; }
    QString getThresholdingType() const;
    bool wasApplied() const { return applied; }
    
signals:
    void previewUpdated(const cv::Mat& preview);
    
private slots:
    void onThresholdTypeChanged(int index);
    void onThresholdValueChanged(int value);
    void onMaxValueChanged(int value);
    void onAdaptiveMethodChanged(int index);
    void onAdaptiveTypeChanged(int index);
    void onBlockSizeChanged(int value);
    void onCConstantChanged(int value);
    void onOtsuLevelsChanged(int value);
    void onApplyClicked();
    void onCancelClicked();
    void updatePreview();
    
private:
    // UI Components
    QComboBox *thresholdTypeCombo;
    QComboBox *adaptiveMethodCombo;
    QComboBox *adaptiveTypeCombo;
    
    QSpinBox *thresholdValueSpinBox;
    QSlider *thresholdValueSlider;
    
    QSpinBox *maxValueSpinBox;
    QSlider *maxValueSlider;
    
    QSpinBox *blockSizeSpinBox;
    QSlider *blockSizeSlider;
    
    QSpinBox *cConstantSpinBox;
    QSlider *cConstantSlider;
    
    QSpinBox *otsuLevelsSpinBox;
    
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QLabel *infoLabel;
    QLabel *thresholdInfoLabel;
    
    // Parameter group boxes
    QGroupBox *simpleThresholdGroup;
    QGroupBox *adaptiveThresholdGroup;
    QGroupBox *otsuGroup;
    QGroupBox *multiOtsuGroup;
    
    // Image canvases
    ImageCanvas *originalCanvas;
    ImageCanvas *thresholdedCanvas;
    
    // Image data
    cv::Mat originalImage;
    cv::Mat thresholdedImage;
    bool applied;
    
    // Current parameters
    int thresholdValue;
    int maxValue;
    int blockSize;
    int cConstant;
    int otsuLevels;
    double computedThreshold;
    
    // Helper methods
    void setupUI();
    void showParametersForThresholdType(int typeIndex);
    void applySimpleThreshold();
    void applyAdaptiveThreshold();
    void applyOtsuThreshold();
    void applyMultiLevelOtsu();
    void applyLocalThreshold();
};

#endif // THRESHOLDINGDIALOG_H
